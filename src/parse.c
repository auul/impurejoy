#include "parse.h"
#include "alloc.h"

#include <ctype.h>
#include <stdio.h>

// Constants

static const char *delim_sequence_list[] = {
	"==",
	"(*",
	"*)",
	"#",
	"{",
	"}",
	"[",
	"]",
	NULL,
};

// Functions

static Joy_ParseToken tag_empty(const char *src, size_t len)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_EMPTY,
		.ptr = NULL,
	};
	return token;
}

static Joy_ParseToken tag_int(const char *src, size_t len, Joy_Int inum)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_INT,
		.inum = inum,
	};
	return token;
}

static Joy_ParseToken tag_float(const char *src, size_t len, Joy_Float fnum)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_FLOAT,
		.fnum = fnum,
	};
	return token;
}

static Joy_ParseToken tag_set(const char *src, size_t len, Joy_Set set)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_SET,
		.set = set,
	};
	return token;
}

static Joy_ParseToken tag_str(const char *src, size_t len, Joy_Str *str)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_STR,
		.str = str,
	};
	return token;
}

static Joy_ParseToken tag_word(const char *src, size_t len, Joy_Word *word)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_WORD,
		.word = word,
	};
	return token;
}

static Joy_ParseToken tag_list(const char *src, size_t len, Joy_ParseList *list)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = JOY_PARSE_LIST,
		.list = list,
	};
	return token;
}

static Joy_ParseToken tag_error(const char *src, size_t len, Joy_ParseType type)
{
	Joy_ParseToken token = {
		.src = src,
		.len = len,
		.type = type,
		.ptr = NULL,
	};
	return token;
}

static void print_parse_list(const Joy_ParseList *list)
{
	printf("[");
	while (list) {
		parse_token_print(list->car);
		list = list->cdr;
		if (list) {
			printf(" ");
		}
	}
	printf("]");
}

void parse_token_print(const Joy_ParseToken token)
{
	switch (token.type) {
	case JOY_PARSE_EMPTY:
		printf("empty(%zu)", token.len);
		break;
	case JOY_PARSE_INT:
		int_print(token.inum);
		break;
	case JOY_PARSE_FLOAT:
		float_print(token.fnum);
		break;
	case JOY_PARSE_SET:
		set_print(token.set);
		break;
	case JOY_PARSE_STR:
		str_print(token.str);
		break;
	case JOY_PARSE_WORD:
		word_print(token.word);
		break;
	case JOY_PARSE_LIST:
		print_parse_list(token.list);
		break;
	}
}

Joy_ParseToken parse_token_ref(Joy_ParseToken token)
{
	switch (token.type) {
	case JOY_PARSE_STR:
		str_ref(token.str);
		break;
	case JOY_PARSE_WORD:
		word_ref(token.word);
		break;
	case JOY_PARSE_LIST:
		alloc_ref(token.list);
		break;
	default:
		break;
	}
	return token;
}

static void unref_parse_list(Joy_ParseList *list)
{
	while (alloc_unref(list)) {
		Joy_ParseToken car = list->car;
		Joy_ParseList *cdr = list->cdr;

		alloc_free(list);
		parse_token_unref(car);

		list = cdr;
	}
}

void parse_token_unref(Joy_ParseToken token)
{
	switch (token.type) {
	case JOY_PARSE_STR:
		str_unref(token.str);
		break;
	case JOY_PARSE_WORD:
		word_unref(token.word);
		break;
	case JOY_PARSE_LIST:
		unref_parse_list(token.list);
		break;
	default:
		break;
	}
}

static bool is_token_type_error(const Joy_ParseType type)
{
	return type >= JOY_PARSE_ERROR;
}

static bool is_partial_match(const char *target, const char *src)
{
	for (unsigned i = 0; target[i]; i++) {
		if (target[i] != src[i]) {
			return false;
		}
	}
	return true;
}

static const char *get_delim_sequence(const char *src)
{
	for (unsigned i = 0; delim_sequence_list[i]; i++) {
		if (is_partial_match(delim_sequence_list[i], src)) {
			return delim_sequence_list[i];
		}
	}
	return NULL;
}

