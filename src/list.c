#include "list.h"
#include "alloc.h"
#include "data.h"

#include <stdio.h>

static Joy_List *new_node(Joy_Data car, Joy_List *cdr)
{
	Joy_List *node = new_alloc(sizeof(Joy_List));
	if (!node) {
		data_unref(car);
		list_unref(cdr);
		return NULL;
	}

	node->car = car;
	node->cdr = cdr;

	return node;
}

void list_print(const Joy_List *list)
{
	printf("[");
	list_print_bare(list);
	printf("]");
}

void list_print_bare(const Joy_List *list)
{
	while (list) {
		data_print(list->car);
		list = list->cdr;
		if (list) {
			printf(" ");
		}
	}
}

Joy_List *list_ref(Joy_List *list)
{
	return alloc_ref(list);
}

void list_unref(Joy_List *list)
{
	while (alloc_unref(list)) {
		Joy_Data car = list->car;
		Joy_List *cdr = list->cdr;

		alloc_free(list);
		data_unref(car);

		list = cdr;
	}
}

Joy_List *list_push(Joy_List *list, Joy_Data data)
{
	return new_node(data, list);
}

static void split_list(Joy_Data *car_dest, Joy_List **cdr_dest, Joy_List *list)
{
	if (alloc_is_editable(list)) {
		*car_dest = list->car;
		*cdr_dest = list->cdr;
		alloc_free(list);
	} else {
		*car_dest = data_ref(list->car);
		*cdr_dest = list_ref(list->cdr);
		list_unref(list);
	}
}

Joy_Data list_pop(Joy_List **list_p)
{
	Joy_Data car;
	split_list(&car, list_p, *list_p);

	return car;
}
