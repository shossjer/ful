#pragma once

#ifndef FUL_INCLUDED_STRING_HPP
# error string.hpp must be included before string_search.hpp
#endif

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern const char8 * ful_dispatch(find_unit_8_8)(const char8 * begin, const char8 * end, char8 c);
		extern const char8 * ful_dispatch(find_unit_8_16)(const char8 * begin, const char8 * end, char16 c);
		extern const char8 * ful_dispatch(find_unit_8_24)(const char8 * begin, const char8 * end, char_fast24 c);
		extern const char8 * ful_dispatch(find_unit_8_32)(const char8 * begin, const char8 * end, char32 c);
		extern const char16 * ful_dispatch(find_unit_16_16)(const char16 * begin, const char16 * end, char16 c);
		extern const char16 * ful_dispatch(find_unit_16_32)(const char16 * begin, const char16 * end, char32 c);
		extern const char32 * ful_dispatch(find_unit_32_32)(const char32 * begin, const char32 * end, char32 c);
#endif

		ful_generic() ful_inline
		const char8 * find_unit_8_8_generic(const char8 * begin, const char8 * end, char8 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(32u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(16u < size))
#else
			if (!ful_expect(8u < size))
#endif
				return end;

			extern const char8 * find_unit_8_8_generic_8(const char8 * begin, const char8 * end, char8 c);

			return find_unit_8_8_generic_8(begin, end, c);
		}

		ful_generic() ful_inline
		const char8 * find_unit_8_16_generic(const char8 * begin, const char8 * end, char16 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			extern const char8 * find_unit_8_16_generic_10(const char8 * begin, const char8 * end, char16 c, usize size);

			return find_unit_8_16_generic_10(begin, end, c, size);
		}

		ful_generic() ful_inline
		const char8 * find_unit_8_24_generic(const char8 * begin, const char8 * end, char_fast24 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			extern const char8 * find_unit_8_24_generic_10(const char8 * begin, const char8 * end, char_fast24 c, usize size);

			return find_unit_8_24_generic_10(begin, end, c, size);
		}

		ful_generic() ful_inline
		const char8 * find_unit_8_32_generic_0_26(const char8 * begin, const char8 * end, char32 c, usize size)
		{
			switch (size)
			{
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
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

				switch (end - begin)
				{
				case 10: goto case_10;
				case 9: goto case_9;
				case 8: goto case_8;
				case 7: goto case_7;
				case 6: goto case_6;
				case 5: goto case_5;
				case 4: goto case_4;
				case 3: goto case_3;
				default: ful_unreachable();
				}
			}
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			{
				if (*reinterpret_cast<const char32 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char32 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char32 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char32 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char32 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char32 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char32 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char32 *>(begin + 7) == c) return begin + 7;
				begin += 8;

				switch (end - begin)
				{
				case 10: goto case_10;
				case 9: goto case_9;
				case 8: goto case_8;
				case 7: goto case_7;
				case 6: goto case_6;
				case 5: goto case_5;
				case 4: goto case_4;
				case 3: goto case_3;
				default: ful_unreachable();
				}
			}
			case 10: case_10: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 9: case_9: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 8: case_8: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 7: case_7: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 6: case_6: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 5: case_5: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 4: case_4: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 3:
			case 2:
			case 1:
			case 0: case_3: return end;
			default: ful_unreachable();
			}
		}

		ful_generic() ful_inline
		const char8 * find_unit_8_32_generic(const char8 * begin, const char8 * end, char32 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(34u < size))
#else
			if (!ful_expect(26u < size))
