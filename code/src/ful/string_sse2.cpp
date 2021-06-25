#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace
{
	ful_target("sse2")
	void memset_sse2_64(ful::char8 * from, ful::char8 * to, __m128i u128)
	{
		_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);

		from = ful_align_next_16(from);

		ful::char8 * const to_word = to - 64;
		if (from < to_word)
		{
			do
			{
				_mm_stream_si128(reinterpret_cast<__m128i *>(from), u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 16), u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 32), u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 48), u128);

				from += 64;
			}
			while (from < to_word);

			from = ful_align_next_16(to_word);
		}

		_mm_stream_si128(reinterpret_cast<__m128i *>(from), u128);
		_mm_stream_si128(reinterpret_cast<__m128i *>(from + 16), u128);
		_mm_stream_si128(reinterpret_cast<__m128i *>(from + 32), u128);
		_mm_sfence();

		_mm_storeu_si128(reinterpret_cast<__m128i *>(to_word + 48), u128);
	}
}

namespace ful
{
	namespace detail
	{
		ful_target("sse2")
		char8 * memcopy_sse2_64(const char8 * first, usize size, char8 * begin)
		{
			const usize begin_offset = ful_align_next_16(begin) - begin;
			{
				const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(begin), a);
			}

			const usize end_index = size - 64;
			usize index = begin_offset;
			if (index < end_index)
			{
				do
				{
					const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index));
					const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index + 16));
					const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index + 32));
					const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + index + 48));
					_mm_stream_si128(reinterpret_cast<__m128i *>(begin + index), a);
					_mm_stream_si128(reinterpret_cast<__m128i *>(begin + index + 16), b);
					_mm_stream_si128(reinterpret_cast<__m128i *>(begin + index + 32), c);
					_mm_stream_si128(reinterpret_cast<__m128i *>(begin + index + 48), d);

					index += 64;
				}
				while (index < end_index);

				_mm_sfence();
			}

			const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index));
			const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index + 16));
			const __m128i c = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index + 32));
			const __m128i d = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + end_index + 48));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index), a);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index + 16), b);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index + 32), c);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + end_index + 48), d);

			return begin + size;
		}

		ful_target("sse2")
		char8 * memmovef_sse2_64(const char8 * first, usize size, char8 * begin)
		{
			// todo benchmark stream vs unaligned
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
		char8 * memmover_sse2_64(usize size, const char8 * last, char8 * end)
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

		ful_target("sse2")
		void memset8_sse2_64(char8 * from, char8 * to, char8 u)
		{
			const __m128i u128 = _mm_set1_epi8(u);

			memset_sse2_64(from, to, u128);
		}

		ful_target("sse2")
		void memset16_sse2_64(char16 * from, char16 * to, char16 u)
		{
			const __m128i u128 = _mm_set1_epi16(u);

			memset_sse2_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u128);
		}

		ful_target("sse2")
		void memset32_sse2_64(char32 * from, char32 * to, char32 u)
		{
			const __m128i u128 = _mm_set1_epi32(u);

			memset_sse2_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u128);
		}

		ful_target("sse2")
		char8 * memswap_sse2_64(char8 * beg1, usize size, char8 * beg2)
		{
			const usize end_index = size - 64;
			usize index = 0;
			do
			{
				const __m128i a1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
				const __m128i b1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index + 16));
				const __m128i a2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index));
				const __m128i b2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index + 16));
				_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + index), a1);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + index + 16), b1);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + index), a2);
				_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + index + 16), b2);

				index += 32;
			}
			while (index <= end_index);

			const __m128i a1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
			const __m128i b1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index + 16));
			const __m128i c1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_index + 32));
			const __m128i d1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_index + 48));
			const __m128i a2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index));
			const __m128i b2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index + 16));
			const __m128i c2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_index + 32));
			const __m128i d2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_index + 48));
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + index), a1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + index + 16), b1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + end_index + 32), c1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg2 + end_index + 48), d1);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + index), a2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + index + 16), b2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + end_index + 32), c2);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(beg1 + end_index + 48), d2);

			return beg2 + size;
		}
	}
}
