/*****************************************************************************

        EnvFollowerPeak.cpp
        Author: Laurent de Soras, 2018

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if defined (_MSC_VER)
	#pragma warning (1 : 4130 4223 4705 4706)
	#pragma warning (4 : 4355 4786 4800)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/fnc.h"
#include "mfx/dsp/dyn/EnvFollowerPeak.h"
#include "mfx/dsp/dyn/EnvHelper.h"

#include <cassert>
#include <cmath>



namespace mfx
{
namespace dsp
{
namespace dyn
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



EnvFollowerPeak::EnvFollowerPeak ()
:	_sample_freq (44100)
,	_time_a (0)
,	_time_r (0)
,	_state (0)
,	_coef_a (1)
,	_coef_r (1)
{
	set_times (0.0001f, 0.100f);
	clear_buffers ();
}



void	EnvFollowerPeak::set_sample_freq (double fs)
{
	assert (fs > 0);

	_sample_freq = float (fs);
	update_parameters ();
}



void	EnvFollowerPeak::set_times (float at, float rt)
{
	assert (at >= 0);
	assert (rt >= 0);

	_time_a = at;
	_time_r = rt;
	update_parameters ();
}



void	EnvFollowerPeak::set_time_atk (float t)
{
	assert (t >= 0);

	_time_a = t;
	update_parameters ();
}



void	EnvFollowerPeak::set_time_rel (float t)
{
	assert (t >= 0);

	_time_r = t;
	update_parameters ();
}



void	EnvFollowerPeak::process_block (float out_ptr [], const float in_ptr [], int nbr_spl)
{
	assert (out_ptr != 0);
	assert (in_ptr != 0);
	assert (nbr_spl > 0);

	float          state = _state;

	int            pos = 0;
	do
	{
		const float    x = in_ptr [pos];
		process_sample_internal (state, x);
		out_ptr [pos] = state;

		++ pos;
	}
	while (pos < nbr_spl);

	_state = state;
}



float	EnvFollowerPeak::analyse_block (const float data_ptr [], int nbr_spl)
{
	assert (data_ptr != 0);
	assert (nbr_spl > 0);

	float          state = _state;

	int            pos = 0;
	do
	{
		const float    x = data_ptr [pos];
		process_sample_internal (state, x);

		++ pos;
	}
	while (pos < nbr_spl);

	_state = state;

	return state;
}



float	EnvFollowerPeak::analyse_block_cst (float x, int nbr_spl)
{
	assert (nbr_spl > 0);

	const float    xa    = fabs (x);
	const float    delta = xa - _state;
	const float    coef  = (delta >= 0) ? _coef_a : _coef_r;

	float          coef_block;
	if (nbr_spl < 100000)
	{
		coef_block = 1 - fstb::ipowp (1 - coef, nbr_spl);
	}
	else
	{
		coef_block = float (1 - pow (1 - coef, nbr_spl));
	}

	_state += delta * coef_block;

	return _state;
}



void	EnvFollowerPeak::clear_buffers ()
{
	_state = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	EnvFollowerPeak::update_parameters ()
{
	_coef_a = float (
		EnvHelper::compute_env_coef_simple (_time_a, _sample_freq)
	);
	_coef_r = float (
		EnvHelper::compute_env_coef_simple (_time_r, _sample_freq)
	);
}



}  // namespace dyn
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
