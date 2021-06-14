#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("sse") ful_inline
		char8 * copy_8_sse_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), b);

			return end;
		}

		ful_target("sse") ful_inline
		char8 * copy_8_sse_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
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
		char8 * copy_8_sse(const char8 * first, const char8 * last, char8 * begin)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return begin;

			if (size <= 32)
			{
				return copy_8_sse_16(first, last, begin, begin + size);
			}
			else if (size <= 64)
			{
				return copy_8_sse_32(first, last, begin, begin + size);
			}
			else
			{
				extern char8 * copy_8_sse_64(const char8 * first, usize size, char8 * begin);

				return copy_8_sse_64(first, size, begin);
			}
		}

		ful_target("sse") ful_inline
		char8 * rcopy_8_sse_16(const char8 * first, const char8 * last, char8 * begin, char8 * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), b);

			return begin;
		}

		ful_target("sse") ful_inline
		char8 * rcopy_8_sse_32(const char8 * first, const char8 * last, char8 * begin, char8 * end)
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
		char8 * rcopy_8_sse(const char8 * first, const char8 * last, char8 * end)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return end;

			if (size <= 32)
			{
				return rcopy_8_sse_16(first, last, end - size, end);
			}
			else if (size <= 64)
			{
				return rcopy_8_sse_32(first, last, end - size, end);
			}
			else
			{
				extern char8 * rcopy_8_sse_64(usize size, const char8 * last, char8 * end);

				return rcopy_8_sse_64(size, last, end);
			}
		}
	}
}
