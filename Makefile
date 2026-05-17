CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Iinclude

TEST_SRCS = $(wildcard tests/*.c)

.PHONY: all test clean

all: test

test:
	@mkdir -p build
	@for f in tests/*.c; do \
		name=$$(basename $$f .c); \
		echo "Compiling $$f -> build/$$name"; \
		$(CC) $(CFLAGS) $$f -o build/$$name || exit 1; \
		echo "Running $$name"; \
		./build/$$name || exit 1; \
	done
	@echo "All tests passed"

clean:
	rm -rf build
