#ifndef PU_TASK_H
#define PU_TASK_H

#include <stdint.h>
#include <stddef.h>
#include <allocator.h>

/*
 * Task descriptor containing the function executed by the task
 * and its argument.
 */
typedef struct
{
    void (*function)(void *);
    void *argument;
} Task_t;

/*
 * Task states used by the scheduler to track task execution and
 * determine which tasks are eligible to run.
 */
typedef enum
{
    RUNNING, /* Currently executing task */
    READY    /* Ready to be scheduled */
} TaskState_t;

/*
 * Task Control Block containing task metadata, execution state,
 * and saved execution context.
 */
typedef struct
{
    MemoryBlock_t memory; /* Task stack bounds */
    Task_t task;          /* Task function and argument */
    uintptr_t context;    /* Opaque saved execution context */

    TaskState_t state;
    uint16_t id;
} TCB_t;

/*
 * Creates a TCB and initializes the resources required to run the task.
 */
TCB_t x_pu_create_task(void (*function)(void *arg), void *arguments, size_t stackSize);

#endif /* PU_TASK_H */
