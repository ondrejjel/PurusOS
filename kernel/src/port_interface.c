#include <stdint.h>
#include <stddef.h>

typedef struct
{
    /*
     * Pointer to the default task context image.
     *
     * This context is copied onto a task's stack during task creation
     * and provides the initial register state expected by the context switch.
     */
    const uint32_t *defaultContext;

    /*
     * Number of 32-bit words contained in the default context image.
     */
    size_t defaultContextWords;

    /*
     * Architecture-specific context switch routine.
     */
    void (*context_switch)(void);

} Interface_t;

/*
 * Active architecture interface used by the kernel.
 */
Interface_t globalInterface = {0};

/*
 * Registers architecture-specific data and callbacks with the kernel.
 */
void v_pu_init_extern_pointers(const Interface_t *interface)
{
    /*
     * Ignore invalid registrations.
     */
    if (interface == NULL)
    {
        return;
    }

    /*
     * Copy architecture-specific callbacks and data into the
     * active kernel interface.
     */
    globalInterface = *interface;
}
