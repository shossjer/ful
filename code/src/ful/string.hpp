#pragma once

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"
//#include "ful/unicode.hpp"
#include "ful/unsafe_ptr.hpp"

#include "string_none.hpp"
#include "string_avx.hpp"
#include "string_avx2.hpp"
#include "string_sse2.hpp"
#include "string_sse4_2.hpp"

#include <memory>

namespace ful
{
	namespace detail
	{
#if defined(FUL_IFUNC)
		extern unit_utf8 * copy_large(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin);
		extern unit_utf8 * rcopy_large(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end);
		extern bool equal_cstr(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
		extern void fill_large(unit_utf8 * from, unit_utf8 * to, unit_utf8 u);
		extern bool less_cstr(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
#elif defined(FUL_FPTR)
		extern unit_utf8 * (* copy_large)(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin);
		extern unit_utf8 * (* rcopy_large)(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end);
		extern bool (* equal_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
		extern void (* fill_large)(unit_utf8 * from, unit_utf8 * to, unit_utf8 u);
		extern bool (* less_cstr)(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2);
#endif
	}

	inline unit_utf8 * copy(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		if (!ful_expect(begin <= first || last <= begin))
			return begin;

		const usize size = last - first;
		switch (size)
		{
		case 15:
		case 14:
		case 13:
		case 12:
		case 11:
		case 10:
		case 9:
		case 8:
		{
			const uint64 first_dword = *reinterpret_cast<const uint64 *>(first);
			const uint64 last_dword = *reinterpret_cast<const uint64 *>(last - 8);
			*reinterpret_cast<uint64 *>(begin) = first_dword;
			*reinterpret_cast<uint64 *>(begin + size - 8) = last_dword;

			return begin + size;
		}
		case 7:
		case 6:
		case 5:
		case 4:
		{
			const uint32 first_dword = *reinterpret_cast<const uint32 *>(first);
			const uint32 last_dword = *reinterpret_cast<const uint32 *>(last - 4);
			*reinterpret_cast<uint32 *>(begin) = first_dword;
			*reinterpret_cast<uint32 *>(begin + size - 4) = last_dword;

			return begin + size;
		}
		case 3:
		case 2:
		{
			const uint16 first_dword = *reinterpret_cast<const uint16 *>(first);
			const uint16 last_dword = *reinterpret_cast<const uint16 *>(last - 2);
			*reinterpret_cast<uint16 *>(begin) = first_dword;
			*reinterpret_cast<uint16 *>(begin + size - 2) = last_dword;

			return begin + size;
		}
		case 1: begin[0] = first[0]; return begin + size;
		case 0: return begin + size;
		default:
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::copy_large(first, last, begin);
#elif defined(__AVX__)
			return detail::copy_large_avx(first, last, begin);
#else
			return detail::copy_large_none(first, last, begin);
#endif
		}
		}
	}

	inline unit_utf8 * rcopy(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
	{
		if (!ful_expect(end <= first || last <= end))
			return end;

		const usize size = last - first;
		switch (size)
		{
		case 15:
		case 14:
		case 13:
		case 12:
		case 11:
		case 10:
		case 9:
		case 8:
		{
			const uint64 first_dword = *reinterpret_cast<const uint64 *>(first);
			const uint64 last_dword = *reinterpret_cast<const uint64 *>(last - 8);
			*reinterpret_cast<uint64 *>(end - size) = first_dword;
			*reinterpret_cast<uint64 *>(end - 8) = last_dword;

			return end - size;
		}
		case 7:
		case 6:
		case 5:
		case 4:
		{
			const uint32 first_dword = *reinterpret_cast<const uint32 *>(first);
			const uint32 last_dword = *reinterpret_cast<const uint32 *>(last - 4);
			*reinterpret_cast<uint32 *>(end - size) = first_dword;
			*reinterpret_cast<uint32 *>(end - 4) = last_dword;

			return end - size;
		}
		case 3:
		case 2:
		{
			const uint16 first_dword = *reinterpret_cast<const uint16 *>(first);
			const uint16 last_dword = *reinterpret_cast<const uint16 *>(last - 2);
			*reinterpret_cast<uint16 *>(end - size) = first_dword;
			*reinterpret_cast<uint16 *>(end - 2) = last_dword;

			return end - size;
		}
		case 1: end[-1] = first[0]; return end - size;
		case 0: return end - size;
		default:
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::rcopy_large(first, last, end);
#elif defined(__AVX__)
			return detail::rcopy_large_avx(first, last, end);
#else
			return detail::rcopy_large_none(first, last, end);
#endif
		}
		}
	}

	inline bool equal(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::equal_cstr(beg1, end1, beg2);
#elif defined(__AVX2__)
		return detail::equal_cstr_avx2(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64)))
		return detail::equal_cstr_sse2(beg1, end1, beg2);
#else
		return detail::equal_cstr_none(beg1, end1, beg2);
#endif
	}

	inline bool less(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::less_cstr(beg1, end1, beg2);
#elif defined(__AVX2__)
		return detail::less_cstr_avx2(beg1, end1, beg2);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64)))
		return detail::less_cstr_sse2(beg1, end1, beg2);
#else
		return detail::less_cstr_none(beg1, end1, beg2);
#endif
	}

	inline void fill(unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
	{
		const usize size = to - from;
		switch (size)
		{
		case 15:
		case 14:
		case 13:
		case 12:
		case 11:
		case 10:
		case 9:
		case 8:
		{
			// todo
			alignas(8) uint8 bytes[] = {(uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u};

			*reinterpret_cast<uint64 *>(from) = *reinterpret_cast<const uint64 *>(bytes);
			*reinterpret_cast<uint64 *>(from + size - 8) = *reinterpret_cast<const uint64 *>(bytes);

			return;
		}
		case 7:
		case 6:
		case 5:
		case 4:
		{
			// todo
			alignas(4) uint8 bytes[] = {(uint8)u, (uint8)u, (uint8)u, (uint8)u};

			*reinterpret_cast<uint32 *>(from) = *reinterpret_cast<const uint32 *>(bytes);
			*reinterpret_cast<uint32 *>(from + size - 4) = *reinterpret_cast<const uint32 *>(bytes);

			return;
		}
		case 3:
		case 2:
		{
			// todo
			alignas(2) uint8 bytes[] = {(uint8)u, (uint8)u};

			*reinterpret_cast<uint16 *>(from) = *reinterpret_cast<const uint16 *>(bytes);
			*reinterpret_cast<uint16 *>(from + size - 2) = *reinterpret_cast<const uint16 *>(bytes);

			return;
		}
		case 1: from[0] = u; return;
		case 0: return;
		default:
		{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
			return detail::fill_large(from, to, u);
#elif defined(__AVX__)
			return detail::fill_large_avx(from, to, u);
#else
			return detail::fill_large_none(from, to, u);
#endif
		}
		}
	}
}
