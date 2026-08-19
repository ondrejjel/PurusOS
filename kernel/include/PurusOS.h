#ifndef PU_PURUSOS_H
#define PU_PURUSOS_H

#include <stddef.h>
#include <kernel_interface.h>

void selectKernelInterface(const KernelInterface_t *interface);
void startKernel(void);
void newTask(void (*task)(void *arg), void *arguments, size_t stackSize);
void taskYield(void);

#endif /* PU_PURUSOS_H */
