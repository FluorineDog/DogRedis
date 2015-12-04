#ifndef DOG_SERVER_H
#define DOG_SERVER_H
#include "common.h"
/*
struct sockaddr
{
	unsigned short sa_family; // Address family tag.
	char sa_data[14]; // Padding.
};
struct sockaddr_in
{
	unsigned short sin_family; // Set to AF_INET.
	unsigned short sin_port; // Port number to bind to.
	struct in_addr sin_addr; // IP address.
	char sin_zero[8]; // Padding.
};
struct in_addr
{
	unsigned long s_addr; // IP address.
};
*/
void HandleConnection(int connect_fd);
int CreateListenfd(unsigned short port);


bool enableLock();
bool unlock();
#endif 	//DOG_SERVER_H