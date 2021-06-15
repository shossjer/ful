#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace ful
{
	namespace detail
	{
		ful_target("sse")
		char8 * memcopy_sse_64(const char8 * first, usize size, char8 * begin)
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
		char8 * memypoc_sse_64(usize size, const char8 * last, char8 * end)
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

		ful_target("sse2")
		char8 * memswap_sse_64(char8 * beg1, usize size, char8 * beg2)
		{
			const usize end_index = size - 64;
			usize index = 0;
			do
			{
				const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
				const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index + 16));
				const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index));
				const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index + 16));
				_mm_storeu_ps(reinterpret_cast<float *>(beg2 + index), a1);
				_mm_storeu_ps(reinterpret_cast<float *>(beg2 + index + 16), b1);
				_mm_storeu_ps(reinterpret_cast<float *>(beg1 + index), a2);
				_mm_storeu_ps(reinterpret_cast<float *>(beg1 + index + 16), b2);

				index += 32;
			}
			while (index <= end_index);

			const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
			const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index + 16));
			const __m128 c1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_index + 32));
			const __m128 d1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_index + 48));
			const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index));
			const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index + 16));
			const __m128 c2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_index + 32));
			const __m128 d2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_index + 48));
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + index), a1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + index + 16), b1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + end_index + 32), c1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + end_index + 48), d1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + index), a2);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + index + 16), b2);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + end_index + 32), c2);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + end_index + 48), d2);

			return beg2 + size;
		}

		ful_target("sse")
		void memset8_sse_64(char8 * from, char8 * to, char8 u)
		{
			alignas(4) uint32 bytes = 0x01010101u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);

			from = ful_align_next_16(from);

			char8 * const to_word = to - 64;
			if (from < to_word)
			{
				do
				{
					_mm_stream_ps(reinterpret_cast<float *>(from), u128);
					_mm_stream_ps(reinterpret_cast<float *>(from + 16), u128);
					_mm_stream_ps(reinterpret_cast<float *>(from + 32), u128);
					_mm_stream_ps(reinterpret_cast<float *>(from + 48), u128);

					from += 64;
				}
				while (from < to_word);

				from = ful_align_next_16(to_word);
			}

			_mm_stream_ps(reinterpret_cast<float *>(from), u128);
			_mm_stream_ps(reinterpret_cast<float *>(from + 16), u128);
			_mm_stream_ps(reinterpret_cast<float *>(from + 32), u128);
			_mm_sfence();

			_mm_storeu_ps(reinterpret_cast<float *>(to_word + 48), u128);
		}
	}
}
