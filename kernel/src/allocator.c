#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <allocator.h>
#include <kernel_interface.h>
#include <fault_code.h>

/*
 * Simple bounded memory block allocator.
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
 * Memory pool helpers and utilities
 */

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
size_t s_pu_get_mempool_size(void)
{
    return (size_t)(get_mempool_end() - get_mempool_start());
}

/* Checks whether a request fits into remaining pool space */
static bool mempool_has_space(size_t Bytes)
{
    const size_t mempoolSize = s_pu_get_mempool_size();

    return (mempoolOffset + Bytes) <= mempoolSize;
}

/*
 * Memory block helpers and utilities
 */

/* Return a size of memory block in bytes */
size_t s_pu_get_memory_block_size(const MemoryBlock_t *block)
{
    if (block == NULL)
    {
        return 0;
    }

    return (size_t)((uint8_t *)block->end - (uint8_t *)block->begin);
}

/*
 * Function to reserve a memory block of given size in bytes
 * allocation is 8-byte aligned to make everything nicely rounded
 */

MemoryBlock_t x_pu_allocate_memory_block(size_t requestedBytes)
{
    MemoryBlock_t block = {0};

    if (!mempool_has_space(requestedBytes))
    {
        v_pu_fault_trap(PU_ALLOCATOR_NOT_ENOUGH_SPACE, false);
    }

    uint8_t *mempoolBase = get_mempool_start();

    block.begin = (void *)(mempoolBase + mempoolOffset);
    block.end = (void *)(mempoolBase + mempoolOffset + requestedBytes);

    mempoolOffset += requestedBytes;

    return block;
}
