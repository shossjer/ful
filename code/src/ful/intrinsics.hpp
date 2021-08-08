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

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
# define ful_add_overflow(ptr, n, out) (out = (ptr) + (n), false)
#elif defined(__GNUC__)
# define ful_add_overflow(ptr, n, out) __builtin_add_overflow(reinterpret_cast<puint>(ptr), (n), reinterpret_cast<puint *>(&out))
#else
# define ful_add_overflow(ptr, n, out) _addcarry_u32(0, reinterpret_cast<puint>(ptr), (n), reinterpret_cast<puint *>(&out))
#endif

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

#if defined(_MSC_VER) || !defined(__LP64__)
		ful_inline unsigned long set_lower_bits(unsigned long x) { return set_lower_bits(static_cast<unsigned int>(x)); }

		ful_inline long set_lower_bits(long x) { return set_lower_bits(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_leading_zeros(unsigned long x) { return count_leading_zeros(static_cast<unsigned int>(x)); }

		ful_inline long count_leading_zeros(long x) { return count_leading_zeros(static_cast<unsigned long>(x)); }
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
#endif

#if defined(__LP64__)
		ful_inline unsigned long set_lower_bits(unsigned long x) { return set_lower_bits(static_cast<unsigned long long>(x)); }

		ful_inline long set_lower_bits(long x) { return set_lower_bits(static_cast<unsigned long>(x)); }

		ful_inline unsigned long count_leading_zeros(unsigned long x) { return count_leading_zeros(static_cast<unsigned long long>(x)); }

		ful_inline long count_leading_zeros(long x) { return count_leading_zeros(static_cast<unsigned long>(x)); }
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
		return detail::popcnt(~set_lower_bits(x)); // todo benchmark
#else
		return x ? static_cast<decltype(detail::bsf(x))>(sizeof(T) * byte_size) - detail::bsr(x) : 0;
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

	struct tag_bmi_type { explicit tag_bmi_type() = default; };
	constexpr auto tag_bmi = tag_bmi_type{};
	struct tag_lzcnt_type { explicit tag_lzcnt_type() = default; };
	constexpr auto tag_lzcnt = tag_lzcnt_type{};
	struct tag_movbe_type { explicit tag_movbe_type() = default; };
	constexpr auto tag_movbe = tag_movbe_type{};
	struct tag_popcnt_type { explicit tag_popcnt_type() = default; };
	constexpr auto tag_popcnt = tag_popcnt_type{};

	// get the number of trailing zero bits
	template <typename T>
	ful_target("bmi") ful_inline
	auto count_trailing_zero_bits(T x, tag_bmi_type) -> decltype(detail::tzcnt(x)) { return detail::tzcnt(x); }

	// get the number of trailing zero bits
	template <typename T>
	ful_target("popcnt") ful_inline
	auto count_trailing_zero_bits(T x, tag_popcnt_type) -> decltype(detail::popcnt(x))
	{
		// todo benchmark
		// Hacker's Delight, 2nd ed, p 107
		return detail::popcnt(~x & (x - 1));
	}

	// get the number of trailing zero bits
	template <typename T>
	ful_generic() ful_inline
	auto count_trailing_zero_bits(T x)
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		-> decltype(count_trailing_zero_bits(x, tag_bmi))
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		-> decltype(count_trailing_zero_bits(x, tag_popcnt))
#else
		-> decltype(detail::bsf(x))
#endif
	{
#if defined(__BMI__) || (defined(_MSC_VER) && defined(__AVX2__))
		return count_trailing_zero_bits(x, tag_bmi);
#elif defined(__POPCNT__) || (defined(_MSC_VER) && defined(__AVX__))
		return count_trailing_zero_bits(x, tag_popcnt);
#else
		return x ? detail::bsf(x) : static_cast<decltype(detail::bsf(x))>(sizeof(T) * byte_size);
#endif
	}

	// get the index of the least significant set bit
	template <typename T>
	ful_generic() ful_inline
	unsigned int least_significant_set_bit(T x)
	{
		return detail::bsf(x);
	}

	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	unsigned int least_significant_zero_byte(unsigned int x, tag_bmi_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned int y = detail::andn(x, x - 0x01010101u) & 0x80808080u;
		return count_trailing_zero_bits(y, tag_bmi) >> 3;
	}

	// get the index of the least significant zero byte
	ful_target("popcnt") ful_inline
	unsigned int least_significant_zero_byte(unsigned int x, tag_popcnt_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned int y = (x - 0x01010101u) & ~x & 0x80808080u;
		return count_trailing_zero_bits(y, tag_popcnt) >> 3;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	unsigned int least_significant_zero_byte(unsigned int x)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned int y = (x - 0x01010101u) & ~x & 0x80808080u;
		return count_trailing_zero_bits(y) >> 3;
	}

	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	bool least_significant_zero_byte(unsigned int x, unsigned int & out, tag_bmi_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned int y = detail::andn(x, x - 0x01010101u) & 0x80808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_byte(unsigned int x, unsigned int & out)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned int y = (x - 0x01010101u) & ~x & 0x80808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}

#if defined(_MSC_VER) || !defined(__LP64__)
	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	unsigned long least_significant_zero_byte(unsigned long x, tag_bmi_type) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned int>(x), tag_bmi)); }

	// get the index of the least significant zero byte
	ful_target("popcnt") ful_inline
	unsigned long least_significant_zero_byte(unsigned long x, tag_popcnt_type) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned int>(x), tag_popcnt)); }

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	unsigned long least_significant_zero_byte(unsigned long x) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned int>(x))); }

	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	bool least_significant_zero_byte(unsigned long x, unsigned long & out, tag_bmi_type)
	{
		unsigned int outout;
		const auto ret = least_significant_zero_byte(static_cast<unsigned int>(x), outout, tag_bmi);
		out = static_cast<unsigned long>(outout);
		return ret;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_byte(unsigned long x, unsigned long & out)
	{
		unsigned int outout;
		const auto ret = least_significant_zero_byte(static_cast<unsigned int>(x), outout);
		out = static_cast<unsigned long>(outout);
		return ret;
	}
#endif

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	unsigned long long least_significant_zero_byte(unsigned long long x, tag_bmi_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned long long y = detail::andn(x, x - 0x0101010101010101u) & 0x8080808080808080u;
		return count_trailing_zero_bits(y, tag_bmi) >> 3;
	}

	// get the index of the least significant zero byte
	ful_target("popcnt") ful_inline
	unsigned long long least_significant_zero_byte(unsigned long long x, tag_popcnt_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned long long y = (x - 0x0101010101010101u) & ~x & 0x8080808080808080u;
		return count_trailing_zero_bits(y, tag_popcnt) >> 3;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	unsigned long long least_significant_zero_byte(unsigned long long x)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned long long y = (x - 0x0101010101010101u) & ~x & 0x8080808080808080u;
		return count_trailing_zero_bits(y) >> 3;
	}

	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	bool least_significant_zero_byte(unsigned long long x, unsigned long long & out, tag_bmi_type)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned long long y = detail::andn(x, x - 0x0101010101010101u) & 0x8080808080808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_byte(unsigned long long x, unsigned long long & out)
	{
		// Hacker's Delight, 2nd ed, p 118
		unsigned long long y = (x - 0x0101010101010101u) & ~x & 0x8080808080808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}
#endif

#if defined(__LP64__)
	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	unsigned long least_significant_zero_byte(unsigned long x, tag_bmi_type) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned long long>(x), tag_bmi)); }

	// get the index of the least significant zero byte
	ful_target("popcnt") ful_inline
	unsigned long least_significant_zero_byte(unsigned long x, tag_popcnt_type) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned long long>(x), tag_popcnt)); }

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	unsigned long least_significant_zero_byte(unsigned long x) { return static_cast<unsigned long>(least_significant_zero_byte(static_cast<unsigned long long>(x))); }

	// get the index of the least significant zero byte
	ful_target("bmi") ful_inline
	bool least_significant_zero_byte(unsigned long x, unsigned long & out, tag_bmi_type)
	{
		unsigned long long outout;
		const auto ret = least_significant_zero_byte(static_cast<unsigned long long>(x), outout, tag_bmi);
		out = static_cast<unsigned long long>(outout);
		return ret;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_byte(unsigned long x, unsigned long & out)
	{
		unsigned long long outout;
		const auto ret = least_significant_zero_byte(static_cast<unsigned long long>(x), outout);
		out = static_cast<unsigned long long>(outout);
		return ret;
	}
#endif

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long long x0, unsigned long long x8, unsigned long long & out)
	{
		// Hacker's Delight, 2nd ed, p 118
		// note modified to detect 2 overlapping 8 bits
		unsigned long long y = ((x0 - 0x0101010101010101u) & ~x0) & ((x8 - 0x0101010101010101u) & ~x8) & 0x8080808080808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long long x0, unsigned long long x8, unsigned long long x16, unsigned long long & out)
	{
		// Hacker's Delight, 2nd ed, p 118
		// note modified to detect 3 overlapping 8 bits
		unsigned long long y = ((x0 - 0x0101010101010101u) & ~x0) & ((x8 - 0x0101010101010101u) & ~x8) & ((x16 - 0x0101010101010101u) & ~x16) & 0x8080808080808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long long x0, unsigned long long x8, unsigned long long x16, unsigned long long x24, unsigned long long & out)
	{
		// Hacker's Delight, 2nd ed, p 118
		// note modified to detect 4 overlapping 8 bits
		unsigned long long y = ((x0 - 0x0101010101010101u) & ~x0) & ((x8 - 0x0101010101010101u) & ~x8) & ((x16 - 0x0101010101010101u) & ~x16) & ((x24 - 0x0101010101010101u) & ~x24) & 0x8080808080808080u;
		if (y == 0)
			return false;

		out = least_significant_set_bit(y) >> 3;
		return true;
	}
