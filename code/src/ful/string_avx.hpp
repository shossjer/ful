#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("avx") ful_inline
		void copy_avx_32_64(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), b);
		}

		ful_target("avx") ful_inline
		void copy_avx_64_96(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + 32));
			const __m256i c = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 32), b);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), c);
		}

		ful_target("avx") ful_inline
		void copy_avx_96_128(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + 32));
			const __m256i c = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + 64));
			const __m256i d = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 32), b);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 64), c);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), d);
		}

		ful_target("avx") ful_inline
		char8 * memcopy_avx(const char8 * first, const char8 * last, char8 * begin)
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
				copy_avx_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern char8 * memcopy_avx_64(const char8 * first, usize size, char8 * begin);

				return memcopy_avx_64(first, size, begin);
			}
		}

		ful_target("avx") ful_inline
		char8 * memmovef_avx(const char8 * first, const char8 * last, char8 * begin)
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
				copy_avx_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 96u)
			{
				copy_avx_64_96(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 128u)
			{
				copy_avx_96_128(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern char8 * memmovef_avx_64(const char8 * first, usize size, char8 * begin);

				return memmovef_avx_64(first, size, begin);
			}
		}

		ful_target("avx") ful_inline
		char8 * memmover_avx(const char8 * first, const char8 * last, char8 * end)
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
				copy_avx_32_64(first, last, end - size, end);

				return end - size;
			}
			else
			{
				extern char8 * memmover_avx_64(usize size, const char8 * last, char8 * end);

				return memmover_avx_64(size, last, end);
			}
		}

		ful_target("avx") inline
		void set8_avx_32_64(char8 * from, char8 * to, char8 u)
		{
			const __m256i u256 = _mm256_set1_epi8(u);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(to - 32), u256);
		}

		ful_target("avx") inline
		void memset8_avx(char8 * from, char8 * to, char8 u)
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
				set8_avx_32_64(from, to, u);
			}
			else
			{
				extern void memset8_avx_64(char8 * from, char8 * to, char8 u);

				memset8_avx_64(from, to, u);
			}
		}

		ful_target("avx") inline
		void set16_avx_32_64(char16 * from, char16 * to, char16 u)
		{
			const __m256i u256 = _mm256_set1_epi16(u);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(to - 16), u256);
		}

		ful_target("avx") inline
		void memset16_avx(char16 * from, char16 * to, char16 u)
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
				set16_avx_32_64(from, to, u);
			}
			else
			{
				extern void memset16_avx_64(char16 * from, char16 * to, char16 u);

				memset16_avx_64(from, to, u);
			}
		}

		ful_target("avx") inline
		void set24_avx_32_64(char24 * from, char24 * to, char24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * (uint32)u;
			const uint64 bytes1 = (bytes0 << 8) | ((uint32)u >> 16);
			const uint64 bytes2 = (bytes0 << 16) | ((uint32)u >> 8);
			// lo 0001000001000001 0000010000010000 0100000100000100 0001000001000001
			// hi 0000010000010000 0100000100000100 0001000001000001 0000010000010000
			const __m256i lo_u256 = _mm256_set_epi64x(bytes0, bytes2, bytes1, bytes0);
			const __m256i hi_u256 = _mm256_set_epi64x(bytes2, bytes1, bytes0, bytes2);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), lo_u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(to) - 32), hi_u256);
		}

		ful_target("avx") inline
		void set24_avx_64_96(char24 * from, char24 * to, char24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * (uint32)u;
			const uint64 bytes1 = (bytes0 << 8) | ((uint32)u >> 16);
			const uint64 bytes2 = (bytes0 << 16) | ((uint32)u >> 8);
			// lo 0001000001000001 0000010000010000 0100000100000100 0001000001000001
			// mi 0100000100000100 0001000001000001 0000010000010000 0100000100000100
			// hi 0000010000010000 0100000100000100 0001000001000001 0000010000010000
			const __m256i lo_u256 = _mm256_set_epi64x(bytes0, bytes2, bytes1, bytes0);
			const __m256i mi_u256 = _mm256_set_epi64x(bytes1, bytes0, bytes2, bytes1);
			const __m256i hi_u256 = _mm256_set_epi64x(bytes2, bytes1, bytes0, bytes2);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), lo_u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(from) + 32), mi_u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(to) - 32), hi_u256);
		}

		ful_target("avx") inline
		void memset24_avx(char24 * from, char24 * to, char24 u)
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
				set24_avx_32_64(from, to, u);
			}
			else if (size <= 96u)
			{
				set24_avx_64_96(from, to, u);
			}
			else
			{
				extern void memset24_avx_96(char24 * from, char24 * to, char24 u);

				memset24_avx_96(from, to, u);
			}
		}

		ful_target("avx") inline
		void set32_avx_32_64(char32 * from, char32 * to, char32 u)
		{
			const __m256i u256 = _mm256_set1_epi32(u);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(to - 8), u256);
		}

		ful_target("avx") inline
		void memset32_avx(char32 * from, char32 * to, char32 u)
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
				set32_avx_32_64(from, to, u);
			}
			else
			{
				extern void memset32_avx_64(char32 * from, char32 * to, char32 u);

				memset32_avx_64(from, to, u);
			}
		}

		ful_target("avx") ful_inline
		void swap_avx_32_64(char8 * beg1, char8 * end1, char8 * beg2, char8 * end2)
		{
			const __m256i a1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
			const __m256i b1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end1 - 32));
			const __m256i a2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2));
			const __m256i b2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end2 - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2), a1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end2 - 32), b1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1), a2);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end1 - 32), b2);
		}

		ful_target("avx") ful_inline
		char8 * memswap_avx(char8 * beg1, char8 * end1, char8 * beg2)
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

			if (size <= 32u)
			{
				swap_sse2_16_32(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else if (size <= 64u)
			{
				swap_avx_32_64(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else
			{
				extern char8 * memswap_avx_64(char8 * beg1, usize size, char8 * beg2);

				return memswap_avx_64(beg1, size, beg2);
			}
		}
	}
}
