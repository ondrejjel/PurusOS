#ifndef PU_TASK_H
#define PU_TASK_H

/* Standard headers */
#include <stdint.h>
#include <stddef.h>

/* Project headers */
#include <fault_code.h>
#include <allocator.h>

typedef struct
{
    void (*function)(void *);
    void *argument;
} Task_t;

typedef enum
{
    INVALID, /* Task is not valid (creation failure or fault) */
    RUNNING, /* Currently executing task */
    READY,   /* Ready to be scheduled */
    BLOCKED  /* Waiting for event or resource */
} TaskState_t;

typedef struct
{
    MemoryBlock_t memory;
    Task_t task;
    uintptr_t context;

    TaskState_t state;
    uint16_t id;
    TaskFault_t faultCode;
} TCB_t;

TCB_t x_pu_create_task(void (*function)(void *arg), void *arguments, size_t stackSize);

#endif // PU_TASK_H
