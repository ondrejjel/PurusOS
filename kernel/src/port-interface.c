#include<stdint.h>
#include<stddef.h>

typedef struct Interface_t{
    uint32_t* defaultContext[16];
    void (*context_switch)(void);
} Interface_t;

Interface_t globalInterface = {0};

void v_pu_init_extern_pointers(const Interface_t* interface){
    globalInterface.context_switch = *interface->context_switch;
    globalInterface.defaultContext = *interface->defaultContext;
}
