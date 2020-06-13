/* Interface to the error handling parts of the library. */

#ifndef ERROR_LIBRARY_INCLUDED
#define ERROR_LIBRARY_INCLUDED

/* For the definitions of TRUE, FALSE, NULL etc... */
#include <roadelou_base.h>

/* An enum that a function can return */
enum Status {
  SUCCESS = 0, /* The function executed successfully */
  WARNING = 1, /* The function call did not complete and you can do something
                  about it */
  FAILURE = 2 /* The function call did not complete and there is nothing you can
                 do about it */
};
/*
Notice the lack of a ',' after the last item in the enum for ANSI C
compatibility
*/

typedef enum Status Status;

/*
A struct that can be passed as a pointer to the calling function.

Certain fields of this struct are undefined depending on the value of the Status
returned by the Callee. In particular :
 - if Status is Status.SUCCESS, all fields are undefined;
 - if Status is Status.FAILURE, Error.code is undefined;
*/
struct Error {
  unsigned short code; /* A contextual error code to explain the internal
                          error. */
  char *msg;           /* A description of the error allocated in heap */
};

typedef struct Error Error;

/*
Because I don't write a lot of multithreaded code, I use an unsafe global instance of Error to make the function more concise.
*/
extern Error error;

/*
This function is used to check if an error occured after a call. It is meant to
be used within an if statement.

The return value of this function is :
 - TRUE (i.e. 1) if a Status.SUCCESS occured
 - FALSE else (even if code holds an invalid value)
*/
int check(const Status code);

/*
Some shortcuts defined to simplify writing the switch statements with the catch
function
*/
#ifndef CATCH_OK
#define CATCH_OK -1
#endif

#ifndef CATCH_FAIL
#define CATCH_FAIL -2
#endif

/*
This function is used to catch an error that can be solved (Status.WARNING). It
is intended to be used within a switch statement. Note that this function
returns an int while the Error.code is an unsigned short, which avoids overflow.

The return value of this function is :
 - CATCH_OK (-1) if a Status.SUCCESS occured,
 - CATCH_FAIL (-2) in case of a Status.FAILURE;
 - >= 0 && == error->code if Status.WARNING,

This function uses the global instance error in a way that is not thread safe.
*/
int catch (const Status code);

/*
This function is a shortcut to declare that the current function encountered an
error and to set the Error struct accordingly. It is meant to be used directly
with return.

It is safe to pass NULL as a message, in this case "(no message)" will be used
as the error message. fail supports printf style format and sets error.code to 0.

Its return value is always Status.FAILURE.

This function uses the global instance error in a way that is not thread safe.
*/
Status fail(const char *msg, ...);

/*
This function is a shortcut to declare that the current function encountered an
error that can be solved and to set the Error struct accordingly. It is meant to
be used directly with return.

It is safe to pass NULL as a message, in this case an empty string will be used
as the error message.

It is safe to pass NULL as a message, in this case "(no message)" will be used
as the error message. fail supports printf style format.

Its return value always is Status.WARNING.

This function uses the global instance error in a way that is not thread safe.
*/
Status warn(const unsigned short code, const char *msg, ...);

/*
This function is a shortcut to declare that the current function did run
successfully. It is meant to be used directly with return.

ok sets the message in error to the default "(no message)" and sets error.code = 0.

NOTE: Your compiler is very likely to inline this function.

Its return value is always Status.SUCCESS
*/
Status ok(void);

#endif /* end of include guard: ERROR_LIBRARY_INCLUDED */
