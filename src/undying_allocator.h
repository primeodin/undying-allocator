#ifndef UNDYING_ALLOCATOR_H
#define UNDYING_ALLOCATOR_H

#include <stddef.h>

#define UNDYING_DEFAULT_ALIGNMENT ((size_t)sizeof(void *))
#define UNDYING_CANARY 0x55414c43UL

typedef struct UndyingBlock UndyingBlock;

typedef struct {
    unsigned char *base;
    size_t capacity;
    size_t offset;
    size_t high_water;
    size_t allocation_count;
    size_t failed_count;
    size_t failed_bytes;
    size_t reset_count;
    UndyingBlock *first;
    UndyingBlock *last;
} UndyingArena;

typedef struct {
    size_t capacity;
    size_t used;
    size_t remaining;
    size_t high_water;
    size_t allocation_count;
    size_t failed_count;
    size_t failed_bytes;
    size_t reset_count;
} UndyingStats;

void undying_arena_init(UndyingArena *arena, void *buffer, size_t capacity);
void *undying_alloc(UndyingArena *arena, size_t size);
void *undying_alloc_aligned(UndyingArena *arena, size_t size, size_t alignment);
void undying_reset(UndyingArena *arena);
UndyingStats undying_stats(const UndyingArena *arena);
int undying_check(const UndyingArena *arena);

#endif
