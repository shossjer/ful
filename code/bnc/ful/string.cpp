#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "ful/point_utils.hpp"

#include "ful/string.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

#include <vector>

#if HAVE_ASMLIB
# include <asmlib.h>
#endif

using namespace ful;
using namespace ful::detail;

namespace
{
#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
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

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
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
	data_any_utf8 txt;
	REQUIRE(txt.init());

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

		REQUIRE(copy_large_none(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(from)) == reinterpret_cast<char8 *>(to));
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy_large_none(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(from)); });
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

		REQUIRE(copy_large_avx(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(from)) == reinterpret_cast<char8 *>(to));
		REQUIRE(std::memcmp(first, from, last - first) == 0);
		meter.measure([&](int){ return copy_large_avx(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(from)); });
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
	data_any_utf8 txt;
	REQUIRE(txt.init());

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
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy_large_none(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)) == reinterpret_cast<char8 *>(begin));
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy_large_none(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)); });
	};

#if defined(__AVX__)
	BENCHMARK_ADVANCED("rcopy large avx")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(txt.size() + before_offset);
		std::copy(txt.beg(), txt.end(), tyt.data() + before_offset);

		auto first = tyt.data() + before_offset;
		auto last = tyt.data() + before_offset + txt.size();
		auto begin = tyt.data() + after_offset;
		auto end = tyt.data() + after_offset + txt.size();

		REQUIRE(rcopy_large_avx(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)) == reinterpret_cast<char8 *>(begin));
		REQUIRE(std::memcmp(begin, txt.beg(), txt.size()) == 0);
		meter.measure([&](int){ return rcopy_large_avx(reinterpret_cast<const char8 *>(first), reinterpret_cast<const char8 *>(last), reinterpret_cast<char8 *>(end)); });
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
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill_large_none(reinterpret_cast<char8 *>(tyt.data() + offset), reinterpret_cast<char8 *>(tyt.data() + offset + size), static_cast<char8>('a' + n % ('z' - 'a' + 1))); });
	};

#if defined(__AVX__)
	BENCHMARK_ADVANCED("fill large avx")(Catch::Benchmark::Chronometer meter)
	{
		buffer_utf8 tyt;
		tyt.allocate(offset + size);

		meter.measure([&](int n){ return fill_large_avx(reinterpret_cast<char8 *>(tyt.data() + offset), reinterpret_cast<char8 *>(tyt.data() + offset + size), static_cast<char8>('a' + n % ('z' - 'a' + 1))); });
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
	const unit_utf8 * point_next_alt_naive(const unit_utf8 * s)
	{
		++s;
		if (static_cast<signed char>(*s) > -65)
			return s;

		++s;
		if (static_cast<signed char>(*s) > -65)
			return s;

		++s;
		if (static_cast<signed char>(*s) > -65)
			return s;

		++s;
		return s;
	}

	const unit_utf8 * point_next_alt_naive(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_naive(s);
		}
		return s;
	}

	const unit_utf8 * point_next_alt_size(const unit_utf8 * s)
	{
		return s + point_size(s);
	}

	const unit_utf8 * point_next_alt_size(const unit_utf8 * s, usize n)
	{
		for (; 0 < n; --n)
		{
			s = point_next_alt_size(s);
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
			const unsigned int npoints = popcnt(mask);

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

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
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
			const unsigned int npoints = popcnt(mask);

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

	BENCHMARK_ADVANCED("point_next (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_naive(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_naive(txt.beg(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_next (size)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_alt_size(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_alt_size(txt.beg(), txt.npoints()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("point_next (avx2)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_next_avx2(txt.beg(), txt.npoints()) - txt.end() == 0);
		meter.measure([&](int){ return point_next_avx2(txt.beg(), txt.npoints()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
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

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
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
		REQUIRE(point_prev_avx2(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_avx2(txt.end(), txt.npoints()); });
	};

	BENCHMARK_ADVANCED("point_prev avx2 (naive)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_avx2_alt_naive(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_avx2_alt_naive(txt.end(), txt.npoints()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
	BENCHMARK_ADVANCED("point_prev sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(point_prev_sse2(txt.end(), txt.npoints()) - txt.beg() == 0);
		meter.measure([&](int){ return point_prev_sse2(txt.end(), txt.npoints()); });
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
		REQUIRE(equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
	BENCHMARK_ADVANCED("equal_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(equal_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())));
		meter.measure([&](int){ return equal_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())); });
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
		REQUIRE(equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		meter.measure([&](int){ return equal_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
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
		REQUIRE(find_unit_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), txt.cunit()) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return find_unit_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), txt.cunit()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
	BENCHMARK_ADVANCED("find_unit sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(find_unit_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), static_cast<char8>(txt.cunit())) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return find_unit_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), static_cast<char8>(txt.cunit())); });
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
		REQUIRE(less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less_cstr_avx2(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less_cstr_avx2(txt.beg(), txt.end(), tyt.cstr()); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(_M_X64) || defined(_M_AMD64)))
	BENCHMARK_ADVANCED("less_cstr sse2")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(less_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())));
		REQUIRE_FALSE(less_cstr_sse2(reinterpret_cast<const char8 *>(tyt.beg()), reinterpret_cast<const char8 *>(tyt.end()), reinterpret_cast<const char8 *>(txt.cstr())));
		meter.measure([&](int){ return less_cstr_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), reinterpret_cast<const char8 *>(tyt.cstr())); });
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
		REQUIRE(less_cstr_none(txt.beg(), txt.end(), tyt.cstr()));
		REQUIRE_FALSE(less_cstr_none(tyt.beg(), tyt.end(), txt.cstr()));
		meter.measure([&](int){ return less_cstr_none(txt.beg(), txt.end(), tyt.cstr()); });
	};
}
