#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <fault_code.h>
#include <rtos_config.h>
#include <kernel_interface.h>
#include <allocator.h>
#include <task.h>
#include <scheduler.h>

static TCB_t readyTasks[PU_MAXIMUM_TASK_COUNT] = {};
static size_t loadedTasks = 0;
static uint16_t taskId = 0;
static size_t activeTaskIndex = 0;
static TCB_t *activeTask = NULL;

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
            task.id = taskId;
            taskId++;

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
        v_pu_fault_trap(PU_SCHEDULER_TASK_COUNT_LIMIT_REACHED, false);
    }
}

TCB_t x_pu_get_active_task(void)
{
    return *activeTask;
}

uintptr_t uptr_pu_get_active_task_context(void)
{
    return activeTask->context;
}

MemoryBlock_t x_pu_get_active_task_memory_bounds(void)
{
    return activeTask->memory;
}

size_t s_pu_get_loaded_task_count(void)
{
    return loadedTasks;
}

void v_pu_check_if_active_task_in_bounds(void)
{
    if ((uintptr_t)activeTask->context >= (uintptr_t)activeTask->memory.begin &&
        (uintptr_t)activeTask->context < (uintptr_t)activeTask->memory.end)
    {
        return;
    }
    v_pu_fault_trap(PU_SCHEDULER_TASK_OUT_OF_BOUNDS, true);
}

void v_pu_set_initial_task(void)
{
    activeTask = &readyTasks[0];
    activeTask->state = RUNNING;
}

void v_pu_choose_next_task(void)
{
    activeTask->state = READY;

    activeTaskIndex++;

    if (activeTaskIndex >= loadedTasks)
    {
        activeTaskIndex = 0;
    }

    activeTask = &readyTasks[activeTaskIndex];
    activeTask->state = RUNNING;
}
