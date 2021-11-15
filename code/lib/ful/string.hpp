#pragma once

#define FUL_INCLUDED_STRING_HPP

#include "ful/compiler.hpp"
#include "ful/dispatch.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

#include "ful/string_none.hpp"
#include "ful/string_sse.hpp"
#include "ful/string_sse2.hpp"
#include "ful/string_sse4_2.hpp"
#include "ful/string_avx.hpp"
#include "ful/string_avx2.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern byte * ful_dispatch(memcopy)(const byte * first, const byte * last, byte * begin);
		extern byte * ful_dispatch(memmovef)(const byte * first, const byte * last, byte * begin);
		extern byte * ful_dispatch(memmover)(const byte * first, const byte * last, byte * end);
		extern byte * ful_dispatch(memswap)(byte * beg1, byte * end1, byte * beg2);
		extern void ful_dispatch(memset8)(char8 * from, char8 * to, char8 u);
		extern void ful_dispatch(memset16)(char16 * from, char16 * to, char16 u);
		extern void ful_dispatch(memset24)(char24 * from, char24 * to, char_fast24 u);
		extern void ful_dispatch(memset32)(char32 * from, char32 * to, char32 u);
		extern bool ful_dispatch(equal_cstr)(const byte * beg1, const byte * end1, const byte * beg2);
		extern bool ful_dispatch(equal_range)(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2);
