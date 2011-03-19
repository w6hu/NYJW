/**
 * @file: dbug.h
 * @author: ECE354 Lab Instructors and TAs
 * @author: Irene Huang  
 * @date: 2010/05/03
 * @brief: Header file of dbug.c.  
 */
#ifndef _DBUG_H_
#define _DBUG_H_

#include "rtx_inc.h"
#include "init.h"

VOID rtx_dbug_out_char( CHAR c );   // output a char to janusROM terminal
SINT32 rtx_dbug_outs( CHAR* s );    // output a string to janusROM terminal
//void print_ready_queue() 			 

#endif /* _DBUG_H_ */
