/*****************************************************************************

        Phaser2Desc.cpp
        Author: Laurent de Soras, 2017

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

#include "mfx/pi/param/HelperDispNum.h"
#include "mfx/pi/param/MapPiecewiseLinLog.h"
#include "mfx/pi/param/MapS.h"
#include "mfx/pi/param/TplEnum.h"
#include "mfx/pi/param/TplInt.h"
#include "mfx/pi/param/TplLin.h"
#include "mfx/pi/param/TplLog.h"
#include "mfx/pi/param/TplMapped.h"
#include "mfx/pi/phase2/LfoType.h"
#include "mfx/pi/phase2/Param.h"
#include "mfx/pi/phase2/Phaser2Desc.h"
#include "mfx/pi/ParamMapFdbkBipolar.h"

#include <cassert>



namespace mfx
{
namespace pi
{
namespace phase2
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



Phaser2Desc::Phaser2Desc ()
:	_desc_set (Param_NBR_ELT, 0)
{
	typedef param::TplMapped <param::MapPiecewiseLinLog> TplPll;
	typedef param::TplMapped <param::MapS <false> > TplMaps;
	typedef param::TplMapped <ParamMapFdbkBipolar> TplFdbk;

	// Speed
	param::TplLog *   log_ptr = new param::TplLog (
		0.01, 100,
		"Sp\nSpd\nSpeed",
		"Hz",
		param::HelperDispNum::Preset_FLOAT_STD,
		0,
		"%7.3f"
	);
	log_ptr->set_categ (piapi::ParamDescInterface::Categ_FREQ_HZ);
	_desc_set.add_glob (Param_SPEED, log_ptr);

	// Mix
	TplMaps *      maps_ptr = new TplMaps (
		0, 1,
		"Phase Mix\nPh Mix\nMix\nM",
		"%",
		param::HelperDispNum::Preset_FLOAT_PERCENT,
		0,
		"%5.1f"
	);
	maps_ptr->use_mapper ().config (
		maps_ptr->get_nat_min (),
		maps_ptr->get_nat_max ()
	);
	_desc_set.add_glob (Param_MIX, maps_ptr);

	// Feedback
	TplFdbk *      fbi_ptr = new TplFdbk (
		TplFdbk::Mapper::get_nat_min (),
		TplFdbk::Mapper::get_nat_max (),
		"Feedback level\nFdbk level\nFeedback\nFdbk\nF",
		"%",
		param::HelperDispNum::Preset_FLOAT_PERCENT,
		0,
		"%+6.1f"
	);
	_desc_set.add_glob (Param_FEEDBACK, fbi_ptr);

	// Number of stages
	param::TplInt *   int_ptr = new param::TplInt (
		1, 20,
		"Number of stages\nStages\nStg",
		"",
		0,
		"%.0f"
	);
	int_ptr->use_disp_num ().set_scale (2);
	_desc_set.add_glob (Param_NBR_STAGES, int_ptr);

	// Minimum frequency
	TplPll *       pll_ptr = new TplPll (
		20, 20480,
		"Minimum frequency\nMin freq\nMinF\nMF",
		"Hz",
		param::HelperDispNum::Preset_FLOAT_STD,
		0,
		"%4.0f"
	);
	pll_ptr->use_mapper ().gen_log (10);
	pll_ptr->set_categ (piapi::ParamDescInterface::Categ_FREQ_HZ);
	_desc_set.add_glob (Param_FREQ_MIN, pll_ptr);

	// Maximum frequency
	pll_ptr = new TplPll (
		20, 20480,
		"Maximum frequency\nMax freq\nMaxF\nMF",
		"Hz",
		param::HelperDispNum::Preset_FLOAT_STD,
		0,
		"%4.0f"
	);
	pll_ptr->use_mapper ().gen_log (10);
	pll_ptr->set_categ (piapi::ParamDescInterface::Categ_FREQ_HZ);
	_desc_set.add_glob (Param_FREQ_MAX, pll_ptr);

	// Feedback source
	int_ptr = new param::TplInt (
		1, 40,
		"Feedback source\nFdbk source\nFdbk src\nFSrc\nFS",
		"",
		0,
		"%.0f"
	);
	_desc_set.add_glob (Param_FDBK_POS, int_ptr);

	// Waveform
	param::TplEnum *  enu_ptr = new param::TplEnum (
		"Sine\nTriangle\nSquare\nSaw\nParabola\nBiphase\nN-Phase\nVarislope\nNoise",
		"W\nWavef\nWaveform",
		"",
		0,
		"%s"
	);
	_desc_set.add_glob (Param_WAVEFORM, enu_ptr);
	assert (enu_ptr->get_nat_max () == LfoType_NBR_ELT - 1);

	// Sample and hold
	param::TplLin *   lin_ptr = new param::TplLin (
		0, 1,
		"SnH\nSplHold\nSample & hold\nSample and hold",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_SNH, lin_ptr);

	// Smoothing
	lin_ptr = new param::TplLin (
		0, 1,
		"Sm\nSmooth\nSmoothing",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_SMOOTH, lin_ptr);

	// Chaos amount
	lin_ptr = new param::TplLin (
		0, 1,
		"C\nChaos\nChaos amt\nChaos amount",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_CHAOS, lin_ptr);

	// Phase distortion amount
	lin_ptr = new param::TplLin (
		0, 1,
		"PDA\nPhDistA\nPhDistAmt\nPhase dist amt\nPhase dist amount\nPhase distortion amount",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_PH_DIST_AMT, lin_ptr);

	// Phase distortion offset
	lin_ptr = new param::TplLin (
		0, 1,
		"PDO\nPhDistO\nPhDistOfs\nPhase dist ofs\nPhase dist offset\nPhase distortion offset",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_PH_DIST_OFS, lin_ptr);

	// Sign
	enu_ptr = new param::TplEnum (
		"Normal\nInvert",
		"Si\nSign",
		"",
		0,
		"%s"
	);
	_desc_set.add_glob (Param_SIGN, enu_ptr);

	// Polarity
	enu_ptr = new param::TplEnum (
		"Bipolar\nUnipolar",
		"Po\nPolar\nPolarity",
		"",
		0,
		"%s"
	);
	_desc_set.add_glob (Param_POLARITY, enu_ptr);

	// Variation 1
	lin_ptr = new param::TplLin (
		0, 1,
		"V1\nVar 1\nVariation 1",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_VAR1, lin_ptr);

	// Variation 2
	lin_ptr = new param::TplLin (
		0, 1,
		"V2\nVar 2\nVariation 2",
		"%",
		0,
		"%5.1f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_PERCENT);
	_desc_set.add_glob (Param_VAR2, lin_ptr);

	// Phase set
	lin_ptr = new param::TplLin (
		0, 1,
		"PS\nPh set\nPhase set",
		"deg",
		0,
		"%3.0f"
	);
	lin_ptr->use_disp_num ().set_preset (param::HelperDispNum::Preset_FLOAT_STD);
	lin_ptr->use_disp_num ().set_scale (360);
	_desc_set.add_glob (Param_PHASE_SET, lin_ptr);
}



ParamDescSet &	Phaser2Desc::use_desc_set ()
{
	return _desc_set;
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



std::string	Phaser2Desc::do_get_unique_id () const
{
	return "phaser2";
}



std::string	Phaser2Desc::do_get_name () const
{
	return "Phaser AP";
}



void	Phaser2Desc::do_get_nbr_io (int &nbr_i, int &nbr_o, int &nbr_s) const
{
	nbr_i = 1;
	nbr_o = 1;
	nbr_s = 0;
}



bool	Phaser2Desc::do_prefer_stereo () const
{
	return true;
}



int	Phaser2Desc::do_get_nbr_param (piapi::ParamCateg categ) const
{
	return _desc_set.get_nbr_param (categ);
}



const piapi::ParamDescInterface &	Phaser2Desc::do_get_param_info (piapi::ParamCateg categ, int index) const
{
	return _desc_set.use_param (categ, index);
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace phase2
}  // namespace pi
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/