#pragma once

#include "ful/compiler.hpp"
#include "ful/types.hpp"

#include "ful/intrinsics.hpp"

#include "ful/ranges.hpp"

namespace ful
{
	template <typename T, unsigned long long N>
	using point_buffer = T[4 * N / sizeof(T)];

	class point_utf
	{
	public:

		uint32 value_;

	public:

		point_utf() = default;

		ful_inline explicit constexpr point_utf(uint32 value) : value_((ful_unused(ful_expect(value < 0x00110000)), ful_unused(ful_expect((value & 0xfffff800) != 0x0000d800)), value))
		{
		}

	public:

		ful_inline explicit operator uint8 () const { return static_cast<uint8>(value_); }
		ful_inline explicit operator uint16 () const { return static_cast<uint16>(value_); }
		ful_inline explicit operator uint32 () const { return value_; }

		ful_inline friend bool operator == (point_utf x, point_utf y) ful_pure { return x.value_ == y.value_; }
		ful_inline friend bool operator == (point_utf x, uint32 y) ful_pure { return x.value_ == y; }
		ful_inline friend bool operator == (uint32 x, point_utf y) ful_pure { return x == y.value_; }
		ful_inline friend bool operator != (point_utf x, point_utf y) ful_pure { return !(x == y); }
		ful_inline friend bool operator != (point_utf x, uint32 y) ful_pure { return !(x == y); }
		ful_inline friend bool operator != (uint32 x, point_utf y) ful_pure { return !(x == y); }
		ful_inline friend bool operator < (point_utf x, point_utf y) ful_pure { return x.value_ < y.value_; }
		ful_inline friend bool operator < (point_utf x, uint32 y) ful_pure { return x.value_ < y; }
		ful_inline friend bool operator < (uint32 x, point_utf y) ful_pure { return x < y.value_; }
		ful_inline friend bool operator <= (point_utf x, point_utf y) ful_pure { return !(y < x); }
		ful_inline friend bool operator <= (point_utf x, uint32 y) ful_pure { return !(y < x); }
		ful_inline friend bool operator <= (uint32 x, point_utf y) ful_pure { return !(y < x); }
		ful_inline friend bool operator > (point_utf x, point_utf y) ful_pure { return y < x; }
		ful_inline friend bool operator > (point_utf x, uint32 y) ful_pure { return y < x; }
		ful_inline friend bool operator > (uint32 x, point_utf y) ful_pure { return y < x; }
		ful_inline friend bool operator >= (point_utf x, point_utf y) ful_pure { return !(x < y); }
		ful_inline friend bool operator >= (point_utf x, uint32 y) ful_pure { return !(x < y); }
		ful_inline friend bool operator >= (uint32 x, point_utf y) ful_pure { return !(x < y); }

	};

	ful_inline
	const unit_utf8 * decode(const unit_utf8 * begin, point_utf & p)
	{
		if ((*begin & 0x80) == 0)
		{
			p = point_utf{uint32(begin[0])};
			return begin + 1;
		}
		else if ((*begin & 0x20) == 0)
		{
			ful_unused(ful_expect((*begin & 0xe0) == 0xc0));
			ful_unused(ful_expect((*(begin + 1) & 0xc0) == 0x80));

			p = point_utf{uint32(begin[0] & 0x1f) << 6 | uint32(begin[1] & 0x3f)};
			return begin + 2;
		}
		else if ((*begin & 0x10) == 0)
		{
			ful_unused(ful_expect((*begin & 0xf0) == 0xe0));
			ful_unused(ful_expect((*(begin + 1) & 0xc0) == 0x80));
			ful_unused(ful_expect((*(begin + 2) & 0xc0) == 0x80));

			p = point_utf{uint32(begin[0] & 0x0f) << 12 | uint32(begin[1] & 0x3f) << 6 | uint32(begin[2] & 0x3f)};
			return begin + 3;
		}
		else
		{
			ful_unused(ful_expect((*begin & 0xf8) == 0xf0));
			ful_unused(ful_expect((*(begin + 1) & 0xc0) == 0x80));
			ful_unused(ful_expect((*(begin + 2) & 0xc0) == 0x80));
			ful_unused(ful_expect((*(begin + 3) & 0xc0) == 0x80));

			p = point_utf{uint32(begin[0] & 0x07) << 18 | uint32(begin[1] & 0x3f) << 12 | uint32(begin[2] & 0x3f) << 6 | uint32(begin[3] & 0x3f)};
			return begin + 4;
		}
	}

