/*****************************************************************************

        DistoDspRandWalk.hpp
        Author: Laurent de Soras, 2018

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (mfx_pi_dist2_DistoDspRandWalk_CODEHEADER_INCLUDED)
#define mfx_pi_dist2_DistoDspRandWalk_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/Approx.h"
#include "fstb/fnc.h"

#include <cassert>



namespace mfx
{
namespace pi
{
namespace dist2
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



float	DistoDspRandWalk::process_sample (float x)
{
	_sum += x;
	_val += _inc;
	++ _pos;

	if (_pos >= _nbr_spl)
	{
		const float    val_end = _sum / _nbr_spl;
		const float    rnd_val = _rnd.process_sample ();
		_nbr_spl = fstb::ceil_int (_avg_per * fstb::Approx::exp2 (rnd_val));
		assert (_nbr_spl > 0);
		_inc = (val_end - _val) / _nbr_spl;
		_pos = 0;
		_sum = 0;
	}

	return (_val);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace dist2
}  // namespace pi
}  // namespace mfx



#endif   // mfx_pi_dist2_DistoDspRandWalk_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
