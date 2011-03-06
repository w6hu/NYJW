/* Process related funtions */
/* Temperally put them here, but this is not the best place for them*/

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "init.h"
#include "dbug.h"

int release_processor();
void schedule_next_process_BACK();

#endif