	ful_inline
	const unit_utf16 * decode(const unit_utf16 * begin, point_utf & p)
	{
		if ((*begin & 0xfc00) != 0xd800)
		{
			ful_unused(ful_expect((*begin & 0xfc00) != 0xdc00));

			p = point_utf{begin[0]};
			return begin + 1;
		}
		else
		{
			ful_unused(ful_expect((*(begin + 1) & 0xfc00) == 0xdc00));

			// 01 23 - utf16le
			//   begin[0] = 10
			//   begin[1] = 32
			// 10 32 - utf16be
			//   begin[0] = 10
			//   begin[1] = 32
			p = point_utf{(uint32(begin[0] & 0x03ff) << 10 | uint32(begin[1] & 0x03ff)) + 0x10000};
			return begin + 2;
		}
	}

	ful_inline
	const unit_utf61 * decode(const unit_utf61 * begin, point_utf & p)
	{
		if ((*begin & 0x00fc) != 0x00d8)
		{
			ful_unused(ful_expect((*begin & 0x00fc) != 0x00dc));

			p = point_utf{load_swap(reinterpret_cast<const uint16 *>(begin))};
			return begin + 1;
		}
		else
		{
			ful_unused(ful_expect((*(begin + 1) & 0x00fc) == 0x00dc));

			// 01 23 - utf16le
			//   chunk = 3210
			// 10 32 - utf16be
			//   chunk = 1032
			const uint32 chunk = load_swap(reinterpret_cast<const uint32 *>(begin));
#if (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))) || (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
			p = point_utf{((chunk & 0x03ff0000) >> 6 | (chunk & 0x000003ff)) + 0x10000};
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			p = point_utf{((chunk & 0x000003ff) << 10 | (chunk & 0x03ff0000) >> 16) + 0x10000};
#else
# error Missing implementation!
#endif
			return begin + 2;
		}
	}

	ful_inline
	const unit_utf32 * decode(const unit_utf32 * begin, point_utf & p)
	{
		p = point_utf{static_cast<uint32>(begin[0])};
		return begin + 1;
	}

	ful_inline
	const unit_utf23 * decode(const unit_utf23 * begin, point_utf & p)
	{
		p = point_utf{load_swap(reinterpret_cast<const uint32 *>(begin))};
		return begin + 1;
	}

	ful_inline
	unit_utf8 * encode(point_utf p, unit_utf8 * begin)
	{
		if (p < 0x80)
		{
			begin[0] = static_cast<uint8>(p);
			return begin + 1;
		}
		else if (p < 0x800)
		{
			begin[0] = static_cast<unit_utf8>(static_cast<uint16>(p) >> 6 | 0xc0);
			begin[1] = static_cast<unit_utf8>((static_cast<uint16>(p) & 0x3f) | 0x80);
			return begin + 2;
		}
		else if (p < 0x10000)
		{
			begin[0] = static_cast<unit_utf8>(static_cast<uint32>(p) >> 12 | 0xe0);
			begin[1] = static_cast<unit_utf8>((static_cast<uint32>(p) >> 6 & 0x3f) | 0x80);
			begin[2] = static_cast<unit_utf8>((static_cast<uint32>(p) & 0x3f) | 0x80);
			return begin + 3;
		}
		else if (ful_expect(p < 0x110000))
		{
			begin[0] = static_cast<unit_utf8>(static_cast<uint32>(p) >> 18 | 0xf0);
			begin[1] = static_cast<unit_utf8>((static_cast<uint32>(p) >> 12 & 0x3f) | 0x80);
			begin[2] = static_cast<unit_utf8>((static_cast<uint32>(p) >> 6 & 0x3f) | 0x80);
			begin[3] = static_cast<unit_utf8>((static_cast<uint32>(p) & 0x3f) | 0x80);
			return begin + 4;
		}
		else // unexpected
		{
			return begin;
		}
	}

