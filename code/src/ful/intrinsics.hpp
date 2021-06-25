#pragma once

// #include "ful/compiler.hpp"
// #include "ful/stdint.hpp"

#if defined(_MSC_VER)
# include <intrin.h>
#endif

#include "intrinsics_x86.hpp"

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4146 )
// warning C4146: unary minus operator applied to unsigned type, result still unsigned
#endif

#define ful_align_next(ptr, n) reinterpret_cast<decltype(ptr)>(reinterpret_cast<ful::puint>((ptr) + (n)) & -(n))
#define ful_align_next_8(ptr) ful_align_next(ptr, 8)
#define ful_align_next_16(ptr) ful_align_next(ptr, 16)
#define ful_align_next_32(ptr) ful_align_next(ptr, 32)

namespace ful
{
	namespace detail
	{
		ful_inline
		unsigned int set_lower_bits(unsigned int x)
		{
			x |= x >> 1;
			x |= x >> 2;
			x |= x >> 4;
			x |= x >> 8;
			x |= x >> 16;
			return x;
		}

		ful_inline int set_lower_bits(int x) { return set_lower_bits(static_cast<unsigned int>(x)); }

		ful_inline
		unsigned int count_leading_zeros(unsigned int x)
		{
			// Hacker's Delight, 2nd ed, p 100
			unsigned int y;
			int n = 32;
			y = x >> 16; if (y != 0) { n -= 16; x = y; }
			y = x >>  8; if (y != 0) { n -=  8; x = y; }
			y = x >>  4; if (y != 0) { n -=  4; x = y; }
			y = x >>  2; if (y != 0) { n -=  2; x = y; }
			y = x >>  1; if (y != 0) return n - 2;
			return n - x;
		}

		ful_inline int count_leading_zeros(int x) { return count_leading_zeros(static_cast<unsigned int>(x)); }

		ful_inline
		unsigned int count_trailing_zeros(unsigned int x)
		{
			// Hacker's Delight, 2nd ed, p 110
			const unsigned int y = x & -x;
			const unsigned int bz = y ? 0 : 1;
			const unsigned int b4 = (y & 0x0000ffff) ? 0 : 16;
			const unsigned int b3 = (y & 0x00ff00ff) ? 0 :  8;
			const unsigned int b2 = (y & 0x0f0f0f0f) ? 0 :  4;
			const unsigned int b1 = (y & 0x33333333) ? 0 :  2;
			const unsigned int b0 = (y & 0x55555555) ? 0 :  1;
			return bz + b4 + b3 + b2 + b1 + b0;
		}

		ful_inline int count_trailing_zeros(int x) { return count_trailing_zeros(static_cast<unsigned int>(x)); }

#if defined(_MSC_VER) || !defined(__LP64__)
		ful_inline unsigned long set_lower_bits(unsigned long x) { return set_lower_bits(static_cast<unsigned int>(x)); }

