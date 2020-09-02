# uint128

A C library supporting operations on 128-bit unsigned integers

## Table of Contents

* [Introduction](#introduction)
* [Build and Test](#build-and-test)
* [Usage](#usage)
* [Future](#future)
* [Dependencies](#dependencies)

## Introduction

The [C standard](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2310.pdf)
does not yet support 128-bit usigned integers but they are useful for
working with very wide data types such as IPv6 addresses. Some
compilers have internal support for 128-bit unsigned integers. For
example, gcc and clang have the `__uint128_t type`, at least on 64-bit
systems. This library aims to provide `uint128_t` until officially
supported by the C standard.

To the degree possible, operations on `uint128_t` follows standards and existing
conventions. For example, functions for swapping bytes around to convert from
big endian to little endian follows established patterns from endian.h.

Some operations on 128-bit unsigned integers are clunky without compiler and
library support. Assigning a 128-bit unsigned integer literal to a variable
requires a function call. Converting a 128-bit unsigned value to a string
requires a function.

## Build and Test

The default target in the make file builds the shared library and
documentation then runs the test. The default compiler is gcc but clang works
as well.

```shell
$ make
gcc -MM -MF uint128_tests.d uint128_tests.c
pandoc -o README.html README.md
gcc -fPIC -I. -Wall -Wpedantic -Wextra -c uint128_tests.c uint128.h
gcc -shared -o uint128_tests.so uint128_tests.o -lcgreen
Running "uint128_tests" (7 tests)...
  "uint128_tests": 14 passes in 1ms.
Completed "uint128_tests": 14 passes in 1ms.
$ make clean
$ CC=clang make
clang -MM -MF uint128_tests.d uint128_tests.c
pandoc -o README.html README.md
clang -fPIC -I. -Wall -Wpedantic -Wextra -c uint128_tests.c uint128.h
clang -shared -o uint128_tests.so uint128_tests.o -lcgreen
Running "uint128_tests" (7 tests)...
  "uint128_tests": 14 passes in 1ms.
Completed "uint128_tests": 14 passes in 1ms.
```

## Usage

The library consists of a single header file containing the implementation.
Include the header file to use the library.

```C
#include "uint128.h"

int main(void) {
  // Create a 128-bit unsigned value from four 32-bit unsigned values.
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);

  // Print the hexidecimal representation of a 128-bit unsigned value
  // to a buffer. Note: make sure there is room for 0x and \0.
  char cbuf[2 * sizeof(uint128_t) + 3];
  char *str = snprint_uint128(cbuf, sizeof(cbuf), v128);

  // Swap the bytes from big endian to little endian or vice versa.
  uint128_t s128 = bswap_128(v128);

  // Convert from host endian to big endian.
  uint128_t be128 = htobe128(v128);

  // Convert from host endian to little endian.
  uint128_t le128 = htole128(v128);

  // Convert from big endian to host endian.
  uint128_t h128 = be128toh(v128);

  // Convert from little endian to host endian.
  uint128_t H128 = le128toh(v128);

  return 0;
}
```

## Future

The hope is that the `uint128_t` type will become part of the C standard in
some form and make this library obsolete. That fortunate outcome will
eliminate the awkwardness which requires the `make_uint128` and
`snprint_uint128` functions. In the mean time, the library will likely expand
to support more functions on `uint128_t` types as the need arises.

## Dependencies

* C compiler: [gcc](https://gcc.gnu.org/) 8.3.0 and [clang](https://clang.llvm.org/) 7.1.0 have been tested
* [Cgreen](https://cgreen-devs.github.io/): testing framework
* [pandoc](https://pandoc.org/): 2.7.3 has been tested
