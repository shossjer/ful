#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/string.hpp"
#include "ful/string_init.hpp"

#include "buffer.hpp"
#include "constants.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>
#include "catchhacks.hpp"

#include <vector>

#if HAVE_ASMLIB
# include <asmlib.h>
#endif

#if HAVE_EASTL
# include <EASTL/string.h>
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
	char8 * memcopy_naive(const char8 * first, const char8 * last, char8 * begin)
	{
		for (; first != last;)
		{
			*begin = *first;
			++first, ++begin;
		}
		return begin;
	}

	void memcopy_test(unsigned long long size, unsigned int from, unsigned int to)
	{
		BENCHMARK_ADVANCED("copy (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 from_buffer;
			from_buffer.allocate(from + size);

			buffer_utf8 to_buffer;
			to_buffer.allocate(to + size);

			char8 * const first = reinterpret_cast<char8 *>(from_buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(to_buffer.data()) + to;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return std::copy(first, last, begin); });
		};

#if HAVE_ASMLIB
		BENCHMARK_ADVANCED("memcopy (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 from_buffer;
			from_buffer.allocate(from + size);

			buffer_utf8 to_buffer;
			to_buffer.allocate(to + size);

			char8 * const first = reinterpret_cast<char8 *>(from_buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(to_buffer.data()) + to;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return A_memcpy(begin, first, last - first); });
		};

		BENCHMARK_ADVANCED("strcpy (asmlib)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 from_buffer;
			from_buffer.allocate(from + size);

			buffer_utf8 to_buffer;
			to_buffer.allocate(to + size);

			char8 * const first = reinterpret_cast<char8 *>(from_buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(to_buffer.data()) + to;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return A_strcpy(reinterpret_cast<char *>(begin), reinterpret_cast<const char *>(first)); });
		};
#endif

		BENCHMARK_ADVANCED("naive for-loop")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 from_buffer;
			from_buffer.allocate(from + size);

			buffer_utf8 to_buffer;
			to_buffer.allocate(to + size);

			char8 * const first = reinterpret_cast<char8 *>(from_buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(to_buffer.data()) + to;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return memcopy_naive(first, last, begin); });
		};

		BENCHMARK_ADVANCED("memcopy")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 from_buffer;
			from_buffer.allocate(from + size);

			buffer_utf8 to_buffer;
			to_buffer.allocate(to + size);

			byte * const first = reinterpret_cast<byte *>(from_buffer.data()) + from;
			byte * const last = first + size;
			byte * const begin = reinterpret_cast<byte *>(to_buffer.data()) + to;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return memcopy(first, last, begin); });
		};

#if defined(__AVX__)
		if (64u < size)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (32u < size)
#else
		if (16u < size)
#endif
		{
			BENCHMARK_ADVANCED("memcopy none")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 from_buffer;
				from_buffer.allocate(from + size);

				buffer_utf8 to_buffer;
				to_buffer.allocate(to + size);

				byte * const first = reinterpret_cast<byte *>(from_buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(to_buffer.data()) + to;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memcopy_none(first, last, begin); });
			};

#if defined(__AVX__)
			BENCHMARK_ADVANCED("memcopy avx")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 from_buffer;
				from_buffer.allocate(from + size);

				buffer_utf8 to_buffer;
				to_buffer.allocate(to + size);

				byte * const first = reinterpret_cast<byte *>(from_buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(to_buffer.data()) + to;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memcopy_avx(first, last, begin); });
			};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			BENCHMARK_ADVANCED("memcopy sse")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 from_buffer;
				from_buffer.allocate(from + size);

				buffer_utf8 to_buffer;
				to_buffer.allocate(to + size);

				byte * const first = reinterpret_cast<byte *>(from_buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(to_buffer.data()) + to;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memcopy_sse(first, last, begin); });
			};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			BENCHMARK_ADVANCED("memcopy sse2")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 from_buffer;
				from_buffer.allocate(from + size);

				buffer_utf8 to_buffer;
				to_buffer.allocate(to + size);

				byte * const first = reinterpret_cast<byte *>(from_buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(to_buffer.data()) + to;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memcopy_sse2(first, last, begin); });
			};
#endif
		}
	}
}

TEST_CASE("memcopy data", "[plain]")
{
	data_any_utf8 data;
	REQUIRE(data.init());

	memcopy_test(data.size(), 7, 1);
}

TEST_CASE("memcopy petty", "[petty]")
{
	memcopy_test(pettyval, 7, 1);
}

