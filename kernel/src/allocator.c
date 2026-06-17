#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* A bounded arena allocator that sub-allocates fixed-size
 * arenas out of the main task memory pool (.mempool)
 */

// Global tracking offset within the main memory pool array
static size_t memoryIndex = 0;

// Linker script markers defining the absolute boundaries of the memory pool
extern uint32_t _smempool;
extern uint32_t _emempool;

typedef struct Arena_t {
    void* stackBottom; // Lowest address of this sub-allocated arena
    void* stackTop;    // Highest address (limit) of this sub-allocated arena
} Arena_t;

// Returns a pointer to the start of the memory pool array
static inline uint8_t* get_ram_start_ptr(void) {
    return (uint8_t*)&_smempool;
}

// Returns a pointer to the end of the memory pool array
static inline uint8_t* get_ram_end_ptr(void) {
    return (uint8_t*)&_emempool;
}

// Calculates the total pool capacity directly from the linker symbols
size_t x_pu_get_mempool_size(void) {
    return (size_t)(get_ram_end_ptr() - get_ram_start_ptr());
}

// Checks if the aligned requested bytes fit within the remaining pool capacity
static bool check_fit_ram(size_t alignedBytes) {
    size_t totalPoolSize = x_pu_get_mempool_size();

    // Check if the current index plus requested size exceeds the overall array bounds
    if (memoryIndex + alignedBytes > totalPoolSize) {
        return false; // Array overflow prevented
    }
    return true;
}

// Allocate a sub-arena from the main pool block
Arena_t x_pu_alloc_arena(size_t requested_bytes) {
    Arena_t arena = {NULL, NULL};

    // Enforce 8-byte alignment required by ARM ABI
    size_t alignedBytes = (requested_bytes + 7) & ~(size_t)7;

    if (check_fit_ram(alignedBytes)) {
        // Base address of the underlying storage array
        uint8_t* poolBase = get_ram_start_ptr();

        // Assign the boundaries of the sub-arena relative to the array base and current offset
        arena.stackBottom = (void*)(poolBase + memoryIndex);
        arena.stackTop    = (void*)(poolBase + memoryIndex + alignedBytes);

        // Advance the index offset to reserve the array block
        memoryIndex += alignedBytes;
    }

    return arena;
}
