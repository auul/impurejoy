#include "file.h"
#include "alloc.h"

void file_print(const Joy_File *file)
{
	if (file->ptr == stdin) {
		printf("stdin");
	} else if (file->ptr == stdout) {
		printf("stdout");
	} else if (file->ptr == stderr) {
		printf("stderr");
	} else {
		printf("file:%u", fileno(file->ptr));
	}
}

Joy_File *file_ref(Joy_File *file)
{
	alloc_ref(file);
}

static void close_stream(FILE *f)
{
	if (f == stdin || f == stdout || f == stderr) {
		return;
	}
	fclose(f);
}

void file_unref(Joy_File *file)
{
	if (!alloc_unref(file)) {
		return;
	}
	close_stream(file->ptr);
	alloc_free(file);
}
