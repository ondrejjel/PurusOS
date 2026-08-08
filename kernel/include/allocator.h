#ifndef PU_ALLOCATOR_H
#define PU_ALLOCATOR_H

/* Include stddef header for size_t type definition */
#include <stddef.h>

/*
 * MemoryBlock_t represents a half-open memory interval [begin, end).
 * begin is the lowest inclusive address and end is the first address
 * after the allocated block.
 */
typedef struct
{
    void *begin; /* Lowest address, inclusive */
    void *end;   /* First address after the block, exclusive */
} MemoryBlock_t;

/*
 * Allocates the requested number of bytes and returns the
 * boundaries of the allocated memory block.
 */
MemoryBlock_t x_pu_allocate_memory_block(size_t requestedBytes);

#endif /* PU_ALLOCATOR_H */
