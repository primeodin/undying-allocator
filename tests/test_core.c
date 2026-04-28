#include "../src/rune_core.h"
#include "../src/undying_allocator.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int is_aligned(void *ptr, size_t alignment)
{
    return ((unsigned long)ptr & (unsigned long)(alignment - 1)) == 0;
}

static void test_rune_core(void)
{
    RuneReport r;
    r = rune_simulate(32, 0.02);
    assert(r.steps == 32);
    assert(r.energy >= 0);
    assert(rune_formula(1, 1, 2) == rune_formula(1, 1, 2));
    assert(rune_formula(-1, 1, 2) == rune_formula(1, 1, 2));
}

static void test_allocator_allocates_and_aligns(void)
{
    unsigned char arena_buffer[256];
    UndyingArena arena;
    UndyingStats stats;
    void *a;
    void *b;

    undying_arena_init(&arena, arena_buffer, sizeof(arena_buffer));
    a = undying_alloc(&arena, 13);
    b = undying_alloc_aligned(&arena, 32, 32);

    assert(a != 0);
    assert(b != 0);
    assert(is_aligned(a, UNDYING_DEFAULT_ALIGNMENT));
    assert(is_aligned(b, 32));
    memset(a, 0x11, 13);
    memset(b, 0x22, 32);
    assert(undying_check(&arena));

    stats = undying_stats(&arena);
    assert(stats.capacity == sizeof(arena_buffer));
    assert(stats.used > 0);
    assert(stats.remaining == sizeof(arena_buffer) - stats.used);
    assert(stats.high_water == stats.used);
    assert(stats.allocation_count == 2);
    assert(stats.failed_count == 0);
}

static void test_allocator_exhaustion_stats_and_reset(void)
{
    unsigned char arena_buffer[160];
    UndyingArena arena;
    UndyingStats stats;
    void *blocks[16];
    int count;
    int i;

    undying_arena_init(&arena, arena_buffer, sizeof(arena_buffer));
    count = 0;
    for (i = 0; i < 16; i++) {
        blocks[i] = undying_alloc(&arena, 24);
        if (blocks[i] == 0) break;
        count++;
    }

    assert(count > 0);
    assert(count < 16);
    assert(undying_check(&arena));
    stats = undying_stats(&arena);
    assert(stats.failed_count == 1);
    assert(stats.failed_bytes == 24);
    assert(stats.high_water == stats.used);

    undying_reset(&arena);
    stats = undying_stats(&arena);
    assert(stats.used == 0);
    assert(stats.high_water > 0);
    assert(stats.allocation_count == 0);
    assert(stats.failed_count == 1);
    assert(stats.reset_count == 1);
    assert(undying_check(&arena));
    assert(undying_alloc(&arena, 64) != 0);
    assert(undying_check(&arena));
}

static void test_allocator_canary_check(void)
{
    unsigned char arena_buffer[192];
    UndyingArena arena;
    unsigned char *p;

    undying_arena_init(&arena, arena_buffer, sizeof(arena_buffer));
    p = (unsigned char *)undying_alloc(&arena, 8);
    assert(p != 0);
    assert(undying_check(&arena));
    p[8] = 0x7f;
    assert(!undying_check(&arena));
}

int main(void)
{
    test_rune_core();
    test_allocator_allocates_and_aligns();
    test_allocator_exhaustion_stats_and_reset();
    test_allocator_canary_check();
    puts("c tests pass");
    return 0;
}
