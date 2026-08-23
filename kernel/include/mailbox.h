#ifndef PU_MAILBOX_H
#define PU_MAILBOX_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uintptr_t data;
    uintptr_t metadata;
} Mail_t;

Mail_t x_pu_read_mailbox(size_t id);
void v_pu_write_mailbox(size_t id, uintptr_t data, uintptr_t metadata);
void v_pu_clear_mailbox(size_t id);

#endif /* PU_MAILBOX_H */
