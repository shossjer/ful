#pragma once

namespace ful
{
	using unit_utf8 = char;
	using unit_utf32 = char32_t;
#if defined(_MSC_VER)
	using unit_utfw = wchar_t;
#endif
}
