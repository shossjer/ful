#pragma once

#define FUL_INCLUDED_STRING_HPP

#include "ful/compiler.hpp"
#include "ful/dispatch.hpp"
#include "ful/stdint.hpp"

#include "ful/ranges.hpp"

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
		template <unsigned long long>
		struct to_chars_impl
		{};

		template <>
		struct to_chars_impl<sizeof(char8)>
		{
			template <typename T>
			const char8 * operator () (const T * x) { return reinterpret_cast<const char8 *>(x); }
			template <typename T>
			char8 * operator () (T * x) { return reinterpret_cast<char8 *>(x); }
		};

		template <>
		struct to_chars_impl<sizeof(char16)>
		{
			template <typename T>
			const char16 * operator () (const T * x) { return reinterpret_cast<const char16 *>(x); }
			template <typename T>
			char16 * operator () (T * x) { return reinterpret_cast<char16 *>(x); }
		};

		template <>
		struct to_chars_impl<sizeof(char24)>
		{
			template <typename T>
			const char24 * operator () (const T * x) { return reinterpret_cast<const char24 *>(x); }
			template <typename T>
			char24 * operator () (T * x) { return reinterpret_cast<char24 *>(x); }
		};

		template <>
		struct to_chars_impl<sizeof(char32)>
		{
			template <typename T>
			const char32 * operator () (const T * x) { return reinterpret_cast<const char32 *>(x); }
			template <typename T>
			char32 * operator () (T * x) { return reinterpret_cast<char32 *>(x); }
		};
	}

	template <typename T>
	ful_inline auto to_chars(T * x)
		-> decltype(detail::to_chars_impl<sizeof(T)>{}(x))
	{
		return detail::to_chars_impl<sizeof(T)>{}(x);
	}

	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern bool ful_dispatch(equal_cstr)(const byte * beg1, const byte * end1, const byte * beg2);
		extern bool ful_dispatch(equal_range)(const byte * beg1, const byte * end1, const byte * beg2, const byte * end2);
#endif

		// prevents implicit casts from inbuilt characters with single quotes
		// (e.g. 'a') to int/char32
		template <typename T>
		void memset(T * from, T * to, char u) = delete;
#if defined(_MSC_VER)
		template <typename T>
		void memset(T * from, T * to, wchar_t u) = delete;
