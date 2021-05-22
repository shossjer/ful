#pragma once

namespace ful
{
	namespace detail
	{
		ful_generic() inline
		unit_utf8 * copy_large_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * begin)
		{
			if (!ful_expect(16 <= last - first))
				return begin;

			const unit_utf8 * const last_word = last - 8;
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
		unit_utf8 * rcopy_large_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf8 * end)
		{
			if (!ful_expect(16 <= last - first))
				return end;

			const unit_utf8 * const first_word = first + 8;
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
			return *beg2 == '\0';
		}

		ful_generic() inline
		void fill_large_none(unit_utf8 * from, unit_utf8 * to, unit_utf8 u)
		{
			if (!ful_expect(16 <= to - from))
				return;

			// todo
			alignas(8) uint8 bytes[] = {(uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u, (uint8)u};

			unit_utf8 * const to_word = to - 8;
			while (from < to_word)
			{
				*reinterpret_cast<uint64 *>(from) = *reinterpret_cast<const uint64 *>(bytes);

				from += 8;
			}

			*reinterpret_cast<uint64 *>(to_word) = *reinterpret_cast<const uint64 *>(bytes);
		}

		ful_generic() inline
		bool less_cstr_none(const unit_utf8 * beg1, const unit_utf8 * end1, const unit_utf8 * beg2)
		{
			for (; beg1 != end1; ++beg1, ++beg2)
			{
				if (*beg1 != *beg2)
					return *beg1 < *beg2;
			}
			return *beg2 != '\0';
		}
	}
}
