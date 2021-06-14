#pragma once

// #include "ful/intrinsics.hpp"
// #include "ful/stdint.hpp"
// #include "ful/unicode.hpp"

// #include <immintrin.h>

namespace ful
{
	namespace detail
	{
		ful_target("avx2") inline
		__m256i rotate(__m256i ab, unsigned int n)
		{
			ful_expect(n < 32);

			static const signed char table[] = {
				 0,  1,  2,  3,  4,  5,  6,  7,
				 8,  9, 10, 11, 12, 13, 14, 15,
				-128, -128, -128, -128, -128, -128, -128, -128,
				-128, -128, -128, -128, -128, -128, -128, -128,
				 0,  1,  2,  3,  4,  5,  6,  7,
				 8,  9, 10, 11, 12, 13, 14, 15,
				-128, -128, -128, -128, -128, -128, -128, -128,
				-128, -128, -128, -128, -128, -128, -128, -128,
			};

			const __m256i table_0 = _mm256_castsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i *>(table + n)));
			const __m256i ab_mask = _mm256_permute2x128_si256(table_0, table_0, 0x00);
			const __m256i table_16 = _mm256_castsi128_si256(_mm_loadu_si128(reinterpret_cast<const __m128i *>(table + 16 + n)));
			const __m256i ba_mask = _mm256_permute2x128_si256(table_16, table_16, 0x00);

			const __m256i ba = _mm256_permute2x128_si256(ab, ab, 1);
			const __m256i ab_shuf = _mm256_shuffle_epi8(ab, ab_mask);
			const __m256i ba_shuf = _mm256_shuffle_epi8(ba, ba_mask);
			return _mm256_or_si256(ab_shuf, ba_shuf);
		}

		ful_target("avx2") ful_inline __m256i rotate(__m256i ab, int n) { return rotate(ab, static_cast<unsigned int>(n) & (32 - 1)); }

		ful_target("avx2") inline
		const unit_utf8 * point_prev_avx2(const unit_utf8 * s, ssize n)
		{
			ful_expect(0 < n);

			alignas(32) static const signed char m65[] = {
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
			};

			--s;
			const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -32);
			const unsigned int offset = reinterpret_cast<puint>(s) & (32 - 1);

			__m256i cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
			unsigned int mask = zero_higher_bits(_mm256_movemask_epi8(cmpi), offset);
			while (true)
			{
				const unsigned int npoints = popcnt(mask);

				n -= npoints;
				if (n <= 0)
					break;

				word -= 32;

				cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
				mask = _mm256_movemask_epi8(cmpi);
			}

			const unsigned int i = index_set_bit(mask, static_cast<unsigned int>(-n)); // -n < 32
			return word + i;
		}

		ful_target("avx2") inline
		bool equal_cstr_avx2(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_expect(beg1 != end1);

			const unit_utf8 * beg2_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg2) & -32);

			const unsigned int beg2_offset = reinterpret_cast<puint>(beg2) & (32 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<puint>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const __m256i word2 = rotate(*reinterpret_cast<const __m256i *>(beg2_word), beg2_offset);

				const auto remaining = (end1 - 1) - beg1;
				const unsigned int word_end = beg2_offset ^ (32 - 1);
				const unsigned int last_in_word = remaining < word_end ? static_cast<int>(remaining) : word_end;

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = set_higher_bits(_mm256_movemask_epi8(cmpi), last_in_word);
				if (mask != static_cast<unsigned int>(-1))
					return false;

				if (remaining < word_end)
					return beg2[end1 - beg1] == '\0';

				beg1 += last_in_word + 1;
				beg2_word += 32;
			}

			const unit_utf8 * const end1_full_word = end1 - 32;
			while (beg1 <= end1_full_word)
			{
				const __m256i word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				const __m256i word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm256_movemask_epi8(cmpi);
				if (mask != static_cast<unsigned int>(-1))
					return false;

				beg1 += 32;
				beg2_word += 32;
			}

			if (end1 - beg1 != 0)
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<puint>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const __m256i word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const unsigned int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 32
				const unsigned int end_bits = static_cast<unsigned int>(-1) << align_end;

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm256_movemask_epi8(cmpi) | end_bits;
				if (mask != static_cast<unsigned int>(-1))
					return false;
			}

			return beg2_word[end1 - beg1] == '\0';
		}

		ful_target("avx2") inline
		bool less_cstr_avx2(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_expect(beg1 != end1);

			const unit_utf8 * beg2_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg2) & -32);

			const unsigned int beg2_offset = reinterpret_cast<puint>(beg2) & (32 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<puint>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const __m256i word2 = rotate(*reinterpret_cast<const __m256i *>(beg2_word), beg2_offset);

				const auto remaining = (end1 - 1) - beg1;
				const unsigned int word_end = beg2_offset ^ (32 - 1);
				const unsigned int remaining_up_to_word_end = remaining < word_end ? static_cast<int>(remaining) : word_end;

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = set_higher_bits(_mm256_movemask_epi8(cmpi), remaining_up_to_word_end);
				if (mask != static_cast<unsigned int>(-1))
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2[i];
				}

				beg1 += remaining_up_to_word_end + 1;
				beg2_word += 32;
			}

			const unit_utf8 * const end1_full_word = end1 - 32;
			while (beg1 <= end1_full_word)
			{
				const __m256i word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				const __m256i word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm256_movemask_epi8(cmpi);
				if (mask != static_cast<unsigned int>(-1))
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2_word[i];
				}

				beg1 += 32;
				beg2_word += 32;
			}

			if (end1 - beg1 != 0)
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<puint>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const __m256i word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const unsigned int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 32
				const unsigned int end_bits = static_cast<unsigned int>(-1) << align_end;

				const __m256i cmpi = _mm256_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm256_movemask_epi8(cmpi) | end_bits;
				if (mask != static_cast<unsigned int>(-1))
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2_word[i];
				}
			}

			return beg2_word[end1 - beg1] != '\0';
		}

		ful_target("avx2") inline
		const char8 * find_unit_8_avx2(const char8 * beg, const char8 * end, char8 c)
		{
			ful_expect(beg != end);

			const char8 * beg_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(beg) & -32);
			const char8 * const end_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

			const unsigned int beg_offset = reinterpret_cast<puint>(beg) & (32 - 1);

			const __m256i cveci = _mm256_set1_epi8(c);

			__m256i cmpi = _mm256_cmpeq_epi8(cveci, *reinterpret_cast<const __m256i *>(beg_word));
			unsigned int mask = zero_low_bits(_mm256_movemask_epi8(cmpi), beg_offset);

			while (beg_word != end_word)
			{
				if (mask != 0)
					goto found;

				beg_word += 32;

				cmpi = _mm256_cmpeq_epi8(cveci, *reinterpret_cast<const __m256i *>(beg_word));
				mask = _mm256_movemask_epi8(cmpi);
			}

			{
				const unsigned int end_offset = reinterpret_cast<puint>(end - 1) & (32 - 1);
				mask = set_higher_bits(mask, end_offset);
			}

		found:
			const unsigned int i = count_trailing_zeros(mask);
			return beg_word + i;
		}
	}
}