TEST_CASE("memcopy small", "[small]")
{
	memcopy_test(smallval, 7, 1);
}

TEST_CASE("memcopy plain", "[plain]")
{
	memcopy_test(plainval, 7, 1);
}

TEST_CASE("memcopy large", "[.][large]")
{
	memcopy_test(largeval, 7, 1);
}

namespace
{
	inline char8 * memmover_naive(const char8 * first, const char8 * last, char8 * end)
	{
		for (; first != last;)
		{
			--last, --end;
			*end = *last;
		}
		return end;
	}

	void memmover_test(unsigned long long size, unsigned int from, unsigned int to)
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

			meter.measure([&](int){ return std::copy_backward(first, last, end); });
		};

		BENCHMARK_ADVANCED("memmove (std)")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			char8 * const first = reinterpret_cast<char8 *>(buffer.data()) + from;
			char8 * const last = first + size;
			char8 * const begin = reinterpret_cast<char8 *>(buffer.data()) + to;

			std::fill(first, last, 'Z');

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

			std::fill(first, last, 'Z');

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

			meter.measure([&](int){ return memmover_naive(first, last, end); });
		};

		BENCHMARK_ADVANCED("memmover")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate((from < to ? to : from) + size);

			byte * const first = reinterpret_cast<byte *>(buffer.data()) + from;
			byte * const last = first + size;
			byte * const begin = reinterpret_cast<byte *>(buffer.data()) + to;
			byte * const end = begin + size;

			std::fill(first, last, 'Z');

			meter.measure([&](int){ return memmover(first, last, end); });
		};

#if defined(__AVX__)
		if (64u < size)
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		if (32u < size)
#else
		if (16u < size)
#endif
		{
			BENCHMARK_ADVANCED("memmover none")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 buffer;
				buffer.allocate((from < to ? to : from) + size);

				byte * const first = reinterpret_cast<byte *>(buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(buffer.data()) + to;
				byte * const end = begin + size;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memmover_none(first, last, end); });
			};

#if defined(__AVX__)
			BENCHMARK_ADVANCED("memmover avx")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 buffer;
				buffer.allocate((from < to ? to : from) + size);

				byte * const first = reinterpret_cast<byte *>(buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(buffer.data()) + to;
				byte * const end = begin + size;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memmover_avx(first, last, end); });
			};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
			BENCHMARK_ADVANCED("memmover sse")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 buffer;
				buffer.allocate((from < to ? to : from) + size);

				byte * const first = reinterpret_cast<byte *>(buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(buffer.data()) + to;
				byte * const end = begin + size;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memmover_sse(first, last, end); });
			};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
			BENCHMARK_ADVANCED("memmover sse2")(Catch::Benchmark::Chronometer meter)
			{
				buffer_utf8 buffer;
				buffer.allocate((from < to ? to : from) + size);

				byte * const first = reinterpret_cast<byte *>(buffer.data()) + from;
				byte * const last = first + size;
				byte * const begin = reinterpret_cast<byte *>(buffer.data()) + to;
				byte * const end = begin + size;

				std::fill(first, last, 'Z');

				meter.measure([&](int){ return detail::memmover_sse2(first, last, end); });
			};
#endif
		}
	}
}

TEST_CASE("memypoc data", "[plain]")
{
	data_any_utf8 data;
	REQUIRE(data.init());

	memmover_test(data.size(), 1, 7);
}

TEST_CASE("memypoc petty", "[petty]")
{
	memmover_test(pettyval, 1, 7);
}

TEST_CASE("memypoc small", "[small]")
{
	memmover_test(smallval, 1, 7);
}

TEST_CASE("memypoc plain", "[plain]")
{
	memmover_test(plainval, 1, 7);
}

TEST_CASE("memypoc large", "[.][large]")
{
	memmover_test(largeval, 1, 7);
}

