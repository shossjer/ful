#include "ful/cpuid.hpp"
#include "ful/string.hpp"
#include "ful/string_search.hpp"

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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint16>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint16>(c) >> 8));

			const char8 * const end_line = end - 17;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));

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
					const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));
			const __m128i c1284 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 24));

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
					const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
				const unsigned int index = count_trailing_zero_bits(mask, tag_popcnt); // todo tag_bmi
				return end_line + index;
			}
		}

		ful_target("avx2")
		const char8 * find_unit_8_16_avx2_33(const char8 * begin, const char8 * end, char16 c)
		{
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint16>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint16>(c) >> 8));

			const char8 * const end_line = end - 32 - 1;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));

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
					const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));
			const __m256i c2564 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 24));

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
					const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint16>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint16>(c) >> 8));

			const char8 * const begin_line = begin + 16 + 1;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 1));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16 - 0));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));

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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
			const __m128i c1281 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m128i c1282 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));
			const __m128i c1284 = _mm_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 24));

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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
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
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint16>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint16>(c) >> 8));

			const char8 * const begin_line = begin + 32 + 1;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 1));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32 - 0));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));

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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
			const __m256i c2561 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) & 0xff));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 16));
			const __m256i c2564 = _mm256_set1_epi8(static_cast<char>(static_cast<uint32>(c) >> 24));

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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
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
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(andcmp));
				if (mask != 0)
				{
					const unsigned int index = most_significant_set_bit(mask);
					return begin_line - 32 - 3 + index;
				}
			}
			return def;
		}

		const char8 * strend_8_generic_aligned(const char8 * cstr)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (8 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 8 / 1;

				const uint64 qword = *reinterpret_cast<const uint64 *>(cstr - 8 / 1);

				uint64 index;
				if (least_significant_zero_byte(qword, index))
					return cstr - 8 / 1 + index;
			}
		}

		const char8 * strend_8_generic_unaligned(const char8 * cstr, usize offset)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 8))
				return cstr;

			// note the first 8 has already been read

			cstr = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(to_bytes(cstr) + 8) & static_cast<puint>(-8));

			return strend_8_generic_aligned(cstr);
		}

		const char8 * strend_8_generic_boundary(const char8 * cstr, usize offset)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 8) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			cstr = reinterpret_cast<const char8 *>(to_bytes(cstr) - offset);

			const uint64 qword = *reinterpret_cast<const uint64 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8));

			uint64 index;
			if (least_significant_zero_byte(qword | low_mask((offset - (FUL_PAGE_SIZE - 8)) * 8), index))
				return reinterpret_cast<const char8 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + index * 1);

			return strend_8_generic_aligned(reinterpret_cast<const char8 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + 8));
		}

		ful_target("sse2")
		const char8 * strend_8_sse2_aligned(const char8 * cstr, __m128i zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (64 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 64 / 1;

				const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 64 / 1));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 48 / 1));
				const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 32 / 1));
				const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 16 / 1));
				const __m128i min1234 = _mm_min_epu8(_mm_min_epu8(line1, line2), _mm_min_epu8(line3, line4));
				const __m128i cmp1234 = _mm_cmpeq_epi8(min1234, zero);
				const unsigned int mask1234 = static_cast<unsigned int>(_mm_movemask_epi8(cmp1234));
				if (mask1234 != 0)
				{
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, zero);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, zero);
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, zero);
					const __m128i cmp4 = _mm_cmpeq_epi8(line4, zero);
					const uint64 mask1 = static_cast<unsigned int>(_mm_movemask_epi8(cmp1));
					const uint64 mask2 = static_cast<unsigned int>(_mm_movemask_epi8(cmp2));
					const uint64 mask3 = static_cast<unsigned int>(_mm_movemask_epi8(cmp3));
					const uint64 mask4 = static_cast<unsigned int>(_mm_movemask_epi8(cmp4));
					const uint64 mask = (mask4 << 48) | (mask3 << 32) | (mask2 << 16) | mask1;
					const uint64 index = least_significant_set_bit(mask);
					return reinterpret_cast<const char8 *>(to_bytes(cstr) + index * 1) - 64 / 1;
				}
			}
		}

		ful_target("sse2")
		const char8 * strend_8_sse2_unaligned(const char8 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 16))
				return cstr;

			// note the first 16 has already been read

			const char8 * const aligned = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(to_bytes(cstr) + (64 - 1)) & static_cast<puint>(-64));
			cstr = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(to_bytes(cstr) + 16) & static_cast<puint>(-16));

			if (cstr < aligned)
			{
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
				}
				cstr += 16 / 1;

				if (cstr < aligned)
				{
					const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
					}
					cstr += 16 / 1;

					if (cstr < aligned)
					{
						const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
						const __m128i cmp4 = _mm_cmpeq_epi8(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_8_sse2_aligned(aligned, zero);
		}

		ful_target("sse2")
		const char8 * strend_8_sse2_boundary(const char8 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 16) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char8 * const aligned = reinterpret_cast<const char8 *>(to_bytes(cstr) - offset);

			const __m128i line = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 16)));
			const __m128i cmp = _mm_cmpeq_epi8(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 16));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
			}

			return strend_8_sse2_aligned(reinterpret_cast<const char8 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}

		ful_target("avx2")
		const char8 * strend_8_avx2_aligned(const char8 * cstr, __m256i ful_big_value zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (128 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 128 / 1;

				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 128 / 1));
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 96 / 1));
				const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 64 / 1));
				const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 32 / 1));
				const __m256i min = _mm256_min_epu8(_mm256_min_epu8(line1, line2), _mm256_min_epu8(line3, line4));
				const __m256i cmp = _mm256_cmpeq_epi8(min, zero);
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
				if (mask != 0)
				{
					const __m256i cmp1 = _mm256_cmpeq_epi8(line1, zero);
					const __m256i cmp2 = _mm256_cmpeq_epi8(line2, zero);
					const __m256i cmp3 = _mm256_cmpeq_epi8(line3, zero);
					const __m256i cmp4 = _mm256_cmpeq_epi8(line4, zero);
					const uint64 mask1 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp1));
					const uint64 mask2 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp2));
					const uint64 mask3 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp3));
					const uint64 mask4 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp4));
					const uint64 mask12 = (mask2 << 32) | mask1;
					if (mask12 != 0)
					{
						const uint64 index = least_significant_set_bit(mask12);
						return reinterpret_cast<const char8 *>(to_bytes(cstr) + index * 1) - 128 / 1;
					}
					const uint64 mask34 = (mask4 << 32) | mask3;
					// if (ful_expect(mask34 != 0))
					{
						const uint64 index = least_significant_set_bit(mask34);
						return reinterpret_cast<const char8 *>(to_bytes(cstr) + index * 1) - 128 / 1 + 64 / 1;
					}
				}
			}
		}

		ful_target("avx2")
		const char8 * strend_8_avx2_unaligned(const char8 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 32))
				return cstr;

			// note the first 32 has already been read

			const char8 * const aligned = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(to_bytes(cstr) + (128 - 1)) & static_cast<puint>(-128));
			cstr = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(to_bytes(cstr) + 32) & static_cast<puint>(-32));

			if (cstr < aligned)
			{
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
				}
				cstr += 32 / 1;

				if (cstr < aligned)
				{
					const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
					const __m256i cmp3 = _mm256_cmpeq_epi8(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
					}
					cstr += 32 / 1;

					if (cstr < aligned)
					{
						const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
						const __m256i cmp4 = _mm256_cmpeq_epi8(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_8_avx2_aligned(aligned, zero);
		}

		ful_target("avx2")
		const char8 * strend_8_avx2_boundary(const char8 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 32) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char8 * const aligned = reinterpret_cast<const char8 *>(to_bytes(cstr) - offset);

			const __m256i line = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 32)));
			const __m256i cmp = _mm256_cmpeq_epi8(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 32));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char8 *>(to_bytes(cstr) + index);
			}

			return strend_8_avx2_aligned(reinterpret_cast<const char8 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}

		const char16 * strend_16_generic_aligned(const char16 * cstr)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (8 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 8 / 2;

				const uint64 qword = *reinterpret_cast<const uint64 *>(cstr - 8 / 2);

				uint64 index;
				if (least_significant_zero_a16(qword, index))
					return cstr - 8 / 2 + index;
			}
		}

		const char16 * strend_16_generic_unaligned(const char16 * cstr, usize offset)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 8))
				return cstr;

			// note the first 8 has already been read

			cstr = reinterpret_cast<const char16 *>(reinterpret_cast<puint>(to_bytes(cstr) + 8) & static_cast<puint>(-8));

			return strend_16_generic_aligned(cstr);
		}

		const char16 * strend_16_generic_boundary(const char16 * cstr, usize offset)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 8) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			cstr = reinterpret_cast<const char16 *>(to_bytes(cstr) - offset);

			const uint64 qword = *reinterpret_cast<const uint64 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8));

			uint64 index;
			if (least_significant_zero_a16(qword | low_mask((offset - (FUL_PAGE_SIZE - 8)) * 8), index))
				return reinterpret_cast<const char16 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + index * 2);

			return strend_16_generic_aligned(reinterpret_cast<const char16 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + 8));
		}

		ful_target("sse2")
		const char16 * strend_16_sse2_aligned(const char16 * cstr, __m128i zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (64 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 64 / 2;

				const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 64 / 2));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 48 / 2));
				const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 32 / 2));
				const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 16 / 2));
				const __m128i pack12 = _mm_packs_epi16(line1, line2);
				const __m128i pack34 = _mm_packs_epi16(line3, line4);
				const __m128i min1234 = _mm_min_epu8(pack12, pack34);
				const __m128i cmp1234 = _mm_cmpeq_epi8(min1234, zero);
				const unsigned int mask1234 = static_cast<unsigned int>(_mm_movemask_epi8(cmp1234));
				if (mask1234 != 0)
				{
					const __m128i cmp12 = _mm_cmpeq_epi8(pack12, zero);
					const __m128i cmp34 = _mm_cmpeq_epi8(pack34, zero);
					const unsigned int mask12 = static_cast<unsigned int>(_mm_movemask_epi8(cmp12));
					const unsigned int mask34 = static_cast<unsigned int>(_mm_movemask_epi8(cmp34));
					const unsigned int mask = (mask34 << 16) | mask12;
					const unsigned int index = least_significant_set_bit(mask);
					return reinterpret_cast<const char16 *>(to_bytes(cstr) + index * 2) - 64 / 2;
				}
			}
		}

		ful_target("sse2")
		const char16 * strend_16_sse2_unaligned(const char16 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 16))
				return cstr;

			// note the first 16 has already been read

			const char16 * const aligned = reinterpret_cast<const char16 *>(reinterpret_cast<puint>(to_bytes(cstr) + (64 - 1)) & static_cast<puint>(-64));
			cstr = reinterpret_cast<const char16 *>(reinterpret_cast<puint>(to_bytes(cstr) + 16) & static_cast<puint>(-16));

			if (cstr < aligned)
			{
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
				const __m128i cmp2 = _mm_cmpeq_epi16(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
				}
				cstr += 16 / 2;

				if (cstr < aligned)
				{
					const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
					const __m128i cmp3 = _mm_cmpeq_epi16(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
					}
					cstr += 16 / 2;

					if (cstr < aligned)
					{
						const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
						const __m128i cmp4 = _mm_cmpeq_epi16(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_16_sse2_aligned(aligned, zero);
		}

		ful_target("sse2")
		const char16 * strend_16_sse2_boundary(const char16 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 16) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char16 * const aligned = reinterpret_cast<const char16 *>(to_bytes(cstr) - offset);

			const __m128i line = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 16)));
			const __m128i cmp = _mm_cmpeq_epi16(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 16));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
			}

			return strend_16_sse2_aligned(reinterpret_cast<const char16 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}

		ful_target("avx2")
		const char16 * strend_16_avx2_aligned(const char16 * cstr, __m256i ful_big_value zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (128 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 128 / 2;

				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 128 / 2));
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 96 / 2));
				const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 64 / 2));
				const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 32 / 2));
				const __m256i min = _mm256_min_epu16(_mm256_min_epu16(line1, line2), _mm256_min_epu16(line3, line4));
				const __m256i cmp = _mm256_cmpeq_epi16(min, zero);
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
				if (mask != 0)
				{
					const __m256i pack12 = _mm256_packs_epi16(line1, line2);
					const __m256i pack34 = _mm256_packs_epi16(line3, line4);
					const __m256i perm12 = _mm256_permute4x64_epi64(pack12, 0xd8);
					const __m256i perm34 = _mm256_permute4x64_epi64(pack34, 0xd8);
					const __m256i cmp12 = _mm256_cmpeq_epi8(perm12, zero);
					const __m256i cmp34 = _mm256_cmpeq_epi8(perm34, zero);
					const uint64 mask12 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp12));
					const uint64 mask34 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp34));
					const uint64 mask1234 = (mask34 << 32) | mask12;
					const uint64 index = least_significant_set_bit(mask1234);
					return reinterpret_cast<const char16 *>(to_bytes(cstr) + index * 2) - 128 / 2;
				}
			}
		}

		ful_target("avx2")
		const char16 * strend_16_avx2_unaligned(const char16 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 32))
				return cstr;

			// note the first 32 has already been read

			const char16 * const aligned = reinterpret_cast<const char16 *>(reinterpret_cast<puint>(to_bytes(cstr) + (128 - 1)) & static_cast<puint>(-128));
			cstr = reinterpret_cast<const char16 *>(reinterpret_cast<puint>(to_bytes(cstr) + 32) & static_cast<puint>(-32));

			if (cstr < aligned)
			{
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
				const __m256i cmp2 = _mm256_cmpeq_epi16(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
				}
				cstr += 32 / 2;

				if (cstr < aligned)
				{
					const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
					const __m256i cmp3 = _mm256_cmpeq_epi16(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
					}
					cstr += 32 / 2;

					if (cstr < aligned)
					{
						const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
						const __m256i cmp4 = _mm256_cmpeq_epi16(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_16_avx2_aligned(aligned, zero);
		}

		ful_target("avx2")
		const char16 * strend_16_avx2_boundary(const char16 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 32) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char16 * const aligned = reinterpret_cast<const char16 *>(to_bytes(cstr) - offset);

			const __m256i line = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 32)));
			const __m256i cmp = _mm256_cmpeq_epi16(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 32));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char16 *>(to_bytes(cstr) + index);
			}

			return strend_16_avx2_aligned(reinterpret_cast<const char16 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}

		const char32 * strend_32_generic_aligned(const char32 * cstr)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (8 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 8 / 4;

				const uint64 qword = *reinterpret_cast<const uint64 *>(cstr - 8 / 4);

				uint64 index;
				if (least_significant_zero_a32(qword, index))
					return cstr - 8 / 4 + index;
			}
		}

		const char32 * strend_32_generic_unaligned(const char32 * cstr, usize offset)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 8))
				return cstr;

			// note the first 8 has already been read

			cstr = reinterpret_cast<const char32 *>(reinterpret_cast<puint>(to_bytes(cstr) + 8) & static_cast<puint>(-8));

			return strend_32_generic_aligned(cstr);
		}

		const char32 * strend_32_generic_boundary(const char32 * cstr, usize offset)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 8) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			cstr = reinterpret_cast<const char32 *>(to_bytes(cstr) - offset);

			const uint64 qword = *reinterpret_cast<const uint64 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8));

			uint64 index;
			if (least_significant_zero_a32(qword | low_mask((offset - (FUL_PAGE_SIZE - 8)) * 8), index))
				return reinterpret_cast<const char32 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + index * 4);

			return strend_32_generic_aligned(reinterpret_cast<const char32 *>(to_bytes(cstr) + (FUL_PAGE_SIZE - 8) + 8));
		}

		ful_target("sse2")
		const char32 * strend_32_sse2_aligned(const char32 * cstr, __m128i zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (64 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 64 / 4;

				const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 64 / 4));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 48 / 4));
				const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 32 / 4));
				const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(cstr - 16 / 4));
				const __m128i pack = _mm_packs_epi16(_mm_packs_epi32(line1, line2), _mm_packs_epi32(line3, line4));
				const __m128i cmp = _mm_cmpeq_epi8(pack, zero);
				const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return reinterpret_cast<const char32 *>(to_bytes(cstr) + index * 4) - 64 / 4;
				}
			}
		}

		ful_target("sse2")
		const char32 * strend_32_sse2_unaligned(const char32 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 16))
				return cstr;

			// note the first 16 has already been read

			const char32 * const aligned = reinterpret_cast<const char32 *>(reinterpret_cast<puint>(to_bytes(cstr) + (64 - 1)) & static_cast<puint>(-64));
			cstr = reinterpret_cast<const char32 *>(reinterpret_cast<puint>(to_bytes(cstr) + 16) & static_cast<puint>(-16));

			if (cstr < aligned)
			{
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
				const __m128i cmp2 = _mm_cmpeq_epi32(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
				}
				cstr += 16 / 4;

				if (cstr < aligned)
				{
					const __m128i line3 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
					const __m128i cmp3 = _mm_cmpeq_epi32(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
					}
					cstr += 16 / 4;

					if (cstr < aligned)
					{
						const __m128i line4 = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(cstr)));
						const __m128i cmp4 = _mm_cmpeq_epi32(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_32_sse2_aligned(aligned, zero);
		}

		ful_target("sse2")
		const char32 * strend_32_sse2_boundary(const char32 * cstr, usize offset, __m128i zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 16) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char32 * const aligned = reinterpret_cast<const char32 *>(to_bytes(cstr) - offset);

			const __m128i line = _mm_load_si128(reinterpret_cast<const __m128i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 16)));
			const __m128i cmp = _mm_cmpeq_epi32(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 16));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
			}

			return strend_32_sse2_aligned(reinterpret_cast<const char32 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}

		ful_target("avx2")
		const char32 * strend_32_avx2_aligned(const char32 * cstr, __m256i ful_big_value zero)
		{
			if (!ful_expect((reinterpret_cast<puint>(cstr) & (128 - 1)) == 0))
				return cstr;

			while (true)
			{
				cstr += 128 / 4;

				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 128 / 4));
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 96 / 4));
				const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 64 / 4));
				const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(cstr - 32 / 4));
				const __m256i min = _mm256_min_epu32(_mm256_min_epu32(line1, line2), _mm256_min_epu32(line3, line4));
				const __m256i cmp = _mm256_cmpeq_epi32(min, zero);
				const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp));
				if (mask != 0)
				{
					const __m256i pack13 = _mm256_packs_epi32(line1, line3);
					const __m256i pack24 = _mm256_packs_epi32(line2, line4);
					const __m256i perm13 = _mm256_permute4x64_epi64(pack13, 0xd8);
					const __m256i perm24 = _mm256_permute4x64_epi64(pack24, 0xd8);
					const __m256i pack1234 = _mm256_packs_epi16(perm13, perm24);
					const __m256i cmp1234 = _mm256_cmpeq_epi8(pack1234, zero);
					const unsigned int mask1234 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp1234));
					const unsigned int index = least_significant_set_bit(mask1234);
					return reinterpret_cast<const char32 *>(to_bytes(cstr) + index * 4) - 128 / 4;
				}
			}
		}

		ful_target("avx2")
		const char32 * strend_32_avx2_unaligned(const char32 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset <= FUL_PAGE_SIZE - 32))
				return cstr;

			// note the first 32 has already been read

			const char32 * const aligned = reinterpret_cast<const char32 *>(reinterpret_cast<puint>(to_bytes(cstr) + (128 - 1)) & static_cast<puint>(-128));
			cstr = reinterpret_cast<const char32 *>(reinterpret_cast<puint>(to_bytes(cstr) + 32) & static_cast<puint>(-32));

			if (cstr < aligned)
			{
				const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
				const __m256i cmp2 = _mm256_cmpeq_epi32(line2, zero);
				const unsigned int mask2 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp2));
				if (mask2 != 0)
				{
					const unsigned int index = least_significant_set_bit(mask2);
					return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
				}
				cstr += 32 / 4;

				if (cstr < aligned)
				{
					const __m256i line3 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
					const __m256i cmp3 = _mm256_cmpeq_epi32(line3, zero);
					const unsigned int mask3 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp3));
					if (mask3 != 0)
					{
						const unsigned int index = least_significant_set_bit(mask3);
						return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
					}
					cstr += 32 / 4;

					if (cstr < aligned)
					{
						const __m256i line4 = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(cstr)));
						const __m256i cmp4 = _mm256_cmpeq_epi32(line4, zero);
						const unsigned int mask4 = static_cast<unsigned int>(_mm256_movemask_epi8(cmp4));
						if (mask4 != 0)
						{
							const unsigned int index = least_significant_set_bit(mask4);
							return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
						}
					}
				}
			}

			return strend_32_avx2_aligned(aligned, zero);
		}

		ful_target("avx2")
		const char32 * strend_32_avx2_boundary(const char32 * cstr, usize offset, __m256i ful_big_value zero)
		{
			if (!ful_expect(offset > FUL_PAGE_SIZE - 32) ||
			    !ful_expect(offset < FUL_PAGE_SIZE))
				return cstr;

			const char32 * const aligned = reinterpret_cast<const char32 *>(to_bytes(cstr) - offset);

			const __m256i line = _mm256_load_si256(reinterpret_cast<const __m256i *>(to_bytes(aligned) + (FUL_PAGE_SIZE - 32)));
			const __m256i cmp = _mm256_cmpeq_epi32(line, zero);
			const unsigned int mask = static_cast<unsigned int>(_mm256_movemask_epi8(cmp)) >> (offset - (FUL_PAGE_SIZE - 32));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return reinterpret_cast<const char32 *>(to_bytes(cstr) + index);
			}

			return strend_32_avx2_aligned(reinterpret_cast<const char32 *>(to_bytes(aligned) + FUL_PAGE_SIZE), zero);
		}
	}
}
