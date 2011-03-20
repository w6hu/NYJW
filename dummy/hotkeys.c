#include "hotkeys.h"

extern struct PCB* current_running_process;
extern struct PCB* ready_queue[5];
extern struct PCB* blocked_queue[2];

void print_ready_queue() {
	int i = 0;
	rtx_dbug_outs((CHAR *) "Current Running Process: ");
	rtx_dbug_out_num(current_running_process->id);
	rtx_dbug_outs((CHAR *) "(");
	rtx_dbug_out_num(current_running_process->priority);
	rtx_dbug_outs((CHAR *) ")");
	rtx_dbug_outs((CHAR *) "\r\n");
	for (i; i < 5; i++) {
		rtx_dbug_outs((CHAR *) "Ready Queue Priority: ");
		rtx_dbug_out_char((CHAR) i + 48);
		rtx_dbug_outs((CHAR *) "\r\n");
	
		if (ready_queue[i] == NULL) {
			rtx_dbug_outs((CHAR *) "Empty\r\n");
		} else {
			rtx_dbug_outs((CHAR *) "Processes: ");
			struct PCB* temp = ready_queue[i];
			while (temp != NULL) {
				rtx_dbug_out_num(temp->id);
				temp = temp->next;
				rtx_dbug_outs((CHAR *) ", ");
			}
			rtx_dbug_outs((CHAR *) "\r\n");
		}
		rtx_dbug_outs((CHAR *) "\r\n");
	}
}

void print_blocked_queues() {
		
	rtx_dbug_outs((CHAR *) "Blocked on Recieve Message: \r\n");
	if (blocked_queue[0] == NULL) {
		rtx_dbug_outs((CHAR *) "Empty\r\n");
	} else {
		rtx_dbug_outs((CHAR *) "Processes (Priority): ");
		struct PCB* temp = blocked_queue[0];
		while (temp != NULL) {
			rtx_dbug_out_num(temp->id);
			rtx_dbug_outs((CHAR *) "(");
			rtx_dbug_out_num(temp->priority);
			rtx_dbug_outs((CHAR *) ")");
			temp = temp->next;
			rtx_dbug_outs((CHAR *) ", ");
		}
		rtx_dbug_outs((CHAR *) "\r\n");
	}
	rtx_dbug_outs((CHAR *) "\r\n");
	
	rtx_dbug_outs((CHAR *) "Blocked on Request Memory Block: \r\n");
	if (blocked_queue[1] == NULL) {
		rtx_dbug_outs((CHAR *) "Empty\r\n");
	} else {
			rtx_dbug_outs((CHAR *) "Processes (Priority): ");
		struct PCB* temp = blocked_queue[1];
		while (temp != NULL) {
			rtx_dbug_out_num(temp->id);
			rtx_dbug_outs((CHAR *) "(");
			rtx_dbug_out_num(temp->priority);
			rtx_dbug_outs((CHAR *) ")");
			temp = temp->next;
			rtx_dbug_outs((CHAR *) ", ");
		}
		rtx_dbug_outs((CHAR *) "\r\n");
	}
	rtx_dbug_outs((CHAR *) "\r\n");
	
}