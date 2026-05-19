CC = gcc
CFLAGS = -std=c11 -Iinclude -Wno-ignored-qualifiers

SHELL := /bin/sh

.PHONY: all test test-pass test-fail clean

all: test

test: test-pass test-fail

test-pass:
	@sh scripts/run_success_tests.sh

test-fail:
	@sh scripts/run_expected_fail_tests.sh
	@echo "All tests passed"

clean:
	rm -rf build
