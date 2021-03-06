/*****************************************************************************

        FixedPoint.h
        Author: Laurent de Soras, 2016

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#pragma once
#if ! defined (fstb_FixedPoint_HEADER_INCLUDED)
#define fstb_FixedPoint_HEADER_INCLUDED

#if defined (_MSC_VER)
	#pragma warning (4 : 4250)
#endif



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/def.h"

#include <cstdint>



namespace fstb
{



class FixedPoint
{

/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

public:

	enum class NoInit
	{
		NO = 0
	};

	inline         FixedPoint ();
	               FixedPoint (const FixedPoint &other) = default;
	inline explicit
	               FixedPoint (int32_t int_val);
	inline explicit
	               FixedPoint (int32_t int_val, uint32_t frac_val);
	inline explicit
	               FixedPoint (double val);
	inline explicit
	               FixedPoint (NoInit dummy);

	FixedPoint &   operator = (const FixedPoint &other) = default;

	inline void    clear ();
	inline void    set_val (double val);
	inline void    set_val (int32_t int_val, uint32_t frac_val);
	inline void    set_val_int64 (int64_t val);
	inline double  get_val_dbl () const;
	inline float   get_val_flt () const;
	inline int64_t
	               get_val_int64 () const;

	inline void    set_int_val (int32_t int_val);
	inline int32_t get_int_val () const;
	inline int32_t get_round () const;
	inline int32_t get_ceil () const;
	inline void    set_frac_val (uint32_t frac_val);
	inline uint32_t
	               get_frac_val () const;
	inline double  get_frac_val_dbl () const;
	inline float   get_frac_val_flt () const;

	inline void    neg ();
	inline void    abs ();

	inline void    bound (int32_t len);
	inline void    bound_positive (int32_t len);
	inline void    bound_and (int32_t and_val);

	inline void    add (const FixedPoint &val);
	inline void    add (const FixedPoint &val, int32_t and_val);
	inline void    add_int (int32_t int_val);
	inline void    add_int (int32_t int_val, int32_t and_val);
	inline void    add_frac (uint32_t frac_val);
	inline void    add_frac (uint32_t frac_val, int32_t and_val);
	inline FixedPoint &
	               operator += (const FixedPoint &other);
	inline FixedPoint &
	               operator += (int32_t int_val);

	inline void    sub (const FixedPoint &val);
	inline void    sub (const FixedPoint &val, int32_t and_val);
	inline void    sub_int (int32_t int_val);
	inline void    sub_int (int32_t int_val, int32_t and_val);
	inline void    sub_frac (uint32_t frac_val);
	inline void    sub_frac (uint32_t frac_val, int32_t and_val);
	inline FixedPoint &
	               operator -= (const FixedPoint &other);
	inline FixedPoint &
	               operator -= (int32_t int_val);

	inline void    shift (int nbr_bits);
	inline void    shift_left (int nbr_bits);
	inline void    shift_right (int nbr_bits);
	inline FixedPoint &
	               operator <<= (int nbr_bits);
	inline FixedPoint &
	               operator >>= (int nbr_bits);

	inline void    mul_int (int32_t val);
	inline void    mul_flt (double val);



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

protected:



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

	union Fixed3232
	{
	public:
		int64_t        _all;
		struct
		{
#if fstb_IS (ENDIAN, BIG)
			int32_t        _msw;
			uint32_t       _lsw;
#else
			uint32_t       _lsw;
			int32_t	      _msw;
#endif
		}              _part;
	};

	Fixed3232      _val;

	friend bool	operator == (const FixedPoint &left, const FixedPoint &right); 
	friend bool	operator != (const FixedPoint &left, const FixedPoint &right); 
	friend bool	operator < (const FixedPoint &left, const FixedPoint &right); 
	friend bool	operator <= (const FixedPoint &left, const FixedPoint &right); 
	friend bool	operator > (const FixedPoint &left, const FixedPoint &right); 
	friend bool	operator >= (const FixedPoint &left, const FixedPoint &right); 



/*\\\ FORBIDDEN MEMBER FUNCTIONS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

private:

}; // class FixedPoint



/*\\\ GLOBAL OPERATOR PROTOTYPES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

inline bool	operator == (const FixedPoint &left, const FixedPoint &right); 
inline bool	operator != (const FixedPoint &left, const FixedPoint &right); 
inline bool	operator < (const FixedPoint &left, const FixedPoint &right); 
inline bool	operator <= (const FixedPoint &left, const FixedPoint &right); 
inline bool	operator > (const FixedPoint &left, const FixedPoint &right); 
inline bool	operator >= (const FixedPoint &left, const FixedPoint &right); 

inline const FixedPoint	operator + (const FixedPoint &left, const FixedPoint &right);
inline const FixedPoint	operator - (const FixedPoint &left, const FixedPoint &right);



}  // namespace fstb



#include "fstb/FixedPoint.hpp"



#endif   // fstb_FixedPoint_HEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
