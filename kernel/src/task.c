/* Standard headers */
#include <stdint.h>
#include <stddef.h>

/* Project headers */
#include <rtos_config.h>
#include <fault_code.h>
#include <allocator.h>
#include <kernel_interface.h>
#include <task.h>

static uint16_t taskId = 0;

TCB_t x_pu_create_task(void (*function)(void *arg), void *arguments, size_t stackSize)
{
    TCB_t tcb = {0};

    if (function == NULL)
    {
        tcb.faultCode = PU_TASK_NULL_FUNCTION;
        return tcb;
    }

    Task_t task = {function, arguments};

    /* Align size to boundary from config */
    size_t alignedStackSize =
        (stackSize + PU_STACK_ALIGNMENT - 1) & ~((size_t)PU_STACK_ALIGNMENT - 1);

    if (alignedStackSize > PU_MAXIMAL_STACK_SIZE)
    {
        tcb.faultCode = PU_TASK_STACK_TOO_BIG;
        return tcb;
    }
    if (alignedStackSize < PU_MINIMAL_STACK_SIZE)
    {
        tcb.faultCode = PU_TASK_STACK_TOO_SMALL;
        return tcb;
    }

    MemoryBlock_t memory = x_pu_allocate_memory_block(alignedStackSize);

    if (memory.begin == NULL || memory.end == NULL)
    {
        tcb.faultCode = PU_TASK_MEMORY_ALLOCATION_FAILED;
        return tcb;
    }

    uintptr_t context = uptr_pu_task_context_create(memory, &task);

    if (context == 0)
    {
        tcb.faultCode = PU_TASK_CONTEXT_CREATION_FAILED;
        return tcb;
    }

    tcb.memory = memory;
    tcb.task = task;
    tcb.context = context;
    tcb.state = READY;
    tcb.id = taskId;
    tcb.faultCode = PU_TASK_OK;

    taskId++;

    return tcb;
}
