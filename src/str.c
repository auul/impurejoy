#include "str.h"
#include "alloc.h"
#include "char.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

static bool is_span_end(const char c)
{
	return c == 0 || c == '"' || c == '\\' || (c != ' ' && isspace(c));
}

static int get_print_span(const Joy_Str *str)
{
	int span;
	for (span = 0; !is_span_end(str[span]); span++) {
		;
	}
	return span;
}

void str_print(const Joy_Str *str)
{
	while (*str) {
		int span = get_print_span(str);
		if (span) {
			printf("%.*s", span, str);
			str += span;
		} else {
			char_print_esc(*str);
			str++;
		}
	}
}

Joy_Str *str_ref(Joy_Str *str)
{
	return alloc_ref(str);
}

void str_unref(Joy_Str *str)
{
	alloc_unref_with_free(str);
}
