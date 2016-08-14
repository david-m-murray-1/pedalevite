/*****************************************************************************

        PEq.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_pi_peq_PEq_HEADER_INCLUDED)
#define mfx_pi_peq_PEq_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/util/NotificationFlag.h"
#include "fstb/util/NotificationFlagCascadeSingle.h"
#include "fstb/DataAlign.h"
#include "mfx/dsp/iir/BiquadPackSimd.h"
#include "mfx/pi/peq/PEqDesc.h"
#include "mfx/pi/peq/PEqType.h"
#include "mfx/pi/ParamStateSet.h"
#include "mfx/piapi/PluginInterface.h"

#include <array>



namespace mfx
{
namespace pi
{
namespace peq
{



class PEq
:	public piapi::PluginInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static const int  _nbr_bands    = PEqDesc::_nbr_bands;
	static const int  _update_resol = 64; // Samples, multiple of 4

	               PEq ();
	virtual        ~PEq () = default;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// mfx::piapi::PluginInterface
	virtual State  do_get_state () const;
	virtual double do_get_param_val (piapi::ParamCateg categ, int index, int note_id) const;
	virtual int    do_reset (double sample_freq, int max_buf_len, int &latency);
	virtual void   do_process_block (ProcInfo &proc);



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef dsp::iir::BiquadPackSimd <
		fstb::DataAlign <true>,
		fstb::DataAlign <true>
	>              BiqPack;

	class BandInfo
	{
	public:
		bool           is_active () const;
		bool           is_bypass () const;

		float          _gain;               // Cached parameters
		float          _freq;
		float          _q;
		PEqType        _type;
		bool           _bypass_flag;

		long           _neutral_duration;   // Time elapsed (samples) since a band is considered as neutral. If == _neutral_time, band is removed from the pack.
		int            _stage_index;        // Stage index in the biquad pack. -1: deactivated.
		fstb::util::NotificationFlagCascadeSingle
		               _change_flag;
	};

	typedef std::array <BandInfo, _nbr_bands> BandInfoArray;
	typedef std::array <int     , _nbr_bands> StageInfoArr;

	int            find_empty_stage () const;
	int            count_nbr_stages () const;
	void           neutralise_stage_immediate (int stage_index);
	void           clear_buffers_stage (int stage_index);

	inline bool    is_band_active (int band) const;
	void           activate_band (int band);
	void           deactivate_band (int band);

	void           collect_parameters (int band);
	void           update_filter_eq (int band);
	void           cook_all_bands ();
	void           cook_band (int band);

	static bool    is_unit_gain (float gain);

	State          _state;

	PEqDesc        _desc;
	ParamStateSet  _state_set;
	float          _sample_freq;        // Hz, > 0. <= 0: not initialized
	float          _inv_fs;             // 1 / _sample_freq
	int            _nbr_chn;

	BiqPack        _biq_pack;
	BandInfoArray  _band_info_arr;
	StageInfoArr   _stage_to_band_arr;  // -1 indicates that the stage is not attributed to a band.
	fstb::util::NotificationFlag
	               _param_change_flag;
	int            _neutral_time;       // Should be greater than the block length



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               PEq (const PEq &other)               = delete;
	PEq &          operator = (const PEq &other)        = delete;
	bool           operator == (const PEq &other) const = delete;
	bool           operator != (const PEq &other) const = delete;

}; // class PEq



}  // namespace peq
}  // namespace pi
}  // namespace mfx



//#include "mfx/pi/peq/PEq.hpp"



#endif   // mfx_pi_peq_PEq_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/