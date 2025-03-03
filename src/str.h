#ifndef JOY_STR_H
#define JOY_STR_H

// Forward Declarations

typedef char Joy_Str;

// Functions

void str_print(const Joy_Str *str);
Joy_Str *str_ref(Joy_Str *str);
void str_unref(Joy_Str *str);

#endif
