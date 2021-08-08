#include "ful/string.hpp"
#include "ful/string_search.hpp"

#include "buffer.hpp"
#include "constants.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>
#include "catchhacks.hpp"

#if HAVE_ASMLIB
# include <asmlib.h>
#endif

#if HAVE_EASTL
# include <EASTL/string.h>
#endif

#include <vector>

using namespace ful;

TEST_CASE("find_8_8", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("strchr (std)")(Catch::Benchmark::Chronometer meter)
	{
		REQUIRE(std::strchr(txt.cstr(), txt.cunit()) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return std::strchr(txt.cstr(), txt.cunit()); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("strstr (asmlib)")(Catch::Benchmark::Chronometer meter)
	{
		// note asmlib does not have strchr
		const char str[2] = {txt.cunit(), '\0'};
		REQUIRE(A_strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return A_strstr(txt.cstr(), str); });
	};

	BENCHMARK_ADVANCED("strcspn (asmlib)")(Catch::Benchmark::Chronometer meter)
	{
		// note asmlib does not have strchr
		const char str[2] = {txt.cunit(), '\0'};
		REQUIRE(A_strcspn(txt.cstr(), str) == txt.iunit());
		meter.measure([&](int){ return A_strcspn(txt.cstr(), str); });
	};
#endif

#if HAVE_EASTL
	BENCHMARK_ADVANCED("string8 find (eastl)")(Catch::Benchmark::Chronometer meter)
	{
		const eastl::string8 tst(txt.cstr(), txt.size());
		REQUIRE(tst.find(txt.cunit()) == txt.iunit());
		meter.measure([&](int){ return tst.find(txt.cunit()); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit_8_8_avx2")(Catch::Benchmark::Chronometer meter)
	{
		const char8 chr = txt.cunit();
		REQUIRE(detail::find_unit_8_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return detail::find_unit_8_8_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("find_unit_8_8_sse2")(Catch::Benchmark::Chronometer meter)
	{
		const char8 chr = txt.cunit();
		REQUIRE(detail::find_unit_8_8_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_8_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
#endif

	BENCHMARK_ADVANCED("find_unit_8_8_generic")(Catch::Benchmark::Chronometer meter)
	{
		const char8 chr = txt.cunit();
		REQUIRE(detail::find_unit_8_8_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_8_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
}

TEST_CASE("find_8_16", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("strstr (std)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[3] = {txt.cunit(), '<', '\0'};
		REQUIRE(std::strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return std::strstr(txt.cstr(), str); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("strstr (asmlib)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[3] = {txt.cunit(), '<', '\0'};
		REQUIRE(A_strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return A_strstr(txt.cstr(), str); });
	};
#endif

#if HAVE_EASTL
	BENCHMARK_ADVANCED("string8 find (eastl)")(Catch::Benchmark::Chronometer meter)
	{
		const eastl::string8 tst(txt.cstr(), txt.size());
		const char str[3] = {txt.cunit(), '<', '\0'};
		REQUIRE(tst.find(str) == txt.iunit());
		meter.measure([&](int){ return tst.find(str); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit_8_16_avx2")(Catch::Benchmark::Chronometer meter)
	{
		const char16 chr = (static_cast<uint16>('<') << 8) | static_cast<uint16>(txt.cunit());
		REQUIRE(detail::find_unit_8_16_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return detail::find_unit_8_16_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("find_unit_8_16_sse2")(Catch::Benchmark::Chronometer meter)
	{
		const char16 chr = (static_cast<uint16>('<') << 8) | static_cast<uint16>(txt.cunit());
		REQUIRE(detail::find_unit_8_16_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_16_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
#endif

	BENCHMARK_ADVANCED("find_unit_8_16_generic")(Catch::Benchmark::Chronometer meter)
	{
		const char16 chr = (static_cast<uint16>('<') << 8) | static_cast<uint16>(txt.cunit());
		REQUIRE(detail::find_unit_8_16_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_16_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
}

TEST_CASE("find_8_24", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("strstr (std)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[4] = {txt.cunit(), '<', '/', '\0'};
		REQUIRE(std::strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return std::strstr(txt.cstr(), str); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("strstr (asmlib)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[4] = {txt.cunit(), '<', '/', '\0'};
		REQUIRE(A_strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return A_strstr(txt.cstr(), str); });
	};
#endif

#if HAVE_EASTL
	BENCHMARK_ADVANCED("string8 find (eastl)")(Catch::Benchmark::Chronometer meter)
	{
		const eastl::string8 tst(txt.cstr(), txt.size());
		const char str[4] = {txt.cunit(), '<', '/', '\0'};
		REQUIRE(tst.find(str) == txt.iunit());
		meter.measure([&](int){ return tst.find(str); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit_8_24_avx2")(Catch::Benchmark::Chronometer meter)
	{
		const char_fast24 chr{(static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit())};
		REQUIRE(detail::find_unit_8_24_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return detail::find_unit_8_24_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("find_unit_8_24_sse2")(Catch::Benchmark::Chronometer meter)
	{
		const char_fast24 chr{(static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit())};
		REQUIRE(detail::find_unit_8_24_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_24_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
#endif

	BENCHMARK_ADVANCED("find_unit_8_24_generic")(Catch::Benchmark::Chronometer meter)
	{
		const char_fast24 chr{(static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit())};
		REQUIRE(detail::find_unit_8_24_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_24_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
}

TEST_CASE("find_8_32", "")
{
	data_any_utf8 txt;
	REQUIRE(txt.init());

	BENCHMARK_ADVANCED("strstr (std)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[5] = {txt.cunit(), '<', '/', 'a', '\0'};
		REQUIRE(std::strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return std::strstr(txt.cstr(), str); });
	};

#if HAVE_ASMLIB
	BENCHMARK_ADVANCED("strstr (asmlib)")(Catch::Benchmark::Chronometer meter)
	{
		const char str[5] = {txt.cunit(), '<', '/', 'a', '\0'};
		REQUIRE(A_strstr(txt.cstr(), str) == txt.beg() + txt.iunit());
		meter.measure([&](int){ return A_strstr(txt.cstr(), str); });
	};
#endif

#if HAVE_EASTL
	BENCHMARK_ADVANCED("string8 find (eastl)")(Catch::Benchmark::Chronometer meter)
	{
		const eastl::string8 tst(txt.cstr(), txt.size());
		const char str[5] = {txt.cunit(), '<', '/', 'a', '\0'};
		REQUIRE(tst.find(str) == txt.iunit());
		meter.measure([&](int){ return tst.find(str); });
	};
#endif

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("find_unit_8_32_avx2")(Catch::Benchmark::Chronometer meter)
	{
		const char32 chr = (static_cast<uint32>('a') << 24) | (static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit());
		REQUIRE(detail::find_unit_8_32_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr) == reinterpret_cast<char8 *>(txt.beg()) + txt.iunit());
		meter.measure([&](int){ return detail::find_unit_8_32_avx2(reinterpret_cast<char8 *>(txt.beg()), reinterpret_cast<char8 *>(txt.end()), chr); });
	};
#endif

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	BENCHMARK_ADVANCED("find_unit_8_32_sse2")(Catch::Benchmark::Chronometer meter)
	{
		const char32 chr = (static_cast<uint32>('a') << 24) | (static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit());
		REQUIRE(detail::find_unit_8_32_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_32_sse2(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
#endif

	BENCHMARK_ADVANCED("find_unit_8_32_generic")(Catch::Benchmark::Chronometer meter)
	{
		const char32 chr =(static_cast<uint32>('a') << 24) | (static_cast<uint32>('/') << 16) | (static_cast<uint32>('<') << 8) | static_cast<uint32>(txt.cunit());
		REQUIRE(detail::find_unit_8_32_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr) == reinterpret_cast<const char8 *>(txt.beg() + txt.iunit()));
		meter.measure([&](int){ return detail::find_unit_8_32_generic(reinterpret_cast<const char8 *>(txt.beg()), reinterpret_cast<const char8 *>(txt.end()), chr); });
	};
}

namespace
{
	const char8 * find_8_8_small_for(const char8 * begin, const char8 * end, char8 c)
	{
		for (; begin != end; begin++)
		{
			if (*begin == c)
				break;
		}
		return begin;
	}

	const char8 * find_8_8_small_if_unroll_8(const char8 * begin, const char8 * end, char8 c)
	{
		const char8 * const end_line = end - 8;
		if (begin <= end_line)
		{
			do
			{
				if (begin[0] == c) return begin + 0;
				if (begin[1] == c) return begin + 1;
				if (begin[2] == c) return begin + 2;
				if (begin[3] == c) return begin + 3;
				if (begin[4] == c) return begin + 4;
				if (begin[5] == c) return begin + 5;
				if (begin[6] == c) return begin + 6;
				if (begin[7] == c) return begin + 7;
				begin += 8;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 2: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 3: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 4: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 5: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 6: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 7: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 8: return begin;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_8_small_if_unroll_16(const char8 * begin, const char8 * end, char8 c)
	{
		const char8 * const end_line = end - 16;
		if (begin <= end_line)
		{
			do
			{
				if (begin[0] == c) return begin + 0;
				if (begin[1] == c) return begin + 1;
				if (begin[2] == c) return begin + 2;
				if (begin[3] == c) return begin + 3;
				if (begin[4] == c) return begin + 4;
				if (begin[5] == c) return begin + 5;
				if (begin[6] == c) return begin + 6;
				if (begin[7] == c) return begin + 7;
				if (begin[8] == c) return begin + 8;
				if (begin[9] == c) return begin + 9;
				if (begin[10] == c) return begin + 10;
				if (begin[11] == c) return begin + 11;
				if (begin[12] == c) return begin + 12;
				if (begin[13] == c) return begin + 13;
				if (begin[14] == c) return begin + 14;
				if (begin[15] == c) return begin + 15;
				begin += 16;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 2: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 3: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 4: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 5: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 6: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 7: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 8: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 9: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 10: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 11: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 12: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 13: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 14: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 15: if (*begin == c) return begin; begin++; ful_fallthrough;
		case 16: return begin;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_8_small_zero_byte(const char8 * begin, const char8 * end, char8 c)
	{
		const char8 * const end_line = end - 8;
		if (begin < end_line)
		{
			const uint64 bytes = 0x0101010101010101u * (uint8)c;

			do
			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(begin);

				unsigned long long index;
				if (least_significant_zero_byte(qword ^ bytes, index))
					return begin + index;

				begin += 8;
			}
			while (begin < end_line);

			{
				const uint64 qword = *reinterpret_cast<const uint64 *>(end_line);

				const unsigned long long index = least_significant_zero_byte(qword ^ bytes);
				return end_line + index;
			}
		}
		else
		{
			switch (begin - end_line)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)c;

				const uint64 qword = (static_cast<uint64>(*reinterpret_cast<const uint32 *>(end - 4)) << (4 - (begin - end_line))) | *reinterpret_cast<const uint32 *>(begin);

				const unsigned long long index = least_significant_zero_byte(qword ^ bytes);
				return end_line + index;
			}
			case 4: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 5: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 6: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 7: if (*begin == c) return begin; begin++; ful_fallthrough;
			case 8: return begin;
			default: ful_unreachable();
			}
		}
	}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const char8 * find_8_8_small_sse2(const char8 * begin, const char8 * end, char8 c)
	{
		if (begin == end)
			return begin;

		const __m128i c128 = _mm_set1_epi8(c);

		const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
		const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

		if (begin_chunk == end_chunk)
		{
			const __m128i line = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
			const __m128i cmp = _mm_cmpeq_epi8(line, c128);
			const unsigned int mask = (_mm_movemask_epi8(cmp) | (static_cast<unsigned int>(-2) << ((end - 1) - begin_chunk))) & (static_cast<unsigned int>(-1) << (begin - begin_chunk));
			const unsigned int index = count_trailing_zero_bits(mask);
			return begin + index;
		}
		else
		{
			const char8 * const end_line = end - 16;
			if (begin < end_line)
			{
				do
				{
					const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i cmp = _mm_cmpeq_epi8(line, c128);
					const unsigned int mask = _mm_movemask_epi8(cmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);

				{
					const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
					const __m128i cmp = _mm_cmpeq_epi8(line, c128);
					const unsigned int mask = _mm_movemask_epi8(cmp);
					const unsigned int index = count_trailing_zero_bits(mask); // todo tag_popcnt
					return end_line + index;
				}
			}
			else
			{
				const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i cmp = _mm_cmpeq_epi8(line, c128);
				const unsigned int mask = set_higher_bits(_mm_movemask_epi8(cmp), static_cast<unsigned int>((end - 1) - begin));
				const unsigned int index = count_trailing_zero_bits(mask);
				return begin + index;
			}
		}
	}
#endif
}

TEST_CASE("dump find_8_8 small", "[.][dump]")
{
	BENCHMARK_DUMP("plot/find_8_8_small.dump", lin_style, 1000, 1, 80, 1)
	{
		BENCHMARK_GROUP("for")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_8_small_for(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 'b'); });
		};

		BENCHMARK_GROUP("if unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_8_small_if_unroll_8(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 'b'); });
		};

		BENCHMARK_GROUP("if unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_8_small_if_unroll_16(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 'b'); });
		};

		BENCHMARK_GROUP("zero byte")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_8_small_zero_byte(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 'b'); });
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_8_small_sse2(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 'b'); });
		};
#endif
	}
}

namespace
{
	const char8 * find_8_16_small_for(const char8 * begin, const char8 * end, char16 c)
	{
		const char8 * const end_word = end - 2;
		for (; begin <= end_word; begin++)
		{
			if (*reinterpret_cast<const char16 *>(begin) == c)
				return begin;
		}
		return end;
	}

	const char8 * find_8_16_small_if_unroll_8(const char8 * begin, const char8 * end, char16 c)
	{
		const char8 * const end_line = end - 8 - 1;
		if (begin <= end_line)
		{
			do
			{
				if (*reinterpret_cast<const char16 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char16 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char16 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char16 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char16 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char16 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char16 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char16 *>(begin + 7) == c) return begin + 7;
				begin += 8;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 2: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 3: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 4: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 5: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 6: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 7: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 8:
		case 9: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_16_small_if_unroll_16(const char8 * begin, const char8 * end, char16 c)
	{
		const char8 * const end_line = end - 16 - 1;
		if (begin <= end_line)
		{
			do
			{
				if (*reinterpret_cast<const char16 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char16 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char16 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char16 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char16 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char16 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char16 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char16 *>(begin + 7) == c) return begin + 7;
				if (*reinterpret_cast<const char16 *>(begin + 8) == c) return begin + 8;
				if (*reinterpret_cast<const char16 *>(begin + 9) == c) return begin + 9;
				if (*reinterpret_cast<const char16 *>(begin + 10) == c) return begin + 10;
				if (*reinterpret_cast<const char16 *>(begin + 11) == c) return begin + 11;
				if (*reinterpret_cast<const char16 *>(begin + 12) == c) return begin + 12;
				if (*reinterpret_cast<const char16 *>(begin + 13) == c) return begin + 13;
				if (*reinterpret_cast<const char16 *>(begin + 14) == c) return begin + 14;
				if (*reinterpret_cast<const char16 *>(begin + 15) == c) return begin + 15;
				begin += 16;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 2: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 3: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 4: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 5: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 6: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 7: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 8: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 9: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 10: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 11: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 12: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 13: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 14: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 15: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 16:
		case 17: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_16_small_zero_word(const char8 * begin, const char8 * end, char16 c)
	{
		const char8 * const end_line = end - 8 - 1;
		if (begin < end_line)
		{
			const uint64 bytes1 = 0x0101010101010101u * (uint8)c;
			const uint64 bytes2 = 0x0101010101010101u * (uint8)(c >> 8);

			do
			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin + 1);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
					return begin + index;

				begin += 8;
			}
			while (begin < end_line);

			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(end_line);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(end_line + 1);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, index))
					return end_line + index;

				return end_line + 8 + 1;
			}
		}
		else
		{
			switch (begin - end_line)
			{
			case 0: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 1: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 2: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 3: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 4: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 5: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 6: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 7: if (*reinterpret_cast<const char16 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 8:
			case 9: return end;
			default: ful_unreachable();
			}
		}
	}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const char8 * find_8_16_small_sse2(const char8 * begin, const char8 * end, char16 c)
	{
		if (static_cast<usize>(end - begin) < 2)
			return end;

		const __m128i c1281 = _mm_set1_epi8((uint8)c);
		const __m128i c1282 = _mm_set1_epi8((uint8)(c >> 8));

		const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
		const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

		if (begin_chunk == end_chunk)
		{
			const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
			const __m128i line2 = _mm_srli_si128(line1, 1);
			const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
			const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
			const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
			const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp) >> (begin - begin_chunk), static_cast<unsigned int>((end - 2) - begin));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return begin + index;
			}
			return end;
		}
		else
		{
			const char8 * const end_line = end - 16 - 1;
			if (begin < end_line)
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);

				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					const unsigned int index = count_trailing_zero_bits(mask); // todo tag_popcnt
					return end_line + index;
				}
			}
			else
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i andcmp = _mm_and_si128(cmp1, cmp2);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp), static_cast<unsigned int>((end - 1 - 1) - begin));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}
		}
	}
#endif
}

TEST_CASE("dump find_8_16 small", "[.][dump]")
{
	BENCHMARK_DUMP("plot/find_8_16_small.dump", lin_style, 1000, 1, 80, 1)
	{
		BENCHMARK_GROUP("for")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_16_small_for(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x6262); }); // 'bb'
		};

		BENCHMARK_GROUP("if unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_16_small_if_unroll_8(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x6262); }); // 'bb'
		};

		BENCHMARK_GROUP("if unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_16_small_if_unroll_16(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x6262); }); // 'bb'
		};

		BENCHMARK_GROUP("zero byte")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_16_small_zero_word(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x6262); }); // 'bb'
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_16_small_sse2(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x6262); }); // 'bb'
		};
#endif
	}
}

namespace
{
	const char8 * find_8_24_small_for(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const char8 * const end_word = end - 3;
		for (; begin <= end_word; begin++)
		{
			if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c)
				return begin;
		}
		return end;
	}

	const char8 * find_8_24_small_if_unroll_8(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const char8 * const end_line = end - 8 - 2;
		if (begin <= end_line)
		{
			do
			{
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 0)) == c) return begin + 0;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 1)) == c) return begin + 1;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 2)) == c) return begin + 2;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 3)) == c) return begin + 3;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 4)) == c) return begin + 4;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 5)) == c) return begin + 5;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 6)) == c) return begin + 6;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 7)) == c) return begin + 7;
				begin += 8;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 2: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 3: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 4: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 5: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 6: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 7: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 8:
		case 9:
		case 10: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_24_small_if_unroll_16(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const char8 * const end_line = end - 16 - 2;
		if (begin <= end_line)
		{
			do
			{
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 0)) == c) return begin + 0;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 1)) == c) return begin + 1;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 2)) == c) return begin + 2;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 3)) == c) return begin + 3;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 4)) == c) return begin + 4;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 5)) == c) return begin + 5;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 6)) == c) return begin + 6;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 7)) == c) return begin + 7;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 8)) == c) return begin + 8;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 9)) == c) return begin + 9;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 10)) == c) return begin + 10;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 11)) == c) return begin + 11;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 12)) == c) return begin + 12;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 13)) == c) return begin + 13;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 14)) == c) return begin + 14;
				if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin + 15)) == c) return begin + 15;
				begin += 16;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 2: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 3: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 4: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 5: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 6: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 7: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 8: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 9: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 10: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 11: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 12: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 13: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 14: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 15: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
		case 16:
		case 17:
		case 18: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_24_small_zero_word(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const char8 * const end_line = end - 8 - 2;
		if (begin < end_line)
		{
			const uint64 bytes1 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c));
			const uint64 bytes2 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 8);
			const uint64 bytes3 = 0x0101010101010101u * static_cast<uint8>(static_cast<uint32>(c) >> 16);

			do
			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin + 1);
				const uint64 qword3 = *reinterpret_cast<const uint64 *>(begin + 2);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
					return begin + index;

				begin += 8;
			}
			while (begin < end_line);

			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(end_line);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(end_line + 1);
				const uint64 qword3 = *reinterpret_cast<const uint64 *>(end_line + 2);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, index))
					return end_line + index;

				return end_line + 8 + 2;
			}
		}
		else
		{
			switch (begin - end_line)
			{
			case 0: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 1: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 2: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 3: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 4: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 5: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 6: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 7: if (static_cast<char_fast24>(*reinterpret_cast<const char24 *>(begin)) == c) return begin; begin++; ful_fallthrough;
			case 8:
			case 9:
			case 10: return end;
			default: ful_unreachable();
			}
		}
	}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const char8 * find_8_24_small_sse2(const char8 * begin, const char8 * end, char_fast24 c)
	{
		const usize size = end - begin;
		if (size < 3)
			return end;

		const __m128i c1281 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c)));
		const __m128i c1282 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 8));
		const __m128i c1283 = _mm_set1_epi8(static_cast<uint8>(static_cast<uint32>(c) >> 16));

		const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
		const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

		if (begin_chunk == end_chunk)
		{
			const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
			const __m128i line2 = _mm_srli_si128(line1, 1);
			const __m128i line3 = _mm_srli_si128(line1, 2);
			const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
			const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
			const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
			const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
			const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp) >> (begin - begin_chunk), static_cast<unsigned int>((end - 3) - begin));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return begin + index;
			}
			return end;
		}
		else
		{
			const char8 * const end_line = end - 16 - 2;
			if (end_line < begin)
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i line3 = _mm_srli_si128(line2, 1);
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp), static_cast<unsigned int>((end - 3) - begin));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}

			if (begin < end_line)
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
					const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 2));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
					const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);
			}

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 2));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), cmp3);
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}
	}
