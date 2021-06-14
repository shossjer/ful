#pragma once

#include "ful/dispatch.hpp"
// #include "ful/stdint.hpp"
// #include "ful/types.hpp"

#include "ful/string.hpp"

#include "ful/point_utils.hpp"

#include "ful/unicode_none.hpp"
#include "ful/unicode_avx2.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern unit_utf16le * ful_dispatch(convert_8_16le)(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_8_16be)(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_8_32le)(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_8_32be)(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_16le_8)(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16be * ful_dispatch(convert_16le_16be)(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_16le_32le)(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_16le_32be)(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_16be_8)(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_16be_16le)(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf32le * ful_dispatch(convert_16be_32le)(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_16be_32be)(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_32le_8)(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_32le_16le)(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_32le_16be)(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32be * ful_dispatch(convert_32le_32be)(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_32be_8)(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_32be_16le)(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_32be_16be)(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_32be_32le)(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end);
		extern usize ful_dispatch(point_count)(const unit_utf8 * beg, const unit_utf8 * end);
		extern const unit_utf8 * ful_dispatch(point_next)(const unit_utf8 * s, usize n);
		extern const unit_utf8 * ful_dispatch(point_prev)(const unit_utf8 * s, usize n);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_16le(first, last, begin, end);
#else
		return detail::convert_8_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_16be(first, last, begin, end);
#else
		return detail::convert_8_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_32le(first, last, begin, end);
#elif defined(__AVX2__)
		return detail::convert_8_32le_avx2(first, last, begin, end);
#else
		return detail::convert_8_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_32be(first, last, begin, end);
#else
		return detail::convert_8_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_8(first, last, begin, end);
#else
		return detail::convert_16le_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_16be(first, last, begin, end);
#else
		return detail::convert_16le_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_32le(first, last, begin, end);
#else
		return detail::convert_16le_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_32be(first, last, begin, end);
#else
		return detail::convert_16le_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_8(first, last, begin, end);
#else
		return detail::convert_16be_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_16le(first, last, begin, end);
#else
		return detail::convert_16be_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_32le(first, last, begin, end);
#else
		return detail::convert_16be_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_32be(first, last, begin, end);
#else
		return detail::convert_16be_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_8(first, last, begin, end);
#else
		return detail::convert_32le_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_16le(first, last, begin, end);
#else
		return detail::convert_32le_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_16be(first, last, begin, end);
#else
		return detail::convert_32le_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_32be(first, last, begin, end);
#else
		return detail::convert_32le_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_8(first, last, begin, end);
#else
		return detail::convert_32be_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_16le(first, last, begin, end);
#else
		return detail::convert_32be_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_16be(first, last, begin, end);
#else
		return detail::convert_32be_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_32le(first, last, begin, end);
#else
		return detail::convert_32be_32le_none(first, last, begin, end);
#endif
	}

	ful_inline usize point_count(const unit_utf8 * beg, const unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_count(beg, end);
#else
		return detail::point_count_none(beg, end);
#endif
	}

	ful_inline const unit_utf8 * point_next(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_next(s, n);
#else
		return detail::point_next_none(s, n);
#endif
	}

	ful_inline const unit_utf8 * point_prev(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_prev(s, n);
#else
		return detail::point_prev_none(s, n);
#endif
	}
}
