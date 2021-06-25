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

	ful_inline unit_utf8 * memcopy(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memcopy(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16le * memcopy(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memcopy(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16be * memcopy(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memcopy(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32le * memcopy(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memcopy(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32be * memcopy(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memcopy(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf8 * memmove(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memmove(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16le * memmove(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memmove(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16be * memmove(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memmove(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32le * memmove(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memmove(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32be * memmove(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memmove(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf8 * memmovef(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memmovef(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16le * memmovef(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memmovef(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf16be * memmovef(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memmovef(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32le * memmovef(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memmovef(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf32be * memmovef(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memmovef(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(begin)));
	}

	ful_inline unit_utf8 * memmover(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
	{
		return reinterpret_cast<unit_utf8 *>(memmover(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)));
	}

	ful_inline unit_utf16le * memmover(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * end)
	{
		return reinterpret_cast<unit_utf16le *>(memmover(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)));
	}

	ful_inline unit_utf16be * memmover(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * end)
	{
		return reinterpret_cast<unit_utf16be *>(memmover(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)));
	}

	ful_inline unit_utf32le * memmover(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * end)
	{
		return reinterpret_cast<unit_utf32le *>(memmover(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)));
	}

	ful_inline unit_utf32be * memmover(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * end)
	{
		return reinterpret_cast<unit_utf32be *>(memmover(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)));
	}

	ful_inline void memset(unit_utf8 * from, unit_utf8 * to, char8 u)
	{
		memset(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u);
	}

	ful_inline void memset(unit_utf8 * from, unit_utf8 * to, char16 u)
	{
		if (!ful_expect((to - from) % sizeof(char16) == 0))
			return;

		memset(reinterpret_cast<char16 *>(from), reinterpret_cast<char16 *>(to), u);
	}

	ful_inline void memset(unit_utf8 * from, unit_utf8 * to, char24 u)
	{
		if (!ful_expect((to - from) % sizeof(char24) == 0))
			return;

		memset(reinterpret_cast<char24 *>(from), reinterpret_cast<char24 *>(to), u);
	}

	ful_inline void memset(unit_utf8 * from, unit_utf8 * to, char32 u)
	{
		if (!ful_expect((to - from) % sizeof(char32) == 0))
			return;

		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline void memset(unit_utf16le * from, unit_utf16le * to, char16 u)
	{
		memset(reinterpret_cast<char16 *>(from), reinterpret_cast<char16 *>(to), u);
	}

	ful_inline void memset(unit_utf16le * from, unit_utf16le * to, char32 u)
	{
		if (!ful_expect((to - from) * sizeof(unit_utf16be) % sizeof(char32) == 0))
			return;

		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline void memset(unit_utf16be * from, unit_utf16be * to, char16 u)
	{
		memset(reinterpret_cast<char16 *>(from), reinterpret_cast<char16 *>(to), u);
	}

	ful_inline void memset(unit_utf16be * from, unit_utf16be * to, char32 u)
	{
		if (!ful_expect((to - from) * sizeof(unit_utf16be) % sizeof(char32) == 0))
			return;

		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline void memset(unit_utf32le * from, unit_utf32le * to, char32 u)
	{
		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline void memset(unit_utf32be * from, unit_utf32be * to, char32 u)
	{
		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline unit_utf8 * memswap(unit_utf8 * beg1, unit_utf8 * end1, unit_utf8 * beg2)
	{
		return reinterpret_cast<unit_utf8 *>(memswap(reinterpret_cast<char8 *>(beg1), reinterpret_cast<char8 *>(end1), reinterpret_cast<char8 *>(beg2)));
	}

	ful_inline unit_utf16le * memswap(unit_utf16le * beg1, unit_utf16le * end1, unit_utf16le * beg2)
	{
		return reinterpret_cast<unit_utf16le *>(memswap(reinterpret_cast<char8 *>(beg1), reinterpret_cast<char8 *>(end1), reinterpret_cast<char8 *>(beg2)));
	}

	ful_inline unit_utf16be * memswap(unit_utf16be * beg1, unit_utf16be * end1, unit_utf16be * beg2)
	{
		return reinterpret_cast<unit_utf16be *>(memswap(reinterpret_cast<char8 *>(beg1), reinterpret_cast<char8 *>(end1), reinterpret_cast<char8 *>(beg2)));
	}

	ful_inline unit_utf32le * memswap(unit_utf32le * beg1, unit_utf32le * end1, unit_utf32le * beg2)
	{
		return reinterpret_cast<unit_utf32le *>(memswap(reinterpret_cast<char8 *>(beg1), reinterpret_cast<char8 *>(end1), reinterpret_cast<char8 *>(beg2)));
	}

	ful_inline unit_utf32be * memswap(unit_utf32be * beg1, unit_utf32be * end1, unit_utf32be * beg2)
	{
		return reinterpret_cast<unit_utf32be *>(memswap(reinterpret_cast<char8 *>(beg1), reinterpret_cast<char8 *>(end1), reinterpret_cast<char8 *>(beg2)));
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