static bool is_delim(const char *src)
{
	return *src == '0' || isspace(*src) || get_delim_sequence(src);
}

static bool is_line_comment(const char *src)
{
	return src[0] == '#';
}

Joy_ParseToken parse_line_comment(const char *src)
{
	size_t len;
	for (len = 1; src[len] != 0 && src[len] != '\n'; len++) {
		;
	}
	return tag_empty(src, len);
}

static bool is_comment_open(const char *src)
{
	return src[0] == '(' && src[1] == '*';
}

static bool is_comment_close(const char *src)
{
	return src[0] == '*' && src[1] == ')';
}

Joy_ParseToken parse_nested_comment(const char *src)
{
	size_t len;
	unsigned nest = 1;

	for (len = 1; nest; len++) {
		if (!src[len]) {
			return tag_error(src, len, JOY_PARSE_ERROR_UNCLOSED_COMMENT);
		} else if (is_comment_open(src + len)) {
			nest++;
		} else if (is_comment_close(src + len)) {
			nest--;
		}
		len++;
	}
	len++;

	return tag_empty(src, len);
}

static bool is_empty(const char *src)
{
	return isspace(src[0]) || is_line_comment(src) || is_comment_open(src);
}

Joy_ParseToken parse_empty(const char *src)
{
	size_t len;
	for (len = 0; is_empty(src + len);) {
		if (is_line_comment(src + len)) {
			Joy_ParseToken empty = parse_line_comment(src + len);
			len += empty.len;
		} else if (is_comment_open(src + len)) {
			Joy_ParseToken empty = parse_nested_comment(src + len);
			if (is_token_type_error(empty.type)) {
				return empty;
			}
			len += empty.len;
		} else {
			len++;
		}
	}
	return tag_empty(src, len);
}

static bool is_tribit_literal(const char c)
{
	return c >= '0' && c <= '7';
}

static unsigned get_tribit_literal(const char c)
{
	return c - '0';
}

static Joy_Int push_tribit(Joy_Int inum, const char c)
{
	return (inum << 3) | get_tribit_literal(c);
}

static bool is_octal_literal(const char *src)
{
	if (*src == '-') {
		src++;
	}

	if (*src != '0') {
		return false;
	}
	src++;

	while (!is_delim(src)) {
		if (!is_tribit_literal(*src)) {
			return false;
		}
		src++;
	}
	return true;
}

Joy_ParseToken parse_octal_literal(const char *src)
{
	Joy_Int inum = 0;
	bool negative = false;
	size_t len = 0;

	if (src[len] == '-') {
		negative = true;
		len++;
	}

	while (!is_delim(src + len)) {
		inum = push_tribit(inum, src[len]);
	}

	if (negative) {
		return tag_int(src, len, -inum);
	}
	return tag_int(src, len, inum);
}

static bool is_nibble_literal(const char c)
{
	return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

static unsigned get_nibble_literal(const char c)
{
	if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	return c - '0';
}

static Joy_Int push_nibble(Joy_Int inum, const char c)
{
	return (inum << 4) | get_nibble_literal(c);
}

static bool is_hex_literal(const char *src)
{
	return src[0] == 'x' || (src[0] == '-' && src[1] == 'x');
}

Joy_ParseToken parse_hex_literal(const char *src)
{
	Joy_Int inum = 0;
	bool negative = false;
	size_t len = 2;

	if (src[0] == '-') {
		negative = true;
		len++;
	}

	while (!is_delim(src + len)) {
		if (is_nibble_literal(src[len])) {
			inum = push_nibble(inum, src[len]);
		} else {
			return tag_error(
				src, len + 1, JOY_PARSE_ERROR_BAD_CHAR_IN_HEX_LITERAL);
		}
		len++;
	}

	if (negative) {
		return tag_int(src, len, -inum);
	}
	return tag_int(src, len, inum);
}

static Joy_Int push_decimal(Joy_Int inum, const char c)
{
	return (10 * inum) + (Joy_Int)(c - '0');
}

static Joy_Float push_decimal_to_float(Joy_Float fnum, const char c)
{
	return (10.0 * fnum) + (Joy_Float)(c - '0');
}
