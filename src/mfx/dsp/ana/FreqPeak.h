/*****************************************************************************

        FreqPeak.h
        Author: Laurent de Soras, 2019

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_ana_FreqPeak_HEADER_INCLUDED)
#define mfx_dsp_ana_FreqPeak_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/ana/ValSmooth.h"
#include "mfx/dsp/dyn/EnvFollowerPeak.h"
#include "mfx/dsp/iir/OnePole.h"
#include "mfx/dsp/iir/Biquad.h"

#include <array>



namespace mfx
{
namespace dsp
{
namespace ana
{



class FreqPeak
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	               FreqPeak ()  = default;
	virtual        ~FreqPeak () = default;

	void           set_sample_freq (double sample_freq);
	void           set_freq_bot (float f);
	void           set_freq_top (float f);
	void           set_smoothing (float responsiveness, float thr);
	void           set_threshold (float thr);
	void           clear_buffers ();
	float          process_block (const float spl_ptr [], int nbr_spl);
	float          process_sample (float x);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	static const int  _freq_buf_len  = 8;  // Must be a power of 2
	static const int  _freq_buf_mask = _freq_buf_len - 1;

	class Polarity
	{
	public:
		dyn::EnvFollowerPeak
		               _env;
		std::array <float, 3>   // Last input values. 0 = oldest
		               _mem_x;
		std::array <float, 3>   // Last envelope values. 0 = oldest
		               _mem_y;
		float          _peak_age;  // Last peak age in samples. Large value: no peak found
	};

	void           update_input_filter ();

	static inline float
	               compute_median3 (float a0, float a1, float a2);
	static inline float
	               compute_median5 (float a0, float a1, float a2, float a3, float a4);
	static inline void
	               sort_pair (float &a, float &b);

	float          _sample_freq = 0;    // Sampling rate, Hz, > 0. 0 = not set
	float          _inv_fs      = 0;
	ValSmooth <float, 0>                // Hz. 0 = not found (yet)
	               _freq_smooth;
	float          _freq_bot    = 20.0f;// Hz, > 0
	float          _freq_top    = 1500; // Hz, > _freq_bot
	float          _threshold   = 0;
	iir::OnePole   _hpf_in;
	iir::Biquad    _lpf_in;
	std::array <Polarity, 2>   // Positive, negative
	               _pol_arr;
	std::array <float, _freq_buf_len>   // Raw frequency values, before median and smoothing
	               _freq_raw_arr;       // Must contain valid frequencies
	int            _freq_buf_pos = 0;   // Current writing position
	int            _polarity    = 0;    // Current polarity: 0 = positive, 1 = negative



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               FreqPeak (const FreqPeak &other)          = delete;
	FreqPeak &     operator = (const FreqPeak &other)        = delete;
	bool           operator == (const FreqPeak &other) const = delete;
	bool           operator != (const FreqPeak &other) const = delete;

}; // class FreqPeak



}  // namespace ana
}  // namespace dsp
}  // namespace mfx



//#include "mfx/dsp/ana/FreqPeak.hpp"



#endif   // mfx_dsp_ana_FreqPeak_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
