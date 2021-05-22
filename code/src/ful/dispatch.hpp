#pragma once

#if defined(FUL_IFUNC)
# define ful_dispatch(stem) stem
#elif defined(FUL_FPTR)
# define ful_dispatch(stem) (* stem)
#endif
