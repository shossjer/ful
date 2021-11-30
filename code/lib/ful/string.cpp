#include "ful/string.hpp"

#if defined(FUL_IFUNC) || defined(FUL_FPTR)

#include "ful/cpuid.hpp"

//#include "ful/compiler.hpp"
//#include "ful/stdint.hpp"
//#include "ful/types.hpp"

//#include "ful/intrinsics.hpp"

// todo remove
#include "ful/string_none.hpp"
#include "ful/string_sse.hpp"
#include "ful/string_sse2.hpp"
#include "ful/string_avx.hpp"
#include "ful/string_avx2.hpp"

using namespace ful;

ful_resolve(memcopy, byte *, const byte * first, const byte * last, byte * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memcopy, avx, first, last, begin);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memcopy, sse2, first, last, begin);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memcopy, sse, first, last, begin);
	else
		return ful_cpucall(memcopy, none, first, last, begin);
}

ful_resolve(memmovef, byte *, const byte * first, const byte * last, byte * end)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memmovef, avx, first, last, end);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memmovef, sse2, first, last, end);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memmovef, sse, first, last, end);
	else
		return ful_cpucall(memmovef, none, first, last, end);
}

ful_resolve(memmover, byte *, const byte * first, const byte * last, byte * begin)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memmover, avx, first, last, begin);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memmover, sse2, first, last, begin);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memmover, sse, first, last, begin);
	else
		return ful_cpucall(memmover, none, first, last, begin);
}

ful_resolve(memswap, byte *, byte * beg1, byte * end1, byte * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memswap, avx, beg1, end1, beg2);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memswap, sse2, beg1, end1, beg2);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memswap, sse, beg1, end1, beg2);
	else
		return ful_cpucall(memswap, none, beg1, end1, beg2);
}

ful_resolve(memset8, void, char8 * from, char8 * to, char8 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset8, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset8, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset8, sse, from, to, u);
	else
		return ful_cpucall(memset8, none, from, to, u);
}

ful_resolve(memset16, void, char16 * from, char16 * to, char16 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset16, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset16, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset16, sse, from, to, u);
	else
		return ful_cpucall(memset16, none, from, to, u);
}

ful_resolve(memset24, void, char24 * from, char24 * to, char_fast24 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset24, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset24, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset24, sse, from, to, u);
	else
		return ful_cpucall(memset24, none, from, to, u);
}

ful_resolve(memset32, void, char32 * from, char32 * to, char32 u)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX))
		return ful_cpucall(memset32, avx, from, to, u);
	else if (ful_cpucheck(SSE2))
		return ful_cpucall(memset32, sse2, from, to, u);
	else if (ful_cpucheck(SSE))
		return ful_cpucall(memset32, sse, from, to, u);
	else
		return ful_cpucall(memset32, none, from, to, u);
}

ful_resolve(equal_cstr, bool, const byte * beg1, const byte * end1, const byte * beg2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(equal_cstr, avx2, beg1, end1, beg2);
	if (ful_cpucheck(AVX))
		return ful_cpucall(equal_cstr, avx, beg1, end1, beg2);
	if (ful_cpucheck(SSE2))
		return ful_cpucall(equal_cstr, sse2, beg1, end1, beg2);
	if (ful_cpucheck(SSE))
		return ful_cpucall(equal_cstr, sse, beg1, end1, beg2);
	else
		return ful_cpucall(equal_cstr, generic, beg1, end1, beg2);
}

