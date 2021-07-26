#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "private/cpuid.hpp"

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"

#include "ful/unicode.hpp"
#include "ful/unicode_none.hpp"
#include "ful/unicode_avx2.hpp"

ful_resolve(point_count_8, ful::usize, const ful::unit_utf8 * beg, const ful::unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(point_count_8, none, beg, end);
}

ful_resolve(point_count_16, ful::usize, const ful::unit_utf16 * beg, const ful::unit_utf16 * end)
{
	ful_cpuinit();
	return ful_cpucall(point_count_16, none, beg, end);
}

ful_resolve(point_count_61, ful::usize, const ful::unit_utf61 * beg, const ful::unit_utf61 * end)
{
	ful_cpuinit();
	return ful_cpucall(point_count_61, none, beg, end);
}

ful_resolve(point_next_8, const ful::unit_utf8 *, const ful::unit_utf8 * s, ful::usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_next_8, none, s, n);
}

ful_resolve(point_prev_8, const ful::unit_utf8 *, const ful::unit_utf8 * s, ful::usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_prev_8, none, s, n);
}

#endif
