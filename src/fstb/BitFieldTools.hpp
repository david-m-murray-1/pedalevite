/*****************************************************************************

        BitFieldTools.hpp
        Author: Laurent de Soras, 2006

--- Legal stuff ---

This program is free software. It comes without any warranty, to
the extent permitted by applicable law. You can redistribute it
and/or modify it under the terms of the Do What The Fuck You Want
To Public License, Version 2, as published by Sam Hocevar. See
http://sam.zoy.org/wtfpl/COPYING for more details.

*Tab=3***********************************************************************/



#if ! defined (fstb_BitFieldTools_CODEHEADER_INCLUDED)
#define fstb_BitFieldTools_CODEHEADER_INCLUDED



/*\\\ INCLUDE FILES \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/

#include "fstb/Err.h"

#include <cassert>



namespace fstb
{



/*\\\ PUBLIC \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



template <typename T, int DL2>
int	BitFieldTools <T, DL2>::calculate_nbr_groups (int nbr_elt)
{
	assert (nbr_elt > 0);

	const int      nbr_groups = ((nbr_elt - 1) >> BITDEPTH_L2) + 1;
	assert (nbr_groups > 0);

	return (nbr_groups);
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::calculate_group_and_pos (int &group, int &gpos, int pos)
{
	assert (pos >= 0);

	group = (pos >> BITDEPTH_L2);
	gpos  = int (pos & (BITDEPTH - 1));
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::calculate_group_and_mask (int &group, GroupType &mask, int pos)
{
	assert (pos >= 0);

	int            gpos;
	calculate_group_and_pos (group, gpos, pos);
	mask = GroupType (1) << gpos;
}



template <typename T, int DL2>
bool	BitFieldTools <T, DL2>::get_bit (const GroupType bit_arr [], int pos)
{
	assert (pos >= 0);

	int            group;
	GroupType      mask;
	calculate_group_and_mask (group, mask, pos);

	return ((bit_arr [group] & mask) != 0);
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::set_bit (GroupType bit_arr [], int pos, bool flag)
{
	assert (&bit_arr != 0);
	assert (pos >= 0);

	int            group;
	GroupType      mask;
	calculate_group_and_mask (group, mask, pos);

	if (flag)
	{
		bit_arr [group] |= mask;
	}
	else
	{
		bit_arr [group] &= ~mask;
	}
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::clear_bit (GroupType bit_arr [], int pos)
{
	assert (&bit_arr != 0);
	assert (pos >= 0);

	int            group;
	GroupType      mask;
	calculate_group_and_mask (group, mask, pos);
	bit_arr [group] &= ~mask;
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::fill_bit (GroupType bit_arr [], int pos)
{
	assert (&bit_arr != 0);
	assert (pos >= 0);

	int            group;
	GroupType      mask;
	calculate_group_and_mask (group, mask, pos);
	bit_arr [group] |= mask;
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::activate_range (GroupType bit_arr [], int pos, int nbr_elt)
{
	assert (bit_arr != nullptr);
	assert (pos >= 0);
	assert (nbr_elt > 0);

	const int     end_pos = pos + nbr_elt - 1;

	int            beg_group;
	int            beg_gpos;
	calculate_group_and_pos (beg_group, beg_gpos, pos);
	int            end_group;
	int            end_gpos;
	calculate_group_and_pos (end_group, end_gpos, end_pos);

	if (beg_group == end_group)
	{
		const GroupType   mask =
			(GroupType (2) << end_gpos) - (GroupType (1) << beg_gpos);
		bit_arr [beg_group] |= mask;
	}

	else
	{
		// Beginning
		GroupType      mask = (~GroupType (0)) << beg_gpos;
		bit_arr [beg_group] |= mask;

		// Middle
		for (int group = beg_group + 1; group < end_group; ++group)
		{
			bit_arr [group] = ~GroupType (0);
		}

		// End
		mask = (GroupType (2) << end_gpos) - 1;
		bit_arr [end_group] |= mask;
	}
}



template <typename T, int DL2>
void	BitFieldTools <T, DL2>::deactivate_range (GroupType bit_arr [], int pos, int nbr_elt)
{
	assert (bit_arr != 0);
	assert (pos >= 0);
	assert (nbr_elt > 0);

	const int      end_pos = pos + nbr_elt - 1;

	int            beg_group;
	int            beg_gpos;
	calculate_group_and_pos (beg_group, beg_gpos, pos);
	int            end_group;
	int            end_gpos;
	calculate_group_and_pos (end_group, end_gpos, end_pos);

	GroupType      mask;

	if (beg_group == end_group)
	{
		mask = (GroupType (2) << end_gpos) - (GroupType (1) << beg_gpos);
		bit_arr [beg_group] &= ~mask;
	}

	else
	{
		// Beginning
		mask = (GroupType (1) << beg_gpos) - 1;
		bit_arr [beg_group] &= mask;

		// Middle
		for (int group = beg_group + 1; group < end_group; ++group)
		{
			bit_arr [group] = 0;
		}

		// End
		mask = (~GroupType (1)) << end_gpos;
		bit_arr [end_group] &= mask;
	}
}



// Returns Err_NOT_FOUND if not found
template <typename T, int DL2>
int	BitFieldTools <T, DL2>::get_next_bit_set_from (const GroupType bit_arr [], int pos, int length)
{
	assert (bit_arr != nullptr);
	assert (pos >= 0);
	assert (pos < length);

	GroupType      mask   = 0;
	int            offset = 0;
	calculate_group_and_mask (offset, mask, pos);
	-- mask;       // All bits before current one
	mask = ~mask;  // All bits after and including current one

	// Rough search
	const int      nbr_groups = (length + (BITDEPTH - 1)) >> BITDEPTH_L2;
	while (   offset < nbr_groups
	       && (bit_arr [offset] & mask) == 0)
	{
		++ offset;
		pos  = offset << BITDEPTH_L2;
		mask = (((GroupType (1) << (BITDEPTH - 1)) - 1) << 1) | 1;
	}

	// Refine
	int            on_pos = Err_NOT_FOUND;
	while (pos < length && on_pos < 0)
	{
		if (get_bit (bit_arr, pos))
		{
			on_pos = pos;
		}
		else
		{
			++ pos;
		}
	}

	return (on_pos);
}



/*\\\ PROTECTED \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



/*\\\ PRIVATE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/



}  // namespace fstb



#endif   // fstb_BitFieldTools_CODEHEADER_INCLUDED



/*\\\ EOF \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
