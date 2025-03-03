#ifndef JOY_PARSE_H
#define JOY_PARSE_H

#include "float.h"
#include "int.h"
#include "set.h"
#include "str.h"
#include "word.h"

#include <stddef.h>

// Forward Declarations

typedef enum joy_parse_type Joy_ParseType;
typedef struct joy_parse_token Joy_ParseToken;
typedef struct joy_parse_list Joy_ParseList;

// Data Types

enum joy_parse_type {
	JOY_PARSE_EMPTY,
	JOY_PARSE_INT,
	JOY_PARSE_FLOAT,
	JOY_PARSE_SET,
	JOY_PARSE_STR,
	JOY_PARSE_WORD,
	JOY_PARSE_LIST,

	JOY_PARSE_ERROR,
	JOY_PARSE_ERROR_UNOPENED_COMMENT,
	JOY_PARSE_ERROR_UNCLOSED_COMMENT,
	JOY_PARSE_ERROR_BAD_CHAR_IN_HEX_LITERAL,
};

struct joy_parse_token {
	const char *src;
	size_t len;
	Joy_ParseType type;
	union {
		void *ptr;
		Joy_Int inum;
		Joy_Float fnum;
		Joy_Set set;
		Joy_Str *str;
		Joy_ParseList *list;
		Joy_Word *word;
	};
};

struct joy_parse_list {
	Joy_ParseToken car;
	Joy_ParseList *cdr;
};

// Functions

void parse_token_print(const Joy_ParseToken token);
Joy_ParseToken parse_token_ref(Joy_ParseToken token);
void parse_token_unref(Joy_ParseToken token);
Joy_ParseToken parse_line_comment(const char *src);
Joy_ParseToken parse_nested_comment(const char *src);

#endif
