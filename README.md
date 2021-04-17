# Fast Unicode Library

String operations are common, they need to be made fast.

## But why?

This library came about from two ideas colliding:

* Unicode in C++ is an embarrassment.

* Which string operations can be made faster with simdtypes?

There are numerous libraries for Unicode processing already, why are
they not good enough? Most aim to be standard conforming, i.e. not
vectorized. Many are implemented in C with wrappers for C++. Some are
written to work together with `std::string` (which has enough problems
as it is already). None of these paths lead to optimal C++.

In order to guarantee some sort of performance improvements, this
library can run benchmarks against the following libraries with
implementations of strings (that may or may not contain some support
for Unicode):

* asmlib - https://www.agner.org/optimize/#asmlib

  Source code available and comes prebuilt for all its targeted
  platforms. It has good performance but is not a Unicode
  library. Only contains vectorized assembly implementations of
  `<cstring>`. Has published a benchmark comparing memcpy/strlen to
  various standard implementations (see
  https://www.agner.org/optimize/asmlib-instructions.pdf).

* eastl - https://github.com/electronicarts/EASTL

  CMake project. Implements an alternative to the standard C++
  library, adding a few utilities over the otherwise standard-like
  interface, supporting only UTF8, UCS2, and UCS4. Has source for a
  benchmark comparing string operations to the standard implementation
  (see
  https://github.com/electronicarts/EASTL/blob/3.17.06/benchmark/source/BenchmarkString.cpp).

* icu4c - http://site.icu-project.org/

  Autoconf project (i.e. not easy to build for Windows). There are,
  however, prebuilt libraries for Windows (although only dynamically
  built). Probably the only feature complete Unicode library. Has
  published a benchmark comparing collation (i.e. string comparison)
  to some functions in GLIBC (see
  http://site.icu-project.org/charts/collation-icu4c48-glibc).

* libiconv - https://www.gnu.org/software/libiconv/

  Autoconf project (i.e. not easy to build for Windows). Only contains
  conversion utilities between encodings.

* libunistring - https://www.gnu.org/software/libunistring/

  Autoconf project (i.e. not easy to build for Windows). Depends on
  libiconv. Adds most Unicode features.

* unicode - https://github.com/stepchowfun/unicode

  Embeddable project (i.e. no build scripts
  provided). Minimalistic. No changes since 2014.

* unicorn-lib - https://github.com/CaptainCrowbar/unicorn-lib

  Makefile project (i.e. not easy to build for Windows). Supports many
  Unicode features.

* unilib - https://github.com/ufal/unilib

  Embeddable project (i.e. no build scripts provided). Supports
  essential Unicode features.

* utf8proc - https://github.com/JuliaStrings/utf8proc

  Makefile/CMake project. Only contains some UTF-8 utilities.

* utfcpp - https://github.com/nemtrif/utfcpp

  CMake project. Contains utility functions to be used together with
  the standard string classes.

* utfpp - https://github.com/Alexhuszagh/UTFPP

  Makefile project (i.e. not easy to build for Windows). Only contains
  conversion utilities between encodings. Has published a benchmark
  comparing UTF8/16/32 conversions to ICU4C and libiconv (see
  https://github.com/Alexhuszagh/UTFPP#performance). No changes since
  2016.

Is your favorite _Unicode_ library not on the list? Tell me about it!

## Inspiration

* Is it okay to cast to simdtypes? https://stackoverflow.com/a/52117639
* Is it okay to read past end? https://stackoverflow.com/a/37801845
