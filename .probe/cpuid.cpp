#include <iostream>

#if defined(_MSC_VER)
# include <intrin.h>
#elif defined(__GNUC__)
# include <cpuid.h>
#endif

#define CHECK_BIT(reg, bit) (reg & (1 << bit))

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

int main()
{
	reg_type info[4];

	get_cpuid(0, info);
	const unsigned int max_id = info[0];

	char name[13] = {};
	reinterpret_cast<int *>(name)[0] = info[1];
	reinterpret_cast<int *>(name)[1] = info[3]; // yes, this is correct
	reinterpret_cast<int *>(name)[2] = info[2];
	std::cout << name;

	if (max_id >= 1)
	{
		get_cpuid(1, info);
		const int eax = info[0];
		const int model = ((static_cast<unsigned int>(eax) >> 12) & 0xf0) | ((static_cast<unsigned int>(eax) >> 4) & 0x0f);
		const int family = ((static_cast<unsigned int>(eax) >> 16) & 0xf0) | ((static_cast<unsigned int>(eax) >> 8) & 0x0f);
		std::cout << "," << model << "," << family;
		const int ecx = info[2];
		const int edx = info[3];
		if (CHECK_BIT(ecx, 0)) std::cout << ";SSE3";
		if (CHECK_BIT(ecx, 9)) std::cout << ";SSSE3";
		if (CHECK_BIT(ecx, 19)) std::cout << ";SSE4_1";
		if (CHECK_BIT(ecx, 20)) std::cout << ";SSE4_2";
		if (CHECK_BIT(ecx, 23)) std::cout << ";POPCNT";
		if (CHECK_BIT(ecx, 28)) std::cout << ";AVX";
		// if (CHECK_BIT(edx, 23)) std::cout << ";MMX";
		if (CHECK_BIT(edx, 25)) std::cout << ";SSE";
		if (CHECK_BIT(edx, 26)) std::cout << ";SSE2";
	}

	if (max_id >= 7)
	{
		get_cpuid(7, info);
		const int ebx = info[1];
		if (CHECK_BIT(ebx, 3)) std::cout << ";BMI";
		if (CHECK_BIT(ebx, 5)) std::cout << ";AVX2";
		if (CHECK_BIT(ebx, 8)) std::cout << ";BMI2";
	}

	get_cpuid(0x80000000, info);
	const unsigned int max_ext_id = info[0];

	if (max_ext_id >= 0x80000001)
	{
		get_cpuid(0x80000001, info);
		const int ecx = info[2];
		if (CHECK_BIT(ecx, 5)) std::cout << ";LZCNT";
	}

	return 0;
}