#endif

		ful_inline byte * memcopy(const byte * first, const byte * last, byte * begin);
		ful_inline byte * memmove(const byte * first, const byte * last, byte * begin);
		ful_inline byte * memmovef(const byte * first, const byte * last, byte * begin);
		ful_inline byte * memmover(const byte * first, const byte * last, byte * end);
		ful_inline void memset(char8 * from, char8 * to, char8 c);
		ful_inline void memset(char8 * from, char8 * to, char16 c);
		ful_inline void memset(char8 * from, char8 * to, char_fast24 c);
		ful_inline void memset(char8 * from, char8 * to, char32 c);
		ful_inline void memset(char16 * from, char16 * to, char16 c);
		ful_inline void memset(char16 * from, char16 * to, char32 c);
		ful_inline void memset(char24 * from, char24 * to, char_fast24 c);
		ful_inline void memset(char32 * from, char32 * to, char32 c);
		ful_inline byte * memswap(byte * beg1, byte * end1, byte * beg2);
	}

	template <typename T>
	const byte * to_bytes(const T * x) { return reinterpret_cast<const byte *>(x); }
	template <typename T>
	byte * to_bytes(T * x) { return reinterpret_cast<byte *>(x); }

	template <typename First, typename Last, typename Begin>
	ful_inline auto memcopy(First first, Last last, Begin begin)
		-> decltype(detail::memcopy(to_bytes(to_address(first)), to_bytes(to_address(last)), to_bytes(to_address(begin))), to_address(begin))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);
		const auto begin_ptr = to_address(begin);

		if (!ful_expect((last_ptr - first_ptr) * sizeof(*first_ptr) % sizeof(*begin_ptr) == 0))
			return begin_ptr;

		return reinterpret_cast<decltype(begin_ptr)>(detail::memcopy(to_bytes(first_ptr), to_bytes(last_ptr), to_bytes(begin_ptr)));
	}

	template <typename First, typename Last, typename Begin>
	ful_inline auto memmove(First first, Last last, Begin begin)
		-> decltype(to_address(first), to_address(last), to_address(begin))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);
		const auto begin_ptr = to_address(begin);

		if (!ful_expect((last_ptr - first_ptr) * sizeof(*first_ptr) % sizeof(*begin_ptr) == 0))
			return begin_ptr;

		return reinterpret_cast<decltype(begin_ptr)>(detail::memmove(reinterpret_cast<const byte *>(first_ptr), reinterpret_cast<const byte *>(last_ptr), reinterpret_cast<byte *>(begin_ptr)));
	}

	template <typename First, typename Last, typename Begin>
	ful_inline auto memmovef(First first, Last last, Begin begin)
		-> decltype(to_address(first), to_address(last), to_address(begin))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);
		const auto begin_ptr = to_address(begin);

		if (!ful_expect((last_ptr - first_ptr) * sizeof(*first_ptr) % sizeof(*begin_ptr) == 0))
			return begin_ptr;

		return reinterpret_cast<decltype(begin_ptr)>(detail::memmovef(reinterpret_cast<const byte *>(first_ptr), reinterpret_cast<const byte *>(last_ptr), reinterpret_cast<byte *>(begin_ptr)));
	}

	template <typename First, typename Last, typename Begin>
	ful_inline auto memmover(First first, Last last, Begin begin)
		-> decltype(to_address(first), to_address(last), to_address(begin))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);
		const auto begin_ptr = to_address(begin);

		if (!ful_expect((last_ptr - first_ptr) * sizeof(*first_ptr) % sizeof(*begin_ptr) == 0))
			return begin_ptr;

		return reinterpret_cast<decltype(begin_ptr)>(detail::memmover(reinterpret_cast<const byte *>(first_ptr), reinterpret_cast<const byte *>(last_ptr), reinterpret_cast<byte *>(begin_ptr)));
	}

	template <typename From, typename To, typename Char>
	ful_inline auto memset(From from, To to, Char c)
		-> decltype(detail::memset(to_chars(to_address(from)), to_chars(to_address(to)), c))
	{
		const auto from_ptr = to_address(from);
		const auto to_ptr = to_address(to);

		detail::memset(to_chars(from_ptr), to_chars(to_ptr), c);
	}

	template <typename R, typename Char>
	ful_inline auto memset(R && r, Char c)
		-> decltype(memset(begin(r), end(r), c))
	{
		memset(begin(r), end(r), c);
	}

	template <typename Begin1, typename End1, typename Begin2>
	ful_inline auto memswap(Begin1 beg1, End1 end1, Begin2 beg2)
		-> decltype(to_address(beg1), to_address(end1), to_address(beg2))
	{
		const auto beg1_ptr = to_address(beg1);
		const auto end1_ptr = to_address(end1);
		const auto beg2_ptr = to_address(beg2);

		if (!ful_expect((end1_ptr - beg1_ptr) * sizeof(*beg1_ptr) % sizeof(*beg2_ptr) == 0))
			return beg2_ptr;

		return reinterpret_cast<decltype(beg2_ptr)>(detail::memswap(reinterpret_cast<byte *>(beg1_ptr), reinterpret_cast<byte *>(end1_ptr), reinterpret_cast<byte *>(beg2_ptr)));
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
			return detail::equal_cstr_rtd(beg1, end1, beg2);
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
			return detail::equal_range_rtd(beg1, end1, beg2, end2);
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

	namespace detail
	{
		ful_inline const char8 * find(const char8 * begin, const char8 * end, char8 c);
		ful_inline const char8 * find(const char8 * begin, const char8 * end, char16 c);
		ful_inline const char8 * find(const char8 * begin, const char8 * end, char_fast24 c);
		ful_inline const char8 * find(const char8 * begin, const char8 * end, char32 c);
		ful_inline const char16 * find(const char16 * begin, const char16 * end, char16 c);
		ful_inline const char16 * find(const char16 * begin, const char16 * end, char32 c);
		ful_inline const char32 * find(const char32 * begin, const char32 * end, char32 c);
	}

	template <typename Begin, typename End, typename Char>
	ful_inline auto find(Begin begin, End end, Char c)
		-> decltype(detail::find(to_chars(to_address(begin)), to_chars(to_address(end)), c), to_address(begin))
	{
		return (decltype(to_address(begin)))detail::find(to_chars(to_address(begin)), to_chars(to_address(end)), c);
	}

	template <typename R, typename Char>
	ful_inline auto find(R && x, Char c)
		-> decltype(find(begin(x), end(x), c))
	{
		return find(begin(x), end(x), c);
	}
}
