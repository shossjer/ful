#include "ful/convert.hpp"
#include "ful/cpuid.hpp"

ful_resolve(convert_8_16, ful::unit_utf16 *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(SSE2))
		return ful_cpucall(convert_8_16, sse2, first, last, begin);
	else
		return ful_cpucall(convert_8_16, none, first, last, begin);
}

ful_resolve(convert_8_61, ful::unit_utf61 *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf61 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_61, none, first, last, begin);
}

ful_resolve(convert_8_32, ful::unit_utf32 *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(SSE2))
		return ful_cpucall(convert_8_32, sse2, first, last, begin);
	else
		return ful_cpucall(convert_8_32, none, first, last, begin);
}

ful_resolve(convert_8_23, ful::unit_utf23 *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf23 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_23, none, first, last, begin);
}

ful_resolve(convert_16_8, ful::unit_utf8 *, const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf8 * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(SSE2))
		return ful_cpucall(convert_16_8, sse2, first, last, begin);
	else
		return ful_cpucall(convert_16_8, none, first, last, begin);
}

ful_resolve(convert_16_61, ful::unit_utf61 *, const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf61 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_16_61, none, first, last, begin);
}

ful_resolve(convert_16_32, ful::unit_utf32 *, const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf32 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_16_32, none, first, last, begin);
}

ful_resolve(convert_16_23, ful::unit_utf23 *, const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf23 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_16_23, none, first, last, begin);
}

ful_resolve(convert_61_8, ful::unit_utf8 *, const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf8 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_61_8, none, first, last, begin);
}

ful_resolve(convert_61_16, ful::unit_utf16 *, const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf16 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_61_16, none, first, last, begin);
}

ful_resolve(convert_61_32, ful::unit_utf32 *, const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf32 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_61_32, none, first, last, begin);
}

ful_resolve(convert_61_23, ful::unit_utf23 *, const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf23 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_61_23, none, first, last, begin);
}

ful_resolve(convert_32_8, ful::unit_utf8 *, const ful::unit_utf32 * first, const ful::unit_utf32 * last, ful::unit_utf8 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_32_8, none, first, last, begin);
}

ful_resolve(convert_32_16, ful::unit_utf16 *, const ful::unit_utf32 * first, const ful::unit_utf32 * last, ful::unit_utf16 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_32_16, none, first, last, begin);
}

ful_resolve(convert_32_61, ful::unit_utf61 *, const ful::unit_utf32 * first, const ful::unit_utf32 * last, ful::unit_utf61 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_32_61, none, first, last, begin);
}

ful_resolve(convert_32_23, ful::unit_utf23 *, const ful::unit_utf32 * first, const ful::unit_utf32 * last, ful::unit_utf23 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_32_23, none, first, last, begin);
}

ful_resolve(convert_23_8, ful::unit_utf8 *, const ful::unit_utf23 * first, const ful::unit_utf23 * last, ful::unit_utf8 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_23_8, none, first, last, begin);
}

ful_resolve(convert_23_16, ful::unit_utf16 *, const ful::unit_utf23 * first, const ful::unit_utf23 * last, ful::unit_utf16 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_23_16, none, first, last, begin);
}

ful_resolve(convert_23_61, ful::unit_utf61 *, const ful::unit_utf23 * first, const ful::unit_utf23 * last, ful::unit_utf61 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_23_61, none, first, last, begin);
}

ful_resolve(convert_23_32, ful::unit_utf32 *, const ful::unit_utf23 * first, const ful::unit_utf23 * last, ful::unit_utf32 * begin)
{
	ful_cpuinit();
	return ful_cpucall(convert_23_32, none, first, last, begin);
}
