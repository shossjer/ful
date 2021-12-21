#pragma once

#include "ful/viewfwd.hpp"

#include "ful/constexpr.hpp"
#include "ful/string.hpp"

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
		// note end_ does not work with constexpr, but siz_ does!
#if defined(_MSC_VER)
		size_t siz_;
#else
		const_pointer end_;
#endif

	public:

		view_base() = default;

		template <typename Begin, typename End, decltype(to_address(hck::declval<Begin>()), to_address(hck::declval<End>()), int()) = 0>
		ful_inline constexpr explicit view_base(Begin begin, End end) noexcept
			: beg_(to_address(begin))
#if defined(_MSC_VER)
			, siz_(to_address(end) - beg_)
#else
			, end_(to_address(end))
#endif
		{}

		ful_inline constexpr explicit view_base(const_pointer data, usize size) noexcept
			: beg_(data)
#if defined(_MSC_VER)
			, siz_(size)
#else
			, end_(data + size)
#endif
		{}

		view_base(const view_base &) = default;

		template <typename R, decltype(hck::adl_begin(hck::declval<R>()), hck::adl_end(hck::declval<R>()), int()) = 0>
		ful_inline constexpr explicit view_base(const R & x) noexcept
			: view_base(hck::adl_begin(x), hck::adl_end(x))
		{}

		ful_inline void swap(view_base & other) noexcept
		{
			const auto tmp_beg = beg_;
#if defined(_MSC_VER)
			const auto tmp_siz = siz_;
#else
			const auto tmp_end = end_;
#endif

			beg_ = other.beg_;
#if defined(_MSC_VER)
			siz_ = other.siz_;
#else
			end_ = other.end_;
#endif

			other.beg_ = tmp_beg;
#if defined(_MSC_VER)
			other.siz_ = tmp_siz;
#else
			other.end_ = tmp_end;
#endif
		}

	public:

		ful_inline constexpr const_iterator begin() const { return beg_; }
#if defined(_MSC_VER)
		ful_inline constexpr const_iterator end() const { return beg_ + siz_; }
#else
		ful_inline constexpr const_iterator end() const { return end_; }
#endif

		ful_inline constexpr const_pointer data() const { return beg_; }
#if defined(_MSC_VER)
		ful_inline constexpr usize size() const { return siz_; }
#else
		ful_inline constexpr usize size() const { return end_ - beg_; }
#endif

#if defined(_MSC_VER)
		ful_inline ful_pure constexpr friend bool cxp_equals(const view_base & x, const view_base & y) { return cxp::equals(x.beg_, x.beg_ + x.siz_, y.beg_, y.beg_ + y.siz_); }
#else
		ful_inline ful_pure constexpr friend bool cxp_equals(const view_base & x, const view_base & y) { return cxp::equals(x.beg_, x.end_, y.beg_, y.end_); }
#endif

		template <typename Stream>
		friend auto operator << (Stream && stream, const view_base & x)
			-> decltype(stream.write(const_pointer{}, usize{}))
		{
			return stream.write(x.data(), x.size());
		}
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
	ful_inline ful_pure constexpr view_base<hck::range_value_t<R>> drop(const R & x, usize size)
	{
		return ful_unused(ful_expect(size <= static_cast<usize>(end(x) - begin(x)))), from(x, begin(x) + size);
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
