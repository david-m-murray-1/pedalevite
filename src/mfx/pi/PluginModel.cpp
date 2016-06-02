/*****************************************************************************

        PluginModel.cpp
        Author: Laurent de Soras, 2016

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

#include "mfx/pi/PluginModel.h"

#include <cassert>



namespace mfx
{
namespace pi
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



std::string	PluginModel_get_name (PluginModel model)
{
	assert (model >= 0);
	assert (model < PluginModel_NBR_ELT);

	switch (model)
	{
	case PluginModel_DRYWET:         return ("Dry/Wet mixer\nDry/Wet");
	case PluginModel_TUNER:          return ("Tuner");
	case PluginModel_DISTO_SIMPLE:   return ("Simple distortion\nDisto S");
	case PluginModel_TREMOLO:        return ("Tremolo");
	case PluginModel_WHA:            return ("Wha-wha");
	case PluginModel_FREQ_SHIFT:     return ("Frequency Shifter\nFreq Shift\nFShift");
	default:
		assert (false);
	}

	return "Anonymous effect";
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace pi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
