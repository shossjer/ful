#pragma once

namespace ful
{
	namespace detail
	{
		ful_inline
		void copy_none_16_32(const byte * first, const byte * last, byte * begin, byte * end)
		{
			const uint64 a = *reinterpret_cast<const uint64 *>(first);
			const uint64 b = *reinterpret_cast<const uint64 *>(first + 8);
			const uint64 c = *reinterpret_cast<const uint64 *>(last - 16);
			const uint64 d = *reinterpret_cast<const uint64 *>(last - 8);
			*reinterpret_cast<uint64 *>(begin) = a;
			*reinterpret_cast<uint64 *>(begin + 8) = b;
			*reinterpret_cast<uint64 *>(end - 16) = c;
			*reinterpret_cast<uint64 *>(end - 8) = d;
		}

		ful_inline
		byte * memcopy_none(const byte * first, const byte * last, byte * begin)
		{
			const usize size = static_cast<usize>(last - first);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32)
			{
				copy_none_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				// todo 64 bytes a time is slightly faster
				extern byte * memcopy_x86_32(const byte * first, usize size, byte * begin);

				return memcopy_x86_32(first, size, begin);
			}
		}

		ful_inline
		byte * memmovef_none(const byte * first, const byte * last, byte * begin)
		{
			const usize size = static_cast<usize>(last - first);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return begin;

			if (size <= 32)
			{
				copy_none_16_32(first, last, begin, begin + size);

				return begin + size;
			}
			else
			{
				extern byte * memmovef_x86_32(const byte * first, usize size, byte * begin);

				return memmovef_x86_32(first, size, begin);
			}
		}

		ful_inline
		byte * memmover_none(const byte * first, const byte * last, byte * end)
		{
			const usize size = static_cast<usize>(last - first);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return end;

			if (size <= 32)
			{
				copy_none_16_32(first, last, end - size, end);

				return end - size;
			}
			else
			{
				extern byte * memmover_x86_32(usize size, const byte * last, byte * end);

				return memmover_x86_32(size, last, end);
			}
		}

		ful_inline
		void swap_none_16_32(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			const uint64 a1 = *reinterpret_cast<const uint64 *>(beg1);
			const uint64 b1 = *reinterpret_cast<const uint64 *>(beg1 + 8);
			const uint64 c1 = *reinterpret_cast<const uint64 *>(end1 - 16);
			const uint64 d1 = *reinterpret_cast<const uint64 *>(end1 - 8);
			const uint64 a2 = *reinterpret_cast<const uint64 *>(beg2);
			const uint64 b2 = *reinterpret_cast<const uint64 *>(beg2 + 8);
			const uint64 c2 = *reinterpret_cast<const uint64 *>(end2 - 16);
			const uint64 d2 = *reinterpret_cast<const uint64 *>(end2 - 8);
			*reinterpret_cast<uint64 *>(beg2) = a1;
			*reinterpret_cast<uint64 *>(beg2 + 8) = b1;
			*reinterpret_cast<uint64 *>(end2 - 16) = c1;
			*reinterpret_cast<uint64 *>(end2 - 8) = d1;
			*reinterpret_cast<uint64 *>(beg1) = a2;
			*reinterpret_cast<uint64 *>(beg1 + 8) = b2;
			*reinterpret_cast<uint64 *>(end1 - 16) = c2;
			*reinterpret_cast<uint64 *>(end1 - 8) = d2;
		}

		inline
		void memset8_none(char8 * from, char8 * to, char8 u)
		{
			const usize size = static_cast<usize>(to - from);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			// todo benchmark this more
			//
			// https://stackoverflow.com/a/33485055
			// https://agner.org/optimize/optimizing_assembly.pdf 16.9 String instructions (all processors)
			const uint64 bytes = 0x0101010101010101u * (uint8)u;

			*reinterpret_cast<uint64 *>(from) = bytes;

			from = ful_align_next_8(from);

			repstosf(reinterpret_cast<unsigned long long *>(from), static_cast<usize>(to - from) / 8, bytes);

			*reinterpret_cast<uint64 *>(to - 8) = bytes;
		}

		inline
		void memset16_none(char16 * from, char16 * to, char16 u)
		{
			const usize size = static_cast<usize>(to - from) * sizeof(char16);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			// todo benchmark this more
			//
			// https://stackoverflow.com/a/33485055
			// https://agner.org/optimize/optimizing_assembly.pdf 16.9 String instructions (all processors)
			const uint64 bytes = 0x0001000100010001u * (uint16)u;

			*reinterpret_cast<uint64 *>(from) = bytes;

			from = ful_align_next_8(from);

			repstosf(reinterpret_cast<unsigned long long *>(from), static_cast<usize>(to - from) / 8, bytes);

			*reinterpret_cast<uint64 *>(to - 8) = bytes;
		}

		inline
		void memset24_none(char24 * from, char24 * to, char_fast24 u)
		{
			const usize size = static_cast<usize>(to - from) * sizeof(char24);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			// todo
			ful_unused(u);
			ful_break();
		}

		inline
		void memset32_none(char32 * from, char32 * to, char32 u)
		{
			const usize size = static_cast<usize>(to - from) * sizeof(char32);
#if defined(__AVX__)
			if (!ful_expect(64u < size))
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_expect(16u < size))
#endif
				return;

			// todo benchmark this more
			//
			// https://stackoverflow.com/a/33485055
			// https://agner.org/optimize/optimizing_assembly.pdf 16.9 String instructions (all processors)
			const uint64 bytes = 0x0000000100000001u * (uint32)u;

			*reinterpret_cast<uint64 *>(from) = bytes;

			from = ful_align_next_8(from);

			repstosf(reinterpret_cast<unsigned long long *>(from), static_cast<usize>(to - from) / 8, bytes);

			*reinterpret_cast<uint64 *>(to - 8) = bytes;
		}

		ful_inline
		byte * memswap_none(byte * beg1, byte * end1, byte * beg2)
		{
			const usize size = static_cast<usize>(end1 - beg1);
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
				swap_none_16_32(beg1, end1, beg2, beg2 + size);

				return beg2 + size;
			}
			else
			{
				extern byte * memswap_x86_32(byte * beg1, usize size, byte * beg2);

				return memswap_x86_32(beg1, size, beg2);
			}
		}
	}
}
