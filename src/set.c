#include "set.h"

#include <stdbool.h>
#include <stdio.h>

static bool is_set_max_at_least(const Joy_Set set, const unsigned member)
{
	return set >> member;
}

static bool is_set_member(const Joy_Set set, const unsigned member)
{
	return (set >> member) & 1;
}

void set_print(const Joy_Set set)
{
	printf("{");

	bool first = true;

	for (unsigned member = 0; is_set_max_at_least(set, member); member++) {
		if (is_set_member(set, member)) {
			if (first) {
				first = false;
			} else {
				printf(" ");
			}
			printf("%u", member);
		}
	}
	printf("}");
}
