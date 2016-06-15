/*****************************************************************************

        ParamList.cpp
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

#include "fstb/fnc.h"
#include "mfx/pi/param/Tools.h"
#include "mfx/pi/DryWet.h"
#include "mfx/uitk/pg/ParamList.h"
#include "mfx/uitk/pg/Tools.h"
#include "mfx/uitk/NodeEvt.h"
#include "mfx/uitk/PageMgrInterface.h"
#include "mfx/uitk/PageSwitcher.h"
#include "mfx/ui/Font.h"
#include "mfx/LocEdit.h"
#include "mfx/Model.h"
#include "mfx/View.h"

#include <cassert>



namespace mfx
{
namespace uitk
{
namespace pg
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



ParamList::ParamList (PageSwitcher &page_switcher, LocEdit &loc_edit)
:	_page_switcher (page_switcher)
,	_loc_edit (loc_edit)
,	_model_ptr (0)
,	_view_ptr (0)
,	_page_ptr (0)
,	_page_size ()
,	_fnt_ptr (0)
,	_menu_sptr (new NWindow (Entry_WINDOW))
,	_fx_setup_sptr (new NText (Entry_FX_SETUP))
,	_param_list ()
{
	_fx_setup_sptr->set_text ("FX Setup");
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	ParamList::do_connect (Model &model, const View &view, PageMgrInterface &page, Vec2d page_size, void *usr_ptr, const ui::Font &fnt_s, const ui::Font &fnt_m, const ui::Font &fnt_l)
{
	_model_ptr = &model;
	_view_ptr  = &view;
	_page_ptr  = &page;
	_page_size = page_size;
	_fnt_ptr   = &fnt_m;

	_fx_setup_sptr->set_font (*_fnt_ptr);

	const int      scr_w = _page_size [0];
	const int      frm_w = (scr_w * 3) >> 2;

	_menu_sptr->set_size (_page_size, Vec2d ());
	_menu_sptr->set_disp_pos (Vec2d ());

	_fx_setup_sptr->set_coord (Vec2d (0, 0));
	_fx_setup_sptr->set_frame (Vec2d (frm_w, 0), Vec2d ());

	_page_ptr->push_back (_menu_sptr);

	set_param_info ();
	_page_ptr->jump_to (conv_loc_edit_to_node_id ());
}



void	ParamList::do_disconnect ()
{
	// Nothing
}



MsgHandlerInterface::EvtProp	ParamList::do_handle_evt (const NodeEvt &evt)
{
	EvtProp        ret_val = EvtProp_PASS;

	const int      node_id = evt.get_target ();

	if (evt.is_cursor ())
	{
		const NodeEvt::Curs  curs = evt.get_cursor ();
		if (node_id >= 0 && node_id < int (_param_list.size ()))
		{
			if (curs == NodeEvt::Curs_ENTER)
			{
				update_loc_edit (node_id);
			}

			// Reflects the (un)selection on the value node
			assert ((node_id & 1) == 0);
			const int      val_id = node_id + 1;
			NodeEvt        evt2 (NodeEvt::create_cursor (val_id, curs));
			_param_list [val_id]->handle_evt (evt2);
		}
	}

	else if (evt.is_button_ex ())
	{
		const Button   but = evt.get_button_ex ();
		switch (but)
		{
		case Button_S:
			/*** To do ***/
			break;
		case Button_E:
			_page_switcher.switch_to (pg::PageType_EDIT_PROG, 0);
			ret_val = EvtProp_CATCH;
			break;
		case Button_L:
			ret_val = change_param (node_id, -1);
			break;
		case Button_R:
			ret_val = change_param (node_id, +1);
			break;
		default:
			// Nothing
			break;
		}
	}

	return ret_val;
}



void	ParamList::do_activate_preset (int index)
{
	_page_switcher.switch_to (PageType_EDIT_PROG, 0);
}



void	ParamList::do_set_param (int pi_id, int index, float val, int slot_index, PiType type)
{
	if (slot_index == _loc_edit._slot_index)
	{
		update_param_txt (type, index);
	}
}



void	ParamList::do_remove_plugin (int slot_index)
{
	if (slot_index == _loc_edit._slot_index)
	{
		_page_switcher.switch_to (PageType_EDIT_PROG, 0);
	}
}



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



void	ParamList::set_param_info ()
{
	assert (_fnt_ptr != 0);

	const int      h_m   = _fnt_ptr->get_char_h ();
	const int      scr_w = _page_size [0];

	const int      slot_index = _loc_edit._slot_index;
	const doc::Preset &  preset = _view_ptr->use_preset_cur ();
	assert (! preset.is_slot_empty (slot_index));
	doc::Slot &    slot = *(preset._slot_list [slot_index]);

	// Order: mixer, main
	const std::array <int, PiType_NBR_ELT> nbr_param_arr = {{
		int (slot._settings_mixer._param_list.size ()),
		int (slot._settings_all [slot._pi_model]._param_list.size ())
	}};
	const int      nbr_param_tot = nbr_param_arr [0] + nbr_param_arr [1];

	PageMgrInterface::NavLocList  nav_list (nbr_param_tot + 1);
	_param_list.resize (nbr_param_tot * 2);

	_menu_sptr->clear_all_nodes ();
	_menu_sptr->push_back (_fx_setup_sptr);

	nav_list [0]._node_id = Entry_FX_SETUP;

	static const std::array <PiType, PiType_NBR_ELT>   type_arr =
	{{
		PiType_MIX,
		PiType_MAIN
	}};

	int            param_pos = 0;
	for (int type_cnt = 0; type_cnt < PiType_NBR_ELT; ++type_cnt)
	{
		const PiType   type      =      type_arr [type_cnt];
		const int      nbr_param = nbr_param_arr [type_cnt];

		for (int index = 0; index < nbr_param; ++index)
		{
			const int      pos_nav  = param_pos + 1; // In the nav_list
			const int      pos_disp = pos_nav;
			const int      pos_menu = param_pos * 2 + 1;
			const int      node_id  = param_pos * 2;

			TxtSPtr        name_sptr (new NText (node_id));
			name_sptr->set_coord (Vec2d (0, h_m * pos_disp));
			name_sptr->set_font (*_fnt_ptr);
			name_sptr->set_frame (Vec2d (scr_w, 0), Vec2d ());

			TxtSPtr        val_sptr (new NText (node_id + 1));
			val_sptr->set_coord (Vec2d (scr_w, h_m * pos_disp));
			val_sptr->set_font (*_fnt_ptr);
			val_sptr->set_justification (1, 0, false);

			_param_list [node_id    ] = name_sptr;
			_param_list [node_id + 1] = val_sptr;

			nav_list [pos_nav]._node_id = node_id;

			assert (pos_menu == _menu_sptr->get_nbr_nodes ());
			_menu_sptr->push_back (name_sptr);
			_menu_sptr->push_back (val_sptr);

			update_param_txt (type, index);

			++ param_pos;
		}
	}

	_page_ptr->set_nav_layout (nav_list);

	_menu_sptr->invalidate_all ();
}



