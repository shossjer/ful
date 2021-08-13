#include "ful/view.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

TEST_CASE("a view of an array", "")
{
	ful::unit_utf8 buffer[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	SECTION("can be created by construction")
	{
		ful::view_utf8 a(buffer + 7, buffer + 19);
		CHECK(begin(a) == buffer + 7);
		CHECK(end(a) == buffer + 19);
		CHECK(size(a) == 12);
	}

	SECTION("can be created by from")
	{
		using ful::from;

		ful::view_utf8 a = from(buffer, buffer + 7);
		CHECK(begin(a) == buffer + 7);
		CHECK(end(a) == buffer + 26);
		CHECK(size(a) == 19);
	}

	SECTION("can be created by to")
	{
		using ful::to;

		ful::view_utf8 a = to(buffer, buffer + 19);
		CHECK(begin(a) == buffer + 0);
		CHECK(end(a) == buffer + 19);
		CHECK(size(a) == 19);
	}

	SECTION("can be created by first")
	{
		using ful::first;

		ful::view_utf8 a = first(buffer, 7);
		CHECK(begin(a) == buffer + 0);
		CHECK(end(a) == buffer + 7);
		CHECK(size(a) == 7);
	}

	SECTION("can be created by last")
	{
		using ful::last;

		ful::view_utf8 a = last(buffer, 7);
		CHECK(begin(a) == buffer + 19);
		CHECK(end(a) == buffer + 26);
		CHECK(size(a) == 7);
	}

	SECTION("can be created by substr")
	{
		using ful::substr;

		ful::view_utf8 a = substr(buffer, 7, 12);
		CHECK(begin(a) == buffer + 7);
		CHECK(end(a) == buffer + 19);
		CHECK(size(a) == 12);
	}

	SECTION("can copy out of")
	{
		ful::unit_utf8 out[26] = {};

		CHECK(copy(ful::view_utf8(buffer + 0, buffer + 26), out) == out + 26);
		CHECK(out[0] == ful::unit_utf8{'a'});
		CHECK(out[1] == ful::unit_utf8{'b'});
		CHECK(out[2] == ful::unit_utf8{'c'});
		CHECK(out[3] == ful::unit_utf8{'d'});
		CHECK(out[4] == ful::unit_utf8{'e'});
		CHECK(out[5] == ful::unit_utf8{'f'});
		CHECK(out[6] == ful::unit_utf8{'g'});
		CHECK(out[7] == ful::unit_utf8{'h'});
		CHECK(out[8] == ful::unit_utf8{'i'});
		CHECK(out[9] == ful::unit_utf8{'j'});
		CHECK(out[10] == ful::unit_utf8{'k'});
		CHECK(out[11] == ful::unit_utf8{'l'});
		CHECK(out[12] == ful::unit_utf8{'m'});
		CHECK(out[13] == ful::unit_utf8{'n'});
		CHECK(out[14] == ful::unit_utf8{'o'});
		CHECK(out[15] == ful::unit_utf8{'p'});
		CHECK(out[16] == ful::unit_utf8{'q'});
		CHECK(out[17] == ful::unit_utf8{'r'});
		CHECK(out[18] == ful::unit_utf8{'s'});
		CHECK(out[19] == ful::unit_utf8{'t'});
		CHECK(out[20] == ful::unit_utf8{'u'});
		CHECK(out[21] == ful::unit_utf8{'v'});
		CHECK(out[22] == ful::unit_utf8{'w'});
		CHECK(out[23] == ful::unit_utf8{'x'});
		CHECK(out[24] == ful::unit_utf8{'y'});
		CHECK(out[25] == ful::unit_utf8{'z'});
	}

	SECTION("can find char8")
	{
		const ful::unit_utf8 * const it = find(ful::view_utf8(buffer + 0, buffer + 26), ful::char8{'p'});
		CHECK(it == buffer + 15);
	}

	SECTION("can find char16")
	{
		const ful::unit_utf8 * const it = find(ful::view_utf8(buffer + 0, buffer + 26), ful::char16{0x7170}); // 'pq'
		CHECK(it == buffer + 15);
	}

	SECTION("can find char24")
	{
		const ful::unit_utf8 * const it = find(ful::view_utf8(buffer + 0, buffer + 26), ful::char_fast24{0x727170}); // 'pqr'
		CHECK(it == buffer + 15);
	}

	SECTION("can find char32")
	{
		const ful::unit_utf8 * const it = find(ful::view_utf8(buffer + 0, buffer + 26), ful::char32{0x73727170}); // 'pqrs'
		CHECK(it == buffer + 15);
	}
}