#endif
				return end;

			extern const char8 * find_unit_8_32_generic_19(const char8 * begin, const char8 * end, char32 c);

			return find_unit_8_32_generic_19(begin, end, c);
		}

		ful_generic() ful_inline
		const char16 * find_unit_16_16_generic(const char16 * begin, const char16 * end, char16 c)
		{
			for (; begin != end; ++begin)
			{
				if (*begin == c)
					break;
			}
			return begin;
		}

		ful_generic() ful_inline
		const char16 * find_unit_16_32_generic(const char16 * begin, const char16 * end, char32 c)
		{
			const char16 * const end_c = end - 2;
			if (begin <= end_c)
			{
				do
				{
					if (*reinterpret_cast<const char32 *>(begin) == c)
						return begin;

					begin++;
				}
				while (begin <= end_c);
			}
			return end;
		}

		ful_generic() ful_inline
		const char32 * find_unit_32_32_generic(const char32 * begin, const char32 * end, char32 c)
		{
			for (; begin != end; ++begin)
			{
				if (*begin == c)
					break;
			}
			return begin;
		}


		ful_target("sse2") ful_inline
		const char8 * find_unit_8_8_sse2(const char8 * begin, const char8 * end, char8 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return end;

			extern const char8 * find_unit_8_8_sse2_16(const char8 * begin, const char8 * end, char8 c);

			return find_unit_8_8_sse2_16(begin, end, c);
		}

		ful_target("sse2") ful_inline
		const char8 * find_unit_8_16_sse2_9(const char8 * begin, const char8 * end, char16 c, usize size)
		{
			const __m128i c1281 = _mm_set1_epi8((uint8)c);
			const __m128i c1282 = _mm_set1_epi8((uint8)(c >> 8));

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

			if (begin_chunk == end_chunk)
			{
				const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
				const __m128i line2 = _mm_srli_si128(line1, 1);
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp) >> (begin - begin_chunk), static_cast<unsigned int>(size - 2));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
			else
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp), static_cast<unsigned int>(size - 2));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}

		ful_target("sse2") ful_inline
		const char8 * find_unit_8_16_sse2(const char8 * begin, const char8 * end, char16 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			if (size <= 17u)
			{
				return find_unit_8_16_sse2_9(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_16_sse2_17(const char8 * begin, const char8 * end, char16 c);

				return find_unit_8_16_sse2_17(begin, end, c);
			}
		}

		ful_target("sse2") ful_inline
		const char8 * find_unit_8_24_sse2_5_17(const char8 * begin, const char8 * end, char_fast24 c, usize size)
		{
			const __m128i c1281 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m128i c1282 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m128i c1283 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

			if (begin_chunk == end_chunk)
			{
				const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
				const __m128i line2 = _mm_srli_si128(line1, 1);
				const __m128i line3 = _mm_srli_si128(line1, 2);
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp) >> (begin - begin_chunk), static_cast<unsigned int>(size - 3));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
			else
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i line3 = _mm_srli_si128(line2, 1);
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp), static_cast<unsigned int>(size - 3));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}

		ful_target("sse2") ful_inline
		const char8 * find_unit_8_24_sse2(const char8 * begin, const char8 * end, char_fast24 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			if (size <= 17u)
			{
				return find_unit_8_24_sse2_5_17(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_24_sse2_18(const char8 * begin, const char8 * end, char_fast24 c);

				return find_unit_8_24_sse2_18(begin, end, c);
			}
		}

		ful_target("sse2") ful_inline
		const char8 * find_unit_8_32_sse2(const char8 * begin, const char8 * end, char32 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(34u < size))
#else
			if (!ful_expect(26u < size))
#endif
				return end;

			if (size <= 26u)
			{
				return find_unit_8_32_generic_0_26(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_32_sse2_19(const char8 * begin, const char8 * end, char32 c);

				return find_unit_8_32_sse2_19(begin, end, c);
			}
		}

		ful_target("sse2") ful_inline
		const char16 * find_unit_16_16_sse2(const char16 * begin, const char16 * end, char16 c)
		{
			return find_unit_16_16_generic(begin, end, c);
		}

		ful_target("sse2") ful_inline
		const char16 * find_unit_16_32_sse2(const char16 * begin, const char16 * end, char32 c)
		{
			return find_unit_16_32_generic(begin, end, c);
		}

		ful_target("sse2") ful_inline
		const char32 * find_unit_32_32_sse2(const char32 * begin, const char32 * end, char32 c)
		{
			return find_unit_32_32_generic(begin, end, c);
		}


		ful_target("avx2") ful_inline
		const char8 * find_unit_8_8_avx2_16(const char8 * begin, const char8 * end, char8 c, usize size)
		{
			const __m256i c256 = _mm256_set1_epi8(c);

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -32);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

			if (begin_chunk == end_chunk)
			{
				const __m256i line = _mm256_load_si256(reinterpret_cast<const __m256i *>(begin_chunk));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = set_higher_bits(_mm256_movemask_epi8(cmp) >> (begin - begin_chunk), static_cast<unsigned int>(size - 1));
				const unsigned int index = count_trailing_zero_bits(mask);
				return begin + index;
			}
			else
			{
				const __m256i line = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i cmp = _mm256_cmpeq_epi8(line, c256);
				const unsigned int mask = set_higher_bits(_mm256_movemask_epi8(cmp), static_cast<unsigned int>(size - 1));
				const unsigned int index = count_trailing_zero_bits(mask);
				return begin + index;
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_8_avx2(const char8 * begin, const char8 * end, char8 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return end;

			if (size <= 32u)
			{
				return find_unit_8_8_avx2_16(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_8_avx2_32(const char8 * begin, const char8 * end, char8 c);

				return find_unit_8_8_avx2_32(begin, end, c);
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_16_avx2_9(const char8 * begin, const char8 * end, char16 c, usize size)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(c >> 8));

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -32);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

			if (begin_chunk == end_chunk)
			{
				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(begin_chunk));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2_ = _mm256_cmpeq_epi8(line1, c2562);
				const unsigned int andcmp = _mm256_movemask_epi8(cmp1) & _mm256_movemask_epi8(cmp2_) >> 1;
				const unsigned int mask = zero_higher_bits(andcmp >> (begin - begin_chunk), static_cast<unsigned int>(size - 2));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
			else
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i andcmp = _mm256_and_si256(cmp1, cmp2);
				const unsigned int mask = zero_higher_bits(_mm256_movemask_epi8(andcmp), static_cast<unsigned int>(size - 2));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_16_avx2(const char8 * begin, const char8 * end, char16 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			if (size <= 33u)
			{
				return find_unit_8_16_avx2_9(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_16_avx2_33(const char8 * begin, const char8 * end, char16 c);

				return find_unit_8_16_avx2_33(begin, end, c);
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_24_avx2_5_33(const char8 * begin, const char8 * end, char_fast24 c, usize size)
		{
			const __m256i c2561 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
			const __m256i c2562 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -32);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

			if (begin_chunk == end_chunk)
			{
				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(begin_chunk));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2_ = _mm256_cmpeq_epi8(line1, c2562);
				const __m256i cmp3_ = _mm256_cmpeq_epi8(line1, c2563);
				const unsigned int andcmp = _mm256_movemask_epi8(cmp1) & _mm256_movemask_epi8(cmp2_) >> 1 & _mm256_movemask_epi8(cmp3_) >> 2;
				const unsigned int mask = zero_higher_bits(andcmp >> (begin - begin_chunk), static_cast<unsigned int>(size - 3));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
			else
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp12 = _mm256_and_si256(cmp1, cmp2);
				const __m256i cmp3_ = _mm256_cmpeq_epi8(line2, c2563);
				const unsigned int andcmp = _mm256_movemask_epi8(cmp12) & _mm256_movemask_epi8(cmp3_) >> 1;
				const unsigned int mask = zero_higher_bits(andcmp, static_cast<unsigned int>(size - 3));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_24_avx2(const char8 * begin, const char8 * end, char_fast24 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(33u < size))
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			if (!ful_expect(17u < size))
#else
			if (!ful_expect(9u < size))
#endif
				return end;

			if (size <= 33u)
			{
				return find_unit_8_24_avx2_5_33(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_24_avx2_34(const char8 * begin, const char8 * end, char_fast24 c);

				return find_unit_8_24_avx2_34(begin, end, c);
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_32_avx2_4_34(const char8 * begin, const char8 * end, char32 c, usize size)
		{
			const __m256i c2561 = _mm256_set1_epi8((uint8)c);
			const __m256i c2562 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 8));
			const __m256i c2563 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 16));
			const __m256i c2564 = _mm256_set1_epi8((uint8)(static_cast<uint32>(c) >> 24));

			const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -32);
			const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

			if (begin_chunk == end_chunk)
			{
				const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(begin_chunk));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2_ = _mm256_cmpeq_epi8(line1, c2562);
				const __m256i cmp3_ = _mm256_cmpeq_epi8(line1, c2563);
				const __m256i cmp4_ = _mm256_cmpeq_epi8(line1, c2564);
				const unsigned int andcmp = _mm256_movemask_epi8(cmp1) & _mm256_movemask_epi8(cmp2_) >> 1 & _mm256_movemask_epi8(cmp3_) >> 2 & _mm256_movemask_epi8(cmp4_) >> 3;
				const unsigned int mask = zero_higher_bits(andcmp >> (begin - begin_chunk), static_cast<unsigned int>(size - 4));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
			else
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 33));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp12 = _mm256_and_si256(cmp1, cmp2);
				const __m256i andcmp34 = _mm256_and_si256(cmp3, cmp4);
				const unsigned int mask = _mm256_movemask_epi8(andcmp12) & _mm256_movemask_epi8(andcmp34) >> (35 - size);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}

		ful_target("avx2") ful_inline
		const char8 * find_unit_8_32_avx2(const char8 * begin, const char8 * end, char32 c)
		{
			const usize size = end - begin;
#if defined(__AVX2__)
			if (!ful_expect(34u < size))
#else
			if (!ful_expect(26u < size))
#endif
				return end;

			if (size <= 26u)
			{
				return find_unit_8_32_generic_0_26(begin, end, c, size);
			}
			else if (size <= 34u)
			{
				return find_unit_8_32_avx2_4_34(begin, end, c, size);
			}
			else
			{
				extern const char8 * find_unit_8_32_avx2_35(const char8 * begin, const char8 * end, char32 c);

				return find_unit_8_32_avx2_35(begin, end, c);
			}
		}

		ful_target("avx2") ful_inline
		const char16 * find_unit_16_16_avx2(const char16 * begin, const char16 * end, char16 c)
		{
			return find_unit_16_16_generic(begin, end, c);
		}

		ful_target("avx2") ful_inline
		const char16 * find_unit_16_32_avx2(const char16 * begin, const char16 * end, char32 c)
		{
			return find_unit_16_32_generic(begin, end, c);
		}

		ful_target("avx2") ful_inline
		const char32 * find_unit_32_32_avx2(const char32 * begin, const char32 * end, char32 c)
		{
			return find_unit_32_32_generic(begin, end, c);
		}
	}

	// prevents implicit casts from inbuilt characters with single quotes
	// (e.g. 'a') to int/char32
	template <typename T>
	const T * find(T * from, T * to, char u) = delete;

	ful_inline const char8 * find(const char8 * begin, const char8 * end, char8 c)
	{
		const usize size = end - begin;
#if defined(__AVX2__)
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			switch (size)
			{
#if defined(__AVX2__)
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
				return detail::find_unit_8_8_avx2_16(begin, end, c, size);
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)c;

				const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(end - 8);

				uint64 index;
				if (least_significant_zero_byte(qword1 ^ bytes, index))
					return begin + index;

				index = least_significant_zero_byte(qword2 ^ bytes);
				return end - 8 + index;
			}
			case 8:
			case 7:
			case 6:
			case 5:
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)c;

				const uint64 qword = (static_cast<uint64>(*reinterpret_cast<const uint32 *>(end - 4)) << (size - 4)) | *reinterpret_cast<const uint32 *>(begin);

				const uint64 index = least_significant_zero_byte(qword ^ bytes);
				return begin + index;
			}
			case 4: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 3: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 2: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 1: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 0: return begin;
			default: ful_unreachable();
			}
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::find_unit_8_8(begin, end, c);
#elif defined(__AVX2__)
			return detail::find_unit_8_8_avx2(begin, end, c);
