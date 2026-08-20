#ifndef PU_SCHEDULER_H
#define PU_SCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>

/* Validates and loads a task into the scheduler's ready queue. */
void v_pu_load_task(TCB_t task);

/* Returns the saved context of the active task. */
uintptr_t uptr_pu_get_active_task_context(void);

/* Returns the memory bounds of the active task. */
MemoryBlock_t x_pu_get_active_task_memory_bounds(void);

/* Returns the number of currently loaded tasks. */
size_t s_pu_get_loaded_task_count(void);

/* Updates the saved context of the active task. */
void v_pu_update_active_task_context(uintptr_t context);

/* Verifies that the active task's saved context is within its allocated memory. */
void v_pu_check_if_active_task_in_bounds(void);

/* Selects the first task before round-robin scheduling begins. */
void v_pu_set_initial_task(void);

/* Selects the next task using round-robin scheduling. */
void v_pu_choose_next_task(void);

#endif /* PU_SCHEDULER_H */