	ful_inline
	unit_utf16 * encode(point_utf p, unit_utf16 * begin)
	{
		if (p < 0x10000)
		{
			begin[0] = static_cast<uint16>(p);
			return begin + 1;
		}
		else
		{
			const uint32 split = static_cast<uint32>(p) - 0x10000;
			begin[0] = static_cast<unit_utf16>((split >> 10) | 0xd800);
			begin[1] = static_cast<unit_utf16>((split & 0x03ff) | 0xdc00);
			return begin + 2;
		}
	}

	ful_inline
	unit_utf61 * encode(point_utf p, unit_utf61 * begin)
	{
		if (p < 0x10000)
		{
			store_swap(begin, static_cast<uint16>(p));
			return begin + 1;
		}
		else
		{
			//   chunk = 3210
			// 01 23 - utf16le
			//   chunk = 1032
			// 10 32 - utf16be
			const uint32 split = static_cast<uint32>(p) - 0x10000;
#if (defined(_MSC_VER) && (defined(_M_X64) || defined(_M_AMD64) || defined(_M_IX86))) || (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
			const uint32 chunk = (split & 0x03ff0000) | (split & 0x000003ff) | 0xd800dc00;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
			const uint32 chunk = (split >> 10) | (split << 16 & 0x03ff0000) | 0xdc00d800;
#endif
			store_swap(reinterpret_cast<uint32 *>(begin), chunk);
			return begin + 2;
		}
	}

	ful_inline
	unit_utf32 * encode(point_utf p, unit_utf32 * begin)
	{
		begin[0] = static_cast<uint32>(p);
		return begin + 1;
	}

	ful_inline
	unit_utf23 * encode(point_utf p, unit_utf23 * begin)
	{
		store_swap(begin, static_cast<uint32>(p));
		return begin + 1;
	}

	namespace detail
	{
		struct no_char_type {};

		template <typename Begin, typename End>
		ful_inline Begin find(Begin, End, no_char_type) { ful_unreachable(); }
		template <typename R>
		ful_inline hck::iterator_t<R> find(R &&, no_char_type) { ful_unreachable(); }
		template <typename R>
		ful_inline hck::iterator_t<R> push_back(R &&, no_char_type) { ful_unreachable(); }
		template <typename R>
		ful_inline hck::iterator_t<R> append(R &&, usize, no_char_type) { ful_unreachable(); }
		template <typename R, typename At>
		ful_inline hck::iterator_t<R> insert(R &&, At, usize, no_char_type) { ful_unreachable(); }
		template <typename R>
		ful_inline hck::iterator_t<R> assign(R &&, usize, no_char_type) { ful_unreachable(); }
		template <typename R, typename From, typename To>
		ful_inline hck::iterator_t<R> replace(R &&, From, To, usize, no_char_type) { ful_unreachable(); }
		template <typename R>
		ful_inline void fill(R &&, no_char_type) { ful_unreachable(); }

		template <typename Char, typename T, ful_requires(sizeof(hck::compact_type_t<Char>) % sizeof(T) == 0)>
		ful_inline Char try_char(const T * buffer, int) { return static_cast<Char>(*reinterpret_cast<const hck::compact_type_t<Char> *>(buffer)); }
		template <typename Char, typename T>
		ful_inline no_char_type try_char(const T *, ...) { return no_char_type{}; }
	}

