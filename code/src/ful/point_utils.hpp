#pragma once

// #include "ful/types.hpp"

namespace ful
{
	inline unsigned int point_size(const unit_utf8 * s)
	{
		if (static_cast<sint8>(*s) >= 0)
			return 1;

		if ((static_cast<sint8>(*s) & 0x20) == 0)
			return 2;

		if ((static_cast<sint8>(*s) & 0x10) == 0)
			return 3;

		return 4;
	}

	inline const unit_utf8 * point_next(const unit_utf8 * s)
	{
		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		++s;
		return s;
	}

	inline const unit_utf8 * point_prev(const unit_utf8 * s)
	{
		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		if (static_cast<sint8>(*s) > -65)
			return s;

		--s;
		return s;
	}
}
