#include "messaging.h"

#define SUCCESS 0
#define FAILURE 1

UINT32 *mailboxStart = 0;
UINT32 *mailboxEnd = 0;

// I'm assuming here process_ID is the receiver ID
int send_message (int process_ID, void * MessageEnvelope) {
	// error check
	if (!process_exists(process_ID)) {
		return FAILURE;
	}
	
	// update mailbox
	if (mailboxStart = 0) {
		mailboxStart = (UINT32 *)MessageEnvelope;
		mailboxEnd = (UINT32 *)MessageEnvelop;
	}
	else {
		mailboxEnd-1 = (UINT32 *)MessageEnvelope;
		*((UINT32 *)MessageEnvelop-1) = NULL;
		mailboxEnd = (UINT32 *)MessageEnvelop;
	}

	// update process states
	int sender_ID = *((int *)MessageEnvelop + 2);
	if (is_waiting_for(process_ID, sender_ID)) {
		put_to_ready(process_ID);
		if (get_process_priority(process_ID) > get_process_priority(sender_ID)) {
			preempt(sender_ID);
			start_to_run(process_ID);
		}
	}
	return SUCCESS;	
}

void * receive_message (int * sender_ID) {
	// error check
	if (!process_exists(sender_ID)) {
		return FAILURE;
	}
	
	// check if the message has arrived yet
	UINT32* message;
	message  = mailboxStart;
	while (message != 0) {
		// if there is a message waiting
		if ((int)(*(message + 2)) == *sender_ID) {
			// take it out of the mailbox
			UINT32 previous = *(UINT32 *)message-2;
			UINT32 next = *((UINT32 *)message-1);
			if (next) {
        		*((UINT32 *)next-2) = previous;
    		}   
    		else {
				mailboxEnd = (UINT32 *)previous;
			}
			if (previous) {
        		*((UINT32 *)previous-1) = next;
    		}
			else {
				mailboxStart = (UINT32 *)next;
			}
			return message;   
		}
		// else check next message
		else {
			message = (UINT32 *)(*(message-1));
		}
		return NULL;
}

