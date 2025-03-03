#include "word.h"
#include "alloc.h"

#include <stdio.h>

void word_print(const Joy_Word *word)
{
	printf("%s", word);
}

Joy_Word *word_ref(Joy_Word *word)
{
	return alloc_ref(word);
}

void word_unref(Joy_Word *word)
{
	alloc_unref_with_free(word);
}
