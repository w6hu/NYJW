/*****************************************************************
 *
 *                           E&CE 354 RTX
 *                   (c)1998, All Rights Reserved
 *
 *                  Chris McKillop and  Craig Stout
 *                  [cdmckill,castout]@uwaterloo.ca
 *
 *****************************************************************/

/*  $Id: rtx_inc.h,v 1.1 2010/05/16 20:24:00 yqhuang Exp $ */

/******************************************************************
 *
 *  $Source: /usr/local/CVS_Repository/ece354/autoeval/new_greybox/dummy/rtx_inc.h,v $
 *  $Author: yqhuang $
 *  $Date: 2010/05/16 20:24:00 $
 *
 *  Purpose:   Base include file for the RTX
 *
 *  $Log: rtx_inc.h,v $
 *  Revision 1.1  2010/05/16 20:24:00  yqhuang
 *  *** empty log message ***
 *
 *  Revision 1.1  2010/05/03 21:32:25  yqhuang
 *  *** empty log message ***
 *
 *
 *  [LOG REMOVED]
 *
 ******************************************************************/

#if !defined( RTX_BASE_H__ )
#define RTX_BASE_H__

/******************************************************************
 *                      CONSTANTS
 ******************************************************************/

/*
 * Data Types by Size
 */
#define SINT32  signed long int
#define UINT32  unsigned long int
#define SINT16  signed short int
#define UINT16  unsigned short int
#define SINT8   signed char
#define UINT8   unsigned char
#define CHAR    signed char
#define BYTE    unsigned char
#define VOID    void
#define BOOLEAN signed long int


#define ESC                 0x1B
#define BKSP                '\b'
#define CR                  '\r'
#define LF                  '\n'


#if !defined( TRUE )
#define TRUE 1
#endif

#if !defined( FALSE )
#define FALSE 0
#endif

#if !defined( NULL )
#define NULL 0
#endif


/*
 * Coldfire system defines
 */
#define RTX_COLDFIRE_MBAR    (BYTE *)(0xF0000000)
#define SIM_IMR              *( (UINT32 *)( RTX_COLDFIRE_MBAR + 0x44 ) )


/*
 * Coldfire Timer Defines
 */
#define TIMER0_TMR  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x140 ) )
#define TIMER0_TRR  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x144 ) )
#define TIMER0_TCN  *( (UINT16 *)( RTX_COLDFIRE_MBAR + 0x14C ) )
#define TIMER0_ICR  *( RTX_COLDFIRE_MBAR + 0x04d )
#define TIMER0_TER  *( RTX_COLDFIRE_MBAR + 0x151 )

#define TIMER1_TMR  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x180 ) )
#define TIMER1_TRR  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x184 ) )
#define TIMER1_TCN  *( (UINT16 *)(RTX_COLDFIRE_MBAR + 0x18C ) )
#define TIMER1_ICR  *( RTX_COLDFIRE_MBAR + 0x04e )
#define TIMER1_TER  *( RTX_COLDFIRE_MBAR + 0x191 )


/*
 * Coldfire Serial Defines
 */
#define SERIAL1_UCR     *( RTX_COLDFIRE_MBAR + 0x208 )
#define SERIAL1_UBG1    *( RTX_COLDFIRE_MBAR + 0x218 )
#define SERIAL1_UBG2    *( RTX_COLDFIRE_MBAR + 0x21C )
#define SERIAL1_UCSR    *( RTX_COLDFIRE_MBAR + 0x204 )
#define SERIAL1_USR     *( RTX_COLDFIRE_MBAR + 0x204 )
#define SERIAL1_UMR     *( RTX_COLDFIRE_MBAR + 0x200 )
#define SERIAL1_ICR     *( RTX_COLDFIRE_MBAR + 0x51  )
#define SERIAL1_IVR     *( RTX_COLDFIRE_MBAR + 0x230 )
#define SERIAL1_ISR     *( RTX_COLDFIRE_MBAR + 0x214 )
#define SERIAL1_IMR     *( RTX_COLDFIRE_MBAR + 0x214 )
#define SERIAL1_RD      *( RTX_COLDFIRE_MBAR + 0x20C )
#define SERIAL1_WD      *( RTX_COLDFIRE_MBAR + 0x20C )


/*
 * RTX Error Codes
 */
#define RTX_SUCCESS 0
#define RTX_ERROR   -1
#define RTX_FAILURE 1

/*
 * PSB state Enum
 */
#define STATE_READY 0
#define STATE_BLOCKED 1
#define STATE_RUNNING 2
#define STATE_NEW 3

/* 
 * Caller Identifier
*/
#define CALLER_SCHEDULER 0
#define CALLER_RELEASE_PROCESSOR 1
#define CALLER_SEND_MESSAGE 2
#define CALLER_RECEIVE_MESSAGE 3
#define CALLER_RELEASE_MEMORY_BLOCK 4
#define CALLER_REQUEST_MEMORY_BLOCK 5
#define CALLER_DELAYED_SEND 6
#define CALLER_GET_PRIORITY 7
#define CALLER_SET_PRIORITY 8

//define number of memory blocks
#define NUM_MEM_BLKS 32
UINT32 *free_blocks;

#define NUM_PROCESS 13

#define NULL_ID -2
#define KEYBOARD_INTERRUPT -3
#define KCD_ID -4
#define CRT_ID -5
#define WALL_CLOCK_ID -6
#define SET_PRIORITY_ID -7
#define TIMER_INTERRUPT -8

#define PROCESS_A 7
#define PROCESS_B 8
#define PROCESS_C 9

//define the message types
#define COMMAND_KEYBOARD 0
#define COMMAND_REGISTER 1
#define COMMAND_WALL_CLOCK 2
#define COMMAND_PRIORITY_MODIFIER 3
#define COMMAND_ERROR 4
#define COMMAND_ERASED 5
#define COMMAND_COUNT_REPORT 6
#define COMMAND_WAKEUP10 7

#endif
