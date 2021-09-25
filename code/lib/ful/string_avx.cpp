#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

namespace
{
	ful_target("avx")
	void memset_avx_64(ful::byte * from, ful::byte * to, __m256i u256)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), u256);

		from = ful_align_next_32(from);

		ful::byte * const to_word = to - 64;
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

	ful_target("avx")
	void memset_avx_96(ful::byte * from, ful::byte * to, __m256i lo_u256, __m256i mi_u256, __m256i hi_u256)
	{
		ful::byte * const to_word = to - 96;
		if (from < to_word)
		{
			do
			{
				_mm256_stream_si256(reinterpret_cast<__m256i *>(from), lo_u256);
				_mm256_stream_si256(reinterpret_cast<__m256i *>(from + 32), mi_u256);
				_mm256_stream_si256(reinterpret_cast<__m256i *>(from + 64), hi_u256);

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
		ful_target("avx")
		byte * memcopy_avx_64(const byte * first, usize size, byte * begin)
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
		byte * memmovef_avx_64(const byte * first, usize size, byte * begin)
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
		byte * memmover_avx_64(usize size, const byte * last, byte * end)
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

			memset_avx_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u256);
		}

		ful_target("avx")
		void memset16_avx_64(char16 * from, char16 * to, char16 u)
		{
			const __m256i u256 = _mm256_set1_epi16(u);

			memset_avx_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u256);
		}

		ful_target("avx")
		void memset24_avx_96(char24 * from, char24 * to, char_fast24 u)
		{
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			// lo 0001000001000001 0000010000010000 0100000100000100 0001000001000001
			// mi 0100000100000100 0001000001000001 0000010000010000 0100000100000100
			// hi 0000010000010000 0100000100000100 0001000001000001 0000010000010000
			const __m256i lo_u256 = _mm256_set_epi64x(bytes0, bytes2, bytes1, bytes0);
			const __m256i mi_u256 = _mm256_set_epi64x(bytes1, bytes0, bytes2, bytes1);
			const __m256i hi_u256 = _mm256_set_epi64x(bytes2, bytes1, bytes0, bytes2);

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(from), lo_u256);

			char24 * const from_aligned = ful_align_next_32(from);

			switch (reinterpret_cast<byte *>(from_aligned) - reinterpret_cast<byte *>(from))
			{
			case 1: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 2: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 3: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 4: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 5: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 6: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 7: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 8: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 9: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 10: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 11: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 12: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 13: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 14: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 15: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 16: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 17: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 18: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 19: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 20: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 21: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 22: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 23: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 24: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 25: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 26: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 27: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 28: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 29: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			case 30: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), lo_u256, mi_u256, hi_u256); break;
			case 31: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), hi_u256, lo_u256, mi_u256); break;
			case 32: memset_avx_96(reinterpret_cast<byte *>(from_aligned), reinterpret_cast<byte *>(to), mi_u256, hi_u256, lo_u256); break;
			default: ful_unreachable();
			}

			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(to) - 96), lo_u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(to) - 64), mi_u256);
			_mm256_storeu_si256(reinterpret_cast<__m256i *>(reinterpret_cast<char8 *>(to) - 32), hi_u256);
		}

		ful_target("avx")
		void memset32_avx_64(char32 * from, char32 * to, char32 u)
		{
			const __m256i u256 = _mm256_set1_epi32(u);

			memset_avx_64(reinterpret_cast<byte *>(from), reinterpret_cast<byte *>(to), u256);
		}

		ful_target("avx")
		byte * memswap_avx_64(byte * beg1, usize size, byte * beg2)
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
