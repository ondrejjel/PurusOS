#ifndef PU_PURUSOS_H
#define PU_PURUSOS_H

#include <stddef.h>
#include <kernel_interface.h>
#include <mailbox.h>

Mail_t readMailbox(size_t id);

void clearMailbox(size_t id);

void writeMailbox(size_t id, uintptr_t data, uintptr_t metadata);

/* Selects and validates the architecture-provided kernel interface. */
void selectKernelInterface(const KernelInterface_t *interface);

/* Starts kernel execution using the first loaded task. */
void startKernel(void);

/* Creates a new task and loads it into the scheduler. */
void newTask(void (*task)(void *arg), void *arguments, size_t stackSize);

/* Saves the current task and switches execution to the next task. */
void taskYield(void);

#endif /* PU_PURUSOS_H */
