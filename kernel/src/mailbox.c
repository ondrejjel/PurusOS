#include <rtos_config.h>

typedef struct
{
    uintptr_t data;
    uintptr_t metadata;
} Mail_t;

static Mail_t mailboxes[PU_MAILBOX_COUNT];

Mail_t x_pu_read_mailbox(size_t id)
{
    return mailboxes[id];
}

void v_pu_write_mailbox(size_t id, uintptr_t data, uintptr_t metadata)
{
    mailboxes[id].data = data;
    mailboxes[id].metadata = metadata;
}

void v_pu_clear_mailbox(size_t id)
{
    mailboxes[id].data = 0;
    mailboxes[id].metadata = 0;
}
