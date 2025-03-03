#include "alloc.h"
#include "error.h"

#include <stdlib.h>

static Joy_Alloc *get_tag(const void *ptr)
{
	return (Joy_Alloc *)ptr - 1;
}

static void init_tag(Joy_Alloc *tag)
{
	tag->ref = 1;
}

static bool is_tag_solo(const Joy_Alloc *tag)
{
	return tag->ref & 1;
}

static void inc_tag_ref(Joy_Alloc *tag)
{
	if (is_tag_solo(tag)) {
		atomic_init(&tag->ref, 2);
	} else {
		atomic_fetch_add(&tag->ref, 2);
	}
}

static bool dec_tag_ref(Joy_Alloc *tag)
{
	if (is_tag_solo(tag)) {
		return true;
	} else if (atomic_fetch_sub(&tag->ref, 2) <= 2) {
		init_tag(tag);
	}
	return false;
}

void *new_alloc(size_t size)
{
	Joy_Alloc *tag = malloc(sizeof(Joy_Alloc) + size);
	if (!tag) {
		return error_alloc();
	}
	init_tag(tag);

	return tag->ptr;
}

void *alloc_ref(void *ptr)
{
	if (ptr) {
		inc_tag_ref(get_tag(ptr));
	}
	return ptr;
}

bool alloc_unref(void *ptr)
{
	if (!ptr) {
		return false;
	}
	return dec_tag_ref(get_tag(ptr));
}

void alloc_free(void *ptr)
{
	free(get_tag(ptr));
}

void alloc_unref_with_free(void *ptr)
{
	if (!ptr) {
		return;
	}

	Joy_Alloc *tag = get_tag(ptr);
	if (dec_tag_ref(tag)) {
		free(tag);
	}
}

bool alloc_is_editable(const void *ptr)
{
	return is_tag_solo(get_tag(ptr));
}
