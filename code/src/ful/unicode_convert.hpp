#pragma once

#include "ful/unicode.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern unit_utf16le * ful_dispatch(convert_8_16le)(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_8_16be)(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_8_32le)(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_8_32be)(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_16le_8)(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16be * ful_dispatch(convert_16le_16be)(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_16le_32le)(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_16le_32be)(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_16be_8)(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_16be_16le)(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf32le * ful_dispatch(convert_16be_32le)(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end);
		extern unit_utf32be * ful_dispatch(convert_16be_32be)(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_32le_8)(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_32le_16le)(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_32le_16be)(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32be * ful_dispatch(convert_32le_32be)(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end);
		extern unit_utf8 * ful_dispatch(convert_32be_8)(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end);
		extern unit_utf16le * ful_dispatch(convert_32be_16le)(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end);
		extern unit_utf16be * ful_dispatch(convert_32be_16be)(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end);
		extern unit_utf32le * ful_dispatch(convert_32be_32le)(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end);
#endif

		ful_generic() inline
		unit_utf16le * convert_8_16le_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(end);

			while (first != last)
			{
				const unsigned int size = point_size(first);
				switch (size)
				{
				case 1:
					*begin = static_cast<unit_utf16le>(static_cast<unsigned int>(first[0]));
					first += 1;
					++begin;
					break;
				case 2:
					*begin = static_cast<unit_utf16le>((static_cast<unsigned int>(first[0] & 0x1f) << 6) | static_cast<unsigned int>(first[1] & 0x3f));
					first += 2;
					++begin;
					break;
				case 3:
					*begin = static_cast<unit_utf16le>((static_cast<unsigned int>(first[0] & 0x0f) << 12) | (static_cast<unsigned int>(first[1] & 0x3f) << 6) | static_cast<unsigned int>(first[2] & 0x3f));
					first += 3;
					++begin;
					break;
				case 4:
				{
					const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					const uint32 valuf = value - 0x10000;

					// 0x03ff = 0000 0011 1111 1111
					// 0xd800 = 1101 1000 0000 0000
					// 0xdc00 = 1101 1100 0000 0000
					begin[0] = static_cast<unit_utf16le>((value >> 10) | 0xd800);
					begin[1] = static_cast<unit_utf16le>((valuf & 0x03ff) | 0xdc00);
					first += 4;
					begin += 2;
					break;
				}
				default:
					ful_unreachable();
				}
			}
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_8_16be_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_8_32le_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(end);

			while (first != last)
			{
				if (!ful_expect(begin != end))
					break;

				const unsigned int size = point_size(first);
				switch (size)
				{
				case 1:
					*begin = static_cast<unit_utf32le>(static_cast<unsigned int>(first[0]));
					first += 1;
					++begin;
					break;
				case 2:
					*begin = static_cast<unit_utf32le>((static_cast<unsigned int>(first[0] & 0x1f) << 6) | static_cast<unsigned int>(first[1] & 0x3f));
					first += 2;
					++begin;
					break;
				case 3:
					*begin = static_cast<unit_utf32le>((static_cast<unsigned int>(first[0] & 0x0f) << 12) | (static_cast<unsigned int>(first[1] & 0x3f) << 6) | static_cast<unsigned int>(first[2] & 0x3f));
					first += 3;
					++begin;
					break;
				case 4:
					*begin = static_cast<unit_utf32le>((static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f));
					first += 4;
					++begin;
					break;
				default:
					ful_unreachable();
				}
			}
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_8_32be_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_16le_8_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_16le_16be_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_16le_32le_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_16le_32be_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_16be_8_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_16be_16le_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_16be_32le_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_16be_32be_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_32le_8_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_32le_16le_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_32le_16be_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_32le_32be_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_32be_8_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_32be_16le_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_32be_16be_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_32be_32le_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_target("avx2") inline
		unit_utf16le * convert_8_16le_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
		{
			const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
			const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

			const __m256i p32 = _mm256_set1_epi8(32);

			const unit_utf8 * last_word = last - 32;
			while (first <= last_word)
			{
				const int head = first[0];
				if (head > -1)
				{
					// word : aaaaaaaa aaaaaaaa | aaaaaaaa aaaaaaaa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

					const unsigned int mask = _mm256_movemask_epi8(word);
					const unsigned int count = count_trailing_zero_bits(mask);

					if (ful_expect(begin <= end - 32))
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
						const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelo, valuehi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuelo, valuehi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 16), value16);
					}
					first += count;
					begin += count;
				}
				else if (head > -17)
				{
					// todo make something more interesting when there is test data for it
					const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					const uint32 valuf = value - 0x10000;

					// 0x03ff = 0000 0011 1111 1111
					// 0xd800 = 1101 1000 0000 0000
					// 0xdc00 = 1101 1100 0000 0000
					begin[0] = static_cast<unit_utf16le>((value >> 10) | 0xd800);
					begin[1] = static_cast<unit_utf16le>((valuf & 0x03ff) | 0xdc00);
					first += 4;
					begin += 2;
				}
				else if (head > -33)
				{
					// todo make something more interesting
					*begin = static_cast<unit_utf16le>((static_cast<uint32>(first[0] & 0x0f) << 12) | (static_cast<uint32>(first[1] & 0x3f) << 6) | static_cast<uint32>(first[2] & 0x3f));
					first += 3;
					begin += 1;
				}
				else if (ful_expect(head > -65))
				{
					// word : babababa babababa | babababa babababa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					// s : each 16 : bbaaaaaa aa000000
					// t : each 16 : 00000000 bbbbbbbb
					const __m256i s = _mm256_slli_epi16(word, 6);
					const __m256i t = _mm256_srli_epi16(word, 8);
					// x : each 16 : 00000aaa aa000000
					// y : each 16 : 00000000 00bbbbbb
					const __m256i x = _mm256_and_si256(s, x_mask2);
					const __m256i y = _mm256_and_si256(t, y_mask2);
					const __m256i value = _mm256_or_si256(x, y);

					const __m256i add = _mm256_adds_epi8(word, p32);
					const unsigned int mask = _mm256_movemask_epi8(add);
					const unsigned int count = count_trailing_zero_bits(~mask) / 2;

					if (ful_expect(begin <= end - 16))
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
					}
					first += count * 2;
					begin += count;
				}
				else
				{
					// note unexpected
					first += 1;
				}
			}
			return convert_8_16le_none(first, last, begin, end);
		}

		ful_target("avx2") inline
		unit_utf32le * convert_8_32le_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
		{
			//const __m256i order3 = _mm256_set_epi64x(
			//	0x80090a0b80060708, 0x8003040580000102,
			//	0x80090a0b80060708, 0x8003040580000102);
			const __m256i order3 = _mm256_set_epi64x(
				0x8005060780020304, 0x800f0001800c0d0e,
				0x80090a0b80060708, 0x8003040580000102);

			const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
			const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

			const __m256i p32 = _mm256_set1_epi8(32);

			const __m256i x_mask3 = _mm256_set1_epi32(0x0000f000);
			const __m256i y_mask3 = _mm256_set1_epi32(0x00000fc0);
			const __m256i z_mask3 = _mm256_set1_epi32(0x0000003f);

			const __m256i cmp_mask3 = _mm256_set1_epi16(0x000e);

			const unit_utf8 * last_word = last - 32;
			while (first <= last_word)
			{
				const int head = first[0];
				if (head > -1)
				{
					// word : aaaaaaaa aaaaaaaa | aaaaaaaa aaaaaaaa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

					const unsigned int mask = _mm256_movemask_epi8(word);
					const unsigned int count = count_trailing_zero_bits(mask);

					if (ful_expect(begin <= end - 32))
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
						const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
						const __m256i valuelolo = _mm256_unpacklo_epi16(valuelo, zero);
						const __m256i valuelohi = _mm256_unpackhi_epi16(valuelo, zero);
						const __m256i valuehilo = _mm256_unpacklo_epi16(valuehi, zero);
						const __m256i valuehihi = _mm256_unpackhi_epi16(valuehi, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelolo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 20), reinterpret_cast<__m128i *>(begin + 4), valuelohi);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehilo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 28), reinterpret_cast<__m128i *>(begin + 12), valuehihi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelolo, valuelohi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuehilo, valuehihi, 0x20);
						const __m256i value32 = _mm256_permute2x128_si256(valuelolo, valuelohi, 0x31);
						const __m256i value48 = _mm256_permute2x128_si256(valuehilo, valuehihi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 8), value16);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 16), value32);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 24), value48);
					}
					first += count;
					begin += count;
				}
				else if (head > -17)
				{
					// todo make something more interesting when there is test data for it
					*begin = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					first += 4;
					begin += 1;
				}
				else if (head > -33)
				{
					// bytes: -------- nmlkjihg | fedcba98 76543210
					//        -n-m-l-k -j-i-h-g | -7-6-5-4 -3-2-1-0 unpacklo
					//        -------- -------- | -f-e-d-c -b-a-9-8 unpackhi
					//
					//        -------- mnjkghde | -------- ab784512
					//        -------- -l-i-f-c | -------- -9-6-3-0
					//
					// word : -lmn-ijk -fgh-cde | -9ab-678 -345-012

					// bytes: -------- cbacbacb | acbacbac bacbacba
					// word : 0abc0abc 0abc0abc | 0abc0abc 0abc0abc
					//const __m256i bytes = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(first + 12), reinterpret_cast<const __m128i *>(first + 0));
					//const __m256i word = _mm256_shuffle_epi8(bytes, order3);
					const __m256i bytes = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					const __m256i word = _mm256_shuffle_epi8(_mm256_permute4x64_epi64(bytes, 0b01100100), order3);
					// s : each 32 : 00000000 0000aaaa aaaabbbb bbbbcccc
					// t : each 32 : 00000000 00aaaaaa aabbbbbb bbcccccc
					const __m256i s = _mm256_srli_epi32(word, 4);
					const __m256i t = _mm256_srli_epi32(word, 2);
					// x : each 32 : 00000000 00000000 aaaa0000 00000000
					// y : each 32 : 00000000 00000000 0000bbbb bb000000
					// z : each 32 : 00000000 00000000 00000000 00cccccc
					const __m256i x = _mm256_and_si256(s, x_mask3);
					const __m256i y = _mm256_and_si256(t, y_mask3);
					const __m256i z = _mm256_and_si256(word, z_mask3);
					const __m256i value = _mm256_or_si256(_mm256_or_si256(x, y), z);

					const __m256i cmp = _mm256_cmpeq_epi16(s, cmp_mask3);
					const unsigned int mask = _mm256_movemask_epi8(cmp);
					const unsigned int count = count_trailing_zero_bits(mask ^ 0xcccccccc) / 4;

					if (ful_expect(begin <= end - 8))
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
					}
					first += count * 3;
					begin += count;
				}
				else if (ful_expect(head > -65))
				{
					// word : babababa babababa | babababa babababa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					// s : each 16 : bbaaaaaa aa000000
					// t : each 16 : 00000000 bbbbbbbb
					const __m256i s = _mm256_slli_epi16(word, 6);
					const __m256i t = _mm256_srli_epi16(word, 8);
					// x : each 16 : 00000aaa aa000000
					// y : each 16 : 00000000 00bbbbbb
					const __m256i x = _mm256_and_si256(s, x_mask2);
					const __m256i y = _mm256_and_si256(t, y_mask2);
					const __m256i value = _mm256_or_si256(x, y);

					const __m256i add = _mm256_adds_epi8(word, p32);
					const unsigned int mask = _mm256_movemask_epi8(add);
					const unsigned int count = count_trailing_zero_bits(~mask) / 2;

					if (ful_expect(begin <= end - 16))
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi16(value, zero);
						const __m256i valuehi = _mm256_unpackhi_epi16(value, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 8), reinterpret_cast<__m128i *>(begin), valuelo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 12), reinterpret_cast<__m128i *>(begin + 4), valuehi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelo, valuehi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuelo, valuehi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 8), value16);
					}
					first += count * 2;
					begin += count;
				}
				else
				{
					// note unexpected
					first += 1;
				}
			}
			return convert_8_32le_none(first, last, begin, end);
		}
	}

	ful_inline unit_utf16le * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_16le(first, last, begin, end);
