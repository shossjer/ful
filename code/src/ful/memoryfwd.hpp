#pragma once

#include "ful/types.hpp"

namespace ful
{
	template <typename T, usize Alignment>
	class memory_string_base;

	template <typename Base>
	class string_container;

	template <typename T, usize Alignment>
	using memory_string = string_container<memory_string_base<T, Alignment>>;

	template <usize Alignment>
	using memory_string_utf8 = memory_string<unit_utf8, Alignment>;
	template <usize Alignment>
	using memory_string_utf16le = memory_string<unit_utf16le, Alignment>;
	template <usize Alignment>
	using memory_string_utf16be = memory_string<unit_utf16be, Alignment>;
	template <usize Alignment>
	using memory_string_utf32le = memory_string<unit_utf32le, Alignment>;
	template <usize Alignment>
	using memory_string_utf32be = memory_string<unit_utf32be, Alignment>;
	template <usize Alignment>
	using memory_string_utf16 = memory_string<unit_utf16, Alignment>;
	template <usize Alignment>
	using memory_string_utf32 = memory_string<unit_utf32, Alignment>;
#if defined(_MSC_VER)
	template <usize Alignment>
	using memory_string_utfw = memory_string<unit_utfw, Alignment>;
#endif
}
