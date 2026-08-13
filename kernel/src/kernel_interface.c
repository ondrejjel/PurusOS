#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>
#include <kernel_interface.h>
#include <fault_code.h>

/*
 * Kernel interface active instance with high trust.
 * Wrappers call functions from here.
 */
static KernelInterface_t activeInterface = {0};

/*
 * Highest-trust failure path.
 * Passes the fault code and runtime state to the architecture layer.
 */
_Noreturn void v_pu_fault_trap(uint32_t faultCode, bool isAtRuntime)
{
    activeInterface.fault_trap(faultCode, isAtRuntime);
    for (;;)
    {
    }
}

/*
 * Arch specific task related functions.
 */

/* Saves the current context and returns its opaque stack pointer. */
uintptr_t uptr_pu_task_context_save(void)
{
    return activeInterface.context_save();
}

/* Restores execution context from an opaque stack pointer. */
void v_pu_task_context_restore(uintptr_t context)
{
    activeInterface.context_restore(context);
}

/* Generates and loads the initial context onto the task stack. */
uintptr_t uptr_pu_task_context_create(MemoryBlock_t memory, const Task_t *task)
{
    return activeInterface.context_create(memory, task);
}

/*
 * Validates and activates an architecture-provided kernel interface.
 * The active interface is updated only after all required functions pass validation.
 */
void v_pu_kernel_use_interface(const KernelInterface_t *interface)
{
    /* Cannot safely invoke the fault trap without a valid interface. */
    if (interface == NULL)
    {
        for (;;)
        {
        }
    }

    /* Cannot safely continue without a valid fault trap. */
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

    /* All checks passed; activate the interface. */
    activeInterface = *interface;
}
