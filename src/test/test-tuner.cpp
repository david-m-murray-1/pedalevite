
// g++ --std=c++11 -I. -Wall `pkg-config --cflags --libs jack` test-tuner.cpp FreqAnalyser.cpp
// jackd -P70 -p16 -t2000 -dalsa -p64 -n3 -r44100 -s &
// ./a.out


#if defined (WIN32) || defined (_WIN32) || defined (__CYGWIN__)
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
#endif

#include "tuner/FreqAnalyser.h"

#include <jack/jack.h>

#if defined (WIN32) || defined (_WIN32) || defined (__CYGWIN__)
	#include <Windows.h> // For Sleep()
#else
	#include <unistd.h>  // For sleep()
#endif
#include <signal.h>

#include <algorithm>
#include <atomic>

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>



static ::jack_client_t * volatile MAIN_client_ptr = 0;



static void MAIN_signal_handler (int sig)
{
	::jack_client_close (MAIN_client_ptr);
	fprintf (stderr, "Signal %d received, exiting ...\n", sig);
	exit (0);
}



static int64_t MAIN_get_time ()
{
	timespec       tp;
	clock_gettime (CLOCK_REALTIME, &tp);

	const long     ns_mul = 1000L * 1000L * 1000L;
	return int64_t (tp.tv_sec) * ns_mul + tp.tv_nsec;
}



class Context
{
public:
	::jack_client_t *
	               _client_ptr;
	::jack_port_t *               // [in/out] [chn]
	               _mfx_port_arr [2] [2];

	int64_t        _time_beg = MAIN_get_time ();
	int64_t        _time_end = _time_beg;
	std::atomic <double>          // Negative: the main thread read the value.
	               _usage;
	tuner::FreqAnalyser
	               _freq_analyser;
	std::atomic <double>
	               _detected_freq;
	const int      _tuner_subspl = 4;
};



static void MAIN_jack_shutdown (void *arg)
{
	exit (0);
}



static int MAIN_process (::jack_nframes_t nbr_spl, void *arg)
{
	const int64_t  time_beg    = MAIN_get_time ();
	Context &      ctx         = *reinterpret_cast <Context *> (arg);
	const int64_t  dur_tot     =      time_beg - ctx._time_beg;
	const int64_t  dur_act     = ctx._time_end - ctx._time_beg;
	const double   usage_frame = double (dur_act) / double (dur_tot);
	double         usage       = ctx._usage.load ();
	usage = std::max (usage, usage_frame);
	ctx._usage.store (usage);
	ctx._time_beg = time_beg;

	const jack_default_audio_sample_t * src_arr [2];
	jack_default_audio_sample_t *       dst_arr [2];
	for (int chn = 0; chn < 2; ++chn)
	{
		src_arr [chn] = reinterpret_cast <const jack_default_audio_sample_t *> (
			::jack_port_get_buffer (ctx._mfx_port_arr [0] [chn], nbr_spl)
		);
		dst_arr [chn] = reinterpret_cast <jack_default_audio_sample_t *> (
			::jack_port_get_buffer (ctx._mfx_port_arr [1] [chn], nbr_spl)
		);
	}

	for (int chn = 0; chn < 2; ++chn)
	{
		for (int pos = 0; pos < int (nbr_spl); ++pos)
		{
			float          val = src_arr [chn] [pos];
			dst_arr [chn] [pos] = val;
		}
	}

	double          freq = 0;
	if (ctx._tuner_subspl == 1)
	{
		freq = ctx._freq_analyser.process_block (&src_arr [0] [0], nbr_spl);
	}
	else
	{
		const int       buf_size = 1024;
		std::array <float, buf_size>   buf;
		int             work_pos = 0;
		while (work_pos < nbr_spl)
		{
			const int       work_len =
				std::min (nbr_spl - work_pos, buf_size * ctx._tuner_subspl);
			const int       len_ss = work_len / ctx._tuner_subspl;
			for (int i = 0; i < len_ss; ++i)
			{
				float            sum  = 0;
				const int        base = work_pos + i * ctx._tuner_subspl;
				for (int j = 0; j < ctx._tuner_subspl; ++j)
				{
					sum += src_arr [0] [base + j];
				}
				buf [i] = sum;
			}
			freq = ctx._freq_analyser.process_block (&buf [0], len_ss);
			
			work_pos += work_len;
		}
	}
	ctx._detected_freq.store (freq);

	ctx._time_end = MAIN_get_time ();

	return 0;
}


