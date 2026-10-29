#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

# Bash syntax and shell conventions.
bash -n tests/run_lint.sh tests/run_host_tests.sh
shellcheck --severity=warning tests/run_lint.sh tests/run_host_tests.sh

# Host-testable C only; firmware-specific C is checked by the IDF build.
cc -std=c11 -Wall -Wextra -Werror -pedantic -fsyntax-only -I main \
  main/chromasic_protocol.c tests/test_chromasic.c

# Check whitespace when run in a Git checkout.
if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  git diff --check
  git diff --cached --check
fi
printf 'PASS: Bash syntax, ShellCheck, strict C11 syntax, Git whitespace\n'
