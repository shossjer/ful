#pragma once

#include "ful/point.hpp"

#include <cstring>

template <typename S, typename T>
struct data_cache : buffer_base<T>, S
{
	static buffer_base<T> cache;

	bool init()
	{
		if (!cache.data())
		{
			if (!data_cache<S, ful::unit_utf8>::cache.data() && !read_buffer(S::filename(), data_cache<S, ful::unit_utf8>::cache))
				return false;

			if (!convert_buffer(data_cache<S, ful::unit_utf8>::cache, cache))
				return false;
		}

		this->allocate(cache.size());
		::memcpy(this->data(), cache.data(), cache.size() * sizeof(T));
		return true;
	}
};

template <typename S, typename T>
buffer_base<T> data_cache<S, T>::cache;

template <typename S>
struct data_cache<S, ful::unit_utf8> : buffer_utf8, S
{
	static buffer_utf8 cache;

	bool init()
	{
		if (!cache.data() && !read_buffer(S::filename(), cache))
			return false;

		this->allocate(cache.size());
		::memcpy(this->data(), cache.data(), cache.size() * sizeof(ful::unit_utf8));
		return true;
	}
};

template <typename S>
buffer_utf8 data_cache<S, ful::unit_utf8>::cache;

// data set (english) with almost all code points being 1 code unit
struct set_eng
{
	static const char * filename() { return "data/eng.utf8"; }

	static ful::usize npoints() { return 1512211; }
	static ful::usize nunits8() { return 1513685; }
	static ful::usize nunits16() { return 1512211; }
	static ful::usize nunits32() { return 1512211; }
};

struct data_eng_utf8 : data_cache<set_eng, ful::unit_utf8>
{
};

struct data_eng_utf16le : data_cache<set_eng, ful::unit_utf16le>
{
};

struct data_eng_utf16be : data_cache<set_eng, ful::unit_utf16be>
{
};

struct data_eng_utf32le : data_cache<set_eng, ful::unit_utf32le>
{
};

struct data_eng_utf32be : data_cache<set_eng, ful::unit_utf32be>
{
};

// data set (gothic) with some code points being 4 code units
struct set_got
{
	static const char * filename() { return "data/got.utf8"; }

	static ful::usize npoints() { return 458953; }
	static ful::usize nunits8() { return 588607; }
	static ful::usize nunits16() { return 498904; }
	static ful::usize nunits32() { return 458953; }
};

struct data_got_utf8 : data_cache<set_got, ful::unit_utf8>
{
};

struct data_got_utf16le : data_cache<set_got, ful::unit_utf16le>
{
};

struct data_got_utf16be : data_cache<set_got, ful::unit_utf16be>
{
};

struct data_got_utf32le : data_cache<set_got, ful::unit_utf32le>
{
};

struct data_got_utf32be : data_cache<set_got, ful::unit_utf32be>
{
};

// data set (japanese) with most code points being 3 code units
struct set_jap
{
	static const char * filename() { return "data/jap.utf8"; }

	static ful::usize npoints() { return 1930446; }
	static ful::usize nunits8() { return 2724497; }
	static ful::usize nunits16() { return 1930456; }
	static ful::usize nunits32() { return 1930446; }
};

struct data_jap_utf8 : data_cache<set_jap, ful::unit_utf8>
{
	static ful::unit_utf8 cunit() { return '^'; }
	static ful::usize iunit() { return 1966917; }

	static const ful::unit_utf8 * cunits() { return "#cite_ref-1"; }
	static ful::usize iunits() { return 1966904; }

	static ful::point_utf cpoint1() { return ful::point_utf{39}; }
	static ful::usize ipoint1() { return 2650756; }

	static ful::point_utf cpoint2() { return ful::point_utf{946}; }
	static ful::usize ipoint2() { return 1845383; }

	static ful::point_utf cpoint3() { return ful::point_utf{35498}; }
	static ful::usize ipoint3() { return 2718908; }

	static ful::point_utf cpoint4() { return ful::point_utf{134047}; }
	static ful::usize ipoint4() { return 739630; }
};

struct data_jap_utf16le : data_cache<set_jap, ful::unit_utf16le>
{
};

struct data_jap_utf16be : data_cache<set_jap, ful::unit_utf16be>
{
};

struct data_jap_utf32le : data_cache<set_jap, ful::unit_utf32le>
{
};

struct data_jap_utf32be : data_cache<set_jap, ful::unit_utf32be>
{
};

// data set (russian) with many code points being 2 code units
struct set_rus
{
	static const char * filename() { return "data/rus.utf8"; }

	static ful::usize npoints() { return 1154446; }
	static ful::usize nunits8() { return 1610954; }
	static ful::usize nunits16() { return 1154446; }
	static ful::usize nunits32() { return 1154446; }
};

struct data_rus_utf8 : data_cache<set_rus, ful::unit_utf8>
{
};

struct data_rus_utf16le : data_cache<set_rus, ful::unit_utf16le>
{
};

struct data_rus_utf16be : data_cache<set_rus, ful::unit_utf16be>
{
};

struct data_rus_utf32le : data_cache<set_rus, ful::unit_utf32le>
{
};

struct data_rus_utf32be : data_cache<set_rus, ful::unit_utf32be>
{
};

using data_any_utf8 = data_jap_utf8;
using data_any_utf16le = data_jap_utf16le;
using data_any_utf16be = data_jap_utf16be;
using data_any_utf32le = data_jap_utf32le;
using data_any_utf32be = data_jap_utf32be;
