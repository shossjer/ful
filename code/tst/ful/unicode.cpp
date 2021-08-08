#include "ful/compiler.hpp"
#include "ful/stdhacks.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"

#include "ful/unicode.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

#if HAVE_ICU4C
# include <unicode/ustring.h>
#endif

TEST_CASE("utf8 test data can be read", "")
{
	SECTION("in eng")
	{
		data_eng_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits8() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in got")
	{
		data_got_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits8() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in jap")
	{
		data_jap_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits8() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in rus")
	{
		data_rus_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits8() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}
}

TEST_CASE("utf16le test data can be read", "[common buffer conversion]")
{
	SECTION("in eng")
	{
		data_eng_utf16le data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
		CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
	}

	SECTION("in got")
	{
		data_got_utf16le data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
		CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
	}

	SECTION("in jap")
	{
		data_jap_utf16le data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
		CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
	}

	SECTION("in rus")
	{
		data_rus_utf16le data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
		CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
	}
}

TEST_CASE("utf16be test data can be read", "[common buffer conversion]")
{
	SECTION("in eng")
	{
		data_eng_utf16be data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in got")
	{
		data_got_utf16be data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in jap")
	{
		data_jap_utf16be data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in rus")
	{
		data_rus_utf16be data;
		REQUIRE(data.init());

		CHECK(data.nunits16() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}
}

TEST_CASE("utf32le test data can be read", "[common buffer conversion]")
{
	SECTION("in eng")
	{
		data_eng_utf32le data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in got")
	{
		data_got_utf32le data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in jap")
	{
		data_jap_utf32le data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in rus")
	{
		data_rus_utf32le data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}
}

TEST_CASE("utf32be test data can be read", "[common buffer conversion]")
{
	SECTION("in eng")
	{
		data_eng_utf32be data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in got")
	{
		data_got_utf32be data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in jap")
	{
		data_jap_utf32be data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}

	SECTION("in rus")
	{
		data_rus_utf32be data;
		REQUIRE(data.init());

		CHECK(data.nunits32() == data.size());
		CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
	}
}
