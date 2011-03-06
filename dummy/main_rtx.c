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

/* test proc initializaiton info. registration function provided by test suite.
 * test suite needs to register its test proc initilization info with rtx
 * The __REGISTER_TEST_PROCS_ENTRY__ symbol is in linker scripts
 */

extern void __REGISTER_TEST_PROCS_ENTRY__();
extern UINT32* mem_end;
struct PCB p [6];

/* gcc expects this function to exist */
int __main( void )
{
    return 0;
}

int main() 
{

    rtx_dbug_outs((CHAR *)"rtx: Entering main()\r\n");

    /* get the third party test proc initialization info */
    __REGISTER_TEST_PROCS_ENTRY__();
	__REGISTER_RTX__();
	

	
	rtx_dbug_outs((CHAR *)"rtx: Exit Init\r\n");
	
	rtx_dbug_outs((CHAR *)"rtx: Entering init()\r\n");
	free_blocks = initBlock(NUM_MEM_BLKS);
	rtx_dbug_outs((CHAR *)"rtx: Created Memory blocks\r\n");
	
	int i = 0;
	UINT32* process_start = mem_end;
	
	for(i; i<5; i++)
	{
		ready_queue[i] = NULL;
	}
	

	i = 0;
	for (i; i < 6; i++) {
		rtx_dbug_outs((CHAR *)"rtx: Infinite Loop\r\n");
		p[i].next = NULL;
		p[i].id = g_test_proc[i].pid;
		rtx_dbug_outs((CHAR *)"rtx: Got PID\r\n");
		p[i].state = STATE_READY;
		p[i].priority = g_test_proc[i].priority;
		p[i].psw = 9984;   // assuming 9984 is the nomal initial state ... eh ?
		rtx_dbug_outs((CHAR *)"rtx: Getting pc\r\n");
		p[i].pc = g_test_proc[i].entry; //point pc to entry point of code
		rtx_dbug_outs((CHAR *)"rtx: pc set\r\n");
		p[i].stack = process_start; // where exactly is the process stack ?
		p[i].returning = FALSE;
		p[i].waiting_on = -1;
		
		int j = 0;
		for (j; j < 14; j++) {
			push(&(p[i]), 0);
		}
		
		// initialize the process to the correct ready queue
		put_to_ready(&(p[i]));

		process_start = process_start + g_test_proc[i].sz_stack/4;
	}
	
	//call the scheduler to start a process
	schedule_next_process();
	
	
	//  if (g_test_proc[0].entry == NULL) {
	//  rtx_dbug_outs((CHAR *)"rtx: Null\r\n");
	// }

    /* The following  is just to demonstrate how to reference 
     * the third party test process entry point inside rtx.
     * Your rtx should NOT call the test process directly!!! 
     * Instead, the scheduler picks the test process to run
     * and the os context switches to the chosen test process
     */
    //g_test_proc[0].entry(); /* DO NOT invoke test proc this way !!*/ 
	
	
	
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