void	ParamList::update_param_txt (PiType type, int index)
{
	const int      node_id    = conv_param_to_node_id (type, index);
	TxtSPtr &      name_sptr  = _param_list [node_id    ];
	TxtSPtr &      val_sptr   = _param_list [node_id + 1];
	const int      slot_index = _loc_edit._slot_index;

	const doc::Preset &  preset = _view_ptr->use_preset_cur ();
	assert (! preset.is_slot_empty (slot_index));
	doc::Slot &    slot = *(preset._slot_list [slot_index]);
	doc::PluginSettings &  settings =
			(type == PiType_MIX)
		? slot._settings_mixer
		: slot._settings_all [slot._pi_model];
	const float    val = settings._param_list [index];

	Tools::set_param_text (
		*_view_ptr, _page_size [0], index, val, slot_index, type,
		*name_sptr, *val_sptr, 0, 0
	);
}



void	ParamList::update_loc_edit (int node_id)
{
	conv_node_id_to_param (_loc_edit._pi_type, _loc_edit._param_index, node_id);
}



// Returns the first node of the pair (param name)
int	ParamList::conv_loc_edit_to_node_id () const
{
	// Default on the first parameter of the main effect
	int            node_id = conv_param_to_node_id (PiType_MAIN, 0);

	if (_loc_edit._param_index >= 0)
	{
		node_id = conv_param_to_node_id (
			_loc_edit._pi_type,
			_loc_edit._param_index
		);
	}

	node_id = std::min (node_id, int (_param_list.size ()) - 2);

	return node_id;
}



// Returns the first node of the pair (param name)
int	ParamList::conv_param_to_node_id (PiType type, int index) const
{
	assert (type >= 0);
	assert (type < PiType_NBR_ELT);
	assert (index >= 0);

	int            line_pos = index;
	if (type == PiType_MAIN)
	{
		line_pos += pi::DryWet::Param_NBR_ELT;
	}

	const int      node_id = line_pos * 2;

	return node_id;
}



void	ParamList::conv_node_id_to_param (PiType &type, int &index, int node_id)
{
	type = PiType_MIX;
	if (node_id < 0 || node_id >= int (_param_list.size ()))
	{
		index = -1;
	}
	else
	{
		int            line_pos = node_id >> 1;
		if (line_pos >= pi::DryWet::Param_NBR_ELT)
		{
			type = PiType_MAIN;
			line_pos -= pi::DryWet::Param_NBR_ELT;
		}
		index = line_pos;
	}
}



MsgHandlerInterface::EvtProp	ParamList::change_param (int node_id, int dir)
{
	assert (node_id >= 0);
	assert (dir != 0);

	EvtProp        ret_val = EvtProp_PASS;

	const int      slot_index = _loc_edit._slot_index;
	PiType         type;
	int            index;
	conv_node_id_to_param (type, index, node_id);
	assert (index >= 0);

	const doc::Preset &  preset = _view_ptr->use_preset_cur ();
	float          val_nrm =
		_view_ptr->get_param_val (preset, slot_index, type, index);
	float          step    = 0.05f;

	bool           done_flag = false;
	const View::SlotInfoList & sil = _view_ptr->use_slot_info_list ();
	if (! sil.empty () && sil [slot_index] [type].get () != 0)
	{
		const ModelObserverInterface::PluginInfo & pi_info =
			*(sil [slot_index] [type]);
		const piapi::ParamDescInterface & desc =
			pi_info._pi.get_param_info (mfx::piapi::ParamCateg_GLOBAL, index);
		const piapi::ParamDescInterface::Range range = desc.get_range ();
		if (range == piapi::ParamDescInterface::Range_DISCRETE)
		{
			double         val_nat = desc.conv_nrm_to_nat (val_nrm);
			val_nat += dir;
			const double   nat_min = desc.get_nat_min ();
			const double   nat_max = desc.get_nat_max ();
			val_nat   = fstb::limit (val_nat, nat_min, nat_max);
			val_nrm   = float (desc.conv_nat_to_nrm (val_nat));
			done_flag = true;
		}
	}

	if (! done_flag)
	{
		val_nrm += step * dir;
	}

	val_nrm = fstb::limit (val_nrm, 0.0f ,1.0f);

	_model_ptr->set_param (slot_index, type, index, val_nrm);

	ret_val = EvtProp_CATCH;

	return ret_val;
}



}  // namespace pg
}  // namespace uitk
}  // namespace mfx



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
