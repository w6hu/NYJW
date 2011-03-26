#include "rtx_inc.h"
#include "memory.h"
#include "process.h"
#include "init.h"
#include "rtx.h"
#include "dbug.h"
#include "timer.h"
#include "hotkeys.h"

void process_a();
void init_process_a (struct PCB* pcb_proc_a, UINT32* stackPtr);
void process_b();
void init_process_b (struct PCB* pcb_proc_b, UINT32* stackPtr);
void process_c();
void init_process_c (struct PCB* pcb_proc_c, UINT32* stackPtr);