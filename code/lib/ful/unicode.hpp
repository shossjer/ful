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

	ful_inline unsigned int point_type(const unit_utf8 * s)
	{
		return point_size(s);
	}

	ful_inline unsigned int point_type(const unit_utf16 * s)
	{
		if (static_cast<uint16>(*s) < 0x80)
			return 1;

		if (static_cast<uint16>(*s) < 0x800)
			return 2;

		if ((static_cast<uint16>(*s) & 0xfc00) != 0xd800)
			return 3;

		return 4;
	}

	namespace detail
	{
		inline
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

		inline
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

		inline
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

		inline
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

		inline
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

		ful_inline usize point_count(const unit_utf8 * beg, const unit_utf8 * end)
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::point_count_8_rtd(beg, end);
#else
			return detail::point_count_8_none(beg, end);
#endif
		}

		ful_inline usize point_count(const unit_utf16 * beg, const unit_utf16 * end)
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::point_count_16_rtd(beg, end);
#else
			return detail::point_count_16_none(beg, end);
#endif
		}

		ful_inline usize point_count(const unit_utf61 * beg, const unit_utf61 * end)
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::point_count_61_rtd(beg, end);
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
	}

	template <typename Begin, typename End>
	ful_inline auto point_count(Begin begin, End end)
		-> decltype(detail::point_count(to_address(begin), to_address(end)))
	{
		return detail::point_count(to_address(begin), to_address(end));
	}

	template <typename R>
	ful_inline auto point_count(R && x)
		-> decltype(point_count(begin(x), end(x)))
	{
		return point_count(begin(x), end(x));
	}

	ful_inline const unit_utf8 * point_next(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_next_8_rtd(s, n);
#else
		return detail::point_next_8_none(s, n);
#endif
	}

	ful_inline const unit_utf8 * point_prev(const unit_utf8 * s, usize n)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::point_prev_8_rtd(s, n);
#else
		return detail::point_prev_8_none(s, n);
#endif
	}
}
