/*----------------------------------------------------------------------------
 *              A Dummy RTX for Testing 
 *----------------------------------------------------------------------------
 */
/**
 * @file: main_rtx.c
 * @author: Thomas Reidemeister 
 * @author: Irene Huang 
 * @date: 2010.02.18
 * @brief: Example dummy rtx for testing
 */

#include "rtx.h"
#include "rtx_test.h"
#include "dbug.h"
#include "memory.h"
#include "init.h"
#include "process.h"
#include "timer.h"
#include "kcd.h"
#include "priority.h"
#include "iprocess.h"
#include "messaging.h"
#include "timer.h";

/* test proc initializaiton info. registration function provided by test suite.
 * test suite needs to register its test proc initilization info with rtx
 * The __REGISTER_TEST_PROCS_ENTRY__ symbol is in linker scripts
 */

extern void __REGISTER_TEST_PROCS_ENTRY__();
extern UINT32* mem_end;
struct PCB p [NUM_PROCESS];
struct PCB null_p;
struct PCB keyboard_i_proc;
struct PCB timer_i_proc;
extern struct PCB* current_running_process;
extern struct PCB* prev_running_process;
struct PCB* ready_queue[5];
struct PCB* blocked_queue[2];

/* gcc expects this function to exist */
int __main( void )
{
    return 0;
}

int main() 
{
	
	// set up the trap
	asm( "move.l #asm_trap_entry,%d0" );
	asm( "move.l %d0,0x10000080" );

	asm( "move.l #asm_trap_entry,%d0" );
    asm( "move.l %d0,0x10000080" );
	
    rtx_dbug_outs((CHAR *)"rtx: Entering main()\r\n");

    /* get the third party test proc initialization info */
    __REGISTER_TEST_PROCS_ENTRY__();
	__REGISTER_RTX__();
	
	free_blocks = initBlock(NUM_MEM_BLKS);
	init_mailboxes();
	init_timer();
	
	int i = 0;
	UINT32* process_start = mem_end;
	current_running_process = NULL;
	prev_running_process = NULL;
	
	// initialize the ready and block queue
	for(i; i<5; i++)
	{
		ready_queue[i] = NULL;
	}
	
	i=0;
	for (i; i < 2; i++) {
		blocked_queue[i] = 0;
	}
	
	i = 0;
	for (i; i < 6; i++) {
		p[i].next = NULL;
		p[i].id = g_test_proc[i].pid;
		p[i].priority = g_test_proc[i].priority;
		process_start = process_start + g_test_proc[i].sz_stack/4;
		p[i].stack = process_start;
		p[i].waiting_on = -1;
				
		int val;
		//back up a7
		int original_a7;
		asm("move.l %%a7, %0" : "=r" (original_a7));	
		val = p[i].stack;
		asm("move.l %0, %%a7" : : "r" (val));
		val = *(g_test_proc[i].entry);			
		asm("move.l %0, %%d0" : : "r" (val));
		asm("move.l %d0, -(%a7)");
		val = 4;		// 1796 is the value that disable all interrupt	
		asm("move.w %0, %%d0" : : "r" (val));
		asm("move.w %d0, -(%a7)");
		val = 16512;			
		asm("move.w %0, %%d0" : : "r" (val));
		asm("move.w %d0, -(%a7)");
		p[i].stack -= 8;	
		
		//restore a7
		asm("move.l %0, %%a7" : : "r" (original_a7));
						
		// initialize the process to the correct ready queue
		// put_to_ready set the state to ready, so reset it to new
		put_to_ready(&(p[i]));		
		p[i].state = STATE_NEW;		
	}
	process_start = process_start + 2048/4;
	init_null_process(&null_p, process_start);
	
	process_start = process_start + 2048/4;
	init_kcd(&p[6], process_start);
	process_start = process_start + 2048/4;

	init_pm(&p[7], process_start);
	process_start = process_start + 2048/4;
	
	init_crt(&p[8], process_start);
	process_start = process_start + 2048/4;
	
	process_start = process_start + 2048/4;
	init_wall_clock(&p[9], process_start);
	
	process_start = process_start + 2048/4;
	init_keyboard_i_proc(&keyboard_i_proc, process_start);
	
	process_start = process_start + 2048/4;
	init_timer_i_proc(&timer_i_proc, process_start);
	
    init_interrupts();
	
	//call the scheduler to start a process
	schedule_next_process();
	//while (TRUE){}
    return 0;
}

SINT32 coldfire_vbr_init( VOID )
{
    /*
     * Move the VBR into real memory
     *
     * DG: actually, it'll already be here.
     */
    asm( "move.l %a0, -(%a7)" );
    asm( "move.l #0x10000000, %a0 " );
    asm( "movec.l %a0, %vbr" );
    asm( "move.l (%a7)+, %a0" );
    
    return RTX_SUCCESS;
}


void init_interrupts(){
	UINT32 mask;

    /* Disable all interupts */
    asm( "move.w #0x2700,%sr" );

    coldfire_vbr_init();
    
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

	asm( "move.l #asm_timer_entry,%d0" );
    asm( "move.l %d0,0x10000078" );

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
	
	
    return 0;
	
	
}

/* register rtx primitives with test suite */

void  __attribute__ ((section ("__REGISTER_RTX__"))) register_rtx() 
{
    rtx_dbug_outs((CHAR *)"rtx: Entering register_rtx()\r\n");
    g_test_fixture.send_message = send_message;
    g_test_fixture.receive_message = receive_message;
    g_test_fixture.request_memory_block = request_memory_block;
    g_test_fixture.release_memory_block = release_memory_block;
    g_test_fixture.release_processor = release_processor;
    g_test_fixture.delayed_send = delayed_send;
    g_test_fixture.set_process_priority = set_process_priority;
    g_test_fixture.get_process_priority = get_process_priority;
    rtx_dbug_outs((CHAR *)"rtx: leaving register_rtx()\r\n");
}
