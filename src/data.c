#include "data.h"

#include <stdio.h>

// Constants

static const unsigned TYPE_BITS =
	JOY_DATA_TYPE_COUNT < 64
		? (JOY_DATA_TYPE_COUNT < 8
               ? (JOY_DATA_TYPE_COUNT >= 4 ? 3
                                           : (JOY_DATA_TYPE_COUNT >= 2 ? 2 : 1))
               : (JOY_DATA_TYPE_COUNT >= 32
                      ? 6
                      : (JOY_DATA_TYPE_COUNT >= 16 ? 5 : 4)))
		: (JOY_DATA_TYPE_COUNT < 512
               ? (JOY_DATA_TYPE_COUNT >= 256
                      ? 9
                      : (JOY_DATA_TYPE_COUNT >= 128 ? 8 : 7))
               : (JOY_DATA_TYPE_COUNT >= 1024 ? 11 : 10));
static const unsigned DATA_BITS = sizeof(Joy_Data) * 8;
static const unsigned VALUE_BITS = DATA_BITS - TYPE_BITS;
static const Joy_Data VALUE_MASK = JOY_DATA_MAX >> TYPE_BITS;

// Functions

Joy_DataType data_get_type(const Joy_Data data)
{
	return (Joy_DataType)(data >> VALUE_BITS);
}

static Joy_Data get_value(const Joy_Data data)
{
	return data & VALUE_MASK;
}

Joy_Bool data_get_bool(const Joy_Data data)
{
	return (Joy_Bool)get_value(data);
}

Joy_Char data_get_char(const Joy_Data data)
{
	return (Joy_Char)get_value(data);
}

Joy_Int data_get_int(const Joy_Data data)
{
	return (Joy_Int)get_value(data);
}

Joy_Float data_get_float(const Joy_Data data)
{
	const Joy_Data data_shifted = data << TYPE_BITS;
	return *((Joy_Float *)&data_shifted);
}

Joy_Set data_get_set(const Joy_Data data)
{
	return (Joy_Set)get_value(data);
}

Joy_Str *data_get_str(const Joy_Data data)
{
	return (Joy_Str *)get_value(data);
}

Joy_File *data_get_file(const Joy_Data data)
{
	return (Joy_File *)get_value(data);
}

Joy_List *data_get_list(const Joy_Data data)
{
	return (Joy_List *)get_value(data);
}

Joy_Word *data_get_word(const Joy_Data data)
{
	return (Joy_Word *)get_value(data);
}

Joy_Fn data_get_fn(const Joy_Data data)
{
	return (Joy_Fn)get_value(data);
}

void data_print(const Joy_Data data)
{
	switch (data_get_type(data)) {
	case JOY_DATA_NIL:
		printf("nil");
		break;
	case JOY_DATA_BOOL:
		bool_print(data_get_bool(data));
		break;
	case JOY_DATA_CHAR:
		char_print(data_get_char(data));
		break;
	case JOY_DATA_INT:
		int_print(data_get_int(data));
		break;
	case JOY_DATA_FLOAT:
		float_print(data_get_float(data));
		break;
	case JOY_DATA_SET:
		set_print(data_get_set(data));
		break;
	case JOY_DATA_STR:
		str_print(data_get_str(data));
		break;
	case JOY_DATA_FILE:
		file_print(data_get_file(data));
		break;
	case JOY_DATA_WORD:
		word_print(data_get_word(data));
		break;
	case JOY_DATA_FN:
		fn_print(data_get_fn(data));
		break;
	case JOY_DATA_LIST:
		list_print(data_get_list(data));
		break;
	}
}

Joy_Data data_ref(Joy_Data data)
{
	switch (data_get_type(data)) {
	case JOY_DATA_STR:
		str_ref(data_get_str(data));
		break;
	case JOY_DATA_FILE:
		file_ref(data_get_file(data));
		break;
	case JOY_DATA_WORD:
		word_ref(data_get_word(data));
		break;
	case JOY_DATA_FN:
		fn_ref(data_get_fn(data));
		break;
	case JOY_DATA_LIST:
		list_ref(data_get_list(data));
		break;
	default:
		break;
	}
	return data;
}

void data_unref(Joy_Data data)
{
	switch (data_get_type(data)) {
	case JOY_DATA_STR:
		str_unref(data_get_str(data));
		break;
	case JOY_DATA_FILE:
		file_unref(data_get_file(data));
		break;
	case JOY_DATA_WORD:
		word_unref(data_get_word(data));
		break;
	case JOY_DATA_FN:
		fn_unref(data_get_fn(data));
		break;
	case JOY_DATA_LIST:
		list_unref(data_get_list(data));
		break;
	default:
		break;
	}
}
