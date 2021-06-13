#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace ful
{
	namespace detail
	{
		ful_target("sse")
		char8 * copy_8_sse_64(const char8 * first, usize size, char8 * begin)
		{
			const usize end_index = size - 64;
			usize index = 0;
			do
			{
				const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first + index));
				const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + index + 16));
				_mm_storeu_ps(reinterpret_cast<float *>(begin + index), a);
				_mm_storeu_ps(reinterpret_cast<float *>(begin + index + 16), b);

				index += 32;
			}
			while (index <= end_index);

			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first + index));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + index + 16));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index + 32));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index + 48));
			_mm_storeu_ps(reinterpret_cast<float *>(begin + index), a);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + index + 16), b);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index + 32), c);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index + 48), d);

			return begin + size;
		}

		ful_target("sse")
		char8 * rcopy_8_sse_64(usize size, const char8 * last, char8 * end)
		{
			const usize begin_index = size - 64;
			usize index = 0;
			do
			{
				const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(last - index - 16));
				const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - index - 32));
				_mm_storeu_ps(reinterpret_cast<float *>(end - index - 16), a);
				_mm_storeu_ps(reinterpret_cast<float *>(end - index - 32), b);

				index += 32;
			}
			while (index <= begin_index);

			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(last - index - 16));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - index - 32));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(last - begin_index - 48));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(last - begin_index - 64));
			_mm_storeu_ps(reinterpret_cast<float *>(end - index - 16), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - index - 32), b);
			_mm_storeu_ps(reinterpret_cast<float *>(end - begin_index - 48), c);
			_mm_storeu_ps(reinterpret_cast<float *>(end - begin_index - 64), d);

			return end - size;
		}
	}
}
