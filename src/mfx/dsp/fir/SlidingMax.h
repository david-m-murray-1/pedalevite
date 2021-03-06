/*****************************************************************************

        SlidingMax.h
        Author: Laurent de Soras, 2019

Algorithm in O (1), designed by Z1202
https://www.kvraudio.com/forum/viewtopic.php?p=7523506#p7523506

Template paramteters:

- T: sample data type. Must be a type working with std::numeric_limits

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://www.wtfpl.net/ for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (mfx_dsp_fir_SlidingMax_HEADER_INCLUDED)
#define mfx_dsp_fir_SlidingMax_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include <limits>
#include <vector>



namespace mfx
{
namespace dsp
{
namespace fir
{



template <typename T>
class SlidingMax
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	typedef T  DataType;

	void           set_length (int len);
	void           clear_buffers ();
	void           fill (const DataType &val);

	DataType       process_sample (DataType x);
	void           process_block (DataType dst_ptr [], const DataType src_ptr [], int nbr_spl);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	typedef std::vector <DataType> Buffer;

	int            _len      { 1 };
	int            _writepos { 0 };
	int            _flip_beg { 0 };
	int            _flip_end { 1 };
	int            _scan_pos { 0 };
	int            _scan_end { 0 }; // 0 or (_len+1)/2
	int            _scan_beg { 0 }; // (_len-1)/2 or _len-1
	DataType       _inmax    { 0 };
	DataType       _scanmax  { 0 };
	Buffer         _data     { 1, std::numeric_limits <DataType>::lowest () };



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	bool           operator == (const SlidingMax &other) const = delete;
	bool           operator != (const SlidingMax &other) const = delete;

}; // class SlidingMax



}  // namespace fir
}  // namespace dsp
}  // namespace mfx



#include "mfx/dsp/fir/SlidingMax.hpp"



#endif   // mfx_dsp_fir_SlidingMax_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
