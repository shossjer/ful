#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace
{
	ful_target("sse")
	void memset_sse_64(ful::char8 * from, ful::char8 * to, __m128 u128)
	{
		_mm_storeu_ps(reinterpret_cast<float *>(from), u128);

		from = ful_align_next_16(from);

		ful::char8 * const to_word = to - 64;
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

	ful_target("sse2")
	void memset_sse_96(ful::char8 * from, ful::char8 * to, __m128 lo_u128, __m128 mi_u128, __m128 hi_u128)
	{
		ful::char8 * const to_word = to - 96;
		if (from < to_word)
		{
			do
			{
				_mm_stream_ps(reinterpret_cast<float *>(from), lo_u128);
				_mm_stream_ps(reinterpret_cast<float *>(from + 16), mi_u128);
				_mm_stream_ps(reinterpret_cast<float *>(from + 32), hi_u128);
				_mm_stream_ps(reinterpret_cast<float *>(from + 48), lo_u128);
				_mm_stream_ps(reinterpret_cast<float *>(from + 64), mi_u128);
				_mm_stream_ps(reinterpret_cast<float *>(from + 80), hi_u128);

				from += 96;
			}
			while (from < to_word);

			_mm_sfence();
		}
	}
}

namespace ful
{
	namespace detail
	{
		ful_target("sse")
		char8 * memcopy_sse_64(const char8 * first, usize size, char8 * begin)
		{
			const usize begin_offset = ful_align_next_16(begin) - begin;
			{
				const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
				_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			}

			const usize end_index = size - 64;
			usize index = begin_offset;
			if (index < end_index)
			{
				do
				{
					const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first + index));
					const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + index + 16));
					const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(first + index + 32));
					const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(first + index + 48));
					_mm_stream_ps(reinterpret_cast<float *>(begin + index), a);
					_mm_stream_ps(reinterpret_cast<float *>(begin + index + 16), b);
					_mm_stream_ps(reinterpret_cast<float *>(begin + index + 32), c);
					_mm_stream_ps(reinterpret_cast<float *>(begin + index + 48), d);

					index += 64;
				}
				while (index < end_index);

				_mm_sfence();
			}

			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index + 16));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index + 32));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(first + end_index + 48));
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index), a);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index + 16), b);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index + 32), c);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + end_index + 48), d);

			return begin + size;
		}

		ful_target("sse")
		char8 * memmovef_sse_64(const char8 * first, usize size, char8 * begin)
		{
			// todo benchmark stream vs unaligned
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
		char8 * memmover_sse_64(usize size, const char8 * last, char8 * end)
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

		ful_target("sse")
		void memset8_sse_64(char8 * from, char8 * to, char8 u)
		{
			uint32 bytes = 0x01010101u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			memset_sse_64(from, to, u128);
		}

		ful_target("sse")
		void memset16_sse_64(char16 * from, char16 * to, char16 u)
		{
			uint32 bytes = 0x00010001u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			memset_sse_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u128);
		}

		ful_target("sse")
		void memset24_sse_96(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			const uint64 bytes[4] = {bytes0, bytes1, bytes2, bytes0};
			// lo 0100000100000100 0001000001000001
			// mi 0001000001000001 0000010000010000
			// hi 0000010000010000 0100000100000100
			const __m128 lo_u128 = _mm_load_ps1(reinterpret_cast<const float *>(bytes));
			const __m128 mi_u128 = _mm_load_ps1(reinterpret_cast<const float *>(bytes + 2));
			const __m128 hi_u128 = _mm_load_ps1(reinterpret_cast<const float *>(bytes + 1));

			_mm_storeu_ps(reinterpret_cast<float *>(from), lo_u128);

			char24 * const from_aligned = ful_align_next_16(from);

			switch (reinterpret_cast<char8 *>(from_aligned) - reinterpret_cast<char8 *>(from))
			{
			case 1: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			case 2: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), mi_u128, hi_u128, lo_u128); break;
			case 3: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), lo_u128, mi_u128, hi_u128); break;
			case 4: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			case 5: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), mi_u128, hi_u128, lo_u128); break;
			case 6: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), lo_u128, mi_u128, hi_u128); break;
			case 7: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			case 8: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), mi_u128, hi_u128, lo_u128); break;
			case 9: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), lo_u128, mi_u128, hi_u128); break;
			case 10: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			case 11: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), mi_u128, hi_u128, lo_u128); break;
			case 12: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), lo_u128, mi_u128, hi_u128); break;
			case 13: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			case 14: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), mi_u128, hi_u128, lo_u128); break;
			case 15: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), lo_u128, mi_u128, hi_u128); break;
			case 16: memset_sse_96(reinterpret_cast<char8 *>(from_aligned), reinterpret_cast<char8 *>(to), hi_u128, lo_u128, mi_u128); break;
			default: ful_unreachable();
			}

			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 96), lo_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 80), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 64), hi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 48), lo_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse")
		void memset32_sse_64(char32 * from, char32 * to, char32 u)
		{
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&u));

			memset_sse_64(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u128);
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
	}
}
