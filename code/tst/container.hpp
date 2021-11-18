#pragma once

template <typename Base>
void test_empty_container_utf8(ful::string_container<Base> & a)
{
	SECTION("can self-swap")
	{
		swap(a, a);
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can reserve zero")
	{
		CHECK(reserve(a, 0));
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can reserve non-zero big")
	{
		CHECK(reserve(a, 111));
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can resize zero")
	{
		CHECK(resize(a, 0));
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can resize non-zero big")
	{
		CHECK(resize(a, 111));
		REQUIRE(size(a) == 111);
		CHECK(a.data()[111] == ful::unit_utf8{});
	}

	SECTION("can push char8")
	{
		const auto it = push_back(a, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can push char16")
	{
		const auto it = push_back(a, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can push char24")
	{
		const auto it = push_back(a, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 3);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{});
	}

	SECTION("can push char32")
	{
		const auto it = push_back(a, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 4);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{});
	}

	SECTION("can append empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = append(a, null_ptr, null_ptr);
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can append non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = append(a, buffer + 0, buffer + 16);
		CHECK(it == a.begin());
		REQUIRE(size(a) == 16);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'y'});
		CHECK(a.data()[2] == ful::unit_utf8{'z'});
		CHECK(a.data()[3] == ful::unit_utf8{'x'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'y'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'z'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'y'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'x'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'z'});
		CHECK(a.data()[14] == ful::unit_utf8{'y'});
		CHECK(a.data()[15] == ful::unit_utf8{'x'});
		CHECK(a.data()[16] == ful::unit_utf8{});
	}

	SECTION("can append zero char8")
	{
		const auto it = append(a, 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can append zero char16")
	{
		const auto it = append(a, 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can append zero char24")
	{
		const auto it = append(a, 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can append zero char32")
	{
		const auto it = append(a, 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can append non-zero char8")
	{
		const auto it = append(a, 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char16")
	{
		const auto it = append(a, 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char24")
	{
		const auto it = append(a, 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char32")
	{
		const auto it = append(a, 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can insert empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = insert(a, a.begin(), null_ptr, null_ptr);
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can insert non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = insert(a, a.begin(), buffer + 0, buffer + 16);
		CHECK(it == a.begin());
		REQUIRE(size(a) == 16);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'y'});
		CHECK(a.data()[2] == ful::unit_utf8{'z'});
		CHECK(a.data()[3] == ful::unit_utf8{'x'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'y'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'z'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'y'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'x'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'z'});
		CHECK(a.data()[14] == ful::unit_utf8{'y'});
		CHECK(a.data()[15] == ful::unit_utf8{'x'});
		CHECK(a.data()[16] == ful::unit_utf8{});
	}

	SECTION("can insert zero char8")
	{
		const auto it = insert(a, a.begin(), 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can insert zero char16")
	{
		const auto it = insert(a, a.begin(), 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can insert zero char24")
	{
		const auto it = insert(a, a.begin(), 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can insert zero char32")
	{
		const auto it = insert(a, a.begin(), 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char8")
	{
		const auto it = insert(a, a.begin(), 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char16")
	{
		const auto it = insert(a, a.begin(), 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char24")
	{
		const auto it = insert(a, a.begin(), 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char32")
	{
		const auto it = insert(a, a.begin(), 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can assign empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = assign(a, null_ptr, null_ptr);
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = assign(a, buffer + 0, buffer + 16);
		CHECK(it == a.begin());
		REQUIRE(size(a) == 16);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'y'});
		CHECK(a.data()[2] == ful::unit_utf8{'z'});
		CHECK(a.data()[3] == ful::unit_utf8{'x'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'y'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'z'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'y'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'x'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'z'});
		CHECK(a.data()[14] == ful::unit_utf8{'y'});
		CHECK(a.data()[15] == ful::unit_utf8{'x'});
		CHECK(a.data()[16] == ful::unit_utf8{});
	}

	SECTION("can assign zero char8")
	{
		const auto it = assign(a, 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char16")
	{
		const auto it = assign(a, 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char24")
	{
		const auto it = assign(a, 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char32")
	{
		const auto it = assign(a, 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char8")
	{
		const auto it = assign(a, 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char16")
	{
		const auto it = assign(a, 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char24")
	{
		const auto it = assign(a, 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char32")
	{
		const auto it = assign(a, 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can replace empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = replace(a, a.begin(), a.end(), null_ptr, null_ptr);
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can replace non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = replace(a, a.begin(), a.end(), buffer + 0, buffer + 16);
		CHECK(it == a.begin());
		REQUIRE(size(a) == 16);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'y'});
		CHECK(a.data()[2] == ful::unit_utf8{'z'});
		CHECK(a.data()[3] == ful::unit_utf8{'x'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'y'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'z'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'y'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'x'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'z'});
		CHECK(a.data()[14] == ful::unit_utf8{'y'});
		CHECK(a.data()[15] == ful::unit_utf8{'x'});
		CHECK(a.data()[16] == ful::unit_utf8{});
	}

	SECTION("can replace zero char8")
	{
		const auto it = replace(a, a.begin(), a.end(), 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can replace zero char16")
	{
		const auto it = replace(a, a.begin(), a.end(), 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can replace zero char24")
	{
		const auto it = replace(a, a.begin(), a.end(), 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can replace zero char32")
	{
		const auto it = replace(a, a.begin(), a.end(), 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char8")
	{
		const auto it = replace(a, a.begin(), a.end(), 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char16")
	{
		const auto it = replace(a, a.begin(), a.end(), 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char24")
	{
		const auto it = replace(a, a.begin(), a.end(), 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char32")
	{
		const auto it = replace(a, a.begin(), a.end(), 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can clear")
	{
		clear(a);
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can reduce")
	{
		reduce(a, a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can erase empty range")
	{
		erase(a, a.begin(), a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can copy out of")
	{
		ful::unit_utf8 * const begin_ = nullptr;
		ful::unit_utf8 * const end_ = nullptr;
		CHECK(copy(a, begin_, end_) == nullptr);
	}

	SECTION("can copy empty range into")
	{
		const ful::unit_utf8 * const first = nullptr;
		const ful::unit_utf8 * const last = nullptr;
		const auto it = copy(first, last, a);
		CHECK(it == a.end());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can copy non-empty range into")
	{
		const ful::unit_utf8 buffer[10] = {'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x'};

		const auto it = copy(buffer, a);
		CHECK(it == a.end());
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'x'});
		CHECK(a.data()[2] == ful::unit_utf8{'y'});
		CHECK(a.data()[3] == ful::unit_utf8{'y'});
		CHECK(a.data()[4] == ful::unit_utf8{'z'});
		CHECK(a.data()[5] == ful::unit_utf8{'z'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'y'});
		CHECK(a.data()[8] == ful::unit_utf8{'x'});
		CHECK(a.data()[9] == ful::unit_utf8{'x'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can fill char8")
	{
		fill(a, ful::char8{0x31}); // '1'
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can fill char16")
	{
		fill(a, ful::char16{0x3232}); // '22'
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can fill char24")
	{
		fill(a, ful::char_fast24{0x333333}); // '333'
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can fill char32")
	{
		fill(a, ful::char32{0x34343434}); // '4444'
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can compare equal")
	{
		CHECK(a == a);
	}
}

template <typename Base>
void test_nonempty_container_utf8(ful::string_container<Base> & a)
{
	SECTION("can self-swap")
	{
		swap(a, a);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can reserve zero")
	{
		CHECK(reserve(a, 0));
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can reserve non-zero big")
	{
		CHECK(reserve(a, 111));
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can reserve non-zero small")
	{
		CHECK(reserve(a, 1));
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can resize zero")
	{
		CHECK(resize(a, 0));
		CHECK(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can resize non-zero big")
	{
		CHECK(resize(a, 111));
		REQUIRE(size(a) == 111);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[111] == ful::unit_utf8{});
	}

	SECTION("can resize non-zero small")
	{
		CHECK(resize(a, 1));
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can push char8")
	{
		const auto it = push_back(a, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 3);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{});
	}

	SECTION("can push char16")
	{
		const auto it = push_back(a, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 4);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{});
	}

	SECTION("can push char24")
	{
		const auto it = push_back(a, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'3'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can push char32")
	{
		const auto it = push_back(a, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 6);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'3'});
		CHECK(a.data()[5] == ful::unit_utf8{'4'});
		CHECK(a.data()[6] == ful::unit_utf8{});
	}

	SECTION("can pop")
	{
		pop_back(a);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});

		pop_back(a);
		REQUIRE(empty(a));
		CHECK(a.data()[0] == ful::unit_utf8{});
	}

	SECTION("can append empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = append(a, null_ptr, null_ptr);
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can append non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = append(a, buffer + 0, buffer + 16);
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 18);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'x'});
		CHECK(a.data()[3] == ful::unit_utf8{'y'});
		CHECK(a.data()[4] == ful::unit_utf8{'z'});
		CHECK(a.data()[5] == ful::unit_utf8{'x'});
		CHECK(a.data()[6] == ful::unit_utf8{'x'});
		CHECK(a.data()[7] == ful::unit_utf8{'y'});
		CHECK(a.data()[8] == ful::unit_utf8{'y'});
		CHECK(a.data()[9] == ful::unit_utf8{'z'});
		CHECK(a.data()[10] == ful::unit_utf8{'z'});
		CHECK(a.data()[11] == ful::unit_utf8{'y'});
		CHECK(a.data()[12] == ful::unit_utf8{'y'});
		CHECK(a.data()[13] == ful::unit_utf8{'x'});
		CHECK(a.data()[14] == ful::unit_utf8{'x'});
		CHECK(a.data()[15] == ful::unit_utf8{'z'});
		CHECK(a.data()[16] == ful::unit_utf8{'y'});
		CHECK(a.data()[17] == ful::unit_utf8{'x'});
		CHECK(a.data()[18] == ful::unit_utf8{});
	}

	SECTION("can append zero char8")
	{
		const auto it = append(a, 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can append zero char16")
	{
		const auto it = append(a, 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can append zero char24")
	{
		const auto it = append(a, 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can append zero char32")
	{
		const auto it = append(a, 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char8")
	{
		const auto it = append(a, 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 7);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char16")
	{
		const auto it = append(a, 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'1'});
		CHECK(a.data()[9] == ful::unit_utf8{'2'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char24")
	{
		const auto it = append(a, 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 11);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'3'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'2'});
		CHECK(a.data()[7] == ful::unit_utf8{'3'});
		CHECK(a.data()[8] == ful::unit_utf8{'1'});
		CHECK(a.data()[9] == ful::unit_utf8{'2'});
		CHECK(a.data()[10] == ful::unit_utf8{'3'});
		CHECK(a.data()[11] == ful::unit_utf8{});
	}

	SECTION("can append non-zero char32")
	{
		const auto it = append(a, 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin() + 2);
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'3'});
		CHECK(a.data()[5] == ful::unit_utf8{'4'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{'4'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can insert empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = insert(a, a.begin() + 1, null_ptr, null_ptr);
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can insert non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = insert(a, a.begin() + 1, buffer + 0, buffer + 16);
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 18);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'x'});
		CHECK(a.data()[2] == ful::unit_utf8{'y'});
		CHECK(a.data()[3] == ful::unit_utf8{'z'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'x'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'y'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'z'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'y'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'x'});
		CHECK(a.data()[14] == ful::unit_utf8{'z'});
		CHECK(a.data()[15] == ful::unit_utf8{'y'});
		CHECK(a.data()[16] == ful::unit_utf8{'x'});
		CHECK(a.data()[17] == ful::unit_utf8{'b'});
		CHECK(a.data()[18] == ful::unit_utf8{});
	}

	SECTION("can insert zero char8")
	{
		const auto it = insert(a, a.begin() + 1, 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can insert zero char16")
	{
		const auto it = insert(a, a.begin() + 1, 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can insert zero char24")
	{
		const auto it = insert(a, a.begin() + 1, 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can insert zero char32")
	{
		const auto it = insert(a, a.begin() + 1, 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char8")
	{
		const auto it = insert(a, a.begin() + 1, 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 7);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'b'});
		CHECK(a.data()[7] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char16")
	{
		const auto it = insert(a, a.begin() + 1, 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'2'});
		CHECK(a.data()[7] == ful::unit_utf8{'1'});
		CHECK(a.data()[8] == ful::unit_utf8{'2'});
		CHECK(a.data()[9] == ful::unit_utf8{'b'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char24")
	{
		const auto it = insert(a, a.begin() + 1, 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 11);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'3'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'1'});
		CHECK(a.data()[8] == ful::unit_utf8{'2'});
		CHECK(a.data()[9] == ful::unit_utf8{'3'});
		CHECK(a.data()[10] == ful::unit_utf8{'b'});
		CHECK(a.data()[11] == ful::unit_utf8{});
	}

	SECTION("can insert non-zero char32")
	{
		const auto it = insert(a, a.begin() + 1, 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'3'});
		CHECK(a.data()[4] == ful::unit_utf8{'4'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'2'});
		CHECK(a.data()[7] == ful::unit_utf8{'3'});
		CHECK(a.data()[8] == ful::unit_utf8{'4'});
		CHECK(a.data()[9] == ful::unit_utf8{'b'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can assign empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = assign(a, null_ptr, null_ptr);
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = assign(a, buffer + 0, buffer + 16);
		CHECK(it == a.begin());
		REQUIRE(size(a) == 16);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'y'});
		CHECK(a.data()[2] == ful::unit_utf8{'z'});
		CHECK(a.data()[3] == ful::unit_utf8{'x'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'y'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'z'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'y'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'x'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'z'});
		CHECK(a.data()[14] == ful::unit_utf8{'y'});
		CHECK(a.data()[15] == ful::unit_utf8{'x'});
		CHECK(a.data()[16] == ful::unit_utf8{});
	}

	SECTION("can assign zero char8")
	{
		const auto it = assign(a, 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char16")
	{
		const auto it = assign(a, 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char24")
	{
		const auto it = assign(a, 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign zero char32")
	{
		const auto it = assign(a, 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char8")
	{
		const auto it = assign(a, 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 5);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char16")
	{
		const auto it = assign(a, 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char24")
	{
		const auto it = assign(a, 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can assign non-zero char32")
	{
		const auto it = assign(a, 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin());
		REQUIRE(size(a) == 8);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{});
	}

	SECTION("can replace empty range")
	{
		const char * const null_ptr = nullptr;

		const auto it = replace(a, a.begin() + 1, a.end(), null_ptr, null_ptr);
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can replace non-empty range")
	{
		const ful::unit_utf8 buffer[16] = {'x', 'y', 'z', 'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x', 'z', 'y', 'x'};

		const auto it = replace(a, a.begin() + 1, a.end(), buffer + 0, buffer + 16);
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 17);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'x'});
		CHECK(a.data()[2] == ful::unit_utf8{'y'});
		CHECK(a.data()[3] == ful::unit_utf8{'z'});
		CHECK(a.data()[4] == ful::unit_utf8{'x'});
		CHECK(a.data()[5] == ful::unit_utf8{'x'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'y'});
		CHECK(a.data()[8] == ful::unit_utf8{'z'});
		CHECK(a.data()[9] == ful::unit_utf8{'z'});
		CHECK(a.data()[10] == ful::unit_utf8{'y'});
		CHECK(a.data()[11] == ful::unit_utf8{'y'});
		CHECK(a.data()[12] == ful::unit_utf8{'x'});
		CHECK(a.data()[13] == ful::unit_utf8{'x'});
		CHECK(a.data()[14] == ful::unit_utf8{'z'});
		CHECK(a.data()[15] == ful::unit_utf8{'y'});
		CHECK(a.data()[16] == ful::unit_utf8{'x'});
		CHECK(a.data()[17] == ful::unit_utf8{});
	}

	SECTION("can replace zero char8")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 0, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can replace zero char16")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 0, ful::char16{0x3232}); // '22'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can replace zero char24")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 0, ful::char_fast24{0x333333}); // '333'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can replace zero char32")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 0, ful::char32{0x34343434}); // '4444'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char8")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 5, ful::char8{0x31}); // '1'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 6);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char16")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 4, ful::char16{0x3231}); // '12'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'2'});
		CHECK(a.data()[7] == ful::unit_utf8{'1'});
		CHECK(a.data()[8] == ful::unit_utf8{'2'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char24")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 3, ful::char_fast24{0x333231}); // '123'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'3'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'1'});
		CHECK(a.data()[8] == ful::unit_utf8{'2'});
		CHECK(a.data()[9] == ful::unit_utf8{'3'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can replace non-zero char32")
	{
		const auto it = replace(a, a.begin() + 1, a.end(), 2, ful::char32{0x34333231}); // '1234'
		CHECK(it == a.begin() + 1);
		REQUIRE(size(a) == 9);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'2'});
		CHECK(a.data()[3] == ful::unit_utf8{'3'});
		CHECK(a.data()[4] == ful::unit_utf8{'4'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'2'});
		CHECK(a.data()[7] == ful::unit_utf8{'3'});
		CHECK(a.data()[8] == ful::unit_utf8{'4'});
		CHECK(a.data()[9] == ful::unit_utf8{});
	}

	SECTION("can clear")
	{
		clear(a);
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can reduce")
	{
		reduce(a, a.end());
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});

		reduce(a, a.begin());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can erase iterator")
	{
		erase(a, a.begin());
		REQUIRE(size(a) == 1);
		CHECK(a.data()[0] == ful::unit_utf8{'b'});
		CHECK(a.data()[1] == ful::unit_utf8{});
	}

	SECTION("can erase empty range")
	{
		erase(a, a.begin(), a.begin());
		REQUIRE(size(a) == 2);
		CHECK(a.data()[0] == ful::unit_utf8{'a'});
		CHECK(a.data()[1] == ful::unit_utf8{'b'});
		CHECK(a.data()[2] == ful::unit_utf8{});
	}

	SECTION("can erase non-empty range")
	{
		erase(a, a.begin(), a.end());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can copy out of")
	{
		ful::unit_utf8 buffer[2] = {'x', 'x'};

		CHECK(copy(a, buffer) == buffer + 2);
		CHECK(buffer[0] == ful::unit_utf8{'a'});
		CHECK(buffer[1] == ful::unit_utf8{'b'});
	}

	SECTION("can copy empty range into")
	{
		const char * const null_ptr = nullptr;

		const auto it = copy(null_ptr, null_ptr, a);
		CHECK(it == a.end());
		REQUIRE(empty(a));
		CHECK(*a.data() == ful::unit_utf8{});
	}

	SECTION("can copy non-empty range into")
	{
		const ful::unit_utf8 buffer[10] = {'x', 'x', 'y', 'y', 'z', 'z', 'y', 'y', 'x', 'x'};

		const auto it = copy(buffer, a);
		CHECK(it == a.end());
		REQUIRE(size(a) == 10);
		CHECK(a.data()[0] == ful::unit_utf8{'x'});
		CHECK(a.data()[1] == ful::unit_utf8{'x'});
		CHECK(a.data()[2] == ful::unit_utf8{'y'});
		CHECK(a.data()[3] == ful::unit_utf8{'y'});
		CHECK(a.data()[4] == ful::unit_utf8{'z'});
		CHECK(a.data()[5] == ful::unit_utf8{'z'});
		CHECK(a.data()[6] == ful::unit_utf8{'y'});
		CHECK(a.data()[7] == ful::unit_utf8{'y'});
		CHECK(a.data()[8] == ful::unit_utf8{'x'});
		CHECK(a.data()[9] == ful::unit_utf8{'x'});
		CHECK(a.data()[10] == ful::unit_utf8{});
	}

	SECTION("can fill char8")
	{
		REQUIRE(resize(a, 12));

		fill(a, ful::char8{0x31}); // '1'
		REQUIRE(size(a) == 12);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'1'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'1'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'1'});
		CHECK(a.data()[8] == ful::unit_utf8{'1'});
		CHECK(a.data()[9] == ful::unit_utf8{'1'});
		CHECK(a.data()[10] == ful::unit_utf8{'1'});
		CHECK(a.data()[11] == ful::unit_utf8{'1'});
		CHECK(a.data()[12] == ful::unit_utf8{});
	}

	SECTION("can fill char16")
	{
		REQUIRE(resize(a, 12));

		fill(a, ful::char16{0x3231}); // '12'
		REQUIRE(size(a) == 12);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'1'});
		CHECK(a.data()[3] == ful::unit_utf8{'2'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'1'});
		CHECK(a.data()[9] == ful::unit_utf8{'2'});
		CHECK(a.data()[10] == ful::unit_utf8{'1'});
		CHECK(a.data()[11] == ful::unit_utf8{'2'});
		CHECK(a.data()[12] == ful::unit_utf8{});
	}

	SECTION("can fill char24")
	{
		REQUIRE(resize(a, 12));

		fill(a, ful::char_fast24{0x333231}); // '123'
		REQUIRE(size(a) == 12);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'1'});
		CHECK(a.data()[4] == ful::unit_utf8{'2'});
		CHECK(a.data()[5] == ful::unit_utf8{'3'});
		CHECK(a.data()[6] == ful::unit_utf8{'1'});
		CHECK(a.data()[7] == ful::unit_utf8{'2'});
		CHECK(a.data()[8] == ful::unit_utf8{'3'});
		CHECK(a.data()[9] == ful::unit_utf8{'1'});
		CHECK(a.data()[10] == ful::unit_utf8{'2'});
		CHECK(a.data()[11] == ful::unit_utf8{'3'});
		CHECK(a.data()[12] == ful::unit_utf8{});
	}

	SECTION("can fill char32")
	{
		REQUIRE(resize(a, 12));

		fill(a, ful::char32{0x34333231}); // '1234'
		REQUIRE(size(a) == 12);
		CHECK(a.data()[0] == ful::unit_utf8{'1'});
		CHECK(a.data()[1] == ful::unit_utf8{'2'});
		CHECK(a.data()[2] == ful::unit_utf8{'3'});
		CHECK(a.data()[3] == ful::unit_utf8{'4'});
		CHECK(a.data()[4] == ful::unit_utf8{'1'});
		CHECK(a.data()[5] == ful::unit_utf8{'2'});
		CHECK(a.data()[6] == ful::unit_utf8{'3'});
		CHECK(a.data()[7] == ful::unit_utf8{'4'});
		CHECK(a.data()[8] == ful::unit_utf8{'1'});
		CHECK(a.data()[9] == ful::unit_utf8{'2'});
		CHECK(a.data()[10] == ful::unit_utf8{'3'});
		CHECK(a.data()[11] == ful::unit_utf8{'4'});
		CHECK(a.data()[12] == ful::unit_utf8{});
	}

	SECTION("can compare equal")
	{
		CHECK(a == a);
	}
}
