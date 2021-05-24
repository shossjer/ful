#pragma once

// #include "ful/types.hpp"

namespace ful
{
	namespace detail
	{
		ful_generic() inline
		unit_utf16le * convert_8_16le_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(end);

			while (first != last)
			{
				const unsigned int size = point_size(first);
				switch (size)
				{
				case 1:
					*begin = static_cast<unit_utf16le>(static_cast<unsigned int>(first[0]));
					first += 1;
					++begin;
					break;
				case 2:
					*begin = static_cast<unit_utf16le>((static_cast<unsigned int>(first[0] & 0x1f) << 6) | static_cast<unsigned int>(first[1] & 0x3f));
					first += 2;
					++begin;
					break;
				case 3:
					*begin = static_cast<unit_utf16le>((static_cast<unsigned int>(first[0] & 0x0f) << 12) | (static_cast<unsigned int>(first[1] & 0x3f) << 6) | static_cast<unsigned int>(first[2] & 0x3f));
					first += 3;
					++begin;
					break;
				case 4:
				{
					const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					const uint32 valuf = value - 0x10000;

					// 0x03ff = 0000 0011 1111 1111
					// 0xd800 = 1101 1000 0000 0000
					// 0xdc00 = 1101 1100 0000 0000
					begin[0] = static_cast<unit_utf16le>((value >> 10) | 0xd800);
					begin[1] = static_cast<unit_utf16le>((valuf & 0x03ff) | 0xdc00);
					first += 4;
					begin += 2;
					break;
				}
				default:
					ful_unreachable();
				}
			}
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_8_16be_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_8_32le_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(end);

			while (first != last)
			{
				if (!ful_expect(begin != end))
					break;

				const unsigned int size = point_size(first);
				switch (size)
				{
				case 1:
					*begin = static_cast<unit_utf32le>(static_cast<unsigned int>(first[0]));
					first += 1;
					++begin;
					break;
				case 2:
					*begin = static_cast<unit_utf32le>((static_cast<unsigned int>(first[0] & 0x1f) << 6) | static_cast<unsigned int>(first[1] & 0x3f));
					first += 2;
					++begin;
					break;
				case 3:
					*begin = static_cast<unit_utf32le>((static_cast<unsigned int>(first[0] & 0x0f) << 12) | (static_cast<unsigned int>(first[1] & 0x3f) << 6) | static_cast<unsigned int>(first[2] & 0x3f));
					first += 3;
					++begin;
					break;
				case 4:
					*begin = static_cast<unit_utf32le>((static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f));
					first += 4;
					++begin;
					break;
				default:
					ful_unreachable();
				}
			}
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_8_32be_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_16le_8_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_16le_16be_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_16le_32le_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_16le_32be_none(const unit_utf16le * first, const unit_utf16le * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_16be_8_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_16be_16le_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_16be_32le_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_16be_32be_none(const unit_utf16be * first, const unit_utf16be * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_32le_8_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_32le_16le_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_32le_16be_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32be * convert_32le_32be_none(const unit_utf32le * first, const unit_utf32le * last, unit_utf32be * begin, unit_utf32be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf8 * convert_32be_8_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf8 * begin, unit_utf8 * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16le * convert_32be_16le_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf16le * begin, unit_utf16le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf16be * convert_32be_16be_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf16be * begin, unit_utf16be * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		unit_utf32le * convert_32be_32le_none(const unit_utf32be * first, const unit_utf32be * last, unit_utf32le * begin, unit_utf32le * end)
		{
			ful_unused(first);
			ful_unused(last);
			ful_unused(end);
			ful_break();
			return begin;
		}

		ful_generic() inline
		usize point_count_none(const unit_utf8 * beg, const unit_utf8 * end)
		{
			usize count = 0;

			while (beg != end)
			{
				count++;
				beg = point_next(beg);
			}
			return count;
		}

		ful_generic() inline
		const unit_utf8 * point_next_none(const unit_utf8 * s, usize n)
		{
			while (0 < n)
			{
				s = point_next(s);
			}
			return s;
		}

		ful_generic() inline
		const unit_utf8 * point_prev_none(const unit_utf8 * s, usize n)
		{
			while (0 < n)
			{
				s = point_prev(s);
			}
			return s;
		}
	}
}
