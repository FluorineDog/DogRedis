//#define DOG_TEST_HASH_IMP
#ifdef DOG_TEST_HASH_IMP
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
using std::cout;
using std::cin;
using std::endl;
using std::vector;
#define PAUSE std::system("pause")
#include "../src/hash_implement.h"
std::vector<Hash_item_t> vs;
std::vector<Hash_item_t> vo;
int main(){
	Hash_table_t ht_data;
	Hash_table_t *table=&ht_data;
	hash_table_init(table);
	Hash_item_t ni;

	vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
	vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	for (int i = 0; i < 25; ++i) {
		auto string =(char*) malloc(5);
		string[4] = '\0';
		string[3] = i % 10 + '0';
		string[2] = i /10 % 10 + '0';
		string[1] = i / 100 % 10 + '0';
		string[0] = i / 1000 % 10 + '0';

		CLEAR(&ni);
		ni.field_name = string;
		hash_table_add(table,&ni);
		vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
		vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	}
	vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
	vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	hash_table_erase(table, "0017");
	vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
	vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	hash_table_erase(table, "0007");
	vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
	vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	bool a = !!hash_table_find(table,"0017");
	bool b = !!hash_table_find(table, "0016");

	auto string = (char*)malloc(5);
	int i = 2000;
	string[4] = '\0';
	string[3] = i % 10 + '0';
	string[2] = i / 10 % 10 + '0';
	string[1] = i / 100 % 10 + '0';
	string[0] = i / 1000 % 10 + '0';
	CLEAR(&ni);
	ni.field_name = string;
	hash_table_add(table, &ni);

	vs = std::vector<Hash_item_t>(table->field_space, table->field_space + table->capacity);
	vo = std::vector<Hash_item_t>(table->overflow_zone, table->overflow_zone + table->capacity);
	hash_table_destruct(table);
	PAUSE;
	return 0;
}
#endif // DOG_TEST_HASH_IMP