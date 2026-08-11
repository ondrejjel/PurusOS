#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>
#include <kernel_interface.h>
#include <fault_code.h>

static KernelInterface_t activeInterface = {0};

void v_pu_fault_trap(uint32_t faultCode, bool isAtRuntime)
{
    activeInterface.fault_trap(faultCode, isAtRuntime);
}

uintptr_t uptr_pu_task_context_save(void)
{
    return activeInterface.context_save();
}

void v_pu_task_context_restore(uintptr_t context)
{
    activeInterface.context_restore(context);
}

uintptr_t uptr_pu_task_context_create(MemoryBlock_t memory, const Task_t *task)
{
    return activeInterface.context_create(memory, task);
}

void v_pu_kernel_use_interface(const KernelInterface_t *interface)
{
    if (interface == NULL)
    {
        for (;;)
        {
        }
    }
    if (interface->fault_trap == NULL)
    {
        for (;;)
        {
        }
    }
    if (interface->context_create == NULL)
    {
        interface->fault_trap(PU_KERNEL_INTERFACE_CONTEXT_CREATE_INVALID, false);
    }
    if (interface->context_restore == NULL)
    {
        interface->fault_trap(PU_KERNEL_INTERFACE_CONTEXT_RESTORE_INVALID, false);
    }
    if (interface->context_save == NULL)
    {
        interface->fault_trap(PU_KERNEL_INTERFACE_CONTEXT_SAVE_INVALID, false);
    }
    activeInterface = *interface;
}