namespace
{
	void memset_naive(unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
	{
		for (; from != to; ++from)
		{
			*from = u;
		}
	}

	void memset_test(unsigned long long size, unsigned int offset)
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

			meter.measure([&](int n){ memset_naive(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
		};

		BENCHMARK_ADVANCED("memset")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(offset + size);

			meter.measure([&](int n){ memset(reinterpret_cast<char8 *>(tyt.data()) + offset, reinterpret_cast<char8 *>(tyt.data()) + offset + size, static_cast<char8>('a' + n % ('z' - 'a' + 1))); return tyt.data() + offset; });
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

TEST_CASE("memset data", "[plain]")
{
	data_any_utf8 data;
	REQUIRE(data.init());

	memset_test(data.size(), 1);
}

TEST_CASE("memset petty", "[petty]")
{
	memset_test(pettyval, 1);
}

TEST_CASE("memset small", "[small]")
{
	memset_test(smallval, 1);
}

TEST_CASE("memset plain", "[plain]")
{
	memset_test(plainval, 1);
}

TEST_CASE("memset large", "[.][large]")
{
	memset_test(largeval, 1);
}

namespace
{
	struct setup_compare_equal
	{
		const int unalignment1 = 0;
		const int unalignment2 = 1;

		template <typename F>
		void operator () (Catch::Benchmark::Groupometer meter, F && func) const
		{
			buffer_utf8 buffer1;
			buffer1.allocate(meter.size() + unalignment1);

			buffer_utf8 buffer2;
			buffer2.allocate(meter.size() + unalignment2);

			std::fill(buffer1.beg(), buffer1.end(), static_cast<ful::unit_utf8>('a'));
			std::fill(buffer2.beg(), buffer2.end(), static_cast<ful::unit_utf8>('a'));

			REQUIRE(func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2), reinterpret_cast<const ful::byte *>(buffer2.end())));
			meter.measure([&](int){ return func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2), reinterpret_cast<const ful::byte *>(buffer2.end())); });
		}
	};
}

TEST_CASE("compare equal", "[compare]")
{
	BENCHMARK_DUMP("plot/compare_equal.dump", log_style, 1, 17)
	{
		const auto setup_and_run = setup_compare_equal{};

#if HAVE_ASMLIB
		BENCHMARK_GROUP("strcmp (asmlib)")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				if (!ful_expect(*end1 == ful::byte{}))
					return false;
				if (!ful_expect(*end2 == ful::byte{}))
					return false;

				return A_strcmp(reinterpret_cast<const char *>(beg1), reinterpret_cast<const char *>(beg2)) == 0;
			});
		};

		BENCHMARK_GROUP("memcmp (asmlib)")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				if (!ful_expect(end1 - beg1 == end2 - beg2))
					return false;

				return A_memcmp(beg1, beg2, end1 - beg1) == 0;
			});
		};
#endif

#if HAVE_EASTL
		BENCHMARK_GROUP("string8 operator == (eastl)")(Catch::Benchmark::Groupometer meter)
		{
			// note cannot unalign
			eastl::string8 string1(meter.size(), 'a');
			eastl::string8 string2(meter.size(), 'a');

			REQUIRE(eastl::operator ==(string1, string2)); // note cannot write string1 == string2 since catch's usage of std::begin clashes with eastl::begin
			meter.measure([&](int){ return string1 == string2; });
		};

		BENCHMARK_GROUP("string8 compare (eastl)")(Catch::Benchmark::Groupometer meter)
		{
			// note cannot unalign
			eastl::string8 string1(meter.size(), 'a');

			buffer_utf8 buffer2;
			buffer2.allocate(meter.size() + setup_and_run.unalignment2);

			std::fill(buffer2.beg(), buffer2.end(), static_cast<ful::unit_utf8>('a'));

			REQUIRE(string1.compare(reinterpret_cast<const char *>(buffer2.beg() + setup_and_run.unalignment2)) == 0);
			meter.measure([&](int){ return string1.compare(reinterpret_cast<const char *>(buffer2.beg() + setup_and_run.unalignment2)) == 0; });
		};
#endif

		BENCHMARK_GROUP("equal\\_cstr")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				if (!ful_expect(*end2 == ful::byte{}))
					return false;

				return ful::equal(beg1, end1, beg2);
			});
		};

		BENCHMARK_GROUP("equal\\_range")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				return ful::equal(beg1, end1, beg2, end2);
			});
		};
	}
}

