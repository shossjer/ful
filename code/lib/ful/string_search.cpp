#include "ful/string.hpp"
#include "ful/string_search.hpp"

#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "ful/cpuid.hpp"

ful_resolve(find_unit_8_8, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char8 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_8_8, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_8_8, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_8_8, generic, begin, end, c);
}

ful_resolve(find_unit_8_16, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char16 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_8_16, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_8_16, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_8_16, generic, begin, end, c);
}

ful_resolve(find_unit_8_24, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char_fast24 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_8_24, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_8_24, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_8_24, generic, begin, end, c);
}

ful_resolve(find_unit_8_32, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_8_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_8_32, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_8_32, generic, begin, end, c);
}

ful_resolve(find_unit_16_16, const ful::char16 *, const ful::char16 * begin, const ful::char16 * end, ful::char16 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_16_16, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_16_16, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_16_16, generic, begin, end, c);
}

ful_resolve(find_unit_16_32, const ful::char16 *, const ful::char16 * begin, const ful::char16 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_16_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_16_32, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_16_32, generic, begin, end, c);
}

ful_resolve(find_unit_32_32, const ful::char32 *, const ful::char32 * begin, const ful::char32 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(find_unit_32_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(find_unit_32_32, sse2, begin, end, c);
	else
		return ful_cpucall(find_unit_32_32, generic, begin, end, c);
}

ful_resolve(rfind_unit_8_8, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char8 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_8_8, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_8_8, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_8_8, generic, begin, end, c);
}

ful_resolve(rfind_unit_8_16, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char16 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_8_16, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_8_16, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_8_16, generic, begin, end, c);
}

ful_resolve(rfind_unit_8_24, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char_fast24 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_8_24, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_8_24, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_8_24, generic, begin, end, c);
}

ful_resolve(rfind_unit_8_32, const ful::char8 *, const ful::char8 * begin, const ful::char8 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_8_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_8_32, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_8_32, generic, begin, end, c);
}

ful_resolve(rfind_unit_16_16, const ful::char16 *, const ful::char16 * begin, const ful::char16 * end, ful::char16 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_16_16, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_16_16, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_16_16, generic, begin, end, c);
}

ful_resolve(rfind_unit_16_32, const ful::char16 *, const ful::char16 * begin, const ful::char16 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_16_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_16_32, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_16_32, generic, begin, end, c);
}

ful_resolve(rfind_unit_32_32, const ful::char32 *, const ful::char32 * begin, const ful::char32 * end, ful::char32 c)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(rfind_unit_32_32, avx2, begin, end, c);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(rfind_unit_32_32, sse2, begin, end, c);
	else
		return ful_cpucall(rfind_unit_32_32, generic, begin, end, c);
}

ful_resolve(strend_8, const ful::char8 *, const ful::char8 * cstr)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(strend_8, avx2, cstr);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(strend_8, sse2, cstr);
	else
		return ful_cpucall(strend_8, generic, cstr);
}

ful_resolve(strend_16, const ful::char16 *, const ful::char16 * cstr)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(strend_16, avx2, cstr);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(strend_16, sse2, cstr);
	else
		return ful_cpucall(strend_16, generic, cstr);
}

ful_resolve(strend_32, const ful::char32 *, const ful::char32 * cstr)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(strend_32, avx2, cstr);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(strend_32, sse2, cstr);
	else
		return ful_cpucall(strend_32, generic, cstr);
}

#endif

namespace ful
{
	namespace detail
	{
		const char8 * find_unit_8_8_generic_8(const char8 * begin, const char8 * end, char8 c)
		{
			const uint64 bytes = 0x0101010101010101u * (uint8)c;

			const char8 * const end_line = end - 8;
			do
			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(begin);

				uint64 index;
				if (least_significant_zero_byte(qword ^ bytes, index))
					return begin + index;

				begin += 8;
			}
			while (begin < end_line);

			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(end_line);

