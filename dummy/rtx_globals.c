#include "rtx_test.h"

test_fixture_t __attribute__ ((section("__RTX_TEST_DATA__"))) g_test_fixture;
test_proc_t __attribute__ ((section("__RTX_TEST_DATA__"))) g_test_proc[NUM_TEST_PROCS];