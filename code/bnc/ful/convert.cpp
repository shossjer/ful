#include "ful/convert.hpp"

#include "buffer.hpp"
#include "data.hpp"

#include <catch2/catch.hpp>

#if defined(_MSC_VER)
# include <Windows.h>
#endif

#if HAVE_EASTL
# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wsign-conversion"
# endif
# include <EASTL/string.h>
# if defined(__clang__)
#  pragma clang diagnostic pop
# endif
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
#  pragma clang diagnostic ignored "-Wsign-conversion"
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
# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wsign-conversion"
# endif
# include <utf8cpp/utf8.h>
# if defined(__clang__)
#  pragma clang diagnostic pop
# endif
#endif

namespace
{
#if defined(_MSC_VER)
	ful::unit_utf16le * conv_utf8_to_utf16le_lib_win32(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
	{
		const int n = ::MultiByteToWideChar(CP_UTF8, 0, first, static_cast<int>(last - first), begin, static_cast<int>(end - begin));
		return begin + n;
	}
#endif

//#if defined(HAVE_EASTL)
//	ful::unit_utf16le * conv_utf8_to_utf16le_lib_eastl(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
//	{
//		const bool ret = eastl::DecodePart(first, last, begin, end);
//		ful_expect(ret);
//		return begin;
//	}
//#endif

#if defined(HAVE_ICU4C)
	ful::unit_utf16le * conv_utf8_to_utf16le_lib_icu4c(UConverter * converter8, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
	{
		static_assert(sizeof(UChar) == sizeof(ful::unit_utf16le), "");
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
	ful::unit_utf16le * conv_utf8_to_utf16le_lib_libiconv(iconv_t converter, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = static_cast<size_t>(last - first) * sizeof(ful::unit_utf8);
		size_t begin_size = static_cast<size_t>(end - begin) * sizeof(ful::unit_utf16le);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<ful::unit_utf16le *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	ful::unit_utf16le * conv_utf8_to_utf16le_lib_libunistring(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
	{
		ful_unused(end);

		size_t size;
		u8_to_u16(reinterpret_cast<const uint8_t *>(first), static_cast<size_t>(last - first), reinterpret_cast<uint16_t *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_UTFCPP)
	ful::unit_utf16le * conv_utf8_to_utf16le_lib_utfcpp(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16le * begin, ful::unit_utf16le * end)
	{
		ful_unused(end);

		return utf8::utf8to16(first, last, begin);
	}
#endif

	ful::unit_utf16 * conv_utf8_to_utf16le_alt_naive(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16 * begin, ful::unit_utf16 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_16_none(first, last, begin);
	}

	//ful::unit_utf61 * conv_utf8_to_utf16le_alt_naive(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf61 * begin, ful::unit_utf61 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_61_none(first, last, begin);
	//}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	ful::unit_utf16 * conv_utf8_to_utf16le_alt_sse2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16 * begin, ful::unit_utf16 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_16_sse2(first, last, begin);
	}

	//ful::unit_utf61 * conv_utf8_to_utf16le_alt_sse2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf61 * begin, ful::unit_utf61 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_61_sse2(first, last, begin);
	//}
#endif

#if defined(__AVX2__)
	ful::unit_utf16 * conv_utf8_to_utf16le_alt_avx2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf16 * begin, ful::unit_utf16 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_16_avx2(first, last, begin);
	}

	//ful::unit_utf61 * conv_utf8_to_utf16le_alt_avx2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf61 * begin, ful::unit_utf61 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_61_avx2(first, last, begin);
	//}
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

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("sse2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf16le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf16le_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits16());
			meter.measure([&](int){ return conv_utf8_to_utf16le_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

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

TEST_CASE("convert utf8 utf16le eng", "")
{
	data_eng_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf16le(txt);
}

TEST_CASE("convert utf8 utf16le got", "")
{
	data_got_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf16le(txt);
}

TEST_CASE("convert utf8 utf16le jap", "")
{
	data_jap_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf16le(txt);
}

TEST_CASE("convert utf8 utf16le rus", "")
{
	data_rus_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf16le(txt);
}

namespace
{
#if defined(HAVE_EASTL)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_eastl(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
	{
		const bool ret = eastl::DecodePart(first, last, begin, end);
		ful_expect(ret);
		return begin;
	}
#endif

#if defined(HAVE_ICU4C)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_icu4c(UConverter * converter8, UConverter * converter32, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
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
				return reinterpret_cast<ful::unit_utf32le *>(begin_char);
		}
	}
#endif

#if defined(HAVE_LIBICONV)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_libiconv(iconv_t converter, const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = static_cast<size_t>(last - first) * sizeof(ful::unit_utf8);
		size_t begin_size = static_cast<size_t>(end - begin) * sizeof(ful::unit_utf32le);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<ful::unit_utf32le *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_libunistring(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
	{
		ful_unused(end);

		size_t size;
		u8_to_u32(reinterpret_cast<const ful::uint8 *>(first), static_cast<size_t>(last - first), reinterpret_cast<ful::uint32 *>(begin), &size);
		return begin + size;
	}
#endif

#if defined(HAVE_TINYUTF8)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_tinyutf8(const tiny_utf8::string & string, ful::unit_utf32le * begin, ful::unit_utf32le * end)
	{
		ful_unused(end);

		string.to_wide_literal(begin);
		return begin + string.length();
	}
#endif

#if defined(HAVE_UTF8PROC)
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_utf8proc(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
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
	ful::unit_utf32le * conv_utf8_to_utf32le_lib_utfcpp(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32le * begin, ful::unit_utf32le * end)
	{
		ful_unused(end);

		return utf8::utf8to32(first, last, begin);
	}
#endif

	ful::unit_utf32 * conv_utf8_to_utf32le_alt_naive(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32 * begin, ful::unit_utf32 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_32_none(first, last, begin);
	}

	//ful::unit_utf23 * conv_utf8_to_utf32le_alt_naive(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf23 * begin, ful::unit_utf23 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_23_none(first, last, begin);
	//}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	ful::unit_utf32 * conv_utf8_to_utf32le_alt_sse2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32 * begin, ful::unit_utf32 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_32_sse2(first, last, begin);
	}

	//ful::unit_utf23 * conv_utf8_to_utf32le_alt_sse2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf23 * begin, ful::unit_utf23 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_23_sse2(first, last, begin);
	//}
#endif

#if defined(__AVX2__)
	ful::unit_utf32 * conv_utf8_to_utf32le_alt_avx2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf32 * begin, ful::unit_utf32 * end)
	{
		ful_unused(end);

		return ful::detail::convert_8_32_avx2(first, last, begin);
	}

	//ful::unit_utf23 * conv_utf8_to_utf32le_alt_avx2(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utf23 * begin, ful::unit_utf23 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_8_23_avx2(first, last, begin);
	//}
#endif

	template <typename Data>
	void conv_utf8_to_utf32le(Data & txt)
	{
		{
			// todo this makes the first benchmark run faster?
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
		}

#if defined(HAVE_EASTL)
		BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_ICU4C)
		BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

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
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

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
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_TINYUTF8)
		BENCHMARK_ADVANCED("lib tinyutf8")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			tiny_utf8::string string(txt.data(), txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_tinyutf8(string, tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTF8PROC)
		BENCHMARK_ADVANCED("lib utf8proc")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_utf8proc(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTFCPP)
		BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

		BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("sse2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(__AVX2__)
		BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf32le tyt;
			tyt.allocate(txt.size());

			REQUIRE(conv_utf8_to_utf32le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.npoints());
			meter.measure([&](int){ return conv_utf8_to_utf32le_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif
	}
}

TEST_CASE("convert utf8 utf32le eng", "")
{
	data_eng_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf32le(txt);
}

TEST_CASE("convert utf8 utf32le got", "")
{
	data_got_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf32le(txt);
}

TEST_CASE("convert utf8 utf32le jap", "")
{
	data_jap_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf32le(txt);
}

TEST_CASE("convert utf8 utf32le rus", "")
{
	data_rus_utf8 txt;
	REQUIRE(txt.init());

	conv_utf8_to_utf32le(txt);
}

namespace
{
#if defined(_MSC_VER)
	ful::unit_utf8 * conv_utf16le_to_utf8_lib_win32(const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		const int n = ::WideCharToMultiByte(CP_UTF8, 0, first, static_cast<int>(last - first), begin, static_cast<int>(end - begin), nullptr, nullptr);
		return begin + n;
	}
#endif

//#if defined(HAVE_EASTL)
//	ful::unit_utf8 * conv_utf16le_to_utf8_lib_eastl(const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
//	{
//		const bool ret = eastl::DecodePart(first, last, begin, end); // todo
//		ful_expect(ret);
//		return begin;
//	}
//#endif

#if defined(HAVE_ICU4C)
	ful::unit_utf8 * conv_utf16le_to_utf8_lib_icu4c(UConverter * converter8, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		static_assert(sizeof(UChar) == sizeof(ful::unit_utf16le), "");
		// note since Windows only runs on little endian (source!?), UChar is
		// guaranteed to be little endian so casting between UChar and
		// unit_utf16le is fine, probably

		UErrorCode err = U_ZERO_ERROR;
		const int n = ::ucnv_fromUChars(converter8, begin, static_cast<int>(end - begin), reinterpret_cast<const UChar *>(first), static_cast<int>(last - first), &err);
		ful_expect(err == U_ZERO_ERROR);

		return begin + n;
	}
#endif

#if defined(HAVE_LIBICONV)
	ful::unit_utf8 * conv_utf16le_to_utf8_lib_libiconv(iconv_t converter, const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		/*const*/ char * first_char = const_cast<char *>(reinterpret_cast<const char *>(first));
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = static_cast<size_t>(last - first) * sizeof(ful::unit_utf16le);
		size_t begin_size = static_cast<size_t>(end - begin) * sizeof(ful::unit_utf8);

		const size_t count = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		ful_expect(count != static_cast<size_t>(-1));

		return reinterpret_cast<ful::unit_utf8 *>(begin_char);
	}
#endif

#if defined(HAVE_LIBUNISTRING)
	ful::unit_utf8 * conv_utf16le_to_utf8_lib_libunistring(const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		ful_unused(end);

		size_t size;
		u16_to_u8(reinterpret_cast<const uint16_t *>(first), static_cast<size_t>(last - first), reinterpret_cast<uint8_t *>(begin), &size); // todo
		return begin + size;
	}
#endif

#if defined(HAVE_UTFCPP)
	ful::unit_utf8 * conv_utf16le_to_utf8_lib_utfcpp(const ful::unit_utf16le * first, const ful::unit_utf16le * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		ful_unused(end);

		return utf8::utf16to8(first, last, begin);
	}
#endif

	ful::unit_utf8 * conv_utf16le_to_utf8_alt_naive(const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		ful_unused(end);

		return ful::detail::convert_16_8_none(first, last, begin);
	}

	//ful::unit_utf8 * conv_utf16le_to_utf8_alt_naive(const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_61_8_none(first, last, begin);
	//}

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
	ful::unit_utf8 * conv_utf16le_to_utf8_alt_sse2(const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	{
		ful_unused(end);

		return ful::detail::convert_16_8_sse2(first, last, begin);
	}

	//ful::unit_utf8 * conv_utf16le_to_utf8_alt_sse2(const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
	//{
	//	ful_unused(end);

	//	return ful::detail::convert_61_8_sse2(first, last, begin);
	//}
#endif

//#if defined(__AVX2__)
//	ful::unit_utf8 * conv_utf16le_to_utf8_alt_avx2(const ful::unit_utf16 * first, const ful::unit_utf16 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
//	{
//		ful_unused(end);
//
//		return ful::detail::convert_16_8_avx2(first, last, begin);
//	}
//
//	//ful::unit_utf8 * conv_utf16le_to_utf8_alt_avx2(const ful::unit_utf61 * first, const ful::unit_utf61 * last, ful::unit_utf8 * begin, ful::unit_utf8 * end)
//	//{
//	//	ful_unused(end);
//
//	//	return ful::detail::convert_61_8_avx2(first, last, begin);
//	//}
//#endif

	template <typename Data>
	void conv_utf16le_to_utf8(Data & txt)
	{
		{
			// todo this makes the first benchmark run faster?
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
		}

#if defined(_MSC_VER)
		BENCHMARK_ADVANCED("lib Win32")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

//#if defined(HAVE_EASTL)
//		BENCHMARK_ADVANCED("lib EASTL")(Catch::Benchmark::Chronometer meter)
//		{
//			buffer_utf8 tyt;
//			tyt.allocate(txt.size() * 3);
//
//			REQUIRE(conv_utf16le_to_utf8_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
//			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
//		};
//#endif

#if defined(HAVE_ICU4C)
		BENCHMARK_ADVANCED("lib ICU4C")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			UErrorCode err = U_ZERO_ERROR;
			UConverter * const converter8 = ucnv_open("UTF-8", &err);
			REQUIRE(err == U_ZERO_ERROR);

			REQUIRE(conv_utf16le_to_utf8_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_icu4c(converter8, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			ucnv_close(converter8);
		};
#endif

#if defined(HAVE_LIBICONV)
		BENCHMARK_ADVANCED("lib libiconv")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			const iconv_t converter = iconv_open("UTF-8", "UTF-16LE");
			REQUIRE(converter != reinterpret_cast<iconv_t>(-1));

			REQUIRE(conv_utf16le_to_utf8_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_libiconv(converter, txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });

			iconv_close(converter);
		};
#endif

#if defined(HAVE_LIBUNISTRING)
		BENCHMARK_ADVANCED("lib libunistring")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_libunistring(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

#if defined(HAVE_UTFCPP)
		BENCHMARK_ADVANCED("lib utfcpp")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_lib_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

		BENCHMARK_ADVANCED("naive")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_alt_naive(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};

#if defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		BENCHMARK_ADVANCED("sse2")(Catch::Benchmark::Chronometer meter)
		{
			buffer_utf8 tyt;
			tyt.allocate(txt.size() * 3);

			REQUIRE(conv_utf16le_to_utf8_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
			meter.measure([&](int){ return conv_utf16le_to_utf8_alt_sse2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
		};
#endif

//#if defined(__AVX2__)
//		BENCHMARK_ADVANCED("avx2")(Catch::Benchmark::Chronometer meter)
//		{
//			buffer_utf8 tyt;
//			tyt.allocate(txt.size() * 3);
//
//			REQUIRE(conv_utf16le_to_utf8_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + txt.nunits8());
//			meter.measure([&](int){ return conv_utf16le_to_utf8_alt_avx2(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
//		};
//#endif
	}
}

TEST_CASE("convert utf16le utf8 eng", "")
{
	data_rus_utf16le txt;
	REQUIRE(txt.init());

	conv_utf16le_to_utf8(txt);
}

TEST_CASE("convert utf16le utf8 got", "")
{
	data_rus_utf16le txt;
	REQUIRE(txt.init());

	conv_utf16le_to_utf8(txt);
}

TEST_CASE("convert utf16le utf8 jap", "")
{
	data_rus_utf16le txt;
	REQUIRE(txt.init());

	conv_utf16le_to_utf8(txt);
}

TEST_CASE("convert utf16le utf8 rus", "")
{
	data_rus_utf16le txt;
	REQUIRE(txt.init());

	conv_utf16le_to_utf8(txt);
}
