#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/string.hpp"

#include "buffer.hpp"
#include "constants.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

#include <vector>

#if HAVE_ASMLIB
# include <asmlib.h>
#endif

using namespace ful;

namespace
{
#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	inline __m128i rotate_alt_wr(__m128i ab, unsigned int n)
	{
		ful_assume(n < 16);

		alignas(16) signed char buff[32];
		_mm_store_si128(reinterpret_cast<__m128i *>(buff), ab);
		_mm_store_si128(reinterpret_cast<__m128i *>(buff + 16), ab);
		return _mm_loadu_si128(reinterpret_cast<__m128i *>(buff + n));
	}

	inline __m128i rotate_alt_wr(__m128i ab, int n) { return rotate_alt_wr(ab, static_cast<unsigned int>(n) & (16 - 1)); }
#endif
}

TEST_CASE("rotate", "[.internal]")
{
#if defined(__AVX2__)
	{
		const __m256i x = _mm256_set_epi8(32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
		const __m256i y1 = _mm256_set_epi8(1, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2);
		const __m256i y15 = _mm256_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16);
		const __m256i y16 = _mm256_set_epi8(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17);
		const __m256i y31 = _mm256_set_epi8(31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 32);

		BENCHMARK_ADVANCED("rotate avx")(Catch::Benchmark::Chronometer meter)
		{
			const __m256i x1 = detail::rotate(x, 1);
			const __m256i x15 = detail::rotate(x, 15);
			const __m256i x16 = detail::rotate(x, 16);
			const __m256i x31 = detail::rotate(x, 31);
			CHECK(std::memcmp(&x1, &y1, 32) == 0);
			CHECK(std::memcmp(&x15, &y15, 32) == 0);
			CHECK(std::memcmp(&x16, &y16, 32) == 0);
			CHECK(std::memcmp(&x31, &y31, 32) == 0);

			meter.measure([&](int n){ return detail::rotate(x, static_cast<unsigned int>(n * 4711) & (32 - 1)); });
		};
	}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	{
		const __m128i x = _mm_set_epi8(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
		const __m128i y1 = _mm_set_epi8(1, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2);
		const __m128i y7 = _mm_set_epi8(7, 6, 5, 4, 3, 2, 1, 16, 15, 14, 13, 12, 11, 10, 9, 8);
		const __m128i y8 = _mm_set_epi8(8, 7, 6, 5, 4, 3, 2, 1, 16, 15, 14, 13, 12, 11, 10, 9);
		const __m128i y15 = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 16);

		BENCHMARK_ADVANCED("rotate sse2")(Catch::Benchmark::Chronometer meter)
		{
			const __m128i x1 = detail::rotate(x, 1);
			const __m128i x7 = detail::rotate(x, 7);
			const __m128i x8 = detail::rotate(x, 8);
			const __m128i x15 = detail::rotate(x, 15);
			CHECK(std::memcmp(&x1, &y1, 16) == 0);
			CHECK(std::memcmp(&x7, &y7, 16) == 0);
			CHECK(std::memcmp(&x8, &y8, 16) == 0);
			CHECK(std::memcmp(&x15, &y15, 16) == 0);

			meter.measure([&](int n){ return detail::rotate(x, static_cast<unsigned int>(n * 4711) & (16 - 1)); });
		};

		BENCHMARK_ADVANCED("rotate sse2 (write-read)")(Catch::Benchmark::Chronometer meter)
		{
			const __m128i x1 = rotate_alt_wr(x, 1);
			const __m128i x7 = rotate_alt_wr(x, 7);
			const __m128i x8 = rotate_alt_wr(x, 8);
			const __m128i x15 = rotate_alt_wr(x, 15);
			CHECK(std::memcmp(&x1, &y1, 16) == 0);
			CHECK(std::memcmp(&x7, &y7, 16) == 0);
			CHECK(std::memcmp(&x8, &y8, 16) == 0);
			CHECK(std::memcmp(&x15, &y15, 16) == 0);

			meter.measure([&](int n){ return rotate_alt_wr(x, static_cast<unsigned int>(n * 4711) & (16 - 1)); });
		};
	}
#endif
}

