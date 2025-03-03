#include "fn.h"
#include "alloc.h"
#include "list.h"

#include <stdbool.h>
#include <stdio.h>

static bool is_built_in(const Joy_Fn fn)
{
	return fn & 1;
}

static void *get_ptr(const Joy_Fn fn)
{
	return (void *)(fn >> 1);
}

void fn_print(const Joy_Fn fn)
{
	if (is_built_in(fn)) {
		printf("built-in:%p", get_ptr(fn));
		return;
	}
	list_print_bare(get_ptr(fn));
}

Joy_Fn fn_ref(Joy_Fn fn)
{
	if (!is_built_in(fn)) {
		alloc_ref(get_ptr(fn));
	}
	return fn;
}

void fn_unref(Joy_Fn fn)
{
	if (is_built_in(fn)) {
		return;
	}
	list_unref(get_ptr(fn));
}
