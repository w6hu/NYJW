 #include "../dummy/rtx_inc.h"

void init_mailboxes();
int send_message (int process_ID, void * MessageEnvelope);
void * receive_message (int * sender_ID);
