#include "src/rune_core.h"
#include "src/undying_allocator.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    unsigned char memory[512];
    UndyingArena arena;
    UndyingStats stats;
    RuneReport r;
    void *a;
    void *b;
    void *too_large;

    (void)argc;
    (void)argv;

    r = rune_simulate(180, 0.016);
    printf("%s report steps=%d energy=%.4f drift=%.4f events=%d\n", "undying-allocator", r.steps, r.energy, r.drift, r.events);
    printf("formula %.5f\n", rune_formula(10, 3.0, 4.0));

    undying_arena_init(&arena, memory, sizeof(memory));
    a = undying_alloc(&arena, 40);
    b = undying_alloc_aligned(&arena, 96, 32);
    too_large = undying_alloc(&arena, sizeof(memory));
    stats = undying_stats(&arena);

    printf("arena alloc_a=%s alloc_b=%s exhausted=%s used=%lu high=%lu failed=%lu check=%s\n",
           a != 0 ? "yes" : "no",
           b != 0 ? "yes" : "no",
           too_large == 0 ? "yes" : "no",
           (unsigned long)stats.used,
           (unsigned long)stats.high_water,
           (unsigned long)stats.failed_count,
           undying_check(&arena) ? "ok" : "bad");

    undying_reset(&arena);
    stats = undying_stats(&arena);
    printf("arena reset used=%lu high=%lu resets=%lu\n",
           (unsigned long)stats.used,
           (unsigned long)stats.high_water,
           (unsigned long)stats.reset_count);
    return 0;
}
