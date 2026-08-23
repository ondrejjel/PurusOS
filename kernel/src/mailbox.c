#include <stdint.h>
#include <stddef.h>
#include <rtos_config.h>
#include <kernel_interface.h>
#include <fault_code.h>
#include <mailbox.h>

static Mail_t mailboxes[PU_MAILBOX_COUNT];

static bool is_mailbox_id_valid(size_t id)
{
    return id < PU_MAILBOX_COUNT;
}

Mail_t x_pu_read_mailbox(size_t id)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    return mailboxes[id];
}

void v_pu_write_mailbox(size_t id, uintptr_t data, uintptr_t metadata)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    mailboxes[id].data = data;
    mailboxes[id].metadata = metadata;
}

void v_pu_clear_mailbox(size_t id)
{
    if (!is_mailbox_id_valid(id))
    {
        v_pu_fault_trap(PU_MAILBOX_INVALID_ID, true);
    }

    mailboxes[id].data = 0;
    mailboxes[id].metadata = 0;
}
