#include "ful/types.hpp"

#include "ful/string.hpp"

#include "buffer.hpp"

#include <catch2/catch.hpp>

#include <vector>

#if HAVE_ASMLIB
# include <asmlib.h>
#endif

using namespace ful;
using namespace ful::detail;

namespace
{
#if defined(__SSE2__)

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

TEST_CASE("rotate", "")
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
			const __m256i x1 = rotate(x, 1);
			const __m256i x15 = rotate(x, 15);
			const __m256i x16 = rotate(x, 16);
			const __m256i x31 = rotate(x, 31);
			CHECK(std::memcmp(&x1, &y1, 32) == 0);
			CHECK(std::memcmp(&x15, &y15, 32) == 0);
			CHECK(std::memcmp(&x16, &y16, 32) == 0);
			CHECK(std::memcmp(&x31, &y31, 32) == 0);

			meter.measure([&](int n){ return rotate(x, static_cast<unsigned int>(n * 4711) & (32 - 1)); });
		};
	}
#endif

#if defined(__SSE2__)
	{
		const __m128i x = _mm_set_epi8(16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
		const __m128i y1 = _mm_set_epi8(1, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2);
		const __m128i y7 = _mm_set_epi8(7, 6, 5, 4, 3, 2, 1, 16, 15, 14, 13, 12, 11, 10, 9, 8);
		const __m128i y8 = _mm_set_epi8(8, 7, 6, 5, 4, 3, 2, 1, 16, 15, 14, 13, 12, 11, 10, 9);
		const __m128i y15 = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 16);

		BENCHMARK_ADVANCED("rotate sse2")(Catch::Benchmark::Chronometer meter)
		{
			const __m128i x1 = rotate(x, 1);
			const __m128i x7 = rotate(x, 7);
			const __m128i x8 = rotate(x, 8);
			const __m128i x15 = rotate(x, 15);
			CHECK(std::memcmp(&x1, &y1, 16) == 0);
			CHECK(std::memcmp(&x7, &y7, 16) == 0);
			CHECK(std::memcmp(&x8, &y8, 16) == 0);
			CHECK(std::memcmp(&x15, &y15, 16) == 0);

			meter.measure([&](int n){ return rotate(x, static_cast<unsigned int>(n * 4711) & (16 - 1)); });
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
	unit_utf8 * copy_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
	{
		for (; first != last;)
		{
			*begin = *first;
			++first, ++begin;
		}
		return begin;
	}
}

TEST_CASE("copy", "")
{
	auto txt = read_utf8("data/jap.txt");

	const auto first = txt.beg() + 3;
	const auto last = txt.end();

	BENCHMARK_ADVANCED("copy (std)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;
		auto to = tyt.end();

		REQUIRE(std::copy(first, last, from) == to);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return std::copy(first, last, from); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("copy (asmlib memcpy)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;

		REQUIRE(A_memcpy(from, first, last - first) == from);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return A_memcpy(from, first, last - first) == 0; });
	};
#endif

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("copy (asmlib strcpy)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;

		REQUIRE(A_strcpy(from, first) == from);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return A_strcpy(from, first) == 0; });
	};
#endif

	BENCHMARK_ADVANCED("copy")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;
		auto to = tyt.end();

		REQUIRE(copy(first, last, from) == to);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy(first, last, from); });
	};

	BENCHMARK_ADVANCED("copy large none")(Catch::Benchmark::Chronometer meter)
	{
		if (last - first < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;
		auto to = tyt.end();

		REQUIRE(copy_large_none(first, last, from) == to);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy_large_none(first, last, from); });
	};

#if defined(__AVX__)
	BENCHMARK_ADVANCED("copy large avx")(Catch::Benchmark::Chronometer meter)
	{
		if (last - first < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;
		auto to = tyt.end();

		REQUIRE(copy_large_avx(first, last, from) == to);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy_large_avx(first, last, from); });
	};
#endif

	BENCHMARK_ADVANCED("copy (naive)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(last - first + 1);

		auto from = tyt.data() + 1;
		auto to = tyt.end();

		REQUIRE(copy_alt_naive(first, last, from) == to);
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy_alt_naive(first, last, from); });
	};
}

namespace
{
	inline unit_utf8 * rcopy_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
	{
		for (; first != last;)
		{
			--last, --end;
			*end = *last;
		}
		return end;
	}
}

TEST_CASE("rcopy", "")
{
	auto txt = read_utf8("data/jap.txt");

	const auto before_offset = 1;
	const auto after_offset = 5;

	BENCHMARK_ADVANCED("rcopy (std copy_backward)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(std::copy_backward(first, last, end) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return std::copy_backward(first, last, end); });
	};

	BENCHMARK_ADVANCED("rcopy (std memmove)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto begin = tyt.data() + after_offset;

		REQUIRE(std::memmove(begin, first, txt.size()) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return std::memmove(begin, first, txt.size()); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("rcopy (asmlib memmove)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto begin = tyt.data() + after_offset;

		REQUIRE(A_memmove(begin, first, txt.size()) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return A_memmove(begin, first, txt.size()); });
	};
#endif

	BENCHMARK_ADVANCED("rcopy")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy(first, last, end) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy(first, last, end); });
	};

	BENCHMARK_ADVANCED("rcopy large none")(Catch::Benchmark::Chronometer meter)
	{
		if (txt.size() < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy_large_none(first, last, end) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy_large_none(first, last, end); });
	};

