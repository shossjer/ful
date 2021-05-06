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
	using equal_cstr_type = bool(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
	using less_cstr_type = bool(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);

#if defined(FUL_FPTR)
	equal_cstr_type resolve_equal_cstr;
	less_cstr_type resolve_less_cstr;
#endif
}

namespace ful
{
	namespace detail
	{
		ful_function(equal_cstr);
		ful_function(less_cstr);
	}
}

namespace
{
#if defined(FUL_IFUNC)
	extern "C"
	{
#endif

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
