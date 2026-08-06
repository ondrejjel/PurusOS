#ifndef PU_FAULT_CODE_H
#define PU_FAULT_CODE_H

typedef enum
{
    PU_TASK_OK = 0,
    PU_TASK_STACK_TOO_SMALL,
    PU_TASK_STACK_TOO_BIG,
    PU_TASK_NULL_FUNCTION,
} TaskFault_t;

#endif /* PU_FAULT_CODE_H */
