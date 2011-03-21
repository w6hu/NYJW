#include "timer.h"

int timeCounter;
UINT32* timeout_queue;

extern struct PCB* current_running_process;
extern struct PCB timer_i_proc;


int init_timer() {
	timeCounter = 0;
	timeout_queue = 0;
}

void put_into_queue(void* message, int delay) {
//rtx_dbug_outs("put into queue");
	if (timeout_queue == NULL) {
		timeout_queue = message;
	}
	else {
		// find the last one whose delay is smaller than the given delay
		UINT32* temp = timeout_queue;
		//rtx_dbug_outs("put into queue");
		/*if (*(temp-1) != 0) {
			rtx_dbug_outs("put into queue stage 1");
		}*/
		
		if (*((int *)temp+4) > delay) {
			*((UINT32 *)message-1) = temp;
			*((UINT32 *)message-2) = NULL;
			*((UINT32 *)temp-2) = message; 
			timeout_queue = message;
		}
		else {
			while (*(temp-1) != 0 && *((int *)(*(temp-1))+4) < delay) {
			//rtx_dbug_outs("put into queue hahahahaha");
				temp = *((UINT32 *)temp-1);
			}
			//rtx_dbug_outs("put into queue");
			// append it
			UINT32 next = *((UINT32 *)temp-1);
			*((UINT32 *)temp-1) = message;
			*((UINT32 *)message-1) = next;
			*((UINT32 *)message-2) = temp;
			if (next != NULL) {
				*((UINT32 *)next-2) = message;
			}
		}
		//rtx_dbug_outs("put into queue 2");
	}
}

void handle_expired() {
	UINT32* message;
	UINT32* next;
	
	// start from the head of the queue
	while (timeout_queue != NULL && *((int *)timeout_queue+4) <= timeCounter) {
//rtx_dbug_outs("expired??\r\n");
//rtx_dbug_out_num(timeCounter);
//rtx_dbug_outs(" ");
//rtx_dbug_out_num(*((int *)timeout_queue+4));
		// remove from the queue
		message = timeout_queue;
		timeout_queue = (UINT32 *)(*(message-1));
		*(message-1) = NULL;
		*(message-2) = NULL;
		*(message+4) = 0;

//rtx_dbug_outs("resending it");
		// send the message
		int receiver = *((int *)message+2);
		int sender = *((int *)message+1);
		//rtx_dbug_outs("sending message from ");
		//rtx_dbug_out_num(sender);
		act_send_message(receiver, sender, message);
	}	
}

void handle_timer() {
	asm( "move.w #0x2700,%sr" );

	//rtx_dbug_outs("timer trigger!");
	
	struct PCB* backup = current_running_process;
	current_running_process = &timer_i_proc; 
	
	// update the time
	timeCounter++;
	
	// receive. should not block
	int sender_ID;
	void *message = receive_message_jessie (&sender_ID, 0); 
	while (message != NULL) {
		int delay = *((int *)message+4);
		delay = delay + timeCounter;
		*((int *)message+4) = delay;
/*rtx_dbug_outs("delay is ");
rtx_dbug_out_num(delay);
rtx_dbug_outs("for sender ");
rtx_dbug_out_num(*((int *)message + 1));*/
		put_into_queue(message, delay);
		message = receive_message_jessie (&sender_ID, 0);
	}
	
		
	// check anyone is expired
	handle_expired();
	
	// done
	//release_processor_kuma_san()
	
	current_running_process = backup;
	
	/*
     * Ack the interupt
     */
    TIMER0_TER = 2;
}

