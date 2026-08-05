#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include<stddef.h>

typedef struct MemoryBlock_t
{
    void *begin;
    void *end;
} MemoryBlock_t;

MemoryBlock_t x_pu_allocate_memory_block(size_t requested_bytes);

#endif // ALLOCATOR_H_
