#include "ful/string.hpp"
#include "ful/unicode.hpp"

#include "buffer.hpp"

#include <catch2/catch.hpp>

#include <vector>

#if defined(_MSC_VER)
# include <Windows.h>
#endif

#if HAVE_EASTL
# include <EASTL/string.h>
#endif

#if HAVE_ICU4C
# include <unicode/ucnv.h>
#endif

#if HAVE_LIBICONV
# include <iconv.h>
#endif

#if HAVE_LIBUNISTRING
# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wimplicit-int-conversion"
# elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
# endif
# include <unistr.h>
# if defined(__clang__)
#  pragma clang diagnostic pop
# elif defined(__GNUC__)
#  pragma GCC diagnostic pop
# endif
#endif

#if HAVE_TINYUTF8
# if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4127 4244)
# elif defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wimplicit-int-conversion"
# elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
# endif
# include <tinyutf8/tinyutf8.h>
# if defined(_MSC_VER)
#  pragma warning(pop)
# elif defined(__clang__)
#  pragma clang diagnostic pop
# elif defined(__GNUC__)
#  pragma GCC diagnostic pop
# endif
#endif

#if HAVE_UTF8PROC
# include <utf8proc.h>
#endif

#if HAVE_UTFCPP
# include <utf8cpp/utf8.h>
#endif

#include "ful/heap_string.hpp"
#include "ful/static_string.hpp"

using namespace ful;
using namespace ful::detail;

namespace
{
#if defined(HAVE_EASTL)
	unit_utf32 * conv_utf8_to_utf32_lib_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		const bool ret = eastl::DecodePart(first, last, begin, end);
		ful_expect(ret);
		return begin;
	}
#endif

#if defined(HAVE_ICU4C)
	unit_utf32 * conv_utf8_to_utf32_lib_icu4c(UConverter * converter8, UConverter * converter32, const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		UChar buffer[2048]; // arbitrary
		UErrorCode err;

		char * begin_char = reinterpret_cast<char *>(begin);
		const char * const end_char = reinterpret_cast<char *>(end);

		while (true)
		{
			UChar * target = buffer;

			err = U_ZERO_ERROR;
			::ucnv_toUnicode(converter8, &target, buffer + (sizeof buffer / sizeof buffer[0]), &first, last, nullptr, true, &err);
			ful_expect(err == U_ZERO_ERROR || err == U_BUFFER_OVERFLOW_ERROR);

			const bool more = err == U_BUFFER_OVERFLOW_ERROR;

			const UChar * source = buffer;

			err = U_ZERO_ERROR;
			::ucnv_fromUnicode(converter32, &begin_char, end_char, &source, target, nullptr, !more, &err);
			ful_expect(err == U_ZERO_ERROR);
			ful_expect(source == target);

			if (!more)
				return reinterpret_cast<unit_utf32 *>(begin_char);
		}
	}
#endif

#if defined(HAVE_LIBICONV)
	unit_utf32 * conv_utf8_to_utf32_lib_libiconv(iconv_t converter, const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = (last - first) * sizeof(unit_utf8);
		size_t begin_size = (end - begin) * sizeof(unit_utf32);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<unit_utf32 *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	unit_utf32 * conv_utf8_to_utf32_lib_libunistring(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		ful_unused(end);

		size_t size;
		u8_to_u32(reinterpret_cast<const uint8_t *>(first), last - first, reinterpret_cast<uint32_t *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_TINYUTF8)
	unit_utf32 * conv_utf8_to_utf32_lib_tinyutf8(const tiny_utf8::string & string, unit_utf32 * begin, unit_utf32 * end)
	{
		ful_unused(end);

		string.to_wide_literal(begin);
		return begin + string.length();
	}
#endif

#if defined(HAVE_UTF8PROC)
	unit_utf32 * conv_utf8_to_utf32_lib_utf8proc(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		const utf8proc_ssize_t count = utf8proc_decompose(reinterpret_cast<const utf8proc_uint8_t *>(first), last - first, reinterpret_cast<utf8proc_int32_t *>(begin), end - begin, utf8proc_option_t{});
		if (ful_expect(0 <= count))
		{
			return begin + count;
		}
		else
		{
			return begin;
		}
	}
#endif

#if defined(HAVE_UTFCPP)
	unit_utf32 * conv_utf8_to_utf32_lib_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		ful_unused(end);

		return utf8::utf8to32(first, last, begin);
	}
#endif

	unit_utf32 * conv_utf8_to_utf32_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		ful_unused(end);
		while (first != last)
		{
			if (!ful_expect(begin != end))
				break;

			const unsigned int size = point_size(first);
			switch (size)
			{
			case 1:
				*begin = static_cast<uint32>(first[0]);
				first += 1;
				++begin;
				break;
			case 2:
				*begin = (static_cast<uint32>(first[0] & 0x1f) << 6) | static_cast<uint32>(first[1] & 0x3f);
				first += 2;
				++begin;
				break;
			case 3:
				*begin = (static_cast<uint32>(first[0] & 0x0f) << 12) | (static_cast<uint32>(first[1] & 0x3f) << 6) | static_cast<uint32>(first[2] & 0x3f);
				first += 3;
				++begin;
				break;
			case 4:
				*begin = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
				first += 4;
				++begin;
				break;
			default:
				ful_unreachable();
			}
		}
		return begin;
	}

