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
	
	int receiver_box = get_process_number_from_ID(process_ID);
	int sender_ID = get_process_ID();
	
	// fill in the message hearder
	*(((int*)MessageEnvelope) + 1) = sender_ID;
	*(((int*)MessageEnvelope) + 2) = process_ID;

	// update mailbox
	if (mailboxStart[receiver_box] == 0) {
		mailboxStart[receiver_box] = (UINT32 *)MessageEnvelope;
		mailboxEnd[receiver_box] = (UINT32 *)MessageEnvelope;
	}
	else {
		*(mailboxEnd[receiver_box]-1) = (UINT32 *)MessageEnvelope;
		*((UINT32 *)MessageEnvelope-1) = NULL;
		mailboxEnd[receiver_box] = (UINT32 *)MessageEnvelope;//?
	}

	// update process states
	if (is_waiting_for(process_ID, 0)) {
		remove_from_blocked(0, get_process_from_ID(process_ID));
		if (get_process_priority_usagi_san(process_ID) > get_process_priority_usagi_san(sender_ID)) {
			release_processor_kuma_san();
		}
	}
	return RTX_SUCCESS;	
}

// If a message is not in the inbox yet, the process is put into the blocked queue
void * receive_message_jessie (int * sender_ID) {
	// check if the message has arrived yet
	int receiver_ID = get_process_ID();
	int receiver_box = get_process_number_from_ID(receiver_ID);
	UINT32* message = 0;
	
	// keep looping and receive message until a message comes in
	while (TRUE) {
		message = mailboxStart[receiver_box];
		if (message == NULL) {
			// if the message is not there yet, put into blocked queue
			put_to_blocked(0, get_process_from_ID(receiver_ID));
			rtx_dbug_outs("No message, blocked\r\n");
			release_processor_kuma_san();
		}
		else {
			// take it out of the mailbox
			UINT32 previous = *((UINT32 *)message-2);
			UINT32 next = *((UINT32 *)message-1);
			if (next) {
				*((UINT32 *)next-2) = previous;
			}   
			else {
				mailboxEnd[receiver_box] = (UINT32 *)previous;
			}
			if (previous) {
				*((UINT32 *)previous-1) = next;
			}
			else {
				mailboxStart[receiver_box] = (UINT32 *)next;
			}
			// fill in the sender_ID
			sender_ID = (int *)message+1;
			return message;   
		}
	}
}

int delayed_send_umi_san (int process_ID, void * MessageEnvelope, int delay) {
	
}
