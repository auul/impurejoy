#include "state.h"
#include "data.h"

#include <stdio.h>

bool state_init(Joy_State *J)
{
	J->ctrl = NULL;
	J->conts = NULL;
	J->stack = NULL;

	return true;
}

void state_exit(Joy_State *J)
{
	list_unref(J->ctrl);
	list_unref(J->conts);
	list_unref(J->stack);
}

void state_debug(const Joy_State *J)
{
	printf("Ctrl: ");
	list_print_bare(J->ctrl);
	printf("\nConts: ");
	list_print_bare(J->conts);
	printf("\nStack: ");
	list_print_bare(J->stack);
	printf("\n");
}

bool state_trigger_exit(Joy_State *J)
{
	J->running = false;
	return true;
}

bool state_eval(Joy_State *J, Joy_List *ctrl)
{
	list_unref(J->ctrl);
	J->ctrl = ctrl;

	do {
		while (J->ctrl) {
			Joy_Data data = list_pop(&J->ctrl);
			switch (data_get_type(data)) {
			default:
				J->stack = list_push(J->stack, data);
				if (!J->stack) {
					return false;
				}
				break;
			}
		}

		while (J->conts && !J->ctrl) {
			J->ctrl = data_get_list(list_pop(&J->conts));
		}
	} while (J->ctrl);

	return true;
}
