#!/usr/bin/env bash
set -euo pipefail
make clean >/dev/null 2>&1 || true
make >/tmp/undying-allocator.build.log 2>&1 || { cat /tmp/undying-allocator.build.log; exit 1; }
make check >/tmp/undying-allocator.test.log 2>&1 || { cat /tmp/undying-allocator.test.log; exit 1; }
./main
./test_core
echo "The rune holds."
