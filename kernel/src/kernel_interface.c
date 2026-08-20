#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>
#include <kernel_interface.h>
#include <fault_code.h>

/*
 * Fully validated kernel interface used by kernel wrappers.
 * This instance is activated only after all required callbacks pass validation.
 */
static KernelInterface_t activeInterface = {0};

static void (*activeFaultTrap)(uint32_t faultCode, bool isAtRuntime);

/*
 * Highest-trust kernel failure path.
 * Passes the fault to the architecture layer and guarantees that execution
 * cannot continue even if the architecture fault handler unexpectedly returns.
 */
_Noreturn void v_pu_fault_trap(uint32_t faultCode, bool isAtRuntime)
{
    activeFaultTrap(faultCode, isAtRuntime);
    for (;;)
    {
    }
}

/*
 * Emergency halt path used when the kernel cannot safely invoke the fault
 * handler, such as before a valid fault handler has been established.
 */
_Noreturn void v_pu_kernel_emergency_halt(void)
{
    for (;;)
    {
    }
}

/*
 * Architecture-specific task context wrappers.
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
 * Independently validated fault handler used during interface initialization.
 * Stored separately because activeInterface cannot be trusted until validation
 * has completed.
 */
void v_pu_kernel_use_interface(const KernelInterface_t *interface)
{
    /*
     * Validates and activates an architecture-provided kernel interface.
     * The fault handler is established first so validation failures can be
     * reported safely. The complete interface is activated only after all
     * required callbacks pass validation.
     */

    /* Cannot safely invoke the fault trap without a valid interface. */
    if (interface == NULL)
    {
        v_pu_kernel_emergency_halt();
    }

    /* Cannot safely continue without a valid fault trap. */
    if (interface->fault_trap == NULL)
    {
        v_pu_kernel_emergency_halt();
    }
    activeFaultTrap = interface->fault_trap;

    if (interface->context_create == NULL)
    {
        v_pu_fault_trap(PU_KERNEL_INTERFACE_CONTEXT_CREATE_INVALID, false);
    }

    if (interface->context_restore == NULL)
    {
        v_pu_fault_trap(PU_KERNEL_INTERFACE_CONTEXT_RESTORE_INVALID, false);
    }

    if (interface->context_save == NULL)
    {
        v_pu_fault_trap(PU_KERNEL_INTERFACE_CONTEXT_SAVE_INVALID, false);
    }

    /* All checks passed; activate the interface. */
    activeInterface = *interface;
}
