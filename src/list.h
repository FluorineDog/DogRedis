#ifndef DOG_LIST_H_
#define DOG_LIST_H_
#include "common.h" 
DOG_CMD_FUNC_DEF(blpop);
DOG_CMD_FUNC_DEF(lrange);
DOG_CMD_FUNC_DEF(brpop);
DOG_CMD_FUNC_DEF(lrem);
DOG_CMD_FUNC_DEF(brpoplpush);
DOG_CMD_FUNC_DEF(lset);
DOG_CMD_FUNC_DEF(lindex);
DOG_CMD_FUNC_DEF(ltrim);
DOG_CMD_FUNC_DEF(linsert);
DOG_CMD_FUNC_DEF(rpop);
DOG_CMD_FUNC_DEF(llen);
DOG_CMD_FUNC_DEF(rpoplpush);
DOG_CMD_FUNC_DEF(lpop);
DOG_CMD_FUNC_DEF(rpush);
DOG_CMD_FUNC_DEF(lpush);
DOG_CMD_FUNC_DEF(rpushx);
DOG_CMD_FUNC_DEF(lpushx);
#endif // DOG_LIST_H_
