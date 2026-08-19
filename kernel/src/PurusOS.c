#include <stddef.h>
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
        uintptr_t initialContext = uptr_pu_get_active_task_context();
        v_pu_task_context_restore(initialContext);
    }
    else
    {
        v_pu_fault_trap(PU_KERNEL_NO_TASK_TO_RUN, false);
    }
}

void newTask(void (*task)(void *arg), void *arguments, size_t stackSize)
{
    TCB_t tcb = x_pu_create_task(task, arguments, stackSize);
    v_pu_load_task(tcb);
}

void taskYield(void)
{
    uintptr_t savedContext = uptr_pu_task_context_save();
    v_pu_update_active_task_context(savedContext);
    v_pu_choose_next_task();
    uintptr_t newContext = uptr_pu_get_active_task_context();
    v_pu_task_context_restore(newContext);
}
