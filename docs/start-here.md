# Start here

A cold-start walkthrough for **undying-allocator**: what to run, in what order,
and what "good" looks like on the current tree. It should take a minute.

## Prerequisites

- A C89 compiler on `PATH`: `cc` or `gcc`.
- `make` on `PATH` (the `Makefile` defaults to `cc`; if your compiler is
  `gcc` instead, pass `make CC=gcc`).
- Nothing else — the code uses the C standard library only.

## 1. Smoke test

From the repository root:

```bash
./scripts/smoke.sh
```

The script runs `make clean`, `make` and `make check`, then `./main` and
`./test_core`. On success it ends with:

```text
The rune holds.
```

## 2. Or compile by hand (no make)

These are the two lines from the README:

```bash
cc -std=c89 -Wall -O2 -Isrc -o main main.c src/rune_core.c src/undying_allocator.c -lm
cc -std=c89 -Wall -O2 -Isrc -o test_core tests/test_core.c src/rune_core.c src/undying_allocator.c -lm
./test_core && ./main
```

## 3. What success looks like

- `./test_core` prints one line:

  ```text
  c tests pass
  ```

- `./main` prints a step report and a formula, then the arena line and its
  reset line:

  ```text
  arena alloc_a=yes alloc_b=yes exhausted=yes used=212 high=212 failed=1 check=ok
  arena reset used=0 high=212 resets=1
  ```

  `alloc_a=yes` / `alloc_b=yes` mean both pools came up, `exhausted=yes` is
  the expected bounded failure, `failed=1` counts it, and `check=ok` says the
  canaries survived. The reset line shows the arena reused with `used=0` and
  `resets=1`.

If any of that differs, stop and read the failure — the smoke script prints the
build or test log instead of hiding it.

## Where to go next

- `ARCHIVE_NOTE.md` — what this repository is and is not (the honesty note).
- `CONTRIBUTING.md` — how to propose a change.
- `docs/why-canaries.md` — a hand-worked one-byte overrun that kills the tail fence.
- `src/undying_allocator.c` — the allocator itself.
