#if HAVE_EASTL
# include "ful/compiler.hpp"

# include <EASTL/string.h>

#if defined(_MSC_VER)
# define ful_cdecl __cdecl
#elif defined(__GNUC__)
# define ful_cdecl
#endif

void * ful_cdecl operator new[](size_t size, const char * name, int flags, unsigned debugFlags, const char * file, int line)
{
	ful_unused(name);
	ful_unused(flags);
	ful_unused(debugFlags);
	ful_unused(file);
	ful_unused(line);
	return new uint8_t[size];
}
#endif
