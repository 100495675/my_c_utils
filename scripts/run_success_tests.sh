#!/bin/sh

set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
CC=${CC:-gcc}
CFLAGS=${CFLAGS:--std=c11 -Wall -Wextra -Wno-psabi -Iinclude -Itests}

BUILD_DIR="$ROOT/build/success"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

find "$ROOT/tests/unit" "$ROOT/tests/integration" -name '*.c' | sort | while IFS= read -r source; do
  rel=${source#"$ROOT/"}
  out="$BUILD_DIR/${rel#tests/}"
  out=${out%.c}
  mkdir -p "$(dirname -- "$out")"
  echo "Compiling $rel -> ${out#$ROOT/}"
  "$CC" $CFLAGS "$source" -o "$out"
  echo "Running ${out##*/}"
  "$out"
done