namespace
{
	char8 * copy_naive(const char8 * first, const char8 * last, char8 * begin)
	{
		for (; first != last;)
		{
			*begin = *first;
			++first, ++begin;
		}
		return begin;
	}

	void copy_test(unsigned long long size, unsigned int from, unsigned int to)
	{
		BENCHMARK_ADVANCED("copy (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size + 1);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return std::copy(first, last, begin); });
		};

#if HAVE_ASMLIB
		BENCHMARK_ADVANCED("memcopy (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return A_memcpy(begin, first, last - first); });
		};

		BENCHMARK_ADVANCED("strcpy (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return A_strcpy(reinterpret_cast<char *>(begin), reinterpret_cast<const char *>(first)); });
		};
#endif

		BENCHMARK_ADVANCED("naive for-loop")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return copy_naive(first, last, begin); });
		};

		BENCHMARK_ADVANCED("memcopy")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return memcopy(first, last, begin); });
		};

		BENCHMARK_ADVANCED("memcopy none")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memcopy_none(first, last, begin); });
		};

#if defined(__AVX__)
		BENCHMARK_ADVANCED("memcopy avx")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memcopy_avx(first, last, begin); });
		};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		BENCHMARK_ADVANCED("memcopy sse")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memcopy_sse(first, last, begin); });
		};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("memcopy sse2")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memcopy_sse2(first, last, begin); });
		};
#endif
	}
}

TEST_CASE("copy", "")
{
	SECTION("petty")
	{
		copy_test(pettyval, 7, 1);
	}

	SECTION("small")
	{
		copy_test(smallval, 7, 1);
	}

	SECTION("plain")
	{
		copy_test(plainval, 7, 1);
	}

	SECTION("large")
	{
		copy_test(largeval, 7, 1);
	}
}

namespace
{
	inline char8 * rcopy_naive(const char8 * first, const char8 * last, char8 * end)
	{
		for (; first != last;)
		{
			--last, --end;
			*end = *last;
		}
		return end;
	}

	void rcopy_test(unsigned long long size, unsigned int from, unsigned int to)
	{
		BENCHMARK_ADVANCED("copy_backward (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return std::copy_backward(first, last, end); });
		};

		BENCHMARK_ADVANCED("memmove (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			// char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return std::memmove(begin, first, size); });
		};

#if HAVE_ASMLIB
		BENCHMARK_ADVANCED("memmove (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			// char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return A_memmove(begin, first, size); });
		};
#endif

		BENCHMARK_ADVANCED("naive for-loop")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return rcopy_naive(first, last, end); });
		};

		BENCHMARK_ADVANCED("memypoc")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return memypoc(first, last, end); });
		};

		BENCHMARK_ADVANCED("memypoc none")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memypoc_none(first, last, end); });
		};

#if defined(__AVX__)
		BENCHMARK_ADVANCED("memypoc avx")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memypoc_avx(first, last, end); });
		};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		BENCHMARK_ADVANCED("memypoc sse")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memypoc_sse(first, last, end); });
		};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("memypoc sse2")(Catch::Benchmark::Chronometer meter)
		{
			if (size <= 16u)
				return;

			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;
			char8 * const end = begin + size;

			std::fill(first, last, 'Z');
			*last = 0;

			meter.measure([&](int){ return detail::memypoc_sse2(first, last, end); });
		};
#endif
	}
}

TEST_CASE("rcopy", "")
{
	SECTION("petty")
	{
		rcopy_test(pettyval, 1, 7);
	}

	SECTION("small")
	{
		rcopy_test(smallval, 1, 7);
	}

	SECTION("plain")
	{
		rcopy_test(plainval, 1, 7);
	}

	SECTION("large")
	{
		rcopy_test(largeval, 1, 7);
	}
}

