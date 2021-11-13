#pragma once

#ifndef FUL_INCLUDED_STRING_HPP
# error string.hpp must be included before string_modify.hpp
#endif

namespace ful
{
	// return end of output (or nullptr if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename Begin, typename End>
	ful_inline auto copy(First first, Last last, Begin begin, End end)
		-> decltype(memcopy(to_address(first), to_address(last), to_address(begin)))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);
		const auto begin_ptr = to_address(begin);
		const auto end_ptr = to_address(end);

		if (!ful_expect(static_cast<usize>(last_ptr - first_ptr) <= static_cast<usize>(end_ptr - begin_ptr)))
			return nullptr;

		return memcopy(first_ptr, last_ptr, begin_ptr);
	}

	// return end of output (or nullptr if the destination is not big enough in debug builds)
	template <typename First, typename Last, typename T, unsigned long long N>
	ful_inline auto copy(First first, Last last, T (& y)[N])
		-> decltype(memcopy(to_address(first), to_address(last), y + 0))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);

		if (!ful_expect(static_cast<usize>(last_ptr - first_ptr) <= N))
			return nullptr;

		return memcopy(first_ptr, last_ptr, y + 0);
	}

	namespace detail
	{
		template <typename R>
		constexpr ful_pure auto get_alignment(const R & x, int) -> decltype(R::alignment) { return ful_unused(x), R::alignment; }
		template <typename R>
		constexpr ful_pure auto get_alignment(const R & x, ...) -> usize { return ful_unused(x), 1; }
	}

	// return end of output (or nullptr if the destination is not big enough in debug builds)
	template <typename R, typename Begin, typename End>
	ful_inline auto copy(const R & x, Begin begin_, End end_)
		-> decltype(copy(to_address(begin(x)), to_address(end(x)), begin_, end_))
	{
		const auto first_ptr = to_address(begin(x));
		const auto last_ptr = to_address(end(x));
		ful_assume(reinterpret_cast<puint>(first_ptr) % detail::get_alignment(x, 0) == 0);

		return copy(first_ptr, last_ptr, begin_, end_);
	}

	// return end of output, or nullptr on failure (as determined by the destination)
	template <typename R1, typename R2>
	ful_inline auto copy(const R1 & x, R2 & y)
		-> decltype(copy(to_address(begin(x)), to_address(end(x)), y))
	{
		const auto first_ptr = to_address(begin(x));
		const auto last_ptr = to_address(end(x));
		ful_assume(reinterpret_cast<puint>(first_ptr) % detail::get_alignment(x, 0) == 0);

		return copy(first_ptr, last_ptr, y);
	}

	template <typename R, typename Char>
	ful_inline auto fill(R & x, Char c)
		-> decltype(memset(to_address(begin(x)), to_address(end(x)), c))
	{
		return memset(to_address(begin(x)), to_address(end(x)), c);
	}
}
