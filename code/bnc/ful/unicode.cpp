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

#if HAVE_UTFCPP
# include <utf8cpp/utf8.h>
#endif

#include "ful/heap_string.hpp"
#include "ful/static_string.hpp"

using namespace ful;
using namespace ful::detail;

namespace
{
#if defined(HAVE_ICU4C)
	struct setup_utf8_converter_once_hack_type
	{
		UConverter * converter;

		~setup_utf8_converter_once_hack_type()
		{
			if (converter)
			{
				ucnv_close(converter);
			}
		}

		setup_utf8_converter_once_hack_type()
			: converter(nullptr)
		{
			UErrorCode err = U_ZERO_ERROR;
			converter = ucnv_open("UTF-8", &err);
			if (!ful_check(err == U_ZERO_ERROR))
				ful_break();
		}
	}
	setup_utf8_converter_once_hack;

	struct setup_utf32_converter_once_hack_type
	{
		UConverter * converter;

		~setup_utf32_converter_once_hack_type()
		{
			if (converter)
			{
				ucnv_close(converter);
			}
		}

		setup_utf32_converter_once_hack_type()
			: converter(nullptr)
		{
			UErrorCode err = U_ZERO_ERROR;
			converter = ucnv_open("UTF-32LE", &err); // todo LE/BE
			if (!ful_check(err == U_ZERO_ERROR))
				ful_break();
		}
	}
	setup_utf32_converter_once_hack;
#endif
}

namespace
{
#if defined(HAVE_EASTL)
	unit_utf32 * conv_utf8_to_utf32_alt_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		const bool ret = eastl::DecodePart(first, last, begin, end);
		ful_expect(ret);
		return begin;
	}
#endif

#if defined(HAVE_ICU4C)
	unit_utf32 * conv_utf8_to_utf32_alt_icu4c(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		if (!ful_expect(setup_utf8_converter_once_hack.converter))
			return begin;

		UChar buffer[2048]; // arbitrary
		UErrorCode err;

		char * begin_char = reinterpret_cast<char *>(begin);
		const char * const end_char = reinterpret_cast<char *>(end);

		while (true)
		{
			UChar * target = buffer;

			err = U_ZERO_ERROR;
			::ucnv_toUnicode(setup_utf8_converter_once_hack.converter, &target, buffer + (sizeof buffer / sizeof buffer[0]), &first, last, nullptr, true, &err);
			ful_expect(err == U_ZERO_ERROR || err == U_BUFFER_OVERFLOW_ERROR);

			const bool more = err == U_BUFFER_OVERFLOW_ERROR;

			const UChar * source = buffer;

			err = U_ZERO_ERROR;
			::ucnv_fromUnicode(setup_utf32_converter_once_hack.converter, &begin_char, end_char, &source, target, nullptr, !more, &err);
			ful_expect(err == U_ZERO_ERROR);
			ful_expect(source == target);

			if (!more)
				return reinterpret_cast<unit_utf32 *>(begin_char);
		}
	}
#endif

#if defined(HAVE_UTFCPP)
	unit_utf32 * conv_utf8_to_utf32_alt_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin, unit_utf32 * end)
	{
		ful_unused(end);

		return utf8::utf8to32(first, last, begin);
	}
#endif
}

TEST_CASE("conv_utf8_to_utf32", "")
{
	auto txt = read_utf8("data/jap.txt");

#if defined(HAVE_EASTL)
	BENCHMARK_ADVANCED("EASTL")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1742);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_icu4c(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1742);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_icu4c(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utf32> tyt(txt.size());

		REQUIRE(conv_utf8_to_utf32_alt_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1742);
		meter.measure([&](int){ return conv_utf8_to_utf32_alt_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}

#if defined(_MSC_VER)
namespace
{
	unit_utfw * conv_utf8_to_utfw_alt_win32(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		const int n = ::MultiByteToWideChar(CP_UTF8, 0, first, static_cast<int>(last - first), begin, static_cast<int>(end - begin));
		return begin + n;
	}

#if defined(HAVE_EASTL)
	unit_utfw * conv_utf8_to_utfw_alt_eastl(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		const bool ret = eastl::DecodePart(first, last, begin, end);
		ful_expect(ret);
		return begin;
	}
#endif

#if defined(HAVE_ICU4C)
	unit_utfw * conv_utf8_to_utfw_alt_icu4c(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		if (!ful_expect(setup_utf8_converter_once_hack.converter))
			return begin;

		static_assert(sizeof(UChar) == sizeof(unit_utfw), "");
		// note since Windows only runs on little endian (source!?), UChar is
		// guaranteed to be little endian so casting between UChar and
		// unit_utfw is fine, probably

		UErrorCode err;
		const int n = ::ucnv_toUChars(setup_utf8_converter_once_hack.converter, reinterpret_cast<UChar *>(begin), static_cast<int>(end - begin), first, static_cast<int>(last - first), &err);
		return begin + n;
	}
#endif

#if defined(HAVE_UTFCPP)
	unit_utfw * conv_utf8_to_utfw_alt_utfcpp(const unit_utf8 * first, const unit_utf8 * last, unit_utfw * begin, unit_utfw * end)
	{
		ful_unused(end);

		return utf8::utf8to16(first, last, begin);
	}
#endif
}

TEST_CASE("conv_utf8_to_utfw", "")
{
	auto txt = read_utf8("data/jap.txt");

	BENCHMARK_ADVANCED("Win32")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		meter.measure([&](int){ return conv_utf8_to_utfw_alt_win32(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};

#if defined(HAVE_EASTL)
	BENCHMARK_ADVANCED("EASTL")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		REQUIRE(conv_utf8_to_utfw_alt_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()) == tyt.data() + 1742);
		meter.measure([&](int){ return conv_utf8_to_utfw_alt_eastl(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_ICU4C)
	BENCHMARK_ADVANCED("ICU4C")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		meter.measure([&](int){ return conv_utf8_to_utfw_alt_icu4c(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif

#if defined(HAVE_UTFCPP)
	BENCHMARK_ADVANCED("utfcpp")(Catch::Benchmark::Chronometer meter)
	{
		std::vector<unit_utfw> tyt(txt.size());

		meter.measure([&](int){ return conv_utf8_to_utfw_alt_utfcpp(txt.beg(), txt.end(), tyt.data(), tyt.data() + tyt.size()); });
	};
#endif
}
#endif
