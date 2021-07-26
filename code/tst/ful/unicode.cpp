#include "ful/compiler.hpp"
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

TEST_CASE("data", "")
{
	SECTION("eng")
	{
		SECTION("utf8")
		{
			data_eng_utf8 data;
			REQUIRE(data.init());

			CHECK(data.nunits8() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf16le")
		{
			data_eng_utf16le data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
			CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
		}

		SECTION("utf16be")
		{
			data_eng_utf16be data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32le")
		{
			data_eng_utf32le data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32be")
		{
			data_eng_utf32be data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}
	}

	SECTION("got")
	{
		SECTION("utf8")
		{
			data_got_utf8 data;
			REQUIRE(data.init());

			CHECK(data.nunits8() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf16le")
		{
			data_got_utf16le data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
			CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
		}

		SECTION("utf16be")
		{
			data_got_utf16be data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32le")
		{
			data_got_utf32le data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32be")
		{
			data_got_utf32be data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}
	}

	SECTION("jap")
	{
		SECTION("utf8")
		{
			data_jap_utf8 data;
			REQUIRE(data.init());

			CHECK(data.nunits8() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf16le")
		{
			data_jap_utf16le data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
			CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
		}

		SECTION("utf16be")
		{
			data_jap_utf16be data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32le")
		{
			data_jap_utf32le data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32be")
		{
			data_jap_utf32be data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}
	}

	SECTION("rus")
	{
		SECTION("utf8")
		{
			data_rus_utf8 data;
			REQUIRE(data.init());

			CHECK(data.nunits8() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf16le")
		{
			data_rus_utf16le data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
#if HAVE_ICU4C
			CHECK(data.npoints() == ::u_countChar32(reinterpret_cast<UChar *>(data.data()), static_cast<int32_t>(data.size())));
#endif
		}

		SECTION("utf16be")
		{
			data_rus_utf16be data;
			REQUIRE(data.init());

			CHECK(data.nunits16() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32le")
		{
			data_rus_utf32le data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}

		SECTION("utf32be")
		{
			data_rus_utf32be data;
			REQUIRE(data.init());

			CHECK(data.nunits32() == data.size());
			CHECK(data.npoints() == ful::point_count(data.beg(), data.end()));
		}
	}
}
