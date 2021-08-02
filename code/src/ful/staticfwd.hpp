#pragma once

#include "ful/types.hpp"

namespace ful
{
	template <typename T, usize Capacity>
	class static_string_base;

	template <typename Base>
	class string_container;

	template <typename T, usize Capacity>
	using static_string = string_container<static_string_base<T, Capacity>>;

	template <usize Capacity>
	using static_string_utf8 = static_string<unit_utf8, Capacity>;
	template <usize Capacity>
	using static_string_utf16le = static_string<unit_utf16le, Capacity>;
	template <usize Capacity>
	using static_string_utf16be = static_string<unit_utf16be, Capacity>;
	template <usize Capacity>
	using static_string_utf32le = static_string<unit_utf32le, Capacity>;
	template <usize Capacity>
	using static_string_utf32be = static_string<unit_utf32be, Capacity>;
	template <usize Capacity>
	using static_string_utf16 = static_string<unit_utf16, Capacity>;
	template <usize Capacity>
	using static_string_utf32 = static_string<unit_utf32, Capacity>;
#if defined(_MSC_VER)
	template <usize Capacity>
	using static_string_utfw = static_string<unit_utfw, Capacity>;
#endif
}
