#pragma once

#include "ful/staticfwd.hpp"
#include "ful/stdhacks.hpp"

#include "ful/string.hpp"

#include "ful/container.hpp"

namespace ful
{
	namespace detail
	{
		ful_inline void swap_small(byte * beg1, byte * end1, byte * beg2, byte * end2);
	}

	template <typename T, usize Capacity>
	class static_string_base
	{
		template <typename T_, usize Capacity_>
		friend class static_string_base;

	public:

		using value_type = T;

		using iterator = T *;
		using const_iterator = const T *;

		using pointer = T *;
		using const_pointer = const T *;
		using size_type = usize;

		static constexpr const usize alignment = alignof(pointer);

	protected:

		pointer end_;
		value_type beg_[Capacity + 1]; // one extra for null

	private:

		ful_inline void swap(hck::true_type /*small*/, static_string_base & other)
		{
			const usize size = end_ - beg_;
			const usize other_size = other.end_ - other.beg_;

			detail::swap_small(beg_, beg_ + Capacity, other.beg_, other.beg_ + Capacity);
			end_ = beg_ + other_size;
			other.end_ = other.beg_ + size;

			*end_ = value_type{};
			*other.end_ = value_type{};
		}

		ful_inline void swap(hck::false_type /*small*/, static_string_base & other)
		{
			const usize size = end_ - beg_;
			const usize other_size = other.end_ - other.beg_;

			static_string_base & smaller = size <= other_size ? *this : other;
			static_string_base & bigger = size <= other_size ? other : *this;

			const pointer tmp_end = memswap(smaller.beg_, smaller.end_, bigger.beg_);
			smaller.end_ = memcopy(tmp_end, bigger.end_, smaller.end_);
			bigger.end_ = tmp_end;

			*smaller.end_ = value_type{};
			*bigger.end_ = value_type{};
		}

	public:

		ful_inline static_string_base() noexcept
			: end_(beg_)
		{
			*end_ = value_type{};
		}

		ful_inline static_string_base(const static_string_base & other) noexcept
		{
			ful_assume(static_cast<usize>(other.end_ - other.beg_) <= Capacity);

			end_ = memcopy(other.beg_, other.end_, beg_);

			*end_ = value_type{};
		}

		template <usize OtherCapacity, typename = char[(OtherCapacity < Capacity) * 2 - 1]>
		ful_inline explicit static_string_base(const static_string_base<T, OtherCapacity> & other) noexcept
		{
			ful_assume(static_cast<usize>(other.end_ - other.beg_) <= OtherCapacity);

			end_ = memcopy(other.beg_, other.end_, beg_);

			*end_ = value_type{};
		}

		ful_inline static_string_base & operator = (const static_string_base & other) noexcept
		{
			if (this != &other) // note otherwise memcopy fails
			{
				ful_assume(static_cast<usize>(other.end_ - other.beg_) <= Capacity);

				end_ = memcopy(other.beg_, other.end_, beg_);

				*end_ = value_type{};
			}
			return *this;
		}

		template <usize OtherCapacity, typename = char[(OtherCapacity < Capacity) * 2 - 1]>
		ful_inline static_string_base & operator = (const static_string_base<T, OtherCapacity> & other) noexcept
		{
			ful_assume(static_cast<usize>(other.end_ - other.beg_) <= OtherCapacity);

			end_ = memcopy(other.beg_, other.end_, beg_);

			*end_ = value_type{};

			return *this;
		}

		ful_inline void swap(static_string_base & other) noexcept
		{
			if (this != &other) // note otherwise memswap fails
			{
#if defined(__AVX__)
				swap(hck::bool_type<Capacity <= 64>{}, other);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
				swap(hck::bool_type<Capacity <= 32>{}, other);
#else
				swap(hck::bool_type<Capacity <= 16>{}, other);
#endif
			}
		}

		// todo dangerous to support?
		//template <usize OtherCapacity>
		//ful_inline void swap(static_string_base<T, OtherCapacity> & other) noexcept
		//{
		//	const usize size = end_ - beg_;
		//	const usize other_size = other.end_ - other.beg_;

		//	if (ful_expect(size <= OtherCapacity) && ful_expect(other_size <= Capacity))
		//	{
		//		if (size <= other_size)
		//		{
		//			const pointer tmp_end = memswap(beg_, end_, other.beg_);
		//			end_ = memcopy(tmp_end, other.end_, end_);
		//			other.end_ = tmp_end;

		//			*end_ = value_type{};
		//			*other.end_ = value_type{};
		//		}
		//		else
		//		{
		//			const pointer tmp_end = memswap(other.beg_, other.end_, beg_);
		//			other.end_ = memcopy(tmp_end, end_, other.end_);
		//			end_ = tmp_end;

		//			*other.end_ = value_type{};
		//			*end_ = value_type{};
		//		}
		//	}
		//}

	public:

		ful_inline const_iterator begin() ful_const { return beg_; }
		ful_inline iterator begin() ful_pure { return iterator(beg_); }
		ful_inline const_iterator end() const { return end_; }
		ful_inline iterator end() { return iterator(end_); }

		ful_inline const_pointer data() ful_const { return beg_; }
		ful_inline pointer data() ful_pure { return beg_; }
		ful_inline size_type size() const { return end_ - beg_; }

		ful_inline constexpr size_type capacity() ful_const { return Capacity; }
		ful_inline constexpr size_type max_size() ful_const { return Capacity; }

		ful_inline const_pointer c_str() ful_const { return beg_; }

	protected:

		ful_inline bool reserve_reserve(usize size)
		{
			ful_unused(size);

			return false;
		}

		ful_inline iterator replace_reserve(const_iterator from, const_iterator to, const_pointer first, const_pointer last, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(from);
			ful_unused(to);
			ful_unused(first);
			ful_unused(last);
			ful_unused(size);

			return nullptr;
		}

		template <typename Char>
		ful_inline iterator replace_reserve(const_iterator from, const_iterator to, usize count, Char u, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(from);
			ful_unused(to);
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}

		ful_inline iterator append_reserve(const_pointer first, const_pointer last, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(first);
			ful_unused(last);
			ful_unused(size);

			return nullptr;
		}

		template <typename Char>
		ful_inline iterator append_reserve(usize count, Char u, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}

		ful_inline iterator assign_reserve(const_pointer first, const_pointer last, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(first);
			ful_unused(last);
			ful_unused(size);

			return nullptr;
		}

		template <typename Char>
		ful_inline iterator assign_reserve(usize count, Char u, usize size)
		{
			ful_unused(ful_expect(size <= max_size()));
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}
	};

	template <typename T, usize Capacity>
	ful_inline void shrink_to_fit(static_string_base<T, Capacity> & x)
	{
		ful_unused(x);
	}
}
