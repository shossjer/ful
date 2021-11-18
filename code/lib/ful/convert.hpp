#pragma once

#include "ful/unicode.hpp"

namespace ful
{
	template <typename From, typename To>
	struct convert_traits;

	template <>
	struct convert_traits<unit_utf8, unit_utf16le>
	{
		static usize max_size(const unit_utf8 * first, const unit_utf8 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf8, unit_utf16be> : convert_traits<unit_utf8, unit_utf16le> {};

	template <>
	struct convert_traits<unit_utf8, unit_utf32le>
	{
		static usize max_size(const unit_utf8 * first, const unit_utf8 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf8, unit_utf32be> : convert_traits<unit_utf8, unit_utf32le> {};

	template <>
	struct convert_traits<unit_utf16, unit_utf8>
	{
		static usize max_size(const unit_utf16 * first, const unit_utf16 * last)
		{
			return (last - first) * 3;
		}
	};

	template <>
	struct convert_traits<unit_utf16, unit_utf61>
	{
		static usize max_size(const unit_utf16 * first, const unit_utf16 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf16, unit_utf32>
	{
		static usize max_size(const unit_utf16 * first, const unit_utf16 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf16, unit_utf23> : convert_traits<unit_utf16, unit_utf32> {};

	template <>
	struct convert_traits<unit_utf61, unit_utf8>
	{
		static usize max_size(const unit_utf61 * first, const unit_utf61 * last)
		{
			return (last - first) * 3;
		}
	};

	template <>
	struct convert_traits<unit_utf61, unit_utf16> : convert_traits<unit_utf16, unit_utf61> {};

	template <>
	struct convert_traits<unit_utf61, unit_utf32>
	{
		static usize max_size(const unit_utf61 * first, const unit_utf61 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf61, unit_utf23> : convert_traits<unit_utf61, unit_utf32> {};

	template <>
	struct convert_traits<unit_utf32, unit_utf8>
	{
		static usize max_size(const unit_utf32 * first, const unit_utf32 * last)
		{
			return (last - first) * 4;
		}
	};

	template <>
	struct convert_traits<unit_utf32, unit_utf16le>
	{
		static usize max_size(const unit_utf32 * first, const unit_utf32 * last)
		{
			return (last - first) * 2;
		}
	};

	template <>
	struct convert_traits<unit_utf32, unit_utf16be> : convert_traits<unit_utf32, unit_utf16le> {};

	template <>
	struct convert_traits<unit_utf32, unit_utf23>
	{
		static usize max_size(const unit_utf32 * first, const unit_utf32 * last)
		{
			return last - first;
		}
	};

	template <>
	struct convert_traits<unit_utf23, unit_utf8>
	{
		static usize max_size(const unit_utf23 * first, const unit_utf23 * last)
		{
			return (last - first) * 4;
		}
	};

	template <>
	struct convert_traits<unit_utf23, unit_utf16le>
	{
		static usize max_size(const unit_utf23 * first, const unit_utf23 * last)
		{
			return (last - first) * 2;
		}
	};

	template <>
	struct convert_traits<unit_utf23, unit_utf16be> : convert_traits<unit_utf23, unit_utf16le> {};

	template <>
	struct convert_traits<unit_utf23, unit_utf32> : convert_traits<unit_utf32, unit_utf23> {};
	namespace detail
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		extern unit_utf16 * ful_dispatch(convert_8_16)(const unit_utf8 * first, const unit_utf8 * last, unit_utf16 * begin);
		extern unit_utf61 * ful_dispatch(convert_8_61)(const unit_utf8 * first, const unit_utf8 * last, unit_utf61 * begin);
		extern unit_utf32 * ful_dispatch(convert_8_32)(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin);
		extern unit_utf23 * ful_dispatch(convert_8_23)(const unit_utf8 * first, const unit_utf8 * last, unit_utf23 * begin);
		extern unit_utf8 * ful_dispatch(convert_16_8)(const unit_utf16 * first, const unit_utf16 * last, unit_utf8 * begin);
		extern unit_utf61 * ful_dispatch(convert_16_61)(const unit_utf16 * first, const unit_utf16 * last, unit_utf61 * begin);
		extern unit_utf32 * ful_dispatch(convert_16_32)(const unit_utf16 * first, const unit_utf16 * last, unit_utf32 * begin);
		extern unit_utf23 * ful_dispatch(convert_16_23)(const unit_utf16 * first, const unit_utf16 * last, unit_utf23 * begin);
		extern unit_utf8 * ful_dispatch(convert_61_8)(const unit_utf61 * first, const unit_utf61 * last, unit_utf8 * begin);
		extern unit_utf16 * ful_dispatch(convert_61_16)(const unit_utf61 * first, const unit_utf61 * last, unit_utf16 * begin);
		extern unit_utf32 * ful_dispatch(convert_61_32)(const unit_utf61 * first, const unit_utf61 * last, unit_utf32 * begin);
		extern unit_utf23 * ful_dispatch(convert_61_23)(const unit_utf61 * first, const unit_utf61 * last, unit_utf23 * begin);
		extern unit_utf8 * ful_dispatch(convert_32_8)(const unit_utf32 * first, const unit_utf32 * last, unit_utf8 * begin);
		extern unit_utf16 * ful_dispatch(convert_32_16)(const unit_utf32 * first, const unit_utf32 * last, unit_utf16 * begin);
		extern unit_utf61 * ful_dispatch(convert_32_61)(const unit_utf32 * first, const unit_utf32 * last, unit_utf61 * begin);
		extern unit_utf23 * ful_dispatch(convert_32_23)(const unit_utf32 * first, const unit_utf32 * last, unit_utf23 * begin);
		extern unit_utf8 * ful_dispatch(convert_23_8)(const unit_utf23 * first, const unit_utf23 * last, unit_utf8 * begin);
		extern unit_utf16 * ful_dispatch(convert_23_16)(const unit_utf23 * first, const unit_utf23 * last, unit_utf16 * begin);
		extern unit_utf61 * ful_dispatch(convert_23_61)(const unit_utf23 * first, const unit_utf23 * last, unit_utf61 * begin);
		extern unit_utf32 * ful_dispatch(convert_23_32)(const unit_utf23 * first, const unit_utf23 * last, unit_utf32 * begin);
#endif

		inline unit_utf16 * convert_8_16_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf16 * begin)
		{
			if (first == last)
				return begin;

			while (true)
			{
				switch (point_size(first))
				{
				case 1:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					*begin = static_cast<unit_utf16>(b0);
					first += 1;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 2:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					*begin = static_cast<unit_utf16>((b0 << 6) ^ b1 ^ 0x3080);
					first += 2;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 3:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					const uint32 b2 = static_cast<uint8>(first[2]);
					*begin = static_cast<unit_utf16>((b0 << 12) ^ (b1 << 6) ^ b2 ^ 0xe2080);
					first += 3;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 4:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					const uint32 b2 = static_cast<uint8>(first[2]);
					const uint32 b3 = static_cast<uint8>(first[3]);
					const uint32 value = static_cast<unit_utf32>((b0 << 18) ^ (b1 << 12) ^ (b2 << 6) ^ b3 ^ 0x3c82080);
					const uint32 valuf = value - 0x10000;

					// 0x03ff = 0000 0011 1111 1111
					// 0xd800 = 1101 1000 0000 0000
					// 0xdc00 = 1101 1100 0000 0000
					begin[0] = static_cast<unit_utf16>((valuf >> 10) | 0xd800);
					begin[1] = static_cast<unit_utf16>((valuf & 0x03ff) | 0xdc00);
					first += 4;
					begin += 2;
					if (first == last)
						return begin;
					break;
				}
				default:
					ful_unreachable();
				}
			}
		}

		inline unit_utf61 * convert_8_61_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf61 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf32 * convert_8_32_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin)
		{
			if (first == last)
				return begin;

			while (true)
			{
				switch (point_type(first))
				{
				case 1:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					*begin = static_cast<unit_utf32>(b0);
					first += 1;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 2:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					*begin = static_cast<unit_utf32>((b0 << 6) ^ b1 ^ 0x3080);
					first += 2;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 3:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					const uint32 b2 = static_cast<uint8>(first[2]);
					*begin = static_cast<unit_utf32>((b0 << 12) ^ (b1 << 6) ^ b2 ^ 0xe2080);
					first += 3;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				case 4:
				{
					const uint32 b0 = static_cast<uint8>(first[0]);
					const uint32 b1 = static_cast<uint8>(first[1]);
					const uint32 b2 = static_cast<uint8>(first[2]);
					const uint32 b3 = static_cast<uint8>(first[3]);
					*begin = static_cast<unit_utf32>((b0 << 18) ^ (b1 << 12) ^ (b2 << 6) ^ b3 ^ 0x3c82080);
					first += 4;
					++begin;
					if (first == last)
						return begin;
					break;
				}
				default:
					ful_unreachable();
				}
			}
		}

		inline unit_utf23 * convert_8_23_none(const unit_utf8 * first, const unit_utf8 * last, unit_utf23 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf8 * convert_16_8_none(const unit_utf16 * first, const unit_utf16 * last, unit_utf8 * begin)
		{
			if (first == last)
				return begin;

			while (true)
			{
				switch (point_type(first))
				{
				case 1:
				{
					const uint32 b0 = static_cast<uint16>(first[0]);
					begin[0] = static_cast<unit_utf8>(b0);
					first += 1;
					begin += 1;
					if (first == last)
						return begin;
					break;
				}
				case 2:
				{
					const uint32 b0 = static_cast<uint16>(first[0]);
					begin[0] = static_cast<unit_utf8>(b0 >> 6 | 0xc0);
					begin[1] = static_cast<unit_utf8>((b0 & 0x3f) | 0x80);
					first += 1;
					begin += 2;
					if (first == last)
						return begin;
					break;
				}
				case 3:
				{
					const uint32 b0 = static_cast<uint16>(first[0]);
					begin[0] = static_cast<unit_utf8>(b0 >> 12 | 0xe0);
					begin[1] = static_cast<unit_utf8>((b0 & 0xfff) >> 6 | 0x80);
					begin[2] = static_cast<unit_utf8>((b0 & 0x3f) | 0x80);
					first += 1;
					begin += 3;
					if (first == last)
						return begin;
					break;
				}
				case 4:
				{
					const uint32 b0 = static_cast<uint16>(first[0]);
					const uint32 b1 = static_cast<uint16>(first[1]);
					const uint32 valuf = (b0 << 10) ^ b1 ^ 0x360dc00;
					const uint32 value = valuf + 0x10000;

					begin[0] = static_cast<unit_utf8>((value >> 18) | 0xf0);
					begin[1] = static_cast<unit_utf8>(((value >> 12) & 0x3f) | 0x80);
					begin[2] = static_cast<unit_utf8>(((value >> 6) & 0x3f) | 0x80);
					begin[3] = static_cast<unit_utf8>((value & 0x3f) | 0x80);
					first += 2;
					begin += 4;
					if (first == last)
						return begin;
					break;
				}
				default:
					ful_unreachable();
				}
			}
		}

		inline unit_utf16be * convert_16_61_none(const unit_utf16 * first, const unit_utf16 * last, unit_utf61 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf32 * convert_16_32_none(const unit_utf16 * first, const unit_utf16 * last, unit_utf32 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf23 * convert_16_23_none(const unit_utf16 * first, const unit_utf16 * last, unit_utf23 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf8 * convert_61_8_none(const unit_utf61 * first, const unit_utf61 * last, unit_utf8 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf16 * convert_61_16_none(const unit_utf61 * first, const unit_utf61 * last, unit_utf16 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf32 * convert_61_32_none(const unit_utf61 * first, const unit_utf61 * last, unit_utf32 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf23 * convert_61_23_none(const unit_utf61 * first, const unit_utf61 * last, unit_utf23 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf8 * convert_32_8_none(const unit_utf32 * first, const unit_utf32 * last, unit_utf8 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf16 * convert_32_16_none(const unit_utf32 * first, const unit_utf32 * last, unit_utf16 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf61 * convert_32_61_none(const unit_utf32 * first, const unit_utf32 * last, unit_utf61 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf23 * convert_32_23_none(const unit_utf32 * first, const unit_utf32 * last, unit_utf23 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf8 * convert_23_8_none(const unit_utf23 * first, const unit_utf23 * last, unit_utf8 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf16 * convert_23_16_none(const unit_utf23 * first, const unit_utf23 * last, unit_utf16 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf61 * convert_23_61_none(const unit_utf23 * first, const unit_utf23 * last, unit_utf61 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		inline unit_utf32 * convert_23_32_none(const unit_utf23 * first, const unit_utf23 * last, unit_utf32 * begin)
		{
			ful_unused(first);
			ful_unused(last);
			ful_break();
			return begin;
		}

		ful_target("sse2") inline unit_utf16 * convert_8_16_sse2(const unit_utf8 * first, const unit_utf8 * last, unit_utf16 * begin)
		{
			const usize size = last - first;
			if (size >= 28)
			{
				const unit_utf8 * const last_chunk = last - 28;
				while (true)
				{
					switch (point_size(first))
					{
					case 1:
					{
						const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));

						const unsigned int mask = _mm_movemask_epi8(line);
						const unsigned int count = least_significant_set_bit(mask | 0x10000);

						const __m128i zero = _mm_setzero_si128();

						// _mm_unpacklo_epi8 zero
						//       arg a  p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a
						// (8 16-bits)     h|    g|    f|    e|    d|    c|    b|    a
						const __m128i val16lo = _mm_unpacklo_epi8(line, zero);

						// note with at least 20 characters of input remaining, it is safe to
						// write 1-8 characters
						// 1300 3003 0030 0300 3001   1  8
						// 1130 0300 3003 0030 0300   2  8
						// 1113 0030 0300 3003 0020   3  9
						// 1111 3003 0030 0300 3001   4 10
						// 1111 1300 3003 0030 0300   5 10
						// 1111 1130 0300 3003 0020   6 11
						// 1111 1113 0030 0300 3001   7 12
						// 1111 1111 3003 0030 0300   8 11
						_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 0), val16lo);

						if (count >= 9)
						{
							// _mm_unpackhi_epi16 zero
							//       arg a  p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a
							// (8 16-bits)     p|    o|    n|    m|    l|    k|    j|    i
							const __m128i val16hi = _mm_unpackhi_epi8(line, zero);

							// note with at least 28 characters of input remaining, it is safe to
							// write 9-16 characters
							// 1111 1111 1300 3003 0030 0300 3001   9 16
							// 1111 1111 1130 0300 3003 0030 0300  10 16
							// 1111 1111 1113 0030 0300 3003 0020  11 17
							// 1111 1111 1111 3003 0030 0300 3001  12 18
							// 1111 1111 1111 1300 3003 0030 0300  13 18
							// 1111 1111 1111 1130 0300 3003 0020  14 19
							// 1111 1111 1111 1113 0030 0300 3001  15 20
							// 1111 1111 1111 1111 3003 0030 0300  16 20
							_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 8), val16hi);
						}

						first += count;
						begin += count;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 2:
					{
						const uint16 b0 = static_cast<uint8>(first[0]);
						const uint16 b1 = static_cast<uint8>(first[1]);
						*begin = static_cast<unit_utf16>((b0 << 6) ^ b1 ^ 0x3080);
						first += 2;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 3:
					{
						const uint16 b0 = static_cast<uint8>(first[0]);
						const uint16 b1 = static_cast<uint8>(first[1]);
						const uint16 b2 = static_cast<uint8>(first[2]);
						*begin = static_cast<unit_utf16>((b0 << 12) ^ (b1 << 6) ^ b2 ^ 0xe2080);
						first += 3;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 4:
					{
						const uint16 b0 = static_cast<uint8>(first[0]);
						const uint16 b1 = static_cast<uint8>(first[1]);
						const uint16 b2 = static_cast<uint8>(first[2]);
						const uint16 b3 = static_cast<uint8>(first[3]);
						*begin = static_cast<unit_utf16>((b0 << 18) ^ (b1 << 12) ^ (b2 << 6) ^ b3 ^ 0x3c82080);
						first += 4;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					default:
						ful_unreachable();
					}
				}
			}

		done:
			return convert_8_16_none(first, last, begin);
		}

		ful_target("sse2") inline unit_utf32 * convert_8_32_sse2(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin)
		{
			const usize size = last - first;
			if (size >= 22)
			{
				const unit_utf8 * const last_chunk = last - 22;
				while (true)
				{
					switch (point_size(first))
					{
					case 1:
					{
						const __m128i line = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first));

						const unsigned int mask = _mm_movemask_epi8(line);
						const unsigned int count = least_significant_set_bit(mask | 0x10000);

						const __m128i zero = _mm_setzero_si128();

						// _mm_unpacklo_epi8 zero
						//       arg a  p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a
						// (8 16-bits)     h|    g|    f|    e|    d|    c|    b|    a
						const __m128i val16lo = _mm_unpacklo_epi8(line, zero);

						// _mm_unpacklo_epi16 zero
						//       arg a     h|    g|    f|    e|    d|    c|    b|    a
						// (4 32-bits)           d|          c|          b|          a
						const __m128i val32lolo = _mm_unpacklo_epi16(val16lo, zero);

						// note with at least 10 characters of input remaining, it is safe to
						// write 1-4 characters
						// 1400 0400 01   1  4
						// 1140 0040 00   2  4
						// 1114 0003 00   3  5
						// 1111 4000 20   4  6
						_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 0), val32lolo);

						if (count >= 5)
						{
							// _mm_unpackhi_epi16 zero
							//       arg a     h|    g|    f|    e|    d|    c|    b|    a
							// (4 32-bits)           h|          g|          f|          e
							const __m128i val32lohi = _mm_unpackhi_epi16(val16lo, zero);

							// note with at least 14 characters of input remaining, it is safe to
							// write 5-8 characters
							// 1111 1400 0400 01   5  8
							// 1111 1140 0040 00   6  8
							// 1111 1114 0003 00   7  9
							// 1111 1111 4000 20   8 10
							_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 4), val32lohi);

							if (count >= 9)
							{
								// _mm_unpackhi_epi8 zero
								//       arg a  p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a
								// (8 16-bits)     p|    o|    n|    m|    l|    k|    j|    i
								const __m128i val16hi = _mm_unpackhi_epi8(line, zero);

								// _mm_unpacklo_epi16 zero
								//       arg a     p|    o|    n|    m|    l|    k|    j|    i
								// (4 32-bits)           l|          k|          j|          i
								const __m128i val32hilo = _mm_unpacklo_epi16(val16hi, zero);

								// note with at least 18 characters of input remaining, it is safe to
								// write 9-12 characters
								// 1111 1111 1400 0400 01   9 12
								// 1111 1111 1140 0040 00  10 12
								// 1111 1111 1114 0003 00  11 13
								// 1111 1111 1111 4000 20  12 14
								_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 8), val32hilo);

								if (count >= 13)
								{
									// _mm_unpackhi_epi16 zero
									//       arg a     p|    o|    n|    m|    l|    k|    j|    i
									// (4 32-bits)           p|          o|          n|          m
									const __m128i val32hihi = _mm_unpackhi_epi16(val16hi, zero);

									// note with at least 22 characters of input remaining, it is safe to
									// write 13-16 characters
									// 1111 1111 1111 1400 0400 01  13 16
									// 1111 1111 1111 1140 0040 00  14 16
									// 1111 1111 1111 1114 0003 00  15 17
									// 1111 1111 1111 1111 4000 20  16 18
									_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 12), val32hihi);
								}
							}
						}

						first += count;
						begin += count;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 2:
					{
						const uint32 b0 = static_cast<uint8>(first[0]);
						const uint32 b1 = static_cast<uint8>(first[1]);
						*begin = static_cast<unit_utf32>((b0 << 6) ^ b1 ^ 0x3080);
						first += 2;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 3:
					{
						const uint32 b0 = static_cast<uint8>(first[0]);
						const uint32 b1 = static_cast<uint8>(first[1]);
						const uint32 b2 = static_cast<uint8>(first[2]);
						*begin = static_cast<unit_utf32>((b0 << 12) ^ (b1 << 6) ^ b2 ^ 0xe2080);
						first += 3;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 4:
					{
						const uint32 b0 = static_cast<uint8>(first[0]);
						const uint32 b1 = static_cast<uint8>(first[1]);
						const uint32 b2 = static_cast<uint8>(first[2]);
						const uint32 b3 = static_cast<uint8>(first[3]);
						*begin = static_cast<unit_utf32>((b0 << 18) ^ (b1 << 12) ^ (b2 << 6) ^ b3 ^ 0x3c82080);
						first += 4;
						++begin;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					default:
						ful_unreachable();
					}
				}
			}

		done:
			return convert_8_32_none(first, last, begin);
		}

		ful_target("sse2") inline unit_utf8 * convert_16_8_sse2(const unit_utf16 * first, const unit_utf16 * last, unit_utf8 * begin)
		{
			const usize size = last - first;
			if (size >= 16)
			{
				const unit_utf16 * const last_chunk = last - 16;
				while (true)
				{
					switch (point_type(first))
					{
					case 1:
					{
						const __m128i line1 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + 0));
						const __m128i line2 = _mm_loadu_si128(reinterpret_cast<const __m128i *>(first + 8));

						// _mm_packus_epi16
						//       arg a     h|    g|    f|    e|    d|    c|    b|    a
						//       arg b     p|    o|    n|    m|    l|    k|    j|    i
						// (16 8-bits)  p| o| n| m| l| k| j| i| h| g| f| e| d| c| b| a
						const __m128i val8 = _mm_packus_epi16(line1, line2);

						const unsigned int mask = _mm_movemask_epi8(val8);
						const unsigned int count = least_significant_set_bit(mask | 0x10000);

						_mm_storeu_si128(reinterpret_cast<__m128i *>(begin + 0), val8);

						first += count;
						begin += count;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 2:
					{
						const uint32 b0 = static_cast<uint16>(first[0]);
						begin[0] = static_cast<unit_utf8>(b0 >> 6 | 0xc0);
						begin[1] = static_cast<unit_utf8>((b0 & 0x3f) | 0x80);
						first += 1;
						begin += 2;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 3:
					{
						const uint32 b0 = static_cast<uint16>(first[0]);
						begin[0] = static_cast<unit_utf8>(b0 >> 12 | 0xe0);
						begin[1] = static_cast<unit_utf8>((b0 & 0xfff) >> 6 | 0x80);
						begin[2] = static_cast<unit_utf8>((b0 & 0x3f) | 0x80);
						first += 1;
						begin += 3;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					case 4:
					{
						const uint32 b0 = static_cast<uint16>(first[0]);
						const uint32 b1 = static_cast<uint16>(first[1]);
						const uint32 valuf = (b0 << 10) ^ b1 ^ 0x360dc00;
						const uint32 value = valuf + 0x10000;

						begin[0] = static_cast<unit_utf8>((value >> 18) | 0xf0);
						begin[1] = static_cast<unit_utf8>(((value >> 12) & 0x3f) | 0x80);
						begin[2] = static_cast<unit_utf8>(((value >> 6) & 0x3f) | 0x80);
						begin[3] = static_cast<unit_utf8>((value & 0x3f) | 0x80);
						first += 2;
						begin += 4;
						if (!(first <= last_chunk))
							goto done;
						break;
					}
					default:
						ful_unreachable();
					}
				}
			}

		done:
			return convert_16_8_none(first, last, begin);
		}

		ful_target("avx2") inline unit_utf16 * convert_8_16_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf16 * begin)
		{
			const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
			const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

			const __m256i p32 = _mm256_set1_epi8(32);

			const unit_utf8 * last_word = last - 32;
			while (first <= last_word)
			{
				const int head = first[0];
				if (head > -1)
				{
					// word : aaaaaaaa aaaaaaaa | aaaaaaaa aaaaaaaa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

					const unsigned int mask = _mm256_movemask_epi8(word);
					const unsigned int count = count_trailing_zero_bits(mask);

					//if (ful_expect(begin <= end - 32)) // todo
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
						const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelo, valuehi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuelo, valuehi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 16), value16);
					}
					first += count;
					begin += count;
				}
				else if (head > -17)
				{
					// todo make something more interesting when there is test data for it
					const uint32 value = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					const uint32 valuf = value - 0x10000;

					// 0x03ff = 0000 0011 1111 1111
					// 0xd800 = 1101 1000 0000 0000
					// 0xdc00 = 1101 1100 0000 0000
					begin[0] = static_cast<unit_utf16le>((value >> 10) | 0xd800);
					begin[1] = static_cast<unit_utf16le>((valuf & 0x03ff) | 0xdc00);
					first += 4;
					begin += 2;
				}
				else if (head > -33)
				{
					// todo make something more interesting
					*begin = static_cast<unit_utf16le>((static_cast<uint32>(first[0] & 0x0f) << 12) | (static_cast<uint32>(first[1] & 0x3f) << 6) | static_cast<uint32>(first[2] & 0x3f));
					first += 3;
					begin += 1;
				}
				else if (ful_expect(head > -65))
				{
					// word : babababa babababa | babababa babababa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					// s : each 16 : bbaaaaaa aa000000
					// t : each 16 : 00000000 bbbbbbbb
					const __m256i s = _mm256_slli_epi16(word, 6);
					const __m256i t = _mm256_srli_epi16(word, 8);
					// x : each 16 : 00000aaa aa000000
					// y : each 16 : 00000000 00bbbbbb
					const __m256i x = _mm256_and_si256(s, x_mask2);
					const __m256i y = _mm256_and_si256(t, y_mask2);
					const __m256i value = _mm256_or_si256(x, y);

					const __m256i add = _mm256_adds_epi8(word, p32);
					const unsigned int mask = _mm256_movemask_epi8(add);
					const unsigned int count = count_trailing_zero_bits(~mask) / 2;

					//if (ful_expect(begin <= end - 16)) // todo
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
					}
					first += count * 2;
					begin += count;
				}
				else
				{
					// note unexpected
					first += 1;
				}
			}
			return convert_8_16_none(first, last, begin);
		}

		ful_target("avx2") inline unit_utf32 * convert_8_32_avx2(const unit_utf8 * first, const unit_utf8 * last, unit_utf32le * begin)
		{
			//const __m256i order3 = _mm256_set_epi64x(
			//	0x80090a0b80060708, 0x8003040580000102,
			//	0x80090a0b80060708, 0x8003040580000102);
			const __m256i order3 = _mm256_set_epi64x(
				0x8005060780020304, 0x800f0001800c0d0e,
				0x80090a0b80060708, 0x8003040580000102);

			const __m256i x_mask2 = _mm256_set1_epi16(0x07c0);
			const __m256i y_mask2 = _mm256_set1_epi16(0x003f);

			const __m256i p32 = _mm256_set1_epi8(32);

			const __m256i x_mask3 = _mm256_set1_epi32(0x0000f000);
			const __m256i y_mask3 = _mm256_set1_epi32(0x00000fc0);
			const __m256i z_mask3 = _mm256_set1_epi32(0x0000003f);

			const __m256i cmp_mask3 = _mm256_set1_epi16(0x000e);

			const unit_utf8 * last_word = last - 32;
			while (first <= last_word)
			{
				const int head = first[0];
				if (head > -1)
				{
					// word : aaaaaaaa aaaaaaaa | aaaaaaaa aaaaaaaa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));

					const unsigned int mask = _mm256_movemask_epi8(word);
					const unsigned int count = count_trailing_zero_bits(mask);

					//if (ful_expect(begin <= end - 32)) // todo
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi8(word, zero);
						const __m256i valuehi = _mm256_unpackhi_epi8(word, zero);
						const __m256i valuelolo = _mm256_unpacklo_epi16(valuelo, zero);
						const __m256i valuelohi = _mm256_unpackhi_epi16(valuelo, zero);
						const __m256i valuehilo = _mm256_unpacklo_epi16(valuehi, zero);
						const __m256i valuehihi = _mm256_unpackhi_epi16(valuehi, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 16), reinterpret_cast<__m128i *>(begin + 0), valuelolo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 20), reinterpret_cast<__m128i *>(begin + 4), valuelohi);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 24), reinterpret_cast<__m128i *>(begin + 8), valuehilo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 28), reinterpret_cast<__m128i *>(begin + 12), valuehihi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelolo, valuelohi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuehilo, valuehihi, 0x20);
						const __m256i value32 = _mm256_permute2x128_si256(valuelolo, valuelohi, 0x31);
						const __m256i value48 = _mm256_permute2x128_si256(valuehilo, valuehihi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 8), value16);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 16), value32);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 24), value48);
					}
					first += count;
					begin += count;
				}
				else if (head > -17)
				{
					// todo make something more interesting when there is test data for it
					*begin = (static_cast<uint32>(first[0] & 0x07) << 18) | (static_cast<uint32>(first[1] & 0x3f) << 12) | (static_cast<uint32>(first[2] & 0x3f) << 6) | static_cast<uint32>(first[3] & 0x3f);
					first += 4;
					begin += 1;
				}
				else if (head > -33)
				{
					// bytes: -------- nmlkjihg | fedcba98 76543210
					//        -n-m-l-k -j-i-h-g | -7-6-5-4 -3-2-1-0 unpacklo
					//        -------- -------- | -f-e-d-c -b-a-9-8 unpackhi
					//
					//        -------- mnjkghde | -------- ab784512
					//        -------- -l-i-f-c | -------- -9-6-3-0
					//
					// word : -lmn-ijk -fgh-cde | -9ab-678 -345-012

					// bytes: -------- cbacbacb | acbacbac bacbacba
					// word : 0abc0abc 0abc0abc | 0abc0abc 0abc0abc
					//const __m256i bytes = _mm256_loadu2_m128i(reinterpret_cast<const __m128i *>(first + 12), reinterpret_cast<const __m128i *>(first + 0));
					//const __m256i word = _mm256_shuffle_epi8(bytes, order3);
					const __m256i bytes = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					const __m256i word = _mm256_shuffle_epi8(_mm256_permute4x64_epi64(bytes, 0b01100100), order3);
					// s : each 32 : 00000000 0000aaaa aaaabbbb bbbbcccc
					// t : each 32 : 00000000 00aaaaaa aabbbbbb bbcccccc
					const __m256i s = _mm256_srli_epi32(word, 4);
					const __m256i t = _mm256_srli_epi32(word, 2);
					// x : each 32 : 00000000 00000000 aaaa0000 00000000
					// y : each 32 : 00000000 00000000 0000bbbb bb000000
					// z : each 32 : 00000000 00000000 00000000 00cccccc
					const __m256i x = _mm256_and_si256(s, x_mask3);
					const __m256i y = _mm256_and_si256(t, y_mask3);
					const __m256i z = _mm256_and_si256(word, z_mask3);
					const __m256i value = _mm256_or_si256(_mm256_or_si256(x, y), z);

					const __m256i cmp = _mm256_cmpeq_epi16(s, cmp_mask3);
					const unsigned int mask = _mm256_movemask_epi8(cmp);
					const unsigned int count = count_trailing_zero_bits(mask ^ 0xcccccccc) / 4;

					//if (ful_expect(begin <= end - 8)) // todo
					{
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin), value);
					}
					first += count * 3;
					begin += count;
				}
				else if (ful_expect(head > -65))
				{
					// word : babababa babababa | babababa babababa
					const __m256i word = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(first));
					// s : each 16 : bbaaaaaa aa000000
					// t : each 16 : 00000000 bbbbbbbb
					const __m256i s = _mm256_slli_epi16(word, 6);
					const __m256i t = _mm256_srli_epi16(word, 8);
					// x : each 16 : 00000aaa aa000000
					// y : each 16 : 00000000 00bbbbbb
					const __m256i x = _mm256_and_si256(s, x_mask2);
					const __m256i y = _mm256_and_si256(t, y_mask2);
					const __m256i value = _mm256_or_si256(x, y);

					const __m256i add = _mm256_adds_epi8(word, p32);
					const unsigned int mask = _mm256_movemask_epi8(add);
					const unsigned int count = count_trailing_zero_bits(~mask) / 2;

					//if (ful_expect(begin <= end - 16)) // todo
					{
						const __m256i zero = _mm256_setzero_si256();
						const __m256i valuelo = _mm256_unpacklo_epi16(value, zero);
						const __m256i valuehi = _mm256_unpackhi_epi16(value, zero);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 8), reinterpret_cast<__m128i *>(begin), valuelo);
						//_mm256_storeu2_m128i(reinterpret_cast<__m128i *>(begin + 12), reinterpret_cast<__m128i *>(begin + 4), valuehi);
						const __m256i value0 = _mm256_permute2x128_si256(valuelo, valuehi, 0x20);
						const __m256i value16 = _mm256_permute2x128_si256(valuelo, valuehi, 0x31);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 0), value0);
						_mm256_storeu_si256(reinterpret_cast<__m256i *>(begin + 8), value16);
					}
					first += count * 2;
					begin += count;
				}
				else
				{
					// note unexpected
					first += 1;
				}
			}
			return convert_8_32_none(first, last, begin);
		}
	}

	ful_inline unit_utf16 * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf16 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_16_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_8_16_avx2(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::convert_8_16_sse2(first, last, begin);
#else
		return detail::convert_8_16_none(first, last, begin);
#endif
	}

	ful_inline unit_utf61 * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf61 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_61_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_8_61_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_61_sse2(first, last, begin);
