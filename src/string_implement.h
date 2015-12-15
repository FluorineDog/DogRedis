#ifndef DOG_STRING_IMPLEMENT_H_
#define DOG_STRING_IMPLEMENT_H_
#include <stdlib.h>
typedef struct String_t_ {
	char* str;
	int size;
	int capacity;
}String_t;
bool string_get_bit(String_t* s, int offset);
void string_set_bit(String_t* s, int offset, bool bit);
void string_init(String_t* s);
void string_append(String_t* s, char* app);
int index(String_t* s, int i);
void string_override(String_t* s, int offset, char* content);
void string_reset(String_t* s, char* new_str);
void string_destruct(String_t* s);

#endif // DOG_STRING_IMPLEMENT_H_