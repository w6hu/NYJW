/**
 * @file: dbug.c
 * @brief: output to janusROM terminal by using trap #15
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2010/05/03
 */

#include "dbug.h"

/**
 * @brief: C Function wrapper for TRAP #15 function to output a character
 * @param: c the charcter to output to janusROM  
 */

//extern struct PCB* ready_queue[5];
 
VOID rtx_dbug_out_char( CHAR c )
{	
    /* Store registers */
    asm( "move.l %d0, -(%a7)" );
    asm( "move.l %d1, -(%a7)" );

    /* Load CHAR c into d1 */
    asm( "move.l 8(%a6), %d1" );  /* Standard Motorola syntax */ 
    //asm( "move.l (8, %a6), %d1" );/* Standard Motorola syntax */
    //asm( "move.l %a6@(8), %d1" ); /* Motorola 680x0 syntax developed by MIT */

    /*
    asm("move.l %0, %%d1"
       : // no output  
       :"g"(c)
       :"d1" 
       );
   */


    /* Setup trap function */
    asm( "move.l #0x13, %d0" );
    asm( "trap #15" );

    /* Restore registers  */
    asm(" move.l (%a7)+, %d1" );
    asm(" move.l (%a7)+, %d0" );
}


/**
 * @brief: Prints a C-style null terminated string
 * @param: s the string to output to janusROM terminal 
 */
SINT32 rtx_dbug_outs( CHAR* s )
{
    if ( s == NULL ) 
    {
        return RTX_ERROR;
    }
    while ( *s != '\0' )
    {
        rtx_dbug_out_char( *s++ );
    }
    return RTX_SUCCESS;
}
/*

void print_ready_queue() {
	int i = 0;
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
				int last; 
				int remain = temp->id;
				if (remain == 0) 
				rtx_dbug_outs((CHAR *) "0");
				while (remain != 0) {
					last = remain%10;
					remain = remain/10;
					rtx_dbug_out_char((CHAR)(last+48));            
				}
				temp = temp->next;
				rtx_dbug_outs((CHAR *) ", ");
			}
			printf ("\n");		
		}
		rtx_dbug_outs((CHAR *) "\r\n");
	}
}
*/