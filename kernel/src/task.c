#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "allocator.h"

/*
 * Task states used by the scheduler.
 */
typedef enum TaskState_t
{
    INVALID,    /* Task is not valid (creation failure or fault) */
    RUNNING,    /* Currently executing task */
    READY,      /* Ready to be scheduled */
    BLOCKED     /* Waiting for event or resource */
} TaskState_t;

/*
 * Task Control Block (TCB)
 *
 * Represents all runtime information required by the scheduler
 * to manage a task.
 */
typedef struct TCB_t
{
    void *stackPtr;              /* Current stack pointer (PSP) */

    void (*task)(void *arg);     /* Task entry function */
    void *arg;                   /* Task argument */

    void *stackBottom;           /* Lower stack boundary */
    void *stackTop;              /* Upper stack boundary */

    uint32_t errorCode;          /* Task fault / error status */
    uint16_t id;                 /* Unique task ID */
    TaskState_t state;           /* Current task state */

} TCB_t;

/* Global task ID counter */
static uint16_t taskId = 0;

/*
 * Creates a new task and allocates its stack from the arena allocator.
 *
 * Stack memory is assumed to grow downward (Cortex-M convention).
 */
TCB_t x_pu_create_task(void (*task)(void *arg), void *arguments, size_t stackSize)
{
    TCB_t tcb = {0};

    /* Validate input early */
    if (task == NULL || stackSize == 0)
    {
        tcb.errorCode = 0xFFFFAAAA;
        tcb.state = INVALID;
        return tcb;
    }

    /* Allocate stack memory for the task */
    Arena_t taskArena = x_pu_alloc_arena(stackSize);

    /* Validate allocation result */
    if (taskArena.stackBottom == NULL || taskArena.stackTop == NULL)
    {
        tcb.errorCode = 0xFFFFAAAA;
        tcb.state = INVALID;
        return tcb;
    }

    /* Initialize task stack */
    tcb.stackBottom = taskArena.stackBottom;
    tcb.stackTop    = taskArena.stackTop;
    tcb.stackPtr    = taskArena.stackTop; /* stack grows downward */

    /* Assign identity */
    tcb.id = taskId++;
    tcb.state = READY;

    /* Assign execution context */
    tcb.task = task;
    tcb.arg  = arguments;

    /* Clear error state on success */
    tcb.errorCode = 0;

    return tcb;
}
