/* Process related funtions */
/* Temperally put them here, but this is not the best place for them*/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "init.h"
#include "rtx_inc.h"
#include "dbug.h"

int release_processor_kuma_san();
void schedule_next_process_BACK();

int get_process_ID();
int process_exists(int process_id);

#endif