#else
		return detail::convert_8_61_none(first, last, begin);
#endif
	}

	ful_inline unit_utf32 * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf32 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_32_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_8_32_avx2(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::convert_8_32_sse2(first, last, begin);
#else
		return detail::convert_8_32_none(first, last, begin);
#endif
	}

	ful_inline unit_utf23 * convert(const unit_utf8 * first, const unit_utf8 * last, unit_utf23 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_8_23_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_8_23_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_8_23_sse2(first, last, begin);
#else
		return detail::convert_8_23_none(first, last, begin);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf16 * first, const unit_utf16 * last, unit_utf8 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16_8_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_16_8_avx2(first, last, begin);
#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
		return detail::convert_16_8_sse2(first, last, begin);
#else
		return detail::convert_16_8_none(first, last, begin);
#endif
	}

	ful_inline unit_utf61 * convert(const unit_utf16 * first, const unit_utf16 * last, unit_utf61 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16_61_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_16_61_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16_61_sse2(first, last, begin);
#else
		return detail::convert_16_61_none(first, last, begin);
#endif
	}

	ful_inline unit_utf32 * convert(const unit_utf16 * first, const unit_utf16 * last, unit_utf32 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16_32_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_16_32_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16_32_sse2(first, last, begin);
#else
		return detail::convert_16_32_none(first, last, begin);
#endif
	}

	ful_inline unit_utf23 * convert(const unit_utf16 * first, const unit_utf16 * last, unit_utf23 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_16_23_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_16_23_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_16_23_sse2(first, last, begin);
#else
		return detail::convert_16_23_none(first, last, begin);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf61 * first, const unit_utf61 * last, unit_utf8 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_61_8_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_61_8_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_61_8_sse2(first, last, begin);
#else
		return detail::convert_61_8_none(first, last, begin);
#endif
	}

	ful_inline unit_utf16 * convert(const unit_utf61 * first, const unit_utf61 * last, unit_utf16 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_61_16_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_61_16_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_61_16_sse2(first, last, begin);
#else
		return detail::convert_61_16_none(first, last, begin);
#endif
	}

	ful_inline unit_utf32 * convert(const unit_utf61 * first, const unit_utf61 * last, unit_utf32 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_61_32_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_61_32_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_61_32_sse2(first, last, begin);
#else
		return detail::convert_61_32_none(first, last, begin);
#endif
	}

	ful_inline unit_utf23 * convert(const unit_utf61 * first, const unit_utf61 * last, unit_utf23 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_61_23_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_61_23_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_61_23_sse2(first, last, begin);
#else
		return detail::convert_61_23_none(first, last, begin);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf32 * first, const unit_utf32 * last, unit_utf8 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32_8_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_32_8_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32_8_sse2(first, last, begin);
#else
		return detail::convert_32_8_none(first, last, begin);
#endif
	}

	ful_inline unit_utf16 * convert(const unit_utf32 * first, const unit_utf32 * last, unit_utf16 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32_16_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_32_16_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32_16_sse2(first, last, begin);
#else
		return detail::convert_32_16_none(first, last, begin);
#endif
	}

	ful_inline unit_utf61 * convert(const unit_utf32 * first, const unit_utf32 * last, unit_utf61 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32_61_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_32_61_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32_61_sse2(first, last, begin);
#else
		return detail::convert_32_61_none(first, last, begin);
#endif
	}

	ful_inline unit_utf23 * convert(const unit_utf32 * first, const unit_utf32 * last, unit_utf23 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_32_23_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_32_23_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_32_23_sse2(first, last, begin);
#else
		return detail::convert_32_23_none(first, last, begin);
#endif
	}

	ful_inline unit_utf8 * convert(const unit_utf23 * first, const unit_utf23 * last, unit_utf8 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_23_8_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_23_8_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_23_8_sse2(first, last, begin);
#else
		return detail::convert_23_8_none(first, last, begin);
#endif
	}

	ful_inline unit_utf16 * convert(const unit_utf23 * first, const unit_utf23 * last, unit_utf16 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_23_16_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_23_16_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_23_16_sse2(first, last, begin);
#else
		return detail::convert_23_16_none(first, last, begin);
#endif
	}

	ful_inline unit_utf61 * convert(const unit_utf23 * first, const unit_utf23 * last, unit_utf61 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_23_61_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_23_61_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_23_61_sse2(first, last, begin);
#else
		return detail::convert_23_61_none(first, last, begin);
#endif
	}

	ful_inline unit_utf32 * convert(const unit_utf23 * first, const unit_utf23 * last, unit_utf32 * begin)
	{
#if defined(FUL_IFUNC) || defined(FUL_FPTR)
		return detail::convert_23_32_rtd(first, last, begin);
//#elif defined(__AVX2__)
//		return detail::convert_23_32_avx2(first, last, begin);
//#elif defined(__SSE2__) || (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
//		return detail::convert_23_32_sse2(first, last, begin);
#else
		return detail::convert_23_32_none(first, last, begin);
#endif
	}

	template <typename Base>
	class string_container;

	// return end of output, or nullptr if allocation fails
	template <typename First, typename Last, typename Base>
	ful_inline auto convert(First first, Last last, string_container<Base> & x)
		-> decltype(convert(to_address(first), to_address(last), x.data()))
	{
		const auto first_ptr = to_address(first);
		const auto last_ptr = to_address(last);

		if (!x.reserve(x.size() + convert_traits<hck::iter_value_t<First>, typename string_container<Base>::value_type>::max_size(first_ptr, last_ptr)))
			return nullptr;

		const auto it = convert(first_ptr, last_ptr, x.data() + x.size());
		x.reduce(it);

		return it;
	}

	template <typename R, typename Base>
	ful_inline auto convert(const R & x, string_container<Base> & y)
		-> decltype(convert(begin(x), end(x), y))
	{
		return convert(begin(x), end(x), y);
	}
}
