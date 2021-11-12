#pragma once

#include "ful/memoryfwd.hpp"

#include "ful/string.hpp"
#include "ful/ranges.hpp"
#include "ful/string_modify.hpp"
#include "ful/string_search.hpp"

#include "ful/container.hpp"

namespace ful
{
	template <typename T, usize Alignment>
	class memory_string_base
	{
	public:

		using value_type = T;

		using iterator = T *;
		using const_iterator = const T *;

		using pointer = T *;
		using const_pointer = const T *;
		using size_type = usize;

		static constexpr const usize alignment = Alignment;

	protected:

		pointer beg_;
		pointer end_;
		pointer cap_;

	public:

		ful_inline explicit memory_string_base(pointer from, pointer to) noexcept
			: beg_(from)
			, end_(from)
			, cap_(to - 1)
		{
			ful_unused(ful_expect(reinterpret_cast<puint>(from) % Alignment == 0));

			if (ful_expect(from != to))
			{
				*from = value_type{};
			}
		}

		ful_inline explicit memory_string_base(pointer from, pointer end, pointer to) noexcept
			: beg_(from)
			, end_(end)
			, cap_(to - 1)
		{
			ful_unused(ful_expect(reinterpret_cast<puint>(from) % Alignment == 0));

			if (ful_expect(from != to))
			{
				*end = value_type{};
			}
		}

		ful_inline explicit memory_string_base(void * ptr, usize size) noexcept
			: beg_(static_cast<pointer>(ptr))
			, end_(static_cast<pointer>(ptr))
			, cap_(reinterpret_cast<pointer>(static_cast<char8 *>(ptr) + size - sizeof(value_type)))
		{
			ful_unused(ful_expect(reinterpret_cast<puint>(ptr) % alignof(value_type) == 0));
			ful_unused(ful_expect(reinterpret_cast<puint>(ptr) % Alignment == 0));

			if (ful_expect(sizeof(value_type) <= size))
			{
				*static_cast<pointer>(ptr) = value_type{};
			}
		}

		memory_string_base(const memory_string_base &) = delete;

		memory_string_base & operator = (const memory_string_base &) = delete;

		ful_inline void swap(memory_string_base & other) noexcept
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
		ful_inline size_type max_size() const { return cap_ - beg_; }

		ful_inline const_pointer c_str() const { return beg_; }

	protected:

		ful_inline bool reserve_reserve(usize size)
		{
			ful_unused(size);

			return false;
		}

		ful_inline iterator replace_reserve(const_iterator from, const_iterator to, const_pointer first, const_pointer last, usize size)
		{
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
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
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
			ful_unused(from);
			ful_unused(to);
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}

		ful_inline iterator append_reserve(const_pointer first, const_pointer last, usize size)
		{
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
			ful_unused(first);
			ful_unused(last);
			ful_unused(size);

			return nullptr;
		}

		template <typename Char>
		ful_inline iterator append_reserve(usize count, Char u, usize size)
		{
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}

		ful_inline iterator assign_reserve(const_pointer first, const_pointer last, usize size)
		{
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
			ful_unused(first);
			ful_unused(last);
			ful_unused(size);

			return nullptr;
		}

		template <typename Char>
		ful_inline iterator assign_reserve(usize count, Char u, usize size)
		{
			const usize max_size_ = max_size();
			ful_unused(ful_expect(size <= max_size_));
			ful_unused(count);
			ful_unused(u);
			ful_unused(size);

			return nullptr;
		}
	};

	template <typename T, usize Alignment>
	ful_inline void shrink_to_fit(memory_string_base<T, Alignment> & x)
	{
		ful_unused(x);
	}
}
