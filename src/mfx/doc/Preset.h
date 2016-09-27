/*****************************************************************************

        Preset.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_doc_Preset_HEADER_INCLUDED)
#define mfx_doc_Preset_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/doc/PedalboardLayout.h"
#include "mfx/doc/Routing.h"
#include "mfx/doc/Slot.h"

#include <memory>
#include <map>



namespace mfx
{
namespace doc
{



class SerRInterface;
class SerWInterface;

class Preset
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef std::shared_ptr <Slot> SlotSPtr;
	typedef std::map <int, SlotSPtr> SlotMap;

	               Preset ()  = default;
	               Preset (const Preset &other);
	virtual        ~Preset () = default;

	Preset &       operator = (const Preset &other);

	bool           is_slot_empty (int slot_id) const;
	bool           is_slot_empty (SlotMap::const_iterator it) const;
	Slot &         use_slot (int slot_id);
	const Slot &   use_slot (int slot_id) const;
	int            gen_slot_id () const;

	void           ser_write (SerWInterface &ser) const;
	void           ser_read (SerRInterface &ser);

	SlotMap        _slot_map;
	Routing        _routing;
	std::string    _name;
	PedalboardLayout
	               _layout;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	void           duplicate_slot_list ();



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const Preset &other) const = delete;
	bool           operator != (const Preset &other) const = delete;

}; // class Preset



}  // namespace doc
}  // namespace mfx



//#include "mfx/doc/Preset.hpp"



#endif   // mfx_doc_Preset_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