#endif
}

TEST_CASE("dump find_8_24 small", "[.][dump]")
{
	BENCHMARK_DUMP("plot/find_8_24_small.dump", lin_style, 1000, 1, 80, 1)
	{
		BENCHMARK_GROUP("for")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_24_small_for(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), char_fast24{0x626262}); }); // 'bbb'
		};

		BENCHMARK_GROUP("if unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_24_small_if_unroll_8(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), char_fast24{0x626262}); }); // 'bbb'
		};

		BENCHMARK_GROUP("if unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_24_small_if_unroll_16(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), char_fast24{0x626262}); }); // 'bbb'
		};

		BENCHMARK_GROUP("zero byte")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_24_small_zero_word(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), char_fast24{0x626262}); }); // 'bbb'
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_24_small_sse2(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), char_fast24{0x626262}); }); // 'bbb'
		};
#endif
	}
}

namespace
{
	const char8 * find_8_32_small_for(const char8 * begin, const char8 * end, char32 c)
	{
		const char8 * const end_word = end - 4;
		for (; begin <= end_word; begin++)
		{
			if (*reinterpret_cast<const char32 *>(begin) == c)
				return begin;
		}
		return end;
	}

	const char8 * find_8_32_small_if_unroll_8(const char8 * begin, const char8 * end, char32 c)
	{
		const char8 * const end_line = end - 8 - 3;
		if (begin <= end_line)
		{
			do
			{
				if (*reinterpret_cast<const char32 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char32 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char32 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char32 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char32 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char32 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char32 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char32 *>(begin + 7) == c) return begin + 7;
				begin += 8;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 2: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 3: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 4: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 5: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 6: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 7: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 8:
		case 9:
		case 10:
		case 11: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_32_small_if_unroll_16(const char8 * begin, const char8 * end, char32 c)
	{
		const char8 * const end_line = end - 16 - 3;
		if (begin <= end_line)
		{
			do
			{
				if (*reinterpret_cast<const char32 *>(begin + 0) == c) return begin + 0;
				if (*reinterpret_cast<const char32 *>(begin + 1) == c) return begin + 1;
				if (*reinterpret_cast<const char32 *>(begin + 2) == c) return begin + 2;
				if (*reinterpret_cast<const char32 *>(begin + 3) == c) return begin + 3;
				if (*reinterpret_cast<const char32 *>(begin + 4) == c) return begin + 4;
				if (*reinterpret_cast<const char32 *>(begin + 5) == c) return begin + 5;
				if (*reinterpret_cast<const char32 *>(begin + 6) == c) return begin + 6;
				if (*reinterpret_cast<const char32 *>(begin + 7) == c) return begin + 7;
				if (*reinterpret_cast<const char32 *>(begin + 8) == c) return begin + 8;
				if (*reinterpret_cast<const char32 *>(begin + 9) == c) return begin + 9;
				if (*reinterpret_cast<const char32 *>(begin + 10) == c) return begin + 10;
				if (*reinterpret_cast<const char32 *>(begin + 11) == c) return begin + 11;
				if (*reinterpret_cast<const char32 *>(begin + 12) == c) return begin + 12;
				if (*reinterpret_cast<const char32 *>(begin + 13) == c) return begin + 13;
				if (*reinterpret_cast<const char32 *>(begin + 14) == c) return begin + 14;
				if (*reinterpret_cast<const char32 *>(begin + 15) == c) return begin + 15;
				begin += 16;
			}
			while (begin <= end_line);
		}

		switch (begin - end_line)
		{
		case 1: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 2: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 3: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 4: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 5: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 6: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 7: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 8: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 9: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 10: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 11: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 12: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 13: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 14: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 15: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
		case 16:
		case 17:
		case 18:
		case 19: return end;
		default: ful_unreachable();
		}
	}

	const char8 * find_8_32_small_zero_word(const char8 * begin, const char8 * end, char32 c)
	{
		const char8 * const end_line = end - 8 - 3;
		if (begin < end_line)
		{
			const uint64 bytes1 = 0x0101010101010101u * (uint8)c;
			const uint64 bytes2 = 0x0101010101010101u * (uint8)(static_cast<uint32>(c) >> 8);
			const uint64 bytes3 = 0x0101010101010101u * (uint8)(static_cast<uint32>(c) >> 16);
			const uint64 bytes4 = 0x0101010101010101u * (uint8)(static_cast<uint32>(c) >> 24);

			do
			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(begin);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(begin + 1);
				const uint64 qword3 = *reinterpret_cast<const uint64 *>(begin + 2);
				const uint64 qword4 = *reinterpret_cast<const uint64 *>(begin + 3);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, qword4 ^ bytes4, index))
					return begin + index;

				begin += 8;
			}
			while (begin < end_line);

			{
				const uint64 qword1 = *reinterpret_cast<const uint64 *>(end_line);
				const uint64 qword2 = *reinterpret_cast<const uint64 *>(end_line + 1);
				const uint64 qword3 = *reinterpret_cast<const uint64 *>(end_line + 2);
				const uint64 qword4 = *reinterpret_cast<const uint64 *>(end_line + 3);

				uint64 index;
				if (least_significant_zero_word(qword1 ^ bytes1, qword2 ^ bytes2, qword3 ^ bytes3, qword4 ^ bytes4, index))
					return end_line + index;

				return end_line + 8 + 3;
			}
		}
		else
		{
			switch (begin - end_line)
			{
			case 0: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 1: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 2: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 3: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 4: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 5: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 6: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 7: if (*reinterpret_cast<const char32 *>(begin) == c) return begin; begin++; ful_fallthrough;
			case 8:
			case 9:
			case 10:
			case 11: return end;
			default: ful_unreachable();
			}
		}
	}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	const char8 * find_8_32_small_sse2(const char8 * begin, const char8 * end, char32 c)
	{
		const usize size = end - begin;
		if (size < 4)
			return end;

		const __m128i c1281 = _mm_set1_epi8((uint8)c);
		const __m128i c1282 = _mm_set1_epi8((uint8)(c >> 8));
		const __m128i c1283 = _mm_set1_epi8((uint8)(c >> 16));
		const __m128i c1284 = _mm_set1_epi8((uint8)(c >> 24));

		const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -16);
		const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -16);

		if (begin_chunk == end_chunk)
		{
			const __m128i line1 = _mm_load_si128(reinterpret_cast<const __m128i *>(begin_chunk));
			const __m128i line2 = _mm_srli_si128(line1, 1);
			const __m128i line3 = _mm_srli_si128(line1, 2);
			const __m128i line4 = _mm_srli_si128(line1, 3);
			const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
			const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
			const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
			const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
			const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
			const unsigned int mask = zero_higher_bits(_mm_movemask_epi8(andcmp) >> (begin - begin_chunk), static_cast<unsigned int>((end - 4) - begin));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return begin + index;
			}
			return end;
		}
		else
		{
			const char8 * const end_line = end - 16 - 3;
			if (end_line < begin) // size < 19
			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 17));
				const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end - 16));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
				const __m128i andcmp12 = _mm_and_si128(cmp1, cmp2);
				const __m128i andcmp34 = _mm_and_si128(cmp3, cmp4);
				const unsigned int mask = _mm_movemask_epi8(andcmp12) & (_mm_movemask_epi8(andcmp34) >> (((end - 4) - begin) ^ (16 - 1)));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}

			if (begin < end_line) // size > 19
			{
				do
				{
					const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin));
					const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 1));
					const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 2));
					const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(begin + 3));
					const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
					const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
					const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
					const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
					const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
					const unsigned int mask = _mm_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 16;
				}
				while (begin < end_line);
			}

			{
				const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line));
				const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 1));
				const __m128i line3 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 2));
				const __m128i line4 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(end_line + 3));
				const __m128i cmp1 = _mm_cmpeq_epi8(line1, c1281);
				const __m128i cmp2 = _mm_cmpeq_epi8(line2, c1282);
				const __m128i cmp3 = _mm_cmpeq_epi8(line3, c1283);
				const __m128i cmp4 = _mm_cmpeq_epi8(line4, c1284);
				const __m128i andcmp = _mm_and_si128(_mm_and_si128(cmp1, cmp2), _mm_and_si128(cmp3, cmp4));
				const unsigned int mask = _mm_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}
	}
