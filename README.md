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

* asmlib - https://www.agner.org/optimize/#asmlib

  Vectorized assembly implementation of most functionality in
  `<cstring>` (well, it is a C library so it should be
  `<string.h>`). It has good performance but is it faster than inlined
  algorithms?

## Inspiration

* Is it okay to cast to simdtypes? https://stackoverflow.com/a/52117639
* Is it okay to read past end? https://stackoverflow.com/a/37801845
