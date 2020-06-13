/* Implementation of the functions defined in roadelou_error.h */

#include <roadelou_error.h>
#include <stdarg.h>

/* Global instance of the Error struct */
Error error;
/* The size allocated for the msg pointer of error. */
static size_t len = 0;

/* A helper function to copy an error message to error. Note that this function does not use error to log its own errors (obviously), it just returns a Status enum */
static Status copy_message(const char *msg, va_list args);

int check(const Status code) {
  /* We explicitely return TRUE and FALSE so that the results of this function remain consistent if TRUE and FALSE are redifined (for some reason). */
  if (code == SUCCESS) {
    return TRUE;
  } else {
    return FALSE;
  }
}

int catch (const Status code) {
  switch (code) {
    case SUCCESS:
      return CATCH_OK;
    case WARNING:
      /* We return the value contained in the global instance of Error */
      return error.code;
    case FAILURE:
      /* Falling through to CATCH_FAIL, same behavior with invalid error code */
    default:
      return CATCH_FAIL;
  }
}

Status fail(const char *msg, ...) {
  /* First we gather our arguments */
  va_list args;
  va_start(args, msg);
  /* We copy the message string to error */
  copy_message(msg, args);
  /* We free args */
  va_end(args);
  /* Setting error.code */
  error.code = 0;
  /* Finally we return the expected value */
  return FAILURE;
}

Status warn(const unsigned short code, const char *msg, ...) {
  /* First we gather our arguments */
  va_list args;
  va_start(args, msg);
  /* We copy the message string to error */
  copy_message(msg, args);
  /* We free args */
  va_end(args);
  /* We copy the code to error.code */
  error.code = code;
  /* Finally we return the expected value */
  return WARNING;
}

Status ok(void) {
  /* Setting default message */
  copy_message(NULL, NULL);
  /* Setting error.code */
  error.code = 0;
  /* Finally we return the expected value */
  return SUCCESS;
}

Status copy_message(const char *msg, va_list args) {
  /* The size of the message to write */
  size_t size_msg;

  /* A pointer used to check if malloc encounters an error */
  void *buffer;

  /* A variable used to create a copy of args if we need it */
  va_list args2;

  /* First we check if msg is NULL */
  if (msg == NULL) {
    size_msg = 13 * sizeof(char);  /* strlen("(no message)") + 1 */

    /* Setting error.msg to a default message */
    if (len == 0) { /* i.e. error has not been initialized yet */
      buffer = malloc(size_msg);
      /* Failing if malloc does not work */
      if (buffer == NULL) {
        /* We haven't changed anything, so we can just return directly */
        return FAILURE;
      };
      error.msg = buffer;
      len = size_msg;
    }
    else if (len < size_msg) {
      /* Reallocating more memory for our message */
      /* I know that not checking the return of realloc is wrong, but I have nothing particular to do if it fails */
      buffer = realloc(error.msg, size_msg);
      /* Asserting that buffer is not NULL */
      if (buffer == NULL) {
        /* "If realloc fails, the original bloc is left untouched" (man page) */
        return FAILURE;
      }
      error.msg = buffer;
      len = size_msg;
    }
    /* else we already have enough space */

    /* We copy the error message */
    strncpy(error.msg, "(no message)", size_msg);
  }
  else {
    /* We have to create a copy of args in case we have to parse them a second time. We assume that the std we are linking against is C99 (even if our code style is C90). */
    va_copy(args2, args);

    /* We try one first time to copy our message, hoping that we have enough space to so in our error.msg buffer */
    /* We assume that the glibc version used is >= 2.1, which is important for the return value of vsnprintf */
    size_msg = vsnprintf(error.msg, len, msg, args);
    /* On error, a negative value is returned */
    if (size_msg <= 0) {
      return FAILURE;
    }

    /* Note : freeing args is not our reposibility, it should be done in the caller */
    /* va_end(args); */

    if (len == 0) {
      /* i.e. error.msg had not been allocated yet */
      buffer = malloc(size_msg + 1);  /* +1 for '\0' */
      if (buffer == NULL) {
        return FAILURE;
      }
      error.msg = buffer;
      len = size_msg + 1;

      /* We try to copy the message once more */
      size_msg = vsnprintf(error.msg, len, msg, args2);
      /* If the copy did not work on the second try, we give up */
      if (size_msg + 1 != len) { /* + 1 for the '\0' */
        return FAILURE;
      }
    }
    else if (size_msg > len) {
      /* i.e. the msg was truncated, we realloc the error.msg string */
      buffer = realloc(error.msg, size_msg + 1);
      if (buffer == NULL) {
        /* "If realloc fails, the original bloc is left untouched" (man page) */
        return FAILURE;
      }
      error.msg = buffer;
      len = size_msg + 1;

      /* We try to copy the message once more */
      size_msg = vsnprintf(error.msg, len, msg, args2);
      /* If the copy did not work on the second try, we give up */
      if (size_msg + 1 != len) {  /* + 1 for the '\0' */
        return FAILURE;
      }
    }

    /* We won't use args2 past this point, thus we can free it */
    va_end(args2);
  }

  /* If we reached this line, then the execution was a success */
  return SUCCESS;
}
