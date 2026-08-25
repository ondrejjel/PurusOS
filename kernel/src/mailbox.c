#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <rtos_config.h>
#include <kernel_interface.h>
#include <fault_code.h>
#include <mailbox.h>

/*
 * Mailbox storage shared by all tasks through the mailbox API.
 */
static Mail_t mailboxes[PU_MAILBOX_COUNT];

/*
 * Checks whether a mailbox ID refers to a valid mailbox.
 */
static bool is_mailbox_id_valid(size_t id)
{
    return id < PU_MAILBOX_COUNT;
}

/*
 * Reads the contents of a mailbox after validating its ID.
 */
Mail_t x_pu_read_mailbox(size_t id)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    return mailboxes[id];
}

/*
 * Writes data and metadata to a mailbox after validating its ID.
 */
void v_pu_write_mailbox(size_t id, uintptr_t data, uintptr_t metadata)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    mailboxes[id].data = data;
    mailboxes[id].metadata = metadata;
}

/*
 * Clears the data and metadata stored in a mailbox after validating
 * its ID.
 */
void v_pu_clear_mailbox(size_t id)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    mailboxes[id].data = 0;
    mailboxes[id].metadata = 0;
}
