#ifndef PU_SCHEDULER_H
#define PU_SCHEDULER_H

#include <stddef.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>

void v_pu_load_task(TCB_t task);
TCB_t x_pu_get_active_task(void);
uintptr_t uptr_pu_get_active_task_context(void);
MemoryBlock_t x_pu_get_active_task_memory_bounds(void);
size_t s_pu_get_loaded_task_count(void);
void v_pu_check_if_active_task_in_bounds(void);
void v_pu_set_initial_task(void);
void v_pu_choose_next_task(void);

#endif /* PU_SCHEDULER_H */
