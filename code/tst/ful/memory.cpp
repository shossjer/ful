#include "ful/memory.hpp"
#include "ful/string_compare.hpp"
#include "ful/string_init.hpp"
#include "ful/string_modify.hpp"

#include <catch2/catch.hpp>

#include "container.hpp"

TEST_CASE("an empty memory string", "")
{
	ful::unit_utf8 memorya[111 + 1]; // one extra for null
	ful::memory_string_utf8<1> a(memorya + 0, memorya + sizeof memorya / sizeof memorya[0]);
	CHECK(empty(a));
	CHECK(*data(a) == ful::unit_utf8{});

	SECTION("can swap")
	{
		ful::unit_utf8 memoryb[0 + 1]; // one extra for null
		ful::memory_string_utf8<1> b(memoryb + 0, memoryb + sizeof memoryb / sizeof memoryb[0]);

		swap(a, b);
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		CHECK(empty(b));
		CHECK(*data(b) == ful::unit_utf8{});
	}

	test_empty_container_utf8(a);

	SECTION("can shrink to fit")
	{
		shrink_to_fit(a);
		REQUIRE(empty(a));
		CHECK(data(a)[0] == ful::unit_utf8{});
	}
}

TEST_CASE("a non-empty memory string", "")
{
	ful::unit_utf8 memorya[111 + 1]; // one extra for null
	memorya[0] = ful::unit_utf8{'a'};
	memorya[1] = ful::unit_utf8{'b'};
	memorya[2] = ful::unit_utf8{'c'};
	ful::memory_string_utf8<1> a(memorya + 0, memorya + 2, memorya + sizeof memorya / sizeof memorya[0]);
	REQUIRE(size(a) == 2);
	CHECK(a.data()[0] == ful::unit_utf8{'a'});
	CHECK(a.data()[1] == ful::unit_utf8{'b'});
	CHECK(a.data()[2] == ful::unit_utf8{});

	SECTION("can swap")
	{
		ful::unit_utf8 memoryb[0 + 1]; // one extra for null
		ful::memory_string_utf8<1> b(memoryb + 0, memoryb + sizeof memoryb / sizeof memoryb[0]);

		swap(a, b);
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		REQUIRE(size(b) == 2);
		CHECK(b.data()[0] == ful::unit_utf8{'a'});
		CHECK(b.data()[1] == ful::unit_utf8{'b'});
		CHECK(b.data()[2] == ful::unit_utf8{});
	}

	test_nonempty_container_utf8(a);

	SECTION("can shrink to fit")
	{
		shrink_to_fit(a);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}
}