#if defined(__AVX2__)
	unit_utf32 * conv_utf8_to_utf32_alt_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		const __m256i order3 = _mm256_set_epi64x(
			0x80090a0b80060708, 0x8003040580000102,
			0x80090a0b80060708, 0x8003040580000102);

		const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
		const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

		const __m256i p32 = _mm256_set1_epi8(32);

		const __m256i x_mask3 = _mm256_set1_epi32(0x0000f000);
		const __m256i y_mask3 = _mm256_set1_epi32(0x00000fc0);
		const __m256i z_mask3 = _mm256_set1_epi32(0x0000003f);

		const __m256i cmp_mask3 = _mm256_set1_epi16(0x000e);

		const unit_utf8 * last_word = last - 32;
		while (first <= last_word)
		{
			const int head = first[0];
			if (head > -1)
			{
				// word : aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
				const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

				const unsigned int mask = _mm256_movemask_epi8(word);
				const unsigned int count = count_trailing_zeros(mask);

				if (ful_expect(begin <= end - 32))
				{
					const __m256i zero = _mm256_setzero_si256();
					const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
					const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
					const __m256i valuelolo = _mm256_unpacklo_epi16(valuelo, zero);
					const __m256i valuelohi = _mm256_unpackhi_epi16(valuelo, zero);
					const __m256i valuehilo = _mm256_unpacklo_epi16(valuehi, zero);
					const __m256i valuehihi = _mm256_unpackhi_epi16(valuehi, zero);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelolo);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 20), reinterpret_cast<__m128i *>(begin + 4), valuelohi);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehilo);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 28), reinterpret_cast<__m128i *>(begin + 12), valuehihi);
				}
				first += count;
				begin += count;
			}
			else if (head > -17)
			{
				// todo make something more interesting when there is test data for it
				*begin = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
				first += 4;
				begin += 1;
			}
			else if (head > -33)
			{
				// word : 0abc0abc 0abc0abc 0abc0abc 0abc0abc
				const __m256i bytes = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(first + 12), reinterpret_cast<const __m128i *>(first + 0));
				const __m256i word = _mm256_shuffle_epi8(bytes, order3);
				// s : each 32 : 00000000 0000aaaa aaaabbbb bbbbcccc
				// t : each 32 : 00000000 00aaaaaa aabbbbbb bbcccccc
				const __m256i s = _mm256_srli_epi32(word, 4);
				const __m256i t = _mm256_srli_epi32(word, 2);
				// x : each 32 : 00000000 00000000 aaaa0000 00000000
				// y : each 32 : 00000000 00000000 0000bbbb bb000000
				// z : each 32 : 00000000 00000000 00000000 00cccccc
				const __m256i x = _mm256_and_si256(s, x_mask3);
				const __m256i y = _mm256_and_si256(t, y_mask3);
				const __m256i z = _mm256_and_si256(word, z_mask3);
				const __m256i value = _mm256_or_si256(_mm256_or_si256(x, y), z);

				const __m256i cmp = _mm256_cmpeq_epi16(s, cmp_mask3);
				const unsigned int mask = _mm256_movemask_epi8(cmp);
				const unsigned int count = count_trailing_zeros(mask ^ 0xcccccccc) / 4;

				if (ful_expect(begin <= end - 8))
				{
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
				}
				first += count * 3;
				begin += count;
			}
			else if (ful_expect(head > -65))
			{
				// word : babababa babababa babababa babababa
				const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				// s : each 16 : bbaaaaaa aa000000
				// t : each 16 : 00000000 bbbbbbbb
				const __m256i s = _mm256_slli_epi16(word, 6);
				const __m256i t = _mm256_srli_epi16(word, 8);
				// x : each 16 : 00000aaa aa000000
				// y : each 16 : 00000000 00bbbbbb
				const __m256i x = _mm256_and_si256(s, x_mask2);
				const __m256i y = _mm256_and_si256(t, y_mask2);
				const __m256i value = _mm256_or_si256(x, y);

				const __m256i add = _mm256_adds_epi8(word, p32);
				const unsigned int mask = _mm256_movemask_epi8(add);
				const unsigned int count = count_trailing_zeros(~mask) / 2;

				if (ful_expect(begin <= end - 16))
				{
					const __m256i zero = _mm256_setzero_si256();
					const __m256i valuelo = _mm256_unpacklo_epi16(value, zero);
					const __m256i valuehi = _mm256_unpackhi_epi16(value, zero);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 8), reinterpret_cast<__m128i *>(begin), valuelo);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 12), reinterpret_cast<__m128i *>(begin + 4), valuehi);
				}
				first += count * 2;
				begin += count;
			}
			else
			{
				// note unexpected
				first += 1;
			}
		}
		return conv_utf8_to_utf32_alt_naive(first, last, begin, end);
	}
#endif
}

TEST_CASE("conv_utf8_to_utf32 eng", "")
{
	auto txt = read_utf8("data/eng.html");
	{
		std::vector<unit_utf32> tyt(txt.size());
		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
	}

#if defined(HAVE_EASTL)
	BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		UErrorCode err = U_ZERO_ERROR;
		UConverter * const converter8 = ucnv_open("UTF-8", &err);
		REQUIRE(err == U_ZERO_ERROR);
		UConverter * const converter32 = ucnv_open("UTF-32LE", &err);
		REQUIRE(err == U_ZERO_ERROR);

		REQUIRE(conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		ucnv_close(converter32);
		ucnv_close(converter8);
	};
#endif

#if defined(HAVE_LIBICONV)
	BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		const iconv_t converter = iconv_open("UTF-32LE", "UTF-8");
		REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

		REQUIRE(conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		iconv_close(converter);
	};
#endif

#if defined(HAVE_LIBUNISTRING)
	BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_TINYUTF8)
	BENCHMARK_ADVANCED("lib tinyutf8")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		tiny_utf8::string string(txt.data(), txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTF8PROC)
	BENCHMARK_ADVANCED("lib utf8proc")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

	BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1344161);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}

TEST_CASE("conv_utf8_to_utf32 rus", "")
{
	auto txt = read_utf8("data/rus.html");
	{
		std::vector<unit_utf32> tyt(txt.size());
		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
	}

#if defined(HAVE_EASTL)
	BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		UErrorCode err = U_ZERO_ERROR;
		UConverter * const converter8 = ucnv_open("UTF-8", &err);
		REQUIRE(err == U_ZERO_ERROR);
		UConverter * const converter32 = ucnv_open("UTF-32LE", &err);
		REQUIRE(err == U_ZERO_ERROR);

		REQUIRE(conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		ucnv_close(converter32);
		ucnv_close(converter8);
	};
#endif

#if defined(HAVE_LIBICONV)
	BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		const iconv_t converter = iconv_open("UTF-32LE", "UTF-8");
		REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

		REQUIRE(conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		iconv_close(converter);
	};
#endif

#if defined(HAVE_LIBUNISTRING)
	BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_TINYUTF8)
	BENCHMARK_ADVANCED("lib tinyutf8")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		tiny_utf8::string string(txt.data(), txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTF8PROC)
	BENCHMARK_ADVANCED("lib utf8proc")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

	BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1426479);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}

TEST_CASE("conv_utf8_to_utf32 jap", "")
{
	auto txt = read_utf8("data/jap.html");
	{
		std::vector<unit_utf32> tyt(txt.size());
		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
	}

#if defined(HAVE_EASTL)
	BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		UErrorCode err = U_ZERO_ERROR;
		UConverter * const converter8 = ucnv_open("UTF-8", &err);
		REQUIRE(err == U_ZERO_ERROR);
		UConverter * const converter32 = ucnv_open("UTF-32LE", &err);
		REQUIRE(err == U_ZERO_ERROR);

		REQUIRE(conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		ucnv_close(converter32);
		ucnv_close(converter8);
	};
#endif

#if defined(HAVE_LIBICONV)
	BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		const iconv_t converter = iconv_open("UTF-32LE", "UTF-8");
		REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

		REQUIRE(conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		iconv_close(converter);
	};
#endif

#if defined(HAVE_LIBUNISTRING)
	BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_TINYUTF8)
	BENCHMARK_ADVANCED("lib tinyutf8")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		tiny_utf8::string string(txt.data(), txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTF8PROC)
	BENCHMARK_ADVANCED("lib utf8proc")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

	BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864343);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}

#if defined(_MSC_VER)
namespace
{
	unit_utfw * conv_utf8_to_utfw_lib_win32(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		const int n = ::MultiByteToWideChar(CP_UTF8, 0, first, static_cast<int>(last - first), begin, static_cast<int>(end - begin));
		return begin + n;
	}

//#if defined(HAVE_EASTL)
//	unit_utfw * conv_utf8_to_utfw_lib_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
//	{
//		const bool ret = eastl::DecodePart(first, last, begin, end);
//		ful_expect(ret);
//		return begin;
//	}
//#endif

#if defined(HAVE_ICU4C)
	unit_utfw * conv_utf8_to_utfw_lib_icu4c(UConverter * converter8, const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		static_assert(sizeof(UChar) == sizeof(unit_utfw), "");
		// note since Windows only runs on little endian (source!?), UChar is
		// guaranteed to be little endian so casting between UChar and
		// unit_utfw is fine, probably

		UErrorCode err = U_ZERO_ERROR;
		const int n = ::ucnv_toUChars(converter8, reinterpret_cast<UChar *>(begin), static_cast<int>(end - begin), first, static_cast<int>(last - first), &err);
		ful_expect(err == U_ZERO_ERROR);

		return begin + n;
	}
#endif

#if defined(HAVE_LIBICONV)
	unit_utfw * conv_utf8_to_utfw_lib_libiconv(iconv_t converter, const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = (last - first) * sizeof(unit_utf8);
		size_t begin_size = (end - begin) * sizeof(unit_utfw);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<unit_utfw *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	unit_utfw * conv_utf8_to_utf16_lib_libunistring(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		size_t size;
		u8_to_u16(reinterpret_cast<const uint8_t *>(first), last - first, reinterpret_cast<uint16_t *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_UTFCPP)
	unit_utfw * conv_utf8_to_utfw_lib_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		ful_unused(end);

		return utf8::utf8to16(first, last, begin);
	}
#endif

	unit_utfw * conv_utf8_to_utfw_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		ful_unused(end);
		while (first != last)
		{
			const unsigned int size = point_size(first);
			switch (size)
			{
			case 1:
				*begin = static_cast<uint16>(first[0]);
				first += 1;
				++begin;
				break;
			case 2:
				*begin = (static_cast<uint16>(first[0] & 0x1f) << 6) | static_cast<uint16>(first[1] & 0x3f);
				first += 2;
				++begin;
				break;
			case 3:
				*begin = (static_cast<uint32>(first[0] & 0x0f) << 12) | (static_cast<uint32>(first[1] & 0x3f) << 6) | static_cast<uint32>(first[2] & 0x3f);
				first += 3;
				++begin;
				break;
			case 4:
			{
				const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
				const uint32 valuf = value - 0x10000;

				// 0x03ff = 0000 0011 1111 1111
				// 0xd800 = 1101 1000 0000 0000
				// 0xdc00 = 1101 1100 0000 0000
				begin[0] = static_cast<unit_utfw>((value >> 10) | 0xd800);
				begin[1] = static_cast<unit_utfw>((valuf & 0x03ff) | 0xdc00);
				first += 4;
				begin += 2;
				break;
			}
			default:
				ful_unreachable();
			}
		}
		return begin;
	}

#if defined(__AVX2__)
	unit_utfw * conv_utf8_to_utfw_alt_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
		const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

		const __m256i p32 = _mm256_set1_epi8(32);

		const unit_utf8 * last_word = last - 32;
		while (first <= last_word)
		{
			const int head = first[0];
			if (head > -1)
			{
				// word : aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa
				const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

				const unsigned int mask = _mm256_movemask_epi8(word);
				const unsigned int count = count_trailing_zeros(mask);

				if (ful_expect(begin <= end - 32))
				{
					const __m256i zero = _mm256_setzero_si256();
					const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
					const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelo);
					_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehi);
				}
				first += count;
				begin += count;
			}
			else if (head > -17)
			{
				// todo make something more interesting when there is test data for it
				const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
				const uint32 valuf = value - 0x10000;

				// 0x03ff = 0000 0011 1111 1111
				// 0xd800 = 1101 1000 0000 0000
				// 0xdc00 = 1101 1100 0000 0000
				begin[0] = static_cast<unit_utfw>((value >> 10) | 0xd800);
				begin[1] = static_cast<unit_utfw>((valuf & 0x03ff) | 0xdc00);
				first += 4;
				begin += 2;
			}
			else if (head > -33)
			{
				// todo make something more interesting
				*begin = static_cast<unit_utfw>((static_cast<uint32>(first[0] & 0x0f) << 12) | (static_cast<uint32>(first[1] & 0x3f) << 6) | static_cast<uint32>(first[2] & 0x3f));
				first += 3;
				begin += 1;
			}
			else if (ful_expect(head > -65))
			{
				// word : babababa babababa babababa babababa
				const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
				// s : each 16 : bbaaaaaa aa000000
				// t : each 16 : 00000000 bbbbbbbb
				const __m256i s = _mm256_slli_epi16(word, 6);
				const __m256i t = _mm256_srli_epi16(word, 8);
				// x : each 16 : 00000aaa aa000000
				// y : each 16 : 00000000 00bbbbbb
				const __m256i x = _mm256_and_si256(s, x_mask2);
				const __m256i y = _mm256_and_si256(t, y_mask2);
				const __m256i value = _mm256_or_si256(x, y);

				const __m256i add = _mm256_adds_epi8(word, p32);
				const unsigned int mask = _mm256_movemask_epi8(add);
				const unsigned int count = count_trailing_zeros(~mask) / 2;

				if (ful_expect(begin <= end - 16))
				{
					_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
				}
				first += count * 2;
				begin += count;
			}
			else
			{
				// note unexpected
				first += 1;
			}
		}
		return conv_utf8_to_utfw_alt_naive(first, last, begin, end);
	}
#endif
}

TEST_CASE("conv_utf8_to_utfw jap", "")
{
	auto txt = read_utf8("data/jap.html");
	{
		std::vector<unit_utfw> tyt(txt.size());
		REQUIRE(conv_utf8_to_utfw_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
	}

	BENCHMARK_ADVANCED("lib Win32")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

//#if defined(HAVE_EASTL)
//	BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
//	{
//		std::vector<unit_utfw> tyt(txt.size());
//
//		REQUIRE(conv_utf8_to_utfw_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
//		meter.measure([&](int){ return conv_utf8_to_utfw_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
//	};
//#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		UErrorCode err = U_ZERO_ERROR;
		UConverter * const converter8 = ucnv_open("UTF-8", &err);
		REQUIRE(err == U_ZERO_ERROR);

		REQUIRE(conv_utf8_to_utfw_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		ucnv_close(converter8);
	};
#endif

#if defined(HAVE_LIBICONV)
	BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		const iconv_t converter = iconv_open("UTF-32LE", "UTF-16LE");
		REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

		REQUIRE(conv_utf8_to_utfw_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

		iconv_close(converter);
	};
#endif

#if defined(HAVE_LIBUNISTRING)
	BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

	BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

#if defined(__AVX2__)
	BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 864344);
		meter.measure([&](int){ return conv_utf8_to_utfw_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}
#endif
