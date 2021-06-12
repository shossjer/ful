#pragma once

namespace ful
{
	namespace detail
	{
		ful_generic() inline
		char8 * copy_large_none(const char8 * first, const char8 * last, char8 * begin)
		{
			if (!ful_expect(16 <= last - first))
				return begin;

			const char8 * const last_word = last - 8;
			while (first <= last_word)
			{
				const uint64 word = *reinterpret_cast<const uint64 *>(first);
				*reinterpret_cast<uint64 *>(begin) = word;

				first += 8;
				begin += 8;
			}

			while (first != last)
			{
				*begin = *first;

				++first, ++begin;
			}

			return begin;
		}

		ful_generic() inline
		char8 * rcopy_large_none(const char8 * first, const char8 * last, char8 * end)
		{
			if (!ful_expect(16 <= last - first))
				return end;

			const char8 * const first_word = first + 8;
			while (first_word <= last)
			{
				last -= 8;
				end -= 8;

				const uint64 word = *reinterpret_cast<const uint64 *>(last);
				*reinterpret_cast<uint64 *>(end) = word;
			}

			while (first != last)
			{
				--last, --end;

				*end = *last;
			}

			return end;
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
		void fill_large_none(char8 * from, char8 * to, char8 u)
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
