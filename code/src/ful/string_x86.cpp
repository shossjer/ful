#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

namespace ful
{
	namespace detail
	{
		ful_generic()
		char8 * copy_8_x86_32(const char8 * first, usize size, char8 * begin)
		{
			const usize end_index = size - 32;
			usize index = 0;
			do
			{
				const uint64 a = *reinterpret_cast<const uint64 *>(first + index);
				const uint64 b = *reinterpret_cast<const uint64 *>(first + index + 8);
				*reinterpret_cast<uint64 *>(begin + index) = a;
				*reinterpret_cast<uint64 *>(begin + index + 8) = b;

				index += 16;
			}
			while (index <= end_index);

			const uint64 a = *reinterpret_cast<const uint64 *>(first + index);
			const uint64 b = *reinterpret_cast<const uint64 *>(first + index + 8);
			const uint64 c = *reinterpret_cast<const uint64 *>(first + end_index + 16);
			const uint64 d = *reinterpret_cast<const uint64 *>(first + end_index + 24);
			*reinterpret_cast<uint64 *>(begin + index) = a;
			*reinterpret_cast<uint64 *>(begin + index + 8) = b;
			*reinterpret_cast<uint64 *>(begin + end_index + 16) = c;
			*reinterpret_cast<uint64 *>(begin + end_index + 24) = d;

			return begin + size;
		}

		ful_generic()
		char8 * rcopy_8_x86_32(usize size, const char8 * last, char8 * end)
		{
			const usize begin_index = size - 32;
			usize index = 0;
			do
			{
				const uint64 a = *reinterpret_cast<const uint64 *>(last - index - 8);
				const uint64 b = *reinterpret_cast<const uint64 *>(last - index - 16);
				*reinterpret_cast<uint64 *>(end - index - 8) = a;
				*reinterpret_cast<uint64 *>(end - index - 16) = b;

				index += 16;
			}
			while (index <= begin_index);

			const uint64 a = *reinterpret_cast<const uint64 *>(last - index - 8);
			const uint64 b = *reinterpret_cast<const uint64 *>(last - index - 16);
			const uint64 c = *reinterpret_cast<const uint64 *>(last - begin_index - 24);
			const uint64 d = *reinterpret_cast<const uint64 *>(last - begin_index - 32);
			*reinterpret_cast<uint64 *>(end - index - 8) = a;
			*reinterpret_cast<uint64 *>(end - index - 16) = b;
			*reinterpret_cast<uint64 *>(end - begin_index - 24) = c;
			*reinterpret_cast<uint64 *>(end - begin_index - 32) = d;

			return end - size;
		}
	}
}