#if defined(__AVX__)
	BENCHMARK_ADVANCED("rcopy large avx")(Catch::Benchmark::Chronometer meter)
	{
		if (txt.size() < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy_large_avx(first, last, end) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy_large_avx(first, last, end); });
	};
#endif

	BENCHMARK_ADVANCED("rcopy (naive)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy_alt_naive(first, last, end) == begin);
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy_alt_naive(first, last, end); });
	};
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
}

TEST_CASE("fill", "")
{
	const unsigned long long size = 4711;
	const unsigned int offset = 1;

	BENCHMARK_ADVANCED("fill (std)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return std::fill(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("fill (asmlib memset)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return A_memset(tyt.data() + offset, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1)), size); });
	};
#endif

	BENCHMARK_ADVANCED("fill")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); });
	};

	BENCHMARK_ADVANCED("fill large none")(Catch::Benchmark::Chronometer meter)
	{
		if (size < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill_large_none(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); });
	};

#if defined(__AVX__)
	BENCHMARK_ADVANCED("fill large avx")(Catch::Benchmark::Chronometer meter)
	{
		if (size < 16)
			return;

		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill_large_avx(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); });
	};
#endif

	BENCHMARK_ADVANCED("fill (naive)")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill_alt_naive(tyt.data() + offset, tyt.data() + offset + size, static_cast<unit_utf8>('a' + n % ('z' - 'a' + 1))); });
	};
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

	const unit_utf8 * point_prev_alt_naive(const unit_utf8 * s, int n)
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

	const unit_utf8 * point_prev_avx2_alt_naive(const unit_utf8 * s, int n)
	{
		for (; 0 < n; --n)
		{
			s = point_prev_avx2_alt_naive(s);
		}
		return s;
	}

#endif

#if defined(__SSE2__)

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

	const unit_utf8 * point_prev_sse2_alt_naive(const unit_utf8 * s, int n)
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
	auto txt = read_utf8("data/jap.txt");
	const auto npoints = 1742;

	BENCHMARK_ADVANCED("point_prev (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_alt_naive(txt.end(), npoints) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_alt_naive(txt.end(), npoints); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("point_prev avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_avx2(txt.end(), npoints) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_avx2(txt.end(), npoints); });
	};

	BENCHMARK_ADVANCED("point_prev avx2 (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_avx2_alt_naive(txt.end(), npoints) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_avx2_alt_naive(txt.end(), npoints); });
	};
#endif

#if defined(__SSE2__)
	BENCHMARK_ADVANCED("point_prev sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_sse2(txt.end(), npoints) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_sse2(txt.end(), npoints); });
	};

	BENCHMARK_ADVANCED("point_prev sse2 (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_sse2_alt_naive(txt.end(), npoints) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_sse2_alt_naive(txt.end(), npoints); });
	};
#endif
}

TEST_CASE("equal_cstr", "")
{
	auto txt = read_utf8("data/jap.txt");
	auto tyt = read_utf8("data/jap.txt");

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
		REQUIRE(equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__)
	BENCHMARK_ADVANCED("equal_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(equal_cstr_sse2(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal_cstr_sse2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if RUNTIME_CPUID
	BENCHMARK_ADVANCED("equal_cstr dispatch")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(equal(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

	BENCHMARK_ADVANCED("equal_cstr none")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
	};
}

TEST_CASE("find_unit", "")
{
	auto txt = read_utf8("data/jap.txt");

	BENCHMARK_ADVANCED("find_unit (std strchr)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(std::strchr(txt.cstr(), '$') == txt.end() - 2);
		meter.measure([&](int){ return std::strchr(txt.cstr(), '$'); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("find_unit (asmlib strstr)")(Catch::Benchmark::Chronometer meter)
	{
		// note asmlib does not have strchr
		REQUIRE(A_strstr(txt.cstr(), u8"$") == txt.end() - 2);
		meter.measure([&](int){ return A_strstr(txt.cstr(), u8"$"); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit avx2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(find_unit_avx2(txt.beg(), txt.end(), '$') == txt.end() - 2);
		meter.measure([&](int){ return find_unit_avx2(txt.beg(), txt.end(), '$'); });
	};
#endif

#if defined(__SSE2__)
	BENCHMARK_ADVANCED("find_unit sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(find_unit_sse2(txt.beg(), txt.end(), '$') == txt.end() - 2);
		meter.measure([&](int){ return find_unit_sse2(txt.beg(), txt.end(), '$'); });
	};
#endif
}

TEST_CASE("less_cstr", "")
{
	auto txt = read_utf8("data/jap.txt");
	auto tyt = read_utf8("data/jap.txt");
	REQUIRE(*(txt.end() - 1) == '\n');
	txt.reduce(txt.end() - 1); // remove newline, it should now be less than tyt

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
		REQUIRE(less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less_cstr_avx2(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__)
	BENCHMARK_ADVANCED("less_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(less_cstr_sse2(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less_cstr_sse2(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less_cstr_sse2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if RUNTIME_CPUID
	BENCHMARK_ADVANCED("less_cstr dispatch")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(less(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

	BENCHMARK_ADVANCED("less_cstr none")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(less_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less_cstr_none(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
	};
}