TEST_CASE("dump memcopy", "[.][dump]")
{
#if defined(__AVX__)
	constexpr int size_first = 7;
#elif defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
	constexpr int size_first = 6;
#else
	constexpr int size_first = 5;
#endif

	BENCHMARK_DUMP("plot/memcopy.dump", log_style, size_first, 29)
	{
		BENCHMARK_GROUP("repmovf")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer_from;
			buffer_from.allocate(meter.size());

			buffer_utf8 buffer_to;
			buffer_to.allocate(meter.size());

			std::fill(buffer_from.beg(), buffer_from.end(), static_cast<ful::unit_utf8>('a'));

			meter.measure([&](int)
			{
				const ful::char8 * const first = reinterpret_cast<const ful::char8 *>(buffer_from.beg());
				const ful::char8 * const last = reinterpret_cast<const ful::char8 *>(buffer_from.end());
				ful::char8 * const begin = reinterpret_cast<ful::char8 *>(buffer_to.beg());
				const ful::usize size = last - first;

				const int alignment_offset = 8 - static_cast<int>(reinterpret_cast<ful::puint>(begin) & (8 - 1));

				*reinterpret_cast<ful::uint64 *>(begin) = *reinterpret_cast<const ful::uint64 *>(first);

				ful::detail::repmovf(reinterpret_cast<const ful::uint64 *>(first + alignment_offset), (size - alignment_offset) / 8, reinterpret_cast<ful::uint64 *>(begin + alignment_offset));

				ful::char8 * const end = begin + size;
				*reinterpret_cast<ful::uint64 *>(end - 8) = *reinterpret_cast<const ful::uint64 *>(last - 8);

				return end;
			});
		};

#if defined(__AVX__)
		BENCHMARK_GROUP("avx")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer_from;
			buffer_from.allocate(meter.size());

			buffer_utf8 buffer_to;
			buffer_to.allocate(meter.size());

			std::fill(buffer_from.beg(), buffer_from.end(), static_cast<ful::unit_utf8>('a'));

			meter.measure([&](int){ return ful::detail::memcopy_avx(reinterpret_cast<const ful::byte *>(buffer_from.beg()), reinterpret_cast<const ful::byte *>(buffer_from.end()), reinterpret_cast<ful::byte *>(buffer_to.data())); });
		};
#endif
	}
}

namespace
{
	struct setup_equal_cstr
	{
		const int unalignment1 = 0;
		const int unalignment2 = 1;

		template <typename F>
		void operator () (Catch::Benchmark::Groupometer meter, F && func) const
		{
			buffer_utf8 buffer1;
			buffer1.allocate(meter.size() + unalignment1);

			buffer_utf8 buffer2;
			buffer2.allocate(meter.size() + unalignment2);

			std::fill(buffer1.beg(), buffer1.end(), static_cast<ful::unit_utf8>('a'));
			std::fill(buffer2.beg(), buffer2.end(), static_cast<ful::unit_utf8>('a'));

			REQUIRE(func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2)));
			meter.measure([&](int){ return func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2)); });
		};
	};
}

#if defined(_MSC_VER)
namespace ful
{
	namespace detail
	{
		extern bool equal8_cstr_x86_8(const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2);
	}
}
#endif

