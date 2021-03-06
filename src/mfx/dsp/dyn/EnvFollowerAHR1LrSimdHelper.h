/*****************************************************************************

        EnvFollowerAHR1LrSimdHelper.h
        Author: Laurent de Soras, 2018

Low-ripple and fast response single-channel peak detection.
Takes only positive values as input (signals must have been rectified before).
4 sub-follower are running in parallel, with their hold state resetting in a
round-robbin fashion periodically.

Inspired by:
Harry Bissell Jr, Envelope Follower Combines Fast Response, Low Ripple,
EDN Network, 2002-12-26
https://www.edn.com/design/analog/4344656/Envelope-follower-combines-fast-response-low-ripple

Template parameters:

- VP: class writing and reading memory with SIMD vectors for parametering and
	internal data..
	Typically, the fstb::DataAlign classes for aligned and unaligned data.
	Requires:
	static bool VP::check_ptr (const void *ptr);
	static fstb::ToolsSimd::VectF32 VP::load_f32 (const void *ptr);
	static void VP::store_f32 (void *ptr, const fstb::ToolsSimd::VectF32 val);

- ORD: filter order. >= 1

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_dyn_EnvFollowerAHR1LrSimdHelper_HEADER_INCLUDED)
#define mfx_dsp_dyn_EnvFollowerAHR1LrSimdHelper_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"
#include "fstb/ToolsSimd.h"



namespace mfx
{
namespace dsp
{
namespace dyn
{



template <class VP, int ORD>
class EnvFollowerAHR1LrSimdHelper
{

	static_assert ((ORD >= 1), "Filter order must be between 1 and 8.");
	static_assert ((ORD <= 8), "Filter order must be between 1 and 8.");

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static const int  _nbr_sub = 4;  // Number of sub-processing units

	typedef VP V128Par;

	               EnvFollowerAHR1LrSimdHelper ();
	               EnvFollowerAHR1LrSimdHelper (const EnvFollowerAHR1LrSimdHelper <VP, ORD> &other);
	               EnvFollowerAHR1LrSimdHelper (EnvFollowerAHR1LrSimdHelper <VP, ORD> &&other);

	virtual        ~EnvFollowerAHR1LrSimdHelper () = default;

	EnvFollowerAHR1LrSimdHelper <VP, ORD> &
	               operator = (const EnvFollowerAHR1LrSimdHelper <VP, ORD> &other);
	EnvFollowerAHR1LrSimdHelper <VP, ORD> &
	               operator = (EnvFollowerAHR1LrSimdHelper <VP, ORD> &&other);

	void           set_atk_coef (float coef);
	void           set_hold_time (int nbr_spl);
	void           set_rls_coef (float coef);

	fstb_FORCEINLINE float
	               process_sample (float in);
	void           process_block (float out_ptr [], const float in_ptr [], int nbr_spl);

	void           clear_buffers ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef	float	VectFloat4 [4];
	fstb_TYPEDEF_ALIGN (16, VectFloat4, VectFloat4Aligned);

	fstb_FORCEINLINE void
	               copy_vect_data (const EnvFollowerAHR1LrSimdHelper <VP, ORD> &other);
	fstb_FORCEINLINE void
	               check_and_reset (fstb::ToolsSimd::VectF32 &hold_state, int nbr_spl);
	fstb_FORCEINLINE static bool
	               test_ge_0 (const fstb::ToolsSimd::VectF32 &in);

	VectFloat4Aligned
	               _state [ORD];
	VectFloat4Aligned                   // Mask. 0 = release, 0xFF..F = hold
	               _hold_state;
	VectFloat4Aligned                   // Mask. 0 = to be reset, 0xFF..F = keep holding
	               _reset_mask;
	float          _coef_atk;
	float          _coef_rls;
	int            _hold_time;
	int            _hold_t_q;
	int            _hold_counter;       // Samples. >= _hold_t_q: reset



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const EnvFollowerAHR1LrSimdHelper <VP, ORD> &other) const = delete;
	bool           operator != (const EnvFollowerAHR1LrSimdHelper <VP, ORD> &other) const = delete;

}; // class EnvFollowerAHR1LrSimdHelper



}  // namespace dyn
}  // namespace dsp
}  // namespace mfx



#include "mfx/dsp/dyn/EnvFollowerAHR1LrSimdHelper.hpp"



#endif   // mfx_dsp_dyn_EnvFollowerAHR1LrSimdHelper_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
