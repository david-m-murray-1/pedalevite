/*****************************************************************************

        AllPass1p.cpp
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

#include "mfx/dsp/iir/AllPass1p.h"

#include <cassert>



namespace mfx
{
namespace dsp
{
namespace iir
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*
==============================================================================
Name: process_block
Description:
	Filters a block of samples. Can work in-place.
Input parameters:
	- in_ptr: Input sample block
	- nbr_spl: Number of samples to process. >= 0
Output parameters:
	- out_ptr: Output sample block
Throws: Nothing
==============================================================================
*/

void	AllPass1p::process_block (float dst_ptr [], const float src_ptr [], int nbr_spl)
{
	assert (dst_ptr != nullptr);
	assert (src_ptr != nullptr);
	assert (nbr_spl > 0);

	float          mem_x = _mem_x;
	float          y     = _mem_y;
	int            pos   = 0;
	do
	{
		// First sample
		float          x = src_ptr [pos];
		y = _eq_z_b0 * (x - y) + mem_x;
		dst_ptr [pos] = y;
		mem_x         = x;
		++ pos;
	}
	while (pos < nbr_spl);
	_mem_y = y;
	_mem_x = mem_x;
}



void	AllPass1p::process_block (float dst_ptr [], const float src_ptr [], int nbr_spl, float inc_b0)
{
	assert (dst_ptr != nullptr);
	assert (src_ptr != nullptr);
	assert (nbr_spl > 0);

	float          mem_x = _mem_x;
	float          y     = _mem_y;
	int            pos   = 0;
	do
	{
		// First sample
		float          x = src_ptr [pos];
		y = _eq_z_b0 * (x - y) + mem_x;
		dst_ptr [pos] = y;
		step_z_eq (inc_b0);
		mem_x         = x;
		++ pos;
	}
	while (pos < nbr_spl);
	_mem_y = y;
	_mem_x = mem_x;
}



void	AllPass1p::clear_buffers ()
{
	_mem_x = 0;
	_mem_y = 0;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace iir
}  // namespace dsp
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
