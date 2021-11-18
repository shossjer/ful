#pragma once

#ifndef FUL_INCLUDED_STRING_HPP
# error string.hpp must be included before string_compare.hpp
#endif

namespace ful
{
	template <typename R, typename T>
	ful_inline ful_pure auto operator == (const R & x, const T * cstr)
		-> decltype(equal(to_address(begin(x)), to_address(end(x)), cstr))
	{
		return equal(to_address(begin(x)), to_address(end(x)), cstr);
	}

	template <typename T, typename R>
	ful_inline ful_pure auto operator == (const T * cstr, const R & x)
		-> decltype(equal(to_address(begin(x)), to_address(end(x)), cstr))
	{
		return equal(to_address(begin(x)), to_address(end(x)), cstr);
	}

	template <typename R1, typename R2>
	ful_inline ful_pure auto operator == (const R1 & x, const R2 & y)
		-> decltype(equal(to_address(begin(x)), to_address(end(x)), to_address(begin(y)), to_address(end(y))))
	{
		return equal(to_address(begin(x)), to_address(end(x)), to_address(begin(y)), to_address(end(y)));
	}

	template <typename R1, typename R2>
	ful_inline ful_pure auto operator != (const R1 & x, const R2 & y)
		-> decltype(x == y)
	{
		return !(x == y);
	}

	template <typename Begin, typename End, typename First, typename Last>
	ful_inline ful_pure auto starts_with(Begin beg1, End end1, First beg2, Last end2)
		-> decltype(equal(to_address(beg1), to_address(end1), to_address(beg2), to_address(end2)))
	{
		const auto beg1_ptr = to_address(beg1);
		const auto end1_ptr = to_address(end1);
		const auto beg2_ptr = to_address(beg2);
		const auto end2_ptr = to_address(end2);

		const ful::usize size = end2_ptr - beg2_ptr;
		if (static_cast<ful::usize>(end1_ptr - beg1_ptr) < size)
			return false;

		return equal(beg1_ptr, beg1_ptr + size, beg2_ptr, end2_ptr);
	}

	template <typename R, typename First, typename Last>
	ful_inline ful_pure auto starts_with(const R & x, First beg2, Last end2)
		-> decltype(starts_with(begin(x), end(x), beg2, end2))
	{
		return starts_with(begin(x), end(x), beg2, end2);
	}

	template <typename Begin, typename End, typename R>
	ful_inline ful_pure auto starts_with(Begin begin1, End end1, const R & x)
		-> decltype(starts_with(begin1, end1, begin(x), end(x)))
	{
		return starts_with(begin1, end1, begin(x), end(x));
	}

	template <typename R1, typename R2>
	ful_inline ful_pure auto starts_with(const R1 & x, const R2 & y)
		-> decltype(starts_with(begin(x), end(x), begin(y), end(y)))
	{
		return starts_with(begin(x), end(x), begin(y), end(y));
	}

	template <typename Begin, typename End, typename First, typename Last>
	ful_inline ful_pure auto ends_with(Begin beg1, End end1, First beg2, Last end2)
		-> decltype(equal(to_address(beg1), to_address(end1), to_address(beg2), to_address(end2)))
	{
		const auto beg1_ptr = to_address(beg1);
		const auto end1_ptr = to_address(end1);
		const auto beg2_ptr = to_address(beg2);
		const auto end2_ptr = to_address(end2);

		const ful::usize size = end2_ptr - beg2_ptr;
		if (static_cast<ful::usize>(end1_ptr - beg1_ptr) < size)
			return false;

		return equal(end1_ptr - size, end1_ptr, beg2_ptr, end2_ptr);
	}

	template <typename R, typename First, typename Last>
	ful_inline ful_pure auto ends_with(const R & x, First beg2, Last end2)
		-> decltype(ends_with(begin(x), end(x), beg2, end2))
	{
		return ends_with(begin(x), end(x), beg2, end2);
	}

	template <typename Begin, typename End, typename R>
	ful_inline ful_pure auto ends_with(Begin beg1, End end1, const R & x)
		-> decltype(ends_with(beg1, end1, begin(x), end(x)))
	{
		return ends_with(beg1, end1, begin(x), end(x));
	}

	template <typename R1, typename R2>
	ful_inline ful_pure auto ends_with(const R1 & x, const R2 & y)
		-> decltype(ends_with(begin(x), end(x), begin(y), end(y)))
	{
		return ends_with(begin(x), end(x), begin(y), end(y));
	}
}
