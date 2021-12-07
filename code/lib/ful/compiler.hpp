#pragma once

#ifndef __has_attribute
# define __has_attribute(x) 0
#endif

#if defined(__clang__)
// optimize knowing that the expression is true
# define ful_assume(x) __builtin_assume(x)
#elif defined(__GNUC__)
// optimize knowing that the expression is true
# define ful_assume(x) static_cast<void>((x) || (ful_unreachable(), false))
#elif defined(_MSC_VER)
// optimize knowing that the expression is true
# define ful_assume(x) __assume(x)
#else
# error Missing implementation!
#endif

#if defined(__clang__)
// clang does not handle big arguments correctly by value, so we make them ref
# define ful_big_value const &
#else
// clang does not handle big arguments correctly by value, so we make them ref
# define ful_big_value
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

#if defined(__GNUC__)
// silence warning about side effects
# define ful_const const __attribute__((const))
// silence warning about side effects
# define ful_pure __attribute__((const))
#else
// silence warning about side effects
# define ful_const const
// silence warning about side effects
# define ful_pure
#endif

#if defined(__GNUC__) && __has_attribute(ifunc)
// declare dispatch function
# define ful_dispatch(stem) stem##_rtd
#else
// declare dispatch function
# define ful_dispatch(stem) (* stem##_rtd)
#endif

#if defined(_DEBUG) || !defined(NDEBUG)
// breaks into the debugger if false (in debug builds), optimize knowing that the expression is true (in nondebug builds)
# define ful_expect(x) ((x) ? true : (ful_break(), false))
#else
// breaks into the debugger if false (in debug builds), optimize knowing that the expression is true (in nondebug builds)
# define ful_expect(x) (ful_assume(x), true)
#endif

#if defined(__GNUC__)
// silence warning about fall through in switches
# define ful_fallthrough __attribute__((fallthrough))
#else
// silence warning about fall through in switches
# define ful_fallthrough do {} while(0)
#endif

#if defined(_MSC_VER)
// optimize this call away
# define ful_inline __forceinline
#else
// optimize this call away
# define ful_inline inline __attribute__((always_inline))
#endif

#if defined(__GNUC__) && __has_attribute(target)
// overrides targeted architecture
# define ful_target(...) __attribute__((target (__VA_ARGS__)))
// https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html#target
// https://gcc.gnu.org/onlinedocs/gcc/Function-Multiversioning.html
#else
// overrides targeted architecture
# define ful_target(...)
#endif

#if defined(__GNUC__)
// optimize knowing that this branch is impossible
# define ful_unreachable() __builtin_unreachable()
#elif defined(_MSC_VER)
// optimize knowing that this branch is impossible
# define ful_unreachable() __assume(0)
#else
# error Missing implementation!
#endif

// silence warning about unused variable
# define ful_unused(x) static_cast<void>(x)
