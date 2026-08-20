#include <kernel_interface.h>
#include <PurusOS.h>

static uintptr_t dummy_context_save(void)
{
    return 1;
}

static void dummy_context_restore(uintptr_t context)
{
    (void)context;

    for (;;)
    {
    }
}

static uintptr_t dummy_context_create(MemoryBlock_t memory, const Task_t *task)
{
    (void)memory;
    (void)task;

    return 1;
}

static void dummy_fault_trap(uint32_t faultCode, bool isAtRuntime)
{
    (void)faultCode;
    (void)isAtRuntime;

    for (;;)
    {
    }
}

static const KernelInterface_t dummyInterface = {.context_save = dummy_context_save,
                                                 .context_restore = dummy_context_restore,
                                                 .context_create = dummy_context_create,
                                                 .fault_trap = dummy_fault_trap};

void dummyTask(void *argument)
{
    (void)argument;

    for (;;)
    {
        taskYield();
    }
}

int main(void)
{
    selectKernelInterface(&dummyInterface);
    newTask(&dummyTask, NULL, 256);
    startKernel();
}
