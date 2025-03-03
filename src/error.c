#include "error.h"
#include <stddef.h>

// Globals

static char *error_msg;

// Constants

static const char ALLOC_MSG[] = "Error allocating memory";

// Functions

void *error_alloc(void)
{
	error_msg = (char *)ALLOC_MSG;
	return NULL;
}
