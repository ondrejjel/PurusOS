#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
 * Simple bounded arena allocator.
 *
 * This allocator sub-allocates linear memory blocks from a fixed
 * memory region defined by linker script symbols.
 *
 * It does NOT support free operations.
 */

/* Current offset within the memory pool */
static size_t mempoolOffset = 0;

/*
 * Linker-defined memory pool boundaries.
 * These are provided by the linker script.
 */
extern uint32_t _smempool;
extern uint32_t _emempool;

/*
 * Represents a linear sub-allocated memory region.
 */
typedef struct Arena_t
{
    void *stackBottom;  /* Start of allocated region */
    void *stackTop;     /* End of allocated region */
} Arena_t;

/* Returns start of memory pool */
static inline uint8_t *get_mempool_start(void)
{
    return (uint8_t *)&_smempool;
}

/* Returns end of memory pool */
static inline uint8_t *get_mempool_end(void)
{
    return (uint8_t *)&_emempool;
}

/* Returns total size of the memory pool in bytes */
size_t x_pu_get_mempool_size(void)
{
    return (size_t)(get_mempool_end() - get_mempool_start());
}

/* Checks whether a request fits into remaining pool space */
static bool mempool_has_space(size_t alignedBytes)
{
    size_t poolSize = x_pu_get_mempool_size();

    return (mempoolOffset + alignedBytes) <= poolSize;
}

/*
 * Allocates a linear arena from the memory pool.
 *
 * Allocation is 8-byte aligned to satisfy ARM ABI requirements.
 * Returns a zeroed Arena_t if allocation fails.
 */
Arena_t x_pu_alloc_arena(size_t requested_bytes)
{
    Arena_t arena = {0};

    /* Align size to 8-byte boundary */
    size_t alignedBytes = (requested_bytes + 7U) & ~(size_t)7;

    if (!mempool_has_space(alignedBytes))
    {
        return arena;
    }

    uint8_t *poolBase = get_mempool_start();

    arena.stackBottom = (void *)(poolBase + mempoolOffset);
    arena.stackTop    = (void *)(poolBase + mempoolOffset + alignedBytes);

    mempoolOffset += alignedBytes;

    return arena;
}
