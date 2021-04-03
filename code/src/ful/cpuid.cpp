#include "ful/cpuid.hpp"

#if defined(_MSC_VER)
# include <intrin.h>
#elif defined(__GNUC__)
# include <cpuid.h>
#endif

#define CHECK_BIT(reg, bit) (reg & (1 << bit))

namespace ful
{
	unsigned int cpuid_feature_cache = 0;
}

namespace
{
#if defined(_MSC_VER)
	using reg_type = int;
#elif defined(__GNUC__)
	using reg_type = unsigned int;
#else
# error Missing implementation!
#endif

	void get_cpuid(int n, reg_type * a)
	{
#if defined(_MSC_VER)
		__cpuid(a, n);
#elif defined(__GNUC__)
		__get_cpuid(n, a + 0, a + 1, a + 2, a + 3);
#else
# error Missing implementation!
#endif
	}

	struct static_hack_type
	{
		static_hack_type()
		{
			// https://software.intel.com/content/www/us/en/develop/download/intel-architecture-instruction-set-extensions-programming-reference.html
			// https://www.amd.com/system/files/TechDocs/24594.pdf
			reg_type info[4];

			get_cpuid(0, info);
			const unsigned int max_id = info[0];

			if (max_id >= 1)
			{
				get_cpuid(1, info);
				const int eax = info[0];
				const int model = ((static_cast<unsigned int>(eax) >> 12) & 0xf0) | ((static_cast<unsigned int>(eax) >> 4) & 0x0f);
				const int family = ((static_cast<unsigned int>(eax) >> 16) & 0xf0) | ((static_cast<unsigned int>(eax) >> 8) & 0x0f);
				const int ecx = info[2];
				const int edx = info[3];
				if (CHECK_BIT(ecx, 0)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSE3);
				if (CHECK_BIT(ecx, 9)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSSE3);
				if (CHECK_BIT(ecx, 19)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSE4_1);
				if (CHECK_BIT(ecx, 20)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSE4_2);
				if (CHECK_BIT(ecx, 23)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::POPCNT);
				if (CHECK_BIT(ecx, 28)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::AVX);
				// if (CHECK_BIT(edx, 23)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::MMX);
				if (CHECK_BIT(edx, 25)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSE);
				if (CHECK_BIT(edx, 26)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::SSE2);
			}

			if (max_id >= 7)
			{
				get_cpuid(7, info);
				const int ebx = info[1];
				if (CHECK_BIT(ebx, 3)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::BMI);
				if (CHECK_BIT(ebx, 5)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::AVX2);
				if (CHECK_BIT(ebx, 8)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::BMI2);
			}

			get_cpuid(0x80000000, info);
			const unsigned int max_ext_id = info[0];

			if (max_ext_id >= 0x80000001)
			{
				get_cpuid(0x80000001, info);
				const int ecx = info[2];
				if (CHECK_BIT(ecx, 5)) ful::cpuid_feature_cache |= static_cast<unsigned int>(ful::cpuid_feature::LZCNT);
			}
		}
	}
	static_hack;
}
