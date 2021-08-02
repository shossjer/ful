#include "ful/static.hpp"

#include <catch2/catch.hpp>

#include "container.hpp"

TEST_CASE("an empty static string", "")
{
	ful::static_string_utf8<111> a;
	CHECK(empty(a));
	CHECK(*data(a) == ful::unit_utf8{});

	SECTION("can copy")
	{
		ful::static_string_utf8<111> b = a;
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		CHECK(empty(b));
		CHECK(*data(b) == ful::unit_utf8{});

		a = b;
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		CHECK(empty(b));
		CHECK(*data(b) == ful::unit_utf8{});
	}

	SECTION("can copy small")
	{
		ful::static_string_utf8<123> b(a);
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		CHECK(empty(b));
		CHECK(*data(b) == ful::unit_utf8{});

		// todo clear b

		b = a;
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
		CHECK(empty(b));
		CHECK(*data(b) == ful::unit_utf8{});
	}

	SECTION("can self-copy")
	{
		a = static_cast<ful::static_string_utf8<111> &&>(a);
		CHECK(empty(a));
		CHECK(*data(a) == ful::unit_utf8{});
	}

	SECTION("can swap")
	{
		ful::static_string_utf8<111> b;

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

TEST_CASE("a non-empty static string", "")
{
	ful::static_string_utf8<111> a;
	const auto it = assign(a, 1, static_cast<ful::char16>(0x6261)); // 'ab'
	CHECK(it == a.begin());
	REQUIRE(size(a) == 2);
	CHECK(a.data()[0] == ful::unit_utf8{'a'});
	CHECK(a.data()[1] == ful::unit_utf8{'b'});
	CHECK(a.data()[2] == ful::unit_utf8{});

	SECTION("can copy")
	{
		ful::static_string_utf8<111> b = a;
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
		REQUIRE(size(b) == 2);
		CHECK(b.data()[0] == ful::unit_utf8{'a'});
		CHECK(b.data()[1] == ful::unit_utf8{'b'});
		CHECK(b.data()[2] == ful::unit_utf8{});

		a = b;
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
		REQUIRE(size(b) == 2);
		CHECK(b.data()[0] == ful::unit_utf8{'a'});
		CHECK(b.data()[1] == ful::unit_utf8{'b'});
		CHECK(b.data()[2] == ful::unit_utf8{});
	}

	SECTION("can copy small")
	{
		ful::static_string_utf8<123> b(a);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
		REQUIRE(size(b) == 2);
		CHECK(b.data()[0] == ful::unit_utf8{'a'});
		CHECK(b.data()[1] == ful::unit_utf8{'b'});
		CHECK(b.data()[2] == ful::unit_utf8{});

		// todo clear b

		b = a;
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
		REQUIRE(size(b) == 2);
		CHECK(b.data()[0] == ful::unit_utf8{'a'});
		CHECK(b.data()[1] == ful::unit_utf8{'b'});
		CHECK(b.data()[2] == ful::unit_utf8{});
	}

	SECTION("can self-copy")
	{
		a = static_cast<ful::static_string_utf8<111> &&>(a);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can swap")
	{
		ful::static_string_utf8<111> b;

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