ful_resolve(equal_range, bool, const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
{
	ful_cpuinit();
	if (ful_cpucheck(AVX2))
		return ful_cpucall(equal_range, avx2, beg1, end1, beg2, end2);
	if (ful_cpucheck(AVX))
		return ful_cpucall(equal_range, avx, beg1, end1, beg2, end2);
	if (ful_cpucheck(SSE2))
		return ful_cpucall(equal_range, sse2, beg1, end1, beg2, end2);
	if (ful_cpucheck(SSE))
		return ful_cpucall(equal_range, sse, beg1, end1, beg2, end2);
	else
		return ful_cpucall(equal_range, generic, beg1, end1, beg2, end2);
}

#endif

namespace ful
{
	namespace detail
	{
		bool equal_cstr_generic_8(const byte * beg1, usize size, const byte * beg2)
		{
			ful::usize index = 0;
			if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }

			const ful::usize end_qword = size - 8;
			if (index < end_qword)
			{
				do
				{
					if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index))
						return false;

					index += 8;
				}
				while (index < end_qword);
			}

			if (*reinterpret_cast<const ful::char64 *>(beg1 + end_qword) != *reinterpret_cast<const ful::char64 *>(beg2 + end_qword))
				return false;

			return beg2[end_qword + 8] == ful::byte{};
		}

		ful_target("sse")
		bool equal_cstr_sse_16(const byte * beg1, usize size, const byte * beg2)
		{
			ful::usize index = 0;
			if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

			const ful::usize end_line = size - 16;
			if (index < end_line)
			{
				do
				{
					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
					const __m128 line2 = _mm_load_ps(reinterpret_cast<const float *>(beg2 + index));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;

					index += 16;
				}
				while (index < end_line);
			}

			const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_line));
			const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_line));
			const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
			const int mask = _mm_movemask_ps(cmpeq);
			if (mask != 0x0000000f)
				return false;

			return beg2[end_line + 16] == ful::byte{};
		}

		ful_target("sse2")
		bool equal_cstr_sse2_16(const byte * beg1, usize size, const byte * beg2)
		{
			ful::usize index = 0;
			if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

			const ful::usize end_line = size - 16;
			if (index < end_line)
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
					const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;

					index += 16;
				}
				while (index < end_line);
			}

			const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
			const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
			const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
			const int mask = _mm_movemask_epi8(cmpeq);
			if (mask != 0x0000ffff)
				return false;

			return beg2[end_line + 16] == ful::byte{};
		}

		ful_target("avx")
		bool equal_cstr_avx_32(const byte * beg1, usize size, const byte * beg2)
		{
			ful::usize index = 0;
			if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x10)
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;

				index += 16;
			}

			const ful::usize end_line = size - 32;
			if (index < end_line)
			{
				do
				{
					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
					const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(beg2 + index));
					const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
					if (_mm256_testz_si256(line_xor, line_xor) == 0)
						return false;

					index += 32;
				}
				while (index < end_line);
			}

			const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_line));
			const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_line));
			const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
			if (_mm256_testz_si256(line_xor, line_xor) == 0)
				return false;

			return beg2[end_line + 32] == ful::byte{};
		}

		ful_target("avx2")
		bool equal_cstr_avx2_32(const byte * beg1, usize size, const byte * beg2)
		{
			ful::usize index = 0;
			if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }
			if (reinterpret_cast<puint>(beg2 + index) & 0x10)
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;

				index += 16;
			}

			const ful::usize end_line = size - 32;
			if (index < end_line)
			{
				do
				{
					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
					const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(beg2 + index));
					const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
					const int mask = _mm256_movemask_epi8(line_cmpeq);
					if (mask != static_cast<int>(0xffffffff))
						return false;

					index += 32;
				}
				while (index < end_line);
			}

			const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_line));
			const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_line));
			const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
			const int mask = _mm256_movemask_epi8(line_cmpeq);
			if (mask != static_cast<int>(0xffffffff))
				return false;

			return beg2[end_line + 32] == ful::byte{};
		}

		bool equal_range_generic_9(const byte * beg1, usize size, const byte * beg2)
		{
			const ful::usize end_size = size - 8;

			ful::usize index = 0;
			do
			{
				if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index))
					return false;

				index += 8;
			}
			while (index < end_size);

			if (*reinterpret_cast<const ful::char64 *>(beg1 + end_size) != *reinterpret_cast<const ful::char64 *>(beg2 + end_size))
				return false;

			return true;
		}

		ful_target("sse")
		bool equal_range_sse_17(const byte * beg1, usize size, const byte * beg2)
		{
			const ful::usize end_size = size - 16;

			ful::usize index = 0;
			do
			{
				const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
				const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index));
				const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
				const int mask = _mm_movemask_ps(cmpeq);
				if (mask != 0x0000000f)
					return false;

				index += 16;
			}
			while (index < end_size);

			const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_size));
			const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_size));
			const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
			const int mask = _mm_movemask_ps(cmpeq);
			if (mask != 0x0000000f)
				return false;

			return true;
		}

		ful_target("sse2")
		bool equal_range_sse2_17(const byte * beg1, usize size, const byte * beg2)
		{
			const ful::usize end_size = size - 16;

			ful::usize index = 0;
			do
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;

				index += 16;
			}
			while (index < end_size);

			const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_size));
			const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_size));
			const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
			const int mask = _mm_movemask_epi8(cmpeq);
			if (mask != 0x0000ffff)
				return false;

			return true;
		}

		ful_target("avx")
		bool equal_range_avx_33(const byte * beg1, usize size, const byte * beg2)
		{
			const ful::usize end_size = size - 32;

			ful::usize index = 0;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
				const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
				if (_mm256_testz_si256(line_xor, line_xor) == 0)
					return false;

				index += 32;
			}
			while (index < end_size);

			const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_size));
			const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_size));
			const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
			if (_mm256_testz_si256(line_xor, line_xor) == 0)
				return false;

			return true;
		}

		ful_target("avx2")
		bool equal_range_avx2_33(const byte * beg1, usize size, const byte * beg2)
		{
			const ful::usize end_size = size - 32;

			ful::usize index = 0;
			do
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
				const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
				const int mask = _mm256_movemask_epi8(line_cmpeq);
				if (mask != static_cast<int>(0xffffffff))
					return false;

				index += 32;
			}
			while (index < end_size);

			const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_size));
			const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_size));
			const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
			const int mask = _mm256_movemask_epi8(line_cmpeq);
			if (mask != static_cast<int>(0xffffffff))
				return false;

			return true;
		}
	}
}
