#include "ful/cpuid.cpp"

#include <iostream>

#define CHECK_FEATURE(name) if (ful::cpuid_supports(ful::cpuid_feature::name)) std::cout << ";" #name

int main()
{
	std::cout << "cpuname"; // todo

	// CHECK_FEATURE(MMX);
	CHECK_FEATURE(SSE);
	CHECK_FEATURE(SSE2);
	CHECK_FEATURE(SSE3);
	CHECK_FEATURE(SSSE3);
	CHECK_FEATURE(SSE4_1);
	CHECK_FEATURE(SSE4_2);
	CHECK_FEATURE(POPCNT);
	CHECK_FEATURE(AVX);
	CHECK_FEATURE(AVX2);
	CHECK_FEATURE(BMI);
	CHECK_FEATURE(BMI2);
	CHECK_FEATURE(LZCNT);

	return 0;
}
