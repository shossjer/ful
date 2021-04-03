#pragma once

#if defined(__AVX2__)

// #include "ful/intrinsics.hpp"
// #include "ful/stdint.hpp"
// #include "ful/unicode.hpp"

// #include <immintrin.h>

namespace ful
{
	namespace detail
	{
		inline __m256i rotate(__m256i ab, unsigned int n)
		{
			ful_assume(n < 32);

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

			const __m256i ab_mask = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(table + n), reinterpret_cast<const __m128i *>(table + n));
			const __m256i ba_mask = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(table + 16 + n), reinterpret_cast<const __m128i *>(table + 16 + n));

			const __m256i ba = _mm256_permute2x128_si256(ab, ab, 1);
			const __m256i ab_shuf = _mm256_shuffle_epi8(ab, ab_mask);
			const __m256i ba_shuf = _mm256_shuffle_epi8(ba, ba_mask);
			return _mm256_or_si256(ab_shuf, ba_shuf);
		}

		inline __m256i rotate(__m256i ab, int n) { return rotate(ab, static_cast<unsigned int>(n) & (32 - 1)); }

		inline const unit_utf8 * point_prev_avx2(const unit_utf8 * s, int n)
		{
			ful_assume(0 < n);

			alignas(32) static const signed char m65[] = {
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
			};

			--s;
			const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(s) & -32);
			const unsigned int offset = reinterpret_cast<uptr>(s) & (32 - 1);

			__m256i cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
			unsigned int mask = zmsb(_mm256_movemask_epi8(cmpi), offset);
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
			// todo mask = zfsb(mask, -n); // zero forward set bits
			const unsigned int keep_mask = zmsb(static_cast<unsigned int>(-1), -n);
			mask = pdep(keep_mask, mask);

			const int i = mssb(mask);
			return word + i;
		}

		inline bool equal_cstr_avx2(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_assume(beg1 != end1);

			const unit_utf8 * beg2_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg2) & -32);

			const int beg2_offset = reinterpret_cast<uptr>(beg2) & (32 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const int beg1_offset = reinterpret_cast<uptr>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const auto word2 = rotate(*reinterpret_cast<const __m256i *>(beg2_word), beg2_offset);

				const auto remaining = end1 - beg1;
				const int word_end = 32 - beg2_offset;
				const int remaining_up_to_word_end = remaining < word_end ? static_cast<int>(remaining) : word_end;
				const int end_bits = static_cast<unsigned int>(-1) << remaining_up_to_word_end;

				const auto cmpi = _mm256_cmpeq_epi8(word1, word2);
				const int mask = _mm256_movemask_epi8(cmpi) | end_bits; // todo smsb(_mm256_movemask_epi8(cmpi), ); // set most significant bits
				if (mask != -1)
					return false;

				beg1 += remaining_up_to_word_end;
				beg2_word += 32;
			}

			const unit_utf8 * const end1_full_word = end1 - 32;
			while (beg1 <= end1_full_word)
			{
				const auto word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				const auto word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const auto cmpi = _mm256_cmpeq_epi8(word1, word2);
				const int mask = _mm256_movemask_epi8(cmpi);
				if (mask != -1)
					return false;

				beg1 += 32;
				beg2_word += 32;
			}

			if (end1 - beg1 != 0)
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg1) & -32);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end1 - 1) & -32);

				__m256i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
				}
				else
				{
					const int beg1_offset = reinterpret_cast<uptr>(beg1) & (32 - 1);
					word1 = rotate(*reinterpret_cast<const __m256i *>(beg1_word), beg1_offset);
				}
				const auto word2 = *reinterpret_cast<const __m256i *>(beg2_word);

				const int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 32
				const int end_bits = static_cast<unsigned int>(-1) << align_end;

				const auto cmpi = _mm256_cmpeq_epi8(word1, word2);
				const int mask = _mm256_movemask_epi8(cmpi) | end_bits;
				if (mask != -1)
					return false;
			}

			return beg2_word[end1 - beg1] == '\0';
		}

		inline const unit_utf8 * find_unit_avx2(const unit_utf8 * beg, const unit_utf8 * end, unit_utf8 c)
		{
			ful_assume(beg != end);

			const unit_utf8 * beg_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg) & -32);
			const unit_utf8 * const end_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end - 1) & -32);

			const unsigned int beg_offset = reinterpret_cast<uptr>(beg) & (32 - 1);
			const unsigned int beg_mask = static_cast<unsigned int>(-1) << beg_offset;

			const __m256i cveci = _mm256_set1_epi8(c);

			__m256i cmpi = _mm256_cmpeq_epi8(cveci, *reinterpret_cast<const __m256i *>(beg_word));
			unsigned int mask = _mm256_movemask_epi8(cmpi) & beg_mask; // todo zlsb(_mm256_movemask_epi8(cmpi), beg_offset); // zero less significant bits

			while (beg_word != end_word)
			{
				if (mask != 0)
					goto found;

				beg_word += 32;

				cmpi = _mm256_cmpeq_epi8(cveci, *reinterpret_cast<const __m256i *>(beg_word));
				mask = _mm256_movemask_epi8(cmpi);
			}

			const unsigned int end_offset = reinterpret_cast<uptr>(end - 1) & (32 - 1);
			const unsigned int end_mask = static_cast<unsigned int>(-1) << end_offset << 1; // note

			mask |= end_mask;

		found:
			const unsigned int i = ntz(mask);
			return beg_word + i;
		}
	}
}

#endif
