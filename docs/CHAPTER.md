
# 2006 chapter notes — undying-allocator

**Archive status:** honest retrospective reconstruction. This chapter is designed to *feel* period-aware without claiming to be a public artifact from 2006.

## Origin

A guarded block allocator and rage-load stress harness. The small machine here is deliberately humble. It is a bench artifact: a thing to run, break, and inspect.

## Engineering constraints

- Keep dependencies era-shaped and minimal.
- Prefer transparent math and explicit state over clever abstraction.
- Preserve the primeodin voice, but never let mythic language replace technical clarity.
- Keep `ARCHIVE_NOTE.md` intact if the repository is pushed or forked.

## What to inspect first

1. Read `README.md` for the winter narrative.
2. Run `scripts/smoke.sh`.
3. Open the main source file and change one constant.
4. Record what changed in the output.

## Rune invariant

The chapter invariant is: **the example must be small enough that a curious reader can hold the whole machine in mind.**
