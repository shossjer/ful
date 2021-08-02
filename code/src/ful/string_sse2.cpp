#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace
{
	ful_target("sse2")
	void memset_sse2_64(ful::byte * from, ful::byte * to, __m128i u128)
	{
		_mm_storeu_si128(reinterpret_cast<__m128i *>(from), u128);

		from = ful_align_next_16(from);

		ful::byte * const to_word = to - 64;
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

	ful_target("sse2")
	void memset_sse2_96(ful::byte * from, ful::byte * to, __m128i lo_u128, __m128i mi_u128, __m128i hi_u128)
	{
		ful::byte * const to_word = to - 96;
		if (from < to_word)
		{
			do
			{
				_mm_stream_si128(reinterpret_cast<__m128i *>(from), lo_u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 16), mi_u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 32), hi_u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 48), lo_u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 64), mi_u128);
				_mm_stream_si128(reinterpret_cast<__m128i *>(from + 80), hi_u128);

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
		ful_target("sse2")
		byte * memcopy_sse2_64(const byte * first, usize size, byte * begin)
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
		byte * memmovef_sse2_64(const byte * first, usize size, byte * begin)
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
		byte * memmover_sse2_64(usize size, const byte * last, byte * end)
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

			memset_sse2_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u128);
		}

		ful_target("sse2")
		void memset16_sse2_64(char16 * from, char16 * to, char16 u)
		{
			const __m128i u128 = _mm_set1_epi16(u);

			memset_sse2_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u128);
		}

		ful_target("sse2")
		void memset24_sse2_96(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			// lo 0100000100000100 0001000001000001
			// mi 0001000001000001 0000010000010000
			// hi 0000010000010000 0100000100000100
			const __m128i lo_u128 = _mm_set_epi64x(bytes1, bytes0);
			const __m128i mi_u128 = _mm_set_epi64x(bytes0, bytes2);
			const __m128i hi_u128 = _mm_set_epi64x(bytes2, bytes1);

			_mm_storeu_si128(reinterpret_cast<__m128i *>(from), lo_u128);

			char24 * const from_aligned = ful_align_next_16(from);

			switch (reinterpret_cast<byte *>(from_aligned) - reinterpret_cast<byte *>(from))
			{
			case 1: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			case 2: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u128, hi_u128, lo_u128); break;
			case 3: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u128, mi_u128, hi_u128); break;
			case 4: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			case 5: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u128, hi_u128, lo_u128); break;
			case 6: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u128, mi_u128, hi_u128); break;
			case 7: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			case 8: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u128, hi_u128, lo_u128); break;
			case 9: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u128, mi_u128, hi_u128); break;
			case 10: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			case 11: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u128, hi_u128, lo_u128); break;
			case 12: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u128, mi_u128, hi_u128); break;
			case 13: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			case 14: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u128, hi_u128, lo_u128); break;
			case 15: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u128, mi_u128, hi_u128); break;
			case 16: memset_sse2_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u128, lo_u128, mi_u128); break;
			default: ful_unreachable();
			}

			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 96), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 80), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 64), hi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 48), lo_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_si128(reinterpret_cast<__m128i *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse2")
		void memset32_sse2_64(char32 * from, char32 * to, char32 u)
		{
			const __m128i u128 = _mm_set1_epi32(u);

			memset_sse2_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u128);
		}

		ful_target("sse2")
		byte * memswap_sse2_64(byte * beg1, usize size, byte * beg2)
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
