#include "undying_allocator.h"

struct UndyingBlock {
    unsigned long head_canary;
    size_t requested;
    size_t payload_offset;
    size_t end_offset;
    struct UndyingBlock *next;
};

static int undying_is_power_of_two(size_t value)
{
    return value != 0 && (value & (value - 1)) == 0;
}

static size_t undying_align_up_size(size_t value, size_t alignment)
{
    size_t mask;
    if (alignment == 0) return value;
    mask = alignment - 1;
    if (value > ((size_t)-1) - mask) return (size_t)-1;
    return (value + mask) & ~mask;
}

static size_t undying_align_up_ptr(const unsigned char *base, size_t offset, size_t alignment)
{
    unsigned long address;
    unsigned long mask;
    unsigned long aligned;
    if (alignment == 0) return offset;
    address = (unsigned long)(base + offset);
    mask = (unsigned long)(alignment - 1);
    aligned = (address + mask) & ~mask;
    return offset + (size_t)(aligned - address);
}

static void undying_record_failure(UndyingArena *arena, size_t bytes)
{
    if (arena == 0) return;
    arena->failed_count++;
    arena->failed_bytes += bytes;
}

void undying_arena_init(UndyingArena *arena, void *buffer, size_t capacity)
{
    if (arena == 0) return;
    arena->base = (unsigned char *)buffer;
    arena->capacity = buffer == 0 ? 0 : capacity;
    arena->offset = 0;
    arena->high_water = 0;
    arena->allocation_count = 0;
    arena->failed_count = 0;
    arena->failed_bytes = 0;
    arena->reset_count = 0;
    arena->first = 0;
    arena->last = 0;
}

void *undying_alloc(UndyingArena *arena, size_t size)
{
    return undying_alloc_aligned(arena, size, UNDYING_DEFAULT_ALIGNMENT);
}

void *undying_alloc_aligned(UndyingArena *arena, size_t size, size_t alignment)
{
    size_t header_offset;
    size_t payload_offset;
    size_t canary_offset;
    size_t end_offset;
    struct UndyingBlock *block;
    unsigned long *tail;

    if (arena == 0 || arena->base == 0 || size == 0) {
        undying_record_failure(arena, size);
        return 0;
    }
    if (!undying_is_power_of_two(alignment)) {
        undying_record_failure(arena, size);
        return 0;
    }
    if (alignment < UNDYING_DEFAULT_ALIGNMENT) alignment = UNDYING_DEFAULT_ALIGNMENT;

    header_offset = undying_align_up_ptr(arena->base, arena->offset, UNDYING_DEFAULT_ALIGNMENT);
    if (header_offset == (size_t)-1) {
        undying_record_failure(arena, size);
        return 0;
    }
    if (header_offset > arena->capacity || sizeof(struct UndyingBlock) > arena->capacity - header_offset) {
        undying_record_failure(arena, size);
        return 0;
    }

    payload_offset = undying_align_up_ptr(arena->base, header_offset + sizeof(struct UndyingBlock), alignment);
    if (payload_offset == (size_t)-1 || payload_offset > arena->capacity) {
        undying_record_failure(arena, size);
        return 0;
    }
    if (size > arena->capacity - payload_offset) {
        undying_record_failure(arena, size);
        return 0;
    }
    canary_offset = payload_offset + size;
    if (sizeof(unsigned long) > arena->capacity - canary_offset) {
        undying_record_failure(arena, size);
        return 0;
    }
    end_offset = canary_offset + sizeof(unsigned long);
    end_offset = undying_align_up_size(end_offset, UNDYING_DEFAULT_ALIGNMENT);
    if (end_offset == (size_t)-1 || end_offset > arena->capacity) {
        undying_record_failure(arena, size);
        return 0;
    }

    block = (struct UndyingBlock *)(void *)(arena->base + header_offset);
    block->head_canary = UNDYING_CANARY;
    block->requested = size;
    block->payload_offset = payload_offset;
    block->end_offset = end_offset;
    block->next = 0;

    tail = (unsigned long *)(void *)(arena->base + canary_offset);
    *tail = UNDYING_CANARY;

    if (arena->last != 0) arena->last->next = block;
    if (arena->first == 0) arena->first = block;
    arena->last = block;
    arena->offset = end_offset;
    arena->allocation_count++;
    if (arena->offset > arena->high_water) arena->high_water = arena->offset;

    return (void *)(arena->base + payload_offset);
}

void undying_reset(UndyingArena *arena)
{
    if (arena == 0) return;
    arena->offset = 0;
    arena->first = 0;
    arena->last = 0;
    arena->allocation_count = 0;
    arena->reset_count++;
}

UndyingStats undying_stats(const UndyingArena *arena)
{
    UndyingStats stats;
    stats.capacity = 0;
    stats.used = 0;
    stats.remaining = 0;
    stats.high_water = 0;
    stats.allocation_count = 0;
    stats.failed_count = 0;
    stats.failed_bytes = 0;
    stats.reset_count = 0;

    if (arena == 0) return stats;
    stats.capacity = arena->capacity;
    stats.used = arena->offset;
    stats.remaining = arena->capacity >= arena->offset ? arena->capacity - arena->offset : 0;
    stats.high_water = arena->high_water;
    stats.allocation_count = arena->allocation_count;
    stats.failed_count = arena->failed_count;
    stats.failed_bytes = arena->failed_bytes;
    stats.reset_count = arena->reset_count;
    return stats;
}

int undying_check(const UndyingArena *arena)
{
    const struct UndyingBlock *block;
    const unsigned long *tail;

    if (arena == 0 || arena->base == 0) return 0;
    block = arena->first;
    while (block != 0) {
        if (block->head_canary != UNDYING_CANARY) return 0;
        if (block->payload_offset > arena->capacity) return 0;
        if (block->requested > arena->capacity - block->payload_offset) return 0;
        if (sizeof(unsigned long) > arena->capacity - (block->payload_offset + block->requested)) return 0;
        if (block->end_offset > arena->capacity) return 0;
        tail = (const unsigned long *)(const void *)(arena->base + block->payload_offset + block->requested);
        if (*tail != UNDYING_CANARY) return 0;
        block = block->next;
    }
    return 1;
}
