#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <rtos_config.h>
#include <fault_code.h>
#include <allocator.h>
#include <kernel_interface.h>
#include <task.h>

/*
 * Creates a task and initializes its stack and execution context.
 */
TCB_t x_pu_create_task(void (*function)(void *arg), void *arguments, size_t stackSize)
{
    /* Initialize an empty TCB. */
    TCB_t tcb = {.memory = {0}, .task = {0}, .context = 0};

    if (function == NULL)
    {
        v_pu_fault_trap(PU_TASK_NULL_FUNCTION, false);
    }

    /* Bundle the task function and its arguments. */
    Task_t task = {function, arguments};

    /* Align the requested stack size to the configured boundary. */
    size_t alignedStackSize =
        (stackSize + PU_STACK_ALIGNMENT - 1) & ~((size_t)PU_STACK_ALIGNMENT - 1);

    if (alignedStackSize > PU_MAXIMUM_STACK_SIZE)
    {
        v_pu_fault_trap(PU_TASK_STACK_TOO_BIG, false);
    }

    if (alignedStackSize < PU_MINIMUM_STACK_SIZE)
    {
        v_pu_fault_trap(PU_TASK_STACK_TOO_SMALL, false);
    }

    /* Allocate memory for the task stack. */
    MemoryBlock_t memory = x_pu_allocate_memory_block(alignedStackSize);

    if (memory.begin == NULL || memory.end == NULL)
    {
        v_pu_fault_trap(PU_TASK_MEMORY_ALLOCATION_FAILED, false);
    }

    /* Create the initial execution context on the task stack. */
    uintptr_t context = uptr_pu_task_context_create(memory, &task);

    if (context == 0)
    {
        v_pu_fault_trap(PU_TASK_CONTEXT_CREATION_FAILED, false);
    }

    /* Populate the TCB only after all task resources are valid. */
    tcb.memory = memory;
    tcb.task = task;
    tcb.context = context;
    tcb.state = READY;

    return tcb;
}
