#pragma once

#include "ful/compiler.hpp"
#include "ful/dispatch.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"

#include "string_none.hpp"
#include "string_sse.hpp"
#include "string_sse2.hpp"
#include "string_sse4_2.hpp"
#include "string_avx.hpp"
#include "string_avx2.hpp"

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern char8 * ful_dispatch(memcopy)(const char8 * first, const char8 * last, char8 * begin);
		extern char8 * ful_dispatch(memypoc)(const char8 * first, const char8 * last, char8 * end);
		extern char8 * ful_dispatch(memswap)(char8 * beg1, char8 * end1, char8 * beg2);
		extern void ful_dispatch(memset8)(char8 * from, char8 * to, char8 u);
		extern bool ful_dispatch(equal_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
		extern bool ful_dispatch(less_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
#endif
	}

	ful_inline
	char8 * memcopy(const char8 * first, const char8 * last, char8 * begin)
	{
		if (!ful_expect(begin <= first || last <= begin))
			return begin;

		const usize size = last - first;
		switch (size)
		{
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
			*reinterpret_cast<uint64 *>(begin + size - 8) = b;

			return begin + size;
		}
		case 8:
		case 7:
		case 6:
		case 5:
		{
			const uint32 a = *reinterpret_cast<const uint32 *>(first);
			const uint32 b = *reinterpret_cast<const uint32 *>(last - 4);
			*reinterpret_cast<uint32 *>(begin) = a;
			*reinterpret_cast<uint32 *>(begin + size - 4) = b;

			return begin + size;
		}
		case 4:
		case 3:
		{
			const uint16 a = *reinterpret_cast<const uint16 *>(first);
			const uint16 b = *reinterpret_cast<const uint16 *>(last - 2);
			*reinterpret_cast<uint16 *>(begin) = a;
			*reinterpret_cast<uint16 *>(begin + size - 2) = b;

			return begin + size;
		}
		case 2:
		{
			*reinterpret_cast<uint16 *>(begin) = *reinterpret_cast<const uint16 *>(first);

			return begin + size;
		}
		case 1:
		{
			*reinterpret_cast<uint8 *>(begin) = *reinterpret_cast<const uint8 *>(first);

			return begin + size;
		}
		case 0: return begin + size;
		default:
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

	ful_inline
	char8 * memypoc(const char8 * first, const char8 * last, char8 * end)
	{
		if (!ful_expect(end <= first || last <= end))
			return end;

		const usize size = last - first;
		switch (size)
		{
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
			*reinterpret_cast<uint64 *>(end - size) = a;
			*reinterpret_cast<uint64 *>(end - 8) = b;

			return end - size;
		}
		case 8:
		case 7:
		case 6:
		case 5:
		{
			const uint32 a = *reinterpret_cast<const uint32 *>(first);
			const uint32 b = *reinterpret_cast<const uint32 *>(last - 4);
			*reinterpret_cast<uint32 *>(end - size) = a;
			*reinterpret_cast<uint32 *>(end - 4) = b;

			return end - size;
		}
		case 4:
		case 3:
		{
			const uint16 a = *reinterpret_cast<const uint16 *>(first);
			const uint16 b = *reinterpret_cast<const uint16 *>(last - 2);
			*reinterpret_cast<uint16 *>(end - size) = a;
			*reinterpret_cast<uint16 *>(end - 2) = b;

			return end - size;
		}
		case 2:
		{
			*reinterpret_cast<uint16 *>(end - 2) = *reinterpret_cast<const uint16 *>(first);

			return end - size;
		}
		case 1:
		{
			*reinterpret_cast<uint8 *>(end - 2) = *reinterpret_cast<const uint8 *>(first);

			return end - size;
		}
		case 0: return end - size;
		default:
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memypoc(first, last, end);
#elif defined(__AVX__)
			return detail::memypoc_avx(first, last, end);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memypoc_sse2(first, last, end);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memypoc_sse(first, last, end);
#else
			return detail::memypoc_none(first, last, end);
#endif
		}
	}

	ful_inline
	char8 * memswap(char8 * beg1, char8 * end1, char8 * beg2)
	{
		const usize size = end1 - beg1;
		if (!ful_expect(end1 <= beg2 || beg2 + size <= beg1))
			return beg2;

		switch (size)
		{
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
			const uint64 b2 = *reinterpret_cast<const uint64 *>(beg2 + size - 8);
			*reinterpret_cast<uint64 *>(beg2) = a1;
			*reinterpret_cast<uint64 *>(beg2 + size - 8) = b1;
			*reinterpret_cast<uint64 *>(beg1) = a2;
			*reinterpret_cast<uint64 *>(end1 - 8) = b2;

			return beg2 + size;
		}
		case 8:
		case 7:
		case 6:
		case 5:
		{
			const uint32 a1 = *reinterpret_cast<const uint32 *>(beg1);
			const uint32 b1 = *reinterpret_cast<const uint32 *>(end1 - 4);
			const uint32 a2 = *reinterpret_cast<const uint32 *>(beg2);
			const uint32 b2 = *reinterpret_cast<const uint32 *>(beg2 + size - 4);
			*reinterpret_cast<uint32 *>(beg2) = a1;
			*reinterpret_cast<uint32 *>(beg2 + size - 4) = b1;
			*reinterpret_cast<uint32 *>(beg1) = a2;
			*reinterpret_cast<uint32 *>(end1 - 4) = b2;

			return beg2 + size;
		}
		case 4:
		case 3:
		{
			const uint16 a1 = *reinterpret_cast<const uint16 *>(beg1);
			const uint16 b1 = *reinterpret_cast<const uint16 *>(end1 - 2);
			const uint16 a2 = *reinterpret_cast<const uint16 *>(beg2);
			const uint16 b2 = *reinterpret_cast<const uint16 *>(beg2 + size - 2);
			*reinterpret_cast<uint16 *>(beg2) = a1;
			*reinterpret_cast<uint16 *>(beg2 + size - 2) = b1;
			*reinterpret_cast<uint16 *>(beg1) = a2;
			*reinterpret_cast<uint16 *>(end1 - 2) = b2;

			return beg2 + size;
		}
		case 2:
		{
			const uint16 a1 = *reinterpret_cast<const uint16 *>(beg1);
			const uint16 a2 = *reinterpret_cast<const uint16 *>(beg2);
			*reinterpret_cast<uint16 *>(beg2) = a1;
			*reinterpret_cast<uint16 *>(beg1) = a2;

			return beg2 + size;
		}
		case 1:
		{
			const uint8 a1 = *reinterpret_cast<const uint8 *>(beg1);
			const uint8 a2 = *reinterpret_cast<const uint8 *>(beg2);
			*reinterpret_cast<uint8 *>(beg2) = a1;
			*reinterpret_cast<uint8 *>(beg1) = a2;

			return beg2 + size;
		}
		case 0: return beg2 + size;
		default:
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::memswap(beg1, end1, beg2);
#elif defined(__AVX__)
			return detail::memswap_avx(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			return detail::memswap_sse2(first, last, end);
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			return detail::memswap_sse(first, last, end);
#else
			return detail::memswap_none(beg1, end1, beg2);
#endif
		}
	}

	ful_inline
	void memset(char8 * from, char8 * to, char8 u)
	{
		const usize size = to - from;
		switch (size)
		{
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

			return;
		}
		case 8:
		case 7:
		case 6:
		case 5:
		{
			const uint32 bytes = 0x01010101u * (uint8)u;

			*reinterpret_cast<uint32 *>(from) = bytes;
			*reinterpret_cast<uint32 *>(to - 4) = bytes;

			return;
		}
		case 4:
		case 3:
		{
			alignas(2) uint8 bytes[2] = {(uint8)u, (uint8)u};

			*reinterpret_cast<uint16 *>(from) = *reinterpret_cast<const uint16 *>(bytes);
			*reinterpret_cast<uint16 *>(to - 2) = *reinterpret_cast<const uint16 *>(bytes);

			return;
		}
		case 2: from[1] = u; ful_fallthrough;
		case 1: from[0] = u; ful_fallthrough;
		case 0: return;
		default:
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

	ful_inline bool equal(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::equal_cstr(beg1, end1, beg2);
#elif defined(__AVX2__)
		return detail::equal_cstr_avx2(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::equal_cstr_sse2(beg1, end1, beg2);
#else
		return detail::equal_cstr_none(beg1, end1, beg2);
#endif
	}

	ful_inline bool less(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::less_cstr(beg1, end1, beg2);
#elif defined(__AVX2__)
		return detail::less_cstr_avx2(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::less_cstr_sse2(beg1, end1, beg2);
#else
		return detail::less_cstr_none(beg1, end1, beg2);
#endif
	}

}
