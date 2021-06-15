#include "ful/compiler.hpp"
#include "ful/stdint.hpp"

namespace ful
{
	namespace detail
	{
		char8 * memcopy_x86_32(const char8 * first, usize size, char8 * begin)
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

		char8 * memypoc_x86_32(usize size, const char8 * last, char8 * end)
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

		char8 * memswap_x86_32(char8 * beg1, usize size, char8 * beg2)
		{
			const usize end_index = size - 32;
			usize index = 0;
			do
			{
				const uint64 a1 = *reinterpret_cast<const uint64 *>(beg1 + index);
				const uint64 b1 = *reinterpret_cast<const uint64 *>(beg1 + index + 8);
				const uint64 a2 = *reinterpret_cast<const uint64 *>(beg2 + index);
				const uint64 b2 = *reinterpret_cast<const uint64 *>(beg2 + index + 8);
				*reinterpret_cast<uint64 *>(beg2 + index) = a1;
				*reinterpret_cast<uint64 *>(beg2 + index + 8) = b1;
				*reinterpret_cast<uint64 *>(beg1 + index) = a2;
				*reinterpret_cast<uint64 *>(beg1 + index + 8) = b2;

				index += 16;
			}
			while (index <= end_index);

			const uint64 a1 = *reinterpret_cast<const uint64 *>(beg1 + index);
			const uint64 b1 = *reinterpret_cast<const uint64 *>(beg1 + index + 8);
			const uint64 c1 = *reinterpret_cast<const uint64 *>(beg1 + end_index + 16);
			const uint64 d1 = *reinterpret_cast<const uint64 *>(beg1 + end_index + 24);
			const uint64 a2 = *reinterpret_cast<const uint64 *>(beg2 + index);
			const uint64 b2 = *reinterpret_cast<const uint64 *>(beg2 + index + 8);
			const uint64 c2 = *reinterpret_cast<const uint64 *>(beg2 + end_index + 16);
			const uint64 d2 = *reinterpret_cast<const uint64 *>(beg2 + end_index + 24);
			*reinterpret_cast<uint64 *>(beg2 + index) = a1;
			*reinterpret_cast<uint64 *>(beg2 + index + 8) = b1;
			*reinterpret_cast<uint64 *>(beg2 + end_index + 16) = c1;
			*reinterpret_cast<uint64 *>(beg2 + end_index + 24) = d1;
			*reinterpret_cast<uint64 *>(beg1 + index) = a2;
			*reinterpret_cast<uint64 *>(beg1 + index + 8) = b2;
			*reinterpret_cast<uint64 *>(beg1 + end_index + 16) = c2;
			*reinterpret_cast<uint64 *>(beg1 + end_index + 24) = d2;

			return beg2 + size;
		}
	}
}