#endif
	}

	namespace detail
	{
		ful_inline
		void copy_small(const byte * first, const byte * last, byte * begin, byte * end)
		{
			switch (last - first)
			{
#if defined(__AVX__)
			case 64:
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
			case 56:
			case 55:
			case 54:
			case 53:
			case 52:
			case 51:
			case 50:
			case 49:
			case 48:
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
			case 40:
			case 39:
			case 38:
			case 37:
			case 36:
			case 35:
			case 34:
			case 33:
			{
				detail::copy_avx_32_64(first, last, begin, end);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::copy_sse2_16_32(first, last, begin, end);
# else
				detail::copy_sse_16_32(first, last, begin, end);
# endif

				break;
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			{
				const uint64 a = *reinterpret_cast<const uint64 *>(first);
				const uint64 b = *reinterpret_cast<const uint64 *>(last - 8);
				*reinterpret_cast<uint64 *>(begin) = a;
				*reinterpret_cast<uint64 *>(end - 8) = b;

				break;
			}
			case 8:
			case 7:
			case 6:
			case 5:
			{
				const uint32 a = *reinterpret_cast<const uint32 *>(first);
				const uint32 b = *reinterpret_cast<const uint32 *>(last - 4);
				*reinterpret_cast<uint32 *>(begin) = a;
				*reinterpret_cast<uint32 *>(end - 4) = b;

				break;
			}
			case 4:
			case 3:
			{
				const uint16 a = *reinterpret_cast<const uint16 *>(first);
				const uint16 b = *reinterpret_cast<const uint16 *>(last - 2);
				*reinterpret_cast<uint16 *>(begin) = a;
				*reinterpret_cast<uint16 *>(end - 2) = b;

				break;
			}
			case 2:
			{
				*reinterpret_cast<uint16 *>(begin) = *reinterpret_cast<const uint16 *>(first);

				break;
			}
			case 1:
			{
				*reinterpret_cast<uint8 *>(begin) = *reinterpret_cast<const uint8 *>(first);

				break;
			}
			case 0: break;
			default: ful_unreachable();
			}
		}

		ful_inline
		void set_small(char8 * from, char8 * to, char8 u)
		{
			switch (to - from)
			{
#if defined(__AVX__)
			case 64:
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
			case 56:
			case 55:
			case 54:
			case 53:
			case 52:
			case 51:
			case 50:
			case 49:
			case 48:
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
			case 40:
			case 39:
			case 38:
			case 37:
			case 36:
			case 35:
			case 34:
			case 33:
			{
				detail::set8_avx_32_64(from, to, u);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::set8_sse2_16_32(from, to, u);
# else
				detail::set8_sse_16_32(from, to, u);
# endif

				break;
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)u;

				*reinterpret_cast<uint64 *>(from) = bytes;
				*reinterpret_cast<uint64 *>(to - 8) = bytes;

				break;
			}
			case 8:
			case 7:
			case 6:
			case 5:
			{
				const uint32 bytes = 0x01010101u * (uint8)u;

				*reinterpret_cast<uint32 *>(from) = bytes;
				*reinterpret_cast<uint32 *>(to - 4) = bytes;

				break;
			}
			case 4:
			case 3:
			{
				alignas(2) uint8 bytes[2] = {(uint8)u, (uint8)u};

				*reinterpret_cast<uint16 *>(from) = *reinterpret_cast<const uint16 *>(bytes);
				*reinterpret_cast<uint16 *>(to - 2) = *reinterpret_cast<const uint16 *>(bytes);

				break;
			}
			case 2: from[1] = u; ful_fallthrough;
			case 1: from[0] = u; ful_fallthrough;
			case 0: break;
			default: ful_unreachable();
			}
		}

		ful_inline
		void set_small(char16 * from, char16 * to, char16 u)
		{
			switch ((to - from) * sizeof(char16))
			{
#if defined(__AVX__)
			case 64:
			case 62:
			case 60:
			case 58:
			case 56:
			case 54:
			case 52:
			case 50:
			case 48:
			case 46:
			case 44:
			case 42:
			case 40:
			case 38:
			case 36:
			case 34:
			{
				detail::set16_avx_32_64(from, to, u);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 30:
			case 28:
			case 26:
			case 24:
			case 22:
			case 20:
			case 18:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::set16_sse2_16_32(from, to, u);
# else
				detail::set16_sse_16_32(from, to, u);
# endif

				break;
			}
#endif
			case 16:
			case 14:
			case 12:
			case 10:
			{
				const uint64 bytes = 0x0001000100010001u * (uint16)u;

				*reinterpret_cast<uint64 *>(from) = bytes;
				*reinterpret_cast<uint64 *>(to - 4) = bytes;

				break;
			}
			case 8:
			case 6:
			{
				const uint32 bytes = 0x00010001u * (uint16)u;

				*reinterpret_cast<uint32 *>(from) = bytes;
				*reinterpret_cast<uint32 *>(to - 2) = bytes;

				break;
			}
			case 4: from[1] = u; ful_fallthrough;
			case 2: from[0] = u; ful_fallthrough;
			case 0: break;
			default: ful_unreachable();
			}
		}

		ful_inline
		void set_small(char24 * from, char24 * to, char_fast24 u)
		{
			switch ((to - from) * sizeof(char24))
			{
#if defined(__AVX__)
			case 63:
			case 60:
			case 57:
			case 54:
			case 51:
			case 48:
			case 45:
			case 42:
			case 39:
			case 36:
			case 33:
			{
				detail::set24_avx_32_64(from, to, u);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 30:
			case 27:
			case 24:
			case 21:
			case 18:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::set24_sse2_16_32(from, to, u);
# else
				detail::set24_sse_16_32(from, to, u);
# endif

				break;
			}
#endif
			case 15:
			case 12:
			case 9:
			{
				// todo benchmark
				const uint64 lo_bytes = 0x0001000001000001u * static_cast<uint32>(u);
				const uint64 hi_bytes = (lo_bytes << 16) | (static_cast<uint32>(u) >> 8);
				// lo 0001000001000001
				// hi 0000010000010000

				*reinterpret_cast<uint64 *>(from) = lo_bytes;
				*reinterpret_cast<uint64 *>(reinterpret_cast<char8 *>(to) - 8) = hi_bytes;

				break;
			}
			case 6:
			{
				// todo benchmark
				const uint32 bytes = 0x01000001u * static_cast<uint32>(u);

				*reinterpret_cast<uint32 *>(from) = bytes;
				*reinterpret_cast<uint16 *>(reinterpret_cast<char8 *>(to) - 2) = static_cast<uint16>(static_cast<uint32>(u) >> 8);

				break;
			}
			case 3: from[0] = static_cast<char24>(u); ful_fallthrough;
			case 0: break;
			default: ful_unreachable();
			}
		}

		ful_inline
		void set_small(char32 * from, char32 * to, char32 u)
		{
			switch ((to - from) * sizeof(char32))
			{
#if defined(__AVX__)
			case 64:
			case 60:
			case 56:
			case 52:
			case 48:
			case 44:
			case 40:
			case 36:
			{
				detail::set32_avx_32_64(from, to, u);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 28:
			case 24:
			case 20:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::set32_sse2_16_32(from, to, u);
# else
				detail::set32_sse_16_32(from, to, u);
# endif

				break;
			}
#endif
			case 16:
			case 12:
			{
				const uint64 bytes = 0x0000000100000001u * (uint32)u;

				*reinterpret_cast<uint64 *>(from) = bytes;
				*reinterpret_cast<uint64 *>(to - 2) = bytes;

				break;
			}
			case 8: from[1] = u; ful_fallthrough;
			case 4: from[0] = u; ful_fallthrough;
			case 0: break;
			default: ful_unreachable();
			}
		}

		ful_inline
		void swap_small(byte * beg1, byte * end1, byte * beg2, byte * end2)
		{
			switch (end1 - beg1)
			{
#if defined(__AVX__)
			case 64:
			case 63:
			case 62:
			case 61:
			case 60:
			case 59:
			case 58:
			case 57:
			case 56:
			case 55:
			case 54:
			case 53:
			case 52:
			case 51:
			case 50:
			case 49:
			case 48:
			case 47:
			case 46:
			case 45:
			case 44:
			case 43:
			case 42:
			case 41:
			case 40:
			case 39:
			case 38:
			case 37:
			case 36:
			case 35:
			case 34:
			case 33:
			{
				detail::swap_avx_32_64(beg1, end1, beg2, end2);

				break;
			}
#endif
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
# if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				detail::swap_sse2_16_32(beg1, end1, beg2, end2);
# else
				detail::swap_sse_16_32(beg1, end1, beg2, end2);
# endif

				break;
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			{
				const uint64 a1 = *reinterpret_cast<const uint64 *>(beg1);
				const uint64 b1 = *reinterpret_cast<const uint64 *>(end1 - 8);
				const uint64 a2 = *reinterpret_cast<const uint64 *>(beg2);
				const uint64 b2 = *reinterpret_cast<const uint64 *>(end2 - 8);
				*reinterpret_cast<uint64 *>(beg2) = a1;
				*reinterpret_cast<uint64 *>(end2 - 8) = b1;
				*reinterpret_cast<uint64 *>(beg1) = a2;
				*reinterpret_cast<uint64 *>(end1 - 8) = b2;

				break;
			}
			case 8:
			case 7:
			case 6:
			case 5:
			{
				const uint32 a1 = *reinterpret_cast<const uint32 *>(beg1);
				const uint32 b1 = *reinterpret_cast<const uint32 *>(end1 - 4);
				const uint32 a2 = *reinterpret_cast<const uint32 *>(beg2);
				const uint32 b2 = *reinterpret_cast<const uint32 *>(end2 - 4);
				*reinterpret_cast<uint32 *>(beg2) = a1;
				*reinterpret_cast<uint32 *>(end2 - 4) = b1;
				*reinterpret_cast<uint32 *>(beg1) = a2;
				*reinterpret_cast<uint32 *>(end1 - 4) = b2;

				break;
			}
			case 4:
			case 3:
			{
				const uint16 a1 = *reinterpret_cast<const uint16 *>(beg1);
				const uint16 b1 = *reinterpret_cast<const uint16 *>(end1 - 2);
				const uint16 a2 = *reinterpret_cast<const uint16 *>(beg2);
				const uint16 b2 = *reinterpret_cast<const uint16 *>(end2 - 2);
				*reinterpret_cast<uint16 *>(beg2) = a1;
				*reinterpret_cast<uint16 *>(end2 - 2) = b1;
				*reinterpret_cast<uint16 *>(beg1) = a2;
				*reinterpret_cast<uint16 *>(end1 - 2) = b2;

				break;
			}
			case 2:
			{
				const uint16 a1 = *reinterpret_cast<const uint16 *>(beg1);
				const uint16 a2 = *reinterpret_cast<const uint16 *>(beg2);
				*reinterpret_cast<uint16 *>(beg2) = a1;
				*reinterpret_cast<uint16 *>(beg1) = a2;

				break;
			}
			case 1:
			{
				const uint8 a1 = *reinterpret_cast<const uint8 *>(beg1);
				const uint8 a2 = *reinterpret_cast<const uint8 *>(beg2);
				*reinterpret_cast<uint8 *>(beg2) = a1;
				*reinterpret_cast<uint8 *>(beg1) = a2;

				break;
			}
			case 0: break;
			default: ful_unreachable();
			}
		}
	}

	ful_inline
	byte * memcopy(const byte * first, const byte * last, byte * begin)
	{
		const usize size = last - first;
		if (!ful_expect(begin + size <= first || last <= begin))
			return begin;

#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::copy_small(first, last, begin, begin + size);

			return begin + size;
		}
		// note repmov yields the fastest copy up to about 200'000 bytes.
		// after that, stream copying vec-words are at least as fast. beyond
		// about 6'000'000 bytes, repmov is noticibly slower
		else if (size <= 5000000 && (reinterpret_cast<puint>(first) & (8 - 1)) == (reinterpret_cast<puint>(begin) & (8 - 1)))
		{
			const int alignment_offset = 8 - static_cast<int>(reinterpret_cast<puint>(begin) & (8 - 1));

			*reinterpret_cast<uint64 *>(begin) = *reinterpret_cast<const uint64 *>(first);

			detail::repmovf(reinterpret_cast<const uint64 *>(first + alignment_offset), (size - alignment_offset) / 8, reinterpret_cast<uint64 *>(begin + alignment_offset));

			byte * const end = begin + size;
			*reinterpret_cast<uint64 *>(end - 8) = *reinterpret_cast<const uint64 *>(last - 8);

			return end;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memcopy(first, last, begin);
#elif defined(__AVX__)
			return detail::memcopy_avx(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memcopy_sse2(first, last, begin);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memcopy_sse(first, last, begin);
#else
			return detail::memcopy_none(first, last, begin);
#endif
		}
	}

	template <typename T1, typename T2>
	ful_inline T2 * memcopy(const T1 * first, const T1 * last, T2 * begin)
	{
		if (!ful_expect((last - first) * sizeof(T1) % sizeof(T2) == 0))
			return begin;

		return reinterpret_cast<T2 *>(memcopy(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline
	byte * memmove(const byte * first, const byte * last, byte * begin)
	{
		const usize size = last - first;
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::copy_small(first, last, begin, begin + size);

			return begin + size;
		}
		else if (begin <= first || last <= begin)
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memmovef(first, last, begin);
#elif defined(__AVX__)
			return detail::memmovef_avx(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memmovef_sse2(first, last, begin);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memmovef_sse(first, last, begin);
#else
			return detail::memmovef_none(first, last, begin);
#endif
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memmover(first, last, begin + size);
#elif defined(__AVX__)
			return detail::memmover_avx(first, last, begin + size);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memmover_sse2(first, last, begin + size);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memmover_sse(first, last, begin + size);
#else
			return detail::memmover_none(first, last, begin + size);
#endif
		}
	}

	template <typename T1, typename T2>
	ful_inline T2 * memmove(const T1 * first, const T1 * last, T2 * begin)
	{
		if (!ful_expect((last - first) * sizeof(T1) % sizeof(T2) == 0))
			return begin;

		return reinterpret_cast<T2 *>(memmove(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline
	byte * memmovef(const byte * first, const byte * last, byte * begin)
	{
		if (!ful_expect(begin <= first || last <= begin))
			return begin;

		const usize size = last - first;
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::copy_small(first, last, begin, begin + size);

			return begin + size;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memmovef(first, last, begin);
#elif defined(__AVX__)
			return detail::memmovef_avx(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memmovef_sse2(first, last, begin);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memmovef_sse(first, last, begin);
#else
			return detail::memmovef_none(first, last, begin);
#endif
		}
	}

	template <typename T1, typename T2>
	ful_inline T2 * memmovef(const T1 * first, const T1 * last, T2 * begin)
	{
		if (!ful_expect((last - first) * sizeof(T1) % sizeof(T2) == 0))
			return begin;

		return reinterpret_cast<T2 *>(memmovef(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	ful_inline
	byte * memmover(const byte * first, const byte * last, byte * end)
	{
		if (!ful_expect(end <= first || last <= end))
			return end;

		const usize size = last - first;
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::copy_small(first, last, end - size, end);

			return end - size;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memmover(first, last, end);
#elif defined(__AVX__)
			return detail::memmover_avx(first, last, end);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memmover_sse2(first, last, end);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memmover_sse(first, last, end);
#else
			return detail::memmover_none(first, last, end);
#endif
		}
	}

	template <typename T1, typename T2>
	ful_inline T2 * memmover(const T1 * first, const T1 * last, T2 * begin)
	{
		if (!ful_expect((last - first) * sizeof(T1) % sizeof(T2) == 0))
			return begin;

		return reinterpret_cast<T2 *>(memmover(reinterpret_cast<const byte *>(first), reinterpret_cast<const byte *>(last), reinterpret_cast<byte *>(begin)));
	}

	// prevents implicit casts from inbuilt characters with single quotes
	// (e.g. 'a') to int/char32
	template <typename T>
	void memset(T * from, T * to, char u) = delete;
#if defined(_MSC_VER)
	template <typename T>
	void memset(T * from, T * to, wchar_t u) = delete;
#endif

	ful_inline
	void memset(char8 * from, char8 * to, char8 u)
	{
		const usize size = to - from;
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::set_small(from, to, u);

			return;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memset8(from, to, u);
#elif defined(__AVX__)
			return detail::memset8_avx(from, to, u);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memset8_sse2(from, to, u);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memset8_sse(from, to, u);
#else
			return detail::memset8_none(from, to, u);
#endif
		}
	}

	template <typename T>
	ful_inline void memset(T * from, T * to, char8 u)
	{
		if (!ful_expect((to - from) * sizeof(T) % sizeof(char8) == 0))
			return;

		memset(reinterpret_cast<char8 *>(from), reinterpret_cast<char8 *>(to), u);
	}

	ful_inline
	void memset(char16 * from, char16 * to, char16 u)
	{
		const usize size = (to - from) * sizeof(char16);
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::set_small(from, to, u);

			return;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memset16(from, to, u);
#elif defined(__AVX__)
			return detail::memset16_avx(from, to, u);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memset16_sse2(from, to, u);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memset16_sse(from, to, u);
#else
			return detail::memset16_none(from, to, u);
#endif
		}
	}

	template <typename T>
	ful_inline void memset(T * from, T * to, char16 u)
	{
		if (!ful_expect((to - from) * sizeof(T) % sizeof(char16) == 0))
			return;

		memset(reinterpret_cast<char16 *>(from), reinterpret_cast<char16 *>(to), u);
	}

	ful_inline
	void memset(char24 * from, char24 * to, char_fast24 u)
	{
		const usize size = (to - from) * sizeof(char24);
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::set_small(from, to, u);

			return;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memset24(from, to, u);
#elif defined(__AVX__)
			return detail::memset24_avx(from, to, u);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memset24_sse2(from, to, u);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memset24_sse(from, to, u);
#else
			return detail::memset24_none(from, to, u);
#endif
		}
	}

	template <typename T>
	ful_inline void memset(T * from, T * to, char_fast24 u)
	{
		if (!ful_expect((to - from) * sizeof(T) % sizeof(char24) == 0))
			return;

		memset(reinterpret_cast<char24 *>(from), reinterpret_cast<char24 *>(to), u);
	}

	ful_inline
	void memset(char32 * from, char32 * to, char32 u)
	{
		const usize size = (to - from) * sizeof(char32);
#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::set_small(from, to, u);

			return;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memset32(from, to, u);
#elif defined(__AVX__)
			return detail::memset32_avx(from, to, u);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memset32_sse2(from, to, u);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memset32_sse(from, to, u);
#else
			return detail::memset32_none(from, to, u);
#endif
		}
	}

	template <typename T>
	ful_inline void memset(T * from, T * to, char32 u)
	{
		if (!ful_expect((to - from) * sizeof(T) % sizeof(char32) == 0))
			return;

		memset(reinterpret_cast<char32 *>(from), reinterpret_cast<char32 *>(to), u);
	}

	ful_inline
	byte * memswap(byte * beg1, byte * end1, byte * beg2)
	{
		const usize size = end1 - beg1;
		if (!ful_expect(end1 <= beg2 || beg2 + size <= beg1))
			return beg2;

#if defined(__AVX__)
		if (size <= 64u)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			detail::swap_small(beg1, end1, beg2, beg2 + size);

			return beg2 + size;
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memswap(beg1, end1, beg2);
#elif defined(__AVX__)
			return detail::memswap_avx(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memswap_sse2(beg1, end1, beg2);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memswap_sse(beg1, end1, beg2);
#else
			return detail::memswap_none(beg1, end1, beg2);
#endif
		}
	}

	template <typename T1, typename T2>
	ful_inline T2 * memswap(T1 * beg1, T1 * end1, T2 * beg2)
	{
		if (!ful_expect((end1 - beg1) * sizeof(T1) % sizeof(T2) == 0))
			return beg2;

		return reinterpret_cast<T2 *>(memswap(reinterpret_cast<byte *>(beg1), reinterpret_cast<byte *>(end1), reinterpret_cast<byte *>(beg2)));
	}

	namespace detail
	{
		ful_inline
		bool equal_cstr_generic(const byte * beg1, const byte * end1, const byte * beg2)
		{
			const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_cstr_generic_8(const byte * beg1, usize size, const byte * beg2);

			return equal_cstr_generic_8(beg1, size, beg2);
		}

		ful_target("sse") ful_inline
		bool equal_cstr_sse(const byte * beg1, const byte * end1, const byte * beg2)
		{
			const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_cstr_sse_16(const byte * beg1, usize size, const byte * beg2);

			return equal_cstr_sse_16(beg1, size, beg2);
		}

		ful_target("sse2") ful_inline
		bool equal_cstr_sse2(const byte * beg1, const byte * end1, const byte * beg2)
		{
			const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_cstr_sse2_16(const byte * beg1, usize size, const byte * beg2);

			return equal_cstr_sse2_16(beg1, size, beg2);
		}

		ful_target("avx") ful_inline
		bool equal_cstr_avx(const byte * beg1, const byte * end1, const byte * beg2)
		{
			const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			if (size <= 32u)
			{
				ful::usize index = 0;

				if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

				ful::usize end_line = size - 16;
				if (index < end_line)
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
					const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}

				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const unsigned int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;

				return beg2[end_line + 16] == ful::byte{};
			}
			else
			{
				extern bool equal_cstr_avx_32(const byte * beg1, usize size, const byte * beg2);

				return equal_cstr_avx_32(beg1, size, beg2);
			}
		}

		ful_target("avx2") ful_inline
		bool equal_cstr_avx2(const byte * beg1, const byte * end1, const byte * beg2)
		{
			const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			if (size <= 32u)
			{
				ful::usize index = 0;

				if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

				ful::usize end_line = size - 16;
				if (index < end_line)
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
					const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}

				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const unsigned int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;

				return beg2[end_line + 16] == ful::byte{};
			}
			else
			{
				extern bool equal_cstr_avx2_32(const byte * beg1, usize size, const byte * beg2);

				return equal_cstr_avx2_32(beg1, size, beg2);
			}
		}

		ful_inline
		bool equal_range_generic(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
		{
			const ful::usize size = end1 - beg1;
			if (size != static_cast<ful::usize>(end2 - beg2))
				return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_range_generic_9(const byte * beg1, usize size, const byte * beg2);

			return equal_range_generic_9(beg1, size, beg2);
		}

		ful_target("sse") ful_inline
		bool equal_range_sse(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
		{
			const ful::usize size = end1 - beg1;
			if (size != static_cast<ful::usize>(end2 - beg2))
				return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_range_sse_17(const byte * beg1, usize size, const byte * beg2);

			return equal_range_sse_17(beg1, size, beg2);
		}

		ful_target("sse2") ful_inline
		bool equal_range_sse2(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
		{
			const ful::usize size = end1 - beg1;
			if (size != static_cast<ful::usize>(end2 - beg2))
				return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			extern bool equal_range_sse2_17(const byte * beg1, usize size, const byte * beg2);

			return equal_range_sse2_17(beg1, size, beg2);
		}

		ful_target("avx") ful_inline
		bool equal_range_avx(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
		{
			const ful::usize size = end1 - beg1;
			if (size != static_cast<ful::usize>(end2 - beg2))
				return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			if (size <= 32u)
			{
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + 0));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + 0));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}
				return true;
			}
			else
			{
				extern bool equal_range_avx_33(const byte * beg1, usize size, const byte * beg2);

				return equal_range_avx_33(beg1, size, beg2);
			}
		}

		ful_target("avx2") ful_inline
		bool equal_range_avx2(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
		{
			const ful::usize size = end1 - beg1;
			if (size != static_cast<ful::usize>(end2 - beg2))
				return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			if (!ful_expect(32u < size))
#else
			if (!ful_except(16u < size))
#endif
				return false;

			if (size <= 32u)
			{
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + 0));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + 0));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
				}
				return true;
			}
			else
			{
				extern bool equal_range_avx2_33(const byte * beg1, usize size, const byte * beg2);

				return equal_range_avx2_33(beg1, size, beg2);
			}
		}
	}

	ful_inline
	bool equal(const byte * beg1, const byte * end1, const byte * beg2)
	{
		const ful::usize size = end1 - beg1;
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			ful::usize index = 0;

			switch (size)
			{
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
				if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

				ful::usize end_line = size - 16;
				if (index < end_line)
				{
#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
					const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
#else
					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
					const __m128 line2 = _mm_load_ps(reinterpret_cast<const float *>(beg2 + index));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const unsigned int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;
#endif
				}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
				const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
				const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
				const unsigned int mask = _mm_movemask_epi8(cmpeq);
				if (mask != 0x0000ffff)
					return false;
#else
				const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_line));
				const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_line));
				const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
				const unsigned int mask = _mm_movemask_ps(cmpeq);
				if (mask != 0x0000000f)
					return false;
