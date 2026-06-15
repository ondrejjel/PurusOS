#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct TCB_t {
    void* stackPtr;             // CURRENT stack pointer

    void (*task)(void *arg);    // Pointer to the task function
    void* arg;                  // Argument passed to the task function

    void* stackBottom;          // bottom stack boundary
    void* stackTop;             // top stack boundary

    uint16_t id;                // Task ID
} TCB_t;

