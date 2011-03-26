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