#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace
{
	ful_target("avx")
	void memset_avx_64(ful::char8 * from, ful::char8 * to, __m256i u256)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);

		from = ful_align_next_32(from);

		ful::char8 * const to_word = to - 64;
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

namespace ful
{
	namespace detail
	{
		ful_target("avx")
		char8 * memcopy_avx_64(const char8 * first, usize size, char8 * begin)
		{
			const usize begin_offset = ful_align_next_32(begin) - begin;
			{
				const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
			}

			const usize end_index = size - 64;
			usize index = begin_offset;
			if (index < end_index)
			{
				do
				{
					const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index));
					const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index + 32));
					_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index), a);
					_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index + 32), b);

					index += 64;
				}
				while (index < end_index);

				_mm_sfence();
			}

			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index + 32));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index), a);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index + 32), b);

			return begin + size;
		}

		ful_target("avx")
		char8 * memmovef_avx_64(const char8 * first, usize size, char8 * begin)
		{
			// todo benchmark stream vs unaligned
			const usize begin_offset = ful_align_next_32(begin) - begin;
			{
				const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + begin_offset));
				_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), a);
				_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + begin_offset), b);
			}

			const usize end_index = size - 128;
			usize index = begin_offset + 32;
			if (index < end_index)
			{
				do
				{
					const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index));
					const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index + 32));
					_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index), a);
					_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index + 32), b);

					index += 64;
				}
				while (index < end_index);
			}

			const __m256i a = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index));
			const __m256i b = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + index + 32));
			const __m256i c = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index + 64));
			const __m256i d = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first + end_index + 96));
			_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index), a);
			_mm256_stream_si256(reinterpret_cast<__m256i *>(begin + index + 32), b);
			_mm_sfence();
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index + 64), c);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + end_index + 96), d);

			return begin + size;
		}

		ful_target("avx")
		char8 * memmover_avx_64(usize size, const char8 * last, char8 * end)
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

		ful_target("avx")
		void memset8_avx_64(char8 * from, char8 * to, char8 u)
		{
			const __m256i u256 = _mm256_set1_epi8(u);

			memset_avx_64(from, to, u256);
		}

		ful_target("avx")
		void memset16_avx_64(char16 * from, char16 * to, char16 u)
		{
			const __m256i u256 = _mm256_set1_epi16(u);

			memset_avx_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u256);
		}

		ful_target("avx")
		void memset32_avx_64(char32 * from, char32 * to, char32 u)
		{
			const __m256i u256 = _mm256_set1_epi32(u);

			memset_avx_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u256);
		}

		ful_target("avx")
		char8 * memswap_avx_64(char8 * beg1, usize size, char8 * beg2)
		{
			const usize end_index = size - 128;
			usize index = 0;
			if (128 <= size)
			{
				do
				{
					const __m256i a1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
					const __m256i b1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index + 32));
					const __m256i a2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
					const __m256i b2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index + 32));
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + index), a1);
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + index + 32), b1);
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + index), a2);
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + index + 32), b2);

					index += 64;
				}
				while (index <= end_index);
			}

			const __m256i a1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
			const __m256i b1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index + 32));
			const __m256i c1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_index + 64));
			const __m256i d1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_index + 96));
			const __m256i a2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
			const __m256i b2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index + 32));
			const __m256i c2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_index + 64));
			const __m256i d2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_index + 96));
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + index), a1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + index + 32), b1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + end_index + 64), c1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg2 + end_index + 96), d1);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + index), a2);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + index + 32), b2);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + end_index + 64), c2);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(beg1 + end_index + 96), d2);

			return beg2 + size;
		}
	}
}