				const uint64 index = least_significant_zero_byte(qword ^ bytes);
				return end_line + index;
			}
		}

		const char8 * find_unit_8_16_generic_10(const char8 * begin, const char8 * end, char16 c, usize size)
		{
			if (size <= 37u) // somewhat arbitrary, see plot of "dump find_8_16 small"
			{
				switch (size)
				{
				case 37: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 36: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 35: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 34: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 33: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 32: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 31: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 30: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 29: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 28: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 27: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 26: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 25: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 24: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 23: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 22: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 21: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 20: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 19: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 18: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 17: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 16: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 15: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 14: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 13: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 12: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 11: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 10: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 9: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 8: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 7: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 6: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 5: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 4: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 3: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 2: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
				case 1:
				case 0: return end;
				default: ful_unreachable();
				}
			}
			else
			{
				const uint64 bytes1 = 0x0101010101010101u * (uint8)c;
				const uint64 bytes2 = 0x0101010101010101u * (uint8)(static_cast<uint16>(c) >> 8);

				const char8 * const end_line = end - 8 - 1;
				do
				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin + 1);

					uint64 index;
					if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
						return begin + index;

					begin += 8;
				}
				while (begin < end_line);

				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(end_line);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(end_line + 1);

					uint64 index;
					if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
						return end_line + index;

					return end_line + 8 + 1;
				}
			}
		}

		const char8 * find_unit_8_24_generic_10(const char8 * begin, const char8 * end, char_fast24 c, usize size)
		{
			if (size <= 35u) // somewhat arbitrary, see plot of "dump find_8_24 small"
			{
				switch (size)
				{
				case 35: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 34: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 33: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 32: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 31: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 30: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 29: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 28: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 27: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 26: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 25: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 24: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 23: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 22: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 21: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 20: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 19: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 18: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 17: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 16: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 15: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 14: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 13: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 12: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 11: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 10: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 9: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 8: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 7: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 6: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 5: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 4: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 3: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 2: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
				case 1:
				case 0: return end;
				default: ful_unreachable();
				}
			}
			else
			{
				const uint64 bytes1 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c));
				const uint64 bytes2 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 8);
				const uint64 bytes3 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 16);

				const char8 * const end_line = end - 8 - 2;
				do
				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin + 1);
					const uint64 qword3 = *reinterpret_cast<const uint64 *>(begin + 2);

					uint64 index;
					if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
						return begin + index;

					begin += 8;
				}
				while (begin < end_line);

				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(end_line);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(end_line + 1);
					const uint64 qword3 = *reinterpret_cast<const uint64 *>(end_line + 2);

					uint64 index;
					if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
						return end_line + index;

					return end_line + 8 + 2;
				}
			}
		}

		const char8 * find_unit_8_32_generic_19(const char8 * begin, const char8 * end, char32 c)
		{
			const char8 * const end_line = end - 16 - 3;
			do
			{
				if (*reinterpret_cast<const char32 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char32 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char32 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char32 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char32 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char32 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char32 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char32 *>(begin + 7) == c) return begin + 7;
				if (*reinterpret_cast<const char32 *>(begin + 8) == c) return begin + 8;
				if (*reinterpret_cast<const char32 *>(begin + 9) == c) return begin + 9;
				if (*reinterpret_cast<const char32 *>(begin + 10) == c) return begin + 10;
				if (*reinterpret_cast<const char32 *>(begin + 11) == c) return begin + 11;
				if (*reinterpret_cast<const char32 *>(begin + 12) == c) return begin + 12;
				if (*reinterpret_cast<const char32 *>(begin + 13) == c) return begin + 13;
				if (*reinterpret_cast<const char32 *>(begin + 14) == c) return begin + 14;
				if (*reinterpret_cast<const char32 *>(begin + 15) == c) return begin + 15;
				begin += 16;
			}
			while (begin <= end_line);

			switch (begin - end_line)
			{
			case 1: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 2: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 3: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 4: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 5: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 6: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 7: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 8: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 9: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 10: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 11: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 12: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 13: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 14: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 15: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 16: return end;
			default: ful_unreachable();
			}
		}

		ful_target("sse2")
		const char8 * find_unit_8_8_sse2_16(const char8 * begin, const char8 * end, char8 c)
		{
			const __m128i c128 = _mm_set1_epi8(c);

			const char8 * const end_line = end - 16;
			do
			{
				const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i cmp = _mm_cmpeq_epi8(line, c128);
				const unsigned int mask = _mm_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}

				begin += 16;
			}
			while (begin < end_line);

			{
				const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i cmp = _mm_cmpeq_epi8(line, c128);
				const unsigned int mask = _mm_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("sse2")
		const char8 * find_unit_8_16_sse2_17(const char8 * begin, const char8 * end, char16 c)
		{
			const __m128i c1281 = _mm_set1_epi8((uint8)c);
			const __m128i c1282 = _mm_set1_epi8((uint8)(static_cast<uint16>(c) >> 8));

			const char8 * const end_line = end - 17;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}

				begin += 16;
			}
			while (begin < end_line);

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("sse2")
		const char8 * find_unit_8_24_sse2_18(const char8 * begin, const char8 * end, char_fast24 c)
		{
			const __m128i c1281 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m128i c1282 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const end_line = end - 16 - 2;
			if (begin < end_line)
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
					const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 2));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
					const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);
			}

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 2));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("sse2")
		const char8 * find_unit_8_32_sse2_19(const char8 * begin, const char8 * end, char32 c)
		{
			const __m128i c1281 = _mm_set1_epi8((uint8)c);
			const __m128i c1282 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 16));
			const __m128i c1284 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 24));

			const char8 * const end_line = end - 16 - 3;
			if (begin < end_line)
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
					const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 2));
					const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 3));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
					const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
					const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);
			}

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 2));
				const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 3));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("avx2")
		const char8 * find_unit_8_8_avx2_32(const char8 * begin, const char8 * end, char8 c)
		{
			const __m256i c256 = _mm256_set1_epi8(c);

			const char8 * const end_line = end - 32;
			do
			{
				const __m256i line = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = _mm256_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}

				begin += 32;
			}
			while (begin < end_line);

			{
				const __m256i line = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = _mm256_movemask_epi8(cmp);
				const unsigned int index = count_trailing_zero_bits(mask, tag_popcnt); // todo tag_bmi
				return end_line + index;
			}
		}

		ful_target("avx2")
		const char8 * find_unit_8_16_avx2_33(const char8 * begin, const char8 * end, char16 c)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(static_cast<uint16>(c) >> 8));

			const char8 * const end_line = end - 32 - 1;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}

				begin += 32;
			}
			while (begin < end_line);

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 1));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("avx2")
		const char8 * find_unit_8_24_avx2_34(const char8 * begin, const char8 * end, char_fast24 c)
		{
			const __m256i c2561 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const end_line = end - 32 - 2;
			if (begin < end_line)
			{
				do
				{
					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
					const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 2));
					const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
					const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
					const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
					const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), cmp3);
					const unsigned int mask = _mm256_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 32;
				}
				while (begin < end_line);
			}

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 2));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		ful_target("avx2")
		const char8 * find_unit_8_32_avx2_35(const char8 * begin, const char8 * end, char32 c)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 16));
			const __m256i c2564 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 24));

			const char8 * const end_line = end - 32 - 3;
			if (begin < end_line)
			{
				do
				{
					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
					const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 2));
					const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 3));
					const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
					const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
					const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
					const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
					const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
					const unsigned int mask = _mm256_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 32;
				}
				while (begin < end_line);
			}

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 2));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 3));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}

		const char8 * rfind_unit_8_8_generic_8(const char8 * begin, const char8 * end, char8 c, const char8 * def)
		{
			const uint64 bytes = 0x0101010101010101u * (uint8)c;

			const char8 * const begin_line = begin + 8 + 0;
			do
			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(end - 8 - 0);

				uint64 index;
				if (most_significant_zero_byte(qword ^ bytes, index))
					return end - 8 - 0 + index;

				end -= 8;
			}
			while (begin_line < end);

			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(begin_line - 8 - 0);

				uint64 index;
				if (most_significant_zero_byte(qword ^ bytes, index))
					return end - 8 - 0 + index;
			}
			return def;
		}

		const char8 * rfind_unit_8_16_generic_10(const char8 * begin, const char8 * end, char16 c, usize size, const char8 * def)
		{
			if (size <= 37u) // somewhat arbitrary, see plot of "dump find_8_16 small"
			{
				switch (size)
				{
				case 37: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 36: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 35: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 34: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 33: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 32: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 31: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 30: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 29: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 28: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 27: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 26: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 25: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 24: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 23: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 22: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 21: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 20: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 19: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 18: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 17: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 16: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 15: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 14: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 13: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 12: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 11: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 10: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 9: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 8: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 7: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 6: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 5: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 4: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 3: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 2: if (*reinterpret_cast<const char16 *>(end - 2) == c) return end - 2; end--; ful_fallthrough;
				case 1:
				case 0: return def;
				default: ful_unreachable();
				}
			}
			else
			{
				const uint64 bytes1 = 0x0101010101010101u * (uint8)c;
				const uint64 bytes2 = 0x0101010101010101u * (uint8)(static_cast<uint16>(c) >> 8);

				const char8 * const begin_line = begin + 8 + 1;
				do
				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(end - 8 - 1);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(end - 8 - 0);

					uint64 index;
					if (most_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
						return end - 8 - 1 + index;

					end -= 8;
				}
				while (begin_line < end);

				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin_line - 8 - 1);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin_line - 8 - 0);

					uint64 index;
					if (most_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
						return begin_line - 8 - 1 + index;
				}
				return def;
			}
		}

		const char8 * rfind_unit_8_24_generic_10(const char8 * begin, const char8 * end, char_fast24 c, usize size, const char8 * def)
		{
			if (size <= 35u) // somewhat arbitrary, see plot of "dump find_8_24 small"
			{
				switch (size)
				{
				case 35: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 34: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 33: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 32: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 31: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 30: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 29: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 28: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 27: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 26: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 25: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 24: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 23: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 22: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 21: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 20: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 19: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 18: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 17: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 16: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 15: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 14: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 13: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 12: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 11: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 10: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 9: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 8: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 7: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 6: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 5: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 4: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 3: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 2: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(end - 3)) == c) return end - 3; end--; ful_fallthrough;
				case 1:
				case 0: return def;
				default: ful_unreachable();
				}
			}
			else
			{
				const uint64 bytes1 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c));
				const uint64 bytes2 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 8);
				const uint64 bytes3 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 16);

				const char8 * const begin_line = begin + 8 + 2;
				do
				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(end - 8 - 2);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(end - 8 - 1);
					const uint64 qword3 = *reinterpret_cast<const uint64 *>(end - 8 - 0);

					uint64 index;
					if (most_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
						return end - 8 - 2 + index;

					end -= 8;
				}
				while (begin_line < end);

				{
					const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin_line - 8 - 2);
					const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin_line - 8 - 1);
					const uint64 qword3 = *reinterpret_cast<const uint64 *>(begin_line - 8 - 0);

					uint64 index;
					if (most_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
						return begin_line - 8 - 2 + index;
				}
				return def;
			}
		}

		const char8 * rfind_unit_8_32_generic_19(const char8 * begin, const char8 * end, char32 c, const char8 * def)
		{
			const char8 * const begin_line = begin + 16 + 3;
			do
			{
				if (*reinterpret_cast<const char32 *>(end - 4 - 0) == c) return end - 4 - 0;
				if (*reinterpret_cast<const char32 *>(end - 4 - 1) == c) return end - 4 - 1;
				if (*reinterpret_cast<const char32 *>(end - 4 - 2) == c) return end - 4 - 2;
				if (*reinterpret_cast<const char32 *>(end - 4 - 3) == c) return end - 4 - 3;
				if (*reinterpret_cast<const char32 *>(end - 4 - 4) == c) return end - 4 - 4;
				if (*reinterpret_cast<const char32 *>(end - 4 - 5) == c) return end - 4 - 5;
				if (*reinterpret_cast<const char32 *>(end - 4 - 6) == c) return end - 4 - 6;
				if (*reinterpret_cast<const char32 *>(end - 4 - 7) == c) return end - 4 - 7;
				if (*reinterpret_cast<const char32 *>(end - 4 - 8) == c) return end - 4 - 8;
				if (*reinterpret_cast<const char32 *>(end - 4 - 9) == c) return end - 4 - 9;
				if (*reinterpret_cast<const char32 *>(end - 4 - 10) == c) return end - 4 - 10;
				if (*reinterpret_cast<const char32 *>(end - 4 - 11) == c) return end - 4 - 11;
				if (*reinterpret_cast<const char32 *>(end - 4 - 12) == c) return end - 4 - 12;
				if (*reinterpret_cast<const char32 *>(end - 4 - 13) == c) return end - 4 - 13;
				if (*reinterpret_cast<const char32 *>(end - 4 - 14) == c) return end - 4 - 14;
				if (*reinterpret_cast<const char32 *>(end - 4 - 15) == c) return end - 4 - 15;
				end -= 16;
			}
			while (begin_line <= end);

			switch (begin_line - end)
			{
			case 1: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 2: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 3: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 4: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 5: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 6: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 7: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 8: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 9: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 10: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 11: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 12: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 13: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 14: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 15: if (*reinterpret_cast<const char32 *>(end - 4) == c) return end - 4; end--; ful_fallthrough;
			case 16: return def;
			default: ful_unreachable();
			}
		}

		ful_target("sse2")
		const char8 * rfind_unit_8_8_sse2_16(const char8 * begin, const char8 * end, char8 c, const char8 * def)
		{
			const __m128i c128 = _mm_set1_epi8(c);

			const char8 * const begin_line = begin + 16 + 0;
			do
			{
				const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 0));
				const __m128i cmp = _mm_cmpeq_epi8(line, c128);
				const unsigned int mask = _mm_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 16 - 0 + index;
				}

				end -= 16;
			}
			while (begin_line < end);

			{
				const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 0));
				const __m128i cmp = _mm_cmpeq_epi8(line, c128);
				const unsigned int mask = _mm_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 16 - 0 + index;
				}
			}
			return def;
		}

		ful_target("sse2")
		const char8 * rfind_unit_8_16_sse2_17(const char8 * begin, const char8 * end, char16 c, const char8 * def)
		{
			const __m128i c1281 = _mm_set1_epi8((uint8)c);
			const __m128i c1282 = _mm_set1_epi8((uint8)(static_cast<uint16>(c) >> 8));

			const char8 * const begin_line = begin + 16 + 1;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 1));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 16 - 1 + index;
				}

				end -= 16;
			}
			while (begin_line < end);

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 1));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 16 - 1 + index;
				}
			}
			return def;
		}

		ful_target("sse2")
		const char8 * rfind_unit_8_24_sse2_18(const char8 * begin, const char8 * end, char_fast24 c, const char8 * def)
		{
			const __m128i c1281 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m128i c1282 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const begin_line = begin + 16 + 2;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 2));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 16 - 2 + index;
				}

				end -= 16;
			}
			while (begin_line < end);

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 2));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 16 - 2 + index;
				}
			}
			return def;
		}

		ful_target("sse2")
		const char8 * rfind_unit_8_32_sse2_19(const char8 * begin, const char8 * end, char32 c, const char8 * def)
		{
			const __m128i c1281 = _mm_set1_epi8((uint8)c);
			const __m128i c1282 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 16));
			const __m128i c1284 = _mm_set1_epi8((uint8)(static_cast<uint32>(c) >> 24));

			const char8 * const begin_line = begin + 16 + 3;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 3));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 2));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 1));
				const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 16 - 3 + index;
				}

				end -= 16;
			}
			while (begin_line < end);

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 3));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 2));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 1));
				const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin_line - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 16 - 3 + index;
				}
			}
			return def;
		}

		ful_target("avx2")
		const char8 * rfind_unit_8_8_avx2_32(const char8 * begin, const char8 * end, char8 c, const char8 * def)
		{
			const __m256i c256 = _mm256_set1_epi8(c);

			const char8 * const begin_line = begin + 32 + 0;
			do
			{
				const __m256i line = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 0));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = _mm256_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 32 - 0 + index;
				}

				end -= 32;
			}
			while (begin_line < end);

			{
				const __m256i line = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 0));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = _mm256_movemask_epi8(cmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 32 - 0 + index;
				}
			}
			return def;
		}

		ful_target("avx2")
		const char8 * rfind_unit_8_16_avx2_33(const char8 * begin, const char8 * end, char16 c, const char8 * def)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(static_cast<uint16>(c) >> 8));

			const char8 * const begin_line = begin + 32 + 1;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 1));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 32 - 1 + index;
				}

				end -= 32;
			}
			while (begin_line < end);

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 1));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 32 - 1 + index;
				}
			}
			return def;
		}

		ful_target("avx2")
		const char8 * rfind_unit_8_24_avx2_34(const char8 * begin, const char8 * end, char_fast24 c, const char8 * def)
		{
			const __m256i c2561 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const begin_line = begin + 32 + 2;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 2));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 32 - 2 + index;
				}

				end -= 32;
			}
			while (begin_line < end);

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 2));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 32 - 2 + index;
				}
			}
			return def;
		}

		ful_target("avx2")
		const char8 * rfind_unit_8_32_avx2_35(const char8 * begin, const char8 * end, char32 c, const char8 * def)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 16));
			const __m256i c2564 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 24));

			const char8 * const begin_line = begin + 32 + 3;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 3));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 2));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 1));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return end - 32 - 3 + index;
				}

				end -= 32;
			}
			while (begin_line < end);

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 3));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 2));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 1));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin_line - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 32 - 3 + index;
				}
			}
			return def;
		}
	}
}
