#ifndef PU_KERNEL_INTERFACE_H
#define PU_KERNEL_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>

/*
 * Kernel interface.
 *
 * The kernel uses this interface to access operations that depend on the
 * target architecture without directly depending on their implementation.
 * The hardware layer provides the required functions through a
 * KernelInterface_t instance, which is stored as the active interface.
 *
 * Kernel code uses the public interface functions below instead of calling
 * architecture-specific functions directly.
 */

typedef struct
{
    uintptr_t (*context_save)(void);
    void (*context_restore)(uintptr_t context);
    uintptr_t (*context_create)(MemoryBlock_t memory, const Task_t *task);
    void (*fault_trap)(uint32_t faultCode, bool isAtRuntime);
} KernelInterface_t;

void v_pu_fault_trap(uint32_t faultCode, bool isAtRuntime);

uintptr_t uptr_pu_task_context_save();

void v_pu_task_context_restore(uintptr_t context);

uintptr_t uptr_pu_task_context_create(MemoryBlock_t memory, const Task_t *task);

void v_pu_kernel_use_interface(const KernelInterface_t *interface);

#endif /* PU_KERNEL_INTERFACE_H */
