#pragma once

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)

#if defined(__GNUC__)
# include <x86intrin.h>
#endif

// note gcc seems to be generating (possibly) unnecessary xor-
// instructions, here is an interesting read that mostly talks about
// that with popcnt
//
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=62011

namespace ful
{
	namespace detail
	{
#if defined(__GNUC__)
		// bit scan forward
		inline unsigned int bsf(int x) { ful_expect(x != 0); return __bsfd(x); }

		// bit scan forward
		inline unsigned int bsf(unsigned int x) { return bsf(static_cast<int>(x)); }

		// bit scan reverse
		inline unsigned int bsr(int x) { ful_expect(x != 0); return __bsrd(x); }

		// bit scan reverse
		inline unsigned int bsr(unsigned int x) { return bsr(static_cast<int>(x)); }

		// byte swap
		inline int bswap(int x) { return __bswapd(x); }

		// byte swap
		inline unsigned int bswap(unsigned int x) { return bswap(static_cast<int>(x)); }

		// rotate left
		inline unsigned int rotl(unsigned int x, int shift) { return __rold(x, shift); }

		// rotate left
		inline int rotl(int x, int shift) { return rotl(static_cast<unsigned int>(x), shift); }

		// rotate left
		inline unsigned long rotl(unsigned long x, int shift) { return _lrotl(x, shift); }

		// rotate left
		inline long rotl(long x, int shift) { return rotl(static_cast<unsigned long>(x), shift); }

		// rotate right
		inline unsigned int rotr(unsigned int x, int shift) { return __rord(x, shift); }

		// rotate right
		inline int rotr(int x, int shift) { return rotr(static_cast<unsigned int>(x), shift); }

		// rotate right
		inline unsigned long rotr(unsigned long x, int shift) { return _lrotr(x, shift); }

		// rotate right
		inline long rotr(long x, int shift) { return rotr(static_cast<unsigned long>(x), shift); }

#if defined(__x86_64__)
		// bit scan forward
		inline unsigned int bsf(long long x) { ful_expect(x != 0); return __bsfq(x); }

