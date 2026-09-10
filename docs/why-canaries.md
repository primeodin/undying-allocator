# Why canaries (not silent overruns)

> Shop teaching for `undying-allocator`: a fixed arena can refuse a request, but a **one-byte write past the payload** is the quiet killer. Head + tail canaries make that bite visible.

## The trap in one picture

Each successful `undying_alloc` lays out a block like this (sizes approximate; alignment pads may sit between pieces):

```text
[ UndyingBlock header | head_canary=UALC ]
[ payload — what you asked for ]
[ tail canary = UALC ]
```

`UNDYING_CANARY` is `0x55414c43UL` — on a little-endian box the four bytes read `C L A U` ("UALC" if you squint the ASCII). `undying_check` walks every live block and fails the moment either canary is not that constant.

## Hand-worked demo (matches `tests/test_core.c`)

Arena: 192-byte stack buffer. Allocate **8** bytes. Fill only the legal payload. Then stomp **one byte past the end**.

| Step | What you do | `undying_check` |
| --- | --- | --- |
| 1 | `p = undying_alloc(&arena, 8)` | `ok` (1) |
| 2 | write `p[0]..p[7]` only | still `ok` |
| 3 | write `p[8] = 0x7f` (first byte of the **tail** canary) | **`bad` (0)** |

That is the whole lesson. The overrun did not crash immediately. It did not flip a magic OS flag. It **poisoned the fence** the allocator left for itself. Without the check, the next alloc or reset would be reading a lie.

Healthy path from `./main` on this tree (512-byte arena, 40-byte + 96-byte aligned alloc, then a too-large request):

```text
arena alloc_a=yes alloc_b=yes exhausted=yes used=232 high=232 failed=1 check=ok
arena reset used=0 high=232 resets=1
```

Exhaustion is loud (`failed=1`, pointer `NULL`). The canary trap is quiet until you call `undying_check`.

## Why both head and tail?

| Fence | Catches | Misses alone |
| --- | --- | --- |
| **Head** canary in the block header | Someone stomped metadata / walked pointers backward | A pure forward overrun that only hits the payload end |
| **Tail** canary just after the payload | Classic off-by-one / `memset` too long | Corruption that only rewrites the header |

Shop rule: **lookup ≠ payload** for attention; **fence ≠ payload** for memory. Keep the thing you check separate from the bytes the caller owns.

## Bounded failure vs unbounded hope

This arena:

1. Takes a **caller-supplied** buffer (no hidden `malloc` growth).
2. Bump-allocates with alignment.
3. Returns `NULL` and bumps `failed_count` / `failed_bytes` when the request will not fit.
4. Resets by rewinding `offset` (high-water and failure counters stay — you can still audit the siege).
5. Lets tests call `undying_check` after writes.

That is the opposite of "hope the heap is fine." Hope is not a strategy when the next traveler inherits your pointers.

## Shop judgment

| Situation | Do this |
| --- | --- |
| Teaching / tests / tools you own | Keep canaries + an explicit check. Fail closed. |
| Production hot path | Same idea, cheaper form (ASAN in CI, redzones in debug builds, size classes with cookies). Do not delete the fence because release "feels fine." |
| Tempted to skip `undying_check` after a `memset` | That is the moment you need it. |

If you change canary layout, update this doc and `test_allocator_canary_check` in the same PR — a fence nobody verifies is decoration.

## Run it

```bash
cc -std=c89 -Wall -O2 -Isrc -o test_core tests/test_core.c src/rune_core.c src/undying_allocator.c -lm
./test_core   # prints: c tests pass
```

Or `./scripts/smoke.sh` once `make` is on the PATH.
