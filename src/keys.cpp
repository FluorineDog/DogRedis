
//#define GETENTRY() ((Key_entry*)hash_table_find(&keySpace,argv[1]))
#include "keys.h"
#include <string.h>
#define __STDC__ 1

DOG_CMD_FUNC_DEF(del)
{
	CHECK_ARGUMENTS(argc >= 2);
	LOCKALL;
	//Hash_item_t* iter = GET_ITER();
	int count = 0;
	for (int i = 1; i < argc; ++i) {
		if (key_remove(argv[i])) {
			++count;
		}
	}
	UNLOCKALL;
	return count;
}

DOG_CMD_FUNC_DEF(exists)
{
	CHECK_ARGUMENTS(argc == 2);
	LOCK;
	bool flag = !!GET_ITER();
	UNLOCK;
	return flag ? 1 : 0;
}


DOG_CMD_FUNC_DEF(randomkey)
{
	CHECK_ARGUMENTS(argc == 1);
	LOCK;
	for (int i = 0; i < keySpace.capacity; ++i) {
		if (keySpace.field_space[i].field_name != nullptr) {
			OUTPUT(keySpace.field_space[i].field_name);
			break;
		}
	}
	UNLOCK;
	return NIL;
}

DOG_CMD_FUNC_DEF(rename)
{
	CHECK_ARGUMENTS(argc == 3);
	LOCKALL;
	Hash_item_t* iter = GET_ITER();
	if (iter) {
		void * data = iter->database;
		iter->database = nullptr;
		key_remove(argv[1]);
		hash_table_add(&keySpace, get_new_item(argv[2], data));
	}
	UNLOCKALL;
	return OK;
}
//struct {
//	char ebuf[128];
//	regex_t reg;
//	regmatch_t pm[1];
//	char * pattern;
//	char* output_buf;
//}regex;
static struct {
	char* output_buf;
}tmp;
void action(Hash_item_t* iter) {
	char* output_buf = tmp.output_buf;
	if (!false) {
		OUTPUT(iter->field_name);//BUG BUG
		strcat(output_buf, "\n");
	}
}

DOG_CMD_FUNC_DEF(keys)
{
	CHECK_ARGUMENTS(argc == 2);
	tmp.output_buf = output_buf;
	hash_table_visit_all(&keySpace, action);
	UNLOCK;
	return NIL;
}

DOG_CMD_FUNC_DEF(type)
{
	CHECK_ARGUMENTS(argc == 2);
	LOCK;
	Hash_item_t* iter = GET_ITER();
	enum type_t tmp;
	if (iter) {
		Key_entry* entry = GET_ENTRY(iter);
		switch (entry->type)
		{
		case HASH_KEY:
			OUTPUT("hash");
			break;
		case LIST_KEY:
			OUTPUT("list");
			break;
		case STRING_KEY:
			OUTPUT("string");
			break;
		case SET_KEY:
			OUTPUT("set");
			break;
		case NIL_KEY:
			OUTPUT("(nil)");
			break;
		default:
			OUT_APED("Data corrupted");
			return ERROR;
		}
		tmp = entry->type;
	}
	UNLOCK;
	return OK;
}
#undef ENTRY_TYPE
