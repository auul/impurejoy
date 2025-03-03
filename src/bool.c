#include "bool.h"

#include <stdio.h>

void bool_print(const Joy_Bool bnum)
{
	if (bnum) {
		printf("true");
	} else {
		printf("false");
	}
}
