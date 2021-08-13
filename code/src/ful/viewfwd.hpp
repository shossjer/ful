#pragma once

#include "ful/types.hpp"

namespace ful
{
	template <typename T>
	class view_base;

	using view_utf8 = view_base<unit_utf8>;
	using view_utf16le = view_base<unit_utf16le>;
	using view_utf16be = view_base<unit_utf16be>;
	using view_utf32le = view_base<unit_utf32le>;
	using view_utf32be = view_base<unit_utf32be>;
	using view_utf16 = view_base<unit_utf16>;
	using view_utf32 = view_base<unit_utf32>;
#if defined(_MSC_VER)
	using view_utfw = view_base<unit_utfw>;
#endif
}