namespace
{
	void fill_alt_naive(unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
	{
		for (; from != to; ++from)
		{
			*from = u;
		}
	}

	void fill_test(unsigned long long size, unsigned int offset)
	{
		BENCHMARK_ADVANCED("fill (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ std::fill(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};

#if HAVE_ASMLIB
		BENCHMARK_ADVANCED("memset (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ return A_memset(tyt.data() + offset, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1)), size); });
		};
#endif

		BENCHMARK_ADVANCED("naive for-loop")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ fill_alt_naive(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};

		BENCHMARK_ADVANCED("memset")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ memset(reinterpret_cast<char8 *>(tyt.data()) + offset, reinterpret_cast<char8 *>(tyt.data()) + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};

		BENCHMARK_ADVANCED("memset8 none")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ detail::memset8_none(reinterpret_cast<char8 *>(tyt.data() + offset), reinterpret_cast<char8 *>(tyt.data() + offset + size), static_cast<char8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("memset8 sse2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ detail::memset8_sse2(reinterpret_cast<char8 *>(tyt.data() + offset), reinterpret_cast<char8 *>(tyt.data() + offset + size), static_cast<char8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};
#endif

#if defined(__AVX__)
		BENCHMARK_ADVANCED("memset8 avx")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ detail::memset8_avx(reinterpret_cast<char8 *>(tyt.data() + offset), reinterpret_cast<char8 *>(tyt.data() + offset + size), static_cast<char8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};
#endif
	}
}

TEST_CASE("fill", "")
{
	SECTION("petty")
	{
		fill_test(pettyval, 1);
	}

	SECTION("small")
	{
		fill_test(smallval, 1);
	}

	SECTION("plain")
	{
		fill_test(plainval, 1);
	}

	SECTION("large")
	{
		fill_test(largeval, 1);
	}
}

namespace
{
	uint32 code_point_alt_naive(const unit_utf8 * s)
	{
		switch (point_size(s))
		{
		case 4: return uint32(s[0] & 0x07) << 18 | uint32(s[1] & 0x3f) << 12 | uint32(s[2] & 0x3f) << 6 | uint32(s[3] & 0x3f);
		case 3: return uint32(s[0] & 0x0f) << 12 | uint32(s[1] & 0x3f) << 6 | uint32(s[2] & 0x3f);
		case 2: return uint32(s[0] & 0x1f) << 6 | uint32(s[1] & 0x3f);
		case 1: return s[0];
		default: ful_unreachable();
		}
	}

#if defined (__BMI2__)
	uint32 code_point_alt_bmi2(const unit_utf8 * s)
	{
		static const unsigned long long table[] = {
			0x0000007f, 0x00001f3f, 0x000f3f3f, 0x033f3f3f,
		};

		const unsigned int size = point_size(s) - 1;

		const unit_utf8 * beg_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -8);
		const unit_utf8 * end_word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s + size) & -8);

		uint64 word = *reinterpret_cast<const uint64 *>(beg_word);
		if (beg_word != end_word)
		{
			word >>= 32;
			word <<= 32;
			word |= static_cast<uint32>(*reinterpret_cast<const uint64 *>(end_word));
		}
		const unsigned int end_offset = reinterpret_cast<puint>(s + size) & (8 - 1);
		const unsigned int shift = (end_offset + 1) * 8;
		word = rotr(word, shift);

		word = byte_swap(word);
		return static_cast<uint32>(pext(word, table[size])); // guaranteed at most 21 bits
	}
#endif
}

TEST_CASE("code_point", "")
{
	static const char * const cstrs[] = {
		u8"\U00000024", u8"\U00000056", u8"\U0000007e", u8"\U000000a2", u8"\U000000b9", u8"\U00000939", u8"\U000020ac", u8"\U0000d55c", u8"\U00010348", u8"\U0001f596"
	};
	static const unsigned int codes[] = {
		0x24, 0x56, 0x7e, 0xa2, 0xb9, 0x939, 0x20ac, 0xd55c, 0x10348, 0x1f596,
	};

	BENCHMARK_ADVANCED("code_point (naive)")(Catch::Benchmark::Chronometer meter)
	{
		for (int i = 0; i < static_cast<int>(sizeof cstrs / sizeof cstrs[0]); i++)
		{
			REQUIRE(code_point_alt_naive(cstrs[i]) == codes[i]);
		}
		meter.measure([&](int n){
			uint32_t sum = 0;
			for (int i = 0; i < 20; i++)
			{
				sum += code_point_alt_naive(cstrs[(i * 4711 + n) % (sizeof cstrs / sizeof cstrs[0])]);
			}
			return sum;
		});
	};

#if defined(__BMI2__)
	BENCHMARK_ADVANCED("code_point (bmi2)")(Catch::Benchmark::Chronometer meter)
	{
		for (int i = 0; i < sizeof cstrs / sizeof cstrs[0]; i++)
		{
			REQUIRE(code_point_alt_bmi2(cstrs[i]) == codes[i]);
		}
		meter.measure([&](int n){
			uint32_t sum = 0;
			for (int i = 0; i < 20; i++)
			{
				sum += code_point_alt_bmi2(cstrs[(i * 4711 + n) % (sizeof cstrs / sizeof cstrs[0])]);
			}
			return sum;
		});
	};
#endif
}

namespace
{
	const unit_utf8 * point_next_alt_ahead(const unit_utf8 * s)
	{
		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		return s;
	}