TEST_CASE("dump equal_cstr", "[.][dump]")
{
	BENCHMARK_DUMP("plot/equal_cstr.dump", log_style, 1, 17)
	{
		const auto setup_and_run = setup_equal_cstr{};

		BENCHMARK_GROUP("for byte")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				for (ful::usize i = 0; i < size; i++)
				{
					if (beg1[i] != beg2[i])
						return false;
				}
				return beg2[size] == ful::byte{};
			});
		};

		BENCHMARK_GROUP("for unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::byte * end1_line = end1 - 8;
				if (beg1 <= end1_line)
				{
					do
					{
						if (beg1[0] != beg2[0]) return false;
						if (beg1[1] != beg2[1]) return false;
						if (beg1[2] != beg2[2]) return false;
						if (beg1[3] != beg2[3]) return false;
						if (beg1[4] != beg2[4]) return false;
						if (beg1[5] != beg2[5]) return false;
						if (beg1[6] != beg2[6]) return false;
						if (beg1[7] != beg2[7]) return false;

						beg1 += 8;
						beg2 += 8;
					}
					while (beg1 <= end1_line);
				}

				switch (beg1 - end1_line)
				{
				case 1: if (end1[-7] != *beg2) return false; beg2++; ful_fallthrough;
				case 2: if (end1[-6] != *beg2) return false; beg2++; ful_fallthrough;
				case 3: if (end1[-5] != *beg2) return false; beg2++; ful_fallthrough;
				case 4: if (end1[-4] != *beg2) return false; beg2++; ful_fallthrough;
				case 5: if (end1[-3] != *beg2) return false; beg2++; ful_fallthrough;
				case 6: if (end1[-2] != *beg2) return false; beg2++; ful_fallthrough;
				case 7: if (end1[-1] != *beg2) return false; beg2++; ful_fallthrough;
				case 8: return *beg2 == ful::byte{};
				default: ful_unreachable();
				}
			});
		};

		BENCHMARK_GROUP("for unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::byte * end1_line = end1 - 16;
				if (beg1 <= end1_line)
				{
					do
					{
						if (beg1[0] != beg2[0]) return false;
						if (beg1[1] != beg2[1]) return false;
						if (beg1[2] != beg2[2]) return false;
						if (beg1[3] != beg2[3]) return false;
						if (beg1[4] != beg2[4]) return false;
						if (beg1[5] != beg2[5]) return false;
						if (beg1[6] != beg2[6]) return false;
						if (beg1[7] != beg2[7]) return false;
						if (beg1[8] != beg2[8]) return false;
						if (beg1[9] != beg2[9]) return false;
						if (beg1[10] != beg2[10]) return false;
						if (beg1[11] != beg2[11]) return false;
						if (beg1[12] != beg2[12]) return false;
						if (beg1[13] != beg2[13]) return false;
						if (beg1[14] != beg2[14]) return false;
						if (beg1[15] != beg2[15]) return false;

						beg1 += 16;
						beg2 += 16;
					}
					while (beg1 <= end1_line);
				}

				switch (beg1 - end1_line)
				{
				case 1: if (end1[-15] != *beg2) return false; beg2++; ful_fallthrough;
				case 2: if (end1[-14] != *beg2) return false; beg2++; ful_fallthrough;
				case 3: if (end1[-13] != *beg2) return false; beg2++; ful_fallthrough;
				case 4: if (end1[-12] != *beg2) return false; beg2++; ful_fallthrough;
				case 5: if (end1[-11] != *beg2) return false; beg2++; ful_fallthrough;
				case 6: if (end1[-10] != *beg2) return false; beg2++; ful_fallthrough;
				case 7: if (end1[-9] != *beg2) return false; beg2++; ful_fallthrough;
				case 8: if (end1[-8] != *beg2) return false; beg2++; ful_fallthrough;
				case 9: if (end1[-7] != *beg2) return false; beg2++; ful_fallthrough;
				case 10: if (end1[-6] != *beg2) return false; beg2++; ful_fallthrough;
				case 11: if (end1[-5] != *beg2) return false; beg2++; ful_fallthrough;
				case 12: if (end1[-4] != *beg2) return false; beg2++; ful_fallthrough;
				case 13: if (end1[-3] != *beg2) return false; beg2++; ful_fallthrough;
				case 14: if (end1[-2] != *beg2) return false; beg2++; ful_fallthrough;
				case 15: if (end1[-1] != *beg2) return false; beg2++; ful_fallthrough;
				case 16: return *beg2 == ful::byte{};
				default: ful_unreachable();
				}
			});
		};

		BENCHMARK_GROUP("for qword")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 8)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }

					const ful::usize end_qword = size - 8;
					if (index < end_qword)
					{
						do
						{
							if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index))
								return false;

							index += 8;
						}
						while (index < end_qword);
					}

					if (*reinterpret_cast<const ful::char64 *>(beg1 + end_qword) != *reinterpret_cast<const ful::char64 *>(beg2 + end_qword))
						return false;

					return beg2[end_qword + 8] == ful::byte{};
				}
			});
		};

#if defined(_MSC_VER)
		BENCHMARK_GROUP("repe cmpsb")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				return ful::detail::equal8_cstr_x86_8(beg1, end1, beg2);
			});
		};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		BENCHMARK_GROUP("sse")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 16)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

					const ful::usize end_line = size - 16;
					if (index < end_line)
					{
						do
						{
							const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
							const __m128 line2 = _mm_load_ps(reinterpret_cast<const float *>(beg2 + index));
							const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
							const unsigned int mask = _mm_movemask_ps(cmpeq);
							if (mask != 0x0000000f)
								return false;

							index += 16;
						}
						while (index < end_line);
					}

					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + end_line));
					const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + end_line));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const unsigned int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;

					return beg2[end_line + 16] == ful::byte{};
				}
			});
		};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 16)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

					const ful::usize end_line = size - 16;
					if (index < end_line)
					{
						do
						{
							const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
							const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
							const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
							const unsigned int mask = _mm_movemask_epi8(cmpeq);
							if (mask != 0x0000ffff)
								return false;

							index += 16;
						}
						while (index < end_line);
					}

					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;

					return beg2[end_line + 16] == ful::byte{};
				}
			});
		};
#endif

