#pragma once

#include "ful/stdint.hpp"

namespace ful
{
	using unit_utf8 = char;

#if defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))
	using unit_utf16be = uint16;
	using unit_utf16le = wchar_t;
	using unit_utf32be = uint32;
	using unit_utf32le = char32_t;

	using unit_utf16 = unit_utf16le;
	using unit_utf32 = unit_utf32le;
	using unit_utfw = unit_utf16le;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	using unit_utf16be = uint16;
	using unit_utf16le = char16_t;
	using unit_utf32be = uint32;
	using unit_utf32le = char32_t;

	using unit_utf16 = unit_utf16le;
	using unit_utf32 = unit_utf32le;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	using unit_utf16be = char16_t;
	using unit_utf16le = uint16;
	using unit_utf32be = char32_t;
	using unit_utf32le = uint32;

	using unit_utf16 = unit_utf16be;
	using unit_utf32 = unit_utf32be;
#else
# error Unknown endianess!
#endif
}
