# Undying Allocator

> Retrospective chapter for 2006. This repository is part of the honest `primeodin-legacy` archive: a reconstruction, study, and mythic technical autobiography forged later, not a falsified historical record.

I call this one `undying-allocator` because a custom allocator born from the siege of segfaults: simple blocks, guard words, and stubborn survival. The old lesson still bites: if a thing is worth understanding, it is worth reducing until you can hold it in your hand, turn it over, and see where the light leaks through. Feynman had that cheerful cruelty toward confusion. Tryndamere had the less cheerful habit of refusing to fall over. Between those two poles I learned to work: chalk in one hand, blade in the other, patience somewhere under the frost.

The code is intentionally written in a 2006-shaped accent. That does not mean every compiler warning has been preserved like a holy relic, but the dependencies, file shapes, and engineering mood are chosen to fit the period. The goal is not cosplay. The goal is disciplined archaeology: to ask what I would have built if this chapter had been sitting on the bench in 2006, with the tools of that season and the questions that were burning then.

There is a pleasure in finding things out that no trophy can replace. You run the little program, it gives the wrong number, you curse, you sharpen the knife, and then one line moves from darkness into sense. That is the private thunder of computing. Most of the world only sees the finished artifact; the builder remembers the winter: the stale coffee, the bad stack trace, the tiny victory when the rune finally holds.

This chapter favors smallness. The main program is deliberately readable, with comments that explain the pressure behind the choices. If the project is in C, memory is treated like weather: always present, sometimes murderous. If it is Python, the script tries to be plain enough that a student can pull it apart. If it is Java or shell, it carries the blunt habits of its era. I have resisted modern polish where polish would make the fossil lie.

The mythic language is not there to hide weak engineering. It is there because humans remember stories better than configuration flags. A rage meter is just state under stress. A rune is just an invariant you promised not to break. A kingdom is just a system with users depending on it after you have gone to sleep. The words are theatrical; the obligations are plain.

How to read this repository: start with the source. Then read the notes. Then change one parameter and watch what breaks. I have always trusted examples more than sermons. A good example is a small animal: alive, limited, and surprisingly hard to fake. If you can make it fail and then explain the failure, it has taught you something better than success.

This is therefore an artifact of memory rather than evidence of 2006 activity. The commits created by the setup script are current commits with retrospective messages. The tag marks the chapter, not a claim that the public record existed in that year. Legends are strongest when they can stand in daylight. No false snow is needed.

If you fork this, keep the honesty file. If you improve the code, say what you changed and when. If you use the voice, use it to make hard ideas clearer, not to make the past blurrier. The frozen path is long enough without inventing footprints behind us.

## Try it

```bash
git clone https://github.com/primeodin/undying-allocator.git
cd undying-allocator
./scripts/smoke.sh
# or without make:
cc -std=c89 -Wall -O2 -Isrc -o main main.c src/rune_core.c src/undying_allocator.c -lm
cc -std=c89 -Wall -O2 -Isrc -o test_core tests/test_core.c src/rune_core.c src/undying_allocator.c -lm
./test_core && ./main
```

**Expected** (deterministic on this tree): `c tests pass`, then an arena line with `alloc_a=yes alloc_b=yes exhausted=yes … failed=1 check=ok`, then a reset line with `used=0` and `resets=1`.

Hand-worked trap (one-byte overrun kills the tail fence): **[docs/why-canaries.md](docs/why-canaries.md)**.

## Built-out archive contents

This public-ready build-out adds `docs/CHAPTER.md`, `examples/transcript.txt`, and `scripts/smoke.sh` so the chapter is not only literary but inspectable. The smoke script is intentionally plain: it compiles or runs the small artifact, prints a short trace, and refuses to hide failure. That is the old bargain. If the rune breaks, the console should say so without ceremony.

## GitHub publication note

If this repository appears under `primeodin/undying-allocator`, read it as a chapter of the retrospective archive. The history is honest current work, not an invented twenty-year activity record. The myth is in the voice and the learning arc; the truth is in the archive note.

## What this repository is

This is a runnable retrospective chapter for **undying-allocator**: C memory-pool allocator experiment focused on bounded failure.
It is not padded to impress a counter. The implementation is deliberately compact, tested by `./scripts/smoke.sh`, and written so a reader can follow the idea without spelunking through generated fog.

The allocator experiment lives in `src/undying_allocator.c` / `.h`. It uses a caller-supplied fixed arena, supports aligned bump allocations, records high-water and failed-allocation counters, resets by rewinding the pool while preserving accounting, and checks per-allocation canaries so small overruns are visible to the test suite. Why the fences matter (and why exhaustion is the loud failure while overrun is the quiet one): [docs/why-canaries.md](docs/why-canaries.md).

## Shipped teaching note

- [docs/why-canaries.md](docs/why-canaries.md) — head+tail canaries vs silent overrun; hand-worked `p[8]=0x7f` trap matching `test_allocator_canary_check`.

## Help / good first issues

First PR friendly — claim one in a comment:

- **#1** — [`docs/start-here.md`](https://github.com/primeodin/undying-allocator/issues/1) walkthrough (`make` → smoke → arena lines)
- **#2** — [`examples/canary-stomp.txt`](https://github.com/primeodin/undying-allocator/issues/2) annotated overrun + check

## Public-readiness notes

- The year marker is narrative context, not a forged GitHub timestamp.
- The `.retrospective` tag marks this as part of the honest archive reconstruction.
- Contributions should improve behavior, tests, explanation, or safety — not bulk.