int main (int argc, char *argv [])
{
	int            ret_val = 0;

	::jack_status_t   status = ::JackServerFailed;
	MAIN_client_ptr = jack_client_open (
		"MultiFX",
		::JackNullOption,
		&status,
		0
	);
	::jack_client_t * client_ptr = MAIN_client_ptr;
	if (client_ptr == 0)
	{
		fprintf (
			stderr,
			"jack_client_open() failed, status = 0x%2.0x\n",
			status
		);
		if ((status & ::JackServerFailed) != 0)
		{
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		ret_val = -1;
	}

	Context        ctx;
	ctx._client_ptr = client_ptr;
	ctx._usage.store (0);
	ctx._detected_freq.store (0);
	const double   sample_freq = double (jack_get_sample_rate (client_ptr));
	ctx._freq_analyser.set_sample_freq (sample_freq / ctx._tuner_subspl);

	static const ::JackPortFlags port_dir [2] =
	{
		::JackPortIsInput, ::JackPortIsOutput
	};
	static const char *  ext_port_dir_name_0_arr [2] =
	{
		"capture", "playback"
	};

	if (ret_val == 0)
	{
		if ((status & ::JackServerStarted) != 0)
		{
			fprintf (stderr, "JACK server started\n");
		}
		if ((status & ::JackNameNotUnique) != 0)
		{
			const char *   name_0 = ::jack_get_client_name (client_ptr);
			fprintf (stderr, "Unique name \"%s\" assigned\n", name_0);
		}

		::jack_set_process_callback (client_ptr, MAIN_process, &ctx);
		::jack_on_shutdown (client_ptr, MAIN_jack_shutdown, &ctx);

		static const char *  port_name_0_arr [2] [2] =
		{
			{ "Input L", "Input R" }, { "Output L", "Output R" }
		};
		for (int dir = 0; dir < 2 && ret_val == 0; ++dir)
		{
			for (int chn = 0; chn < 2 && ret_val == 0; ++chn)
			{
				ctx._mfx_port_arr [dir] [chn] = ::jack_port_register (
					client_ptr,
					port_name_0_arr [dir] [chn],
					JACK_DEFAULT_AUDIO_TYPE,
					port_dir [dir],
					0
				);
				if (ctx._mfx_port_arr [dir] [chn] == 0)
				{
					fprintf (stderr, "No more JACK ports available.\n");
					ret_val = -1;
				}
			}
		}
	}

	if (ret_val == 0)
	{
		ret_val = ::jack_activate (client_ptr);
		if (ret_val != 0)
		{
			fprintf (stderr, "cannot activate client, returned %d.\n", ret_val);
		}
	}

	for (int dir = 0; dir < 2 && ret_val == 0; ++dir)
	{
		const char **  port_0_arr = ::jack_get_ports (
			client_ptr,
			0,
			0,
			::JackPortIsPhysical | port_dir [1 - dir]
		);
		if (port_0_arr == 0)
		{
			fprintf (
				stderr,
				"No physical %s port available.\n",
				ext_port_dir_name_0_arr [dir]
			);
			ret_val = -1;
		}
		else
		{
			fprintf (stderr, "Available ports for %s:\n", ext_port_dir_name_0_arr [dir]);
			for (int index = 0; port_0_arr [index] != 0; ++index)
			{
				fprintf (stderr, "%d: %s\n", index, port_0_arr [index]);
			}

			for (int chn = 0; chn < 2 && ret_val == 0; ++chn)
			{
				if (port_0_arr [chn] == 0)
				{
					fprintf (
						stderr,
						"Not enough physical %s port available.\n",
						ext_port_dir_name_0_arr [dir]
					);
					ret_val = -1;
				}
				else
				{
					const char * inout_0 [2];
					inout_0 [1 - dir] = ::jack_port_name (ctx._mfx_port_arr [dir] [chn]);
					inout_0 [    dir] = port_0_arr [chn];
					ret_val = ::jack_connect (client_ptr, inout_0 [0], inout_0 [1]);
					if (ret_val != 0)
					{
						fprintf (
							stderr,
							"Cannot connect to %s port %d.\n",
							ext_port_dir_name_0_arr [dir],
							chn
						);
					}
				}
			}
		}

		if (port_0_arr != 0)
		{
			::jack_free (port_0_arr);
		}
	}

	if (ret_val == 0)
	{
		signal (SIGINT,  MAIN_signal_handler);
		signal (SIGTERM, MAIN_signal_handler);
#if defined (WIN32) || defined (_WIN32) || defined (__CYGWIN__)
		signal (SIGABRT, MAIN_signal_handler);
#else
		signal (SIGQUIT, MAIN_signal_handler);
		signal (SIGHUP,  MAIN_signal_handler);
#endif

		fprintf (stderr, "Now running...\n");
	}

	while (ret_val == 0)
	{
#if defined (WIN32) || defined (_WIN32) || defined (__CYGWIN__)
		::Sleep (1000);
#else
		sleep (1);
#endif
		const double   usage = ctx._usage.exchange (-1);
		const double   freq  = ctx._detected_freq.load ();
		char           cpu_0 [127+1] = "";
		if (usage >= 0)
		{
			sprintf (cpu_0, "CPU usage: %6.2f %%", usage * 100);
		}
		char           freq_0 [127+1] = "";
		if (freq > 0)
		{
			sprintf (freq_0, "Freq: %7.2f Hz", freq);
		}

		fprintf (stderr, "%s %s\r", cpu_0, freq_0);
		fflush (stderr);
	}

	if (client_ptr != 0)
	{
		::jack_client_close (client_ptr);
		MAIN_client_ptr = 0;
	}

	return ret_val;
}

