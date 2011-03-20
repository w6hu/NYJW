#include "kcd.h"



extern struct PCB p [NUM_PROCESS];
void kcd()
{
    while (1) 
    {
		//rtx_dbug_outs((CHAR *)"kcd: calling recieve message \r\n");
		int sender_id;
		//*sender_id = 3;
		
		//rtx_dbug_outs((CHAR *)"kcd: before message:");
		
        void * block =  receive_message (&sender_id);
		
		//rtx_dbug_outs((CHAR *)"kcd: after message:");
		
		//rtx_dbug_outs((CHAR*)"sender id:");
		//rtx_dbug_out_num(sender_id);
		if (block!= NULL && sender_id == -3){
			//rtx_dbug_outs((CHAR*)"release memory location @ ");
			//rtx_dbug_out_num((int)block);	
			//rtx_dbug_outs((CHAR *)"kcd: recieved message:");
			//rtx_dbug_out_char(*(((CHAR*)block)+100));
			//rtx_dbug_outs((CHAR *)"\r\n");
			release_memory_block(block);//just mock up~
			
		}
		release_processor();
		//put_to_blocked(1,&p[6]);
    }
}

void init_kcd (struct PCB* pcb_kcd, UINT32* stackPtr)
{	
	rtx_dbug_outs((CHAR *)"init_kcd \r\n");
	pcb_kcd->next = NULL;
	pcb_kcd->id = -4;
	pcb_kcd->priority = 3;
	pcb_kcd->stack = stackPtr;
	pcb_kcd->returning = FALSE;
	pcb_kcd->waiting_on = -1;
		
	int val;
	//back up a7
	int original_a7;
	asm("move.l %%a7, %0" : "=r" (original_a7));	
	val = pcb_kcd->stack;
	asm("move.l %0, %%a7" : : "r" (val));
	val = kcd;			
	asm("move.l %0, %%d0" : : "r" (val));
	asm("move.l %d0, -(%a7)");
	val = 4;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	val = 16512;			
	asm("move.w %0, %%d0" : : "r" (val));
	asm("move.w %d0, -(%a7)");
	pcb_kcd->stack -= 8;

	//restore a7
	asm("move.l %0, %%a7" : : "r" (original_a7));
		
	// initialize the process to the correct ready queue
	put_to_ready(pcb_kcd);
	pcb_kcd->state = STATE_NEW;	
	rtx_dbug_outs((CHAR *)"init_kcd: exited \r\n");
}
/*
void kcd()
{
	char availabe_command[10] = {' '};
	while(1)
	{
		void* user_command = receive_message(NULL);
		int	message_type = *((int*)user_command);
		char command = *((char*)user_command+400);
		
		if(message_type == COMMAND_REGISTER)
		{
			// do the registyeration
		}
		else if(message_type == COMMAND_KEYBOARD)
		{
			if(command == '%')
			{
				user_command = receive_message(NULL);
				message_type = *((int*)user_command);
				command = *((char*)user_command+400);		
				// sanity check
				if(message_type != COMMAND_KEYBOARD)
					continue;
					
				if(command == 'W')
				{
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					command = *((char*)user_command+400);	
					// sanity check
					
					if(message_type != COMMAND_KEYBOARD)
						continue;
					if(command == 'S')
					{	
						if(handle_time_string() == FALSE)
							continue;
						
						// send a message to call clock
						continue;
					}
					else if(command == 'T')
					{
						user_command = receive_message(NULL);
						message_type = *((int*)user_command);
						command = *((char*)user_command+400);
						// this one has to be a white space
						if(message_type != COMMAND_KEYBOARD || command == CR)
							continue;	
						
						// send a message to wall clock
						continue;
					}
				}
				else if(command == 'C')
				{
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					command = *((char*)user_command+400);
					// this one has to be a white space
					if(message_type != COMMAND_KEYBOARD || command == ' ')
						continue;
					
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					int process_id = *((int*)user_command+100) - 48;
					if(message_type != COMMAND_KEYBOARD)
						continue;
	
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					command = *((char*)user_command+400);
					// this one has to be a white space
					if(message_type != COMMAND_KEYBOARD || command == ' ')
						continue;	
	
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					int priority = *((int*)user_command+100) - 48;
					if(message_type != COMMAND_KEYBOARD)
						continue;
						
					user_command = receive_message(NULL);
					message_type = *((int*)user_command);
					command = *((char*)user_command+400);
					// this one has to be a white space
					if(message_type != COMMAND_KEYBOARD || command == CR)
						continue;							
						
					// send a message to set process priority PROCESS
					continue;
				}
			}
			else 
				continue;
		}
	
	}
}

int handle_time_string()
{
	int hour = 0;
	int minute = 0;
	int second = 0;
	char command;
	
	void* user_command = receive_message(NULL);
	int message_type = *((int*)user_command);
	int hour10 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (hour10 >2 || hour10 <0))
		return FALSE;
		
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	int hour1 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (hour1 >4 || hour1 <0))
		return FALSE;
	
	hour = hour10*10 + hour1;
	
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	command = *((char*)user_command+400);
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || command != ':')
		return FALSE;
		
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	int minute10 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (minute10 >5 || minute10 <0))
		return FALSE;
		
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	int minute1 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (minute1 >9 || minute1 <0))
		return FALSE;
	
	minute = minute10*10 + minute1;
	
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	command = *((char*)user_command+400);
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || command != ':')
		return FALSE;
	
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	int second10 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (second10 >5 || second10 <0))
		return FALSE;
		
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	int second1 = *((int*)user_command+100) - 48;
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || (second1 >9 || second1 <0))
		return FALSE;
	
	user_command = receive_message(NULL);
	message_type = *((int*)user_command);
	command = *((char*)user_command+400);
	// this one has to be a white space
	if(message_type != COMMAND_KEYBOARD || command == CR)
		return FALSE;	
		
	second = second10*10 + second1;
	
	// assuming that the KCD has a relatively high priority, higher than all other process, then it
	// should continue to execute after the send message and gets blocked on the next receive message
	// which is happening right afterward.
	// send a message to the wall clock
	
	return;
}*/
