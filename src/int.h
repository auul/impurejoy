#ifndef JOY_INT_H
#define JOY_INT_H

#include <stdint.h>

// Forward Declarations

typedef intptr_t Joy_Int;

// Constants

static const char JOY_INT_FMT_STRING[] = "%li";

// Functions

void int_print(const Joy_Int inum);

#endif
