/* A simple standalone header that defines a lot of constants that will be used
 * throughout the roadelou library */

#ifndef BASE_LIBRARY_INCLUDED
#define BASE_LIBRARY_INCLUDED

/* Used for malloc */
#include <stdlib.h>

/* Used for format string support */
#include <stdio.h>

/* Used to support variadic functions */
#include <stdarg.h>

/* Used for strncpy */
#include <string.h>

/* Used for assert */
#include <assert.h>

/* For the sake of readability, we also define TRUE and FALSE to be 1 and 0
 * respectively. */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif /* end of include guard: BASE_LIBRARY_INCLUDED */
