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

#define ful_align_next(ptr, n) reinterpret_cast<decltype(ptr)>(reinterpret_cast<puint>((ptr) + (n)) & -(n))
#define ful_align_next_8(ptr) ful_align_next(ptr, 8)
#define ful_align_next_16(ptr) ful_align_next(ptr, 16)
#define ful_align_next_32(ptr) ful_align_next(ptr, 32)

namespace ful
{
	// generate mask where the n least significant bits are set
	inline unsigned int low_mask(unsigned int n)
	{
		ful_expect(0 < n);
		ful_expect(n <= sizeof(unsigned int) * byte_size);

		return static_cast<unsigned int>(-1) >> (sizeof(unsigned int) * byte_size - n);
	}

	// get the index of the least significant set bit
	template <typename T>
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
	unsigned int most_significant_set_bit(T x)
	{
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		return detail::bsr(x);
#else
# error Missing implementation!
#endif
	}

	// get the index of the nth set bit
	inline unsigned int index_set_bit(unsigned int x, unsigned int n)
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
	auto count_leading_zeros(T x)
#if defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::lzcnt(x))
#endif
	{
#if defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::lzcnt(x);
#else
# error Missing implementation!
#endif
	}

	// get the number of trailing zero bits
	template <typename T>
	auto count_trailing_zeros(T x)
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::tzcnt(x))
#elif defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(detail::lzcnt(x))
#endif
	{
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		return detail::tzcnt(x);
#elif defined(__LZCNT__) || (defined(_MSC_VER) && defined(__AVX2__))
		// Hacker's Delight, 2nd ed, p 107
		return static_cast<decltype(detail::lzcnt(x))>(sizeof(T) * byte_size) - detail::lzcnt(~x & (x - 1));
#else
# error Missing implementation!
#endif
	}

	// if `0 < n < Po2`, where Po2 is a power of two, computes `Po2 - n`
	template <unsigned int Po2>
	unsigned int mirror(unsigned int n)
	{
		ful_expect(0 < n);
		ful_expect(n < Po2);

		return -n & (Po2 - 1);
	}

	// set all bits more significant than the specified index
	inline unsigned int set_higher_bits(unsigned int x, unsigned int index)
	{
		ful_expect(index < sizeof(unsigned int) * byte_size);

		const unsigned int mask = static_cast<unsigned int>(-2) << index;
		return x | mask;
	}

	// reset all bits more significant than the specified index
	inline unsigned int zero_higher_bits(unsigned int x, unsigned int index)
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
	inline unsigned int zero_low_bits(unsigned int x, unsigned int n)
	{
		ful_expect(n < sizeof(unsigned int) * byte_size);

		x >>= n;
		x <<= n;

		return x;
	}

	// resets all but the n least significant set bits
	inline unsigned int keep_low_set_bits(unsigned int x, unsigned int n)
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
