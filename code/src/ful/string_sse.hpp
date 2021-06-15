#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("sse") ful_inline
		char8 * memcopy_sse_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), b);

			return end;
		}

		ful_target("sse") ful_inline
		char8 * memcopy_sse_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + 16));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(last - 32));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + 16), b);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 32), c);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), d);

			return end;
		}

		ful_target("sse") ful_inline
		char8 * memcopy_sse(const char8 * first, const char8 * last, char8 * begin)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return begin;

			if (size <= 32)
			{
				return memcopy_sse_16(first, last, begin, begin + size);
			}
			else if (size <= 64)
			{
				return memcopy_sse_32(first, last, begin, begin + size);
			}
			else
			{
				extern char8 * memcopy_sse_64(const char8 * first, usize size, char8 * begin);

				return memcopy_sse_64(first, size, begin);
			}
		}

		ful_target("sse") ful_inline
		char8 * memypoc_sse_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), b);

			return begin;
		}

		ful_target("sse") ful_inline
		char8 * memypoc_sse_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + 16));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(last - 32));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + 16), b);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 32), c);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), d);

			return begin;
		}

		ful_target("sse") ful_inline
		char8 * memypoc_sse(const char8 * first, const char8 * last, char8 * end)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return end;

			if (size <= 32)
			{
				return memypoc_sse_16(first, last, end - size, end);
			}
			else if (size <= 64)
			{
				return memypoc_sse_32(first, last, end - size, end);
			}
			else
			{
				extern char8 * memypoc_sse_64(usize size, const char8 * last, char8 * end);

				return memypoc_sse_64(size, last, end);
			}
		}

		ful_target("sse") ful_inline
		char8 * memswap_sse_16(char8 * beg1, char8 * end1, char8 * beg2, char8 * end2)
		{
			const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1));
			const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
			const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2));
			const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(beg2), a1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 16), b1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1), a2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 16), b2);

			return end2;
		}

		ful_target("sse") ful_inline
		char8 * memswap_sse_32(char8 * beg1, char8 * end1, char8 * beg2, char8 * end2)
		{
			const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1));
			const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + 16));
			const __m128 c1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 32));
			const __m128 d1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
			const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2));
			const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + 16));
			const __m128 c2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 32));
			const __m128 d2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(beg2), a1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + 16), b1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 32), c1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 16), d1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1), a2);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + 16), b2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 32), c2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 16), d2);

			return end2;
		}

		ful_target("sse") ful_inline
		char8 * memswap_sse(char8 * beg1, char8 * end1, char8 * beg2)
		{
			const usize size = end1 - beg1;
			if (!ful_expect(16u < size))
				return beg2;

			if (size <= 32)
			{
				return memswap_sse_16(beg1, end1, beg2, beg2 + size);
			}
			else if (size <= 64)
			{
				return memswap_sse_32(beg1, end1, beg2, beg2 + size);
			}
			else
			{
				extern char8 * memswap_sse_64(char8 * beg1, usize size, char8 * beg2);

				return memswap_sse_64(beg1, size, beg2);
			}
		}

		ful_target("sse") ful_inline
		void memset8_sse(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
			if (!ful_expect(16u < size))
				return;

			if (size <= 32)
			{
				alignas(4) uint32 bytes = 0x01010101u * u;
				const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

				_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
				_mm_storeu_ps(reinterpret_cast<float *>(to - 16), u128);
			}
			else if (size <= 64)
			{
				alignas(4) uint32 bytes = 0x01010101u * u;
				const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

				_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
				_mm_storeu_ps(reinterpret_cast<float *>(from + 16), u128);
				_mm_storeu_ps(reinterpret_cast<float *>(to - 32), u128);
				_mm_storeu_ps(reinterpret_cast<float *>(to - 16), u128);
			}
			else
			{
				extern void memset8_sse_64(char8 * from, char8 * to, char8 u);

				memset8_sse_64(from, to, u);
			}
		}
	}
}
