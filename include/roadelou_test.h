/* This part of the library is meant to simplify testing */

#ifndef TEST_LIBRARY_INCLUDED
#define TEST_LIBRARY_INCLUDED

#include <roadelou_error.h>

/* The number of int to allocated at once for TestBench.failno */
#ifndef TEST_CHUNK
#define TEST_CHUNK 5
#endif

/* The TestBench struct used to store the statistics of a test bench */
struct TestBench {
  int success; /* A count of all the tests that have succeeded */
  int failure; /* A count of all the tests that have failed */
  int *failno; /* The list of all the identifiers of the failed tests */
};

typedef struct TestBench TestBench;

/* Just like with Error, we use a global instance to make the functions more
 * convenient, but it also means that they are not safe to use in a
 * multithreaded environment */
extern TestBench testBench;

/*
This function is the one used to perform some test bench. It relies on the
global instance of TestBench and is therefore not safe to use in a multithreaded
environment.

The first argument to this function is the the test number, which is a unique
identifier to this test (this way you can know which tests failed).
test_condition is the boolean that you are testing: the test is considered
successfull if test_condition is TRUE (==1) and it is a failure if
test_condition is set to any other value, FALSE (==0) in particular.

This function returns a status indicating whether it managed to do its job or
not. It will return either SUCCESS or FAILURE.
*/
Status test(const int testno, const int test_condition);

/*
This function should be called at the end of the test bench. It will print a
nice summary of the results of the tests.

This is a read-only method, it won't change the state of the test bench.
*/
Status test_summary(void);

/*
Small helper function to free any memory that was allocated to the test bench.
*/
Status end_test(void);

#endif /* end of include guard: TEST_LIBRARY_INCLUDED */
