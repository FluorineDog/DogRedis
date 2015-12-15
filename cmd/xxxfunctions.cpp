#include "exec_func.h"
#include "../src/hash.h"
#include "../src/keys.h"
function_map_item func_map[] = {
	// hash.h
	{ "hdel", hdel },
	{ "hlen", hlen },
	{ "hexists", hexists },
	{ "hmget", hmget },
	{ "hget", hget },
	{ "hmset", hmset },
	{ "hgetall", hgetall },
	{ "hincrby", hincrby },
	{ "hset", hset },
	{ "hincrbyfloat", hincrbyfloat },
	{ "hsetnx", hsetnx },
	{ "hkeys", hkeys },
	{ "havls", havls },
	// keys.h
	{ "del", del },
	{ "exists", exists },
	{ "randomkey", randomkey },
	{ "rename", rename },
	{ "keys", keys },
	{ "type", type },
};
func_t function(char* name) {
	for (int i = 0; \
		i < sizeof(func_map) / sizeof(func_map[0]); ++i) 
	{
		if (!strcmp(name, func_map[i].name)) {
			return func_map[i].func;
		}
	}
	return nullptr;
}
