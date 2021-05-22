#pragma once

// #include "ful/stdint.hpp"
// #include "ful/types.hpp"


namespace ful
{
	inline unsigned int point_size(const unit_utf8 * s)
	{
		if (static_cast<signed char>(*s) >= 0)
			return 1;

		if ((static_cast<signed char>(*s) & 0x20) == 0)
			return 2;

		if ((static_cast<signed char>(*s) & 0x10) == 0)
			return 3;

		return 4;
	}

	inline const unit_utf8 * point_next(const unit_utf8 * s) { return s + point_size(s); }

	inline const unit_utf8 * point_next(const unit_utf8 * s, usize n)
	{
		extern const unit_utf8 * (* point_next_dispatch)(const unit_utf8 * s, usize n);

		return point_next_dispatch(s, n);
	}

	inline const unit_utf8 * point_prev(const unit_utf8 * s)
	{
		--s;
		if ((*s & 0x80) == 0)
			return s;

		--s;
		if (*s & 0x40)
			return s;

		--s;
		if (*s & 0x40)
			return s;

		--s;
		return s;
	}
}
