#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "ful/cpuid.hpp"

#include "ful/unicode_convert.hpp"

ful_resolve(convert_8_16le, ful::unit_utf16le *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(convert_8_16le, avx2, first, last, begin, end);
	else
		return ful_cpucall(convert_8_16le, none, first, last, begin, end);
}

ful_resolve(convert_8_16be, ful::unit_utf16be *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16be * begin, ful::unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_16be, none, first, last, begin, end);
}

ful_resolve(convert_8_32le, ful::unit_utf32le *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(convert_8_32le, avx2, first, last, begin, end);
	else
		return ful_cpucall(convert_8_32le, none, first, last, begin, end);
}

ful_resolve(convert_8_32be, ful::unit_utf32be *, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32be * begin, ful::unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_32be, none, first, last, begin, end);
}

ful_resolve(convert_16le_8, ful::unit_utf8 *, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_8, none, first, last, begin, end);
}

ful_resolve(convert_16le_16be, ful::unit_utf16be *, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf16be * begin, ful::unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_16be, none, first, last, begin, end);
}

ful_resolve(convert_16le_32le, ful::unit_utf32le *, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_32le, none, first, last, begin, end);
}

ful_resolve(convert_16le_32be, ful::unit_utf32be *, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf32be * begin, ful::unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_32be, none, first, last, begin, end);
}

ful_resolve(convert_16be_8, ful::unit_utf8 *, const ful::unit_utf16be * first, const ful::unit_utf16be * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_8, none, first, last, begin, end);
}

ful_resolve(convert_16be_16le, ful::unit_utf16le *, const ful::unit_utf16be * first, const ful::unit_utf16be * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_16le, none, first, last, begin, end);
}

ful_resolve(convert_16be_32le, ful::unit_utf32le *, const ful::unit_utf16be * first, const ful::unit_utf16be * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_32le, none, first, last, begin, end);
}

ful_resolve(convert_16be_32be, ful::unit_utf32be *, const ful::unit_utf16be * first, const ful::unit_utf16be * last, ful::unit_utf32be * begin, ful::unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_32be, none, first, last, begin, end);
}

ful_resolve(convert_32le_8, ful::unit_utf8 *, const ful::unit_utf32le * first, const ful::unit_utf32le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_8, none, first, last, begin, end);
}

ful_resolve(convert_32le_16le, ful::unit_utf16le *, const ful::unit_utf32le * first, const ful::unit_utf32le * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_16le, none, first, last, begin, end);
}

ful_resolve(convert_32le_16be, ful::unit_utf16be *, const ful::unit_utf32le * first, const ful::unit_utf32le * last, ful::unit_utf16be * begin, ful::unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_16be, none, first, last, begin, end);
}

ful_resolve(convert_32le_32be, ful::unit_utf32be *, const ful::unit_utf32le * first, const ful::unit_utf32le * last, ful::unit_utf32be * begin, ful::unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_32be, none, first, last, begin, end);
}

ful_resolve(convert_32be_8, ful::unit_utf8 *, const ful::unit_utf32be * first, const ful::unit_utf32be * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_8, none, first, last, begin, end);
}

ful_resolve(convert_32be_16le, ful::unit_utf16le *, const ful::unit_utf32be * first, const ful::unit_utf32be * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_16le, none, first, last, begin, end);
}

ful_resolve(convert_32be_16be, ful::unit_utf16be *, const ful::unit_utf32be * first, const ful::unit_utf32be * last, ful::unit_utf16be * begin, ful::unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_16be, none, first, last, begin, end);
}

ful_resolve(convert_32be_32le, ful::unit_utf32le *, const ful::unit_utf32be * first, const ful::unit_utf32be * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_32le, none, first, last, begin, end);
}

#endif
