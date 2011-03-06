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
#include "../init/init.h"

extern test_fixture_t g_test_fixture;
extern test_proc_t g_test_proc[6];

/* test proc initializaiton info. registration function provided by test suite.
 * test suite needs to register its test proc initilization info with rtx
 * The __REGISTER_TEST_PROCS_ENTRY__ symbol is in linker scripts
 */

extern void __REGISTER_TEST_PROCS_ENTRY__();

/* gcc expects this function to exist */
int __main( void )
{
    return 0;
}

int main() 
{

    rtx_dbug_outs((CHAR *)"rtx: Entering main()2 main_rtx.c\r\n");

    /* get the third party test proc initialization info */
    __REGISTER_TEST_PROCS_ENTRY__();
	__REGISTER_RTX__();

    /* The following  is just to demonstrate how to reference 
     * the third party test process entry point inside rtx.
     * Your rtx should NOT call the test process directly!!! 
     * Instead, the scheduler picks the test process to run
     * and the os context switches to the chosen test process
     */
	 //init();
	  rtx_dbug_outs((CHAR *)"rtx: Exit Init\r\n");
	  if (g_test_proc[0].entry == NULL) {
	  
		int temp_end = &(g_test_proc[0].entry);
		int last; //= tempEnd%10;
		int remain = temp_end;
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		
		rtx_dbug_outs((CHAR *)"rtx: Null\r\n");
	  }
	  
    g_test_proc[0].entry(); /* DO NOT invoke test proc this way !!*/ 

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
