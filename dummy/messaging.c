#include "messaging.h"


UINT32 *mailboxStart[NUM_PROCESS+2];
UINT32 *mailboxEnd[NUM_PROCESS+2];


/* Header format
|				|			|				|				|
|  Message Type	| Sender ID	| Receiver ID	| Delayed Time	|
|				|			|				|				|
*/ 

void init_mailboxes() {
	int i;
	for (i = 0; i < NUM_PROCESS+2; i++) {
		mailboxStart[i] = 0;
		mailboxEnd[i] = 0;
	}
}

// executes before sending message and returns the sender ID
int pre_send_message (int process_ID, void *MessageEnvelope) {
	int sender_ID = get_process_ID();
	
	// fill in the message hearder
	*(((int*)MessageEnvelope) + 1) = sender_ID;
	*(((int*)MessageEnvelope) + 2) = process_ID;
	
	return sender_ID;
}

int act_send_message (int process_ID, int sender_ID, void * MessageEnvelope) {
	int receiver_box = get_process_number_from_ID(process_ID);
	
	// fill in the message hearder
	//*(((int*)MessageEnvelope) + 2) = process_ID;
	//rtx_dbug_outs((CHAR*)"receiver box");
	//rtx_dbug_out_num(receiver_box);

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
		/*if (get_process_priority_usagi_san(process_ID) < get_process_priority_usagi_san(sender_ID)) {
			release_processor_kuma_san();
		}*/
	}
}

// I'm assuming here process_ID is the receiver ID
int send_message_jessie (int process_ID, void * MessageEnvelope) {
	// error check
	if (process_exists(process_ID) == FALSE) {
		return RTX_FAILURE;
	}
	int sender = pre_send_message (process_ID, MessageEnvelope);
	
	act_send_message(process_ID, sender, MessageEnvelope);
	return RTX_SUCCESS;	
}

// If a message is not in the inbox yet, the process is put into the blocked queue
void * receive_message_jessie (int * sender_ID, int block) {
	// check if the message has arrived yet
	int receiver_ID = get_process_ID();
	//rtx_dbug_outs((CHAR*)"receive id=");
	//rtx_dbug_out_num(receiver_ID);
	int receiver_box = get_process_number_from_ID(receiver_ID);
	//rtx_dbug_outs((CHAR*)"receive box=");
	//rtx_dbug_out_num(receiver_box);
	UINT32* message = 0;

	// keep looping and receive message until a message comes in
	while (TRUE) {
		message = mailboxStart[receiver_box];
		if (message == NULL) {
			// if the message is not there yet, put into blocked queue
			//rtx_dbug_outs((CHAR*)"going to put to block, pid=");
			//rtx_dbug_out_num(receiver_ID);
			//put_to_blocked(0, get_process_from_ID(receiver_ID));
			if (block == 1) {
				put_to_blocked(0, get_process_from_ID(receiver_ID));
				release_processor_kuma_san();
			}
			else {
				//rtx_dbug_outs("No message, return null\r\n");
				return 0;
			}
		}
		else {
			/*rtx_dbug_outs((CHAR*)"PROCESS GOT MESSAGE: ");
			rtx_dbug_out_char(receiver_ID + 48);
			rtx_dbug_outs((CHAR*)" !!!!!!!!!!!!!!!!!!\r\n");*/
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
			int sender = *((int *)message+1);
			
			//rtx_dbug_out_char((CHAR)(sender+48));
			//rtx_dbug_outs(" is the sender in receive\r\n");
			
			*sender_ID = sender;
			
			// clear the message about the queue info
			*((UINT32 *)message - 1) = NULL;
			*((UINT32 *)message - 2) = NULL;
			
			return message;   
		}
	}
}

int delayed_send_umi_san (int process_ID, void * MessageEnvelope, int delay) {
	// error check
	if (process_exists(process_ID) == FALSE) {
		return RTX_FAILURE;
	}
	int sender_ID = get_process_ID();
	
	// fill in the message hearder
	*(((int*)MessageEnvelope) + 1) = sender_ID;
	*(((int*)MessageEnvelope) + 2) = process_ID;
	*(((int*)MessageEnvelope) + 4) = delay;
	
	//pre_send_message
	act_send_message (TIMER_INTERRUPT, sender_ID, MessageEnvelope);
	
	return RTX_SUCCESS;
}

/*void clear_mailbox (int process_ID) {
	int boxNumber = get_process_number_from_ID(process_ID);
	UINT32* next = mailboxStart[boxNumber];
	UINT32* temp = NULL;
	while (next != NULL) {
		 temp = next;
		 next = *((UINT32 *)temp - 1);
		*((UINT32 *)temp - 1) = NULL;
		*((UINT32 *)temp - 2) = NULL;
		release_memory_block(temp);
	}
	mailboxStart[boxNumber] = NULL;
	mailboxEnd[boxNumber] = NULL;
}*/
