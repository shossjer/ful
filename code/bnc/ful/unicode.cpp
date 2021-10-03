#include "ful/unicode.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

using namespace ful;

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
