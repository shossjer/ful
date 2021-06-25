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

ful_resolve(memcopy, char8 *, const char8 * first, const char8 * last, char8 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memcopy, avx, first, last, begin);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memcopy, sse2, first, last, begin);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memcopy, sse, first, last, begin);
	else
		return ful_cpucall(memcopy, none, first, last, begin);
}

ful_resolve(memmovef, char8 *, const char8 * first, const char8 * last, char8 * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memmovef, avx, first, last, end);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memmovef, sse2, first, last, end);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memmovef, sse, first, last, end);
	else
		return ful_cpucall(memmovef, none, first, last, end);
}

ful_resolve(memmover, char8 *, const char8 * first, const char8 * last, char8 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memmover, avx, first, last, begin);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memmover, sse2, first, last, begin);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memmover, sse, first, last, begin);
	else
		return ful_cpucall(memmover, none, first, last, begin);
}

ful_resolve(memswap, char8 *, char8 * beg1, char8 * end1, char8 * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memswap, avx, beg1, end1, beg2);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memswap, sse2, beg1, end1, beg2);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memswap, sse, beg1, end1, beg2);
	else
		return ful_cpucall(memswap, none, beg1, end1, beg2);
}

ful_resolve(memset8, void, char8 * from, char8 * to, char8 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset8, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset8, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset8, sse, from, to, u);
	else
		return ful_cpucall(memset8, none, from, to, u);
}

ful_resolve(memset16, void, char16 * from, char16 * to, char16 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset16, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset16, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset16, sse, from, to, u);
	else
		return ful_cpucall(memset16, none, from, to, u);
}

ful_resolve(memset24, void, char24 * from, char24 * to, char24 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset24, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset24, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset24, sse, from, to, u);
	else
		return ful_cpucall(memset24, none, from, to, u);
}

ful_resolve(memset32, void, char32 * from, char32 * to, char32 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset32, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset32, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset32, sse, from, to, u);
	else
		return ful_cpucall(memset32, none, from, to, u);
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
