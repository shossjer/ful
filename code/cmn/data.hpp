#pragma once

// data set (english) with almost all code points being 1 code unit
struct data_eng_utf8 : buffer_utf8
{
	bool init() { return read_buffer("data/eng.html", *this); }

	constexpr static ful::usize nunits() { return 1513685; }
	constexpr static ful::usize npoints() { return 1512211; }

	constexpr static ful::usize nunits16() { return 1512211; }
};

// data set (gothic) with some code points being 4 code units
struct data_got_utf8 : buffer_utf8
{
	bool init() { return read_buffer("data/got.html", *this); }

	constexpr static ful::usize nunits() { return 588607; }
	constexpr static ful::usize npoints() { return 458953; }

	constexpr static ful::usize nunits16() { return 498904; }
};

// data set (japanese) with most code points being 3 code units
struct data_jap_utf8 : buffer_utf8
{
	bool init() { return read_buffer("data/jap.html", *this); }

	constexpr static ful::usize nunits() { return 2724497; }
	constexpr static ful::usize npoints() { return 1930446; }

	constexpr static ful::usize nunits16() { return 1930456; }

	constexpr static ful::unit_utf8 cunit() { return '^'; }
	constexpr static ful::usize iunit() { return 1966917; }
};

// data set (russian) with many code points being 2 code units
struct data_rus_utf8 : buffer_utf8
{
	bool init() { return read_buffer("data/rus.html", *this); }

	constexpr static ful::usize nunits() { return 1610954; }
	constexpr static ful::usize npoints() { return 1154446; }

	constexpr static ful::usize nunits16() { return 1154446; }
};

using data_any_utf8 = data_jap_utf8;
