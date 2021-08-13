#pragma once

#include "ful/types.hpp"

namespace ful
{
	template <typename T>
	class cstr_base;

	using cstr_utf8 = cstr_base<unit_utf8>;
	using cstr_utf16le = cstr_base<unit_utf16le>;
	using cstr_utf16be = cstr_base<unit_utf16be>;
	using cstr_utf32le = cstr_base<unit_utf32le>;
	using cstr_utf32be = cstr_base<unit_utf32be>;
	using cstr_utf16 = cstr_base<unit_utf16>;
	using cstr_utf32 = cstr_base<unit_utf32>;
#if defined(_MSC_VER)
	using cstr_utfw = cstr_base<unit_utfw>;
#endif
}
