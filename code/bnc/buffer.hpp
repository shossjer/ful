#include "ful/unicode.hpp"

class buffer_utf8
{
private:

	ful::unit_utf8 * beg_;
	ful::unit_utf8 * end_;
	ful::unit_utf8 * cap_;

public:

	~buffer_utf8()
	{
		if (beg_)
		{
			::operator delete(beg_, cap_ - beg_);
		}
	}

	buffer_utf8()
		: beg_(nullptr)
		, end_(nullptr)
		, cap_(nullptr)
	{}

	buffer_utf8(buffer_utf8 && other) noexcept
		: beg_(other.beg_)
		, end_(other.end_)
		, cap_(other.cap_)
	{
		other.beg_ = nullptr;
		other.end_ = nullptr;
		other.cap_ = nullptr;
	}

	buffer_utf8 & operator = (buffer_utf8 && other) noexcept
	{
		if (beg_)
		{
			::operator delete(beg_, cap_ - beg_);
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

	const ful::unit_utf8 * beg() const { return beg_; }
	const ful::unit_utf8 * end() const { return end_; }
	// const ful::unit_utf8 * cap() const { return cap_; }

	const ful::unit_utf8 * cstr() const { return beg_; }

	ful::unit_utf8 * data() { return beg_; }
	const ful::unit_utf8 * data() const { return beg_; }
	unsigned long long size() const { return end_ - beg_; }
	unsigned long long capacity() const { return cap_ - beg_; }

	void allocate(unsigned long long size)
	{
		// assume beg_ == nullptr
		void * const ptr = ::operator new(size + 1); // one extra for null
		beg_ = static_cast<ful::unit_utf8 *>(ptr);
		end_ = static_cast<ful::unit_utf8 *>(ptr) + size;
		cap_ = static_cast<ful::unit_utf8 *>(ptr) + size + 1;
		*end_ = '\0';
	}

	void reduce(const ful::unit_utf8 * end)
	{
		// assume beg_ <= end <= end_
		*const_cast<ful::unit_utf8 *>(end) = '\0';
		end_ = const_cast<ful::unit_utf8 *>(end);
	}
};

buffer_utf8 read_utf8(const char * fname);
