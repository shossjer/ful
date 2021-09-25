#pragma once

#include "ful/compiler.hpp"
#include "ful/stdhacks.hpp"
#include "ful/stdint.hpp"

namespace ful
{
	template <typename T, unsigned long long N>
	ful_inline ful_pure constexpr T * begin(T (& x)[N]) { return x + 0; }

	template <typename T, unsigned long long N>
	ful_inline ful_pure constexpr T * end(T (& x)[N]) { return x + N; }

	template <typename T, unsigned long long N>
	ful_inline ful_pure constexpr T * data(T (& x)[N]) { return x + 0; }

	template <typename T, unsigned long long N>
	ful_inline ful_pure constexpr usize size(const T (& x)[N]) { return ful_unused(x), N; }

	namespace hck
	{
		template <typename R>
		using iterator_t = decltype(begin(hck::declval<R &>()));

		namespace detail
		{
			template <typename T, unsigned long long N>
			static auto range_value_impl(const T (&)[N]) -> T;
			template <typename R>
			static auto range_value_impl(const R &) -> typename R::value_type;
		}
		template <typename R>
		using range_value_t = decltype(detail::range_value_impl(hck::declval<R &>()));
		// note this is a simplification of the standard range_value_t
	}
}