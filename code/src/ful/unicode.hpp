#pragma once

#include "ful/dispatch.hpp"
// #include "ful/stdint.hpp"
// #include "ful/types.hpp"

//#include "ful/point_utils.hpp"

#include "ful/unicode_none.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern usize ful_dispatch(point_count)(const unit_utf8 * beg, const unit_utf8 * end);
		extern const unit_utf8 * ful_dispatch(point_next)(const unit_utf8 * s, usize n);
		extern const unit_utf8 * ful_dispatch(point_prev)(const unit_utf8 * s, usize n);
#endif
	}

	inline usize point_count(const unit_utf8 * beg, const unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_count(beg, end);
#else
		return detail::point_count_none(beg, end);
#endif
	}

	inline const unit_utf8 * point_next(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_next(s, n);
#else
		return detail::point_next_none(s, n);
#endif
	}

	inline const unit_utf8 * point_prev(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_prev(s, n);
#else
		return detail::point_prev_none(s, n);
#endif
	}
}
