#pragma once

// #include "ful/types.hpp"

namespace ful
{
	namespace detail
	{
		ful_generic() inline
		usize point_count_none(const unit_utf8 * beg, const unit_utf8 * end)
		{
			usize count = 0;

			while (beg != end)
			{
				count++;
				beg = point_next(beg);
			}
			return count;
		}

		ful_generic() inline
		const unit_utf8 * point_next_none(const unit_utf8 * s, usize n)
		{
			while (0 < n)
			{
				s = point_next(s);
			}
			return s;
		}

		ful_generic() inline
		const unit_utf8 * point_prev_none(const unit_utf8 * s, usize n)
		{
			while (0 < n)
			{
				s = point_prev(s);
			}
			return s;
		}
	}
}