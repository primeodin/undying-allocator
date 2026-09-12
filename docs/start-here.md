# Start Here

This guide gets you from a fresh checkout to a working build and test run.

## Prerequisites

You need:

* A C89 compiler such as `cc` or `gcc`
* `make`
* A standard C library

No third-party packages are required.

## Run the project

Clone the repository and enter it:

```bash
git clone https://github.com/primeodin/undying-allocator.git
cd undying-allocator
```

The easiest way to build and test everything is:

```bash
./scripts/smoke.sh
```

The smoke script builds the project, runs the tests, and then runs the example program.

### Without make

You can also compile the two programs directly:

```bash
cc -std=c89 -Wall -O2 -Isrc -o main main.c src/rune_core.c src/undying_allocator.c -lm
cc -std=c89 -Wall -O2 -Isrc -o test_core tests/test_core.c src/rune_core.c src/undying_allocator.c -lm
```

Then run them:

```bash
./test_core && ./main
```

## What success looks like

The smoke script should finish with:

```text
The rune holds.
```

Running `./test_core` should print:

```text
c tests pass
```

`./main` should print an arena line containing:

```text
alloc_a=yes alloc_b=yes exhausted=yes ... failed=1 check=ok
```

It should then print a reset line containing:

```text
used=0
resets=1
```

## Pointers

* [`ARCHIVE_NOTE.md`](../ARCHIVE_NOTE.md) — explains the retrospective nature of the repository.
* [`CONTRIBUTING.md`](../CONTRIBUTING.md) — contribution guidelines.
* [`docs/why-canaries.md`](why-canaries.md) — explains the allocator's canary checks.
* [`src/undying_allocator.c`](../src/undying_allocator.c) — the allocator implementation.
