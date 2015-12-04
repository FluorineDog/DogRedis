#include "server.h"
/* vim:set ts=4 sw=4 noai sr sta et cin:
* simpleserver.c
* by Keith Gaughan <kmgaughan@eircom.net>
*
* Simple server implementing the ‘String Length’ protocol.
*
* Copyright (c) Keith Gaughan, 2003.
* This software is free; you can redistribute it and/or modify it
* under the terms of the Design Science License (DSL). If you didn’t
* recieve a copy of the DSL with this software, one can be obtained
* at <http://www.dsl.org/copyleft/dsl.txt>.
*/

#define QUEUE_SIZE 5
int CreateTCPServerSocket(unsigned short port, int nQueued);
void DispatchIncoming(int fdQueue); // From earlier in the notes.
void HandleConnection(int fd);
int ReadString(int fd, char* buf, int n); // From client.c
int main(int argc, char* argv[])
{
	int port;
	int fdQueue;
	// Check if enough arguments were passed in.
	if (argc < 2)
	{
		// Not enough! Complain.
		fprintf(stderr, "%s: Not enough arguments!\n", argv[0]);
		fprintf(stderr, "Syntax: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// argv[1] needs to be parsed to get the port.
	port = (unsigned short) atoi(argv[1]);
	// Initialise server.
	fdQueue = CreateTCPServerSocket(port, QUEUE_SIZE);
	if (fdQueue == -1)
		exit(EXIT_FAILURE);
	// Enter connection dispatch loop.
	DispatchIncoming(fdQueue);
	return 0;
}
/**
* Opens a socket for the server to listen for connections on.
*
* @param port Port to bind socket to.
* @param nQueued Maximum number of queued requests to allow.
*
* @return Socket to wait for new connections on.
*/
int CreateTCPServerSocket(unsigned short port, int nQueued)
{
	int fd;
	struct sockaddr_in addr;
	// Create a socket.
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("Could not create socket.");
		return -1;
	}
	// Build address.
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	// Bind to any of the server’s addresses.
	if (bind(fd, (struct sockaddr*) &addr, sizeof addr) == -1)
	{
		perror("Could not bind socket.");
		close(fd);
		return -1;
	}
	// Listen for request and allow up to nQueued outstanding.
	if (listen(fd, nQueued) == -1)
	{
		perror("listen() failed.");
		close(fd);
		return -1;
	}
	return fd;
}
/**
* Handle’s communications with a client.
*/
void HandleConnection(int fd)
{
	int nRead;
	char buf[1024];
	// Get any incoming data for processing.
	nRead = ReadString(fd, buf, sizeof buf);
	// With nRead, we’ve effectively got the length of the string,
	// so write that out.
	sprintf(buf, "%d", nRead);
	// Send the data back to the client.
	printf("serverbuf: %s\n",buf);
	write(fd, buf, strlen(buf)+1);
	// Clean up!
	close(fd);
}

int ReadString(int fd, char* buf, int n)
{
	int nTtlRead = 0;
	int nRead = 0;
	while (nTtlRead < n)
	{
		// Read a character at a time, and check for error...
		if (read(fd, buf, 1) < 0)
		{
			perror("Error reading!");
			return -1;
		}
		// Have we reached the end?
		if (*buf == '\0')
		break;
		// Record amount read and move buffer on.
		nTtlRead += 1;
		buf += 1;
	}
	return nTtlRead;
}
/**
* Waits on and dispatches any incoming client connections to a
* handler.
*
* @param fdQueue File descriptor of socket to wait on.
*/
void DispatchIncoming(int fdQueue)
{
	int fd;
	for (;;)
	{
		// Fetch a new connection off the queue.
		fd = accept(fdQueue, NULL, NULL);
		if (fd == -1)
		{
			// Oh dear! Something’s gone wrong! Whimper and die.
			perror("Could not accept incoming connection.");
			exit(EXIT_FAILURE);
		}
		// Got a connection! Handle it.
		HandleConnection(fd);
	}
}
/**
* Binds a socket to a given port.
*
* @param fd File descriptor of socket to bind.
* @param port Port number to bind to.
*
* @return 0 if successful, -1 for failure.
*/

int BindSocket(int fd, unsigned short port)
{
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; // Let the host decide.
	addr.sin_port = htons(port); // Port to bind to.
	return bind(fd, (struct sockaddr*) &addr, sizeof(addr));
}