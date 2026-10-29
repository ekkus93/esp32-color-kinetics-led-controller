#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
TEST_BIN="$(mktemp)"
trap 'rm -f "$TEST_BIN"' EXIT
cc -std=c11 -O2 -Wall -Wextra -Werror -pedantic -I main \
  tests/test_chromasic.c main/chromasic_protocol.c -o "$TEST_BIN"
"$TEST_BIN"
