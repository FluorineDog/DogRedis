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
int ConnectToServer(char* hostname, unsigned short port)
{
	int fd;
	struct hostent* info;
	struct sockaddr_in addr;
	// Get the server’s details.
	info = gethostbyname(hostname);
	if (info == NULL)
	{
		err_quit("Host not found.");
	}
	// Get the server’s address.
	addr.sin_family = info->h_addrtype;
	addr.sin_port = htons(port);
	memcpy((void*) &addr.sin_addr, info->h_addr, info->h_length);
	// Create a socket.
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		err_quit("Could not open socket.")
	}
	// Try to establish connection to server.
	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		close(fd);
		err_quit("Could not connect to server.");
	}
	return fd;

#endif //DOG_LAZY_TCP_H_