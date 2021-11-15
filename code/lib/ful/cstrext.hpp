#pragma once

#include "ful/cstr.hpp"
#include "ful/string_search.hpp"

namespace ful
{
	template <typename U, typename T>
	cstr_base<U> make_cstr_base(const T * cstr)
	{
		return cstr_base<U>(reinterpret_cast<const U *>(cstr), reinterpret_cast<const U *>(strend(cstr)));
	}

	template <typename T>
	cstr_utf8 make_cstr_utf8(const T * cstr) { return make_cstr_base<unit_utf8>(cstr); }
	template <typename T>
	cstr_utf16le make_cstr_utf16le(const T * cstr) { return make_cstr_base<unit_utf16le>(cstr); }
	template <typename T>
	cstr_utf16be make_cstr_utf16be(const T * cstr) { return make_cstr_base<unit_utf16be>(cstr); }
	template <typename T>
	cstr_utf32le make_cstr_utf32le(const T * cstr) { return make_cstr_base<unit_utf32le>(cstr); }
	template <typename T>
	cstr_utf32be make_cstr_utf32be(const T * cstr) { return make_cstr_base<unit_utf32be>(cstr); }
#if defined(_MSC_VER)
	template <typename T>
	cstr_utfw make_cstr_utfw(const T * cstr) { return make_cstr_base<unit_utfw>(cstr); }
#endif
}
