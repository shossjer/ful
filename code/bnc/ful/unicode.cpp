#include "ful/compiler.hpp"
#include "ful/types.hpp"

#include "ful/point_utils.hpp"

#include "ful/string.hpp"
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
			std::vector<unit_utf16le> tyt(txt.size());
			REQUIRE(conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
		}

#if defined(_MSC_VER)
		BENCHMARK_ADVANCED("lib Win32")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf16le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

//#if defined(HAVE_EASTL)
//		BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
//		{
//			std::vector<unit_utf16le> tyt(txt.size());
//
//			REQUIRE(conv_utf8_to_utf16le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
//			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
//		};
//#endif

#if defined(HAVE_ICU4C)
		BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf16le> tyt(txt.size());

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
			std::vector<unit_utf16le> tyt(txt.size());

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
			std::vector<unit_utf16le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTFCPP)
		BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf16le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf16le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

		BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf16le> tyt(txt.size());

			REQUIRE(conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};

#if defined(__AVX2__)
		BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
		{
			std::vector<unit_utf16le> tyt(txt.size());

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