#endif

				return beg2[end_line + 16] == ful::byte{};
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
			{
				if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }

				ful::usize end_qword = size - 8;
				if (index < end_qword)
				{
					if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index))
						return false;
				}

				if (*reinterpret_cast<const ful::char64 *>(beg1 + end_qword) != *reinterpret_cast<const ful::char64 *>(beg2 + end_qword))
					return false;

				return beg2[end_qword + 8] == ful::byte{};
			}
			case 8:
			case 7:
			case 6:
			case 5:
			{
				if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
				if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }

				ful::usize end_qword = size - 4;
				if (index < end_qword)
				{
					if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index))
						return false;
				}

				if (*reinterpret_cast<const ful::char32 *>(beg1 + end_qword) != *reinterpret_cast<const ful::char32 *>(beg2 + end_qword))
					return false;

				return beg2[end_qword + 4] == ful::byte{};
			}
			case 4: if (beg1[index] != beg2[index]) return false; index++; ful_fallthrough;
			case 3: if (beg1[index] != beg2[index]) return false; index++; ful_fallthrough;
			case 2: if (beg1[index] != beg2[index]) return false; index++; ful_fallthrough;
			case 1: if (beg1[index] != beg2[index]) return false; index++; ful_fallthrough;
			case 0: return beg2[index] == ful::byte{};
			default: ful_unreachable();
			}
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::equal_cstr(beg1, end1, beg2);
#elif defined(__AVX2__)
			return detail::equal_cstr_avx2(beg1, end1, beg2);
