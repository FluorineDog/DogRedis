#include "common.h"
#include "hash_implement.h"
static volatile int queryProcess = 0;
static volatile int status = 0;
Hash_table_t keySpace;
bool waitKeyModifyCtl()
{
	return true;
}
void unlockKeyModifyCtl() {

}
bool key_remove(char* keyname) {
	Hash_item_t* item = hash_table_find(&keySpace, keyname);
	if (!item) {
		return false;
	}
	Key_entry* entry= (Key_entry*)item->database;
	switch (entry->type)
	{
	case NIL_KEY:
		abort();
		break;
	case HASH_KEY:
	case SET_KEY:
		hash_table_destruct((Hash_table_t*)entry->container);
		free( entry->container);
		hash_table_erase(&keySpace, keyname);
		break;
	//case LIST_KEY:
	//	list_destruct((Hash_table_t*)entry->container);
	//	delete entry->container;
	//	hash_table_erase(&keySpace, keyname);
	//	break;
	case STRING_KEY:
		string_destruct((String_t*)entry->container);
		free(entry->container);
		hash_table_erase(&keySpace, keyname);
		break;
	default:
		abort();
		break;
	}
	return true;
}