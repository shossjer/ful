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
