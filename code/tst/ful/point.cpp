#include "ful/point.hpp"
#include "ful/static.hpp"
#include "ful/string_init.hpp"
#include "ful/string_search.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

TEST_CASE("point decode/encode", "")
{
	data_any_utf8 data;
	REQUIRE(data.init());

	for (const ful::unit_utf8 * ptr = data.beg(); ptr != data.end();)
	{
		ful::point_utf p;
		ful::point_buffer<ful::unit_utf8, 1> buffer;

		const ful::unit_utf8 * step = decode(ptr, p);
		const ful::unit_utf8 * it = encode(p, buffer);

		REQUIRE(it - buffer == step - ptr);
		REQUIRE(it - buffer >= 1);
		REQUIRE(it - buffer <= 4);

		switch (it - buffer)
		{
		case 4: REQUIRE(buffer[3] == ptr[3]); ful_fallthrough;
		case 3: REQUIRE(buffer[2] == ptr[2]); ful_fallthrough;
		case 2: REQUIRE(buffer[1] == ptr[1]); ful_fallthrough;
		case 1: REQUIRE(buffer[0] == ptr[0]); break;
		default: ful_unreachable();
		}

		ptr = step;
	}
}

TEST_CASE("point find", "")
{
	data_any_utf8 data;
	REQUIRE(data.init());

	SECTION("point 1")
	{
		CHECK(find(data.beg(), data.end(), data.cpoint1()) - data.beg() == data.ipoint1());
		CHECK(find(data.beg(), data.end(), data.cpoint1()) - data.beg() == data.ipoint1());
	}

	SECTION("point 2")
	{
		CHECK(find(data.beg(), data.end(), data.cpoint2()) - data.beg() == data.ipoint2());
		CHECK(find(data.beg(), data.end(), data.cpoint2()) - data.beg() == data.ipoint2());
	}

	SECTION("point 3")
	{
		CHECK(find(data.beg(), data.end(), data.cpoint3()) - data.beg() == data.ipoint3());
		CHECK(find(data.beg(), data.end(), data.cpoint3()) - data.beg() == data.ipoint3());
	}

	SECTION("point 4")
	{
		CHECK(find(data.beg(), data.end(), data.cpoint4()) - data.beg() == data.ipoint4());
		CHECK(find(data.beg(), data.end(), data.cpoint4()) - data.beg() == data.ipoint4());
	}
}

TEST_CASE("point push_back", "")
{
	data_any_utf8 data;

	ful::static_string_utf8<10> str;

	const auto it1 = push_back(str, data.cpoint1());
	CHECK(it1 == str.begin());
	const auto it2 = push_back(str, data.cpoint2());
	CHECK(it2 == str.begin() + 1);
	const auto it3 = push_back(str, data.cpoint3());
	CHECK(it3 == str.begin() + 1 + 2);
	const auto it4 = push_back(str, data.cpoint4());
	CHECK(it4 == str.begin() + 1 + 2 + 3);
	CHECK(str.size() == 10);
}

TEST_CASE("point append", "")
{
	data_any_utf8 data;

	ful::static_string_utf8<10> str;

	const auto it1 = append(str, 1, data.cpoint1());
	CHECK(it1 == str.begin());
	const auto it2 = append(str, 1, data.cpoint2());
	CHECK(it2 == str.begin() + 1);
	const auto it3 = append(str, 1, data.cpoint3());
	CHECK(it3 == str.begin() + 1 + 2);
	const auto it4 = append(str, 1, data.cpoint4());
	CHECK(it4 == str.begin() + 1 + 2 + 3);
	CHECK(str.size() == 10);
}