	const unit_utf8 * point_next_alt_ahead(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_ahead(s);
		}
		return s;
	}

	const unit_utf8 * point_next_alt_if_and(const unit_utf8 * s)
	{
		if (static_cast<sint8>(*s) >= 0)
		{
			return s + 1;
		}
		else if ((static_cast<sint8>(*s) & 0x20) == 0)
		{
			return s + 2;
		}
		else if ((static_cast<sint8>(*s) & 0x10) == 0)
		{
			return s + 3;
		}
		else
		{
			return s + 4;
		}
	}

	const unit_utf8 * point_next_alt_if_and(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_if_and(s);
		}
		return s;
	}

	const unit_utf8 * point_next_alt_if_less(const unit_utf8 * s)
	{
		const sint8 v = static_cast<sint8>(*s);

		++s;
		if (v >= 0)
			return s;

		++s;
		if ((v & 0x20) == 0)
			return s;

		++s;
		if ((v & 0x10) == 0)
			return s;

		++s;
		return s;
	}

	const unit_utf8 * point_next_alt_if_less(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_if_less(s);
		}
		return s;
	}

	const unit_utf8 * point_next_alt_switch(const unit_utf8 * s)
	{
		switch (static_cast<uint8>(*s) >> 4)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return s + 1;
		case 12:
		case 13:
			return s + 2;
		case 14:
			return s + 3;
		case 15:
			return s + 4;
		default: ful_unreachable();
		}
	}

	const unit_utf8 * point_next_alt_switch(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_switch(s);
		}
		return s;
	}

#if defined(__AVX2__)
	const unit_utf8 * point_next_avx2(const unit_utf8 * s, usize n)
	{
		ful_assume(0 < n);

		alignas(32) static const signed char m65[] = {
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
		};

		const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -32);
		const unsigned int offset = reinterpret_cast<puint>(s) & (32 - 1);

		__m256i cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
		unsigned int mask = zero_low_bits(_mm256_movemask_epi8(cmpi), offset);
		while (true)
		{
			const unsigned int npoints = detail::popcnt(mask);

			if (n <= npoints)
				break;

			n -= npoints;

			word += 32;

			cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
			mask = _mm256_movemask_epi8(cmpi);
		}
		const unsigned int i = index_set_bit(mask, static_cast<unsigned int>(n - 1)); // n <= 32
		word += i;
		return word + point_size(word);
	}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const unit_utf8 * point_next_sse2(const unit_utf8 * s, usize n)
	{
		ful_assume(0 < n);

		alignas(16) static const signed char m65[] = {
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
		};

		const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -16);
		const unsigned int offset = reinterpret_cast<puint>(s) & (16 - 1);

		__m128i cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
		unsigned int mask = zero_low_bits(_mm_movemask_epi8(cmpi), offset);
		while (true)
		{
			const unsigned int npoints = detail::popcnt(mask);

			if (n <= npoints)
				break;

			n -= npoints;

			word += 16;

			cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
			mask = _mm_movemask_epi8(cmpi);
		}
		const unsigned int i = index_set_bit(mask, static_cast<unsigned int>(n - 1)); // n <= 32
		word += i;
		return word + point_size(word);
	}
#endif
}

