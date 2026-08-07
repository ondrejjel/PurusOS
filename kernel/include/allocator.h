#ifndef PU_ALLOCATOR_H_
#define PU_ALLOCATOR_H_

#include <stddef.h>

typedef struct
{
    void *begin;
    void *end;
} MemoryBlock_t;

MemoryBlock_t x_pu_allocate_memory_block(size_t requested_bytes);

#endif // PU_ALLOCATOR_H_
