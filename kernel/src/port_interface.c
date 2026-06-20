#include <stdint.h>
#include <stddef.h>

/* Type definitions and local variables */

/*
 * Architecture interface structure.
 *
 * This structure contains architecture-specific data and callback
 * functions required by the kernel. An instance of this structure is
 * provided by the selected architecture port during kernel
 * initialisation.
 */
typedef struct
{
    /*
     * Pointer to the default task context image.
     *
     * This context image is copied onto a task's stack during task
     * creation and provides the initial register state expected by
     * the architecture port.
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

} PortInterface_t;

/*
 * Active architecture interface currently registered with the kernel.
 */
static PortInterface_t activeInterface = {0};

/* Interface accessors */

/*
 * Returns a pointer to the default task context image.
 */
const uint32_t *cu32p_pu_get_default_context(void)
{
    return activeInterface.defaultContext;
}

/*
 * Returns the size of the default task context image in 32-bit words.
 */
size_t s_pu_get_default_context_words(void)
{
    return activeInterface.defaultContextWords;
}

/*
 * Requests a context switch through the active architecture port.
 *
 * If no architecture interface has been registered, the request is
 * ignored.
 */
void v_pu_context_switch(void)
{
    if (activeInterface.context_switch != NULL)
    {
        activeInterface.context_switch();
    }
}

/* Initialisation */

/*
 * Registers an architecture interface with the kernel.
 *
 * The supplied interface provides architecture-specific data and
 * callback functions used by kernel components.
 */
void v_pu_init_extern_pointers(const PortInterface_t *interface)
{
    /*
     * Ignore invalid interface registrations.
     */
    if (interface == NULL)
    {
        return;
    }

    /*
     * Copy the supplied interface into the active kernel interface.
     */
    activeInterface = *interface;
}
