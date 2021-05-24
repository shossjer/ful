#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "private/cpuid.hpp"

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"
#include "ful/point_utils.hpp"
#include "ful/unsafe_ptr.hpp"

#include "ful/unicode_none.hpp"

using namespace ful;

ful_resolve(point_count, usize, const unit_utf8 * beg, const unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(point_count, none, beg, end);
}

ful_resolve(point_next, const unit_utf8 *, const unit_utf8 * s, usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_next, none, s, n);
}

ful_resolve(point_prev, const unit_utf8 *, const unit_utf8 * s, usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_prev, none, s, n);
}

#endif