#if defined(__SSE4_1__) || (defined(_MSC_VER) && defined(__AVX__))
		BENCHMARK_GROUP("sse4.1")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 16)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }

					const ful::usize end_line = size - 16;
					if (index < end_line)
					{
						do
						{
							const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
							const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
							const __m128i line_xor = _mm_xor_si128(line1, line2);
							if (_mm_testz_si128(line_xor, line_xor) == 0)
								return false;

							index += 16;
						}
						while (index < end_line);
					}

					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + end_line));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + end_line));
					const __m128i line_xor = _mm_xor_si128(line1, line2);
					if (_mm_testz_si128(line_xor, line_xor) == 0)
						return false;

					return beg2[end_line + 16] == ful::byte{};
				}
			});
		};
#endif

#if defined(__AVX__)
		BENCHMARK_GROUP("avx")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 32)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x10)
					{
						const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
						const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
						const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
						const unsigned int mask = _mm_movemask_epi8(cmpeq);
						if (mask != 0x0000ffff)
							return false;

						index += 16;
					}

					const ful::usize end_line = size - 32;
					if (index < end_line)
					{
						do
						{
							const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
							const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(beg2 + index));
							const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
							if (_mm256_testz_si256(line_xor, line_xor) == 0)
								return false;

							index += 32;
						}
						while (index < end_line);
					}

					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_line));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_line));
					const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
					if (_mm256_testz_si256(line_xor, line_xor) == 0)
						return false;

					return beg2[end_line + 32] == ful::byte{};
				}
			});
		};
#endif

#if defined(__AVX2__)
		BENCHMARK_GROUP("avx2")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2)
			{
				const ful::usize size = end1 - beg1;
				if (size <= 32)
				{
					ful::usize index = 0;

					switch (size)
					{
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
					ful::usize index = 0;
					if (reinterpret_cast<puint>(beg2 + index) & 0x1) { if (*reinterpret_cast<const ful::char8 *>(beg1 + index) != *reinterpret_cast<const ful::char8 *>(beg2 + index)) return false; index += 1; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x2) { if (*reinterpret_cast<const ful::char16 *>(beg1 + index) != *reinterpret_cast<const ful::char16 *>(beg2 + index)) return false; index += 2; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x4) { if (*reinterpret_cast<const ful::char32 *>(beg1 + index) != *reinterpret_cast<const ful::char32 *>(beg2 + index)) return false; index += 4; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x8) { if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index)) return false; index += 8; }
					if (reinterpret_cast<puint>(beg2 + index) & 0x10)
					{
						const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
						const __m128i line2 = _mm_load_si128(reinterpret_cast<const __m128i *>(beg2 + index));
						const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
						const unsigned int mask = _mm_movemask_epi8(cmpeq);
						if (mask != 0x0000ffff)
							return false;

						index += 16;
					}

					const ful::usize end_line = size - 32;
					if (index < end_line)
					{
						do
						{
							const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
							const __m256i line2 = _mm256_load_si256(reinterpret_cast<const __m256i *>(beg2 + index));
							const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
							const unsigned int mask = _mm256_movemask_epi8(line_cmpeq);
							if (mask != 0xffffffff)
								return false;

							index += 32;
						}
						while (index < end_line);
					}

					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + end_line));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + end_line));
					const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm256_movemask_epi8(line_cmpeq);
					if (mask != 0xffffffff)
						return false;

					return beg2[end_line + 32] == ful::byte{};
				}
			});
		};
#endif
	}
}

namespace
{
	struct setup_equal_range
	{
		const int unalignment1 = 0;
		const int unalignment2 = 1;

		template <typename F>
		void operator () (Catch::Benchmark::Groupometer meter, F && func) const
		{
			buffer_utf8 buffer1;
			buffer1.allocate(meter.size() + unalignment1);

			buffer_utf8 buffer2;
			buffer2.allocate(meter.size() + unalignment2);

			std::fill(buffer1.beg(), buffer1.end(), static_cast<ful::unit_utf8>('a'));
			std::fill(buffer2.beg(), buffer2.end(), static_cast<ful::unit_utf8>('a'));

			REQUIRE(func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2), reinterpret_cast<const ful::byte *>(buffer2.end())));
			meter.measure([&](int){ return func(reinterpret_cast<const ful::byte *>(buffer1.beg() + unalignment1), reinterpret_cast<const ful::byte *>(buffer1.end()), reinterpret_cast<const ful::byte *>(buffer2.beg() + unalignment2), reinterpret_cast<const ful::byte *>(buffer2.end())); });
		};
	};
}

