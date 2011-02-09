/**
 * @file: timer.c
 * @brief: timer 0 smaple code
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang
 * @date: 2011/01/04
 */

#include "../shared/rtx_inc.h"
#include "../dbug/dbug.h"

/*
 * Global Variables
 */
SINT32 Counter = 0;
long time = 0;
CHAR timeString[]= "hh:mm:ss\r";
volatile BYTE CharIn = '!';
volatile BOOLEAN Caught = TRUE;
volatile BYTE CharOut = '\0';
/*
 * gcc expects this function to exist
*/
int __main( void )
{
    return 0;
}

/*
 * This function is called by the assembly STUB function
 */
VOID c_timer_handler( VOID )
{
    Counter++;
    //rtx_dbug_out_char('.');
    /*
     * Ack the interupt
     */
    TIMER0_TER = 2;
}

VOID c_serial_handler( VOID )
{
    BYTE temp;

    temp = SERIAL1_USR;    /* Ack the interrupt */

    //rtx_dbug_outs("serial port called\r\n");
    /* See if data is waiting.... */    
    if( temp & 1 )
    {
		if (CharOut == 13){
			CharIn = 10;
		}else{
			CharIn = SERIAL1_RD;			
        }
		//Caught = FALSE;
    }

    /* See if port is ready to accept data */    
    else if ( temp & 4 )
    {
        SERIAL1_WD = CharOut;   /* Write data to port */
        SERIAL1_IMR = 2;        /* Disable tx Interupt */
		Caught = FALSE;
    }
    return;
}


SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
	 */
    asm( "move.l %a0, -(%a7)" );
    asm( "move.l #0x10000000, %a0 " );
    asm( "movec.l %a0, %vbr" );
    asm( "move.l (%a7)+, %a0" );
    
    return RTX_SUCCESS;
}

void convert (long time) {
	int h = time / 3600 % 24;
	int m = time % 3600 / 60;
	int s = time % 60;
	
	timeString[0] = h / 10 + 48;
	timeString[1] = h % 10 + 48;
	timeString[3] = m / 10 + 48;
	timeString[4] = m % 10 + 48;
	timeString[6] = s / 10 + 48;
	timeString[7] = s % 10 + 48;
}

void print () {
int i = 0;
//rtx_dbug_outs("print called\r\n");
	while(i <= 8)
    {
		if (Caught == FALSE){
			Caught = TRUE;
			CharOut = timeString[i];
			            
            /* Nasty hack to get a dynamic string format, 
             * grab the character before turning the interrupts back on. 
             */
           
            /* enable tx interrupts  */
			//SERIAL1_USR = 4;
			
            SERIAL1_IMR = 3;
			
	
	        /* Now print the string to debug, 
             * note that interrupts are now back on. 
             */
			
			i++;
		}
    }
}


/*
 * Entry point, check with m68k-coff-nm
 */
int main( void )
{
    UINT32 mask;

    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    coldfire_vbr_init();

    /*
     * Store the timer ISR at auto-vector #6
     */
    asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );
	
	/*
     * Store the serial ISR at user vector #64
     */
    asm( "move.l #asm_serial_entry,%d0" );
    asm( "move.l %d0,0x10000100" );
	
	   /* Reset the entire UART */
    SERIAL1_UCR = 0x10;





    /* Reset the receiver */
    SERIAL1_UCR = 0x20;
    
    /* Reset the transmitter */
    SERIAL1_UCR = 0x30;





    /* Reset the error condition */
    SERIAL1_UCR = 0x40;


    /* Install the interupt */

    SERIAL1_ICR = 0x17;
    SERIAL1_IVR = 64;

    /* enable interrupts on rx only */
    SERIAL1_IMR = 0x02;

    /* Set the baud rate */
    SERIAL1_UBG1 = 0x00;
#ifdef _CFSERVER_           /* add -D_CFSERVER_ for cf-server build */
    SERIAL1_UBG2 = 0x49;    /* cf-server baud rate 19200 */ 
#else
    SERIAL1_UBG2 = 0x92;    /* lab board baud rate 9600 */
#endif /* _CFSERVER_ */

    /* Set clock mode */
    SERIAL1_UCSR = 0xDD;

    /* Setup the UART (no parity, 8 bits ) */
    SERIAL1_UMR = 0x13;
    
    /* Setup the rest of the UART (noecho, 1 stop bit ) */
    SERIAL1_UMR = 0x07;

    /* Setup for transmit and receive */
    SERIAL1_UCR = 0x05;
	
	

    /*
     * Setup to use auto-vectored interupt level 6, priority 3
     */
    TIMER0_ICR = 0x9B;

    /*
     * Set the reference counts, ~10ms
     */
    TIMER0_TRR = 1758;

    /*
     * Setup the timer prescaler and stuff
     */
    TIMER0_TMR = 0xFF1B;

    /*
     * Set the interupt mask
     */
    mask = SIM_IMR;
    mask &= 0x0003fdff | 0x0003dfff;
    SIM_IMR = mask;    

    /* Let the timer interrupt fire, lower running priority */
    asm( "move.w #0x2000,%sr" );
    
    /* Wait for 5 seconds to pass */
    //rtx_dbug_outs( (CHAR *) "Waiting approx. 5 seconds for Counter > 500\n\r" );
    Counter=0;
	time =0;
	while (1) {
		if (Counter == 100 ) {
			time++;
			convert(time);
			Caught = FALSE;
			rtx_dbug_outs(timeString);			
			print();
			Counter = 0;
		}
	}
    return 0;
}




