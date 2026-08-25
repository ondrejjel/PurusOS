#ifndef PU_MAILBOX_H
#define PU_MAILBOX_H

#include <stdint.h>
#include <stddef.h>

/*
 * Mailbox containing a data value and associated metadata.
 */
typedef struct
{
    uintptr_t data;
    uintptr_t metadata;
} Mail_t;

/*
 * Reads the contents of a mailbox identified by its ID.
 */
Mail_t x_pu_read_mailbox(size_t id);

/*
 * Writes data and metadata to a mailbox identified by its ID.
 */
void v_pu_write_mailbox(size_t id, uintptr_t data, uintptr_t metadata);

/*
 * Clears the data and metadata stored in a mailbox identified by its ID.
 */
void v_pu_clear_mailbox(size_t id);

#endif /* PU_MAILBOX_H */
