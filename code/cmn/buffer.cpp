#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "buffer.hpp"

#include <cstdio>

#if defined(_MSC_VER)
# include <Windows.h>
#endif

#if HAVE_ICU4C
# include <unicode/ucnv.h>
#elif HAVE_LIBICONV
# include <iconv.h>
#endif

namespace
{
#if HAVE_ICU4C || HAVE_LIBICONV
	const char * converter_name(ful::unit_utf16le) { return "UTF-16LE"; }
	const char * converter_name(ful::unit_utf16be) { return "UTF-16BE"; }
	const char * converter_name(ful::unit_utf32le) { return "UTF-32LE"; }
	const char * converter_name(ful::unit_utf32be) { return "UTF-32BE"; }
#endif

#if defined(_MSC_VER)

	// not necessary but faster than the others
	ful::unit_utfw * convert(const ful::unit_utf8 * first, const ful::unit_utf8 * last, ful::unit_utfw * begin, ful::unit_utfw * end)
	{
		const int n = ::MultiByteToWideChar(CP_UTF8, 0, first, static_cast<int>(last - first) * sizeof(char), begin, static_cast<int>(end - begin));
		if (!ful_check(n != 0))
			return begin;

		return begin + n;
	}

#endif

#if HAVE_ICU4C

	template <typename T>
	T * convert(const ful::unit_utf8 * first, const ful::unit_utf8 * last, T * begin, T * end)
	{
		char * begin_char = reinterpret_cast<char *>(begin);
		char * const end_char = reinterpret_cast<char *>(end);

		UChar buffer[2048]; // arbitrary
		bool more;

		UErrorCode err = U_ZERO_ERROR;
		UConverter * const converter8 = ucnv_open("UTF-8", &err);
		if (!ful_check(err == U_ZERO_ERROR))
			goto close0;
		UConverter * const converter = ucnv_open(converter_name(T{}), &err);
		if (!ful_check(err == U_ZERO_ERROR))
			goto close1;

		do
		{
			UChar * target = buffer;

			err = U_ZERO_ERROR;
			::ucnv_toUnicode(converter8, &target, buffer + (sizeof buffer / sizeof buffer[0]), &first, last, nullptr, true, &err);
			if (!ful_check(err == U_ZERO_ERROR || err == U_BUFFER_OVERFLOW_ERROR))
				goto close2;

			more = err == U_BUFFER_OVERFLOW_ERROR;

			const UChar * source = buffer;

			err = U_ZERO_ERROR;
			::ucnv_fromUnicode(converter, &begin_char, end_char, &source, target, nullptr, !more, &err);
			if (!ful_check(err == U_ZERO_ERROR))
				goto close2;
			if (!ful_check(source == target))
				goto close2;
		}
		while (more);

		begin = reinterpret_cast<T *>(begin_char);

	close2:
		ucnv_close(converter);
	close1:
		ucnv_close(converter8);
	close0:
		return begin;
	}

#elif HAVE_LIBICONV

	template <typename T>
	T * convert(const ful::unit_utf8 * first, const ful::unit_utf8 * last, T * begin, T * end)
	{
		/*const*/ char * first_char = const_cast<char *>(first);
		char * begin_char = reinterpret_cast<char *>(begin);
		size_t first_size = static_cast<ful::usize>(last - first) * sizeof(ful::unit_utf8);
		size_t begin_size = static_cast<ful::usize>(end - begin) * sizeof(T);

		iconv_t converter;
		size_t n;

		converter = iconv_open(converter_name(T{}), "UTF-8");
		if (!ful_check(converter != iconv_t(-1)))
			goto close0;

		n = iconv(converter, &first_char, &first_size, &begin_char, &begin_size);
		if (!ful_check(n != size_t(-1)))
			goto close1;

		begin = reinterpret_cast<T *>(begin_char);

	close1:
		iconv_close(converter);
	close0:
		return begin;
	}

#else

	template <typename T>
	T * convert(const ful::unit_utf8 * first, const ful::unit_utf8 * last, T * begin, T * end)
	{
		ful_unused(ful_check(false));
		ful_unused(first);
		ful_unused(last);
		ful_unused(end);

		return begin;
	}

#endif
}

bool read_buffer(const char * fname, buffer_utf8 & buffer)
{
#if defined(_MSC_VER)
	FILE * file;
	if (!ful_check(fopen_s(&file, fname, "rb") == 0))
#else
	FILE * const file = std::fopen(fname, "rb");
	if (!ful_check(file))
#endif
		return false;

	std::fseek(file, 0, SEEK_END);
	const std::size_t size = static_cast<unsigned long>(std::ftell(file));
	std::fseek(file, 0, SEEK_SET);

	buffer.allocate(size);
	if (!ful_check(std::fread(buffer.data(), sizeof(char), size, file) == size))
	{
		fclose(file);

		return false;
	}

	fclose(file);

	return true;
}

bool convert_buffer(const buffer_utf8 & src, buffer_utf16le & dst)
{
	dst.allocate(src.size());

	ful::unit_utf16le * const end = convert(src.beg(), src.end(), dst.beg(), dst.end());

	dst.reduce(end);

	return dst.beg() != dst.end();
}

bool convert_buffer(const buffer_utf8 & src, buffer_utf16be & dst)
{
	dst.allocate(src.size());

	ful::unit_utf16be * const end = convert(src.beg(), src.end(), dst.beg(), dst.end());

	dst.reduce(end);

	return dst.beg() != dst.end();
}

bool convert_buffer(const buffer_utf8 & src, buffer_utf32le & dst)
{
	dst.allocate(src.size());

	ful::unit_utf32le * const end = convert(src.beg(), src.end(), dst.beg(), dst.end());

	dst.reduce(end);

	return dst.beg() != dst.end();
}

bool convert_buffer(const buffer_utf8 & src, buffer_utf32be & dst)
{
	dst.allocate(src.size());

	ful::unit_utf32be * const end = convert(src.beg(), src.end(), dst.beg(), dst.end());

	dst.reduce(end);

	return dst.beg() != dst.end();
}
