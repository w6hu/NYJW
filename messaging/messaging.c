#include "messaging.h"

#define SUCCESS 0
#define FAILURE 1

//extern int NUM_PROCESS;
int NUM_PROCESS = 6;

UINT32 *mailboxStart[NUM_PROCESS] = 0;
UINT32 *mailboxEnd[NUM_PROCESS] = 0;

// I'm assuming here process_ID is the receiver ID
int send_message (int process_ID, void * MessageEnvelope) {
	// error check
	if (process_exists(process_ID) == 0) {
		return FAILURE;
	}
	
	int id = get_process_ID() - 1;

	// update mailbox
	if (mailboxStart[id] = 0) {
		mailboxStart[id] = (UINT32 *)MessageEnvelope;
		mailboxEnd[id] = (UINT32 *)MessageEnvelop;
	}
	else {
		mailboxEnd[id]-1 = (UINT32 *)MessageEnvelope;
		*((UINT32 *)MessageEnvelop-1) = NULL;
		mailboxEnd[id] = (UINT32 *)MessageEnvelop;
	}

	// update process states
	int sender_ID = id;
	//int sender_ID = *((int *)MessageEnvelop + 2);
	if (is_waiting_for(process_ID, sender_ID)) {
		put_to_ready(process_ID);
		if (get_process_priority(process_ID) > get_process_priority(sender_ID)) {
			preempt(sender_ID);
			start_to_run(process_ID);
		}
	}
	return SUCCESS;	
}

// If a message is not in the inbox yet, the process is put into the blocked queue
void * receive_message (int * sender_ID) {
	// error check
	if (!process_exists(sender_ID)) {
		return FAILURE;
	}
	
	// check if the message has arrived yet
	int id = get_process_ID;
	int sender_box = sender_ID - 1;
	UINT32* message;
	message  = mailboxStart[sender_box];
	while (message != 0) {
		// if there is a message waiting
		if ((int)(*(message + 2)) == id) {
			// take it out of the mailbox
			UINT32 previous = *(UINT32 *)message-2;
			UINT32 next = *((UINT32 *)message-1);
			if (next) {
        		*((UINT32 *)next-2) = previous;
    		}   
    		else {
				mailboxEnd[sender_box] = (UINT32 *)previous;
			}
			if (previous) {
        		*((UINT32 *)previous-1) = next;
    		}
			else {
				mailboxStart[sender_box] = (UINT32 *)next;
			}
			return message;   
		}
		// else check next message
		else {
			message = (UINT32 *)(*(message-1));
		}
	}
	
	// if the message is not there yet, put into blocked queue
	put_to_block(sender_ID);
}



// some stubs
int process_exists(process_ID) {
return 1;
}

int get_process_ID() {
return 1;
}

