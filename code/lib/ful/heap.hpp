#pragma once

#include "ful/heapfwd.hpp"
#include "ful/stdhacks.hpp"

#include "ful/string.hpp"

#include "ful/container.hpp"

// todo malloc without stdlib?
// * windows has HeapAlloc/HeapFree, performance is unknown
#include <cstdlib>

namespace ful
{
	namespace detail
	{
		// todo thread safe without atomics?
		template <typename T>
		ful_inline T * heap_string_null()
		{
			extern char64 heap_string_null_value;

			return reinterpret_cast<T *>(&heap_string_null_value);
		}
	}

	template <typename T>
	class heap_string_base
	{
	public:

		using value_type = T;

		using iterator = T *;
		using const_iterator = const T *;

		using pointer = T *;
		using const_pointer = const T *;
		using size_type = usize;

		static constexpr const usize alignment = alignof(long double);

	protected:

		pointer beg_;
		pointer end_;
		pointer cap_;

	private:

		ful_inline static void * allocate(usize size)
		{
			if (!ful_expect(1u < size))
				return nullptr;

			void * const ptr = ::malloc(size);
			return ptr;
		}

		ful_inline static void deallocate(void * ptr, usize size)
		{
			::free(ptr);
			static_cast<void>(size);
		}

		ful_inline void maybe_purge()
		{
			if (beg_ != detail::heap_string_null<T>())
			{
				deallocate(beg_, (cap_ - beg_ + 1) * sizeof(value_type)); // one extra for null
			}
		}

	public:

		ful_inline ~heap_string_base()
		{
			maybe_purge();
		}

		ful_inline heap_string_base() noexcept
			: beg_(detail::heap_string_null<T>())
			, end_(detail::heap_string_null<T>())
			, cap_(detail::heap_string_null<T>())
		{}

		ful_inline heap_string_base(heap_string_base && other) noexcept
			: beg_(other.beg_)
			, end_(other.end_)
			, cap_(other.cap_)
		{
			other.beg_ = detail::heap_string_null<T>();
			other.end_ = detail::heap_string_null<T>();
			other.cap_ = detail::heap_string_null<T>();
		}

		ful_inline heap_string_base & operator = (heap_string_base && other) noexcept
		{
			maybe_purge();

			beg_ = other.beg_;
			end_ = other.end_;
			cap_ = other.cap_;

			other.beg_ = detail::heap_string_null<T>();
			other.end_ = detail::heap_string_null<T>();
			other.cap_ = detail::heap_string_null<T>();

			return *this;
		}

		ful_inline void swap(heap_string_base & other) noexcept
		{
			const auto tmp_beg = beg_;
			const auto tmp_end = end_;
			const auto tmp_cap = cap_;

			beg_ = other.beg_;
			end_ = other.end_;
			cap_ = other.cap_;

			other.beg_ = tmp_beg;
			other.end_ = tmp_end;
			other.cap_ = tmp_cap;
		}

	public:

		ful_inline const_iterator begin() const { return beg_; }
		ful_inline iterator begin() { return beg_; }
		ful_inline const_iterator end() const { return end_; }
		ful_inline iterator end() { return end_; }

		ful_inline const_pointer data() const { return beg_; }
		ful_inline pointer data() { return beg_; }
		ful_inline size_type size() const { return end_ - beg_; }

		ful_inline size_type capacity() const { return cap_ - beg_; }
		ful_inline constexpr size_type max_size() ful_const { return static_cast<usize>(-1) / sizeof(value_type); }

		ful_inline const_pointer c_str() const { return beg_; }
		ful_inline pointer c_str() { return beg_; }

	protected:

		bool reserve_reserve(usize size) noexcept
		{
			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return false;

			ful_assume((reinterpret_cast<puint>(beg_) & (8 - 1)) == 0);
			ful_assume((reinterpret_cast<puint>(new_beg) & (8 - 1)) == 0);

			const pointer new_end = memcopy(beg_, end_, new_beg);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return true;
		}

		iterator replace_reserve(const_iterator from, const_iterator to, const_pointer first, const_pointer last, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			ful_assume((reinterpret_cast<puint>(beg_) & (8 - 1)) == 0);
			ful_assume((reinterpret_cast<puint>(new_beg) & (8 - 1)) == 0);

			const pointer space = memcopy(beg_, from, new_beg);
			const pointer pivot = memcopy(first, last, space);
			const pointer new_end = memcopy(to, end_, pivot);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return space;
		}

		template <typename Char>
		iterator replace_reserve(const_iterator from, const_iterator to, usize count, Char u, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			ful_assume((reinterpret_cast<puint>(beg_) & (8 - 1)) == 0);
			ful_assume((reinterpret_cast<puint>(new_beg) & (8 - 1)) == 0);

			const pointer space = memcopy(beg_, from, new_beg);
			const pointer pivot = space + count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type); memset(space, pivot, u);
			const pointer new_end = memcopy(to, end_, pivot);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return space;
		}

		iterator append_reserve(const_pointer first, const_pointer last, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			ful_assume((reinterpret_cast<puint>(beg_) & (8 - 1)) == 0);
			ful_assume((reinterpret_cast<puint>(new_beg) & (8 - 1)) == 0);

			const pointer pivot = memcopy(beg_, end_, new_beg);
			const pointer new_end = memcopy(first, last, pivot);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return pivot;
		}

		template <typename Char>
		iterator append_reserve(usize count, Char u, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			ful_assume((reinterpret_cast<puint>(beg_) & (8 - 1)) == 0);
			ful_assume((reinterpret_cast<puint>(new_beg) & (8 - 1)) == 0);

			const pointer pivot = memcopy(beg_, end_, new_beg);
			const pointer new_end = pivot + count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type); memset(pivot, new_end, u);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return pivot;
		}

		iterator assign_reserve(const_pointer first, const_pointer last, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			const pointer new_end = memcopy(first, last, new_beg);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return beg_;
		}

		template <typename Char>
		iterator assign_reserve(usize count, Char u, usize size) noexcept
		{
			if (!ful_expect(size <= max_size()))
				return nullptr;

			const pointer new_beg = static_cast<pointer>(allocate((size + 1) * sizeof(value_type))); // one extra for null
			if (!ful_check(new_beg))
				return nullptr;

			const pointer new_end = new_beg + count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type); memset(new_beg, new_end, u);
			const pointer new_cap = new_beg + size;

			*new_end = value_type{};

			maybe_purge();

			beg_ = new_beg;
			end_ = new_end;
			cap_ = new_cap;

			return beg_;
		}
	};

	template <typename T>
	ful_inline void shrink_to_fit(heap_string<T> & x)
	{
		heap_string<T> y;

		const auto it = y.assign(x.begin(), x.end());
		if (it != y.end())
		{
			x.swap(y);
		}
	}
}
