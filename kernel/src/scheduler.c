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

/* Validates that a TCB contains all fields required by the scheduler. */
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

/* Validates and loads a task into the ready queue if capacity is available. */
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

/* Active task and scheduler state accessors. */

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

/* Updates the active task's saved context before a context switch. */
void v_pu_update_active_task_context(uintptr_t context)
{
    activeTask->context = context;
}

/* Verifies that the active task's saved context remains within its allocated memory. */
void v_pu_check_if_active_task_in_bounds(void)
{
    if ((uintptr_t)activeTask->context >= (uintptr_t)activeTask->memory.begin &&
        (uintptr_t)activeTask->context < (uintptr_t)activeTask->memory.end)
    {
        return;
    }
    v_pu_fault_trap(PU_SCHEDULER_TASK_OUT_OF_BOUNDS, true);
}

/* Selects the first task explicitly before round-robin scheduling begins. */
void v_pu_set_initial_task(void)
{
    activeTask = &readyTasks[0];
    activeTask->state = RUNNING;
}

/* Selects the next task using round-robin scheduling. */
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
