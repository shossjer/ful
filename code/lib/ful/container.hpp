#pragma once

namespace ful
{
	template <typename Base>
	class string_container
		: public Base
	{
	public:

		using value_type = typename Base::value_type;

		using iterator = typename Base::iterator;
		using const_iterator = typename Base::const_iterator;

		using pointer = typename Base::pointer;
		using const_pointer = typename Base::const_pointer;
		using size_type = typename Base::size_type;

	public:

		using Base::Base;

		using Base::operator =;

	public:

		ful_inline bool reserve(usize size)
		{
			const usize max_size_ = this->max_size();
			if (!ful_expect(size <= max_size_))
				return false;

			if (this->capacity() < size)
				return this->reserve_reserve(size);

			return true;
		}

		template <typename First, typename Last>
		ful_inline auto insert(const_iterator at, First first, Last last)
			-> decltype(to_address(first), to_address(last), iterator())
		{
			const const_pointer first_ptr = to_address(first);
			const const_pointer last_ptr = to_address(last);

			pointer new_end;
			if (ful_add_overflow(this->end_, last_ptr - first_ptr, new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->replace_reserve(at, at, first_ptr, last_ptr, new_end - this->beg_);
			}
			else
			{
				memmover(at, this->end_, new_end);
				memcopy(first_ptr, last_ptr, const_cast<iterator>(at));

				*new_end = value_type{};

				this->end_ = new_end;

				return const_cast<iterator>(at);
			}
		}

		template <typename Char>
		ful_inline iterator insert(const_iterator at, usize count, Char u)
		{
			pointer new_end;
			if (ful_add_overflow(this->end_, count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type), new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->replace_reserve(at, at, count, u, new_end - this->beg_);
			}
			else
			{
				const pointer pivot = memmover(at, this->end_, new_end);
				memset(const_cast<iterator>(at), pivot, u);

				*new_end = value_type{};

				this->end_ = new_end;

				return const_cast<iterator>(at);
			}
		}

		template <typename First, typename Last>
		ful_inline auto replace(const_iterator from, const_iterator to, First first, Last last)
			-> decltype(to_address(first), to_address(last), iterator())
		{
			const const_pointer first_ptr = to_address(first);
			const const_pointer last_ptr = to_address(last);

			pointer new_end;
			if (ful_add_overflow(const_cast<iterator>(from) + (this->end_ - to), last_ptr - first_ptr, new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->replace_reserve(from, to, first_ptr, last_ptr, new_end - this->beg_);
			}
			else
			{
				memmove(to, this->end_, new_end - (this->end_ - to));
				memcopy(first_ptr, last_ptr, const_cast<iterator>(from));

				*new_end = value_type{};

				this->end_ = new_end;

				return const_cast<iterator>(from);
			}
		}

		template <typename Char>
		ful_inline iterator replace(const_iterator from, const_iterator to, usize count, Char u)
		{
			pointer new_end;
			if (ful_add_overflow(const_cast<iterator>(from) + (this->end_ - to), count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type), new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->replace_reserve(from, to, count, u, new_end - this->beg_);
			}
			else
			{
				memmove(to, this->end_, new_end - (this->end_ - to));
				memset(const_cast<iterator>(from), const_cast<iterator>(from) + count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type), u);

				*new_end = value_type{};

				this->end_ = new_end;

				return const_cast<iterator>(from);
			}
		}

		ful_inline void reduce(const_iterator from)
		{
			const pointer new_end = const_cast<iterator>(from);

			*new_end = value_type{};

			this->end_ = new_end;
		}

		// note strictly not necessary since this is equivalent to
		// `insert(end_, first, last)` but the compilers have some trouble
		// optimizing that
		template <typename First, typename Last>
		ful_inline auto append(First first, Last last)
			-> decltype(to_address(first), to_address(last), iterator())
		{
			const const_pointer first_ptr = to_address(first);
			const const_pointer last_ptr = to_address(last);

			pointer new_end;
			if (ful_add_overflow(this->end_, last_ptr - first_ptr, new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->append_reserve(first_ptr, last_ptr, new_end - this->beg_);
			}
			else
			{
				const iterator pivot = this->end_;

				memcopy(first_ptr, last_ptr, pivot);

				*new_end = value_type{};

				this->end_ = new_end;

				return pivot;
			}
		}

		// note strictly not necessary since this is equivalent to
		// `insert(end_, first, last)` but the compilers have some trouble
		// optimizing that
		template <typename Char>
		ful_inline iterator append(usize count, Char u)
		{
			pointer new_end;
			if (ful_add_overflow(this->end_, count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type), new_end) || this->beg_ + this->capacity() < new_end)
			{
				return this->append_reserve(count, u, new_end - this->beg_);
			}
			else
			{
				const iterator pivot = this->end_;

				memset(pivot, new_end, u);

				*new_end = value_type{};

				this->end_ = new_end;

				return pivot;
			}
		}

		// note strictly not necessary since this is equivalent to
		// `replace(it, it + 1, nullptr, nullptr)` but the compilers have some
		// trouble optimizing that
		ful_inline void erase(const_iterator at)
		{
			if (!ful_expect(at < this->end_))
				return;

			const pointer new_end = memmovef(at + 1, this->end_, const_cast<iterator>(at));

			*new_end = value_type{};

			this->end_ = new_end;
		}

		// note strictly not necessary since this is equivalent to
		// `replace(from, to, nullptr, nullptr)` but the compilers have some
		// trouble optimizing that
		ful_inline void erase(const_iterator from, const_iterator to)
		{
			const pointer new_end = memmovef(to, this->end_, const_cast<iterator>(from));

			*new_end = value_type{};

			this->end_ = new_end;
		}

		// note strictly not necessary since this is equivalent to
		// `replace(begin, end, first, last)` but the compilers have some
		// trouble optimizing that
		template <typename First, typename Last>
		ful_inline auto assign(First first, Last last)
			-> decltype(to_address(first), to_address(last), iterator())
		{
			const const_pointer first_ptr = to_address(first);
			const const_pointer last_ptr = to_address(last);

			if (this->capacity() < static_cast<usize>(last_ptr - first_ptr))
			{
				return this->assign_reserve(first_ptr, last_ptr, static_cast<usize>(last_ptr - first_ptr));
			}
			else
			{
				const pointer new_end = memcopy(first_ptr, last_ptr, this->beg_);

				*new_end = value_type{};

				this->end_ = new_end;

				return this->beg_;
			}
		}

		// note strictly not necessary since this is equivalent to
		// `replace(begin, end, first, last)` but the compilers have some
		// trouble optimizing that
		template <typename Char>
		ful_inline iterator assign(usize count, Char u)
		{
			if (this->capacity() < count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type))
			{
				return this->assign_reserve(count, u, count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type));
			}
			else
			{
				const pointer new_end = this->beg_ + count * sizeof(hck::compact_type_t<Char>) / sizeof(value_type); memset(this->beg_, new_end, u);

				*new_end = value_type{};

				this->end_ = new_end;

				return this->beg_;
			}
		}

