/* Standard headers */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Project headers */
#include <rtos_config.h>
#include <fault_code.h>
#include <allocator.h>
#include <kernel_interface.h>
#include <task.h>

static uint16_t taskId = 0;

TCB_t x_pu_create_task(void (*function)(void *arg), void *arguments, size_t stackSize)
{
    TCB_t tcb = {.memory = {0}, .task = {0}, .context = 0};

    if (function == NULL)
    {
        v_pu_fault_trap(PU_TASK_NULL_FUNCTION, false);
    }

    Task_t task = {function, arguments};

    /* Align size to boundary from config */
    size_t alignedStackSize =
        (stackSize + PU_STACK_ALIGNMENT - 1) & ~((size_t)PU_STACK_ALIGNMENT - 1);

    if (alignedStackSize > PU_MAXIMAL_STACK_SIZE)
    {
        v_pu_fault_trap(PU_TASK_STACK_TOO_BIG, false);
    }
    if (alignedStackSize < PU_MINIMAL_STACK_SIZE)
    {
        v_pu_fault_trap(PU_TASK_STACK_TOO_SMALL, false);
    }

    MemoryBlock_t memory = x_pu_allocate_memory_block(alignedStackSize);

    if (memory.begin == NULL || memory.end == NULL)
    {
        v_pu_fault_trap(PU_TASK_MEMORY_ALLOCATION_FAILED, false);
    }

    uintptr_t context = uptr_pu_task_context_create(memory, &task);

    if (context == 0)
    {
        v_pu_fault_trap(PU_TASK_CONTEXT_CREATION_FAILED, false);
    }

    tcb.memory = memory;
    tcb.task = task;
    tcb.context = context;
    tcb.state = READY;
    tcb.id = taskId;

    taskId++;

    return tcb;
}
