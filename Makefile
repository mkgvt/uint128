## Support uint128_t until official support becomes available.
##
## Copyright (C) 2020, Mark Gardner <mkg@vt.edu>.
##
## This file is part of uint128.
##
## uint128 is free software: you can redistribute it and/or modify it under the
## terms of the GNU Lesser General Public License as published by the Free
## Software Foundation, either version 3 of the License, or (at your option)
## any later version.
##
## uint128 is distributed in the hope that it will be useful, but WITHOUT ANY
## WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
## FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
## more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with uint128. If not, see <https://www.gnu.org/licenses/>.

CC     ?= gcc
CFLAGS  = -fPIC -I. -Wall -Wpedantic -Wextra
LDFLAGS = -lcgreen
DFLAGS  = -MM -MF

TESTSRC = $(wildcard *_tests.c)
TESTOBJ = ${TESTSRC:.c=.o}
TESTDEP = ${TESTSRC:.c=.d}
TESTLIB = ${TESTSRC:.c=.so}
MDSRC   = $(wildcard *.md)
HTML    = ${MDSRC:.md=.html}

.PHONY: all html tests
all: html tests

tests: $(TESTLIB)
	@for test in $(TESTLIB) ; do cgreen-runner $${test} ; done

%_tests.so: %_tests.o
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c | %.d
	$(CC) $(CFLAGS) -c $^

%.d: %.c
	$(CC) $(DFLAGS) $(patsubst %.c,%.d,$^) $^

html: $(HTML)

%.html: %.md
	pandoc -o $@ $^

.PHONY: clean distclean
clean:
	rm -f $(TESTDEP) $(TESTOBJ) $(TESTLIB) *.gch *.d $(HTML)

distclean: clean
	rm -f *~

-include $(TESTDEP)
