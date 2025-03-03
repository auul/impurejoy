#ifndef JOY_WORD_H
#define JOY_WORD_H

// Forward Declarations

typedef char Joy_Word;

// Functions

void word_print(const Joy_Word *word);
Joy_Word *word_ref(Joy_Word *word);
void word_unref(Joy_Word *word);

#endif
