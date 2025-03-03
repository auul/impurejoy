#ifndef JOY_ALLOC_H
#define JOY_ALLOC_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>

// Forward Declarations

typedef struct joy_alloc Joy_Alloc;

// Data Types

struct joy_alloc {
	atomic_uintptr_t ref;
	char ptr[];
};

// Functions

void *new_alloc(size_t size);
void *alloc_ref(void *ptr);
bool alloc_unref(void *ptr);
void alloc_free(void *ptr);
void alloc_unref_with_free(void *ptr);
bool alloc_is_editable(const void *ptr);

#endif
