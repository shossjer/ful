#pragma once

#include "ful/types.hpp"

namespace ful
{
	template <typename T>
	class heap_string_base;

	template <typename Base>
	class string_container;

	template <typename T>
	using heap_string = string_container<heap_string_base<T>>;

	using heap_string_utf8 = heap_string<unit_utf8>;
	using heap_string_utf16le = heap_string<unit_utf16le>;
	using heap_string_utf16be = heap_string<unit_utf16be>;
	using heap_string_utf32le = heap_string<unit_utf32le>;
	using heap_string_utf32be = heap_string<unit_utf32be>;
	using heap_string_utf16 = heap_string<unit_utf16>;
	using heap_string_utf32 = heap_string<unit_utf32>;
#if defined(_MSC_VER)
	using heap_string_utfw = heap_string<unit_utfw>;
#endif
}
