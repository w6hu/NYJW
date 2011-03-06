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

#include "rtx.h"
#include "rtx_test.h"
#include "dbug.h"

extern test_fixture_t g_test_fixture;
extern test_proc_t g_test_proc[6];

/* third party dummy test process 1 */ 
void test1()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test1\r\n");
    
    while (1) 
    {
		  rtx_dbug_outs((CHAR *)"rtx_test: test1a\r\n");
		  
		g_test_fixture.request_memory_block();
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
    }
}

/* third party dummy test process 2 */ 
void test2()
{
    rtx_dbug_outs((CHAR *)"rtx_test: test2\r\n");
    while (1) 
    {
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
        /* execute a rtx primitive to test */
        g_test_fixture.release_processor();
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
	
	if (g_test_proc[0].entry == NULL) {
		rtx_dbug_outs((CHAR *)"rtx: Null!!\r\n");
	  } else {rtx_dbug_outs((CHAR *)"rtx: Not Null!!\r\n");}
	  
	  /*int temp_end = &(g_test_proc[0].entry);
		int last; //= tempEnd%10;
		int remain = temp_end;
		//int i = 0; 
		while (remain != 0) {
			//rtx_dbug_out_char((CHAR)(last+48));
			last = remain%10;
			remain = remain/10;
			rtx_dbug_out_char((CHAR)(last+48));            
		}
		rtx_dbug_outs((CHAR *) "\r\n");*/
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
