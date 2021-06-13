#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "private/cpuid.hpp"

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"

#include "ful/string_none.hpp"
#include "ful/string_sse.hpp"
#include "ful/string_sse2.hpp"
#include "ful/string_avx.hpp"
#include "ful/string_avx2.hpp"

using namespace ful;

ful_resolve(copy_8, char8 *, const char8 * first, const char8 * last, char8 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(copy_8, avx, first, last, begin);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(copy_8, sse2, first, last, begin);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(copy_8, sse, first, last, begin);
	else
		return ful_cpucall(copy_8, none, first, last, begin);
}

ful_resolve(rcopy_8, char8 *, const char8 * first, const char8 * last, char8 * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(rcopy_8, avx, first, last, end);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rcopy_8, sse2, first, last, end);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(rcopy_8, sse, first, last, end);
	else
		return ful_cpucall(rcopy_8, none, first, last, end);
}

ful_resolve(fill_large, void, char8 * from, char8 * to, char8 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(fill_large, avx, from, to, u);
	else
		return ful_cpucall(fill_large, none, from, to, u);
}

ful_resolve(equal_cstr, bool, const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(equal_cstr, avx2, beg1, end1, beg2);
	else
		return ful_cpucall(equal_cstr, none, beg1, end1, beg2);
}

ful_resolve(less_cstr, bool, const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(less_cstr, avx2, beg1, end1, beg2);
	else
		return ful_cpucall(less_cstr, none, beg1, end1, beg2);
}

#endif
