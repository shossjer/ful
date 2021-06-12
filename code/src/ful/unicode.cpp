#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "private/cpuid.hpp"

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"
#include "ful/point_utils.hpp"

#include "ful/unicode_none.hpp"
#include "ful/unicode_avx2.hpp"

using namespace ful;

ful_resolve(convert_8_16le, unit_utf16le *, const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(convert_8_16le, avx2, first, last, begin, end);
	return ful_cpucall(convert_8_16le, none, first, last, begin, end);
}

ful_resolve(convert_8_16be, unit_utf16be *, const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_16be, none, first, last, begin, end);
}

ful_resolve(convert_8_32le, unit_utf32le *, const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(convert_8_32le, avx2, first, last, begin, end);
	return ful_cpucall(convert_8_32le, none, first, last, begin, end);
}

ful_resolve(convert_8_32be, unit_utf32be *, const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_8_32be, none, first, last, begin, end);
}

ful_resolve(convert_16le_8, unit_utf8 *, const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_8, none, first, last, begin, end);
}

ful_resolve(convert_16le_16be, unit_utf16be *, const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_16be, none, first, last, begin, end);
}

ful_resolve(convert_16le_32le, unit_utf32le *, const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_32le, none, first, last, begin, end);
}

ful_resolve(convert_16le_32be, unit_utf32be *, const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16le_32be, none, first, last, begin, end);
}

ful_resolve(convert_16be_8, unit_utf8 *, const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_8, none, first, last, begin, end);
}

ful_resolve(convert_16be_16le, unit_utf16le *, const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_16le, none, first, last, begin, end);
}

ful_resolve(convert_16be_32le, unit_utf32le *, const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_32le, none, first, last, begin, end);
}

ful_resolve(convert_16be_32be, unit_utf32be *, const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_16be_32be, none, first, last, begin, end);
}

ful_resolve(convert_32le_8, unit_utf8 *, const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_8, none, first, last, begin, end);
}

ful_resolve(convert_32le_16le, unit_utf16le *, const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_16le, none, first, last, begin, end);
}

ful_resolve(convert_32le_16be, unit_utf16be *, const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_16be, none, first, last, begin, end);
}

ful_resolve(convert_32le_32be, unit_utf32be *, const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32le_32be, none, first, last, begin, end);
}

ful_resolve(convert_32be_8, unit_utf8 *, const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_8, none, first, last, begin, end);
}

ful_resolve(convert_32be_16le, unit_utf16le *, const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_16le, none, first, last, begin, end);
}

ful_resolve(convert_32be_16be, unit_utf16be *, const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_16be, none, first, last, begin, end);
}

ful_resolve(convert_32be_32le, unit_utf32le *, const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end)
{
	ful_cpuinit();
	return ful_cpucall(convert_32be_32le, none, first, last, begin, end);
}

ful_resolve(point_count, usize, const unit_utf8 * beg, const unit_utf8 * end)
{
	ful_cpuinit();
	return ful_cpucall(point_count, none, beg, end);
}

ful_resolve(point_next, const unit_utf8 *, const unit_utf8 * s, usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_next, none, s, n);
}

ful_resolve(point_prev, const unit_utf8 *, const unit_utf8 * s, usize n)
{
	ful_cpuinit();
	return ful_cpucall(point_prev, none, s, n);
}

#endif
