#pragma once

#include "ful/types.hpp"

template <typename T>
class buffer_base
{
private:

	T * beg_;
	T * end_;
	T * cap_;

public:

	~buffer_base()
	{
		if (beg_)
		{
			::operator delete(beg_);
		}
	}

	buffer_base()
		: beg_(nullptr)
		, end_(nullptr)
		, cap_(nullptr)
	{}

	buffer_base(buffer_base && other) noexcept
		: beg_(other.beg_)
		, end_(other.end_)
		, cap_(other.cap_)
	{
		other.beg_ = nullptr;
		other.end_ = nullptr;
		other.cap_ = nullptr;
	}

	buffer_base & operator = (buffer_base && other) noexcept
	{
		if (beg_)
		{
			::operator delete(beg_);
		}
		beg_ = other.beg_;
		end_ = other.end_;
		cap_ = other.cap_;
		other.beg_ = nullptr;
		other.end_ = nullptr;
		other.cap_ = nullptr;

		return *this;
	}

public:

	T * beg() { return beg_; }
	const T * beg() const { return beg_; }
	T * end() { return end_; }
	const T * end() const { return end_; }

	const T * cstr() const { return beg_; }

	T * data() { return beg_; }
	const T * data() const { return beg_; }
	unsigned long long size() const { return end_ - beg_; }
	unsigned long long capacity() const { return cap_ - beg_; }

	void allocate(unsigned long long size)
	{
		if (!ful_expect(beg_ == nullptr))
			return;

		void * const ptr = ::operator new((size + 1) * sizeof(T)); // one extra for null
		beg_ = static_cast<T *>(ptr);
		end_ = static_cast<T *>(ptr) + size;
		cap_ = static_cast<T *>(ptr) + size + 1;
		*end_ = static_cast<T>('\0');
	}

	void reduce(const T * end)
	{
		if (!ful_expect(beg_ <= end && end < end_))
			return;

		*const_cast<T *>(end) = static_cast<T>('\0');
		end_ = const_cast<T *>(end);
	}
};

using buffer_utf8 = buffer_base<ful::unit_utf8>;
using buffer_utf16 = buffer_base<ful::unit_utf16>;
using buffer_utf16le = buffer_base<ful::unit_utf16le>;
using buffer_utf16be = buffer_base<ful::unit_utf16be>;
using buffer_utf32 = buffer_base<ful::unit_utf32>;
using buffer_utf32le = buffer_base<ful::unit_utf32le>;
using buffer_utf32be = buffer_base<ful::unit_utf32be>;

bool read_buffer(const char * fname, buffer_utf8 & buffer);

bool convert_buffer(const buffer_utf8 & src, buffer_utf16le & dst);
bool convert_buffer(const buffer_utf8 & src, buffer_utf16be & dst);
bool convert_buffer(const buffer_utf8 & src, buffer_utf32le & dst);
bool convert_buffer(const buffer_utf8 & src, buffer_utf32be & dst);
