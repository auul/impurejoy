#ifndef JOY_DATA_H
#define JOY_DATA_H

#include "bool.h"
#include "char.h"
#include "file.h"
#include "float.h"
#include "fn.h"
#include "global.h"
#include "int.h"
#include "list.h"
#include "set.h"
#include "str.h"
#include "word.h"

#include <limits.h>
#include <stdint.h>

// Forward Declarations

typedef enum joy_data_type Joy_DataType;

// Constants

enum joy_data_type {
	JOY_DATA_NIL,
	JOY_DATA_BOOL,
	JOY_DATA_CHAR,
	JOY_DATA_INT,
	JOY_DATA_FLOAT,
	JOY_DATA_SET,
	JOY_DATA_STR,
	JOY_DATA_FILE,
	JOY_DATA_WORD,
	JOY_DATA_FN,
	JOY_DATA_LIST,
	JOY_DATA_TYPE_COUNT,
};

static const Joy_Data JOY_DATA_MAX = UINTPTR_MAX;

// Functions

Joy_DataType data_get_type(const Joy_Data data);
Joy_Bool data_get_bool(const Joy_Data data);
Joy_Char data_get_char(const Joy_Data data);
Joy_Int data_get_int(const Joy_Data data);
Joy_Float data_get_float(const Joy_Data data);
Joy_Set data_get_set(const Joy_Data data);
Joy_Str *data_get_str(const Joy_Data data);
Joy_File *data_get_file(const Joy_Data data);
Joy_List *data_get_list(const Joy_Data data);
Joy_Word *data_get_word(const Joy_Data data);
Joy_Fn data_get_fn(const Joy_Data data);
void data_print(const Joy_Data data);
Joy_Data data_ref(Joy_Data data);
void data_unref(Joy_Data data);

#endif
