#include "ful/compiler.hpp"
#include "ful/stdint.hpp"
#include "ful/types.hpp"

#include "buffer.hpp"

#include <cstdio>

bool read_buffer(const char * fname, buffer_utf8 & buffer)
{
#if defined(_MSC_VER)
	FILE * file;
	if (ful_check(fopen_s(&file, fname, "rb") == 0))
#else
	FILE * const file = std::fopen(fname, "rb");
	if (ful_check(file))
#endif
	{
		std::fseek(file, 0, SEEK_END);
		const std::size_t size = std::ftell(file);
		std::fseek(file, 0, SEEK_SET);
		buffer.allocate(size * sizeof(char));
		const std::size_t read = std::fread(buffer.data(), sizeof(char), size, file);

		fclose(file);

		return ful_check(read == size);
	}
	return false;
}
