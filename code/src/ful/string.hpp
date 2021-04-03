#pragma once

 #include "ful/intrinsics.hpp"
 // #include "ful/stdint.hpp"
 #include "ful/unicode.hpp"

#include "string_avx2.hpp"
#include "string_sse2.hpp"
#include "string_sse4_2.hpp"

namespace ful
{
	namespace detail
	{
		inline bool equal_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return false;
			}
			return *beg2 == '\0';
		}

		inline bool equal_cstr(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			extern bool (* equal_cstr_dispatch)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);

			return equal_cstr_dispatch(beg1, end1, beg2);
		}
	}
}
