#pragma once

#include "ful/cstrfwd.hpp"

#include "ful/view.hpp"

namespace ful
{
	template <typename T>
	class cstr_base : public view_base<T>
	{
	public:

		using value_type = typename view_base<T>::value_type;
		using pointer = typename view_base<T>::pointer;
		using const_pointer = typename view_base<T>::const_pointer;
		using iterator = typename view_base<T>::iterator;
		using const_iterator = typename view_base<T>::const_iterator;
		using size_type = typename view_base<T>::size_type;

	public:

		cstr_base() = default;

		template <typename Begin, typename End>
		ful_inline constexpr explicit cstr_base(Begin begin, End end) noexcept
			: view_base<T>(begin, end)
		{
			ful_unused(ful_expect(*end == value_type{}));
		}

		ful_inline explicit cstr_base(const_pointer data, usize size)
			: view_base<T>(data, size)
		{
			ful_unused(ful_expect(data[size] == value_type{}));
		}

		template <unsigned long long N>
		ful_inline cstr_base(const value_type (& str)[N])
			: view_utf8(str, str + N - 1) // subtract terminating null
		{
			ful_unused(ful_expect(str[N - 1] == value_type{}));
		}

	public:

		ful_inline constexpr const_pointer c_str() const { return this->data(); }
	};

	template <typename T>
	ful_inline ful_pure constexpr typename cstr_base<T>::const_pointer c_str(const cstr_base<T> & x) { return x.c_str(); }

	template <typename T>
	ful_inline ful_pure constexpr cstr_base<T> from(const cstr_base<T> & x, typename cstr_base<T>::const_pointer from)
	{
		return ful_unused(ful_expect(begin(x) <= from)), ful_unused(ful_expect(from <= end(x))), cstr_base<T>(from, x.end());
	}

	template <typename T>
	ful_inline ful_pure constexpr cstr_base<T> last(const cstr_base<T> & x, usize size)
	{
		return ful_unused(ful_expect(size <= static_cast<usize>(end(x) - begin(x)))), from(x, x.end() - size);
	}
}
