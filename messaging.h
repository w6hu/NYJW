#include "rtx_inc.h"
#include "memory.h"
#include "process.h"
#include "init.h"
#include "rtx.h"
 
void init_mailboxes();
int send_message_jessie (int process_ID, void * MessageEnvelope);
void * receive_message_jessie (int * sender_ID);
