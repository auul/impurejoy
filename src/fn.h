#ifndef JOY_FN_H
#define JOY_FN_H

#include <stdint.h>

// Forward Declarations

typedef uintptr_t Joy_Fn;

// Functions

void fn_print(const Joy_Fn fn);
Joy_Fn fn_ref(Joy_Fn fn);
void fn_unref(Joy_Fn fn);

#endif
