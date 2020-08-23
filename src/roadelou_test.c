/* Implementation of roadelou_test.h */

#include <roadelou_test.h>

/* Global instance of testBench */
TestBench testBench = {0, 0, NULL};

/* A helper function t print a line of given length filled with the character
 * style */
Status style_line(int length, char style);

Status test(const int testno, const int test_condition) {
  /* The pointer used to hold the results of malloc and realloc */
  void *buffer;

  /* First, we see if the test was a failure */
  if (test_condition == TRUE) {
    /* We just increment the success counter */
    testBench.success++;
    return SUCCESS;
  } else {
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
      buffer = realloc(testBench.failno,
                       (testBench.failure + TEST_CHUNK) * sizeof(int));
      if (buffer == NULL) {
        return FAILURE;
      }
      testBench.failno = buffer;
      /* Putting the test number in failno */
      testBench.failno[testBench.failure] = testno;
    } else {
      /* There is no need to realloc, we have enough space to put the testno */
      testBench.failno[testBench.failure] = testno;
    }
    /* Incrementing the number of failures */
    testBench.failure++;
    return SUCCESS;
  }
}

Status test_summary(void) {
  /* A cursor used for for loops. */
  int cursor;
  /* Used to keep track of the length of the line, in order to know when the
  line should be wrapped. */
  int wrap;

  /* One line for the style. */
  style_line(80, '#');

  /* We print some general statitics. */
  printf("Passed %d tests out of %d.\n", testBench.success,
         testBench.success + testBench.failure);

  /* If we have any failed test, we print their numbers. */
  switch (testBench.failure) {
  case 0:
    /* No failed tests. */
    break;
  case 1:
    /* A single failed test, there is a dedicated message in this case. */
    printf("The single failed test is %d.\n", testBench.failno[0]);
    break;
  default:
    /* More than one failed tests. */
    wrap = printf("The list of failed tests is %d", testBench.failno[0]);

    /* We print all the intermediate tests the same way, with a leading comma to
     * get a more friendly output. */
    for (cursor = 1; cursor < testBench.failure - 1; cursor++) {
      wrap += printf(", %d", testBench.failno[cursor]);
      if (wrap >= 74) {
        /* We want to wrap before the 80th character, and we assume that there
         * won't be more than 3 digits in the test number, hence we can start
         * wrapping at 74. */
        wrap = 0;
        putchar('\n');
      }
    }

    /* We print the last failed test number differently. We know that there will
     * be one because we have more than two failed tests. */
    printf(" and %d.\n", testBench.failno[cursor]);
    break;
  }

  /* Another line for the style. If printing this line succeeds, we assume that
   * all the other printf calls worked. */
  return style_line(80, '#');
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

Status style_line(int length, char style) {
  /* It is a bit cheap, but because the output is line-buffered in UNIX, we
   * simply call putchar over qnd over. */
  int i;

  for (i = 0; i < length; i++) {
    putchar(style);
  }
  /* We end the line with a newline character. We reuse i to get the return
   * value of putchar. */
  i = putchar('\n');

  /* We assume that if the last putchar worked, all the others also did. */
  if (i == EOF) {
    /* An error occured. */
    return fail(
        "A putchar called failed, maybe the standard output is unavailable ?");
  } else {
    return SUCCESS;
  }
}
