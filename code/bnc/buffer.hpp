#include "ful/unicode.hpp"

class buffer_utf8
{
private:

	ful::unit_utf8 * beg_;
	ful::unit_utf8 * end_;

public:

	~buffer_utf8()
	{
		if (beg_)
		{
			::operator delete(beg_, end_ - beg_ + 1);
		}
	}

	buffer_utf8()
		: beg_(nullptr)
		, end_(nullptr)
	{}

	buffer_utf8(buffer_utf8 && other) noexcept
		: beg_(other.beg_)
		, end_(other.end_)
	{
		other.beg_ = nullptr;
		other.end_ = nullptr;
	}

	buffer_utf8 & operator = (buffer_utf8 && other) noexcept
	{
		if (beg_)
		{
			::operator delete(beg_, end_ - beg_ + 1);
		}
		beg_ = other.beg_;
		end_ = other.end_;
		other.beg_ = nullptr;
		other.end_ = nullptr;

		return *this;
	}

public:

	const ful::unit_utf8 * beg() const { return beg_; }
	const ful::unit_utf8 * end() const { return end_; }

	const ful::unit_utf8 * cstr() const { return beg_; }

	ful::unit_utf8 * data() { return beg_; }
	const ful::unit_utf8 * data() const { return beg_; }
	unsigned long long size() const { return end_ - beg_; }

	void resize(unsigned long long size)
	{
		void * const ptr = ::operator new(size + 1); // one extra for null
		beg_ = static_cast<ful::unit_utf8 *>(ptr);
		end_ = static_cast<ful::unit_utf8 *>(ptr) + size;
		*end_ = '\0';
	}
};

buffer_utf8 read_utf8(const char * fname);
