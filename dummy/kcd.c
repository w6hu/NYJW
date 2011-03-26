#include "kcd.h"

void register_command(int registering_process, char registering_command)
{
	void* register_request = request_memory_block();
	*((int *)register_request) = COMMAND_REGISTER;
	*((int *)register_request + 26) = registering_process;
	*((char *)register_request + 68) = registering_command;
	send_message(KCD_ID, register_request);
}

void init_kcd (struct PCB* pcb_kcd, UINT32* stackPtr)
{	
	pcb_kcd->next = NULL;
	pcb_kcd->id = KCD_ID;
	pcb_kcd->priority = 0;
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
	val = 9988;			
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
}

void kcd()
{	
	// make a buff of 30 ... for now, we might be able to use the stack as well
	char command_buff[30];
	char commands[28];
	int  handlers[28];
	int  num_handlers = 0;
	int  num_characters = 0;

	// declare all the local variable that will be used
	void* user_command;
	int	message_type;
	char command;
	int  registering_process;
	
	
	KCD_START:
		num_characters = 0;
	
	while(1)
	{				
		user_command = receive_message(NULL);
		message_type = *((int*)user_command);
		command = *((char*)user_command + 68);
		registering_process = *((int *)user_command + 26);
		release_memory_block(user_command);
		
		if(message_type == COMMAND_REGISTER)
		{
			if(num_handlers < 28)
			{
			
				commands[num_handlers] = command;
				handlers[num_handlers] = registering_process;
			
				num_handlers += 1;
			}
		}
		else if(message_type == COMMAND_KEYBOARD)
		{
			if(FALSE)
			{
				// here to process the list of hot keys
				// and then restart, wipe out the buff
				goto KCD_START;
			} 
			else 
			{
				// put whatever thing onto the buffer if it has any space
				if(command == CR && (num_characters < 3 || num_characters >= 30))
					goto KCD_START;
				
				if(num_characters < 30)
				{
					command_buff[num_characters] = command;
					num_characters += 1;
					
					if(command == CR && num_characters >= 3)
					{
						// if the user pressed ENTER, then parse the string
						if(command_buff[0] != '%')
						{
							// not a command
							goto KCD_START;
						}
						
						int i=0;
						int destination_process = -1;
						char command_identifier = command_buff[1];
						
						for(i; i<num_handlers; i++)
						{
							if(commands[i] == command_identifier)
							{
								destination_process = handlers[i];
								break;
							}
						}
						
						// send the message to the handler
						void* initiate_action_request = request_memory_block();
						*((int *)initiate_action_request) = COMMAND_PRIORITY_MODIFIER;
						*((int *)initiate_action_request + 16) = num_characters - 2;
						
						// put the command string in the message 
						int j=0;
						for(j; j<num_characters-2; j++)
						{
							*((char *)initiate_action_request + 68 + j) = command_buff[j+2];
						}
						
						send_message(destination_process, initiate_action_request);
						goto KCD_START;
					}
				}
			}
		}
	}
}
