/*****************************************************************************

        Ramp.hpp
        Author: Laurent de Soras, 2017

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (mfx_dsp_ctrl_Ramp_CODEHEADER_INCLUDED)
#define mfx_dsp_ctrl_Ramp_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <cassert>



namespace mfx
{
namespace dsp
{
namespace ctrl
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



Ramp::Ramp (float x)
{
	_val_beg = x;
	_val_end = x;
	_val_tgt = x;
}



void	Ramp::set_time (int nbr_spl, float time_step)
{
	assert (nbr_spl > 0);
	assert (time_step > 0);

	if (nbr_spl != _ramp_time)
	{
		_ramp_time = nbr_spl;
		_time_step = time_step;
		if (_ramp_pos > 0 && _step != 0)
		{
			set_val (_val_tgt);
		}
	}
}



void	Ramp::set_val (float x)
{
	_val_tgt  = x;
	_step     = (x - _val_end) * _time_step;
	_ramp_pos = _ramp_time;
}



void	Ramp::clear_buffers ()
{
	_val_beg  = _val_tgt;
	_val_end  = _val_tgt;
	_step     = 0;
	_ramp_pos = 0;
}



void	Ramp::tick (int nbr_spl)
{
	assert (nbr_spl > 0);

	_val_beg   = _val_end;
	if (_ramp_pos > 0)
	{
		_val_end  += _step * nbr_spl;
		_ramp_pos -= nbr_spl;
		if (_ramp_pos <= 0)
		{
			_ramp_pos = 0;
			_step     = 0;
			_val_end  = _val_tgt;
		}
	}
}



float	Ramp::get_beg () const
{
	return _val_beg;
}



float	Ramp::get_end () const
{
	return _val_end;
}



float	Ramp::get_tgt () const
{
	return _val_tgt;
}



bool	Ramp::is_ramping () const
{
	return (_ramp_pos > 0);
}



int	Ramp::get_rem_len () const
{
	return _ramp_pos;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace ctrl
}  // namespace dsp
}  // namespace mfx



#endif   // mfx_dsp_ctrl_Ramp_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
