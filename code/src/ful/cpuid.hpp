#pragma once

// #if RUNTIME_CPUID

namespace ful
{
// note "Intel considers POPCNT as part of SSE4.2, and LZCNT as part of BMI1"
//
// https://en.wikipedia.org/w/index.php?title=Bit_manipulation_instruction_set&oldid=1003418999

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

	inline bool cpuid_supports(cpuid_feature feature)
	{
		extern unsigned int cpuid_feature_cache;

		return (cpuid_feature_cache & static_cast<unsigned int>(feature)) != 0;
	}

	template <typename V>
	V cpuid_switch(V value)
	{
		return value;
	}
	template <typename V, typename ...Ps>
	V cpuid_switch(cpuid_feature feature, V value, Ps ...ps)
	{
		if (cpuid_supports(feature))
			return value;

		return cpuid_switch(ps...);
	}
}

// #endif
