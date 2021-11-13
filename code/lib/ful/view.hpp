#pragma once

#include "ful/viewfwd.hpp"

#include "ful/string.hpp"
#include "ful/ranges.hpp"

namespace ful
{
	template <typename T>
	class view_base
	{
	public:

		using value_type = T;
		using pointer = T *;
		using const_pointer = const T *;
		using iterator = T *;
		using const_iterator = const T *;
		using size_type = usize;

	private:

		const_pointer beg_;
		const_pointer end_;

	public:

		view_base() = default;

		template <typename Begin, typename End, decltype(to_address(hck::declval<Begin>()), to_address(hck::declval<End>()), int()) = 0>
		ful_inline constexpr explicit view_base(Begin begin, End end) noexcept
			: beg_(to_address(begin))
			, end_(to_address(end))
		{}

		ful_inline constexpr explicit view_base(const_pointer data, usize size) noexcept
			: beg_(data)
			, end_(data + size)
		{}

		ful_inline void swap(view_base & other) noexcept
		{
			const auto tmp_beg = beg_;
			const auto tmp_end = end_;

			beg_ = other.beg_;
			end_ = other.end_;

			other.beg_ = tmp_beg;
			other.end_ = tmp_end;
		}

	public:

		ful_inline constexpr const_pointer begin() const { return beg_; }
		ful_inline constexpr const_pointer end() const { return end_; }

		ful_inline constexpr usize size() const { return end_ - beg_; }
		ful_inline constexpr const_pointer data() const { return beg_; }
	};

	template <typename T>
	ful_inline ful_pure constexpr typename view_base<T>::const_iterator begin(const view_base<T> & x) { return x.begin(); }

	template <typename T>
	ful_inline ful_pure constexpr typename view_base<T>::const_iterator end(const view_base<T> & x) { return x.end(); }

	template <typename T>
	ful_inline ful_pure constexpr typename view_base<T>::const_pointer data(const view_base<T> & x) { return x.data(); }

	template <typename T>
	ful_inline ful_pure constexpr typename view_base<T>::size_type size(const view_base<T> & x) { return x.size(); }

	template <typename T>
	ful_inline ful_pure constexpr bool empty(const view_base<T> & x) { return x.begin() == x.end(); }

	template <typename T>
	ful_inline void swap(view_base<T> & x, view_base<T> & y)
	{
		x.swap(y);
	}

	template <typename R, typename From>
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> from(const R & x, From from)
	{
		return ful_unused(ful_expect(begin(x) <= from)), ful_unused(ful_expect(from <= end(x))), view_base<hck::range_value_t<R>>(from, end(x));
	}

	template <typename R, typename To>
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> to(const R & x, To to)
	{
		return ful_unused(ful_expect(begin(x) <= to)), ful_unused(ful_expect(to <= end(x))), view_base<hck::range_value_t<R>>(begin(x), to);
	}

	template <typename R>
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> first(const R & x, usize size)
	{
		return ful_unused(ful_expect(size <= static_cast<usize>(end(x) - begin(x)))), view_base<hck::range_value_t<R>>(begin(x), size);
	}

	template <typename R>
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> last(const R & x, usize size)
	{
		return ful_unused(ful_expect(size <= static_cast<usize>(end(x) - begin(x)))), from(x, end(x) - size);
	}

	template <typename R>
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> substr(const R & x, usize offset, usize size)
	{
		return ful_unused(ful_expect(begin(x) + offset + size <= end(x))), view_base<hck::range_value_t<R>>(begin(x) + offset, size);
	}
}
