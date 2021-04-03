#include "ful/cpuid.hpp"
#include "ful/string.hpp"

using namespace ful;
using namespace ful::detail;

namespace
{
	bool equal_cstr_dispatch_init(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
}

namespace ful
{
	namespace detail
	{
		bool (* equal_cstr_dispatch)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2) = equal_cstr_dispatch_init;
	}
}

namespace
{
	bool equal_cstr_dispatch_init(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
	{
		equal_cstr_dispatch = cpuid_switch(
			cpuid_feature::AVX2, equal_cstr_avx2,
			cpuid_feature::SSE2, equal_cstr_sse2,
			equal_cstr_none);

		return equal_cstr_dispatch(beg1, end1, beg2);
	}
}
