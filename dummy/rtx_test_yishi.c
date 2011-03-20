/*--------------------------------------------------------------------------
 *                      RTX Test Suite 
 *--------------------------------------------------------------------------
 */
/**
 * @file:   rtx_test_dummy.c   
 * @author: Thomas Reidemeister
 * @author: Irene Huang
 * @date:   2010.02.11
 * @brief:  rtx test suite 
 */

#include "rtx_test.h"
#include "rtx.h"

int test = 0;

/* third party dummy test process 1 */ 
void test1()
{
	test = 0;
    
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 1 START\r\n");
		
		void* one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = '%';
		g_test_fixture.send_message(-4, one_message);
		
		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = 'C';
		g_test_fixture.send_message(-4, one_message);
	
		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = ' ';
		g_test_fixture.send_message(-4, one_message);

		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = '2';
		g_test_fixture.send_message(-4, one_message);
	
		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = ' ';
		g_test_fixture.send_message(-4, one_message);
	
		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = '0';
		g_test_fixture.send_message(-4, one_message);
		
		one_message = g_test_fixture.request_memory_block();
		*((int *)one_message) = COMMAND_KEYBOARD;
		*((char *)one_message + 100) = CR;
		g_test_fixture.send_message(-4, one_message);

		rtx_dbug_outs((CHAR *)"rtx_test: TEST 1 end of transimission\r\n");
		
        g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 1 END\r\n");
    }
}

/* third party dummy test process 2 */ 
void test2()
{
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 2 START\r\n");

        g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 2 END\r\n");
    }
}
/* third party dummy test process 3 */ 
void test3()
{
    while (1) 
    {		
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 3 START\r\n");
		
        g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 3 END\r\n");
    }
}

/* third party dummy test process 4 */ 
void test4()
{
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 4 START\r\n");		
	
        g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 4 END\r\n");
    }
}
/* third party dummy test process 5 */ 
void test5()
{
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 5 START\r\n");

		
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 5 END\r\n");
    }
}
/* third party dummy test process 6 */ 
void test6()
{
    while (1) 
    {
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 6 START\r\n");
		
		g_test_fixture.release_processor();
		rtx_dbug_outs((CHAR *)"rtx_test: TEST 6 END\r\n");
    }
}

/* register the third party test processes with RTX */
void __attribute__ ((section ("__REGISTER_TEST_PROCS__")))register_test_proc()
{
    int i;

    rtx_dbug_outs((CHAR *)"rtx_test: register_test_proc()\r\n");

    for (i =0; i< NUM_TEST_PROCS; i++ ) {
        g_test_proc[i].pid = i + 1;
        g_test_proc[i].priority = 3;
        g_test_proc[i].sz_stack = 2048;
    }
    g_test_proc[0].entry = test1;
    g_test_proc[1].entry = test2;
    g_test_proc[2].entry = test3;
    g_test_proc[3].entry = test4;
    g_test_proc[4].entry = test5;
    g_test_proc[5].entry = test6;
}

/**
 * Main entry point for this program.
 * never get invoked
 */
int main(void)
{
    rtx_dbug_outs((CHAR *)"rtx_test: started\r\n");
    return 0;
}
