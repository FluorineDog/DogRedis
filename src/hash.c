#include "hash.h"
static enum type_t KEY_TYPE = HASH_KEY;

#define HASH_OPEN(TABLE) OPEN(TABLE,Hash_table_t,hash_table_init)
#define HASH_INIT(TABLE) INIT(TABLE,Hash_table_t)
DOG_CMD_FUNC_DEF(hdel)
{
	CHECK_ARGUMENTS(argc >= 3);
	HASH_INIT(table);
	int count = 0;
	for (int i = 2; i < argc; ++i) {
		if (hash_table_erase(table, argv[i])) {
			++count;
		}
	}
	if (table->size == 0)
		key_remove(argv[1]);
	UNLOCK;
	return count;
}

DOG_CMD_FUNC_DEF(hlen)
{
	CHECK_ARGUMENTS(argc == 2);
	HASH_INIT(table);
	int length = table->size;
	UNLOCK;
	return length;
}

DOG_CMD_FUNC_DEF(hexists)
{
	CHECK_ARGUMENTS(argc == 3);
	HASH_INIT(table);
	bool flag = !!hash_table_find(table, argv[2]);
	UNLOCK;
	return flag ? 1 : 0;
}

DOG_CMD_FUNC_DEF(hmget)
{
	CHECK_ARGUMENTS(argc >= 3 );
	HASH_INIT(table);
	int n = 0;
	for (int i = 2; i < argc; ++i) {
		Hash_item_t* iter = hash_table_find(table, argv[i]);
		n+=snprintf(output_buf+n, BUF_SZ-n, "(%d) %s\n", i - 2,
			iter ? (char*)iter->database : "(nil)");
	}
	UNLOCK;
	return OK;
}
DOG_CMD_FUNC_DEF(hget)
{
	CHECK_ARGUMENTS(argc == 3);
	Hash_table_t* table;
	do{
		LOCK;										
		Hash_item_t* iter = GET_ITER();				
		if (!iter) {								
			OUT_APED("key not found");				
			UNLOCK;									
			return ERROR;							
		}											
		Key_entry* entry = GET_ENTRY(iter);			
		if (entry->type != KEY_TYPE)				
		{											
			OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");
			UNLOCK;									
			return ERROR;							
		}											
		table = (Hash_table_t*)entry->container;
	} while (0);
	Hash_item_t* iter = hash_table_find(table, argv[2]);
	snprintf(output_buf, BUF_SZ, "%s\n",
		iter ? (char*)iter->database : "(nil)");
	UNLOCK;
	return OK;
}

DOG_CMD_FUNC_DEF(hmset)
{
	CHECK_ARGUMENTS(argc >= 4 && argc % 2 == 0);
	HASH_OPEN(table);
	for (int i = 2; i < argc; i += 2) {
		Hash_item_t* new_item = get_new_item(argv[i],\
			get_new_str(argv[i + 1]));
		hash_table_add(table, new_item);	// Won't fail. Thus atomic
	}
	UNLOCK;
	return OK;
}



DOG_CMD_FUNC_DEF(hincrby)
{
	CHECK_ARGUMENTS(argc == 4);
	HASH_OPEN(table);
	Hash_item_t* iter = hash_table_find(table, argv[2]);
	long long n = atoll((char*)iter->database);
	n += atoll(argv[3]);
	delete iter->database;
	iter->database = malloc(MAX_NUM_SIZE);
	int size = snprintf((char*)iter->database, MAX_NUM_SIZE, "%lld", n);
	((char*)iter->database)[size] = 0;
	OUTPUT((char*)iter->database);
	return OK;
}

DOG_CMD_FUNC_DEF(hset)
{
	CHECK_ARGUMENTS(argc == 4);
	Hash_table_t* table;do{
	LOCK;														
	Hash_item_t* iter = GET_ITER();								
	Key_entry* entry;											
	if (!iter) {												
		table = (Hash_table_t*)malloc(sizeof Hash_table_t);
		hash_table_init(table);										
		entry = (Key_entry* )malloc(sizeof(Key_entry));			
		entry->container = table;							
		entry->type = KEY_TYPE;									
		Hash_item_t* item = get_new_item(argv[1], entry);///////////BUGBUGBUG		
		hash_table_add(&keySpace, item);						
	}															
	else {														
		entry = GET_ENTRY(iter);								
		if (entry->type != KEY_TYPE)							
		{														
			OUT_APED("WRONGTYPE Operation against a key holding the wrong kind of value");		
			UNLOCK;												
			return ERROR;										
		}														
		table = (Hash_table_t*) entry->container;					
	}															
		} while (0);
	Hash_item_t* new_item = get_new_item(argv[2],
		get_new_str(argv[3]));
	hash_table_add(table, new_item);
	UNLOCK;
	return OK;
}

DOG_CMD_FUNC_DEF(hincrbyfloat)
{
	CHECK_ARGUMENTS(argc == 4);
	HASH_INIT(table);
	Hash_item_t* iter = hash_table_find(table, argv[2]);
	long double n = strtold((char*)iter->database,NULL);
	n += strtold(argv[3], NULL);
	delete iter->database;
	iter->database = malloc(MAX_NUM_SIZE);
	int size = snprintf((char*)iter->database, MAX_NUM_SIZE, "%lf", n);
	((char*)iter->database)[size] = 0;
	OUTPUT((char*)iter->database);
	return OK;
}

DOG_CMD_FUNC_DEF(hsetnx)
{
	CHECK_ARGUMENTS(argc == 4);
	HASH_OPEN(table);
	Hash_item_t* iter = hash_table_find(table, argv[2]);
	if (iter) {
		OUTPUT("key exists");
		return ERROR;
	}
	return OK;
}

static char* tmp_output_buf;
static int n;
static int count;
void show_field(Hash_item_t* iter) {
	n += snprintf(tmp_output_buf+n, BUF_SZ - n, "(%d) %s\n", count++, iter->field_name);
}
DOG_CMD_FUNC_DEF(hkeys)
{
	CHECK_ARGUMENTS(argc == 2);
	HASH_INIT(table);
	n = count = 0;
	tmp_output_buf = output_buf;
	hash_table_visit_all(table, show_field);
	return NIL;
}
void show_data(Hash_item_t* iter) {
	n += snprintf(tmp_output_buf+n, BUF_SZ-n, "(%d) %s\n", count++, (char*)iter->database);
}
DOG_CMD_FUNC_DEF(hvals)
{
	CHECK_ARGUMENTS(argc == 2);
	HASH_INIT(table);
	n = count = 0;

	tmp_output_buf = output_buf;
	hash_table_visit_all(table, show_data);
	return NIL;
}
void show_field_data(Hash_item_t* iter) {
	show_field(iter);
	show_data(iter);
}
DOG_CMD_FUNC_DEF(hgetall)
{
	CHECK_ARGUMENTS(argc == 2);
	HASH_INIT(table);
	n = count = 0;
	tmp_output_buf = output_buf;
	hash_table_visit_all(table, show_field_data);
	return NIL;
}