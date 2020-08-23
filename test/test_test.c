/* This weirdly named file contains the test benche for roadelou_test.c */

#include <roadelou_test.h>

/* For the final printf */
#include <stdio.h>

/* To debug heap corruption */
#include <mcheck.h>

int main(int argc, char const *argv[]) {
  mcheck_pedantic(NULL);

  /* Simple iterator */
  int i = 0;

  mcheck_check_all();

  /* Testing the test function on success */
  assert(check(test(0, TRUE)));
  /* Looking if testBench.success is correct */
  assert(testBench.success == 1);
  /* Looking if testBench.failure is correct */
  assert(testBench.failure == 0);
  /* Looking if testBench.failno is correct */
  assert(testBench.failno == NULL);

  mcheck_check_all();

  /* Testing the behavior on first failure */
  assert(check(test(i++, FALSE)));
  /* Looking if testBench.success is correct */
  assert(testBench.success == 1);
  /* Looking if testBench.failure is correct */
  assert(testBench.failure == 1);
  /* Looking if testBench.failno is correct */
  assert(testBench.failno[i - 1] == i - 1);

  mcheck_check_all();

  /* Testing the behavior on second failure */
  assert(check(test(i++, FALSE)));
  /* Looking if testBench.success is correct */
  assert(testBench.success == 1);
  /* Looking if testBench.failure is correct */
  assert(testBench.failure == 2);
  /* Looking if testBench.failno is correct */
  assert(testBench.failno[i - 1] == i - 1);

  mcheck_check_all();

  /* Testing the reallocation behavior */
  for (; i < 2 * TEST_CHUNK; ++i) {
    assert(check(test(i, FALSE)));
  }
  /* Looking if testBench.success is correct */
  assert(testBench.success == 1);
  /* Looking if testBench.failure is correct */
  assert(testBench.failure == 2 * TEST_CHUNK);
  /* Looking if testBench.failno is correct */
  assert(testBench.failno[i - 1] == i - 1);

  mcheck_check_all();

  /* Testing outputting the summary of a test. */
  assert(check(test_summary()));
  
  mcheck_check_all();

  end_test();

  mcheck_check_all();

  /* If we reach this line, then the execution was successfull */
  printf("---> %s <---\n", "Test of test succeeded");
  return 0;
}
