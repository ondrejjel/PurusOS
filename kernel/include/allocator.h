#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include<stddef.h>

typedef struct Arena_t {
    void* stackBottom; //stack bounds
    void* stackTop;
} Arena_t;

Arena_t x_pu_alloc_arena(size_t requested_bytes);

#endif // ALLOCATOR_H_
