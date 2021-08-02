#pragma once

#ifndef FUL_INCLUDED_STRING_HPP
# error string.hpp must be included before string_modify.hpp
#endif

namespace ful
{
	// return end of copy (or begin if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename Begin, typename End>
	ful_inline auto copy(First first, Last last, Begin begin, End end)
		-> decltype(memcopy(first, last, begin))
	{
		if (!ful_expect(static_cast<usize>(last - first) <= static_cast<usize>(end - begin)))
			return begin;

		return memcopy(first, last, begin);
	}

	// return end of copy (or begin if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename T, unsigned long long N>
	ful_inline auto copy(First first, Last last, T (& y)[N])
		-> decltype(memcopy(first, last, y + 0))
	{
		if (!ful_expect(static_cast<usize>(last - first) <= N))
			return y + 0;

		return memcopy(first, last, y + 0);
	}

	// return end of copy (or begin if the destination is not big enough in debug builds)
	template <typename T, unsigned long long N, typename Begin, typename End>
	ful_inline auto copy(T (& x)[N], Begin begin, End end)
		-> decltype(memcopy(x + 0, x + N, begin))
	{
		if (!ful_expect(N <= static_cast<usize>(end - begin)))
			return begin;

		return memcopy(x + 0, x + N, begin);
	}

	// return end of copy, or begin on failure (as determined by the destination)
	template <typename T, unsigned long long N, typename R>
	ful_inline auto copy(T (& x)[N], R && y)
		-> decltype(copy(x + 0, x + N, hck::forward<R &&>(y)))
	{
		return copy(x + 0, x + N, hck::forward<R &&>(y));
	}

	template <typename T, unsigned long long N, typename Char>
	ful_inline auto fill(T (& x)[N], Char c)
		-> decltype(memset(x + 0, x + N, c))
	{
		return memset(x + 0, x + N, c);
	}
}
