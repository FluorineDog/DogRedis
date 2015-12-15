#ifndef DOG_HASH_IMPLEMENT_H_
#define DOG_HASH_IMPLEMENT_H_
#include <stdbool.h>
#define _CRT_SECURE_NO_WARNINGS
unsigned long long hash_function_sub(char*  str);
unsigned int hash_function_main(char * str);
#define CLEAR(iter) memset(iter,0,sizeof(*iter))
typedef struct Hash_item_t {
	char* field_name;
	int next_index;
	unsigned int main_hash_value;
	unsigned long long sub_hash_value;
	void* database;
}Hash_item_t;

typedef struct Hash_table_t_ {
	int size;
	int capacity;	//must be power of 2
	Hash_item_t* field_space;
	int overflow_index;			//mark the right most overflow element
	Hash_item_t* overflow_zone;	//jump array-like list
}Hash_table_t;
void hash_table_init(Hash_table_t* table);
void hash_table_destruct(Hash_table_t* table);
bool hash_table_add(Hash_table_t* table, Hash_item_t* new_item);
Hash_item_t* get_new_item(char* field_name, void* database);
bool hash_table_erase(Hash_table_t* table, char* new_key);
int hash_table_visit_all(Hash_table_t* table, void(*action)(Hash_item_t* data));
Hash_item_t* hash_table_random(Hash_table_t* table);
bool hash_table_rename_key(Hash_table_t* table, char* oldname, char* new_new_name);
Hash_item_t* hash_table_find(Hash_table_t* table, char* new_key);
char* get_new_str(char* str);
Hash_item_t* hash_table_visit_next(Hash_table_t* table, Hash_item_t* iter, bool(*cmp)());
int hash_table_visit_by_cursor(Hash_table_t * table, int cursor, int count, bool(*action)(char*));

#endif // DOG_HASH_IMPLEMENT_H_