#elif defined(__SSE2__)
			return detail::find_unit_8_8_sse2(begin, end, c);
#else
			return detail::find_unit_8_8_generic(begin, end, c);
#endif
		}
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline const T * find(const T * begin, const T * end, char8 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char8 *>(begin), reinterpret_cast<const char8 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline T * find(T * begin, T * end, char8 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char8 * find(const char8 * begin, const char8 * end, char16 c)
	{
		const usize size = end - begin;
#if defined(__AVX2__)
		if (size <= 33u)
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		if (size <= 17u)
#else
		if (size <= 9u)
#endif
		{
			switch (size)
			{
#if defined(__AVX2__)
			case 33:
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			{
				return detail::find_unit_8_16_avx2_9(begin, end, c, size);
			}
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			{
				return detail::find_unit_8_16_sse2_9(begin, end, c, size);
			}
#endif
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
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::find_unit_8_16(begin, end, c);
#elif defined(__AVX2__)
			return detail::find_unit_8_16_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::find_unit_8_16_sse2(begin, end, c);
#else
			return detail::find_unit_8_16_generic(begin, end, c);
#endif
		}
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline const T * find(const T * begin, const T * end, char16 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char8 *>(begin), reinterpret_cast<const char8 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline T * find(T * begin, T * end, char16 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char8 * find(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const usize size = end - begin;
#if defined(__AVX2__)
		if (size <= 33u)
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		if (size <= 17u)
#else
		if (size <= 9u)
#endif
		{
			switch (size)
			{
#if defined(__AVX2__)
			case 33:
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			{
				return detail::find_unit_8_24_avx2_5_33(begin, end, c, size);
			}
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			{
				return detail::find_unit_8_24_sse2_5_17(begin, end, c, size);
			}
#endif
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
			case 2:
			case 1:
			case 0: return end;
			default: ful_unreachable();
			}
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::find_unit_8_24(begin, end, c);
#elif defined(__AVX2__)
			return detail::find_unit_8_24_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::find_unit_8_24_sse2(begin, end, c);
#else
			return detail::find_unit_8_24_generic(begin, end, c);
#endif
		}
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline const T * find(const T * begin, const T * end, char_fast24 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char8 *>(begin), reinterpret_cast<const char8 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline T * find(T * begin, T * end, char_fast24 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char8 * find(const char8 * begin, const char8 * end, char32 c)
	{
		const usize size = end - begin;
#if defined(__AVX2__)
		if (size <= 34u)
#else
		if (size <= 26u)
#endif
		{
			switch (size)
			{
#if defined(__AVX2__)
			case 34:
			case 33:
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			{
				return detail::find_unit_8_32_avx2_4_34(begin, end, c, size);
			}
#else
			case 26:
			case 25:
			case 24:
			case 23:
#endif
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			case 8:
			case 7:
			case 6:
			case 5:
			case 4:
			case 3:
			case 2:
			case 1:
			case 0:
			{
				return detail::find_unit_8_32_generic_0_26(begin, end, c, size);
			}
			default: ful_unreachable();
			}
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::find_unit_8_32(begin, end, c);
#elif defined(__AVX2__)
			return detail::find_unit_8_32_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::find_unit_8_32_sse2(begin, end, c);
#else
			return detail::find_unit_8_32_generic(begin, end, c);
#endif
		}
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline const T * find(const T * begin, const T * end, char32 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char8 *>(begin), reinterpret_cast<const char8 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char8))>
	ful_inline T * find(T * begin, T * end, char32 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char16 * find(const char16 * begin, const char16 * end, char16 c)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::find_unit_16_16(begin, end, c);
#elif defined(__AVX2__)
		return detail::find_unit_16_16_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::find_unit_16_16_sse2(begin, end, c);
#else
		return detail::find_unit_16_16_generic(begin, end, c);
#endif
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char16))>
	ful_inline const T * find(const T * begin, const T * end, char16 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char16 *>(begin), reinterpret_cast<const char16 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char16))>
	ful_inline T * find(T * begin, T * end, char16 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char16 * find(const char16 * begin, const char16 * end, char32 c)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::find_unit_16_32(begin, end, c);
#elif defined(__AVX2__)
		return detail::find_unit_16_32_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::find_unit_16_32_sse2(begin, end, c);
#else
		return detail::find_unit_16_32_generic(begin, end, c);
#endif
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char16))>
	ful_inline const T * find(const T * begin, const T * end, char32 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char16 *>(begin), reinterpret_cast<const char16 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char16))>
	ful_inline T * find(T * begin, T * end, char32 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	ful_inline const char32 * find(const char32 * begin, const char32 * end, char32 c)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::find_unit_32_32(begin, end, c);
#elif defined(__AVX2__)
		return detail::find_unit_32_32_avx2(begin, end, c);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::find_unit_32_32_sse2(begin, end, c);
#else
		return detail::find_unit_32_32_generic(begin, end, c);
#endif
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char32))>
	ful_inline const T * find(const T * begin, const T * end, char32 c)
	{
		return reinterpret_cast<const T *>(find(reinterpret_cast<const char32 *>(begin), reinterpret_cast<const char32 *>(end), c));
	}

	template <typename T, ful_requires(sizeof(T) == sizeof(char32))>
	ful_inline T * find(T * begin, T * end, char32 c)
	{
		return const_cast<T *>(find(const_cast<const T *>(begin), const_cast<const T *>(end), c));
	}

	template <typename T, unsigned long long N, typename C>
	ful_inline auto find(T (& x)[N], C c)
		-> decltype(find(x + 0, x + N, c))
	{
		return find(x + 0, x + N, c);
	}
}
