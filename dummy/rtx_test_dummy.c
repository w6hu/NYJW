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

/* third party dummy test process 1 */ 
void test1()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test1\r\n");
    
    while (1) 
    {
        /* execute a rtx primitive to test */
		void* msg1 = g_test_fixture.request_memory_block();
		
		
		rtx_dbug_outs("request memory address\r\n");
		int last = (int)msg1%10;
		int remain = (int)msg1;
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		
		
		*((UINT32 *)msg1 + 10) = 2;
		*((UINT32 *)msg1 + 11) = 3;
		//*((UINT32 *)msg2 + 10) = 4;
		//*((UINT32 *)msg2 + 11) = 5;
		int result = g_test_fixture.send_message(4, msg1);
		if (result == RTX_SUCCESS) {
			rtx_dbug_outs("Success!\r\n");
		}
		else {
			rtx_dbug_outs("Failed!\r\n");
		}
        g_test_fixture.release_processor();
    }
}

/* third party dummy test process 2 */ 
void test2()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test2\r\n");
    while (1) 
    {
		int val  = 1;
		int *sender_id = &val; 
		void* reMsg1 = g_test_fixture.receive_message(sender_id);
		if (reMsg1 != NULL) {
			rtx_dbug_outs("SUCCESS!!\r\n");
			rtx_dbug_out_char(*((UINT32 *)reMsg1 + 10)+48);
			rtx_dbug_out_char(*((UINT32 *)reMsg1 + 11)+48);
		}
		else {
			rtx_dbug_outs("NO Message!!\r\n");
		}
	
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
    }
}
/* third party dummy test process 3 */ 
void test3()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test3\r\n");
    while (1) 
    {
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
    }
}

/* third party dummy test process 4 */ 
void test4()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test4\r\n");
    while (1) 
    {
		rtx_dbug_outs("Back to beginning of the loop \r\n");
		int val  = 6;
		int *sender_id = &val; 
		void* reMsg1 = g_test_fixture.receive_message(sender_id);
		if (reMsg1 != NULL) {
			rtx_dbug_outs("SUCCESS!!\r\n");
			rtx_dbug_out_char(*((UINT32 *)reMsg1 + 10)+48);
			rtx_dbug_out_char(*((UINT32 *)reMsg1 + 11)+48);
		}
		else {
			rtx_dbug_outs("NO Message!!\r\n");
		}
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
		rtx_dbug_outs("Coming back to P1 \r\n");
    }
}
/* third party dummy test process 5 */ 
void test5()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test5\r\n");
    while (1) 
    {
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
    }
}
/* third party dummy test process 6 */ 
void test6()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test6\r\n");
    while (1) 
    {
        /* execute a rtx primitive to test */
		
		void* msg1 = g_test_fixture.request_memory_block();
		
		
		rtx_dbug_outs("request memory address\r\n");
		int last = (int)msg1%10;
		int remain = (int)msg1;
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");
		
		
		*((UINT32 *)msg1 + 10) = 2;
		*((UINT32 *)msg1 + 11) = 3;
		//*((UINT32 *)msg2 + 10) = 4;
		//*((UINT32 *)msg2 + 11) = 5;
		int result = g_test_fixture.send_message(4, msg1);
		if (result == RTX_SUCCESS) {
			rtx_dbug_outs("Success!\r\n");
		}
		else {
			rtx_dbug_outs("Failed!\r\n");
		}
		
		
        g_test_fixture.release_processor();
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
