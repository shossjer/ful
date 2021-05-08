#pragma once

// #include "ful/stdint.hpp"

namespace ful
{
	template <usize W>
	class unsafe_ptr;

	template <typename P>
	P unsafe_cast(unsafe_ptr<sizeof(*static_cast<P>(nullptr))> x);

	template <usize W>
	class unsafe_ptr
	{
	private:

		using this_type = unsafe_ptr<W>;

	private:

		template <typename P>
		friend P unsafe_cast(unsafe_ptr<sizeof(*static_cast<P>(nullptr))> x);

	private:

		puint value_;

	public:

		explicit unsafe_ptr(puint value)
			: value_(value)
		{}

		template <typename T>
		explicit unsafe_ptr(const T * ptr)
			: value_(reinterpret_cast<puint>(ptr))
		{}

		template <usize U>
		explicit unsafe_ptr(unsafe_ptr<U> other)
			: value_(other.value_)
		{}

	public:

		this_type & operator ++ () { value_ += W; return *this; }
		this_type operator ++ (int) { auto tmp = *this; value_ += W; return tmp; }
		this_type & operator -- () { value_ -= W; return *this; }
		this_type operator -- (int) { auto tmp = *this; value_ -= W; return tmp; }

		this_type & operator += (ssize n) { value_ += n * W; return *this; }
		this_type & operator -= (ssize n) { value_ -= n * W; return *this; }

		this_type & operator &= (usize n) { value_ &= n; return *this; }
		this_type & operator |= (usize n) { value_ |= n; return *this; }

		friend this_type operator + (this_type x, ssize n) { return x += n; }
		friend this_type operator + (ssize n, this_type x) { return x += n; }
		friend this_type operator - (this_type x, ssize n) { return x -= n; }

		friend this_type operator & (this_type x, ssize n) { return x &= n; }
		friend this_type operator | (this_type x, ssize n) { return x |= n; }

		friend bool operator == (this_type x, this_type y) { return x.value_ == y.value_; }
		friend bool operator != (this_type x, this_type y) { return !(x == y); }
		friend bool operator < (this_type x, this_type y) { return x.value_ < y.value_; }
		friend bool operator <= (this_type x, this_type y) { return !(y < x); }
		friend bool operator > (this_type x, this_type y) { return y < x; }
		friend bool operator >= (this_type x, this_type y) { return !(x < y); }

		friend pdiff operator - (this_type x, this_type y) { return (x.value_ - y.value_) / W; }
	};

	template <typename P>
	P unsafe_cast(unsafe_ptr<sizeof(*static_cast<P>(nullptr))> x) { return reinterpret_cast<P>(x.value_); }

	template <typename T>
	bool operator == (unsafe_ptr<sizeof(T)> x, const T * y) { return x == unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator == (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) == y; }
	template <typename T>
	bool operator != (unsafe_ptr<sizeof(T)> x, const T * y) { return x != unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator != (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) != y; }
	template <typename T>
	bool operator < (unsafe_ptr<sizeof(T)> x, const T * y) { return x < unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator < (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) < y; }
	template <typename T>
	bool operator <= (unsafe_ptr<sizeof(T)> x, const T * y) { return x <= unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator <= (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) <= y; }
	template <typename T>
	bool operator > (unsafe_ptr<sizeof(T)> x, const T * y) { return x > unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator > (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) > y; }
	template <typename T>
	bool operator >= (unsafe_ptr<sizeof(T)> x, const T * y) { return x >= unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	bool operator >= (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) >= y; }

	template <typename T>
	pdiff operator - (unsafe_ptr<sizeof(T)> x, const T * y) { return x - unsafe_ptr<sizeof(T)>(y); }
	template <typename T>
	pdiff operator - (const T * x, unsafe_ptr<sizeof(T)> y) { return unsafe_ptr<sizeof(T)>(x) - y; }

	template <typename T>
	unsafe_ptr<sizeof(T)> make_unsafe(const T * ptr)
	{
		return unsafe_ptr<sizeof(T)>(ptr);
	}
}
