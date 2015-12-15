#ifndef DOG_EXEC_H_
#define DOG_EXEC_H_
#include <stdlib.h>
#include <string.h>
typedef int (*func_t)(int, char**, char*);

typedef struct function_map_item_ {
	char name[15];
	func_t func;
}function_map_item;

func_t function(char* name);
int exec(char* output, char* input);
int analyse(char* input, char** argv_buf);
#endif