		ful_inline long set_lower_bits(long x) { return set_lower_bits(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_leading_zeros(unsigned long x) { return count_leading_zeros(static_cast<unsigned int>(x)); }

		ful_inline long count_leading_zeros(long x) { return count_leading_zeros(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_trailing_zeros(unsigned long x) { return count_trailing_zeros(static_cast<unsigned int>(x)); }

		ful_inline long count_trailing_zeros(long x) { return count_trailing_zeros(static_cast<unsigned long>(x)); }
#endif

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		ful_inline
		unsigned long long set_lower_bits(unsigned long long x)
		{
			x |= x >> 1;
			x |= x >> 2;
			x |= x >> 4;
			x |= x >> 8;
			x |= x >> 16;
			x |= x >> 32;
			return x;
		}

		ful_inline long long set_lower_bits(long long x) { return set_lower_bits(static_cast<unsigned long long>(x)); }

		ful_inline
		unsigned long long count_leading_zeros(unsigned long long x)
		{
			// Hacker's Delight, 2nd ed, p 100
			unsigned long long y;
			int n = 64;
			y = x >> 32; if (y != 0) { n -= 32; x = y; }
			y = x >> 16; if (y != 0) { n -= 16; x = y; }
			y = x >>  8; if (y != 0) { n -=  8; x = y; }
			y = x >>  4; if (y != 0) { n -=  4; x = y; }
			y = x >>  2; if (y != 0) { n -=  2; x = y; }
			y = x >>  1; if (y != 0) return n - 2;
			return n - x;
		}

		ful_inline long long count_leading_zeros(long long x) { return count_leading_zeros(static_cast<unsigned long long>(x)); }

		ful_inline
		unsigned long long count_trailing_zeros(unsigned long long x)
		{
			// Hacker's Delight, 2nd ed, p 110
			const unsigned long long y = x & -x;
			const unsigned long long bz = y ? 0 : 1;
			const unsigned long long b5 = (y & 0x00000000ffffffff) ? 0 : 32;
			const unsigned long long b4 = (y & 0x0000ffff0000ffff) ? 0 : 16;
			const unsigned long long b3 = (y & 0x00ff00ff00ff00ff) ? 0 :  8;
			const unsigned long long b2 = (y & 0x0f0f0f0f0f0f0f0f) ? 0 :  4;
			const unsigned long long b1 = (y & 0x3333333333333333) ? 0 :  2;
			const unsigned long long b0 = (y & 0x5555555555555555) ? 0 :  1;
			return bz + b5 + b4 + b3 + b2 + b1 + b0;
		}

		ful_inline long long count_trailing_zeros(long long x) { return count_trailing_zeros(static_cast<unsigned long long>(x)); }
#endif

#if defined(__LP64__)
		ful_inline unsigned long set_lower_bits(unsigned long x) { return set_lower_bits(static_cast<unsigned long long>(x)); }

		ful_inline long set_lower_bits(long x) { return set_lower_bits(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_leading_zeros(unsigned long x) { return count_leading_zeros(static_cast<unsigned long long>(x)); }

		ful_inline long count_leading_zeros(long x) { return count_leading_zeros(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_trailing_zeros(unsigned long x) { return count_trailing_zeros(static_cast<unsigned long long>(x)); }

		ful_inline long count_trailing_zeros(long x) { return count_trailing_zeros(static_cast<unsigned long>(x)); }
#endif
	}

	// generate mask where the n least significant bits are set
	ful_inline
	unsigned int low_mask(unsigned int n)
	{
		ful_expect(0 < n);
		ful_expect(n <= sizeof(unsigned int) * byte_size);

		return static_cast<unsigned int>(-1) >> (sizeof(unsigned int) * byte_size - n);
	}

	// get the index of the least significant set bit
	template <typename T>
	ful_inline
	unsigned int least_significant_set_bit(T x)
	{
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		return detail::bsf(x);
#else
# error Missing implementation!
#endif
	}

	// get the index of the most significant set bit
	template <typename T>
	ful_inline
	unsigned int most_significant_set_bit(T x)
	{
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		return detail::bsr(x);
#else
# error Missing implementation!
#endif
	}

	// get the index of the nth set bit
	ful_inline
	unsigned int index_set_bit(unsigned int x, unsigned int n)
	{
#if defined(__BMI2__) || (defined(_MSC_VER) && defined(__AVX2__))
		const unsigned int nth_mask = detail::pdep(1u << n, x);
		return detail::bsf(nth_mask); // todo bsf or tzcnt
#elif defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		for (; n != 0; --n) x &= x - 1;
		return detail::bsf(x);
#else
# error Missing implementation!
#endif
	}

	// get the number of leading zero bits
	template <typename T>
#if defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
	ful_target("lzcnt")
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
	ful_target("popcnt")
#endif
	ful_inline
	auto count_leading_zeros(T x)
#if defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::lzcnt(x))
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		-> decltype(detail::popcnt(x))
#else
		-> decltype(detail::count_leading_zeros(x))
#endif
	{
#if defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::lzcnt(x);
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		// Hacker's Delight, 2nd ed, p 102
		return detail::popcnt(~set_lower_bits(x));
#else
		return detail::count_leading_zeros(x);
#endif
	}

	// get the number of trailing zero bits
	template <typename T>
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
	ful_target("bmi")
#elif defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
	ful_target("lzcnt")
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
	ful_target("popcnt")
#endif
	ful_inline
	auto count_trailing_zeros(T x)
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::tzcnt(x))
#elif defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::lzcnt(x))
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		-> decltype(detail::popcnt(x))
#else
		-> decltype(detail::count_trailing_zeros(x))
#endif
	{
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::tzcnt(x);
#elif defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		// Hacker's Delight, 2nd ed, p 107
		return static_cast<decltype(detail::lzcnt(x))>(sizeof(T) * byte_size) - detail::lzcnt(~x & (x - 1));
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		// Hacker's Delight, 2nd ed, p 107
		return detail::popcnt(~x & (x - 1));
#else
		return detail::count_trailing_zeros(x);
#endif
	}

	// if `0 < n < Po2`, where Po2 is a power of two, computes `Po2 - n`
	template <unsigned int Po2>
	ful_inline
	unsigned int mirror(unsigned int n)
	{
		ful_expect(0 < n);
		ful_expect(n < Po2);

		return -n & (Po2 - 1);
	}

	// set all bits more significant than the specified index
	ful_inline
	unsigned int set_higher_bits(unsigned int x, unsigned int index)
	{
		ful_expect(index < sizeof(unsigned int) * byte_size);

		const unsigned int mask = static_cast<unsigned int>(-2) << index;
		return x | mask;
	}

	// reset all bits more significant than the specified index
#if defined(__BMI2__) || (defined(_MSC_VER) && defined(__AVX2__))
	ful_target("bmi2")
#endif
	ful_inline
	unsigned int zero_higher_bits(unsigned int x, unsigned int index)
	{
		ful_expect(index < sizeof(unsigned int) * byte_size);

#if defined(__BMI2__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::bzhi(x, index + 1);
#else
		const unsigned int mask = static_cast<unsigned int>(-1) >> (index ^ (sizeof (unsigned int) * byte_size - 1));
		return x & mask;
#endif
	}

	// resets the n least significant bits
	ful_inline
	unsigned int zero_low_bits(unsigned int x, unsigned int n)
	{
		ful_expect(n < sizeof(unsigned int) * byte_size);

		x >>= n;
		x <<= n;

		return x;
	}

	// resets all but the n least significant set bits
#if defined(__BMI2__) || (defined(_MSC_VER) && defined(__AVX2__))
	ful_target("bmi2")
#endif
	ful_inline
	unsigned int keep_low_set_bits(unsigned int x, unsigned int n)
	{
#if defined(__BMI2__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::pdep(low_mask(n), x);
#else
		unsigned int y = x;
		for (; n != 0; --n) y &= y - 1;
		return x & ~y;
#endif
	}

	// reorder bytes for converting between little- and big-endian
	template <typename T>
	ful_inline
	T byte_swap(T x)
	{
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		return detail::bswap(x);
#else
# error Missing implementation!
#endif
	}
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif
