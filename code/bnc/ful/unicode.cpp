#include "ful/compiler.hpp"
#include "ful/stdhacks.hpp"
#include "ful/types.hpp"

#include "ful/convert.hpp"
#include "ful/unicode.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

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

using namespace ful;

namespace
{
#if defined(HAVE_EASTL)
	unit_utf32le * conv_utf8_to_utf32le_lib_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		const bool ret = eastl::DecodePart(first, last, begin, end);
		ful_expect(ret);
		return begin;
	}
#endif

#if defined(HAVE_ICU4C)
	unit_utf32le * conv_utf8_to_utf32le_lib_icu4c(UConverter * converter8, UConverter * converter32, const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
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
				return reinterpret_cast<unit_utf32le *>(begin_char);
		}
	}
#endif

#if defined(HAVE_LIBICONV)
	unit_utf32le * conv_utf8_to_utf32le_lib_libiconv(iconv_t converter, const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = (last - first) * sizeof(unit_utf8);
		size_t begin_size = (end - begin) * sizeof(unit_utf32le);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<unit_utf32le *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	unit_utf32le * conv_utf8_to_utf32le_lib_libunistring(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		ful_unused(end);

		size_t size;
		u8_to_u32(reinterpret_cast<const uint8_t *>(first), last - first, reinterpret_cast<uint32_t *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_TINYUTF8)
	unit_utf32le * conv_utf8_to_utf32le_lib_tinyutf8(const tiny_utf8::string & string, unit_utf32le * begin, unit_utf32le * end)
	{
		ful_unused(end);

		string.to_wide_literal(begin);
		return begin + string.length();
	}
#endif

#if defined(HAVE_UTF8PROC)
	unit_utf32le * conv_utf8_to_utf32le_lib_utf8proc(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
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
	unit_utf32le * conv_utf8_to_utf32le_lib_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		ful_unused(end);

		return utf8::utf8to32(first, last, begin);
	}
#endif

	unit_utf32le * conv_utf8_to_utf32le_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		return detail::convert_8_32le_none(first, last, begin, end);
	}

#if defined(__AVX2__)
	unit_utf32le * conv_utf8_to_utf32le_alt_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
	{
		return detail::convert_8_32le_avx2(first, last, begin, end);
	}
#endif

	template <typename Data>
	void conv_utf8_to_utf32le(Data & txt)
	{
		{
			// todo this makes the first benchmark run faster?
			std::vector<unit_utf32le> tyt(txt.size());
			REQUIRE(conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
		}

#if defined(HAVE_EASTL)
		BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_ICU4C)
		BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			UErrorCode err = U_ZERO_ERROR;
			UConverter * const converter8 = ucnv_open("UTF-8", &err);
			REQUIRE(err == U_ZERO_ERROR);
			UConverter * const converter32 = ucnv_open("UTF-32LE", &err);
			REQUIRE(err == U_ZERO_ERROR);

			REQUIRE(conv_utf8_to_utf32le_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_icu4c(converter8, converter32, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			ucnv_close(converter32);
			ucnv_close(converter8);
		};
#endif

#if defined(HAVE_LIBICONV)
		BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			const iconv_t converter = iconv_open("UTF-32LE", "UTF-8");
			REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

			REQUIRE(conv_utf8_to_utf32le_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			iconv_close(converter);
		};
#endif

#if defined(HAVE_LIBUNISTRING)
		BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_TINYUTF8)
		BENCHMARK_ADVANCED("lib tinyutf8")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			tiny_utf8::string string(txt.data(), txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTF8PROC)
		BENCHMARK_ADVANCED("lib utf8proc")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTFCPP)
		BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

		BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};

#if defined(__AVX2__)
		BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf32le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif
	}
}

TEST_CASE("conv_utf8_to_utf32le", "")
{
	SECTION("eng")
	{
		data_eng_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf32le(txt);
	}

	SECTION("got")
	{
		data_got_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf32le(txt);
	}

	SECTION("jap")
	{
		data_jap_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf32le(txt);
	}

	SECTION("rus")
	{
		data_rus_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf32le(txt);
	}
}

