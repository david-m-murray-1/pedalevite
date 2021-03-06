/*****************************************************************************

        FileIOPi3.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_hw_FileIOPi3_HEADER_INCLUDED)
#define mfx_hw_FileIOPi3_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/FileIOInterface.h"



namespace mfx
{

namespace ui
{
	class LedInterface;
}

namespace hw
{



class FileIOPi3 final
:	public FileIOInterface
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	explicit       FileIOPi3 (ui::LedInterface &led);
	virtual        ~FileIOPi3 () = default;



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:

	// mfx::FileIOInterface
	int            do_write_txt_file (const std::string &pathname, const std::string &content) final;
	int            do_read_txt_file (const std::string &pathname, std::string &content) final;



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	ui::LedInterface &
	               _led;



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               FileIOPi3 ()                               = delete;
	               FileIOPi3 (const FileIOPi3 &other)         = delete;
	               FileIOPi3 (FileIOPi3 &&other)              = delete;
	FileIOPi3 &    operator = (const FileIOPi3 &other)        = delete;
	FileIOPi3 &    operator = (FileIOPi3 &&other)             = delete;
	bool           operator == (const FileIOPi3 &other) const = delete;
	bool           operator != (const FileIOPi3 &other) const = delete;

}; // class FileIOPi3



}  // namespace hw
}  // namespace mfx



//#include "mfx/hw/FileIOPi3.hpp"



#endif   // mfx_hw_FileIOPi3_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
