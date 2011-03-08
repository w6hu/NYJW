#include "messaging.h"


UINT32 *mailboxStart[NUM_PROCESS];
UINT32 *mailboxEnd[NUM_PROCESS];


/* Header format
|				|			|				|
|  Message Type	| Sender ID	| Receiver ID	| 
|				|			|				|
*/ 



void init_mailboxes() {
	int i;
	for (i = 0; i < NUM_PROCESS; i++) {
		mailboxStart[i] = 0;
		mailboxEnd[i] = 0;
	}
}

// I'm assuming here process_ID is the receiver ID
int send_message_jessie (int process_ID, void * MessageEnvelope) {
	// error check
	if (process_exists(process_ID) == FALSE) {
		return RTX_FAILURE;
	}
	
	int sender_ID = get_process_ID();
	int id = sender_ID - 1;
	
	// fill in the message hearder
	*(((int*)MessageEnvelope) + 1) = sender_ID;
	*(((int*)MessageEnvelope) + 2) = process_ID;
	
	rtx_dbug_out_char((CHAR)(id+48));
		rtx_dbug_out_char('\r');
		rtx_dbug_out_char('\n');	

	// update mailbox
	if (mailboxStart[id] == 0) {
		mailboxStart[id] = (UINT32 *)MessageEnvelope;
		mailboxEnd[id] = (UINT32 *)MessageEnvelope;		
	}
	else {
		*(mailboxEnd[id]-1) = (UINT32 *)MessageEnvelope;
		*((UINT32 *)MessageEnvelope-1) = NULL;
		mailboxEnd[id] = (UINT32 *)MessageEnvelope;//?
	}

	// update process states
	if (is_waiting_for(process_ID, sender_ID)) {
		remove_from_blocked(sender_ID, get_process_from_ID(process_ID));
		if (get_process_priority_usagi_san(process_ID) > get_process_priority_usagi_san(sender_ID)) {
			// still problem here
			//preempt(sender_ID);
			//start_to_run(process_ID);
		
			release_processor_kuma_san();
		}
	}
	return RTX_SUCCESS;	
}

// If a message is not in the inbox yet, the process is put into the blocked queue
void * receive_message_jessie (int * sender_ID) {
	// error check
	if (process_exists(*sender_ID) == FALSE) {
		rtx_dbug_outs((CHAR *)"rtx_test: sender dne\r\n");
		return NULL;
	}
	
	// check if the message has arrived yet
	int sender_box = *sender_ID - 1;
	int id = get_process_ID();
	UINT32* message;
	
	while (TRUE) {
	message  = mailboxStart[sender_box];
	
	
	
	
	while (message != 0) {
		// if there is a message waiting*/		
		
		if ((int)(*(message + 2)) == id) {
			// take it out of the mailbox
			UINT32 previous = *((UINT32 *)message-2);
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
			message = *(UINT32 *)(message-1);
	
		}
		
	}
	
	// if the message is not there yet, put into blocked queue
	put_to_blocked(*sender_ID, get_process_from_ID(id));

	release_processor_kuma_san();
	}
}

