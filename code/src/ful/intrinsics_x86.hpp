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
		// not and
		ful_target("bmi") ful_inline unsigned int andn(unsigned int x, unsigned int y) { return __andn_u32(x, y); }

		// not and
		ful_target("bmi") ful_inline int andn(int x, int y) { return andn(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(int x) { ful_expect(x != 0); return __bsfd(x); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned int x) { return bsf(static_cast<int>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(int x) { ful_expect(x != 0); return __bsrd(x); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned int x) { return bsr(static_cast<int>(x)); }

		// byte swap
		ful_inline unsigned short bswap(unsigned short x) { return __rolw(x, 8); }

		// byte swap
		ful_inline short bswap(short x) { return bswap(static_cast<unsigned short>(x)); }

		// byte swap
		ful_inline int bswap(int x) { return __bswapd(x); }

		// byte swap
		ful_inline unsigned int bswap(unsigned int x) { return bswap(static_cast<int>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned int * src, unsigned int cnt, unsigned int * dst)
		{
			// note missing intrinsics
			__asm__ volatile("rep movsd" : "=D" (dst), "=S" (src), "=c" (cnt) : "0" (dst), "1" (src), "2" (cnt) : "memory");
		}

		// store string
		ful_inline void repstosf(unsigned int * dst, unsigned int cnt, unsigned int val)
		{
			// note missing intrinsics
			__asm__ volatile("rep stosd" : "=D" (dst), "=a" (val), "=c" (cnt) : "0" (dst), "1" (val), "2" (cnt) : "memory");
		}

		// rotate left
		ful_inline unsigned int rotl(unsigned int x, int shift) { return __rold(x, shift); }

		// rotate left
		ful_inline int rotl(int x, int shift) { return rotl(static_cast<unsigned int>(x), shift); }

		// rotate left
		ful_inline unsigned long rotl(unsigned long x, int shift) { return _lrotl(x, shift); }

		// rotate left
		ful_inline long rotl(long x, int shift) { return rotl(static_cast<unsigned long>(x), shift); }

		// rotate right
		ful_inline unsigned int rotr(unsigned int x, int shift) { return __rord(x, shift); }

		// rotate right
		ful_inline int rotr(int x, int shift) { return rotr(static_cast<unsigned int>(x), shift); }

		// rotate right
		ful_inline unsigned long rotr(unsigned long x, int shift) { return _lrotr(x, shift); }

		// rotate right
		ful_inline long rotr(long x, int shift) { return rotr(static_cast<unsigned long>(x), shift); }

# if defined(__x86_64__)
		// not and
		ful_target("bmi") ful_inline unsigned long long andn(unsigned long long x, unsigned long long y) { return __andn_u64(x, y); }

		// not and
		ful_target("bmi") ful_inline long long andn(long long x, long long y) { return andn(static_cast<unsigned long long>(x), static_cast<unsigned long long>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(long long x) { ful_expect(x != 0); return __bsfq(x); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned long long x) { return bsf(static_cast<long long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(long long x) { ful_expect(x != 0); return __bsrq(x); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned long long x) { return bsr(static_cast<long long>(x)); }

		// byte swap
		ful_inline long long bswap(long long x) { return __bswapq(x); }

		// byte swap
		ful_inline unsigned long long bswap(unsigned long long x) { return bswap(static_cast<long long>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned long long * src, unsigned long long cnt, unsigned long long * dst)
		{
			// note missing intrinsics
			__asm__ volatile("rep movsq" : "=D" (dst), "=S" (src), "=c" (cnt) : "0" (dst), "1" (src), "2" (cnt) : "memory");
		}

		// store string
		ful_inline void repstosf(unsigned long long * dst, unsigned long long cnt, unsigned long long val)
		{
			// note missing intrinsics
			__asm__ volatile("rep stosq" : "=D" (dst), "=a" (val), "=c" (cnt) : "0" (dst), "1" (val), "2" (cnt) : "memory");
		}

		// rotate left
		ful_inline unsigned long long rotl(unsigned long long x, int shift) { return __rolq(x, shift); }

		// rotate left
		ful_inline long long rotl(long long x, int shift) { return rotl(static_cast<unsigned long long>(x), shift); }

		// rotate right
		ful_inline unsigned long long rotr(unsigned long long x, int shift) { return __rorq(x, shift); }

		// rotate right
		ful_inline long long rotr(long long x, int shift) { return rotr(static_cast<unsigned long long>(x), shift); }
# endif

# if defined(__LP64__)
		// not and
		ful_target("bmi") ful_inline unsigned long andn(unsigned long x, unsigned long y) { return andn(static_cast<unsigned long long>(x), static_cast<unsigned long long>(y)); }

		// not and
		ful_target("bmi") ful_inline long andn(long x, long y) { return andn(static_cast<unsigned long>(x), static_cast<unsigned long>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(long x) { return bsf(static_cast<long long>(x)); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned long x) { return bsf(static_cast<long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(long x) { return bsr(static_cast<long long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned long x) { return bsr(static_cast<long>(x)); }

		// byte swap
		ful_inline long bswap(long x) { return bswap(static_cast<long long>(x)); }

		// byte swap
		ful_inline unsigned long bswap(unsigned long x) { return bswap(static_cast<long>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned long * src, unsigned long cnt, unsigned long * dst) { repmovf(reinterpret_cast<const unsigned long long *>(src), static_cast<unsigned long long>(cnt), reinterpret_cast<unsigned long long *>(dst)); }

		// store string
		ful_inline void repstosf(unsigned long * dst, unsigned long cnt, unsigned long val) { repstosf(reinterpret_cast<unsigned long long *>(dst), static_cast<unsigned long long>(cnt), static_cast<unsigned long long>(val)); }
# else
		// not and
		ful_target("bmi") ful_inline unsigned long andn(unsigned long x, unsigned long y) { return andn(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); }

		// not and
		ful_target("bmi") ful_inline long andn(long x, long y) { return andn(static_cast<unsigned long>(x), static_cast<unsigned long>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(long x) { return bsf(static_cast<int>(x)); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned long x) { return bsf(static_cast<long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(long x) { return bsr(static_cast<int>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned long x) { return bsr(static_cast<long>(x)); }

		// byte swap
		ful_inline long bswap(long x) { return bswap(static_cast<int>(x)); }

		// byte swap
		ful_inline unsigned long bswap(unsigned long x) { return bswap(static_cast<long>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned long * src, unsigned long cnt, unsigned long * dst) { repmovf(reinterpret_cast<const unsigned int *>(src), static_cast<unsigned int>(cnt), reinterpret_cast<unsigned int *>(dst)); }

		// store string
		ful_inline void repstosf(unsigned long * dst, unsigned long cnt, unsigned long val) { repstosf(reinterpret_cast<unsigned int *>(dst), static_cast<unsigned int>(cnt), static_cast<unsigned int>(val)); }
# endif
#elif defined(_MSC_VER)
		// not and
		ful_target("bmi") ful_inline unsigned int andn(unsigned int x, unsigned int y) { return _andn_u32(x, y); }

		// not and
		ful_target("bmi") ful_inline int andn(int x, int y) { return andn(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); }

		// not and
		ful_target("bmi") ful_inline unsigned long andn(unsigned long x, unsigned long y) { return andn(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); }

		// not and
		ful_target("bmi") ful_inline long andn(long x, long y) { return andn(static_cast<unsigned long>(x), static_cast<unsigned long>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanForward(&i, x);

			return i;
		}

		// bit scan forward
		ful_inline unsigned int bsf(long x) { return bsf(static_cast<unsigned long>(x)); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned int x) { return bsf(static_cast<unsigned long>(x)); }

		// bit scan forward
		ful_inline unsigned int bsf(int x) { return bsf(static_cast<unsigned int>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanReverse(&i, x);

			return i;
		}

		// bit scan reverse
		ful_inline unsigned int bsr(long x) { return bsr(static_cast<unsigned long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned int x) { return bsr(static_cast<unsigned long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(int x) { return bsr(static_cast<unsigned int>(x)); }

		// byte swap
		ful_inline unsigned short bswap(unsigned short x) { return _rotl16(x, 8); }

		// byte swap
		ful_inline short bswap(short x) { return bswap(static_cast<unsigned short>(x)); }

		// byte swap
		ful_inline unsigned long bswap(unsigned long x) { return _byteswap_ulong(x); }

		// byte swap
		ful_inline long bswap(long x) { return bswap(static_cast<unsigned long>(x)); }

		// byte swap
		ful_inline unsigned int bswap(unsigned int x) { return bswap(static_cast<unsigned long>(x)); }

		// byte swap
		ful_inline int bswap(int x) { return bswap(static_cast<unsigned int>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned long * src, unsigned long cnt, unsigned long * dst) { __movsd(dst, src, cnt); }

		// move data from string to string
		ful_inline void repmovf(const unsigned int * src, unsigned int cnt, unsigned int * dst) { repmovf(reinterpret_cast<const unsigned long *>(src), static_cast<unsigned long>(cnt), reinterpret_cast<unsigned long *>(dst)); }

		// move data from string to string

		// store string
		ful_inline void repstosf(unsigned long * dst, unsigned long cnt, unsigned long val) { __stosd(dst, val, cnt); }

		// store string
		ful_inline void repstosf(unsigned int * dst, unsigned int cnt, unsigned int val) { repstosf(reinterpret_cast<unsigned long *>(dst), static_cast<unsigned long>(cnt), static_cast<unsigned long>(val)); }

		// rotate left
		ful_inline unsigned int rotl(unsigned int x, int shift) { return _rotl(x, shift); }

		// rotate left
		ful_inline int rotl(int x, int shift) { return rotl(static_cast<unsigned int>(x), shift); }

		// rotate left
		ful_inline unsigned long rotl(unsigned long x, int shift) { return _lrotl(x, shift); }

		// rotate left
		ful_inline long rotl(long x, int shift) { return rotl(static_cast<unsigned long>(x), shift); }

		// rotate right
		ful_inline unsigned int rotr(unsigned int x, int shift) { return _rotr(x, shift); }

		// rotate right
		ful_inline int rotr(int x, int shift) { return rotr(static_cast<unsigned int>(x), shift); }

		// rotate right
		ful_inline unsigned long rotr(unsigned long x, int shift) { return _lrotr(x, shift); }

		// rotate right
		ful_inline long rotr(long x, int shift) { return rotr(static_cast<unsigned long>(x), shift); }

# if defined(_M_X64) || defined(_M_AMD64)
		// not and
		ful_target("bmi") ful_inline unsigned long long andn(unsigned long long x, unsigned long long y) { return _andn_u64(x, y); }

		// not and
		ful_target("bmi") ful_inline long long andn(long long x, long long y) { return andn(static_cast<unsigned long long>(x), static_cast<unsigned long long>(y)); }

		// bit scan forward
		ful_inline unsigned int bsf(unsigned long long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanForward64(&i, x);

			return i;
		}

		// bit scan forward
		ful_inline unsigned int bsf(long long x) { return bsf(static_cast<unsigned long long>(x)); }

		// bit scan reverse
		ful_inline unsigned int bsr(unsigned long long x)
		{
			ful_expect(x != 0);

			unsigned long i;
			_BitScanReverse64(&i, x);

			return i;
		}

		// bit scan reverse
		ful_inline unsigned int bsr(long long x) { return bsr(static_cast<unsigned long long>(x)); }

		// byte swap
		ful_inline unsigned long long bswap(unsigned long long x) { return _byteswap_uint64(x); }

		// byte swap
		ful_inline long long bswap(long long x) { return bswap(static_cast<unsigned long long>(x)); }

		// move data from string to string
		ful_inline void repmovf(const unsigned long long * src, unsigned long long cnt, unsigned long long * dst) { __movsq(dst, src, cnt); }

		// store string
		ful_inline void repstosf(unsigned long long * dst, unsigned long long cnt, unsigned long long val) { __stosq(dst, val, cnt); }

		// rotate left
		ful_inline unsigned long long rotl(unsigned long long x, int shift) { return _rotl64(x, shift); }

		// rotate left
		ful_inline long long rotl(long long x, int shift) { return rotl(static_cast<unsigned long long>(x), shift); }

		// rotate right
		ful_inline unsigned long long rotr(unsigned long long x, int shift) { return _rotr64(x, shift); }

		// rotate right
		ful_inline long long rotr(long long x, int shift) { return rotr(static_cast<unsigned long long>(x), shift); }
# endif
#endif

#if defined(__GNUC__) && !defined(__clang__)

		// move data after swapping bytes
		ful_target("movbe") ful_inline short loadbe(const short * x)
		{
			short y;

			__asm__("movbe %w0, %1" : "=r" (y) : "mr" (x));

			return y;
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned short loadbe(const unsigned short * x) { return loadbe(reinterpret_cast<const short *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline int loadbe(const int * x)
		{
			int y;

			__asm__("movbe %k0, %1" : "=r" (y) : "mr" (x));

			return y;
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned int loadbe(const unsigned int * x) { return loadbe(reinterpret_cast<const int *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(short * x, short y)
		{
			__asm__ volatile("movbe %0, %w1" : "+mr" (x) : "r" (y) : "memory");
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned short * x, unsigned short y) { storebe(reinterpret_cast<short *>(x), static_cast<short>(y)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(int * x, int y)
		{
			__asm__ volatile("movbe %0, %k1" : "+mr" (x) : "r" (y) : "memory");
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned int * x, unsigned int y) { storebe(reinterpret_cast<int *>(x), static_cast<int>(y)); }

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)

		// move data after swapping bytes
		ful_target("movbe") ful_inline long long loadbe(const long long * x)
		{
			long long y;

			__asm__("movbe %q0, %1" : "=r" (y) : "mr" (x));

			return y;
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned long long loadbe(const unsigned long long * x) { return loadbe(reinterpret_cast<const long long *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(long long * x, long long y)
		{
			__asm__ volatile("movbe %0, %q1" : "+mr" (x) : "r" (y) : "memory");
		}

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned long long * x, unsigned long long y) { storebe(reinterpret_cast<long long *>(x), static_cast<long long>(y)); }

# endif

#else

		// move data after swapping bytes
		ful_target("movbe") ful_inline short loadbe(const short * x) { return _loadbe_i16(x); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned short loadbe(const unsigned short * x) { return loadbe(reinterpret_cast<const short *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline int loadbe(const int * x) { return _loadbe_i32(x); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned int loadbe(const unsigned int * x) { return loadbe(reinterpret_cast<const int *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(short * x, short y) { _storebe_i16(x, y); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned short * x, unsigned short y) { storebe(reinterpret_cast<short *>(x), static_cast<short>(y)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(int * x, int y) { _storebe_i32(x, y); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned int * x, unsigned int y) { storebe(reinterpret_cast<int *>(x), static_cast<int>(y)); }

# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)

		// move data after swapping bytes
		ful_target("movbe") ful_inline long long loadbe(const long long * x) { return _loadbe_i64(x); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned long long loadbe(const unsigned long long * x) { return loadbe(reinterpret_cast<const long long *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(long long * x, long long y) { _storebe_i64(x, y); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned long long * x, unsigned long long y) { storebe(reinterpret_cast<long long *>(x), static_cast<long long>(y)); }

# endif

#endif

#if defined(__LP64__)

		// move data after swapping bytes
		ful_target("movbe") ful_inline long loadbe(const long * x) { return loadbe(reinterpret_cast<const long long *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned long loadbe(const unsigned long * x) { return loadbe(reinterpret_cast<const long *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(long * x, long y) { storebe(reinterpret_cast<long long *>(x), static_cast<long long>(y)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned long * x, unsigned long y) { storebe(reinterpret_cast<long *>(x), static_cast<long>(y)); }

#else

		// move data after swapping bytes
		ful_target("movbe") ful_inline long loadbe(const long * x) { return loadbe(reinterpret_cast<const int *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline unsigned long loadbe(const unsigned long * x) { return loadbe(reinterpret_cast<const long *>(x)); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(long * x, long y) { storebe(reinterpret_cast<int *>(x), y); }

		// move data after swapping bytes
		ful_target("movbe") ful_inline void storebe(unsigned long * x, unsigned long y) { storebe(reinterpret_cast<long *>(x), static_cast<long>(y)); }

#endif

		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline unsigned int bzhi(unsigned int x, unsigned int index) { return _bzhi_u32(x, index); }

		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline int bzhi(int x, unsigned int index) { return bzhi(static_cast<unsigned int>(x), index); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned int lzcnt(unsigned int x) { return _lzcnt_u32(x); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned int lzcnt(int x) { return lzcnt(static_cast<unsigned int>(x)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline unsigned int pdep(unsigned int x, unsigned int mask) { return _pdep_u32(x, mask); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline int pdep(int x, unsigned int mask) { return pdep(static_cast<unsigned int>(x), mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline unsigned int pext(unsigned int x, unsigned int mask) { return _pext_u32(x, mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline int pext(int x, unsigned int mask) { return pext(static_cast<unsigned int>(x), mask); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned int popcnt(unsigned int x) { return _mm_popcnt_u32(x); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned int popcnt(int x) { return popcnt(static_cast<unsigned int>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned int tzcnt(unsigned int x) { return _tzcnt_u32(x); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned int tzcnt(int x) { return tzcnt(static_cast<unsigned int>(x)); }

#if defined(_MSC_VER) || !defined(__LP64__)
		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline unsigned long bzhi(unsigned long x, unsigned int index) { return bzhi(static_cast<unsigned int>(x), index); }

		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline long bzhi(long x, unsigned int index) { return bzhi(static_cast<unsigned long>(x), index); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long lzcnt(unsigned long x) { return lzcnt(static_cast<unsigned int>(x)); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long lzcnt(long x) { return lzcnt(static_cast<unsigned long>(x)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline unsigned int pdep(unsigned long x, unsigned long mask) { return pdep(static_cast<unsigned int>(x), static_cast<unsigned int>(mask)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline long pdep(long x, unsigned long mask) { return pdep(static_cast<unsigned long>(x), mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline unsigned int pext(unsigned long x, unsigned int mask) { return pext(static_cast<unsigned int>(x), mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline long pext(long x, unsigned int mask) { return pext(static_cast<unsigned long>(x), mask); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long popcnt(unsigned long x) { return popcnt(static_cast<unsigned int>(x)); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long tzcnt(unsigned long x) { return tzcnt(static_cast<unsigned int>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long tzcnt(long x) { return tzcnt(static_cast<unsigned long>(x)); }
#endif

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline unsigned long long bzhi(unsigned long long x, unsigned int index) { return _bzhi_u64(x, index); }

		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline long long bzhi(long long x, unsigned int index) { return bzhi(static_cast<unsigned long long>(x), index); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long long lzcnt(unsigned long long x) { return _lzcnt_u64(x); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long long lzcnt(long long x) { return lzcnt(static_cast<unsigned long long>(x)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline unsigned long long pdep(unsigned long long x, unsigned long long mask) { return _pdep_u64(x, mask); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline long long pdep(long long x, unsigned long long mask) { return pdep(static_cast<unsigned long long>(x), mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline unsigned long long pext(unsigned long long x, unsigned long long mask) { return _pext_u64(x, mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline long long pext(long long x, unsigned long long mask) { return pext(static_cast<unsigned long long>(x), mask); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long long popcnt(unsigned long long x) { return _mm_popcnt_u64(x); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long long popcnt(long long x) { return popcnt(static_cast<unsigned long long>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long long tzcnt(unsigned long long x) { return _tzcnt_u64(x); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long long tzcnt(long long x) { return tzcnt(static_cast<unsigned long long>(x)); }
#endif

#if defined(__LP64__)
		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline unsigned long bzhi(unsigned long x, unsigned int index) { return bzhi(static_cast<unsigned long long>(x), index); }

		// zero high bits starting with specified bit position
		ful_target("bmi2") ful_inline long bzhi(long x, unsigned int index) { return bzhi(static_cast<unsigned long>(x), index); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long lzcnt(unsigned long x) { return lzcnt(static_cast<unsigned long long>(x)); }

		// count the number of leading zero bits
		ful_target("lzcnt") ful_inline unsigned long lzcnt(long x) { return lzcnt(static_cast<unsigned long>(x)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline unsigned long pdep(unsigned long x, unsigned long mask) { return pdep(static_cast<unsigned long long>(x), static_cast<unsigned long long>(mask)); }

		// parallel bits deposit
		ful_target("bmi2") ful_inline long pdep(long x, unsigned long mask) { return pdep(static_cast<unsigned long>(x), mask); }

		// parallel bits extract
		ful_target("bmi2") ful_inline unsigned long pext(unsigned long x, unsigned long mask) { return pext(static_cast<unsigned long long>(x), static_cast<unsigned long long>(mask)); }

		// parallel bits extract
		ful_target("bmi2") ful_inline long pext(long x, unsigned long mask) { return pext(static_cast<unsigned long>(x), mask); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long popcnt(unsigned long x) { return popcnt(static_cast<unsigned long long>(x)); }

		// return the count of number of bits set to 1
		ful_target("popcnt") ful_inline unsigned long popcnt(long x) { return popcnt(static_cast<unsigned long>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long tzcnt(unsigned long x) { return tzcnt(static_cast<unsigned long long>(x)); }

		// count the number of trailing zero bits
		ful_target("bmi") ful_inline unsigned long tzcnt(long x) { return tzcnt(static_cast<unsigned long>(x)); }
#endif
	}
}

#endif
