#pragma once

#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#if defined(_MSC_VER)
# include <intrin.h>
#endif

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
# include <immintrin.h>
#endif

namespace ful
{
#if defined(__LZCNT__)

	// number of leading zeros
	inline unsigned int nlz(unsigned int x)
	{
		return _lzcnt_u32(x);
	}

	// number of leading zeros
	inline unsigned int nlz(int x) { return nlz(static_cast<unsigned int>(x)); }

# if defined(_MSC_VER) || !defined(__LP64__)
	// number of leading zeros
	inline unsigned int nlz(unsigned long x) { return nlz(static_cast<unsigned int>(x)); }

	// number of leading zeros
	inline unsigned int nlz(long x) { return nlz(static_cast<unsigned long>(x)); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// number of leading zeros
	inline unsigned int nlz(unsigned long long x)
	{
		return static_cast<unsigned int>(_lzcnt_u64(x)); // at most 64
	}

	// number of leading zeros
	inline unsigned int nlz(long long x) { return nlz(static_cast<unsigned long long>(x)); }

#  if defined(__LP64__)
	// number of leading zeros
	inline unsigned int nlz(unsigned long x) { return nlz(static_cast<unsigned long long>(x)); }

	// number of leading zeros
	inline unsigned int nlz(long x) { return nlz(static_cast<unsigned long>(x)); }
#  endif

# endif

#endif

#if defined(__BMI__)

	// number of trailing zeros
	inline unsigned int ntz(unsigned int x)
	{
		return _tzcnt_u32(x);
	}

	// number of trailing zeros
	inline unsigned int ntz(int x) { return ntz(static_cast<unsigned int>(x)); }

# if defined(_MSC_VER) || !defined(__LP64__)
	// number of trailing zeros
	inline unsigned int ntz(unsigned long x) { return ntz(static_cast<unsigned int>(x)); }

	// number of trailing zeros
	inline unsigned int ntz(long x) { return ntz(static_cast<unsigned long>(x)); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// number of trailing zeros
	inline unsigned int ntz(unsigned long long x)
	{
		return static_cast<unsigned int>(_tzcnt_u64(x)); // at most 64
	}

	// number of trailing zeros
	inline unsigned int ntz(long long x) { return ntz(static_cast<unsigned long long>(x)); }

#  if defined(__LP64__)
	// number of trailing zeros
	inline unsigned int ntz(unsigned long x) { return ntz(static_cast<unsigned long long>(x)); }

	// number of trailing zeros
	inline unsigned int ntz(long x) { return ntz(static_cast<unsigned long>(x)); }
#  endif

# endif

#endif

#if defined(_MSC_VER)

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(unsigned long x)
	{
		ful_assume(x != 0);

		unsigned long i;
		_BitScanForward(&i, x);

		return i;
	}

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(long x) { return lssb(static_cast<unsigned long>(x)); }

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(unsigned int x) { return lssb(static_cast<unsigned long>(x)); }

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(int x) { return lssb(static_cast<unsigned int>(x)); }

# if defined(_M_X64) || defined(_M_AMD64)
	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(unsigned long long x)
	{
		ful_assume(x != 0);

		unsigned long i;
		_BitScanForward64(&i, x);

		return i;
	}

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int lssb(long long x) { return lssb(static_cast<unsigned long long>(x)); }
# endif

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(unsigned long x)
	{
		ful_assume(x != 0);

		unsigned long i;
		_BitScanReverse(&i, x);

		return i;
	}

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(long x) { return mssb(static_cast<unsigned long>(x)); }

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(unsigned int x) { return mssb(static_cast<unsigned long>(x)); }

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(int x) { return mssb(static_cast<unsigned int>(x)); }

# if defined(_M_X64) || defined(_M_AMD64)
	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(unsigned long long x)
	{
		ful_assume(x != 0);

		unsigned long i;
		_BitScanReverse64(&i, x);

		return i;
	}

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	inline unsigned int mssb(long long x) { return mssb(static_cast<unsigned long long>(x)); }
# endif

# if !defined(__LZCNT__)
	// number of leading zeros
	template <typename T>
	auto nlz(T x) -> decltype(mssb(x))
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : mssb(x) ^ (sizeof x * BYTE_SIZE - 1);
	}
# endif

# if !defined(__BMI__)
	// number of trailing zeros
	template <typename T>
	auto ntz(T x) -> decltype(lssb(x))
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : lssb(x);
	}
#endif

# if defined(_M_X64) || defined(_M_AMD64) || (defined(_MSC_VER) && defined(_M_IX86))
	// number of set bits
	inline unsigned int popcnt(unsigned int x) { return __popcnt(x); }

	// number of set bits
	inline unsigned int popcnt(int x) { return popcnt(static_cast<unsigned int>(x)); }

	// number of set bits
	inline unsigned int popcnt(unsigned long x) { return popcnt(static_cast<unsigned int>(x)); }

	// number of set bits
	inline unsigned int popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }
# endif

# if defined(_M_X64) || defined(_M_AMD64)
	// number of set bits
	inline unsigned int popcnt(unsigned long long x) { return static_cast<int>(__popcnt64(x)); } // at most 64

	// number of set bits
	inline unsigned int popcnt(long long x) { return popcnt(static_cast<unsigned long long>(x)); }
# endif

#elif defined(__GNUC__)

// note gcc seems to be generating (possibly) unnecessary xor-
// instructions, here is an interesting read that mostly talks about
// that with popcnt
//
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=62011
//
// todo clang does not, why?

