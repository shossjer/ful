#pragma once

#if defined(__clang__)
// optimize knowing that the expression is true
# define ful_assume(x) __builtin_assume(x)
#elif defined(__GNUC__)
// optimize knowing that the expression is true
# define ful_assume(x) [](bool c){ if (!c) ful_unreachable(); }(x)
#elif defined(_MSC_VER)
// optimize knowing that the expression is true
# define ful_assume(x) __assume(x)
#else
# error Missing implementation!
#endif

#if defined(__clang__)
# if __has_builtin(__builtin_debugtrap)
// break into the debugger
#  define ful_break() __builtin_debugtrap()
# else
#  error Missing implementation!
# endif
#elif defined(_MSC_VER)
// break into the debugger
# define ful_break() __debugbreak()
#elif defined(__x86_64__) || defined(__i386__)
// break into the debugger
# define ful_break() [](){ __asm__("int3"); }()
#else
# error Missing implementation!
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
// breaks into the debugger if false (in debug builds)
# define ful_check(x) ((x) ? true : (ful_break(), false))
#else
// breaks into the debugger if false (in debug builds)
# define ful_check(x) !!(x)
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
// breaks into the debugger if false (in debug builds), optimize knowing that the expression is true (in nondebug builds)
# define ful_expect(x) ((x) ? true : (ful_break(), false))
#else
// breaks into the debugger if false (in debug builds), optimize knowing that the expression is true (in nondebug builds)
# define ful_expect(x) (ful_assume(x), true)
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

# define ful_unused(x) static_cast<void>(x)
