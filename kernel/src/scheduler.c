#include <stddef.h>
#include <stdbool.h>
#include <fault_code.h>
#include <rtos_config.h>
#include <kernel_interface.h>
#include <allocator.h>
#include <task.h>

static TCB_t readyTasks[PU_MAXIMUM_TASK_COUNT] = {};
static size_t loadedTasks = 0;

static bool check_tcb(const TCB_t *tcb)
{
    if (tcb->context == 0)
    {
        return false;
    }
    if (tcb->memory.begin == NULL || tcb->memory.end == NULL)
    {
        return false;
    }
    if (tcb->state != READY)
    {
        return false;
    }
    if (tcb->task.function == NULL)
    {
        return false;
    }
    return true;
}

void v_pu_load_task(TCB_t task)
{
    if (loadedTasks < PU_MAXIMUM_TASK_COUNT)
    {
        if (check_tcb(&task))
        {
            readyTasks[loadedTasks] = task;
            loadedTasks++;
        }
        else
        {
            v_pu_fault_trap(PU_SCHEDULER_INVALID_TCB, false);
        }
    }
    else
    {
        v_pu_fault_trap(PU_TASK_COUNT_LIMIT_REACHED, false);
    }
}
