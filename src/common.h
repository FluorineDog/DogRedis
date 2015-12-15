#ifndef DOG_COMMON_H_
#define DOG_COMMON_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stdbool.h>
#include <regex>
#include "string_implement.h"
#include "hash_implement.h" 
#define DOG_CMD_FUNC_DEF(CMD_NAME) \
int CMD_NAME(int argc, char* argv[], char* output_buf)
#ifndef __cplusplus
#define nullptr ((void *)0)
#endif
const int BUF_SZ = 0x400;
#define ERROR -1
#define OK	-0x2333333
#define NIL -0x23333333
#ifndef MAXLINE
#define MAXLINE 1024
#define MAX_NUM_SIZE 50
#endif //MAXLINE

#define OUTPUT(STRING_) strcat(output_buf, (STRING_))
#define PUT_NAME(ITER) OUTPUT(ITER? ITER->field_name:"(nil)")
#define OUT_APED(STRING_) OUTPUT(STRING_)


#define CHECK_ARGUMENTS(COND) do{							\
	if(!(COND)){											\
		OUT_APED("ERR Wrong number of arguments for" );		\
		return ERROR;										\
	}														\
}while(0)	




#define WAITCTL(CTL) do {				\
	if (!CTL) {							\
		OUT_APED("ERR timeout");		\
		return ERROR;}					\
}while(0)



#define QUIT(ENTRY_) unlock(&(ENTRY_)->status);

enum type_t { NIL_KEY, HASH_KEY, LIST_KEY, STRING_KEY, SET_KEY };
typedef struct Key_entry_ {
	void* container;
	int status;
	enum type_t type;
}Key_entry;
extern Hash_table_t keySpace;

bool waitKeyModifyCtl();
bool waitKeyQueryCtl();
bool waitEntry(volatile int*);
void unlockKeyModifyCtl();
void unlockKeyQueryCtl();
void unlockEntry(volatile int*);
#define GET_ITER_N(N_)		(hash_table_find(&keySpace, argv[N_]))
#define GET_ITER()			GET_ITER_N(1)
#define GET_ENTRY(ITER)		((Key_entry*)(ITER->database))
#define GET_CONTAINER(ITER)	(GET_ENTRY(ITER)->container)
#define LOCKALL				WAITCTL(waitKeyModifyCtl())
#define UNLOCKALL			unlockKeyModifyCtl()
#define LOCK				LOCKALL
#define UNLOCK				UNLOCKALL
#define INIT(CONTAINER,ToC)						\
	ToC* CONTAINER;do{							\
	LOCK;										\
	Hash_item_t* iter = GET_ITER();				\
	if (!iter) {								\
		OUT_APED("key not found");				\
		UNLOCK;									\
		return ERROR;							\
	}											\
	Key_entry* entry = GET_ENTRY(iter);			\
	if (entry->type != KEY_TYPE)				\
	{											\
		OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");\
		UNLOCK;									\
		return ERROR;							\
	}											\
	CONTAINER = (ToC*)entry->container;			\
	}while(0)


#define INIT_N_PEACE(CONTAINER,ToC,INDEX)		\
	ToC* CONTAINER;do{							\
	LOCK;										\
	Hash_item_t* iter = GET_ITER_N(INDEX);		\
	if (!iter) {								\
		CONTAINER = nullptr;					\
		break;									\
	}											\
	Key_entry* entry = GET_ENTRY(iter);			\
	if (entry->type != KEY_TYPE)				\
	{											\
		CONTAINER = nullptr;					\
		break;									\
		OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");\
		UNLOCK;									\
		return ERROR;							\
	}											\
	CONTAINER = (ToC*)entry->container;			\
	}while(0)

//ToC = type of container
#define OPEN(CONTAINER,ToC,INITER)	 							\
	ToC* CONTAINER;do{											\
	LOCK;														\
	Hash_item_t* iter = GET_ITER();								\
	Key_entry* entry;											\
	if (!iter) {												\
		CONTAINER = (ToC*)malloc(sizeof ToC);					\
		INITER(CONTAINER);										\
		entry = (Key_entry* )malloc(sizeof(Key_entry));			\
		entry->container = CONTAINER;							\
		entry->type = KEY_TYPE;									\
		Hash_item_t* item = get_new_item(argv[1], entry);		\
		hash_table_add(&keySpace, item);						\
	}															\
	else {														\
		entry = GET_ENTRY(iter);								\
		if (entry->type != KEY_TYPE)							\
		{														\
			OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");		\
			UNLOCK;												\
			return ERROR;										\
		}														\
		CONTAINER = (ToC*) entry->container;					\
	}															\
	}while(0)

#define OPEN_N(CONTAINER,ToC,INITER, INDEX)	 					\
	ToC* CONTAINER;do{											\
	LOCK;														\
	Hash_item_t* iter = GET_ITER_N(INDEX);						\
	Key_entry* entry;											\
	if (!iter) {												\
		CONTAINER = (ToC*)malloc(sizeof ToC);					\
		INITER(CONTAINER);										\
		entry = (Key_entry* )malloc(sizeof(Key_entry));			\
		entry->container = CONTAINER;							\
		entry->type = KEY_TYPE;									\
		Hash_item_t* item = get_new_item(argv[INDEX], entry);	\
		hash_table_add(&keySpace, item);						\
	}															\
	else {														\
		entry = GET_ENTRY(iter);								\
		if (entry->type != KEY_TYPE)							\
		{														\
			OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");		\
			UNLOCK;												\
			return ERROR;										\
		}														\
		CONTAINER = (ToC*) entry->container;					\
	}															\
	}while(0)

bool key_remove(char* keyname);
#endif // !DOG_COMMON_H_