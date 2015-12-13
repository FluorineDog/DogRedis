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
	
	if(hostname){
		info = gethostbyname(hostname);
	}
	else{
		info = gethostent();
	}
	if(!info){
		error_quit("Host Not Found");
	}
	// Get the server’s address.
	addr.sin_family = info->h_addrtype;
	addr.sin_port = htons(port);
	memcpy((void*) &addr.sin_addr, \
		info->h_addr_list[0], info->h_length);
	// Create a socket
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		error_quit("Could not open socket.");
	}
	// Try to establish connection to server.
	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		close(fd);
		error_quit("Could not connect to server.");
	}
	
	fprintf(stdout,\
		"Successfully Connected Host at Port %d\n",(int)port);
	
	return fd;
}
#endif //DOG_LAZY_TCP_H_