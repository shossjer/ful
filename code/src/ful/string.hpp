#pragma once

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"
#include "ful/unicode.hpp"

#include "string_avx2.hpp"
#include "string_sse2.hpp"
#include "string_sse4_2.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC)
		extern bool equal_cstr(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
		extern bool less_cstr(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
#elif defined(FUL_FPTR)
		extern bool (* equal_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
		extern bool (* less_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
#endif

		inline bool equal_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return false;
			}
			return *beg2 == '\0';
		}

		inline bool less_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return *beg1 < *beg2;
			}
			return *beg2 != '\0';
		}
	}
}