void wall_clock() {
	int clock_on = FALSE;
	int sender;
	int hour, minute, second;
	void * new_msg = NULL;
	register_command(WALL_CLOCK_ID, 'W');
	rtx_dbug_outs("inside clock\r\n");
	while (TRUE) {
		void* incoming_msg = receive_message(&sender);
		
		int sender_ID = *((int *)incoming_msg + 1);
		
		rtx_dbug_outs("inside clock loop\r\n");
		
		// if the msg is from KCD
		if (sender_ID == KCD_ID) {
			int size = *((int *)incoming_msg + 16);
			if (size != 11 && size != 2) {
				goto ERROR_INPUT;
			}
			
			char* next = (char *)incoming_msg + 68;
			// start the wall clock;
			if (*next == 'S' && *(next+1) == ' ') {
				next += 2;
				//hour = 0;
				//minute = 0;
				//second = 0;
				new_msg = request_memory_block();
				*((int *)new_msg + 16) = 8;
				if (*next == '0' || *next == '1' || *next == '2') {
					int tempHr = (int)(*next - 48)*10;
					*((char *)new_msg + 68) = *next;
					next++;
					if (*next <= 57 && *next >= 48) {
						tempHr += (int)(*next - 48);
						*((char *)new_msg + 69) = *next;
						if (tempHr >= 24) {
							goto ERROR_INPUT;
						}
						next++;
						if (*next == ':') {
							*((char *)new_msg + 70) = *next;
							next++;
							if (*next >=48 && *next <= 54) {
								int tempMin = (int)(*next - 48) * 10;
								*((char *)new_msg + 71) = *next;
								next++;
								if (*next <= 57 && *next >= 48) {
									tempMin += (int)(*next - 48);
									*((char *)new_msg + 72) = *next;
									if (tempMin >= 60) {
										goto ERROR_INPUT;
									}
									next++;
									if (*next == ':') {
										*((char *)new_msg + 73) = *next;
										next++;
										if (*next >=48 && *next <= 54) {
											int tempSec = (int)(*next - 48) * 10;
											*((char *)new_msg + 74) = *next;
											next++;
											if (*next <= 57 && *next >= 48) {
												tempSec += (int)(*next - 48);
												*((char *)new_msg + 75) = *next;
												if (tempSec >= 60) {
													goto ERROR_INPUT;
												}
												hour = tempHr;
												minute = tempMin;
												second = tempSec;
												goto CORRECT_INPUT;
											}
											else {
												goto ERROR_INPUT;
											}
										}
										else {
											goto ERROR_INPUT;
										}
									}
									else {
										goto ERROR_INPUT;
									}
								}
								else {
									goto ERROR_INPUT;
								}
							}
							else {
								goto ERROR_INPUT;
							}
						}
						else {
							goto ERROR_INPUT;
						}
					}
					else {
						goto ERROR_INPUT;
					}
				}
				else {
					goto ERROR_INPUT;
				}
				
				ERROR_INPUT:
					rtx_dbug_outs("invalid command in clock");
					if (new_msg != NULL) {
						release_memory_block(new_msg);
					}
					goto FINALLY;
					
				CORRECT_INPUT:
					rtx_dbug_outs("Correct input!");
					if (clock_on == FALSE) {
						// send a message to myself
						void* my_message = request_memory_block();
						delayed_send(-6, my_message, 1000);
						clock_on = TRUE;
					}
					
					// send message to crt
					send_message(-5, new_msg);
					
					/*rtx_dbug_out_num(hour);
					rtx_dbug_out_char(':');
					rtx_dbug_out_num(minute);
					rtx_dbug_out_char(':');
					rtx_dbug_out_num(second);
					rtx_dbug_out_char('\r');
					rtx_dbug_out_char('\n');*/
					
				FINALLY:
					release_memory_block(incoming_msg);
			}
			else if (*next == 'T') {
				// terminate the clock
				clock_on = FALSE;
				release_memory_block(incoming_msg);
				//clear_mailbox(-6);
			}
			else {
				rtx_dbug_outs("invalid command in clock");
				release_memory_block(incoming_msg);
			}
		}
		// if the message is from myself
		else if (sender == -6) {
			if (clock_on) {
				// increament the clock
				second++;
				if (second == 60) {
					second = 0;
					minute++;
					if (minute == 60) {
						minute = 0;
						hour++;
						if (hour == 24) {
							hour = 0;
						}
					}
				}
				
				// resend the message to myself
				delayed_send(-6, incoming_msg, 1000);
				
				// send message to CRT
				new_msg = request_memory_block();
				*((int *)new_msg + 16) = 8;
				*((char *)new_msg + 68) = (char)(hour/10 + 48);
				*((char *)new_msg + 69) = (char)(hour%10 + 48);
				*((char *)new_msg + 70) = (char)58;
				*((char *)new_msg + 71) = (char)(minute/10 + 48);
				*((char *)new_msg + 72) = (char)(minute%10 + 48);
				*((char *)new_msg + 73) = (char)58;
				*((char *)new_msg + 74) = (char)(second/10 + 48);
				*((char *)new_msg + 75) = (char)(second%10 + 48);
				send_message(-5, new_msg);
				
				/*rtx_dbug_out_num(hour);
					rtx_dbug_out_char(':');
					rtx_dbug_out_num(minute);
					rtx_dbug_out_char(':');
					rtx_dbug_out_num(second);
					rtx_dbug_out_char('\r');
					rtx_dbug_out_char('\n');*/
				
			}
			else {
				release_memory_block(incoming_msg);
			}
		}
		else {
			release_memory_block(incoming_msg);
		}
	}
}

void init_wall_clock(struct PCB* pcb_wall_clock, UINT32* process_start)
{	
	pcb_wall_clock->next = NULL;
	pcb_wall_clock->id = -6;
	pcb_wall_clock->priority = 0;
	pcb_wall_clock->stack = process_start;
	pcb_wall_clock->returning = FALSE;
	pcb_wall_clock->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_wall_clock->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = wall_clock;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_wall_clock->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_wall_clock);
	pcb_wall_clock->state = STATE_NEW;	
}
