/*
-- Support uint128_t until official support becomes available.
--
-- Copyright (C) 2020, Mark Gardner <mkg@vt.edu>.
--
-- This file is part of uint128.
--
-- uint128 is free software: you can redistribute it and/or modify it under the
-- terms of the GNU Lesser General Public License as published by the Free
-- Software Foundation, either version 3 of the License, or (at your option)
-- any later version.
--
-- uint128 is distributed in the hope that it will be useful, but WITHOUT ANY
-- WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
-- FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
-- more details.
--
-- You should have received a copy of the GNU Lesser General Public License
-- along with uint128. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef __UINT128_H
#define __UINT128_H

#include <byteswap.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* --- 128-bit equivalent to man (3) stdint.h or inttypes.h --- */

// Only available with 64-bit gcc
#if !defined(uint128_t)
typedef __uint128_t uint128_t;
#endif

static inline uint128_t make_uint128(uint32_t x[4]) {
  return
    ((uint128_t) x[0]) << 96 |
    ((uint128_t) x[1]) << 64 |
    ((uint128_t) x[2]) << 32 |
     (uint128_t) x[3];
}


/* --- 128-bit substitute for man (3) snprintf --- */

// This probably should not be a inline function but it is the only in the
// library (so far)...
static inline char *stringify_uint128(char *str, size_t size, uint128_t x) {
  char buf[2 * sizeof(uint128_t) + 3];
  strncpy(buf, "0x", sizeof(buf));
  for (int o = 2, s = 96; o < 32; o += 8, s -= 32) {
    snprintf(buf +  o, 9, "%08x", (uint32_t) (x >> s));
  }
  memcpy(str, buf, size);
  return str;
}


/* --- 128-bit equivalent to man (3) bswap --- */

static inline uint128_t bswap_128 (uint128_t x) {
  uint64_t hi = (uint64_t) (x >> 64);
  uint64_t lo = (uint64_t) x;
  return ((uint128_t) bswap_64(lo)) << 64 | (uint128_t) bswap_64(hi);
}


/* --- 128-bit equivalents to man (3) endian --- */

static inline uint128_t htobe128(uint128_t x) {
  #if BYTE_ORDER == BIG_ENDIAN
  // noop
  #elif BYTE_ORDER == LITTLE_ENDIAN
  x = bswap_128(x);
  #endif
  return x;
}


static inline uint128_t htole128(uint128_t x) {
  #if BYTE_ORDER == LITTLE_ENDIAN
  // noop
  #elif BYTE_ORDER == BIG_ENDIAN
  return bswap_128(x);
  #endif
  return x;
}


static inline uint128_t be128toh(uint128_t x) {
  #if BYTE_ORDER == BIG_ENDIAN
  // noop
  #elif BYTE_ORDER == LITTLE_ENDIAN
  return bswap_128(x);
  #endif
  return x;
}


static inline uint128_t le128toh(uint128_t x) {
  #if BYTE_ORDER == LITTLE_ENDIAN
  // noop
  #elif BYTE_ORDER == BIG_ENDIAN
  return bswap_128(x);
  #endif
  return x;
}


#endif // __UINT128_H
