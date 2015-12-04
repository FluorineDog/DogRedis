#ifndef DOG_COMMON_H_
#define DOG_COMMON_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

typedef void (*func_t)();
typedef struct operationList_item{
	char name[16];
	func_t func;
}operationList_item;
extern operationList_item *operationList;

#endif