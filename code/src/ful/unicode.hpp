#pragma once

#include "ful/dispatch.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/string.hpp"

#include "ful/unicode_none.hpp"
#include "ful/unicode_avx2.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern usize ful_dispatch(point_count_8)(const unit_utf8 * beg, const unit_utf8 * end);
		extern usize ful_dispatch(point_count_16)(const unit_utf16 * beg, const unit_utf16 * end);
		extern usize ful_dispatch(point_count_61)(const unit_utf61 * beg, const unit_utf61 * end);
		extern const unit_utf8 * ful_dispatch(point_next_8)(const unit_utf8 * s, usize n);
		extern const unit_utf8 * ful_dispatch(point_prev_8)(const unit_utf8 * s, usize n);
#endif
	}

	ful_inline const unit_utf8 * point_next(const unit_utf8 * s)
	{
		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		return s;
	}

	ful_inline const unit_utf16 * point_next(const unit_utf16 * s)
	{
		// todo benchmark

		++s;
		if ((static_cast<uint16>(*s) & 0xfc00) != 0xdc00)
			return s;

		++s;
		return s;
	}

	ful_inline const unit_utf61 * point_next(const unit_utf61 * s)
	{
		// todo benchmark

		++s;
		if ((static_cast<uint16>(*s) & 0x00fc) != 0x00dc)
			return s;

		++s;
		return s;
	}

	ful_inline const unit_utf32 * point_next(const unit_utf32 * s)
	{
		++s;
		return s;
	}

	ful_inline const unit_utf23 * point_next(const unit_utf23 * s)
	{
		++s;
		return s;
	}

	ful_inline const unit_utf8 * point_prev(const unit_utf8 * s)
	{
		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		return s;
	}

	ful_inline const unit_utf16 * point_prev(const unit_utf16 * s)
	{
		// todo benchmark

		--s;
		if ((static_cast<uint16>(*s) & 0xfc00) != 0xdc00)
			return s;

		--s;
		return s;
	}

	ful_inline const unit_utf61 * point_prev(const unit_utf61 * s)
	{
		// todo benchmark

		--s;
		if ((static_cast<uint16>(*s) & 0x00fc) != 0x00dc)
			return s;

		--s;
		return s;
	}

	ful_inline const unit_utf32 * point_prev(const unit_utf32 * s)
	{
		--s;
		return s;
	}

	ful_inline const unit_utf23 * point_prev(const unit_utf23 * s)
	{
		--s;
		return s;
	}

	ful_inline unsigned int point_size(const unit_utf8 * s)
	{
		if (static_cast<sint8>(*s) >= 0)
			return 1;

		if ((static_cast<sint8>(*s) & 0x20) == 0)
			return 2;

		if ((static_cast<sint8>(*s) & 0x10) == 0)
			return 3;

		return 4;
	}

	ful_inline unsigned int point_size(const unit_utf16 * s)
	{
		// todo benchmark

		if ((static_cast<uint16>(*s) & 0xfc00) != 0xd800)
			return 1;

		return 2;
	}

	ful_inline unsigned int point_size(const unit_utf61 * s)
	{
		// todo benchmark

		if ((static_cast<uint16>(*s) & 0x00fc) != 0x00d8)
			return 1;

		return 2;
	}

	ful_inline unsigned int point_size(const unit_utf32 * s)
	{
		ful_unused(s);

		return 1;
	}

	ful_inline unsigned int point_size(const unit_utf23 * s)
	{
		ful_unused(s);

		return 1;
	}

	namespace detail
	{
		ful_generic() inline
		usize point_count_8_none(const unit_utf8 * beg, const unit_utf8 * end)
		{
			// todo word

			usize count = 0;

			while (beg != end)
			{
				count++;
				beg = ful::point_next(beg);
			}
			return count;
		}

		ful_generic() inline
		usize point_count_16_none(const unit_utf16 * beg, const unit_utf16 * end)
		{
			// todo word

			usize count = 0;

			while (beg != end)
			{
				count++;
				beg = ful::point_next(beg);
			}
			return count;
		}

		ful_generic() inline
		usize point_count_61_none(const unit_utf61 * beg, const unit_utf61 * end)
		{
			// todo word

			usize count = 0;

			while (beg != end)
			{
				count++;
				beg = ful::point_next(beg);
			}
			return count;
		}

		ful_generic() inline
		const unit_utf8 * point_next_8_none(const unit_utf8 * s, usize n)
		{
			// todo word

			while (0 < n)
			{
				s = ful::point_next(s);
				--n;
			}
			return s;
		}

		ful_generic() inline
		const unit_utf8 * point_prev_8_none(const unit_utf8 * s, usize n)
		{
			// todo word

			while (0 < n)
			{
				s = ful::point_prev(s);
				--n;
			}
			return s;
		}
	}

	ful_inline unit_utf8 * memcopy(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16le * memcopy(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16be * memcopy(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32le * memcopy(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32be * memcopy(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf8 * memmove(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16le * memmove(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16be * memmove(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32le * memmove(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32be * memmove(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf8 * memmovef(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		return reinterpret_cast<unit_utf8 *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16le * memmovef(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * begin)
	{
		return reinterpret_cast<unit_utf16le *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf16be * memmovef(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * begin)
	{
		return reinterpret_cast<unit_utf16be *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32le * memmovef(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * begin)
	{
		return reinterpret_cast<unit_utf32le *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf32be * memmovef(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * begin)
	{
		return reinterpret_cast<unit_utf32be *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline unit_utf8 * memmover(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
	{
		return reinterpret_cast<unit_utf8 *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(end)));
	}

	ful_inline unit_utf16le * memmover(const unit_utf16le * first, const unit_utf16le * last, unit_utf16le * end)
	{
		return reinterpret_cast<unit_utf16le *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(end)));
	}

	ful_inline unit_utf16be * memmover(const unit_utf16be * first, const unit_utf16be * last, unit_utf16be * end)
	{
		return reinterpret_cast<unit_utf16be *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(end)));
	}

	ful_inline unit_utf32le * memmover(const unit_utf32le * first, const unit_utf32le * last, unit_utf32le * end)
	{
		return reinterpret_cast<unit_utf32le *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(end)));
	}

	ful_inline unit_utf32be * memmover(const unit_utf32be * first, const unit_utf32be * last, unit_utf32be * end)
	{
		return reinterpret_cast<unit_utf32be *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(end)));
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

	ful_inline void memset(unit_utf8 * from, unit_utf8 * to, char_fast24 u)
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
		return reinterpret_cast<unit_utf8 *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	ful_inline unit_utf16le * memswap(unit_utf16le * beg1, unit_utf16le * end1, unit_utf16le * beg2)
	{
		return reinterpret_cast<unit_utf16le *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	ful_inline unit_utf16be * memswap(unit_utf16be * beg1, unit_utf16be * end1, unit_utf16be * beg2)
	{
		return reinterpret_cast<unit_utf16be *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	ful_inline unit_utf32le * memswap(unit_utf32le * beg1, unit_utf32le * end1, unit_utf32le * beg2)
	{
		return reinterpret_cast<unit_utf32le *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	ful_inline unit_utf32be * memswap(unit_utf32be * beg1, unit_utf32be * end1, unit_utf32be * beg2)
	{
		return reinterpret_cast<unit_utf32be *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	ful_inline usize point_count(const unit_utf8 * beg, const unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_count_8(beg, end);
#else
		return detail::point_count_8_none(beg, end);
#endif
	}

	ful_inline usize point_count(const unit_utf16 * beg, const unit_utf16 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_count_16(beg, end);
#else
		return detail::point_count_16_none(beg, end);
#endif
	}

	ful_inline usize point_count(const unit_utf61 * beg, const unit_utf61 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_count_61(beg, end);
#else
		return detail::point_count_61_none(beg, end);
#endif
	}

	ful_inline usize point_count(const unit_utf32 * beg, const unit_utf32 * end)
	{
		return end - beg;
	}

	ful_inline usize point_count(const unit_utf23 * beg, const unit_utf23 * end)
	{
		return end - beg;
	}

	ful_inline const unit_utf8 * point_next(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_next_8(s, n);
#else
		return detail::point_next_8_none(s, n);
#endif
	}

	ful_inline const unit_utf8 * point_prev(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_prev_8(s, n);
#else
		return detail::point_prev_8_none(s, n);
#endif
	}
}
