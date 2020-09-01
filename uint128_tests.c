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
#include <assert.h>
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "uint128.h"

Describe(uint128_tests);
BeforeEach(uint128_tests) {}
AfterEach(uint128_tests) {}

//-------------------------------------------------------------------------------

Ensure(uint128_tests, make_uint128_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  assert_that((uint32_t) (v128 >> 96), is_equal_to(a32[0]));
  assert_that((uint32_t) (v128 >> 64), is_equal_to(a32[1]));
  assert_that((uint32_t) (v128 >> 32), is_equal_to(a32[2]));
  assert_that((uint32_t)  v128,        is_equal_to(a32[3]));
}

Ensure(uint128_tests, stringify_uint128_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  char cbuf[2 * sizeof(uint128_t) + 3];  // leave room for 0x prefix and \0
  char *str = stringify_uint128(cbuf, sizeof(cbuf), v128);
  assert_that(str, is_equal_to(cbuf));
  assert_that(cbuf, is_equal_to_string("0x000102030405060708090a0b0c0d0e0f"));
}

Ensure(uint128_tests, bswap_128_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t s128 = bswap_128(v128);
  assert_that((uint32_t) (s128 >> 96), is_equal_to(bswap_32(a32[3])));
  assert_that((uint32_t) (s128 >> 64), is_equal_to(bswap_32(a32[2])));
  assert_that((uint32_t) (s128 >> 32), is_equal_to(bswap_32(a32[1])));
  assert_that((uint32_t) s128,         is_equal_to(bswap_32(a32[0])));
}

Ensure(uint128_tests, htobe128_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t be128 = htobe128(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(be128, is_equal_to(v128));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(be128, is_equal_to(bswap_128(v128)));
#endif
}

Ensure(uint128_tests, htole128_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t le128 = htole128(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(le128, is_equal_to(bswap_128(v128)));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(le128, is_equal_to(v128));
#endif
}

Ensure(uint128_tests, be128toh_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t h128 = be128toh(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(h128, is_equal_to(v128));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(h128, is_equal_to(bswap_128(v128)));
#endif
}

Ensure(uint128_tests, le128toh_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t h128 = le128toh(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(h128, is_equal_to(bswap_128(v128)));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(h128, is_equal_to(v128));
#endif
}

Ensure(uint128_tests, ne128tohe_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t h128 = be128toh(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(h128, is_equal_to(v128));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(h128, is_equal_to(bswap_128(v128)));
#endif
}

Ensure(uint128_tests, he128tone_is_valid) {
  uint32_t a32[4] = {0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};
  uint128_t v128 = make_uint128(a32);
  uint128_t h128 = be128toh(v128);
#if BYTE_ORDER == BIG_ENDIAN
  assert_that(h128, is_equal_to(v128));
#elif BYTE_ORDER == LITTLE_ENDIAN
  assert_that(h128, is_equal_to(bswap_128(v128)));
#endif
}

//-------------------------------------------------------------------------------

TestSuite *uint128_tests() {
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, uint128_tests, make_uint128_is_valid);
    add_test_with_context(suite, uint128_tests, stringify_uint128_is_valid);
    add_test_with_context(suite, uint128_tests, bswap_128_is_valid);
    add_test_with_context(suite, uint128_tests, htobe128_is_valid);
    add_test_with_context(suite, uint128_tests, htole128_is_valid);
    add_test_with_context(suite, uint128_tests, be128toh_is_valid);
    add_test_with_context(suite, uint128_tests, le128toh_is_valid);
    add_test_with_context(suite, uint128_tests, ne128tohe_is_valid);
    add_test_with_context(suite, uint128_tests, he128tone_is_valid);
    
    return suite;
}