TEST_CASE("dump equal_range", "[.][dump]")
{
	BENCHMARK_DUMP("plot/equal_range.dump", log_style, 1, 17)
	{
		const auto setup_and_run = setup_equal_range{};

		BENCHMARK_GROUP("for byte")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				for (ful::usize i = 0; i < size; i++)
				{
					if (beg1[i] != beg2[i])
						return false;
				}
				return true;
			});
		};

		BENCHMARK_GROUP("for unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				ful::usize index = 8;
				if (index <= size)
				{
					do
					{
						if (beg1[index - 8] != beg2[index - 8]) return false;
						if (beg1[index - 7] != beg2[index - 7]) return false;
						if (beg1[index - 6] != beg2[index - 6]) return false;
						if (beg1[index - 5] != beg2[index - 5]) return false;
						if (beg1[index - 4] != beg2[index - 4]) return false;
						if (beg1[index - 3] != beg2[index - 3]) return false;
						if (beg1[index - 2] != beg2[index - 2]) return false;
						if (beg1[index - 1] != beg2[index - 1]) return false;

						// todo gcc generates 3 adds for this one add
						index += 8;
					}
					while (index <= size);
				}

				switch (index - size)
				{
				case 1: if (end1[-7] != end2[-7]) return false; ful_fallthrough;
				case 2: if (end1[-6] != end2[-6]) return false; ful_fallthrough;
				case 3: if (end1[-5] != end2[-5]) return false; ful_fallthrough;
				case 4: if (end1[-4] != end2[-4]) return false; ful_fallthrough;
				case 5: if (end1[-3] != end2[-3]) return false; ful_fallthrough;
				case 6: if (end1[-2] != end2[-2]) return false; ful_fallthrough;
				case 7: if (end1[-1] != end2[-1]) return false; ful_fallthrough;
				case 8: return true;
				default: ful_unreachable();
				}
			});
		};

		BENCHMARK_GROUP("for unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				ful::usize index = 16;
				if (index <= size)
				{
					do
					{
						if (beg1[index - 16] != beg2[index - 16]) return false;
						if (beg1[index - 15] != beg2[index - 15]) return false;
						if (beg1[index - 14] != beg2[index - 14]) return false;
						if (beg1[index - 13] != beg2[index - 13]) return false;
						if (beg1[index - 12] != beg2[index - 12]) return false;
						if (beg1[index - 11] != beg2[index - 11]) return false;
						if (beg1[index - 10] != beg2[index - 10]) return false;
						if (beg1[index - 9] != beg2[index - 9]) return false;
						if (beg1[index - 8] != beg2[index - 8]) return false;
						if (beg1[index - 7] != beg2[index - 7]) return false;
						if (beg1[index - 6] != beg2[index - 6]) return false;
						if (beg1[index - 5] != beg2[index - 5]) return false;
						if (beg1[index - 4] != beg2[index - 4]) return false;
						if (beg1[index - 3] != beg2[index - 3]) return false;
						if (beg1[index - 2] != beg2[index - 2]) return false;
						if (beg1[index - 1] != beg2[index - 1]) return false;

						// todo gcc generates 3 adds for this one add
						index += 16;
					}
					while (index <= size);
				}

				switch (index - size)
				{
				case 1: if (end1[-15] != end2[-15]) return false; ful_fallthrough;
				case 2: if (end1[-14] != end2[-14]) return false; ful_fallthrough;
				case 3: if (end1[-13] != end2[-13]) return false; ful_fallthrough;
				case 4: if (end1[-12] != end2[-12]) return false; ful_fallthrough;
				case 5: if (end1[-11] != end2[-11]) return false; ful_fallthrough;
				case 6: if (end1[-10] != end2[-10]) return false; ful_fallthrough;
				case 7: if (end1[-9] != end2[-9]) return false; ful_fallthrough;
				case 8: if (end1[-8] != end2[-8]) return false; ful_fallthrough;
				case 9: if (end1[-7] != end2[-7]) return false; ful_fallthrough;
				case 10: if (end1[-6] != end2[-6]) return false; ful_fallthrough;
				case 11: if (end1[-5] != end2[-5]) return false; ful_fallthrough;
				case 12: if (end1[-4] != end2[-4]) return false; ful_fallthrough;
				case 13: if (end1[-3] != end2[-3]) return false; ful_fallthrough;
				case 14: if (end1[-2] != end2[-2]) return false; ful_fallthrough;
				case 15: if (end1[-1] != end2[-1]) return false; ful_fallthrough;
				case 16: return true;
				default: ful_unreachable();
				}
			});
		};

		BENCHMARK_GROUP("for qword")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 8)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 8;

					ful::usize index = 0;
					do
					{
						if (*reinterpret_cast<const ful::char64 *>(beg1 + index) != *reinterpret_cast<const ful::char64 *>(beg2 + index))
							return false;

						index += 8;
					}
					while (index < end_size);

					if (*reinterpret_cast<const ful::char64 *>(end1 - 8) != *reinterpret_cast<const ful::char64 *>(end2 - 8))
						return false;

					return true;
				}
			});
		};

