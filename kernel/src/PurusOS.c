#include <stdint.h>
#include <allocator.h>
#include <fault_code.h>
#include <kernel_interface.h>
#include <PurusOS.h>
#include <scheduler.h>
#include <task.h>

void useInterface(const KernelInterface_t *interface)
{
    v_pu_kernel_use_interface(interface);
}

void startKernel(void)
{
    if (s_pu_get_loaded_task_count() != 0)
    {
        v_pu_set_initial_task();
    }
    else
    {
        v_pu_fault_trap(PU_KERNEL_NO_TASK_TO_RUN, false);
    }
}

void taskYield(void)
{
    uintptr_t saved_context = uptr_pu_task_context_save();
    v_pu_update_active_task_context(saved_context);
    v_pu_choose_next_task();
    uintptr_t new_context = uptr_pu_get_active_task_context();
    v_pu_task_context_restore(new_context);
}
