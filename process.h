/* Process related funtions */
/* Temperally put them here, but this is not the best place for them*/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "init.h"
#include "rtx_inc.h"
#include "dbug.h"

void null_process();
void init_null_process( struct PCB* null_process, UINT32* process_start);

int release_processor_kuma_san();
void schedule_next_process_BACK();

struct PCB* get_process_from_ID(int process_id);
int get_process_number_from_ID(int process_id);
int get_process_ID();
int get_process_priority_usagi_san(int process_id);
int process_exists(int process_id);
int is_waiting_for(int process_ID, int sender_ID);

// Only send the valid process_state, which is either STATE_READY or STATE_BLOCKED,
// this funtion does not check the validity of the state, so make sure you know what 
// you are doing with it.
void set_process_state(int process_id, int process_state);

// again, set the process with process ID to run.  Make sure to call scheduler
// or release_processor aftercalling this function.  This will override
// the default behavior of the scheduler and run the specified process instead
// of the next most priority process.
// Not used for now.
void set_process_to_run(int process_id);

#endif