namespace
{
#if defined(_MSC_VER)
	unit_utf16le * conv_utf8_to_utf16le_lib_win32(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		const int n = ::MultiByteToWideChar(CP_UTF8, 0, first, static_cast<int>(last - first), begin, static_cast<int>(end - begin));
		return begin + n;
	}
#endif

//#if defined(HAVE_EASTL)
//	unit_utf16le * conv_utf8_to_utf16le_lib_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
//	{
//		const bool ret = eastl::DecodePart(first, last, begin, end);
//		ful_expect(ret);
//		return begin;
//	}
//#endif

#if defined(HAVE_ICU4C)
	unit_utf16le * conv_utf8_to_utf16le_lib_icu4c(UConverter * converter8, const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		static_assert(sizeof(UChar) == sizeof(unit_utf16le), "");
		// note since Windows only runs on little endian (source!?), UChar is
		// guaranteed to be little endian so casting between UChar and
		// unit_utf16le is fine, probably

		UErrorCode err = U_ZERO_ERROR;
		const int n = ::ucnv_toUChars(converter8, reinterpret_cast<UChar *>(begin), static_cast<int>(end - begin), first, static_cast<int>(last - first), &err);
		ful_expect(err == U_ZERO_ERROR);

		return begin + n;
	}
#endif

#if defined(HAVE_LIBICONV)
	unit_utf16le * conv_utf8_to_utf16le_lib_libiconv(iconv_t converter, const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = (last - first) * sizeof(unit_utf8);
		size_t begin_size = (end - begin) * sizeof(unit_utf16le);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<unit_utf16le *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	unit_utf16le * conv_utf8_to_utf16le_lib_libunistring(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		ful_unused(end);

		size_t size;
		u8_to_u16(reinterpret_cast<const uint8_t *>(first), last - first, reinterpret_cast<uint16_t *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_UTFCPP)
	unit_utf16le * conv_utf8_to_utf16le_lib_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		ful_unused(end);

		return utf8::utf8to16(first, last, begin);
	}
#endif

	unit_utf16le * conv_utf8_to_utf16le_alt_naive(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		return detail::convert_8_16le_none(first, last, begin, end);
	}

#if defined(__AVX2__)
	unit_utf16le * conv_utf8_to_utf16le_alt_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
	{
		return detail::convert_8_16le_avx2(first, last, begin, end);
	}
#endif

	template <typename Data>
	void conv_utf8_to_utf16le(Data & txt)
	{
		{
			// todo this makes the first benchmark run faster?
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
		}

#if defined(_MSC_VER)
		BENCHMARK_ADVANCED("lib Win32")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

//#if defined(HAVE_EASTL)
//		BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
//		{
//			buffer_utf16le tyt;
//			tyt.allocate(txt.size());
//
//			REQUIRE(conv_utf8_to_utf16le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
//			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
//		};
//#endif

#if defined(HAVE_ICU4C)
		BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			UErrorCode err = U_ZERO_ERROR;
			UConverter * const converter8 = ucnv_open("UTF-8", &err);
			REQUIRE(err == U_ZERO_ERROR);

			REQUIRE(conv_utf8_to_utf16le_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			ucnv_close(converter8);
		};
#endif

#if defined(HAVE_LIBICONV)
		BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			const iconv_t converter = iconv_open("UTF-16LE", "UTF-8");
			REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

			REQUIRE(conv_utf8_to_utf16le_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			iconv_close(converter);
		};
#endif

#if defined(HAVE_LIBUNISTRING)
		BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTFCPP)
		BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

		BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};

#if defined(__AVX2__)
		BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif
	}
}

TEST_CASE("conv_utf8_to_utf16le", "")
{
	SECTION("eng")
	{
		data_eng_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf16le(txt);
	}

	SECTION("got")
	{
		data_got_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf16le(txt);
	}

	SECTION("jap")
	{
		data_jap_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf16le(txt);
	}

	SECTION("rus")
	{
		data_rus_utf8 txt;
		REQUIRE(txt.init());

		conv_utf8_to_utf16le(txt);
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
