#ifndef JOY_LIST_H
#define JOY_LIST_H

#include "global.h"

// Forward Declarations

typedef struct joy_list Joy_List;

// Data Structures

struct joy_list {
	Joy_Data car;
	Joy_List *cdr;
};

// Functions

void list_print(const Joy_List *list);
void list_print_bare(const Joy_List *list);
Joy_List *list_ref(Joy_List *list);
void list_unref(Joy_List *list);
Joy_List *list_push(Joy_List *list, Joy_Data data);
Joy_Data list_pop(Joy_List **list_p);

#endif
