/*
5.2.2 UART I-Process
The UART i-process uses interrupts for both the transmission and receiving of characters from the serial port.
 No polling or busy waiting strategies may be implemented. 
 The UART i-process forwards characters (or commands) received to the KCD, 
 and also responds to messages received from the CRT display process to transmit characters to the serial port.
*/

#include "iprocess.h"
/*
*/

CHAR charOut;
CHAR charIn;

void uart_i_process(){
	//rtx_dbug_outs((CHAR*)"key interrupt detected!\n\r");
	BYTE temp;
	temp = SERIAL1_USR;
	//if data is waiting
	if (temp & 1){
		charIn = SERIAL1_RD;
		SERIAL1_IMR = 3;//enable interrupt
		//void * msg = request_memory_block();
		//*((CHAR *)msg + 100) =  charIn;
		//send_message(-2, msg);//assume uart_i_process has id = -2.
	}else if (temp & 4)
	// if port is ready to accept data
	{
		//rtx_dbug_outs((CHAR*)"putting char to the other scren\n\r");
		charOut = charIn;
		SERIAL1_WD = charOut;
		rtx_dbug_outs((CHAR*)"you typed ");
		rtx_dbug_out_char(charOut);
		rtx_dbug_outs((CHAR*)"\n\r");
		SERIAL1_IMR = 2;//disable interrupt
		if (charOut == CR){
			while (! temp & 4){
				temp = SERIAL1_USR;
			}// blocking here?
			charOut = LF;
			SERIAL1_WD = charOut;	
		}
		//rtx_dbug_outs((CHAR*)"done putting stuff to the other screen\n\r");
	}
	//rtx_dbug_outs((CHAR*)"key interrupt finished!\n\n");
	
}