	template <typename Begin, typename End>
	ful_inline Begin find(Begin begin, End end, point_utf p)
	{
		point_buffer<hck::iter_value_t<Begin>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(T))
		{
		case 1: return find(begin, end, detail::try_char<char8>(buffer, 0));
		case 2: return find(begin, end, detail::try_char<char16>(buffer, 0));
		case 3: return find(begin, end, detail::try_char<char_fast24>(buffer, 0));
		case 4: return find(begin, end, detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R>
	ful_inline hck::iterator_t<R> find(R && x, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return find(hck::forward<R>(x), detail::try_char<char8>(buffer, 0));
		case 2: return find(hck::forward<R>(x), detail::try_char<char16>(buffer, 0));
		case 3: return find(hck::forward<R>(x), detail::try_char<char_fast24>(buffer, 0));
		case 4: return find(hck::forward<R>(x), detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R>
	ful_inline hck::iterator_t<R> push_back(R && x, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return push_back(hck::forward<R>(x), detail::try_char<char8>(buffer, 0));
		case 2: return push_back(hck::forward<R>(x), detail::try_char<char16>(buffer, 0));
		case 3: return push_back(hck::forward<R>(x), detail::try_char<char_fast24>(buffer, 0));
		case 4: return push_back(hck::forward<R>(x), detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R>
	ful_inline hck::iterator_t<R> append(R && x, usize count, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return append(hck::forward<R>(x), count, detail::try_char<char8>(buffer, 0));
		case 2: return append(hck::forward<R>(x), count, detail::try_char<char16>(buffer, 0));
		case 3: return append(hck::forward<R>(x), count, detail::try_char<char_fast24>(buffer, 0));
		case 4: return append(hck::forward<R>(x), count, detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R, typename At>
	ful_inline hck::iterator_t<R> insert(R && x, At at, usize count, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return insert(hck::forward<R>(x), at, count, detail::try_char<char8>(buffer, 0));
		case 2: return insert(hck::forward<R>(x), at, count, detail::try_char<char16>(buffer, 0));
		case 3: return insert(hck::forward<R>(x), at, count, detail::try_char<char_fast24>(buffer, 0));
		case 4: return insert(hck::forward<R>(x), at, count, detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R>
	ful_inline hck::iterator_t<R> assign(R && x, usize count, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return assign(hck::forward<R>(x), count, detail::try_char<char8>(buffer, 0));
		case 2: return assign(hck::forward<R>(x), count, detail::try_char<char16>(buffer, 0));
		case 3: return assign(hck::forward<R>(x), count, detail::try_char<char_fast24>(buffer, 0));
		case 4: return assign(hck::forward<R>(x), count, detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R, typename From, typename To>
	ful_inline hck::iterator_t<R> replace(R && x, From from, To to, usize count, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return replace(hck::forward<R>(x), from, to, count, detail::try_char<char8>(buffer, 0));
		case 2: return replace(hck::forward<R>(x), from, to, count, detail::try_char<char16>(buffer, 0));
		case 3: return replace(hck::forward<R>(x), from, to, count, detail::try_char<char_fast24>(buffer, 0));
		case 4: return replace(hck::forward<R>(x), from, to, count, detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}

	template <typename R>
	ful_inline void fill(R && x, point_utf p)
	{
		point_buffer<hck::range_value_t<R>, 1> buffer;
		const auto it = encode(p, buffer);

		switch ((it - buffer) * sizeof(hck::range_value_t<R>))
		{
		case 1: return fill(hck::forward<R>(x), detail::try_char<char8>(buffer, 0));
		case 2: return fill(hck::forward<R>(x), detail::try_char<char16>(buffer, 0));
		case 3: return fill(hck::forward<R>(x), detail::try_char<char_fast24>(buffer, 0));
		case 4: return fill(hck::forward<R>(x), detail::try_char<char32>(buffer, 0));
		default: ful_unreachable();
		}
	}
}
