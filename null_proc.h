


typedef struct test_proc
{
  UINT8  pid;           /* pid of a test process */
  UINT8  priority;      /* initial priority of a test process */
  UINT32 sz_stack;      /* stack size of a test process */
  VOID   (*entry)();    /* entry point of a test process */
} test_proc_t; 