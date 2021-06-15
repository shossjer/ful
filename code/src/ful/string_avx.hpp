#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("avx") ful_inline
		char8 * memcopy_avx_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), b);

			return end;
		}

		ful_target("avx") ful_inline
		char8 * memcopy_avx(const char8 * first, const char8 * last, char8 * begin)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return begin;

			if (size <= 32)
			{
				return memcopy_sse2_16(first, last, begin, begin + size);
			}
			else if (size <= 64)
			{
				return memcopy_avx_32(first, last, begin, begin + size);
			}
			else
			{
				extern char8 * memcopy_avx_64(const char8 * first, usize size, char8 * begin);

				return memcopy_avx_64(first, size, begin);
			}
		}

		ful_target("avx") ful_inline
		char8 * memypoc_avx_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), b);

			return begin;
		}

		ful_target("avx") ful_inline
		char8 * memypoc_avx(const char8 * first, const char8 * last, char8 * end)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return end;

			if (size <= 32)
			{
				return memypoc_sse2_16(first, last, end - size, end);
			}
			else if (size <= 64)
			{
				return memypoc_avx_32(first, last, end - size, end);
			}
			else
			{
				extern char8 * memypoc_avx_64(usize size, const char8 * last, char8 * end);

				return memypoc_avx_64(size, last, end);
			}
		}

		ful_target("avx") ful_inline
		char8 * memswap_avx_32(char8 * beg1, char8 * end1, char8 * beg2, char8 * end2)
		{
			const __m256i a1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1));
			const __m256i b1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end1 - 32));
			const __m256i a2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2));
			const __m256i b2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end2 - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2), a1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end2 - 32), b1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1), a2);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end1 - 32), b2);

			return end2;
		}

		ful_target("avx") ful_inline
		char8 * memswap_avx(char8 * beg1, char8 * end1, char8 * beg2)
		{
			const usize size = end1 - beg1;
			if (!ful_expect(16u < size))
				return beg2;

			if (size <= 32)
			{
				return memswap_sse2_16(beg1, end1, beg2, beg2 + size);
			}
			else if (size <= 64)
			{
				return memswap_avx_32(beg1, end1, beg2, beg2 + size);
			}
			else
			{
				extern char8 * memswap_avx_64(char8 * beg1, usize size, char8 * beg2);

				return memswap_avx_64(beg1, size, beg2);
			}
		}

		ful_target("avx") inline
		void memset8_avx(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
			if (!ful_expect(16u < size))
				return;

			if (size <= 32)
			{
				const __m128i u128 = _mm_set1_epi8(u);

				_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(to - 16), u128);
			}
			else if (size <= 64)
			{
				const __m256i u256 = _mm256_set1_epi8(u);

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(to - 32), u256);
			}
			else
			{
				extern void memset8_avx_64(char8 * from, char8 * to, char8 u);

				memset8_avx_64(from, to, u);
			}
		}
	}
}
