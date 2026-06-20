#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "allocator.h"



typedef enum TaskState_t{       // tasks will have one of those states
    INVALID,                    // task is invalid when any fault happens or if there is a initialisation issue
    RUNNING,                    // task is running when scheduler swiches to it
    READY,                      // task is ready after yielding back to scheduler
    BLOCKED                     // task is blocked when waiting for external condition or when fault is detected
}TaskState_t;

typedef struct TCB_t {
    void* stackPtr;             // CURRENT stack pointer

    void (*task)(void *arg);    // Pointer to the task function
    void* arg;                  // Arguments passed to the task function

    void* stackBottom;          // bottom stack boundary
    void* stackTop;             // top stack boundary

    uint32_t errorCode;         // task error field
    uint16_t id;                // Task ID
    TaskState_t state;          // tasks current state
} TCB_t;


static uint16_t taskId = 0;

__attribute__((nonnull(1)))
TCB_t x_pu_create_task(void (*task)(void* arg),void* arguments, size_t stackSize){
    TCB_t tcb = {0}; // reset everything

    Arena_t taskAdresses = x_pu_alloc_arena(stackSize); // get adresses
    tcb.stackBottom = taskAdresses.stackBottom;         // assign bottom adress
    tcb.stackTop = taskAdresses.stackTop;               // assign top adress
    tcb.stackPtr = taskAdresses.stackTop;               // stack pointer has to be on upper bound because stack grows downward

    if (tcb.stackBottom == NULL || tcb.stackTop == NULL || stackSize == 0) { // null ptr safety check
        //todo: change hardcoded value to enum field
        tcb.errorCode = 0xFFFFAAAA; // set error code to signalise a fault
        return tcb;                 // return invalid task block
    }

    tcb.id = taskId;    //  set the id
    taskId++;           //  increment global id counter

    tcb.state = READY;  // default state


    tcb.task = task;        // pass function pointer
    tcb.arg = arguments;    // pass arguments

    return tcb;

}

