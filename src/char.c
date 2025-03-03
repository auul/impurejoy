#include "char.h"

#include <ctype.h>
#include <stdio.h>

// Constants

static const uint8_t UTF8_XBYTE = 128;
static const uint8_t UTF8_2BYTE = 192;
static const uint8_t UTF8_3BYTE = 224;
static const uint8_t UTF8_4BYTE = 240;

static const Joy_Char UTF8_XMASK = 192;
static const Joy_Char UTF8_2MASK = 224;
static const Joy_Char UTF8_3MASK = 240;
static const Joy_Char UTF8_4MASK = 248;

static const Joy_Char UTF8_1MAX = 127;
static const Joy_Char UTF8_2MAX = 2047;
static const Joy_Char UTF8_3MAX = 65535;

static const unsigned UTF8_SHIFT = 6;

// Functions

void char_print(const Joy_Char cnum)
{
	printf("'");
	if (cnum == '\'') {
		printf("\\'");
	} else {
		char_print_esc(cnum);
	}
	printf("'");
}

static void print_as_ascii(const Joy_Char cnum)
{
	printf("%c", cnum);
}

static void print_as_byte_esc(const Joy_Char cnum)
{
	printf("\\%#2x", cnum);
}

static void print_as_utf8(const Joy_Char cnum)
{
	if (cnum > UTF8_3MAX) {
		printf("%c%c%c%c",
		       (uint8_t)(cnum >> (3 * UTF8_SHIFT)) | UTF8_4BYTE,
		       (uint8_t)((cnum >> (2 * UTF8_SHIFT)) & ~UTF8_XMASK) | UTF8_XBYTE,
		       (uint8_t)((cnum >> UTF8_SHIFT) & ~UTF8_XMASK) | UTF8_XBYTE,
		       (uint8_t)(cnum & ~UTF8_XMASK) | UTF8_XBYTE);
	} else if (cnum > UTF8_2MAX) {
		printf("%c%c%c",
		       (uint8_t)(cnum >> (2 * UTF8_SHIFT)) | UTF8_3BYTE,
		       (uint8_t)((cnum >> UTF8_SHIFT) & ~UTF8_XMASK) | UTF8_XBYTE,
		       (uint8_t)(cnum & ~UTF8_XMASK) | UTF8_XBYTE);
	} else if (cnum > UTF8_1MAX) {
		printf("%c%c",
		       (uint8_t)(cnum >> UTF8_SHIFT) | UTF8_2BYTE,
		       (uint8_t)(cnum & ~UTF8_XMASK) | UTF8_XBYTE);
	} else {
		print_as_ascii(cnum);
	}
}

void char_print_esc(const Joy_Char cnum)
{
	switch (cnum) {
	case '\a':
		printf("\\a");
		break;
	case '\b':
		printf("\\b");
		break;
	case '\e':
		printf("\\e");
		break;
	case '\f':
		printf("\\f");
		break;
	case '\n':
		printf("\\n");
		break;
	case '\r':
		printf("\\r");
		break;
	case '\t':
		printf("\\t");
		break;
	case '\v':
		printf("\\v");
		break;
	case '\\':
		printf("\\\\");
		break;
	default:
		if (cnum > UTF8_1MAX) {
			print_as_utf8(cnum);
		} else if (isgraph(cnum)) {
			print_as_ascii(cnum);
		} else {
			print_as_byte_esc(cnum);
		}
		break;
	}
}
