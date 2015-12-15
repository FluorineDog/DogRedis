#include "string.h"
#include "string_implement.h"
enum type_t KEY_TYPE = STRING_KEY;

#define STRING_OPEN(STRING) OPEN(STRING,String_t,string_init)
#define STRING_OPEN_N(STRING,INDEX) OPEN_N(STRING,String_t,string_init,INDEX)
#define STRING_INIT(STRING) INIT(STRING,String_t)
#define STRING_INIT_N_PEACE(STRING,INDEX) INIT_N_PEACE(STRING,String_t,INDEX)

DOG_CMD_FUNC_DEF(append) {
	CHECK_ARGUMENTS(argc == 3);
	STRING_OPEN(string);
	string_append(string, argv[2]);
	return OK;
}
DOG_CMD_FUNC_DEF(getbit) {
	CHECK_ARGUMENTS(argc == 3);
	STRING_INIT_N_PEACE(string,1);
	if (!string)
		return 0;
	return string_get_bit(string, atoi(argv[2])) ? 1 : 0;
}
DOG_CMD_FUNC_DEF(setbit) {
	CHECK_ARGUMENTS(argc == 4);
	STRING_OPEN(string);
	string_set_bit(string, atoi(argv[2]), !!atoi(argv[3]));
	return OK;
}
static int n, print_count;
static char* tmp_outbuf;

void show_str(char* output_buf, char* str) {
	tmp_outbuf = output_buf;
	n += snprintf(tmp_outbuf + n, BUF_SZ - n, \
		"(%d) %s\n", ++print_count, str);
}
DOG_CMD_FUNC_DEF(mget) {
	CHECK_ARGUMENTS(argc >= 2);
	n = print_count = 0;
	for (int i = 1; i < argc; ++i) {
		STRING_INIT_N_PEACE(string, i);
		if (string)
			show_str(output_buf, string->str);
		else
			show_str(output_buf, "(NIL)");
	}
	return NIL;
}
DOG_CMD_FUNC_DEF(bitcount) {
	CHECK_ARGUMENTS(argc <= 4&&argc>=2);
	STRING_INIT_N_PEACE(string,1);
	if (!string)
		return 0;
	int end = argc >= 4 ? atoi(argv[3]) : -1;
	int beg = argc >= 3 ? atoi(argv[2]) : 0;
	end = index(string, end);
	beg = index(string, beg);
	++end;
	int count = 0;
	for (int i = beg; i < end; ++i) {
		int tmp = string->str[i];
		for (char j = 0; j < j; ++j) {
			if (tmp & 1) {
				++count;
			}
			tmp >>= 1;
		}
	}
	return count;
}
DOG_CMD_FUNC_DEF(setrange) {
	CHECK_ARGUMENTS(argc == 4);
	STRING_OPEN(string);
	int offset = atoi(argv[2]);
	string_override(string, offset, argv[3]);
	return OK;
}	
DOG_CMD_FUNC_DEF(getrange) {
	CHECK_ARGUMENTS(argc == 4);
	STRING_INIT_N_PEACE(string,1);
	if (!string)
		return NIL;
	int end = argc >= 4 ? atoi(argv[3]) : -1;
	int beg = argc >= 3 ? atoi(argv[2]) : 0;
	end = index(string, end);
	beg = index(string, beg);
	++end;
	snprintf(output_buf, end - beg + 1, "%s", string->str + beg);
	return NIL;
}
DOG_CMD_FUNC_DEF(incr) {
	CHECK_ARGUMENTS(argc == 2);
	STRING_OPEN(string);
	char num_buf[MAX_NUM_SIZE];
	long long d = atoll(string->str);
	++d;
	snprintf(num_buf, MAX_NUM_SIZE, "%lld", d);
	string_reset(string, num_buf);
	OUTPUT(num_buf);
	return NIL;
}
DOG_CMD_FUNC_DEF(decr) {
	CHECK_ARGUMENTS(argc == 2);
	STRING_OPEN(string);
	char num_buf[MAX_NUM_SIZE];
	long long d = atoll(string->str);
	--d;
	snprintf(num_buf, MAX_NUM_SIZE, "%lld", d);
	string_reset(string, num_buf);
	OUTPUT(num_buf);
	return NIL;
}
DOG_CMD_FUNC_DEF(incrby) {
	CHECK_ARGUMENTS(argc == 3);
	STRING_OPEN(string);
	char num_buf[MAX_NUM_SIZE];
	long long d = atoll(argv[2]);
	d += atoll(string->str);
	snprintf(num_buf, MAX_NUM_SIZE, "%lld", d);
	string_reset(string, num_buf);
	OUTPUT(num_buf);
	return NIL;
}
DOG_CMD_FUNC_DEF(decrby) {
	CHECK_ARGUMENTS(argc == 3);
	STRING_OPEN(string);
	char num_buf[MAX_NUM_SIZE];
	long long d = atoll(argv[2]);
	d -= atoll(string->str);
	snprintf(num_buf, MAX_NUM_SIZE, "%lld", d);
	string_reset(string, num_buf);
	OUTPUT(num_buf);;
	return NIL;
}
DOG_CMD_FUNC_DEF(msetnx) {
	CHECK_ARGUMENTS(argc >= 3 && argc % 2 == 1);
	for (int i = 1; i < argc; i += 2) {
		STRING_INIT_N_PEACE(string_current, i);
		if (string_current)
			return 0;
	}
	for (int i = 1; i < argc; i += 2) {
		STRING_OPEN_N(string_current, i);
		string_reset(string_current, argv[i + 1]);
	}
	return 1;
}
DOG_CMD_FUNC_DEF(get) {
	CHECK_ARGUMENTS(argc == 2);
	STRING_INIT_N_PEACE(string,1);
	if(string)
		OUTPUT(string->str);
	return NIL;
}
DOG_CMD_FUNC_DEF(set) {
	CHECK_ARGUMENTS(argc == 3);
	STRING_OPEN(string);
	string_reset(string, argv[2]);
	return OK;
}
DOG_CMD_FUNC_DEF(strlen) {
	CHECK_ARGUMENTS(argc == 2);
	STRING_INIT_N_PEACE(string, 1);
	if (string)
		return string->size;
	else
		return 0;
}