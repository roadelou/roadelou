/* Implementation of roadelou_test.h */

#include <roadelou_test.h>

/* Global instance of testBench */
TestBench testBench = {0, 0, NULL};

Status test(const int testno, const int test_condition) {
  /* The pointer used to hold the results of malloc and realloc */
  void *buffer;

  /* First, we see if the test was a failure */
  if (test_condition == TRUE) {
    /* We just increment the success counter */
    testBench.success++;
    return SUCCESS;
  }
  else {
    /* First, we check if failno has been allocated */
    if (testBench.failure == 0) {
      buffer = malloc(TEST_CHUNK * sizeof(int));
      if (buffer == NULL) {
        /* We couldn't alloc, so we can't go any further */
        return FAILURE;
      }
      testBench.failno = buffer;
      /* Setting the test number as the first value in failno */
      testBench.failno[testBench.failure] = testno;
    }
    /* We have to realloc once the units reach a TEST_CHUNK */
    else if (testBench.failure % TEST_CHUNK == 0) {
      /* We allocated one more TEST_CHUNK */
      buffer = realloc(testBench.failno, (testBench.failure + TEST_CHUNK) * sizeof(int));
      if (buffer == NULL) {
        return FAILURE;
      }
      testBench.failno = buffer;
      /* Putting the test number in failno */
      testBench.failno[testBench.failure] = testno;
    }
    else {
      /* There is no need to realloc, we have enough space to put the testno */
      testBench.failno[testBench.failure] = testno;
    }
    /* Incrementing the number of failures */
    testBench.failure++;
    return SUCCESS;
  }
}

Status end_test(void) {
  /* We check if some memory has been allocated to testBench */
  if (testBench.failure > 0 && testBench.failno != NULL) {
    /* If so, we free the allocated memory */
    free(testBench.failno);
  }
  testBench.failure = 0;
  testBench.success = 0;
  testBench.failno = NULL;
  return SUCCESS;
}