# if !defined(__LZCNT__)

	// number of leading zeros
	inline unsigned int nlz(unsigned int x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_clz(x);
	}

	// number of leading zeros
	inline unsigned int nlz(int x) { return nlz(static_cast<unsigned int>(x)); }

	// number of leading zeros
	inline unsigned int nlz(unsigned long x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_clzl(x);
	}

	// number of leading zeros
	inline unsigned int nlz(long x) { return nlz(static_cast<unsigned long>(x)); }

#  if defined(__x86_64__)
	// number of leading zeros
	inline unsigned int nlz(unsigned long long x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_clzll(x);
	}

	// number of leading zeros
	inline unsigned int nlz(long long x) { return nlz(static_cast<unsigned long long>(x)); }
#  endif

# endif

# if !defined(__BMI__)

	// number of trailing zeros
	inline unsigned int ntz(unsigned int x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_ctz(x);
	}

	// number of trailing zeros
	inline unsigned int ntz(int x) { return ntz(static_cast<unsigned int>(x)); }

	// number of trailing zeros
	inline unsigned int ntz(unsigned long x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_ctzl(x);
	}

	// number of trailing zeros
	inline unsigned int ntz(long x) { return ntz(static_cast<unsigned long>(x)); }

#  if defined(__x86_64__)
	// number of trailing zeros
	inline unsigned int ntz(unsigned long long x)
	{
		return x == 0 ? (sizeof x * BYTE_SIZE) : __builtin_ctzll(x);
	}

	// number of trailing zeros
	inline unsigned int ntz(long long x) { return ntz(static_cast<unsigned long long>(x)); }
#  endif

# endif

	/* least significant set bit
	 *
	 * `x = 0` is undefined
	 */
	template <typename T>
	auto lssb(T x) -> decltype(ntz(x))
	{
		ful_assume(x != 0);

		return ntz(x);
	}

	/* most significant set bit
	 *
	 * `x = 0` is undefined
	 */
	template <typename T>
	auto mssb(T x) -> decltype(nlz(x))
	{
		ful_assume(x != 0);

		return nlz(x) ^ (sizeof x * BYTE_SIZE - 1);
	}

# if defined(__POPCNT__)

	// number of set bits
	inline unsigned int popcnt(unsigned int x) { return __builtin_popcount(x); }

	// number of set bits
	inline unsigned int popcnt(int x) { return popcnt(static_cast<unsigned int>(x)); }

	// number of set bits
	inline unsigned int popcnt(unsigned long x) { return __builtin_popcountl(x); }

	// number of set bits
	inline unsigned int popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }

#  if defined(__x86_64__)
	// number of set bits
	inline unsigned int popcnt(unsigned long long x) { return __builtin_popcountll(x); }

	// number of set bits
	inline unsigned int popcnt(long long x) { return popcnt(static_cast<unsigned long long>(x)); }
#  endif

# endif

#endif

#if defined(__BMI2__)

	// deposit low bits
	inline unsigned int pdep(unsigned int x, unsigned int mask)
	{
		return _pdep_u32(x, mask);
	}

	// deposit low bits
	inline int pdep(int x, unsigned int mask) { return pdep(static_cast<unsigned int>(x), mask); }

# if defined(_MSC_VER) || !defined(__LP64__)
	// deposit low bits
	inline unsigned int pdep(unsigned long x, unsigned int mask) { return pdep(static_cast<unsigned int>(x), mask); }

	// deposit low bits
	inline long pdep(long x, unsigned int mask) { return pdep(static_cast<unsigned long>(x), mask); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// deposit low bits
	inline unsigned long long pdep(unsigned long long x, unsigned long long mask)
	{
		return _pdep_u64(x, mask);
	}

	// deposit low bits
	inline long long pdep(long long x, unsigned long long mask) { return pdep(static_cast<unsigned long long>(x), mask); }

#  if defined(__LP64__)
	// deposit low bits
	inline unsigned int pdep(unsigned long x, unsigned long long mask) { return pdep(static_cast<unsigned long long>(x), mask); }

	// deposit low bits
	inline long pdep(long x, unsigned long long mask) { return pdep(static_cast<unsigned long>(x), mask); }
#  endif

# endif

	// zero more significant bits
	inline unsigned int zmsb(unsigned int x, unsigned int index)
	{
		return _bzhi_u32(x, index + 1);
	}

	// zero more significant bits
	inline int zmsb(int x, unsigned int index) { return zmsb(static_cast<unsigned int>(x), index); }

# if defined(_MSC_VER) || !defined(__LP64__)
	// zero more significant bits
	inline unsigned int zmsb(unsigned long x, unsigned int index) { return zmsb(static_cast<unsigned int>(x), index); }

	// zero more significant bits
	inline long zmsb(long x, unsigned int index) { return zmsb(static_cast<unsigned long>(x), index); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
	// zero more significant bits
	inline unsigned long long zmsb(unsigned long long x, unsigned int index)
	{
		return _bzhi_u64(x, index + 1);
	}

	// zero more significant bits
	inline long long zmsb(long long x, unsigned int index) { return zmsb(static_cast<unsigned long long>(x), index); }

#  if defined(__LP64__)
	// zero more significant bits
	inline unsigned int zmsb(unsigned long x, unsigned int index) { return zmsb(static_cast<unsigned int>(x), index); }

	// zero more significant bits
	inline long zmsb(long x, unsigned int index) { return zmsb(static_cast<unsigned long>(x), index); }
#  endif

# endif

#endif
}
