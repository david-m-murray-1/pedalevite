/*****************************************************************************

        Param.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_pi_envf_Param_HEADER_INCLUDED)
#define mfx_pi_envf_Param_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



namespace mfx
{
namespace pi
{
namespace envf
{



enum Param
{

	Param_ATTACK = 0,
	Param_RELEASE,
	Param_HOLD,
	Param_GAIN,
	Param_THRESH,
	Param_MODE,
	Param_CLIP_E,
	Param_CLIP_S,
	Param_LC_FREQ,

	Param_NBR_ELT

}; // enum Param



}  // namespace envf
}  // namespace pi
}  // namespace mfx



//#include "mfx/pi/envf/Param.hpp"



#endif   // mfx_pi_envf_Param_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
