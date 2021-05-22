#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "ful/string.hpp"
#include "ful/probe/cpuid.hpp"

#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"
//#include "ful/unicode.hpp"
#include "ful/unsafe_ptr.hpp"

#include "ful/string_avx.hpp"
#include "ful/string_avx2.hpp"
#include "ful/string_none.hpp"

using namespace ful;

ful_resolve(copy_large, unit_utf8 * , const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck("avx", AVX))
		return ful_cpucall(copy_large, avx, first, last, begin);
	else
		return ful_cpucall(copy_large, none, first, last, begin);
}

ful_resolve(rcopy_large, unit_utf8 *, const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
{
	ful_cpuinit();
	if (ful_cpucheck("avx", AVX))
		return ful_cpucall(rcopy_large, avx, first, last, end);
	else
		return ful_cpucall(rcopy_large, none, first, last, end);
}

ful_resolve(equal_cstr, bool, const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck("avx2", AVX2))
		return ful_cpucall(equal_cstr, avx2, beg1, end1, beg2);
	else
		return ful_cpucall(equal_cstr, none, beg1, end1, beg2);
}

ful_resolve(fill_large, void, unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
{
	ful_cpuinit();
	if (ful_cpucheck("avx", AVX))
		return ful_cpucall(fill_large, avx, from, to, u);
	else
		return ful_cpucall(fill_large, none, from, to, u);
}

ful_resolve(less_cstr, bool, const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck("avx2", AVX2))
		return ful_cpucall(less_cstr, avx2, beg1, end1, beg2);
	else
		return ful_cpucall(less_cstr, none, beg1, end1, beg2);
}

#endif
