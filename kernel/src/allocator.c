#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* A temporary arena allocator
 * that allocates a requested
 * number of bytes and returns
 * both bottom and top addresses.
 */

// Use size_t for tracking offsets and sizes safely across architectures
static size_t memoryIndex = 0;

extern void* ramStart; // Placeholder for valid linker variables
extern void* ramEnd;

typedef struct Arena_t {
    void* stackBottom; //stack bounds
    void* stackTop;
} Arena_t;

// Helper to cast void* to a byte pointer for proper arithmetic
static inline uint8_t* get_ram_start_ptr(void) {
    return (uint8_t*)ramStart;
}

static inline uint8_t* get_ram_end_ptr(void) {
    return (uint8_t*)ramEnd;
}

// Checks if the already-aligned number of bytes fits into the RAM
static bool check_fit_ram(size_t aligned_bytes) {
    uint8_t* current_ptr = get_ram_start_ptr() + memoryIndex;

    // Check for overflow and bound limits
    if (current_ptr + aligned_bytes > get_ram_end_ptr()) {
        return false; // Fail: doesn't fit
    }
    return true;
}

// Allocate arena and return bottom and top bounds
Arena_t x_pu_alloc_arena(size_t requested_bytes) {
    Arena_t arena = {NULL, NULL}; // Explicitly zero out both pointers

    // 8-byte alignment calculation done once here
    size_t aligned_bytes = (requested_bytes + 7) & ~(size_t)7;

    if (check_fit_ram(aligned_bytes)) {
        uint8_t* start = get_ram_start_ptr() + memoryIndex;

        arena.stackBottom = (void*)start;
        arena.stackTop    = (void*)(start + aligned_bytes);

        memoryIndex += aligned_bytes; // Increment global tracker
    }

    return arena;
}
