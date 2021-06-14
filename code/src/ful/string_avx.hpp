#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("avx") ful_inline
		char8 * copy_8_avx_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), b);

			return end;
		}

		ful_target("avx") ful_inline
		char8 * copy_8_avx(const char8 * first, const char8 * last, char8 * begin)
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
				return copy_8_avx_32(first, last, begin, begin + size);
			}
			else
			{
				extern char8 * copy_8_avx_64(const char8 * first, usize size, char8 * begin);

				return copy_8_avx_64(first, size, begin);
			}
		}

		ful_target("avx") ful_inline
		char8 * rcopy_8_avx_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - 32), b);

			return begin;
		}

		ful_target("avx") ful_inline
		char8 * rcopy_8_avx(const char8 * first, const char8 * last, char8 * end)
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
				return rcopy_8_avx_32(first, last, end - size, end);
			}
			else
			{
				extern char8 * rcopy_8_avx_64(usize size, const char8 * last, char8 * end);

				return rcopy_8_avx_64(size, last, end);
			}
		}

		ful_target("avx") inline
		void fill_large_avx(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
			if (!ful_expect(16 <= size))
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
				const __m256i u256 = _mm256_set1_epi8(u);

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);

				from = ful_align_next_32(from);

				char8 * const to_word = to - 64;
				if (from < to_word)
				{
					do
					{
						_mm256_stream_si256(reinterpret_cast<__m256i *>(from), u256);
						_mm256_stream_si256(reinterpret_cast<__m256i *>(from + 32), u256);

						from += 64;
					}
					while (from < to_word);

					from = ful_align_next_32(to_word);
				}

				_mm256_stream_si256(reinterpret_cast<__m256i *>(from), u256);
				_mm_sfence();

				_mm256_storeu_si256(reinterpret_cast<__m256i *>(to_word + 32), u256);
			}
		}
	}
}
