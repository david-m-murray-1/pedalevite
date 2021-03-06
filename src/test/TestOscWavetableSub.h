/*****************************************************************************

        TestOscWavetableSub.h
        Author: Laurent de Soras, 2019

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (TestOscWavetableSub_HEADER_INCLUDED)
#define TestOscWavetableSub_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "mfx/dsp/osc/OscWavetableSub.h"
#include "mfx/dsp/rspl/InterpFtor.h"



class TestOscWavetableSub
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	static int     perform_test ();



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	template <typename O>
	static int		test_valid ();

	template <typename O>
	static void		test_speed ();

	template <typename O>
	static void		configure_osc (O &osc, typename O::WavetableDataType &wt);

	template <typename O>
	static float   get_data_scale ();

	template <typename O>
	static void    add_result (std::vector <float> &result_m, const std::vector <typename O::DataType> &data, size_t len = 0);



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	               TestOscWavetableSub ()                               = delete;
	               TestOscWavetableSub (const TestOscWavetableSub &other) = delete;
	virtual        ~TestOscWavetableSub ()                              = delete;
	TestOscWavetableSub &
	               operator = (const TestOscWavetableSub &other)        = delete;
	bool           operator == (const TestOscWavetableSub &other) const = delete;
	bool           operator != (const TestOscWavetableSub &other) const = delete;

}; // class TestOscWavetableSub



//#include "test/TestOscWavetableSub.hpp"



#endif   // TestOscWavetableSub_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
