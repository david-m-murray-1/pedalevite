/*****************************************************************************

        DcKiller1p.h
        Author: Laurent de Soras, 2018

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_iir_DcKiller1p_HEADER_INCLUDED)
#define mfx_dsp_iir_DcKiller1p_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/iir/OnePole.h"



namespace mfx
{
namespace dsp
{
namespace iir
{



class DcKiller1p
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	               DcKiller1p ()  = default;
	virtual        ~DcKiller1p () = default;

	void           set_sample_freq (double sample_freq);
	void           set_cutoff_freq (float f0);
	inline float   process_sample (float x);
	void           process_block (float dst_ptr [], const float src_ptr [], int nbr_spl);

	void           clear_buffers ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	void           update_z_eq ();

	OnePole        _filter;
	float          _sample_freq = 0;    // Sampling rate, Hz, > 0. 0 = not set
	float          _inv_fs      = 0;
	float          _f0          = 5;    // Cutoff frequency, Hz, > 0



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               DcKiller1p (const DcKiller1p &other)        = delete;
	DcKiller1p &   operator = (const DcKiller1p &other)        = delete;
	bool           operator == (const DcKiller1p &other) const = delete;
	bool           operator != (const DcKiller1p &other) const = delete;

}; // class DcKiller1p



}  // namespace iir
}  // namespace dsp
}  // namespace mfx



#include "mfx/dsp/iir/DcKiller1p.hpp"



#endif   // mfx_dsp_iir_DcKiller1p_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
