#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("sse2") ful_inline
		char8 * copy_8_sse2_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), b);

			return end;
		}

		ful_target("sse2") ful_inline
		char8 * copy_8_sse2_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + 16));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 32));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 16), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 32), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), d);

			return end;
		}

		ful_target("sse2") ful_inline
		char8 * copy_8_sse2(const char8 * first, const char8 * last, char8 * begin)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return begin;

			if (size <= 32)
			{
				return copy_8_sse2_16(first, last, begin, begin + size);
			}
			else if (size <= 64)
			{
				return copy_8_sse2_32(first, last, begin, begin + size);
			}
			else
			{
				extern char8 * copy_8_sse2_64(const char8 * first, usize size, char8 * begin);

				return copy_8_sse2_64(first, size, begin);
			}
		}

		ful_target("sse2") ful_inline
		char8 * rcopy_8_sse2_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), b);

			return begin;
		}

		ful_target("sse2") ful_inline
		char8 * rcopy_8_sse2_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + 16));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 32));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 16), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 32), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), d);

			return begin;
		}

		ful_target("sse2") ful_inline
		char8 * rcopy_8_sse2(const char8 * first, const char8 * last, char8 * end)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return end;

			if (size <= 32)
			{
				return rcopy_8_sse2_16(first, last, end - size, end);
			}
			else if (size <= 64)
			{
				return rcopy_8_sse2_32(first, last, end - size, end);
			}
			else
			{
				extern char8 * rcopy_8_sse2_64(usize size, const char8 * last, char8 * end);

				return rcopy_8_sse2_64(size, last, end);
			}
		}

		ful_target("sse2") inline
		__m128i rotate(__m128i ab, unsigned int n)
		{
			ful_expect(n < 16);

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

		ful_target("sse2") ful_inline __m128i rotate(__m128i ab, int n) { return rotate(ab, static_cast<unsigned int>(n) & (16 - 1)); }

		ful_target("sse2") inline
		__m128i loadu_first_sse2(const char8 * begin, const char8 * end)
		{
			if (!ful_expect(begin != end))
				return _mm_setzero_si128();

			const __m128i * const begin_word = reinterpret_cast<const __m128i *>(reinterpret_cast<puint>(begin) & -16);
			const __m128i * const end_word = reinterpret_cast<const __m128i *>(reinterpret_cast<puint>(end - 1) & -16);

			__m128i word;
			if (begin_word != end_word)
			{
				word = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
			}
			else
			{
				const unsigned int first_offset = reinterpret_cast<puint>(begin) & (16 - 1);
				word = rotate(*begin_word, first_offset);
			}
			return word;
		}

		ful_target("sse2") inline
		const unit_utf8 * point_prev_sse2(const unit_utf8 * s, ssize n)
		{
			ful_expect(0 < n);

			alignas(16) static const signed char m65[] = {
				-65, -65, -65, -65, -65, -65, -65, -65,
				-65, -65, -65, -65, -65, -65, -65, -65,
			};

			--s;
			const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -16);
			const unsigned int offset = reinterpret_cast<puint>(s) & (16 - 1);

			__m128i cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
			unsigned int mask = zero_higher_bits(_mm_movemask_epi8(cmpi), offset);
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

			const unsigned int i = index_set_bit(mask, static_cast<unsigned int>(-n)); // -n < 32
			return word + i;
		}

		ful_target("sse2") inline
		bool equal_cstr_sse2(const char8 * beg1, const char8 * end1, const char8 * beg2)
		{
			ful_expect(beg1 != end1);

			const char8 * beg2_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(beg2) & -16);

			const unsigned int beg2_offset = reinterpret_cast<puint>(beg2) & (16 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const __m128i word1 = loadu_first_sse2(beg1, end1);
				const __m128i word2 = rotate(*reinterpret_cast<const __m128i *>(beg2_word), beg2_offset);

				const auto remaining = end1 - beg1;
				const unsigned int word_end = 16 - beg2_offset;
				const unsigned int remaining_up_to_word_end = remaining < word_end ? static_cast<int>(remaining) : word_end;
				const unsigned int end_bits = static_cast<unsigned int>(-1) << remaining_up_to_word_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits; // todo smsb(_mm256_movemask_epi8(cmpi), ); // set most significant bits
				if (mask != static_cast<unsigned int>(-1))
					return false;

				if (remaining < word_end)
					return beg2[end1 - beg1] == '\0';

				beg1 += remaining_up_to_word_end;
				beg2_word += 16;
			}

			const char8 * const end1_full_word = end1 - 16;
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
				const __m128i word1 = loadu_first_sse2(beg1, end1);
				const __m128i word2 = *reinterpret_cast<const __m128i *>(beg2_word);

				const unsigned int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 16
				const unsigned int end_bits = static_cast<unsigned int>(-1) << align_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits;
				if (mask != static_cast<unsigned int>(-1))
					return false;
			}

			return beg2_word[end1 - beg1] == '\0';
		}

		ful_target("sse2") inline
		const char8 * find_unit_sse2(const char8 * beg, const char8 * end, char8 c)
		{
			ful_expect(beg != end);

			const char8 * beg_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(beg) & -16);
			const char8 * const end_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

			const unsigned int beg_offset = reinterpret_cast<puint>(beg) & (16 - 1);
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

			{
				const unsigned int end_offset = reinterpret_cast<puint>(end - 1) & (16 - 1);
				mask = set_higher_bits(mask, end_offset);
			}

		found:
			const unsigned int i = count_trailing_zeros(mask);
			return beg_word + i;
		}

		ful_target("sse2") inline
		bool less_cstr_sse2(const char8 * beg1, const char8 * end1, const char8 * beg2)
		{
			ful_expect(beg1 != end1);

			const char8 * beg2_word = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(beg2) & -16);

			const unsigned int beg2_offset = reinterpret_cast<puint>(beg2) & (16 - 1);

			if (beg2_offset != 0) // unaligned
			{
				const __m128i word1 = loadu_first_sse2(beg1, end1);
				const __m128i word2 = rotate(*reinterpret_cast<const __m128i *>(beg2_word), beg2_offset);

				const auto remaining = end1 - beg1;
				const unsigned int word_end = 16 - beg2_offset;
				const unsigned int remaining_up_to_word_end = remaining < word_end ? static_cast<int>(remaining) : word_end;
				const unsigned int end_bits = static_cast<unsigned int>(-1) << remaining_up_to_word_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits; // todo smsb(_mm256_movemask_epi8(cmpi), ); // set most significant bits
				if (mask != static_cast<unsigned int>(-1))
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2[i];
				}

				beg1 += remaining_up_to_word_end;
				beg2_word += 16;
			}

			const char8 * const end1_full_word = end1 - 16;
			while (beg1 <= end1_full_word)
			{
				const __m128i word1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
				const __m128i word2 = *reinterpret_cast<const __m128i *>(beg2_word);

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi);
				if (mask != 0xffff)
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2_word[i];
				}

				beg1 += 16;
				beg2_word += 16;
			}

			if (end1 - beg1 != 0)
			{
				const __m128i word1 = loadu_first_sse2(beg1, end1);
				const __m128i word2 = *reinterpret_cast<const __m128i *>(beg2_word);

				const unsigned int align_end = static_cast<int>(end1 - beg1); // guaranteed to be < 16
				const unsigned int end_bits = static_cast<unsigned int>(-1) << align_end;

				const __m128i cmpi = _mm_cmpeq_epi8(word1, word2);
				const unsigned int mask = _mm_movemask_epi8(cmpi) | end_bits;
				if (mask != static_cast<unsigned int>(-1))
				{
					const unsigned int i = least_significant_set_bit(~mask);
					return beg1[i] < beg2_word[i];
				}
			}

			return beg2_word[end1 - beg1] != '\0';
		}
	}
}
