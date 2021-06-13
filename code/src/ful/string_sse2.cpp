#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace ful
{
	namespace detail
	{
		ful_target("sse2")
		char8 * copy_8_sse2_64(const char8 * first, usize size, char8 * begin)
		{
			const usize end_index = size - 64;
			usize index = 0;
			do
			{
				const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index));
				const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index + 16));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + index), a);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + index + 16), b);

				index += 32;
			}
			while (index <= end_index);

			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index + 16));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index + 32));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index + 48));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + index), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + index + 16), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index + 32), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index + 48), d);

			return begin + size;
		}

		ful_target("sse2")
		char8 * rcopy_8_sse2_64(usize size, const char8 * last, char8 * end)
		{
			const usize begin_index = size - 64;
			usize index = 0;
			do
			{
				const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - index - 16));
				const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - index - 32));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(end - index - 16), a);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(end - index - 32), b);

				index += 32;
			}
			while (index <= begin_index);

			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - index - 16));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - index - 32));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - begin_index - 48));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(last - begin_index - 64));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - index - 16), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - index - 32), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - begin_index - 48), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(end - begin_index - 64), d);

			return end - size;
		}
	}
}