TEST_CASE("point_next", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("point_next (ahead)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_ahead(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_ahead(txt.beg(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_next (if and)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_if_and(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_if_and(txt.beg(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_next (if less)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_if_less(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_if_less(txt.beg(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_next (switch)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_switch(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_switch(txt.beg(), txt.npoints()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("point_next (avx2)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_avx2(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_avx2(txt.beg(), txt.npoints()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("point_next sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_sse2(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_sse2(txt.beg(), txt.npoints()); });
	};
#endif
}

namespace
{
	const unit_utf8 * point_prev_alt_naive(const unit_utf8 * s)
	{
		--s;
		if ((*s & 0x80) == 0)
			return s;

		--s;
		if (*s & 0x40)
			return s;

		--s;
		if (*s & 0x40)
			return s;

		--s;
		return s;
	}

	const unit_utf8 * point_prev_alt_naive(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_prev_alt_naive(s);
		}
		return s;
	}

#if defined(__AVX2__)
	const unit_utf8 * point_prev_avx2_alt_naive(const unit_utf8 * s)
	{
		alignas(32) static const signed char m65[] = {
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
		};

		--s;
		const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -32);
		const unsigned int offset = reinterpret_cast<puint>(s) & (32 - 1);

		__m256i cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
		unsigned int mask = zero_higher_bits(_mm256_movemask_epi8(cmpi), offset);
		if (mask == 0)
		{
			word -= 32;

			cmpi = _mm256_cmpgt_epi8(*reinterpret_cast<const __m256i *>(word), *reinterpret_cast<const __m256i *>(m65));
			mask = _mm256_movemask_epi8(cmpi);
		}
		const unsigned int i = most_significant_set_bit(mask);
		return word + i;
	}

	const unit_utf8 * point_prev_avx2_alt_naive(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_prev_avx2_alt_naive(s);
		}
		return s;
	}
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const unit_utf8 * point_prev_sse2_alt_naive(const unit_utf8 * s)
	{
		alignas(16) static const signed char m65[] = {
			-65, -65, -65, -65, -65, -65, -65, -65,
			-65, -65, -65, -65, -65, -65, -65, -65,
		};

		--s;
		const unit_utf8 * word = reinterpret_cast<const unit_utf8 *>(reinterpret_cast<puint>(s) & -16);
		const unsigned int offset = reinterpret_cast<puint>(s) & (16 - 1);

		__m128i cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
		unsigned int mask = zero_higher_bits(_mm_movemask_epi8(cmpi), offset);
		if (mask == 0)
		{
			word -= 16;

			cmpi = _mm_cmpgt_epi8(*reinterpret_cast<const __m128i *>(word), *reinterpret_cast<const __m128i *>(m65));
			mask = _mm_movemask_epi8(cmpi);
		}
		const unsigned int i = most_significant_set_bit(mask);
		return word + i;
	}

	const unit_utf8 * point_prev_sse2_alt_naive(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_prev_sse2_alt_naive(s);
		}
		return s;
	}
#endif
}

