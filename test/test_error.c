/* This test functions are meant to verify the functionnality of roadelou_error,
 * but also to showcase how the functions in roadelou_error.h are meant to be
 * used. */

/* The library we are testing */
#include <roadelou_error.h>

/* Used to write simple asserts */
#include <assert.h>

/* Used for strcmp */
#include <string.h>

/* For the final printf */
#include <stdio.h>

/* Three functions to perform the tests with */
static Status failing_silent_function(void);
static Status failing_verbose_function(void);
static Status warning_function(void);
static Status succeeding_function(void);

int main(int argc, char const *argv[]) {
  /* The variable used to perform the tests */
  Status test;

  /* Unlike in this test bench, the error struct is not supoosed to be used
   * directly */
  test = failing_silent_function();
  /* Testing the check function */
  assert(check(test) == FALSE);
  /* Testing the catch function */
  assert(catch (test) == CATCH_FAIL);
  /* Testing if the error.code was set correctly */
  assert(error.code == 0);
  /* Testing if copy_message works correctly */
  assert(strcmp(error.msg, "(no message)") == 0);

  test = failing_verbose_function();
  /* Testing the check function */
  assert(check(test) == FALSE);
  /* Testing the catch function */
  assert(catch (test) == CATCH_FAIL);
  /* Testing if the error.code was set correctly */
  assert(error.code == 0);
  /* Testing if copy_message works correctly */
  assert(strcmp(error.msg, "The test function failed successfully :-)\n") == 0);

  test = warning_function();
  /* Testing the check function */
  assert(check(test) == FALSE);
  /* Testing the catch function */
  assert(catch (test) == 45);
  /* Testing if copy_message works correctly */
  assert(strcmp(error.msg, "The warning code is: 45\n") == 0);

  test = succeeding_function();
  /* Testing the check function */
  assert(check(test) == TRUE);
  /* Testing the catch function */
  assert(catch (test) == CATCH_OK);
  /* Testing if the error.code was set correctly */
  assert(error.code == 0);
  /* Testing if copy_message works correctly */
  assert(strcmp(error.msg, "(no message)") == 0);

  /* Freing memory */
  end_error();
  
  printf("---> %s <---\n", "Test of error succeeded");
  return 0;
}

Status failing_silent_function(void) {
  /* Failing without an error message */
  return fail(NULL);
}

Status failing_verbose_function(void) {
  /* Failing with an error message */
  return fail("%s\n", "The test function failed successfully :-)");
}

Status warning_function(void) {
  return warn(45, "%s %d\n", "The warning code is:", 45);
}

Status succeeding_function(void) { return ok(); }
