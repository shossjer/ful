#include "ful/cstr.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

TEST_CASE("a cstr of an array", "")
{
	const ful::unit_utf8 buffer[27] = "abcdefghijklmnopqrstuvwxuz";

	SECTION("can be created by construction")
	{
		ful::cstr_utf8 a(buffer);
		CHECK(begin(a) == buffer + 0);
		CHECK(end(a) == buffer + 26);
		CHECK(size(a) == 26);
	}

	SECTION("can be created by from")
	{
		ful::cstr_utf8 a = from(ful::cstr_utf8(buffer), buffer + 11);
		CHECK(begin(a) == buffer + 11);
		CHECK(end(a) == buffer + 26);
		CHECK(size(a) == 15);
	}

	SECTION("decays into view by to")
	{
		ful::view_utf8 a = to(ful::cstr_utf8(buffer), buffer + 15);
		CHECK(begin(a) == buffer + 0);
		CHECK(end(a) == buffer + 15);
		CHECK(size(a) == 15);
	}

	SECTION("decays into view by first")
	{
		ful::view_utf8 a = first(ful::cstr_utf8(buffer), 11);
		CHECK(begin(a) == buffer + 0);
		CHECK(end(a) == buffer + 11);
		CHECK(size(a) == 11);
	}

	SECTION("can be created by last")
	{
		ful::cstr_utf8 a = last(ful::cstr_utf8(buffer), 11);
		CHECK(begin(a) == buffer + 15);
		CHECK(end(a) == buffer + 26);
		CHECK(size(a) == 11);
	}

	SECTION("decays into view by substr")
	{
		ful::view_utf8 a = substr(ful::cstr_utf8(buffer), 11, 4);
		CHECK(begin(a) == buffer + 11);
		CHECK(end(a) == buffer + 15);
		CHECK(size(a) == 4);
	}
}