#elif defined(__AVX__)
			return detail::equal_cstr_avx(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::equal_cstr_sse2(beg1, end1, beg2);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::equal_cstr_sse(beg1, end1, beg2);
#else
			return detail::equal_cstr_generic(beg1, end1, beg2);
#endif
		}
	}

	ful_inline
	bool equal(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2)
	{
		const ful::usize size = end1 - beg1;
		if (size != static_cast<ful::usize>(end2 - beg2))
			return false;

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (size <= 32u)
#else
		if (size <= 16u)
#endif
		{
			switch (size)
			{
#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			case 32:
			case 31:
			case 30:
			case 29:
			case 28:
			case 27:
			case 26:
			case 25:
			case 24:
			case 23:
			case 22:
			case 21:
			case 20:
			case 19:
			case 18:
			case 17:
			{
				{
#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + 0));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + 0));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
#else
					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + 0));
					const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + 0));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const unsigned int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;
#endif
				}
				{
#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;
#else
					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
					const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const unsigned int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;
#endif
				}
				return true;
			}
#endif
			case 16:
			case 15:
			case 14:
			case 13:
			case 12:
			case 11:
			case 10:
			case 9:
				if (*reinterpret_cast<const ful::char64 *>(beg1 + 0) != *reinterpret_cast<const ful::char64 *>(beg2 + 0))
					return false;
				if (*reinterpret_cast<const ful::char64 *>(end1 - 8) != *reinterpret_cast<const ful::char64 *>(end2 - 8))
					return false;
				return true;
			case 8:
			case 7:
			case 6:
			case 5:
				if (*reinterpret_cast<const ful::char32 *>(beg1 + 0) != *reinterpret_cast<const ful::char32 *>(beg2 + 0))
					return false;
				if (*reinterpret_cast<const ful::char32 *>(end1 - 4) != *reinterpret_cast<const ful::char32 *>(end2 - 4))
					return false;
				return true;
			case 4:
			case 3:
				if (*reinterpret_cast<const ful::char16 *>(beg1 + 0) != *reinterpret_cast<const ful::char16 *>(beg2 + 0))
					return false;
				if (*reinterpret_cast<const ful::char16 *>(end1 - 2) != *reinterpret_cast<const ful::char16 *>(end2 - 2))
					return false;
				return true;
			case 2: if (end1[-2] != end2[-2]) return false; ful_fallthrough;
			case 1: if (end1[-1] != end2[-1]) return false; ful_fallthrough;
			case 0: return true;
			default: ful_unreachable();
			}
		}
		else
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::equal_range(beg1, end1, beg2, end2);
#elif defined(__AVX2__)
			return detail::equal_range_avx2(beg1, end1, beg2, end2);
#elif defined(__AVX__)
			return detail::equal_range_avx(beg1, end1, beg2, end2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::equal_range_sse2(beg1, end1, beg2, end2);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::equal_range_sse(beg1, end1, beg2, end2);
#else
			return detail::equal_range_generic(beg1, end1, beg2, end2);
#endif
		}
	}

	template <typename T>
	ful_inline ful_pure bool equal(const T * beg1, const T * end1, const T * beg2)
	{
		return equal(reinterpret_cast<const byte *>(beg1), reinterpret_cast<const byte *>(end1), reinterpret_cast<const byte *>(beg2));
	}

	template <typename T>
	ful_inline ful_pure bool equal(const T * beg1, const T * end1, const T * beg2, const T * end2)
	{
		return equal(reinterpret_cast<const byte *>(beg1), reinterpret_cast<const byte *>(end1), reinterpret_cast<const byte *>(beg2), reinterpret_cast<const byte *>(end2));
	}
}
