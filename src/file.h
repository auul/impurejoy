#ifndef JOY_FILE_H
#define JOY_FILE_H

#include <stdio.h>

// Forward Declarations

typedef struct joy_file Joy_File;

// Data Structures

struct joy_file {
	FILE *ptr;
};

// Functions

void file_print(const Joy_File *file);
Joy_File *file_ref(Joy_File *file);
void file_unref(Joy_File *file);

#endif