#endif

#if defined(__AVX2__)
	const char8 * find_8_32_small_avx2(const char8 * begin, const char8 * end, char32 c)
	{
		const usize size = end - begin;
		if (size < 4)
			return end;

		const __m256i c2561 = _mm256_set1_epi8((uint8)c);
		const __m256i c2562 = _mm256_set1_epi8((uint8)(c >> 8));
		const __m256i c2563 = _mm256_set1_epi8((uint8)(c >> 16));
		const __m256i c2564 = _mm256_set1_epi8((uint8)(c >> 24));

		const char8 * const begin_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(begin) & -32);
		const char8 * const end_chunk = reinterpret_cast<const char8 *>(reinterpret_cast<puint>(end - 1) & -32);

		if (begin_chunk == end_chunk)
		{
			const __m256i line1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(begin_chunk));
			const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
			const __m256i cmp2_ = _mm256_cmpeq_epi8(line1, c2562);
			const __m256i cmp3_ = _mm256_cmpeq_epi8(line1, c2563);
			const __m256i cmp4_ = _mm256_cmpeq_epi8(line1, c2564);
			const unsigned int andcmp = _mm256_movemask_epi8(cmp1) & _mm256_movemask_epi8(cmp2_) >> 1 & _mm256_movemask_epi8(cmp3_) >> 2 & _mm256_movemask_epi8(cmp4_) >> 3;
			const unsigned int mask = zero_higher_bits(andcmp >> (begin - begin_chunk), static_cast<unsigned int>(size - 4));
			if (mask != 0)
			{
				const unsigned int index = least_significant_set_bit(mask);
				return begin + index;
			}
			return end;
		}
		else
		{
			const char8 * const end_line = end - 32 - 3;
			if (end_line < begin) // size < 35
			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 33));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end - 32));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp12 = _mm256_and_si256(cmp1, cmp2);
				const __m256i andcmp34 = _mm256_and_si256(cmp3, cmp4);
				const unsigned int mask = _mm256_movemask_epi8(andcmp12) & (_mm256_movemask_epi8(andcmp34) >> (((end - 4) - begin) ^ (16 - 1)));
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return begin + index;
				}
				return end;
			}

			if (begin < end_line) // size > 35
			{
				do
				{
					const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin));
					const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 1));
					const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 2));
					const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(begin + 3));
					const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
					const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
					const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
					const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
					const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
					const unsigned int mask = _mm256_movemask_epi8(andcmp);
					if (mask != 0)
					{
						const unsigned int index = least_significant_set_bit(mask);
						return begin + index;
					}

					begin += 32;
				}
				while (begin < end_line);
			}

			{
				const __m256i line1 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line));
				const __m256i line2 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 1));
				const __m256i line3 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 2));
				const __m256i line4 = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(end_line + 3));
				const __m256i cmp1 = _mm256_cmpeq_epi8(line1, c2561);
				const __m256i cmp2 = _mm256_cmpeq_epi8(line2, c2562);
				const __m256i cmp3 = _mm256_cmpeq_epi8(line3, c2563);
				const __m256i cmp4 = _mm256_cmpeq_epi8(line4, c2564);
				const __m256i andcmp = _mm256_and_si256(_mm256_and_si256(cmp1, cmp2), _mm256_and_si256(cmp3, cmp4));
				const unsigned int mask = _mm256_movemask_epi8(andcmp);
				if (mask != 0)
				{
					const unsigned int index = least_significant_set_bit(mask);
					return end_line + index;
				}
				return end;
			}
		}
	}
#endif
}

TEST_CASE("dump find_8_32 small", "[.][dump]")
{
	BENCHMARK_DUMP("plot/find_8_32_small.dump", lin_style, 1000, 1, 80, 1)
	{
		BENCHMARK_GROUP("for")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_for(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};

		BENCHMARK_GROUP("if unroll 8")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_if_unroll_8(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};

		BENCHMARK_GROUP("if unroll 16")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_if_unroll_16(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};

		BENCHMARK_GROUP("zero byte")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_zero_word(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_GROUP("sse2")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_sse2(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};
#endif

#if defined(__AVX2__)
		BENCHMARK_GROUP("avx2")(Catch::Benchmark::Groupometer meter)
		{
			buffer_utf8 buffer;
			buffer.allocate(meter.size() + 3);

			std::fill(buffer.beg(), buffer.end(), static_cast<unit_utf8>('a'));

			meter.measure([&](int){ return find_8_32_small_avx2(reinterpret_cast<const char8 *>(buffer.beg() + 3), reinterpret_cast<const char8 *>(buffer.end()), 0x62626262); }); // 'bbbb'
		};
#endif
	}
}
