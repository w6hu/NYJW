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
	rtx_dbug_outs("Gid_test: START\r\n");
	int sender;
	void* msg = g_test_fixture.receive_message(&sender);
	int pass = 0;
	int fail = 0;
	int state = 1;
	
	if (sender != 4) {
		rtx_dbug_outs("Gid_test: test 1 FAIL!\r\n");
		fail = 1;
		pass = 0;
	}
	else {
		if (*((char*)msg + 100) != 'z') {
			fail = 1;
			rtx_dbug_outs("Gid_test: test 1 FAIL!!\r\n");
		}
		else {
			pass = *((int*)msg+25);
			fail = *((int*)msg+26);
			if (pass == 1) {
				rtx_dbug_outs("Gid_test: test 1 OK\r\n");
			}
			else {
				rtx_dbug_outs("Gid_test: test 1 FAIL!!!\r\n");
			}
		}
	}
	g_test_fixture.release_memory_block(msg);
	
	msg = g_test_fixture.receive_message(&sender);
	char content = *((char *)msg+64);
	if (sender == 2 && content == 'a') {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID");
		rtx_dbug_out_char(sender+48);
		rtx_dbug_outs("\r\n");
	}
	else {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID\r\n");
		state = 0;
	}
	g_test_fixture.release_memory_block(msg);
	
	msg = g_test_fixture.receive_message(&sender);
	content = *((char *)msg+64);
	if (sender == 3 && content == 'b') {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID");
		rtx_dbug_out_char(sender+48);
		rtx_dbug_outs("\r\n");
	}
	else {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID\r\n");
		state = 0;
	}
	g_test_fixture.release_memory_block(msg);
	
	msg = g_test_fixture.receive_message(&sender);
	content = *((char *)msg+64);
	if (sender == 2 && content == 'c') {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID");
		rtx_dbug_out_char(sender+48);
		rtx_dbug_outs("\r\n");
	}
	else {
		rtx_dbug_outs("Gid_test: TEST1 received message from PID\r\n");
		state = 0;
	}
	g_test_fixture.release_memory_block(msg);
	
	if (state == 1) {
		pass++;
		rtx_dbug_outs("Gid_test: test 2 OK\r\n");
	}
	else {
		fail++;
		rtx_dbug_outs("Gid_test: test 2 FAIL\r\n");
	}
	
	rtx_dbug_outs("Gid_test: ");
	rtx_dbug_out_char(pass+48);
	rtx_dbug_outs("/2 tests OK\r\n");
	rtx_dbug_outs("Gid_test: ");
	rtx_dbug_out_char(fail+48);
	rtx_dbug_outs("/2 tests FAIL\r\n");
	rtx_dbug_outs("Gid_test: END");
	
	while (1) {
		g_test_fixture.release_processor();
	}
}

/* third party dummy test process 2 */ 
void test2()
{
	void* newMsg = g_test_fixture.request_memory_block();
	*((char *)newMsg + 64) = 'c';
	rtx_dbug_outs("Gid_test: TEST2 delay send message from PID1 with 10 second\r\n");
	g_test_fixture.delayed_send(1, newMsg, 10000);
	
	void* newMsg2 = g_test_fixture.request_memory_block();
	*((char *)newMsg2 + 64) = 'a';
	rtx_dbug_outs("Gid_test: TEST2 delay send message from PID1 with 6 second\r\n");
	g_test_fixture.delayed_send(1, newMsg2, 6000);
	
	while (1) {
		g_test_fixture.release_processor();
	}
	
	// nobody sends message to test2. should be blocked forever
	/*int sender;
	void* msg = g_test_fixture.receive_message(&sender);
	int fail = *((int*)msg+26);
	rtx_dbug_outs("Gid_test: TEST1 received message from PID");
	rtx_dbug_out_char(*sender+48);
	rtx_dbug_outs("\r\n");
	fail++;
	rtx_dbug_outs("Gid_test: test 1 FAIL\r\n");
	g_test_fixture.release_memory_block(msg);*/
}
/* third party dummy test process 3 */ 
void test3()
{
	void* newMsg = g_test_fixture.request_memory_block();
	*((char *)newMsg + 64) = 'b';
	rtx_dbug_outs("Gid_test: TEST3 delay send message from PID1 with 8 second\r\n");
	g_test_fixture.delayed_send(1, newMsg, 8000);
	
	while (1) {
		g_test_fixture.release_processor();
	}
}

/* third party dummy test process 4 */ 
void test4()
{
	int sender;
	void* msg = g_test_fixture.receive_message(&sender);
	rtx_dbug_outs("Gid_test: TEST4 receives message from PID");
	rtx_dbug_out_char(sender+48);
	rtx_dbug_outs("\r\n");
	int pass = 0;
	int fail = 0;
	int state = 1;
	g_test_fixture.release_memory_block(msg);
	if (sender != 6) {
		state = 0;
	}
	
	int priority = g_test_fixture.get_process_priority(6);
	rtx_dbug_outs("Gid_test: TEST4 get TEST6 priority: ");
	rtx_dbug_out_char(priority + 48);
	rtx_dbug_outs("\r\n");
	if (priority != 2) {
		state = 0;
	}
	
	if (state == 1) {
		pass++;
	}
	else {
		fail++;
	}
	
	void* newMsg = g_test_fixture.request_memory_block();
	*((int*)newMsg + 25) = pass;
	*((int*)newMsg + 26) = fail;
	*((char*)newMsg + 100) = 'z';
	
	rtx_dbug_outs("Gid_test: TEST4 sends message to PID1\r\n");
	g_test_fixture.send_message(1, newMsg);
	
	while (1) {
		g_test_fixture.release_processor();
	}
}
/* third party dummy test process 5 */ 
void test5()
{	
	rtx_dbug_outs("Gid_test: TEST5 sets priority of PID6 to 2\r\n");
	g_test_fixture.set_process_priority(6, 2);
	
	void* newMsg = g_test_fixture.request_memory_block();
	rtx_dbug_outs("Gid_test: TEST5 sends message to PID4\r\n");
	g_test_fixture.send_message(4, newMsg);
	
	while (1) {
		g_test_fixture.release_processor();
	}
}
/* third party dummy test process 6 */ 
void test6()
{
	void* newMsg = g_test_fixture.request_memory_block();
	rtx_dbug_outs("Gid_test: TEST6 sends message to PID4\r\n");
	g_test_fixture.send_message(4, newMsg);
	
	while (1) {
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
