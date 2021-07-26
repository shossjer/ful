#pragma once

#if defined(FUL_IFUNC)

# define ful_cpucall(stem, alt, ...) ful::detail::stem##_##alt
# define ful_cpuinit() __builtin_cpu_init()
# define ful_cpucheck(feature) __builtin_cpu_supports(FUL_FEATURE_NAME_##feature)
# define ful_resolve(stem, ret, ...) \
namespace ful { namespace detail { __attribute__((ifunc ("resolve_" #stem))) ret stem(__VA_ARGS__); }} \
extern "C" { ret (* resolve_##stem())(__VA_ARGS__); } \
ret (* resolve_##stem())(__VA_ARGS__)

#define FUL_FEATURE_NAME_AVX "avx"
#define FUL_FEATURE_NAME_AVX2 "avx2"
#define FUL_FEATURE_NAME_SSE "sse"
#define FUL_FEATURE_NAME_SSE2 "sse2"
#define FUL_FEATURE_NAME_SSE3 "sse3"
#define FUL_FEATURE_NAME_SSSE3 "ssse3"
#define FUL_FEATURE_NAME_SSE4_1 "sse4.1"
#define FUL_FEATURE_NAME_SSE4_2 "sse4.2"
#define FUL_FEATURE_NAME_POPCNT "popcnt"
#define FUL_FEATURE_NAME_ABM "abm"
#define FUL_FEATURE_NAME_LZCNT "lzcnt"
#define FUL_FEATURE_NAME_BMI "bmi"
#define FUL_FEATURE_NAME_BMI2 "bmi2"

#elif defined(FUL_FPTR)

# define ful_cpucall(stem, alt, ...) ful::detail::stem = ful::detail::stem##_##alt, ful::detail::stem##_##alt(__VA_ARGS__)
# define ful_cpuinit() ful::cpuid_init()
# define ful_cpucheck(feature) ful::cpuid_supports(ful::cpuid_feature::feature)
# define ful_resolve(stem, ret, ...) \
static ret resolve_##stem(__VA_ARGS__); \
namespace ful { namespace detail { ret (* stem)(__VA_ARGS__) = resolve_##stem; }} \
static ret resolve_##stem(__VA_ARGS__)

# include "ful/compiler.hpp"

namespace ful
{
	enum class cpuid_feature : unsigned int
	{
		// NONE   = 0,

		AVX    = 1u << 7,
		AVX2   = 1u << 8,
		// MMX    = 1u << 0,
		SSE    = 1u << 1,
		SSE2   = 1u << 2,
		SSE3   = 1u << 3,
		SSSE3  = 1u << 4,
		SSE4_1 = 1u << 5,
		SSE4_2 = 1u << 6,

		POPCNT = 1u << 9,
		ABM    = 1u << 10,
		LZCNT  = 1u << 10,
		BMI    = 1u << 11,
		BMI2   = 1u << 12,
	};

	extern void cpuid_init();

	inline bool cpuid_supports(cpuid_feature feature)
	{
		extern unsigned int cpuid_feature_cache;

		if (ful_expect(cpuid_feature_cache != 0))
		{
			return (cpuid_feature_cache & static_cast<unsigned int>(feature)) != 0;
		}
		else
		{
			return false;
		}
	}
}

#else

# error Not supported!

#endif
