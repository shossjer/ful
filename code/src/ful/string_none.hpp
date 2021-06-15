#pragma once

namespace ful
{
	namespace detail
	{
		ful_inline
		char8 * memcopy_none(const char8 * first, const char8 * last, char8 * begin)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return begin;

			if (size <= 32)
			{
				const uint64 a = *reinterpret_cast<const uint64 *>(first);
				const uint64 b = *reinterpret_cast<const uint64 *>(first + 8);
				const uint64 c = *reinterpret_cast<const uint64 *>(last - 16);
				const uint64 d = *reinterpret_cast<const uint64 *>(last - 8);
				*reinterpret_cast<uint64 *>(begin) = a;
				*reinterpret_cast<uint64 *>(begin + 8) = b;
				*reinterpret_cast<uint64 *>(begin + size - 16) = c;
				*reinterpret_cast<uint64 *>(begin + size - 8) = d;

				return begin + size;
			}
			else
			{
				// todo 64 bytes a time is slightly faster
				extern char8 * memcopy_x86_32(const char8 * first, usize size, char8 * begin);

				return memcopy_x86_32(first, size, begin);
			}
		}

		ful_inline
		char8 * memypoc_none(const char8 * first, const char8 * last, char8 * end)
		{
			const usize size = last - first;
			if (!ful_expect(16u < size))
				return end;

			if (size <= 32)
			{
				const uint64 a = *reinterpret_cast<const uint64 *>(first);
				const uint64 b = *reinterpret_cast<const uint64 *>(first + 8);
				const uint64 c = *reinterpret_cast<const uint64 *>(last - 16);
				const uint64 d = *reinterpret_cast<const uint64 *>(last - 8);
				*reinterpret_cast<uint64 *>(end - size) = a;
				*reinterpret_cast<uint64 *>(end - size + 8) = b;
				*reinterpret_cast<uint64 *>(end - 16) = c;
				*reinterpret_cast<uint64 *>(end - 8) = d;

				return end - size;
			}
			else
			{
				extern char8 * memypoc_x86_32(usize size, const char8 * last, char8 * end);

				return memypoc_x86_32(size, last, end);
			}
		}

		ful_inline
		char8 * memswap_none(char8 * beg1, char8 * end1, char8 * beg2)
		{
			const usize size = end1 - beg1;
			if (!ful_expect(16u < size))
				return beg2;

			if (size <= 32)
			{
				const uint64 a1 = *reinterpret_cast<const uint64 *>(beg1);
				const uint64 b1 = *reinterpret_cast<const uint64 *>(beg1 + 8);
				const uint64 c1 = *reinterpret_cast<const uint64 *>(end1 - 16);
				const uint64 d1 = *reinterpret_cast<const uint64 *>(end1 - 8);
				const uint64 a2 = *reinterpret_cast<const uint64 *>(beg2);
				const uint64 b2 = *reinterpret_cast<const uint64 *>(beg2 + 8);
				const uint64 c2 = *reinterpret_cast<const uint64 *>(beg2 + size - 16);
				const uint64 d2 = *reinterpret_cast<const uint64 *>(beg2 + size - 8);
				*reinterpret_cast<uint64 *>(beg2) = a1;
				*reinterpret_cast<uint64 *>(beg2 + 8) = b1;
				*reinterpret_cast<uint64 *>(beg2 + size - 16) = c1;
				*reinterpret_cast<uint64 *>(beg2 + size - 8) = d1;
				*reinterpret_cast<uint64 *>(beg1) = a2;
				*reinterpret_cast<uint64 *>(beg1 + 8) = b2;
				*reinterpret_cast<uint64 *>(end1 - 16) = c2;
				*reinterpret_cast<uint64 *>(end1 - 8) = d2;

				return beg2 + size;
			}
			else
			{
				extern char8 * memswap_x86_32(char8 * beg1, usize size, char8 * beg2);

				return memswap_x86_32(beg1, size, beg2);
			}
		}

		ful_generic() inline
		void memset8_none(char8 * from, char8 * to, char8 u)
		{
			const usize size = to - from;
			if (!ful_expect(16 <= size))
				return;

#if defined(_MSC_VER)
			// todo benchmark this more
			//
			// https://stackoverflow.com/a/33485055
			// https://agner.org/optimize/optimizing_assembly.pdf 16.9 String instructions (all processors)
			const uint64 bytes = 0x0101010101010101u * (uint8)u;

			*reinterpret_cast<uint64 *>(from) = bytes;

			from = ful_align_next_8(from);

			__stosq(reinterpret_cast<unsigned long long *>(from), bytes, (to - from) / 8);

			*reinterpret_cast<uint64 *>(to - 8) = bytes;
#else
			// todo gcc does not support stosq?
			//
			// https://stackoverflow.com/q/10761950
			if (size <= 32)
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)u;

				*reinterpret_cast<uint64 *>(from) = bytes;
				*reinterpret_cast<uint64 *>(from + 8) = bytes;
				*reinterpret_cast<uint64 *>(to - 16) = bytes;
				*reinterpret_cast<uint64 *>(to - 8) = bytes;
			}
			else
			{
				const uint64 bytes = 0x0101010101010101u * (uint8)u;

				*reinterpret_cast<uint64 *>(from) = bytes;

				from = ful_align_next_8(from);

				char8 * const to_word = to - 32;
				if (from < to_word)
				{
					do
					{
						*reinterpret_cast<uint64 *>(from) = bytes;
						*reinterpret_cast<uint64 *>(from + 8) = bytes;
						*reinterpret_cast<uint64 *>(from + 16) = bytes;
						*reinterpret_cast<uint64 *>(from + 24) = bytes;

						from += 32;
					}
					while (from < to_word);

					from = ful_align_next_8(to_word);
				}

				*reinterpret_cast<uint64 *>(from) = bytes;
				*reinterpret_cast<uint64 *>(from + 8) = bytes;
				*reinterpret_cast<uint64 *>(from + 16) = bytes;
				*reinterpret_cast<uint64 *>(to_word + 24) = bytes;
			}
#endif
		}

		ful_generic() inline
		bool equal_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return false;
			}
			return *beg2 == unit_utf8{};
		}

		ful_generic() inline
		bool less_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return *beg1 < *beg2;
			}
			return *beg2 != unit_utf8{};
		}
	}
}
