#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace ful
{
	namespace detail
	{
		ful_target("avx")
		char8 * copy_8_avx_64(const char8 * first, usize size, char8 * begin)
		{
			// todo no compiler generates "perfect" assembly
			const usize end_index = size - 128;
			usize index = 0;
			if (128 <= size)
			{
				do
				{
					const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index));
					const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index + 32));
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + index), a);
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + index + 32), b);

					index += 64;
				}
				while (index <= end_index);
			}

			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index + 32));
			const __m256i c = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index + 64));
			const __m256i d = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index + 96));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + index), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + index + 32), b);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index + 64), c);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index + 96), d);

			return begin + size;
		}

		ful_target("avx")
		char8 * rcopy_8_avx_64(usize size, const char8 * last, char8 * end)
		{
			const usize begin_index = size - 128;
			usize index = 0;
			if (128 <= size)
			{
				do
				{
					const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - index - 32));
					const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - index - 64));
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - index - 32), a);
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - index - 64), b);

					index += 64;
				}
				while (index <= begin_index);
			}

			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - index - 32));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - index - 64));
			const __m256i c = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - begin_index - 96));
			const __m256i d = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(last - begin_index - 128));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - index - 32), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - index - 64), b);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - begin_index - 96), c);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(end - begin_index - 128), d);

			return end - size;
		}
	}
}
