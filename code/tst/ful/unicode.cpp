#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"
#include "ful/point_utils.hpp"

#include "ful/unicode.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

using namespace ful;

TEST_CASE("data", "")
{
	SECTION("eng")
	{
		data_eng_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits() == data.size());
		CHECK(data.npoints() == point_count(data.beg(), data.end()));

		SECTION("")
		{
			buffer_utf16 buff;
			buff.allocate(data.size() * 2);

			CHECK(data.nunits16() == convert(data.beg(), data.end(), buff.beg(), buff.end()) - buff.beg());
		}
	}

	SECTION("got")
	{
		data_got_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits() == data.size());
		CHECK(data.npoints() == point_count(data.beg(), data.end()));

		SECTION("")
		{
			buffer_utf16 buff;
			buff.allocate(data.size() * 2);

			CHECK(data.nunits16() == convert(data.beg(), data.end(), buff.beg(), buff.end()) - buff.beg());
		}
	}

	SECTION("jap")
	{
		data_jap_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits() == data.size());
		CHECK(data.npoints() == point_count(data.beg(), data.end()));

		SECTION("")
		{
			buffer_utf16 buff;
			buff.allocate(data.size() * 2);

			CHECK(data.nunits16() == convert(data.beg(), data.end(), buff.beg(), buff.end()) - buff.beg());
		}
	}

	SECTION("rus")
	{
		data_rus_utf8 data;
		REQUIRE(data.init());

		CHECK(data.nunits() == data.size());
		CHECK(data.npoints() == point_count(data.beg(), data.end()));

		SECTION("")
		{
			buffer_utf16 buff;
			buff.allocate(data.size() * 2);

			CHECK(data.nunits16() == convert(data.beg(), data.end(), buff.beg(), buff.end()) - buff.beg());
		}
	}
}