TEST_CASE("point_prev", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("point_prev (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_alt_naive(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_alt_naive(txt.end(), txt.npoints()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("point_prev avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::point_prev_avx2(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return detail::point_prev_avx2(txt.end(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_prev avx2 (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_avx2_alt_naive(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_avx2_alt_naive(txt.end(), txt.npoints()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("point_prev sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::point_prev_sse2(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return detail::point_prev_sse2(txt.end(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_prev sse2 (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_sse2_alt_naive(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_sse2_alt_naive(txt.end(), txt.npoints()); });
	};
#endif
}

namespace
{
	const unit_utf8 * point_size_(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s += point_size(s);
		}
		return s;
	}

	unsigned int point_size_alt_table(const unit_utf8 * s)
	{
		static const unsigned char table[16] = {
			1, 1, 1, 1,
			1, 1, 1, 1,
			0, 0, 0, 0, // all unused
			2, 2, 3, 4,
		};

		return table[static_cast<uint8>(s[0]) >> 4];
	}

	const unit_utf8 * point_size_alt_table(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s += point_size_alt_table(s);
		}
		return s;
	}

#if defined(__LZCNT__)
	unsigned int point_size_alt_lzcnt(const unit_utf8 * s)
	{
		// counts the number of "sign" bits, i.e. the number of bits
		// following the sign that are the same as the sign + 1
		const int n = *s | 0x40;
		return detail::lzcnt(n ^ (n >> 1)) & 7;
	}

	const unit_utf8 * point_size_alt_lzcnt(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s += point_size_alt_lzcnt(s);
		}
		return s;
	}
#endif
}

TEST_CASE("point_size", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("point_size")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_size_(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_size_(txt.beg(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_size (table)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_size_alt_table(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_size_alt_table(txt.beg(), txt.npoints()); });
	};

#if defined(__LZCNT__)
	BENCHMARK_ADVANCED("point_size (lzcnt)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_size_alt_lzcnt(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_size_alt_lzcnt(txt.beg(), txt.npoints()); });
	};
#endif
}

TEST_CASE("equal_cstr", "")
{
	data_any_utf8 txt;
	data_any_utf8 tyt;
	REQUIRE(txt.init());
	REQUIRE(tyt.init());

	BENCHMARK_ADVANCED("equal_cstr (std strcmp)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(std::strcmp(txt.cstr(), tyt.cstr()) == 0);
		meter.measure([&](int){ return std::strcmp(txt.cstr(), tyt.cstr()) == 0; });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("equal_cstr (asmlib strcmp)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(A_strcmp(txt.cstr(), tyt.cstr()) == 0);
		meter.measure([&](int){ return A_strcmp(txt.cstr(), tyt.cstr()) == 0; });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("equal_cstr avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return detail::equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("equal_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::equal_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())));
		meter.measure([&](int){ return detail::equal_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())); });
	};
#endif

#if defined(FUL_IFUNC) || defined(FUL_FPTR)
	BENCHMARK_ADVANCED("equal_cstr dispatch")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(equal(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

	BENCHMARK_ADVANCED("equal_cstr none")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return detail::equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
	};
}

TEST_CASE("find_unit", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("find_unit (std strchr)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(std::strchr(txt.cstr(), txt.cunit()) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return std::strchr(txt.cstr(), txt.cunit()); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("find_unit (asmlib strstr)")(Catch::Benchmark::Chronometer meter)
	{
		// note asmlib does not have strchr
		const char str[2] = {txt.cunit(), '\0'};
		REQUIRE(A_strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return A_strstr(txt.cstr(), str); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::find_unit_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), txt.cunit()) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return detail::find_unit_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), txt.cunit()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("find_unit sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::find_unit_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), static_cast<char8>(txt.cunit())) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), static_cast<char8>(txt.cunit())); });
	};
#endif
}

TEST_CASE("less_cstr", "")
{
	data_any_utf8 txt;
	data_any_utf8 tyt;
	REQUIRE(txt.init());
	REQUIRE(tyt.init());

	// decrement last character, now txt is less than tyt
	ful::unit_utf8 & txt_last = txt.data()[txt.size() - 1];
	REQUIRE(0 < static_cast<int>(txt_last));
	txt_last--;

	BENCHMARK_ADVANCED("less_cstr (std strcmp)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(std::strcmp(txt.cstr(), tyt.cstr()) < 0);
		REQUIRE(std::strcmp(tyt.cstr(), txt.cstr()) > 0);
		meter.measure([&](int){ return std::strcmp(txt.cstr(), tyt.cstr()) < 0; });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("less_cstr (asmlib strcmp)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(A_strcmp(txt.cstr(), tyt.cstr()) < 0);
		REQUIRE(A_strcmp(tyt.cstr(), txt.cstr()) > 0);
		meter.measure([&](int){ return A_strcmp(txt.cstr(), tyt.cstr()) < 0; });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("less_cstr avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(detail::less_cstr_avx2(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return detail::less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("less_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::less_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())));
		REQUIRE_FALSE(detail::less_cstr_sse2(reinterpret_cast<const char8 *>(tyt.beg()), reinterpret_cast<const char8 *>(tyt.end()), reinterpret_cast<const char8 *>(txt.cstr())));
		meter.measure([&](int){ return detail::less_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())); });
	};
#endif

#if defined(FUL_IFUNC) || defined(FUL_FPTR)
	BENCHMARK_ADVANCED("less_cstr dispatch")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(less(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

	BENCHMARK_ADVANCED("less_cstr none")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(detail::less_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(detail::less_cstr_none(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return detail::less_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
	};
}
