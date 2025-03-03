#ifndef JOY_STATE_H
#define JOY_STATE_H

#include "list.h"

#include <stdbool.h>

// Forward Declarations

typedef struct joy_state Joy_State;

// Data Structures

struct joy_state {
	unsigned running : 1;
	Joy_List *ctrl;
	Joy_List *conts;
	Joy_List *stack;
};

// Functions

bool state_init(Joy_State *J);
void state_exit(Joy_State *J);
void state_debug(const Joy_State *J);
bool state_trigger_exit(Joy_State *J);
bool state_eval(Joy_State *J, Joy_List *ctrl);

#endif
