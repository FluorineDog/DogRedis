#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include "string_implement.h"
#define max(a,b) ((a>b)?a:b)
void expand(String_t* s) {
	if (s->size >= s->capacity)
		s->capacity = 4 * s->size;
	s->str = (char*)realloc(s->str, s->capacity);
}
void string_init(String_t* s) {
	s->size = 0;
	s->capacity = 16;
	s->str = (char*)malloc(s->capacity);
	memset(s->str, 0, s->capacity);
}
bool string_get_bit(String_t* s, int offset) {
	int n = offset / 8;
	if (n >= s->size) {
		return false;
	}
	else return !!(s->str[n] & (1 << (offset % 8)));
}
void string_set_bit(String_t* s, int offset, bool bit) {
	int n = offset / 8;
	int old_size = s->size;
	if (old_size <= n) {
		s->size = n + 1;
		expand(s);
		memset(s->str + old_size, 0, s->size - old_size);
	}
	if (bit) {
		s->str[n] |= (1 << (offset % 8));
	}
	else {
		s->str[n] &= ~(1 << (offset % 8));
	}
}
int index(String_t* s, int i) {
	if (i < 0)
		i += s->size;
	if (i < 0)
		return 0;
	else if (i > s->size)
		return s->size;
	else
		return i;
}
void string_append(String_t* s, char* app) {
	int n = strlen(app);
	s->size += n;
	expand(s);
	strcpy(s->str + s->size - n, app);
	
}

void string_override(String_t* s, int offset, char* content) {
	int n = strlen(content);
	s->size = max(s->size, offset + n);
	expand(s);
	strcpy(s->str + offset, content);
}

void string_reset(String_t* s, char* new_str) {
	int n = strlen(new_str);
	s->size = n;
	expand(s);
	strcpy(s->str, new_str);
}
void string_destruct(String_t* s) {
	free(s->str);
	s->capacity = s->size = 0;
}