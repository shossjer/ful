#pragma once

#ifndef FUL_PAGE_SIZE
// https://en.wikipedia.org/wiki/Page_(computer_memory)#Multiple_page_sizes
# if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__) || \
     (defined(_MSC_VER) && defined(_M_IX86)) || defined(__i386__)
#  define FUL_PAGE_SIZE 4096
# else
#  error Missing implementation!
# endif
#endif
