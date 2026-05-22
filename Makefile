CC = gcc
CFLAGS = -std=c11 -Iinclude -Wno-ignored-qualifiers

SHELL := /bin/sh

PREFIX ?= /usr/local

.PHONY: all test test-pass test-fail test-asan clean install uninstall

all: test

test: test-pass test-fail

test-pass:
	@sh scripts/run_success_tests.sh

test-fail:
	@sh scripts/run_expected_fail_tests.sh
	@echo "All tests passed"

test-asan:
	@echo "Running success tests with AddressSanitizer and UndefinedBehaviorSanitizer..."
	@CC=$(CC) CFLAGS="-std=c11 -Wall -Wextra -Wno-psabi -Iinclude -Itests -fsanitize=address,undefined -g" sh scripts/run_success_tests.sh
	@echo "Running expected fail tests with AddressSanitizer and UndefinedBehaviorSanitizer..."
	@CC=$(CC) CFLAGS="-std=c11 -Wall -Wextra -Wno-psabi -Iinclude -Itests -fsanitize=address,undefined -g" sh scripts/run_expected_fail_tests.sh
	@echo "All AddressSanitizer/UndefinedBehaviorSanitizer tests passed"

clean:
	rm -rf build

install:
	mkdir -p $(DESTDIR)$(PREFIX)/include/my_c_utils
	cp -rp include/my_c_utils/* $(DESTDIR)$(PREFIX)/include/my_c_utils/

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/include/my_c_utils
