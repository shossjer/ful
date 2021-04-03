#pragma once

namespace ful
{
#if defined(__SIZE_TYPE__)
	using usize = __SIZE_TYPE__;
#elif defined(_MSC_VER)
# if defined(_WIN64)
	using usize = unsigned long long;
# else
	using usize = unsigned int;
# endif
#else
# error Missing implementation!
#endif

	using uptr = usize;
	static_assert(sizeof(uptr) == sizeof(void *), "uptr must have the same size as a pointer!");

#if defined(__CHAR_BIT__)
	constexpr const usize byte_size = __CHAR_BIT__;
#elif defined(_MSC_VER)
	constexpr const usize byte_size = 8;
#else
# error Missing implementation!
#endif
}
