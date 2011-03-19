/*
5.2.2 UART I-Process
The UART i-process uses interrupts for both the transmission and receiving of characters from the serial port.
 No polling or busy waiting strategies may be implemented. 
 The UART i-process forwards characters (or commands) received to the KCD, 
 and also responds to messages received from the CRT display process to transmit characters to the serial port.
*/

extern PCB* current_running_process;
/*
*/

void uart_i_process(){
	
	BYTE temp = SERIAL1_USR;
	BOOLEAN newLine = TRUE;
	if (!newLine){ 
		charOut = charIn;
	}
	//if data is waiting
	if (temp & 1){
		CHAR charIn = SERIAL1_RD;
		void * msg = request_memory_block();
		*(msg + 3) =  charIn;
		send_message(-2, msg);//assume uart_i_process has id = -2.
	}else if (temp & 4)
	// if port is ready to accept data
	{
		SERIAL1_WD = charOut;
		SERIAL1_IMR = 2;
		if (charOut == CR){
			while (! temp & 4){}// blocking here?
			charOut = LF;
			SERIAL1_WD = charOut;	
		}
	}
	
}
