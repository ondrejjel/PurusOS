#ifndef PU_KERNEL_INTERFACE_H
#define PU_KERNEL_INTERFACE_H

#include <stdint.h>
#include <allocator.h>
#include <task.h>

typedef struct
{
    uintptr_t (*context_save)(void);
    void (*context_restore)(uintptr_t context);
    uintptr_t (*context_create)(MemoryBlock_t memory, const Task_t *task);
} KernelInterface_t;

extern KernelInterface_t activeInterface;

uintptr_t uptr_pu_task_context_save(void);

void v_pu_task_context_restore(uintptr_t context);

uintptr_t uptr_pu_task_context_create(MemoryBlock_t memory, const Task_t *task);

void v_pu_kernel_use_interface(const KernelInterface_t *interface);

#endif // PU_KERNEL_INTERFACE_H
