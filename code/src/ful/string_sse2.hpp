#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("sse2") ful_inline
		void copy_sse2_16_32(const byte * first, const byte * last, byte * begin, byte * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), b);
		}

		ful_target("sse2") ful_inline
		void copy_sse2_32_64(const byte * first, const byte * last, byte * begin, byte * end)
		{
			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + 16));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 32));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 16), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 32), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - 16), d);
		}

		ful_target("sse2") ful_inline
		byte * memcopy_sse2(const byte * first, const byte * last, byte * begin)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32u)
			{
				copy_sse2_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 64u)
			{
				copy_sse2_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern byte * memcopy_sse2_64(const byte * first, usize size, byte * begin);

				return memcopy_sse2_64(first, size, begin);
			}
		}

		ful_target("sse2") ful_inline
		byte * memmovef_sse2(const byte * first, const byte * last, byte * begin)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32u)
			{
				copy_sse2_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 64u)
			{
				copy_sse2_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern byte * memmovef_sse2_64(const byte * first, usize size, byte * begin);

				return memmovef_sse2_64(first, size, begin);
			}
		}

		ful_target("sse2") ful_inline
		byte * memmover_sse2(const byte * first, const byte * last, byte * end)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return end;

			if (size <= 32u)
			{
				copy_sse2_16_32(first, last, end - size, end);

				return end - size;
			}
			else if (size <= 64u)
			{
				copy_sse2_32_64(first, last, end - size, end);

				return end - size;
			}
			else
			{
				extern byte * memmover_sse2_64(usize size, const byte * last, byte * end);

				return memmover_sse2_64(size, last, end);
			}
		}

		ful_target("sse2") inline
		void set8_sse2_16_32(char8 * from, char8 * to, char8 u)
		{
			const __m128i u128 = _mm_set1_epi8(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 16), u128);
		}

		ful_target("sse2") inline
		void set8_sse2_32_64(char8 * from, char8 * to, char8 u)
		{
			const __m128i u128 = _mm_set1_epi8(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(from + 16), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 32), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 16), u128);
		}

		ful_target("sse2") ful_inline
		void memset8_sse2(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set8_sse2_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set8_sse2_32_64(from, to, u);
			}
			else
			{
				extern void memset8_sse2_64(char8 * from, char8 * to, char8 u);

				memset8_sse2_64(from, to, u);
			}
		}

		ful_target("sse2") inline
		void set16_sse2_16_32(char16 * from, char16 * to, char16 u)
		{
			const __m128i u128 = _mm_set1_epi16(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 8), u128);
		}

		ful_target("sse2") inline
		void set16_sse2_32_64(char16 * from, char16 * to, char16 u)
		{
			const __m128i u128 = _mm_set1_epi16(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(from + 8), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 16), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 8), u128);
		}

		ful_target("sse2") ful_inline
		void memset16_sse2(char16 * from, char16 * to, char16 u)
		{
			const usize size = (to - from) * sizeof(char16);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set16_sse2_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set16_sse2_32_64(from, to, u);
			}
			else
			{
				extern void memset16_sse2_64(char16 * from, char16 * to, char16 u);

				memset16_sse2_64(from, to, u);
			}
		}

		ful_target("sse2") inline
		void set24_sse2_16_32(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			// lo 0100000100000100 0001000001000001
			// hi 0000010000010000 0100000100000100
			const __m128i lo_u128 = _mm_set_epi64x(bytes1, bytes0);
			const __m128i hi_u128 = _mm_set_epi64x(bytes2, bytes1);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse2") inline
		void set24_sse2_32_64(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			// lo 0100000100000100 0001000001000001
			// mi 0001000001000001 0000010000010000
			// hi 0000010000010000 0100000100000100
			const __m128i lo_u128 = _mm_set_epi64x(bytes1, bytes0);
			const __m128i mi_u128 = _mm_set_epi64x(bytes0, bytes2);
			const __m128i hi_u128 = _mm_set_epi64x(bytes2, bytes1);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 16), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse2") inline
		void set24_sse2_64_96(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			// lo 0100000100000100 0001000001000001
			// mi 0001000001000001 0000010000010000
			// hi 0000010000010000 0100000100000100
			const __m128i lo_u128 = _mm_set_epi64x(bytes1, bytes0);
			const __m128i mi_u128 = _mm_set_epi64x(bytes0, bytes2);
			const __m128i hi_u128 = _mm_set_epi64x(bytes2, bytes1);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 16), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 32), hi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 48), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 64), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(from) + 80), hi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse2") ful_inline
		void memset24_sse2(char24 * from, char24 * to, char_fast24 u)
		{
			const usize size = (to - from) * sizeof(char24);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set24_sse2_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set24_sse2_32_64(from, to, u);
			}
			else if (size <= 96u)
			{
				set24_sse2_64_96(from, to, u);
			}
			else
			{
				extern void memset24_sse2_96(char24 * from, char24 * to, char_fast24 u);

				memset24_sse2_96(from, to, u);
			}
		}

		ful_target("sse2") inline
		void set32_sse2_16_32(char32 * from, char32 * to, char32 u)
		{
			const __m128i u128 = _mm_set1_epi32(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 4), u128);
		}

		ful_target("sse2") inline
		void set32_sse2_32_64(char32 * from, char32 * to, char32 u)
		{
			const __m128i u128 = _mm_set1_epi32(u);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(from + 4), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 8), u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 4), u128);
		}

		ful_target("sse2") ful_inline
		void memset32_sse2(char32 * from, char32 * to, char32 u)
		{
			const usize size = (to - from) * sizeof(char32);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set32_sse2_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set32_sse2_32_64(from, to, u);
			}
			else
			{
				extern void memset32_sse2_64(char32 * from, char32 * to, char32 u);

				memset32_sse2_64(from, to, u);
			}
		}

		ful_target("sse2") ful_inline
		void swap_sse2_16_32(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			const __m128i a1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
			const __m128i b1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
			const __m128i a2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2));
			const __m128i b2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2), a1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end2 - 16), b1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1), a2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end1 - 16), b2);
		}

		ful_target("sse2") ful_inline
		void swap_sse2_32_64(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			const __m128i a1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1));
			const __m128i b1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + 16));
			const __m128i c1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 32));
			const __m128i d1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
			const __m128i a2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2));
			const __m128i b2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + 16));
			const __m128i c2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 32));
			const __m128i d2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2), a1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + 16), b1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end2 - 32), c1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end2 - 16), d1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1), a2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + 16), b2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end1 - 32), c2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end1 - 16), d2);
		}

		ful_target("sse2") ful_inline
		byte * memswap_sse2(byte * beg1, byte * end1, byte * beg2)
		{
			const usize size = end1 - beg1;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return beg2;

			if (size <= 32)
			{
				swap_sse2_16_32(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else if (size <= 64)
			{
				swap_sse2_32_64(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else
			{
				extern byte * memswap_sse2_64(byte * beg1, usize size, byte * beg2);

				return memswap_sse2_64(beg1, size, beg2);
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
	}
}