#endif

#if defined(__LP64__)
	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long x0, unsigned long x8, unsigned long & out)
	{
		unsigned long long outout;
		const auto ret = least_significant_zero_word(static_cast<unsigned long long>(x0), static_cast<unsigned long long>(x8), outout);
		out = static_cast<unsigned long>(outout);
		return ret;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long x0, unsigned long x8, unsigned long x16, unsigned long & out)
	{
		unsigned long long outout;
		const auto ret = least_significant_zero_word(static_cast<unsigned long long>(x0), static_cast<unsigned long long>(x8), static_cast<unsigned long long>(x16), outout);
		out = static_cast<unsigned long>(outout);
		return ret;
	}

	// get the index of the least significant zero byte
	ful_generic() ful_inline
	bool least_significant_zero_word(unsigned long x0, unsigned long x8, unsigned long x16, unsigned long x24, unsigned long & out)
	{
		unsigned long long outout;
		const auto ret = least_significant_zero_word(static_cast<unsigned long long>(x0), static_cast<unsigned long long>(x8), static_cast<unsigned long long>(x16), static_cast<unsigned long long>(x24), outout);
		out = static_cast<unsigned long>(outout);
		return ret;
	}
#endif

	// reverse the byte order
	template <typename T>
	ful_generic() ful_inline
	T byte_swap(T x)
	{
#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
		return detail::bswap(x);
#else
# error Missing implementation!
#endif
	}

	// load and reverse the byte order
	template <typename T>
	ful_target("movbe") ful_inline
	auto load_swap(const T * x, tag_movbe_type) -> decltype(detail::loadbe(x)) { return detail::loadbe(x); }

	// load and reverse the byte order
	template <typename T>
	ful_generic() ful_inline
	T load_swap(const T * x)
	{
#if defined(__MOVBE__) || (defined(_MSC_VER) && defined(__AVX2__))
		return load_swap(x, tag_movbe);
#else
		return detail::bswap(*x);
#endif
	}

	// load and reverse the byte order
	template <typename T>
	ful_target("movbe") ful_inline
	void store_swap(T * x, T y, tag_movbe_type) { return detail::storebe(x, y); }

	// load and reverse the byte order
	template <typename T>
	ful_generic() ful_inline
	void store_swap(T * x, T y)
	{
#if defined(__MOVBE__) || (defined(_MSC_VER) && defined(__AVX2__))
		return store_swap(x, y, tag_movbe);
#else
		*x = detail::bswap(y);
#endif
	}
}

#if defined(_MSC_VER)
#pragma warning( pop )
#endif
