#pragma once

#if defined(__clang__)
// optimize knowing that the expression x is true
# define ful_assume(x) __builtin_assume(x)
#elif defined(__GNUC__)
// optimize knowing that the expression x is true
# define ful_assume(x) while (!(x)) ful_unreachable()
#elif defined(_MSC_VER)
// optimize knowing that the expression x is true
# define ful_assume(x) __assume(x)
#else
# error Missing implementation!
#endif

#if defined(__GNUC__)
// optimize knowing that this branch is impossible
# define ful_unreachable() __builtin_unreachable()
#elif defined (_MSC_VER)
 // optimize knowing that this branch is impossible
# define ful_unreachable() __assume(0)
#else
# error Missing implementation!
#endif