		ful_inline ful_pure friend bool operator == (const string_container & x, const string_container & y) { return equal(reinterpret_cast<const byte *>(x.beg_), reinterpret_cast<const byte *>(x.end_), reinterpret_cast<const byte *>(y.beg_), reinterpret_cast<const byte *>(y.end_)); }
		ful_inline ful_pure friend bool operator == (const string_container & x, const_pointer s) { return equal(reinterpret_cast<const byte *>(x.beg_), reinterpret_cast<const byte *>(x.end_), reinterpret_cast<const byte *>(s)); }
		ful_inline ful_pure friend bool operator == (const_pointer s, const string_container & x) { return equal(reinterpret_cast<const byte *>(x.beg_), reinterpret_cast<const byte *>(x.end_), reinterpret_cast<const byte *>(s)); }
		ful_inline ful_pure friend bool operator != (const string_container & x, const string_container & y) { return !(x == y); }
		ful_inline ful_pure friend bool operator != (const string_container & x, const_pointer s) { return !(x == s); }
		ful_inline ful_pure friend bool operator != (const_pointer s, const string_container & x) { return !(x == s); }
	};

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::const_iterator begin(const string_container<Base> & x) { return x.begin(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::iterator begin(string_container<Base> & x) { return x.begin(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::const_iterator end(const string_container<Base> & x) { return x.end(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::iterator end(string_container<Base> & x) { return x.end(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::const_pointer data(const string_container<Base> & x) { return x.data(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::pointer data(string_container<Base> & x) { return x.data(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::size_type size(const string_container<Base> & x) { return x.size(); }

	template <typename Base>
	ful_inline ful_pure constexpr typename string_container<Base>::size_type max_size(const string_container<Base> & x) { return x.max_size(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::size_type capacity(const string_container<Base> & x) { return x.capacity(); }

	template <typename Base>
	ful_inline ful_pure typename string_container<Base>::const_pointer c_str(const string_container<Base> & x) { return x.c_str(); }

	template <typename Base>
	ful_inline ful_pure bool empty(const string_container<Base> & x) { return x.begin() == x.end(); }

	template <typename Base>
	ful_inline void swap(string_container<Base> & x, string_container<Base> & y)
	{
		x.swap(y);
	}

	template <typename Base>
	ful_inline bool reserve(string_container<Base> & x, usize size)
	{
		return x.reserve(size);
	}

	template <typename Base>
	ful_inline bool resize(string_container<Base> & x, usize size)
	{
		if (x.reserve(size))
		{
			x.reduce(x.begin() + size); // note hack

			return true;
		}
		return false;
	}

	// return element, or nullptr if allocation fails
	template <typename Base, typename Char>
	ful_inline auto push_back(string_container<Base> & x, Char u)
		-> decltype(memset(x.begin(), x.end(), u), typename string_container<Base>::iterator())
	{
		return x.append(1, u);
	}

	template <typename Base>
	ful_inline void pop_back(string_container<Base> & x)
	{
		x.reduce(x.end() - 1);
	}

	// return element, or nullptr if allocation fails
	template <typename Base, typename First, typename Last>
	ful_inline typename string_container<Base>::iterator append(string_container<Base> & x, First first, Last last)
	{
		return x.append(first, last);
	}

	// return element, or nullptr if allocation fails
	template <typename Base, typename Char>
	ful_inline auto append(string_container<Base> & x, usize count, Char u)
		-> decltype(memset(x.begin(), x.end(), u), typename string_container<Base>::iterator())
	{
		return x.append(count, u);
	}

	// return (possibly relocated) it, or nullptr if allocation fails
	template <typename Base, typename First, typename Last>
	ful_inline typename string_container<Base>::iterator insert(string_container<Base> & x, typename string_container<Base>::const_iterator at, First first, Last last)
	{
		return x.insert(at, first, last);
	}

	// return (possibly relocated) it, or nullptr if allocation fails
	template <typename Base, typename Char>
	ful_inline auto insert(string_container<Base> & x, typename string_container<Base>::const_iterator at, usize count, Char u)
		-> decltype(memset(x.begin(), x.end(), u), typename string_container<Base>::iterator())
	{
		return x.insert(at, count, u);
	}

	// return begin, or nullptr if allocation fails
	template <typename Base, typename First, typename Last>
	ful_inline typename string_container<Base>::iterator assign(string_container<Base> & x, First first, Last last)
	{
		return x.assign(first, last);
	}

	// return begin, or nullptr if allocation fails
	template <typename Base, typename Char>
	ful_inline auto assign(string_container<Base> & x, usize count, Char u)
		-> decltype(memset(x.begin(), x.end(), u), typename string_container<Base>::iterator())
	{
		return x.assign(count, u);
	}

	// return (possibly relocated) from, or nullptr if allocation fails
	template <typename Base, typename First, typename Last>
	ful_inline typename string_container<Base>::iterator replace(string_container<Base> & x, typename string_container<Base>::const_iterator from, typename string_container<Base>::const_iterator to, First first, Last last)
	{
		return x.replace(from, to, first, last);
	}

	// return (possibly relocated) from, or nullptr if allocation fails
	template <typename Base, typename Char>
	ful_inline auto replace(string_container<Base> & x, typename string_container<Base>::const_iterator from, typename string_container<Base>::const_iterator to, usize count, Char u)
		-> decltype(memset(x.begin(), x.end(), u), typename string_container<Base>::iterator())
	{
		return x.replace(from, to, count, u);
	}

	template <typename Base>
	ful_inline void clear(string_container<Base> & x)
	{
		x.reduce(x.begin());
	}

	template <typename Base>
	ful_inline void reduce(string_container<Base> & x, typename string_container<Base>::const_iterator from)
	{
		x.reduce(from);
	}

	template <typename Base>
	ful_inline void erase(string_container<Base> & x, typename string_container<Base>::const_iterator at)
	{
		x.erase(at);
	}

	template <typename Base>
	ful_inline void erase(string_container<Base> & x, typename string_container<Base>::const_iterator from, typename string_container<Base>::const_iterator to)
	{
		x.erase(from, to);
	}

	// return end of copy, or nullptr if allocation fails
	template <typename First, typename Last, typename Base>
	ful_inline auto copy(First first, Last last, string_container<Base> & x)
		-> decltype(x.assign(first, last))
	{
		const auto it = x.assign(first, last);
		return it == nullptr ? nullptr : x.end();
	}

	//template <typename Base>
	//ful_inline auto valid(const string_container<Base> & x)
	//	-> decltype(valid(x.begin(), x.end()))
	//{
	//	return valid(x.begin(), x.end());
	//}

	//template <typename Base>
	//ful_inline auto valid(string_container<Base> & x)
	//	-> decltype(valid(x.begin(), x.end()))
	//{
	//	return valid(x.begin(), x.end());
	//}
}