		// bit scan forward
		inline unsigned int bsf(unsigned long long x) { return bsf(static_cast<long long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(long long x) { ful_expect(x != 0); return __bsrq(x); }

		// bit scan reverse
		inline unsigned int bsr(unsigned long long x) { return bsr(static_cast<long long>(x)); }

		// byte swap
		inline long long bswap(long long x) { return __bswapq(x); }

		// byte swap
		inline unsigned long long bswap(unsigned long long x) { return bswap(static_cast<long long>(x)); }

		// rotate left
		inline unsigned long long rotl(unsigned long long x, int shift) { return __rolq(x, shift); }

		// rotate left
		inline long long rotl(long long x, int shift) { return rotl(static_cast<unsigned long long>(x), shift); }

		// rotate right
		inline unsigned long long rotr(unsigned long long x, int shift) { return __rorq(x, shift); }

		// rotate right
		inline long long rotr(long long x, int shift) { return rotr(static_cast<unsigned long long>(x), shift); }
#endif

# if defined(__LP64__)
		// bit scan forward
		inline unsigned int bsf(long x) { return bsf(static_cast<long long>(x)); }

		// bit scan forward
		inline unsigned int bsf(unsigned long x) { return bsf(static_cast<long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(long x) { return bsr(static_cast<long long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(unsigned long x) { return bsr(static_cast<long>(x)); }

		// byte swap
		inline long bswap(long x) { return bswap(static_cast<long long>(x)); }

		// byte swap
		inline unsigned long bswap(unsigned long x) { return bswap(static_cast<long>(x)); }
# else
		// bit scan forward
		inline unsigned int bsf(long x) { return bsf(static_cast<int>(x)); }

		// bit scan forward
		inline unsigned int bsf(unsigned long x) { return bsf(static_cast<long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(long x) { return bsr(static_cast<int>(x)); }

		// bit scan reverse
		inline unsigned int bsr(unsigned long x) { return bsr(static_cast<long>(x)); }

		// byte swap
		inline long bswap(long x) { return bswap(static_cast<int>(x)); }

		// byte swap
		inline unsigned long bswap(unsigned long x) { return bswap(static_cast<long>(x)); }
# endif
#elif defined(_MSC_VER)
		// bit scan forward
		inline unsigned int bsf(unsigned long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanForward(&i, x);

			return i;
		}

		// bit scan forward
		inline unsigned int bsf(long x) { return bsf(static_cast<unsigned long>(x)); }

		// bit scan forward
		inline unsigned int bsf(unsigned int x) { return bsf(static_cast<unsigned long>(x)); }

		// bit scan forward
		inline unsigned int bsf(int x) { return bsf(static_cast<unsigned int>(x)); }

		// bit scan reverse
		inline unsigned int bsr(unsigned long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanReverse(&i, x);

			return i;
		}

		// bit scan reverse
		inline unsigned int bsr(long x) { return bsr(static_cast<unsigned long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(unsigned int x) { return bsr(static_cast<unsigned long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(int x) { return bsr(static_cast<unsigned int>(x)); }

		// byte swap
		inline unsigned long bswap(unsigned long x) { return _byteswap_ulong(x); }

		// byte swap
		inline long bswap(long x) { return bswap(static_cast<unsigned long>(x)); }

		// byte swap
		inline unsigned int bswap(unsigned int x) { return bswap(static_cast<unsigned long>(x)); }

		// byte swap
		inline int bswap(int x) { return bswap(static_cast<unsigned int>(x)); }

		// rotate left
		inline unsigned int rotl(unsigned int x, int shift) { return _rotl(x, shift); }

		// rotate left
		inline int rotl(int x, int shift) { return rotl(static_cast<unsigned int>(x), shift); }

		// rotate left
		inline unsigned long rotl(unsigned long x, int shift) { return _lrotl(x, shift); }

		// rotate left
		inline long rotl(long x, int shift) { return rotl(static_cast<unsigned long>(x), shift); }

		// rotate right
		inline unsigned int rotr(unsigned int x, int shift) { return _rotr(x, shift); }

		// rotate right
		inline int rotr(int x, int shift) { return rotr(static_cast<unsigned int>(x), shift); }

		// rotate right
		inline unsigned long rotr(unsigned long x, int shift) { return _lrotr(x, shift); }

		// rotate right
		inline long rotr(long x, int shift) { return rotr(static_cast<unsigned long>(x), shift); }

# if defined(_M_X64) || defined(_M_AMD64)
		// bit scan forward
		inline unsigned int bsf(unsigned long long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanForward64(&i, x);

			return i;
		}

		// bit scan forward
		inline unsigned int bsf(long long x) { return bsf(static_cast<unsigned long long>(x)); }

		// bit scan reverse
		inline unsigned int bsr(unsigned long long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanReverse64(&i, x);

			return i;
		}

		// bit scan reverse
		inline unsigned int bsr(long long x) { return bsr(static_cast<unsigned long long>(x)); }

		// byte swap
		inline unsigned long long bswap(unsigned long long x) { return _byteswap_uint64(x); }

		// byte swap
		inline long long bswap(long long x) { return bswap(static_cast<unsigned long long>(x)); }

		// rotate left
		inline unsigned long long rotl(unsigned long long x, int shift) { return _rotl64(x, shift); }

		// rotate left
		inline long long rotl(long long x, int shift) { return rotl(static_cast<unsigned long long>(x), shift); }

		// rotate right
		inline unsigned long long rotr(unsigned long long x, int shift) { return _rotr64(x, shift); }

		// rotate right
		inline long long rotr(long long x, int shift) { return rotr(static_cast<unsigned long long>(x), shift); }
# endif
#endif

# if defined(__POPCNT__)
		// return the count of number of bits set to 1
		inline unsigned int popcnt(unsigned int x) { return _mm_popcnt_u32(x); }

		// return the count of number of bits set to 1
		inline unsigned int popcnt(int x) { return popcnt(static_cast<unsigned int>(x)); }

# if defined(_MSC_VER) || !defined(__LP64__)
		// return the count of number of bits set to 1
		inline unsigned long popcnt(unsigned long x) { return popcnt(static_cast<unsigned int>(x)); }

		// return the count of number of bits set to 1
		inline unsigned long popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }
#endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		// return the count of number of bits set to 1
		inline unsigned long long popcnt(unsigned long long x) { return _mm_popcnt_u64(x); }

		// return the count of number of bits set to 1
		inline unsigned long long popcnt(long long x) { return popcnt(static_cast<unsigned long long>(x)); }
#  endif

# if defined(__LP64__)
		// return the count of number of bits set to 1
		inline unsigned long popcnt(unsigned long x) { return popcnt(static_cast<unsigned long long>(x)); }

		// return the count of number of bits set to 1
		inline unsigned long popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }
#endif
# endif

#if defined(__LZCNT__)
		// count the number of leading zero bits
		inline unsigned int lzcnt(unsigned int x) { return _lzcnt_u32(x); }

		// count the number of leading zero bits
		inline unsigned int lzcnt(int x) { return lzcnt(static_cast<unsigned int>(x)); }

# if defined(_MSC_VER) || !defined(__LP64__)
		// count the number of leading zero bits
		inline unsigned long lzcnt(unsigned long x) { return lzcnt(static_cast<unsigned int>(x)); }

		// count the number of leading zero bits
		inline unsigned long lzcnt(long x) { return lzcnt(static_cast<unsigned long>(x)); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		// count the number of leading zero bits
		inline unsigned long long lzcnt(unsigned long long x) { return _lzcnt_u64(x); }

		// count the number of leading zero bits
		inline unsigned long long lzcnt(long long x) { return lzcnt(static_cast<unsigned long long>(x)); }
# endif

# if defined(__LP64__)
		// count the number of leading zero bits
		inline unsigned long lzcnt(unsigned long x) { return lzcnt(static_cast<unsigned long long>(x)); }

		// count the number of leading zero bits
		inline unsigned long lzcnt(long x) { return lzcnt(static_cast<unsigned long>(x)); }
# endif
#endif

#if defined(__BMI__)
		// count the number of trailing zero bits
		inline unsigned int tzcnt(unsigned int x) { return _tzcnt_u32(x); }

		// count the number of trailing zero bits
		inline unsigned int tzcnt(int x) { return tzcnt(static_cast<unsigned int>(x)); }

# if defined(_MSC_VER) || !defined(__LP64__)
		// count the number of trailing zero bits
		inline unsigned long tzcnt(unsigned long x) { return tzcnt(static_cast<unsigned int>(x)); }

		// count the number of trailing zero bits
		inline unsigned long tzcnt(long x) { return tzcnt(static_cast<unsigned long>(x)); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		// count the number of trailing zero bits
		inline unsigned long long tzcnt(unsigned long long x) { return _tzcnt_u64(x); }

		// count the number of trailing zero bits
		inline unsigned long long tzcnt(long long x) { return tzcnt(static_cast<unsigned long long>(x)); }
# endif

# if defined(__LP64__)
		// count the number of trailing zero bits
		inline unsigned long tzcnt(unsigned long x) { return tzcnt(static_cast<unsigned long long>(x)); }

		// count the number of trailing zero bits
		inline unsigned long tzcnt(long x) { return tzcnt(static_cast<unsigned long>(x)); }
# endif
#endif

#if defined(__BMI2__)
		// zero high bits starting with specified bit position
		inline unsigned int bzhi(unsigned int x, unsigned int index) { return _bzhi_u32(x, index); }

		// zero high bits starting with specified bit position
		inline int bzhi(int x, unsigned int index) { return bzhi(static_cast<unsigned int>(x), index); }

		// parallel bits deposit
		inline unsigned int pdep(unsigned int x, unsigned int mask) { return _pdep_u32(x, mask); }

		// parallel bits deposit
		inline int pdep(int x, unsigned int mask) { return pdep(static_cast<unsigned int>(x), mask); }

		// parallel bits extract
		inline unsigned int pext(unsigned int x, unsigned int mask) { return _pext_u32(x, mask); }

		// parallel bits extract
		inline int pext(int x, unsigned int mask) { return pext(static_cast<unsigned int>(x), mask); }

# if defined(_MSC_VER) || !defined(__LP64__)
		// zero high bits starting with specified bit position
		inline unsigned long bzhi(unsigned long x, unsigned int index) { return bzhi(static_cast<unsigned int>(x), index); }

		// zero high bits starting with specified bit position
		inline long bzhi(long x, unsigned int index) { return bzhi(static_cast<unsigned long>(x), index); }

		// parallel bits deposit
		inline unsigned int pdep(unsigned long x, unsigned long mask) { return pdep(static_cast<unsigned int>(x), static_cast<unsigned int>(mask)); }

		// parallel bits deposit
		inline long pdep(long x, unsigned long mask) { return pdep(static_cast<unsigned long>(x), mask); }

		// parallel bits extract
		inline unsigned int pext(unsigned long x, unsigned int mask) { return pext(static_cast<unsigned int>(x), mask); }

		// parallel bits extract
		inline long pext(long x, unsigned int mask) { return pext(static_cast<unsigned long>(x), mask); }
# endif

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		// zero high bits starting with specified bit position
		inline unsigned long long bzhi(unsigned long long x, unsigned int index) { return _bzhi_u64(x, index); }

		// zero high bits starting with specified bit position
		inline long long bzhi(long long x, unsigned int index) { return bzhi(static_cast<unsigned long long>(x), index); }

		// parallel bits deposit
		inline unsigned long long pdep(unsigned long long x, unsigned long long mask) { return _pdep_u64(x, mask); }

		// parallel bits deposit
		inline long long pdep(long long x, unsigned long long mask) { return pdep(static_cast<unsigned long long>(x), mask); }

		// parallel bits extract
		inline unsigned long long pext(unsigned long long x, unsigned long long mask) { return _pext_u64(x, mask); }

		// parallel bits extract
		inline long long pext(long long x, unsigned long long mask) { return pext(static_cast<unsigned long long>(x), mask); }
# endif

# if defined(__LP64__)
		// zero high bits starting with specified bit position
		inline unsigned int bzhi(unsigned long x, unsigned int index) { return bzhi(static_cast<unsigned int>(x), index); }

		// zero high bits starting with specified bit position
		inline long bzhi(long x, unsigned int index) { return bzhi(static_cast<unsigned long>(x), index); }

		// parallel bits deposit
		inline unsigned int pdep(unsigned long x, unsigned long mask) { return pdep(static_cast<unsigned long long>(x), static_cast<unsigned long long>(mask)); }

		// parallel bits deposit
		inline long pdep(long x, unsigned long mask) { return pdep(static_cast<unsigned long>(x), mask); }

		// parallel bits extract
		inline unsigned int pext(unsigned long x, unsigned long mask) { return pext(static_cast<unsigned long long>(x), static_cast<unsigned long long>(mask)); }

		// parallel bits extract
		inline long pext(long x, unsigned long mask) { return pext(static_cast<unsigned long>(x), mask); }
# endif
#endif
	}
}

#endif
