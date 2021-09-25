#pragma once

namespace ful
{
	namespace detail
	{
		ful_target("sse") ful_inline
		void copy_sse_16_32(const byte * first, const byte * last, byte * begin, byte * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), b);
		}

		ful_target("sse") ful_inline
		void copy_sse_32_64(const byte * first, const byte * last, byte * begin, byte * end)
		{
			const __m128 a = _mm_loadu_ps(reinterpret_cast<const float *>(first));
			const __m128 b = _mm_loadu_ps(reinterpret_cast<const float *>(first + 16));
			const __m128 c = _mm_loadu_ps(reinterpret_cast<const float *>(last - 32));
			const __m128 d = _mm_loadu_ps(reinterpret_cast<const float *>(last - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(begin), a);
			_mm_storeu_ps(reinterpret_cast<float *>(begin + 16), b);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 32), c);
			_mm_storeu_ps(reinterpret_cast<float *>(end - 16), d);
		}

		ful_target("sse") ful_inline
		byte * memcopy_sse(const byte * first, const byte * last, byte * begin)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32u)
			{
				copy_sse_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 64u)
			{
				copy_sse_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern byte * memcopy_sse_64(const byte * first, usize size, byte * begin);

				return memcopy_sse_64(first, size, begin);
			}
		}

		ful_target("sse") ful_inline
		byte * memmovef_sse(const byte * first, const byte * last, byte * begin)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32u)
			{
				copy_sse_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else if (size <= 64u)
			{
				copy_sse_32_64(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern byte * memmovef_sse_64(const byte * first, usize size, byte * begin);

				return memmovef_sse_64(first, size, begin);
			}
		}

		ful_target("sse") ful_inline
		byte * memmover_sse(const byte * first, const byte * last, byte * end)
		{
			const usize size = last - first;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return end;

			if (size <= 32u)
			{
				copy_sse_16_32(first, last, end - size, end);

				return end - size;
			}
			else if (size <= 64u)
			{
				copy_sse_32_64(first, last, end - size, end);

				return end - size;
			}
			else
			{
				extern byte * memmover_sse_64(usize size, const byte * last, byte * end);

				return memmover_sse_64(size, last, end);
			}
		}

		ful_target("sse") inline
		void set8_sse_16_32(char8 * from, char8 * to, char8 u)
		{
			const uint32 bytes = 0x01010101u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 16), u128);
		}

		ful_target("sse") inline
		void set8_sse_32_64(char8 * from, char8 * to, char8 u)
		{
			const uint32 bytes = 0x01010101u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(from + 16), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 32), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 16), u128);
		}

		ful_target("sse") ful_inline
		void memset8_sse(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set8_sse_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set8_sse_32_64(from, to, u);
			}
			else
			{
				extern void memset8_sse_64(char8 * from, char8 * to, char8 u);

				memset8_sse_64(from, to, u);
			}
		}

		ful_target("sse") inline
		void set16_sse_16_32(char16 * from, char16 * to, char16 u)
		{
			const uint32 bytes = 0x00010001u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 8), u128);
		}

		ful_target("sse") inline
		void set16_sse_32_64(char16 * from, char16 * to, char16 u)
		{
			const uint32 bytes = 0x00010001u * u;
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&bytes));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(from + 8), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 16), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 8), u128);
		}

		ful_target("sse") ful_inline
		void memset16_sse(char16 * from, char16 * to, char16 u)
		{
			const usize size = (to - from) * sizeof(char16);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set16_sse_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set16_sse_32_64(from, to, u);
			}
			else
			{
				extern void memset16_sse_64(char16 * from, char16 * to, char16 u);

				memset16_sse_64(from, to, u);
			}
		}

		ful_target("sse") inline
		void set24_sse_16_32(char24 * from, char24 * to, char_fast24 u)
		{
			// todo benchmark
			const uint64 bytes0 = 0x0001000001000001u * static_cast<uint32>(u);
			const uint64 bytes1 = (bytes0 << 8) | (static_cast<uint32>(u) >> 16);
			const uint64 bytes2 = (bytes0 << 16) | (static_cast<uint32>(u) >> 8);
			const uint64 bytes[3] = {bytes0, bytes1, bytes2};
			// lo 0100000100000100 0001000001000001
			// hi 0000010000010000 0100000100000100
			const __m128 lo_u128 = _mm_load_ps1(reinterpret_cast<const float *>(bytes));
			const __m128 hi_u128 = _mm_load_ps1(reinterpret_cast<const float *>(bytes + 1));

			_mm_storeu_ps(reinterpret_cast<float *>(from), lo_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse") inline
		void set24_sse_32_64(char24 * from, char24 * to, char_fast24 u)
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
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 16), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse") inline
		void set24_sse_64_96(char24 * from, char24 * to, char_fast24 u)
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
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 16), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 32), hi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 48), lo_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 64), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(from) + 80), hi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 32), mi_u128);
			_mm_storeu_ps(reinterpret_cast<float *>(reinterpret_cast<char8 *>(to) - 16), hi_u128);
		}

		ful_target("sse") ful_inline
		void memset24_sse(char24 * from, char24 * to, char_fast24 u)
		{
			const usize size = (to - from) * sizeof(char24);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set24_sse_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set24_sse_32_64(from, to, u);
			}
			else if (size <= 96u)
			{
				set24_sse_64_96(from, to, u);
			}
			else
			{
				extern void memset24_sse_96(char24 * from, char24 * to, char_fast24 u);

				memset24_sse_96(from, to, u);
			}
		}

		ful_target("sse") inline
		void set32_sse_16_32(char32 * from, char32 * to, char32 u)
		{
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&u));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 4), u128);
		}

		ful_target("sse") inline
		void set32_sse_32_64(char32 * from, char32 * to, char32 u)
		{
			const __m128 u128 = _mm_load_ps1(reinterpret_cast<const float *>(&u));

			_mm_storeu_ps(reinterpret_cast<float *>(from), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(from + 4), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 8), u128);
			_mm_storeu_ps(reinterpret_cast<float *>(to - 4), u128);
		}

		ful_target("sse") ful_inline
		void memset32_sse(char32 * from, char32 * to, char32 u)
		{
			const usize size = (to - from) * sizeof(char32);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			if (size <= 32u)
			{
				set32_sse_16_32(from, to, u);
			}
			else if (size <= 64u)
			{
				set32_sse_32_64(from, to, u);
			}
			else
			{
				extern void memset32_sse_64(char32 * from, char32 * to, char32 u);

				memset32_sse_64(from, to, u);
			}
		}

		ful_target("sse") ful_inline
		void swap_sse_16_32(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1));
			const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
			const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2));
			const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(beg2), a1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 16), b1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1), a2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 16), b2);
		}

		ful_target("sse") ful_inline
		void swap_sse_32_64(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			const __m128 a1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1));
			const __m128 b1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + 16));
			const __m128 c1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 32));
			const __m128 d1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
			const __m128 a2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2));
			const __m128 b2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + 16));
			const __m128 c2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 32));
			const __m128 d2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
			_mm_storeu_ps(reinterpret_cast<float *>(beg2), a1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg2 + 16), b1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 32), c1);
			_mm_storeu_ps(reinterpret_cast<float *>(end2 - 16), d1);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1), a2);
			_mm_storeu_ps(reinterpret_cast<float *>(beg1 + 16), b2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 32), c2);
			_mm_storeu_ps(reinterpret_cast<float *>(end1 - 16), d2);
		}

		ful_target("sse") ful_inline
		byte * memswap_sse(byte * beg1, byte * end1, byte * beg2)
		{
			const usize size = end1 - beg1;
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return beg2;

			if (size <= 32)
			{
				swap_sse_16_32(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else if (size <= 64)
			{
				swap_sse_32_64(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else
			{
				extern byte * memswap_sse_64(byte * beg1, usize size, byte * beg2);

				return memswap_sse_64(beg1, size, beg2);
			}
		}
	}
}
