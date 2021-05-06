#pragma once

#if defined(FUL_FPTR)

// #include "ful/compiler.hpp"

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

#endif
