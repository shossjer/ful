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
	}
}
