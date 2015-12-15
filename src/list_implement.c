//#include "hash_implement.h"
//#include <stdlib.h>
//#include <string.h>
//#define NEXT(iter) (&table->overflow_zone[iter->next_index])
//#define OLD_NEXT(iter) (&oldTable->overflow_zone[iter->next_index])
//void hash_table_expand(Hash_table_t *table);
//char* get_new_str(char* str) {
//	int size = strlen(str);
//	char *newstr =(char*) malloc(size + 1);
//	strcpy(newstr, str);
//	return newstr;
//}
//Hash_item_t* get_new_item(char* field_name, void* database) {
//	Hash_item_t* new_item = (Hash_item_t*)malloc(sizeof(Hash_item_t));
//	new_item->database = database;
//	new_item->field_name = get_new_str(field_name);
//	new_item->main_hash_value = hash_function_main(field_name);
//	new_item->sub_hash_value = 0;
//	return new_item;
//}
//
//unsigned long long hash_function_sub(char* str)
//{
//	typedef unsigned long long T;
//	const T BitsInUnsignedInt = (T)(sizeof(T) * 8);
//	const T ThreeQuarters = (T)((BitsInUnsignedInt * 3) / 4);
//	const T OneEighth = (T)(BitsInUnsignedInt / 8);
//	const T HighBits = (T)((T) - 1) << (BitsInUnsignedInt - OneEighth);
//	T hash = 0x123456789ABCDEF0LL;
//	T test = 0;
//	for (; *str; str++)
//	{
//		hash = (hash << OneEighth) + (*str);
//		if ((test = hash & HighBits) != 0)
//		{
//			hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
//		}
//	}
//	if (!hash)
//		++hash;	//make 0 impossible
//	return hash;
//}
///* End Of  P. J. Weinberger Hash Function */
//unsigned int hash_function_main(char * str)
//{
//	//BKDRHash
//	unsigned int seed = 131;
//	unsigned int hash = 0;
//	while (*str) {
//		hash = hash * seed + (*str++);
//	}
//	if (!hash)
//		++hash;	//make 0 impossible
//	return hash;
//}
//
//
//
//void hash_table_initialize(Hash_table_t *table, int size) {
//	table->size = 0;
//	table->capacity = size;
//	table->overflow_index = 0;
//	table->field_space = (Hash_item_t*)malloc(sizeof(Hash_item_t)*size);
//	memset(table->field_space, 0, sizeof(Hash_item_t)*size);
//	table->overflow_zone = (Hash_item_t*)malloc(sizeof(Hash_item_t)*size);
//	memset(table->overflow_zone, 0, sizeof(Hash_item_t)*size);
//	table->overflow_zone->next_index = 1;	//make it the header of free-slot-list for reuse
//}
//void hash_table_init(Hash_table_t* table) {
//	int init_size = 16; // must be power pf 2
//	hash_table_initialize(table, init_size);
//}
//
//
//bool equal(Hash_item_t* iter_a, Hash_item_t* iter_b) {
//	if (!iter_a->sub_hash_value)
//		iter_a->sub_hash_value = hash_function_sub(iter_a->field_name);
//	if (!iter_b->sub_hash_value)
//		iter_b->sub_hash_value = hash_function_sub(iter_b->field_name);
//	return iter_a->sub_hash_value == iter_b->sub_hash_value;
//}
//
//
//bool hash_table_add(Hash_table_t *table, Hash_item_t* new_item) {
//	// leave out slot 0 as a deleted-list indicator
//
//	int hashmask = table->capacity - 1;
//	if (new_item->main_hash_value == 0)
//		new_item->main_hash_value = hash_function_main(new_item->field_name);
//
//	int index = hashmask & new_item->main_hash_value;
//	Hash_item_t* const head_iter = &table->overflow_zone[0];
//	Hash_item_t* iter = &table->field_space[index];
//
//	if (iter->main_hash_value == 0) {
//		//raw slot
//		memcpy(iter, new_item, sizeof(Hash_item_t));
//		iter->next_index = 0;
//		++table->size;
//		return true;
//	}
//	for (; iter->next_index != 0		//stop at the last valid value
//		; iter = NEXT(iter))
//	{
//		if (equal(iter, new_item))
//		{
//			free(iter->database);
//			iter->database = new_item->database;
//			free(new_item->field_name);
//			free(new_item);
//			return false;
//		}
//	}
//	if (equal(iter, new_item))
//	{
//		free(iter->database);
//		iter->database = new_item->database;
//		free(new_item->field_name);
//		free(new_item);
//		return false;
//	}
//	
//	if (head_iter->next_index > table->overflow_index) {
//		// no free slot
//		++table->overflow_index;
//		if (table->overflow_index < table->capacity) {
//			//max load factor : 2.0
//			head_iter->next_index = table->overflow_index + 1;
//			iter->next_index = table->overflow_index;
//			iter = NEXT(iter);
//
//			memcpy(iter, new_item, sizeof(Hash_item_t));
//			iter->next_index = 0;
//		}
//		else {
//			// EXPAND
//			hash_table_expand(table);
//			return hash_table_add(table, new_item);
//		}
//	}
//	else {
//		//free slot found
//		iter->next_index = head_iter->next_index;
//		head_iter->next_index = NEXT(iter)->next_index;
//		iter = NEXT(iter);
//
//		memcpy(iter, new_item, sizeof(Hash_item_t));
//		iter->next_index = 0;
//	}
//	++table->size;
//	return true;
//}
//
//bool hash_table_erase(Hash_table_t *table, char* delete_key_name) {
//	Hash_item_t new_item_data;
//	Hash_item_t* new_item = &new_item_data;
//	CLEAR(new_item);
//	new_item->field_name = delete_key_name;
//	new_item->main_hash_value = hash_function_main(delete_key_name);
//	int hashmask = table->capacity - 1;
//	int index = hashmask & (new_item->main_hash_value);
//	Hash_item_t* const free_iter = &table->overflow_zone[0];
//	Hash_item_t* slot_iter = &table->field_space[index];
//	Hash_item_t* iter = slot_iter;
//
//	for (; iter->main_hash_value != 0; iter = NEXT(iter)) {
//		if (equal(iter, new_item)) {
//			free(iter->database);
//			free(iter->field_name);
//			if (slot_iter->next_index == 0) {
//				CLEAR(iter);
//				--table->size;
//				return true;
//			}
//			Hash_item_t* delete_item = NEXT(slot_iter);
//			if (delete_item != iter) {
//				int tmp = iter->next_index;
//				memcpy(iter, delete_item, sizeof(*iter));
//				iter->next_index = tmp;
//			}
//			slot_iter->next_index = delete_item->next_index;
//			delete_item->next_index = free_iter->next_index;
//			free_iter->next_index = delete_item - free_iter;
//			--table->size;
//			return true;
//		}
//	}
//	return false;
//}
//
//int hash_table_visit_by_cursor(Hash_table_t * table, int cursor, int count, bool(*action)(char*))
//{
//	for (int i = cursor; i < table->capacity; i++)
//	{
//		if (count <= 0) {
//			return i;
//		}
//		for (Hash_item_t* iter = &table->field_space[i]
//			; iter->main_hash_value != 0
//			; iter = NEXT(iter))
//		{
//			if (action(iter->field_name))
//				--count;
//		}
//	}
//	return 0;
//}
//
//int hash_table_visit_all(Hash_table_t * table, void(*action)(Hash_item_t *))
//{
//	for (int i = 0; i < table->capacity; i++)
//	{
//		for (Hash_item_t* iter = &table->field_space[i]
//			; iter->main_hash_value != 0
//			; iter = NEXT(iter))
//		{
//			action(iter);
//		}
//	}
//	return 0;
//}
//Hash_item_t* hash_table_visit_next(Hash_table_t * table,\
//	Hash_item_t* iter, bool (*cmp)(char*))
//{
//	iter = NEXT(iter);
//	// regain slot index
//	for (int i = iter->main_hash_value%table->capacity; \
//		i < table->capacity; \
//		iter = &table->field_space[++i])	//seems to "GOTO" the inner most
//	{
//		for (; iter->main_hash_value != 0
//			; iter = NEXT(iter))
//		{
//			if (cmp(iter->field_name))
//				return iter;
//		}
//	}
//	return 0;
//}
//
//void hash_table_expand(Hash_table_t *table) {
//	Hash_table_t oldTableData = *table;
//	Hash_table_t* oldTable = &oldTableData;
//	hash_table_initialize(table, oldTable->capacity * 8); //load factor from 2 to 0.25
//	for (int i = 0; i < oldTable->capacity; i++)
//	{
//		for (Hash_item_t* iter = &oldTable->field_space[i]
//			; iter->main_hash_value != 0
//			; iter = OLD_NEXT(iter))
//		{
//			//char* s= iter->field_name;
//			//int tmp = iter->next_index;
//			hash_table_add(table, iter);
//			//iter->next_index = tmp;
//		}
//	}
//	free(oldTable->overflow_zone);
//	free(oldTable->field_space);
//}
//
//Hash_item_t * hash_table_random(Hash_table_t * table)
//{	// return the first valid element. WHY NOT RANDOM?? Lazy:)
//	for (int i = 0; i < table->capacity; i++)
//	{
//		Hash_item_t *iter;
//		if (iter = &table->field_space[i])
//			return iter;
//	}
//	return nullptr;
//}
//
//
//Hash_item_t* hash_table_find(Hash_table_t *table, char* new_key) {
//	Hash_item_t new_item_data;
//	Hash_item_t* new_item = &new_item_data;
//	CLEAR(new_item);
//	new_item->field_name = new_key;
//	new_item->main_hash_value = hash_function_main(new_key);
//	int hashmask = table->capacity - 1;
//	int index = hashmask & (new_item->main_hash_value);
//	for (Hash_item_t* iter = &table->field_space[index]
//		; iter->main_hash_value != 0
//		; iter = NEXT(iter)) {
//		if (equal(iter, new_item)) {
//			return iter;
//		}
//	}
//	return nullptr;
//}
//
//void hash_table_destruct(Hash_table_t *table) {
//	for (int i = 0; i < table->capacity; i++)
//	{
//		for (Hash_item_t* iter = &table->field_space[i]
//			; iter->main_hash_value != 0
//			; iter = NEXT(iter))
//		{
//			free(iter->database);
//			free(iter->field_name);
//		}
//	}
//	free(table->overflow_zone);
//	free(table->field_space);
//}