#include "set.h"
static enum type_t KEY_TYPE = SET_KEY;

#define SET_OPEN(SET) OPEN(SET,Hash_table_t,hash_table_init)
#define SET_OPEN_N(SET,INDEX) OPEN_N(SET,Hash_table_t,hash_table_init,INDEX)
#define SET_INIT(SET) INIT(SET,Hash_table_t)
#define SET_INIT_N_PEACE(SET,INDEX) INIT_N_PEACE(SET,Hash_table_t,INDEX)
static int n, print_count;	//for print procedure

DOG_CMD_FUNC_DEF(sadd) {
	CHECK_ARGUMENTS(argc >= 3);
	SET_OPEN(set);
	int count = 0;
	for (int i = 2; i < argc; ++i) {
		if (hash_table_add(set, get_new_item(argv[i], nullptr)))
			++count;
	}
	return count;
}

DOG_CMD_FUNC_DEF(smove) {
	CHECK_ARGUMENTS(argc == 4);
	SET_INIT(set);
	if (!hash_table_find(set, argv[3]))
	{
		OUT_APED("element not found");
		return ERROR;
	}
	hash_table_erase(set, argv[3]);
	if (set->size) {
		hash_table_erase(set, argv[1]);
	}
	SET_OPEN_N(set2, 2);
	hash_table_add(set2, get_new_item(argv[3], nullptr));
	return OK;
}

DOG_CMD_FUNC_DEF(scard) {
	CHECK_ARGUMENTS(argc == 2);
	SET_INIT(set);
	return set->size;
}

DOG_CMD_FUNC_DEF(spop) {
	CHECK_ARGUMENTS(argc == 2);
	SET_INIT(set);
	for (int i = 0; i < set->capacity; ++i) {
		if (set->field_space[i].field_name != nullptr) {
			OUTPUT(set->field_space[i].field_name);
			hash_table_erase(set, set->field_space[i].field_name);
			break;
		}
	}
	if (set->size == 0) {
		key_remove(argv[1]);
	}
	return NIL;
}

DOG_CMD_FUNC_DEF(srandmember) {
	CHECK_ARGUMENTS(argc == 2);
	SET_INIT(set);
	for (int i = 0; i < set->capacity; ++i) {
		if (set->field_space[i].field_name != nullptr) {
			OUTPUT(set->field_space[i].field_name);
			break;
		}
	}
	return NIL;
}

static Hash_table_t* tmp_des;
void add_it_to_des(Hash_item_t* x) {
	hash_table_add(tmp_des, get_new_item(x->field_name, nullptr));
}

void del_it_from_des(Hash_item_t* x) {
	hash_table_erase(tmp_des, x->field_name);
}



DOG_CMD_FUNC_DEF(srem) {
	CHECK_ARGUMENTS(argc >= 3);
	SET_INIT(set);
	int count = 0;
	for (int i = 2; i < argc; ++i) {
		if (hash_table_erase(set, argv[i]))
			++count;
	}
	return count;
}

static char* tmp_outbuf;
void show_item(Hash_item_t* x) {
	n += snprintf(tmp_outbuf + n, BUF_SZ - n, \
		"(%d) %s\n", ++print_count, x->field_name);
}
void show(char* output_buf, Hash_table_t* set) {
	n = 0;
	print_count = 0;
	tmp_outbuf = output_buf;
	hash_table_visit_all(set, show_item);
}
DOG_CMD_FUNC_DEF(smembers) {
	CHECK_ARGUMENTS(argc == 2);
	SET_INIT_N_PEACE(set, 1);
	if (!set)
		return NIL;
	show(output_buf, set);
	return NIL;
}

static Hash_table_t* tmp_current;
void inter(Hash_item_t* x) {
	if (!hash_table_find(tmp_current, x->field_name)) {
		hash_table_erase(tmp_des, x->field_name);
	}
}
DOG_CMD_FUNC_DEF(sdiff) {
	CHECK_ARGUMENTS(argc >= 3);
	SET_INIT_N_PEACE(set1, 1);
	if (!set1)
		return NIL;
	Hash_table_t tmp_set;
	tmp_des = &tmp_set;
	hash_table_init(tmp_des);
	hash_table_visit_all(set1, add_it_to_des);
	for (int i = 2; i < argc; ++i) {
		SET_INIT_N_PEACE(set_current, i);
		if (set_current)
			hash_table_visit_all(set_current, del_it_from_des);
	}
	show(output_buf, tmp_des);
	hash_table_destruct(tmp_des);
	return NIL;

}
DOG_CMD_FUNC_DEF(sinter) {
	CHECK_ARGUMENTS(argc >= 3);
	Hash_table_t* minset = nullptr;
	int minsize = ((unsigned int)-1) / 2;
	for (int i = 1; i < argc; ++i) {
		SET_INIT_N_PEACE(set_current, i);
		if (set_current == nullptr) {
			return NIL;
		}
		if (set_current->size < minsize) {
			minsize = set_current->size; //find min
			minset = set_current;
		}
	}
	Hash_table_t tmp_table;		// origin
	Hash_table_t tmp_table2;	// for modifying
	tmp_des = &tmp_table;
	hash_table_init(tmp_des);
	hash_table_visit_all(minset, add_it_to_des);	

	tmp_des = &tmp_table2;		
	hash_table_init(tmp_des);
	hash_table_visit_all(minset, add_it_to_des);


	for (int i = 1; i < argc; ++i) {
		SET_INIT_N_PEACE(set_current, i);
		tmp_current = set_current;
		hash_table_visit_all(&tmp_table, inter);
	}
	show(output_buf, tmp_des);
	hash_table_destruct(&tmp_table);
	hash_table_destruct(&tmp_table2);
	return NIL;
}
static char* pattern;
bool cmp_print(char* str) {
	if (true) {
		n += snprintf(tmp_outbuf + n, BUF_SZ - n, "(%d) %s\n" \
			, ++print_count, str);
		return true;
	}
	else 
		return false;
}
DOG_CMD_FUNC_DEF(sscan) {
	CHECK_ARGUMENTS(argc >= 3 && argc <= 5);
	SET_INIT(set);
	int count = (argc == 5) ? atoi(argv[4]) : 10;
	pattern = (argc >= 4) ? argv[3] : "*";
	int cursor = atoi(argv[2]);
	tmp_outbuf = output_buf;
	n = 0;
	print_count = 0;
	cursor = hash_table_visit_by_cursor(set, cursor, count, cmp_print);
	n += snprintf(output_buf + n, BUF_SZ - n, "new cursor:%d\n", cursor);
	return NIL;
}

DOG_CMD_FUNC_DEF(sunion) {
	CHECK_ARGUMENTS(argc >= 3);
	Hash_table_t result;
	tmp_des = &result;
	hash_table_init(tmp_des);
	for (int i = 1; i < argc; ++i) {
		SET_INIT_N_PEACE(set_current, i);
		if (!set_current)
			continue;
		hash_table_visit_all(set_current, add_it_to_des);
	}

	show(output_buf, tmp_des);
	return NIL;
}

DOG_CMD_FUNC_DEF(sismember) {
	CHECK_ARGUMENTS(argc == 3);
	SET_INIT(set);
	bool flag = !!hash_table_find(set, argv[2]);
	return flag ? 1 : 0;
}
