#pragma once

#if defined(FUL_IFUNC)
# define ful_dispatch(stem) stem##_rtd
#elif defined(FUL_FPTR)
# define ful_dispatch(stem) (* stem##_rtd)
#endif
