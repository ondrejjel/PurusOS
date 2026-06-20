#include <stdint.h>
#include <stddef.h>

typedef struct Interface_t {
    // pointer to default context
    uint32_t** defaultContext;
    void (*context_switch)(void);
} Interface_t;

// This lives on the kernel side
static Interface_t globalInterface = {0};

void v_pu_init_extern_pointers(const Interface_t* interface) {
    if (interface == NULL) return;

    // Direct assignment works perfectly for function pointers
    globalInterface.context_switch = interface->context_switch;

    // Assign the pointer to the BSP's context structure
    globalInterface.defaultContext = interface->defaultContext;
}
