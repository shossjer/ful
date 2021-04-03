#pragma once

#if defined(__SSE2__)

// #include "ful/intrinsics.hpp"
// #include "ful/stdint.hpp"
// #include "ful/unicode.hpp"

// #include <immintrin.h>

namespace ful
{
	namespace detail
	{
		inline __m128i rotate(__m128i ab, unsigned int n)
		{
			ful_assume(n < 16);

			static const long long tabler[] = {
				 0,  8, 16, 24, 32, 40, 48, 56,
				 0,  8, 16, 24, 32, 40, 48, 56,  0,
			};
			static const long long tablel[] = {
				64, 56, 48, 40, 32, 24, 16,  8,
				64, 56, 48, 40, 32, 24, 16,  8,  0,
			};
			const auto countr = _mm_loadu_si128(reinterpret_cast<const __m128i *>(tabler + n));
			const auto countl = _mm_loadu_si128(reinterpret_cast<const __m128i *>(tablel + n));

			const auto ba = _mm_shuffle_epi32(ab, 0b01001110);

			// n = [0, 8) => >>
			if (n < 8)
			{
				const auto abr = _mm_srl_epi64(ab, countr);
				const auto bal = _mm_sll_epi64(ba, countl);
				return _mm_or_si128(abr, bal);
			}
			// n = [8, 16) => <<
			else
			{
				const auto bar = _mm_srl_epi64(ba, countr);
				const auto abl = _mm_sll_epi64(ab, countl);
				return _mm_or_si128(bar, abl);
			}
		}

		inline __m128i rotate(__m128i ab, int n) { return rotate(ab, static_cast<unsigned int>(n) & (16 - 1)); }

		inline const unit_utf8 * point_prev_sse2(const unit_utf8 * s, int n)
		{
			ful_assume(0 < n);

			alignas(16) static const signed char m65[] = {
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
			};

			--s;
			const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(s) & -16);
			const unsigned int offset = reinterpret_cast<uptr>(s) & (16 - 1);

			__m128i cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
			unsigned int mask = zmsb(_mm_movemask_epi8(cmpi), offset);
			while (true)
			{
				const unsigned int npoints = popcnt(mask);

				n -= npoints;
				if (n <= 0)
					break;

				word -= 16;

				cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
				mask = _mm_movemask_epi8(cmpi);
			}
			// todo mask = zfsb(mask, -n); // zero forward set bits
			const unsigned int keep_mask = zmsb(static_cast<unsigned int>(-1), -n);
			mask = pdep(keep_mask, mask);

			const unsigned int i = mssb(mask);
			return word + i;
		}

		inline bool equal_cstr_sse2(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			ful_assume(beg1 != end1);

			const unit_utf8 * beg2_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg2) & -16);

			const unsigned int beg2_offset = reinterpret_cast<uptr>(beg2) & (16 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg1) & -16);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end1 - 1) & -16);

				__m128i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<uptr>(beg1) & (16 - 1);
					word1 = rotate(*reinterpret_cast<const __m128i *>(beg1_word), beg1_offset);
				}
				const __m128i word2 = rotate(*reinterpret_cast<const __m128i *>(beg2_word), beg2_offset);

				const auto remaining = end1 - beg1;
				const unsigned int word_end = 16 - beg2_offset;
				const unsigned int remaining_up_to_word_end = remaining < word_end ? static_cast<int>(remaining) : word_end;
				const unsigned int end_bits = static_cast<unsigned int>(-1) << remaining_up_to_word_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits; // todo smsb(_mm256_movemask_epi8(cmpi), ); // set most significant bits
				if (mask != -1)
					return false;

				beg1 += remaining_up_to_word_end;
				beg2_word += 16;
			}

			const unit_utf8 * const end1_full_word = end1 - 16;
			while (beg1 <= end1_full_word)
			{
				const __m128i word1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
				const __m128i word2 = *reinterpret_cast<const __m128i *>(beg2_word);

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi);
				if (mask != 0xffff)
					return false;

				beg1 += 16;
				beg2_word += 16;
			}

			if (end1 - beg1 != 0)
			{
				const unit_utf8 * const beg1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg1) & -16);
				const unit_utf8 * const end1_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end1 - 1) & -16);

				__m128i word1;
				if (beg1_word != end1_word)
				{
					word1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
				}
				else
				{
					const unsigned int beg1_offset = reinterpret_cast<uptr>(beg1) & (16 - 1);
					word1 = rotate(*reinterpret_cast<const __m128i *>(beg1_word), beg1_offset);
				}
				const __m128i word2 = *reinterpret_cast<const __m128i *>(beg2_word);

				const unsigned int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 16
				const unsigned int end_bits = static_cast<unsigned int>(-1) << align_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits;
				if (mask != -1)
					return false;
			}

			return beg2_word[end1 - beg1] == '\0';
		}

		inline const unit_utf8 * find_unit_sse2(const unit_utf8 * beg, const unit_utf8 * end, unit_utf8 c)
		{
			ful_assume(beg != end);

			const unit_utf8 * beg_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(beg) & -16);
			const unit_utf8 * const end_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<uptr>(end - 1) & -16);

			const unsigned int beg_offset = reinterpret_cast<uptr>(beg) & (16 - 1);
			const unsigned int beg_mask = static_cast<unsigned int>(-1) << beg_offset;

			const __m128i cveci = _mm_set1_epi8(c);

			__m128i cmpi = _mm_cmpeq_epi8(cveci, *reinterpret_cast<const __m128i *>(beg_word));
			unsigned int mask = _mm_movemask_epi8(cmpi) & beg_mask; // todo zlsb(_mm256_movemask_epi8(cmpi), beg_offset); // zero less significant bits

			while (beg_word != end_word)
			{
				if (mask != 0)
					goto found;

				beg_word += 16;

				cmpi = _mm_cmpeq_epi8(cveci, *reinterpret_cast<const __m128i *>(beg_word));
				mask = _mm_movemask_epi8(cmpi);
			}

			const unsigned int end_offset = reinterpret_cast<uptr>(end - 1) & (16 - 1);
			const unsigned int end_mask = static_cast<unsigned int>(-1) << end_offset << 1; // note

			mask |= end_mask;

		found:
			const unsigned int i = ntz(mask);
			return beg_word + i;
		}
	}
}

#endif