#if defined(_MSC_VER)
		BENCHMARK_GROUP("repe cmpsb")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				return ful::detail::equal8_cstr_x86_8(beg1, end1, beg2);
			});
		};
#endif

#if defined(__SSE__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)))
		BENCHMARK_GROUP("sse")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 16)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 16;

					ful::usize index = 0;
					do
					{
						const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(beg1 + index));
						const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(beg2 + index));
						const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
						const unsigned int mask = _mm_movemask_ps(cmpeq);
						if (mask != 0x0000000f)
							return false;

						index += 16;
					}
					while (index < end_size);

					const __m128 line1 = _mm_loadu_ps(reinterpret_cast<const float *>(end1 - 16));
					const __m128 line2 = _mm_loadu_ps(reinterpret_cast<const float *>(end2 - 16));
					const __m128 cmpeq = _mm_cmpeq_ps(line1, line2);
					const unsigned int mask = _mm_movemask_ps(cmpeq);
					if (mask != 0x0000000f)
						return false;

					return true;
				}
			});
		};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 16)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 16;

					ful::usize index = 0;
					do
					{
						const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
						const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index));
						const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
						const unsigned int mask = _mm_movemask_epi8(cmpeq);
						if (mask != 0x0000ffff)
							return false;

						index += 16;
					}
					while (index < end_size);

					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
					const __m128i cmpeq = _mm_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm_movemask_epi8(cmpeq);
					if (mask != 0x0000ffff)
						return false;

					return true;
				}
			});
		};
#endif

#if defined(__SSE4_1__) || (defined(_MSC_VER) && defined(__AVX__))
		BENCHMARK_GROUP("sse4.1")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 16)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 16;

					ful::usize index = 0;
					do
					{
						const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg1 + index));
						const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(beg2 + index));
						const __m128i line_xor = _mm_xor_si128(line1, line2);
						if (_mm_testz_si128(line_xor, line_xor) == 0)
							return false;

						index += 16;
					}
					while (index < end_size);

					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end1 - 16));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end2 - 16));
					const __m128i line_xor = _mm_xor_si128(line1, line2);
					if (_mm_testz_si128(line_xor, line_xor) == 0)
						return false;

					return true;
				}
			});
		};
#endif

#if defined(__AVX__)
		BENCHMARK_GROUP("avx")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 32)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 32;

					ful::usize index = 0;
					do
					{
						const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
						const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
						const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
						if (_mm256_testz_si256(line_xor, line_xor) == 0)
							return false;

						index += 32;
					}
					while (index < end_size);

					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end1 - 32));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end2 - 32));
					const __m256i line_xor = _mm256_castps_si256(_mm256_xor_ps(_mm256_castsi256_ps(line1), _mm256_castsi256_ps(line2)));
					if (_mm256_testz_si256(line_xor, line_xor) == 0)
						return false;

					return true;
				}
			});
		};
#endif

#if defined(__AVX2__)
		BENCHMARK_GROUP("avx2")(Catch::Benchmark::Groupometer meter)
		{
			setup_and_run(meter, [](const ful::byte * beg1, const ful::byte * end1, const ful::byte * beg2, const ful::byte * end2)
			{
				const ful::usize size = end1 - beg1;
				if (size != static_cast<ful::usize>(end2 - beg2))
					return false;

				if (size <= 32)
				{
					switch (size)
					{
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
					const ful::usize end_size = size - 32;

					ful::usize index = 0;
					do
					{
						const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg1 + index));
						const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(beg2 + index));
						const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
						const unsigned int mask = _mm256_movemask_epi8(line_cmpeq);
						if (mask != 0xffffffff)
							return false;

						index += 32;
					}
					while (index < end_size);

					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end1 - 32));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end2 - 32));
					const __m256i line_cmpeq = _mm256_cmpeq_epi8(line1, line2);
					const unsigned int mask = _mm256_movemask_epi8(line_cmpeq);
					if (mask != 0xffffffff)
						return false;

					return true;
				}
			});
		};
#endif
	}
}
