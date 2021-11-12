#pragma once

#ifndef FUL_INCLUDED_STRING_HPP
# error string.hpp must be included before string_modify.hpp
#endif

namespace ful
{
	// return end of output (or nullptr if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename Begin, typename End>
	ful_inline auto copy(First first, Last last, Begin begin, End end)
		-> decltype(memcopy(first, last, begin))
	{
		if (!ful_expect(static_cast<usize>(last - first) <= static_cast<usize>(end - begin)))
			return nullptr;

		return memcopy(first, last, begin);
	}

	// return end of output (or nullptr if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename T, unsigned long long N>
	ful_inline auto copy(First first, Last last, T (& y)[N])
		-> decltype(memcopy(first, last, y + 0))
	{
		if (!ful_expect(static_cast<usize>(last - first) <= N))
			return nullptr;

		return memcopy(first, last, y + 0);
	}

	namespace detail
	{
		template <typename R>
		constexpr ful_pure auto get_alignment(const R & x, int) -> decltype(R::alignment) { return ful_unused(x), R::alignment; }
		template <typename R>
		constexpr ful_pure auto get_alignment(const R & x, ...) -> usize { return ful_unused(x), 1; }
	}

	// return end of copy (or begin if the destination is not big enough in debug builds)
	template <typename R, typename Begin, typename End>
	ful_inline auto copy(const R & x, Begin begin_, End end_)
		-> decltype(copy(begin(x), end(x), begin_, end_))
	{
		const auto first = begin(x);
		const auto last = end(x);
		ful_assume(reinterpret_cast<puint>(first) % detail::get_alignment(x, 0) == 0);

		return copy(first, last, begin_, end_);
	}

	// return end of copy, or begin on failure (as determined by the destination)
	template <typename R1, typename R2>
	ful_inline auto copy(const R1 & x, R2 & y)
		-> decltype(copy(begin(x), end(x), y))
	{
		const auto first = begin(x);
		const auto last = end(x);
		ful_assume(reinterpret_cast<puint>(first) % detail::get_alignment(x, 0) == 0);

		return copy(first, last, y);
	}

	template <typename R, typename Char>
	ful_inline auto fill(R & x, Char c)
		-> decltype(memset(begin(x), end(x), c))
	{
		return memset(begin(x), end(x), c);
	}
}