#elif defined(__AVX2__)
		return detail::convert_8_16le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_16le_sse2(first, last, begin, end);
#else
		return detail::convert_8_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_16be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_8_16be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_16be_sse2(first, last, begin, end);
#else
		return detail::convert_8_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_32le(first, last, begin, end);
#elif defined(__AVX2__)
		return detail::convert_8_32le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_32le_sse2(first, last, begin, end);
#else
		return detail::convert_8_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_32be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_8_32be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_32be_sse2(first, last, begin, end);
#else
		return detail::convert_8_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_8(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16le_8_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16le_8_sse2(first, last, begin, end);
#else
		return detail::convert_16le_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_16be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16le_16be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16le_16be_sse2(first, last, begin, end);
#else
		return detail::convert_16le_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_32le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16le_32le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16le_32le_sse2(first, last, begin, end);
#else
		return detail::convert_16le_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16le_32be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16le_32be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16le_32be_sse2(first, last, begin, end);
#else
		return detail::convert_16le_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_8(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16be_8_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16be_8_sse2(first, last, begin, end);
#else
		return detail::convert_16be_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_16le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16be_16le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16be_16le_sse2(first, last, begin, end);
#else
		return detail::convert_16be_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_32le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16be_32le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16be_32le_sse2(first, last, begin, end);
#else
		return detail::convert_16be_32le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16be_32be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_16be_32be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16be_32be_sse2(first, last, begin, end);
#else
		return detail::convert_16be_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_8(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32le_8_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32le_8_sse2(first, last, begin, end);
#else
		return detail::convert_32le_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_16le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32le_16le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32le_16le_sse2(first, last, begin, end);
#else
		return detail::convert_32le_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_16be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32le_16be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32le_16be_sse2(first, last, begin, end);
#else
		return detail::convert_32le_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32be * convert(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32le_32be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32le_32be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32le_32be_sse2(first, last, begin, end);
#else
		return detail::convert_32le_32be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_8(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32be_8_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32be_8_sse2(first, last, begin, end);
#else
		return detail::convert_32be_8_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16le * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_16le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32be_16le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32be_16le_sse2(first, last, begin, end);
#else
		return detail::convert_32be_16le_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf16be * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_16be(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32be_16be_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32be_16be_sse2(first, last, begin, end);
#else
		return detail::convert_32be_16be_none(first, last, begin, end);
#endif
	}

	ful_inline unit_utf32le * convert(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32be_32le(first, last, begin, end);
//#elif defined(__AVX2__)
//		return detail::convert_32be_32le_avx2(first, last, begin, end);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32be_32le_sse2(first, last, begin, end);
#else
		return detail::convert_32be_32le_none(first, last, begin, end);
#endif
	}
}
