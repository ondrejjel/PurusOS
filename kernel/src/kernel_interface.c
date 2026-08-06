//new interface file that will replace port_interface
#include <stddef.h>
#include <stdint.h>
#include <allocator.h>
#include <task.h>

typedef struct
{
    uintptr_t (*context_save)(void);
    void (*context_restore)(uintptr_t context);
    uintptr_t (*context_create)(MemoryBlock_t *memory, const Task_t *task); //todo: finish task.h implementation
} KernelInterface_t ;

KernelInterface_t activeInterface = {0};

uintptr_t uptr_pu_task_context_save(void)
{
    if(activeInterface.context_save == NULL)
    {
        return (uintptr_t)0;
    }

    return activeInterface.context_save();
}

void v_pu_task_context_restore(uintptr_t context)
{
    if(activeInterface.context_restore == NULL)
    {
        return;
    }
    activeInterface.context_restore(context);
}

uintptr_t uptr_pu_task_context_create(MemoryBlock_t memory, const Task_t *task)
{
    if(activeInterface.context_create == NULL)
    {
        return (uintptr_t)0;
    }

    return activeInterface.context_create(memory, task);
}


void v_pu_kernel_use_interface(const KernelInterface_t *interface)
{
    if(interface == NULL)
    {
        return;
    }
    activeInterface = *interface;
}
