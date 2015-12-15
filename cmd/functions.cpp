#include "exec_func.h"
#include "../src/hash.h"
#include "../src/keys.h"
#include "../src/set.h"
#include "../src/string.h"
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
	{ "hvals", hvals },
	// keys.h
	{ "del", del },
	{ "exists", exists },
	{ "randomkey", randomkey },
	{ "rename", rename },
	{ "keys", keys },
	{ "type", type },
	// set.h
	{ "sadd", sadd },
	{ "smove", smove },
	{ "scard", scard },
	{ "spop", spop },
	{ "sdiff", sdiff },
	{ "srandmember", srandmember },
	{ "srem", srem },
	{ "sinter", sinter },
	{ "sscan", sscan },
	{ "sunion", sunion },
	{ "sismember", sismember },
	{ "smembers", smembers },
	// string.h
	{ "append", append },
	{ "getbit", getbit },
	{ "setbit", setbit },
	{ "mget", mget },
	{ "bitcount", bitcount },
	{ "setrange", setrange },
	{ "getrange", getrange },
	{ "incr", incr },
	{ "decr", decr },
	{ "incrby", incrby },
	{ "decrby", decrby },
	{ "msetnx", msetnx },
	{ "get", get },
	{ "set", set },
	{ "strlen", strlen },
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
