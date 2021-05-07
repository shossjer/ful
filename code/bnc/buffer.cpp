#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

#include "ful/intrinsics.hpp"

#include "buffer.hpp"

#include <cstdio>

buffer_utf8 read_utf8(const char * fname)
{
	buffer_utf8 buffer;

#if defined(_MSC_VER)
	FILE * file;
	if (!fopen_s(&file, fname, "rb"))
#else
	FILE * const file = std::fopen(fname, "rb");
	if (file)
#endif
	{
		std::fseek(file, 0, SEEK_END);
		const auto size = std::ftell(file);
		std::fseek(file, 0, SEEK_SET);
		buffer.allocate(size * sizeof(char));
		std::fread(buffer.data(), sizeof(char), size, file);

		fclose(file);
	}
	return buffer;
}