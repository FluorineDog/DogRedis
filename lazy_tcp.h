#ifndef DOG_LAZY_TCP_H_
#define DOG_LAZY_TCP_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "unp.h"
int ConnectToServer(char* hostname, unsigned short port)
{
	int fd;
	struct hostent* info;
	struct sockaddr_in addr;
	// Get the server’s details.
	info = gethostbyname(hostname);
	if (info == NULL)
	{
		error_quit("Host not found.");
	}
	fprintf(stderr, "999999\n");
	// Get the server’s address.
	addr.sin_family = info->h_addrtype;
	addr.sin_port = htons(port);
	memcpy((void*) &addr.sin_addr, info->h_addr_list[0], info->h_length);
	// Create a socket.
	
	fprintf(stderr, "7777777\n");
	fd = socket(AF_INET, SOCK_STREAM, 0);
	fprintf(stdout, "88888888\n");
	if (fd == -1)
	{
		error_quit("Could not open socket.");
	}
	fprintf(stderr, "44xxx444444\n");
	// Try to establish connection to server.
	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		close(fd);
		error_quit("Could not connect to server.");
	}
	fprintf(stderr, "44xxx444444\n");
	return fd;
}
#endif //DOG_LAZY_TCP_H_