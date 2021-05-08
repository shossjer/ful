#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "ful/compiler.hpp"
#include "ful/cpuid.hpp"
#include "ful/string.hpp"

using namespace ful;
using namespace ful::detail;

#if defined(FUL_IFUNC)
# define ful_cpucall(stem, func, ...) func
# define ful_cpuinit() __builtin_cpu_init()
# define ful_cpucheck(name, value) __builtin_cpu_supports(name)
# define ful_function(stem) stem##_type stem __attribute__((ifunc ("resolve_" #stem)))
# define ful_resolve(stem) (* resolve_##stem())
#elif defined(FUL_FPTR)
# define ful_cpucall(stem, func, ...) stem = func, func(__VA_ARGS__)
# define ful_cpuinit() ful::cpuid_init()
# define ful_cpucheck(name, value) ful::cpuid_supports(cpuid_feature::value)
# define ful_function(stem) stem##_type * stem = resolve_##stem
# define ful_resolve(stem) resolve_##stem
#endif

namespace
{
	using copy_large_type = unit_utf8 *(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin);
	using rcopy_large_type = unit_utf8 *(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end);
	using equal_cstr_type = bool(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
	using fill_large_type = void(unit_utf8 * from, unit_utf8 * to, unit_utf8 u);
	using less_cstr_type = bool(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);

#if defined(FUL_FPTR)
	copy_large_type resolve_copy_large;
	rcopy_large_type resolve_rcopy_large;
	equal_cstr_type resolve_equal_cstr;
	fill_large_type resolve_fill_large;
	less_cstr_type resolve_less_cstr;
#endif
}

namespace ful
{
	namespace detail
	{
		ful_function(copy_large);
		ful_function(rcopy_large);
		ful_function(equal_cstr);
		ful_function(fill_large);
		ful_function(less_cstr);
	}
}

namespace
{
#if defined(FUL_IFUNC)
	extern "C"
	{
#endif

		unit_utf8 * ful_resolve(copy_large)(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
		{
			ful_cpuinit();
			if (ful_cpucheck("avx", AVX))
				return ful_cpucall(copy_large, copy_large_avx, first, last, begin);
			else
				return ful_cpucall(copy_large, copy_large_none, first, last, begin);
		}

		unit_utf8 * ful_resolve(rcopy_large)(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
		{
			ful_cpuinit();
			if (ful_cpucheck("avx", AVX))
				return ful_cpucall(rcopy_large, rcopy_large_avx, first, last, end);
			else
				return ful_cpucall(rcopy_large, rcopy_large_none, first, last, end);
		}

		bool ful_resolve(equal_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_cpuinit();
			if (ful_cpucheck("avx2", AVX2))
				return ful_cpucall(equal_cstr, equal_cstr_avx2, beg1, end1, beg2);
			//else if (ful_cpucheck("sse2", SSE2))
			//	return ful_cpucall(equal_cstr, equal_cstr_sse2, beg1, end1, beg2);
			else
				return ful_cpucall(equal_cstr, equal_cstr_none, beg1, end1, beg2);
		}

		void ful_resolve(fill_large)(unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
		{
			ful_cpuinit();
			if (ful_cpucheck("avx", AVX))
				return ful_cpucall(fill_large, fill_large_avx, from, to, u);
			else
				return ful_cpucall(fill_large, fill_large_none, from, to, u);
		}

		bool ful_resolve(less_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_cpuinit();
			if (ful_cpucheck("avx2", AVX2))
				return ful_cpucall(less_cstr, less_cstr_avx2, beg1, end1, beg2);
			//else if (ful_cpucheck("sse2", SSE2))
			//	return ful_cpucall(less_cstr, less_cstr_sse2, beg1, end1, beg2);
			else
				return ful_cpucall(less_cstr, less_cstr_none, beg1, end1, beg2);
		}

#if defined(FUL_IFUNC)
	}
#endif
}

#endif
