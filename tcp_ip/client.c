#include "client.h"
// Sample wrapper from the notes -- connects to a given server.
int ConnectToServer(char* hostname, unsigned short port);

int main(int argc, char* argv[])
{
	int fd;
	unsigned short port;
	char buf[1024];
	int nRead;
	// Check if enough arguments were passed in.
	if (argc < 3)
	{
		// Not enough! Complain.
		fprintf(stderr, "%s: Not enough arguments!\n", argv[0]);
		fprintf(stderr, "Syntax: %s hostname port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// argv[2] needs to be parsed to get the port.
	port = (unsigned short) atoi(argv[2]);
	// Connect to server -- argv[1] contains the hostname.
	fd = ConnectToServer(argv[1], port);
	if (fd == -1)
	{
		perror("Could not connect to server!");
		exit(EXIT_FAILURE);
	}
	// Get some input from the user.
	printf("> ");
	fgets(buf, sizeof(buf) - 1,stdin);
	// Send everything in the buffer to the server.
	printf("Dog 1 is opening \n%s\n",buf);
	
	write(fd, buf, sizeof(buf));
	// Read the response from the server.
	
	nRead = ReadString(fd, buf, sizeof buf);
	
	if (nRead > 0)
		printf("Length of string is: %s\n", buf);
	else
		perror("Something went wrong talking to server!");
	// Clean up and exit.
	close(fd);
	return 0;
}
/**
* Reads data from the file descriptor until it reaches a NUL.
*
* @param fd File descriptor to read from.
* @param buf Buffer to write data to.
* @param n Size of buffer.
*
* @return Length of string read, or -1 if error.
*/
int ReadString(int fd, char* buf, int n)
{
	int nTtlRead = 0;
	int nRead = 0;
	while (nTtlRead < n)
	{
		printf("Count %d\n", nTtlRead);
		// Read a character at a time, and check for error...
		if (read(fd, buf, 1) < 0)
		{
			perror("Error reading!");
			break;
		}
		// Have we reached the end?
		printf("ascii value %d \n ",(int)*buf);
		if (*buf == '\0')
			break;
		// Record amount read and move buffer on.
		nTtlRead += 1;
		buf += 1;
	}
	return nTtlRead;
}
/**
* Creates a socket and connects to the specified server.
*
* @param hostname Host server is running on.
* @param port Port server is bound to.
*
* @return File descriptor for connected socket, or -1 if failed.
*/
int ConnectToServer(char* hostname, unsigned short port)
{
	int fd;
	struct hostent* info;
	struct sockaddr_in addr;
	// Get the server’s details.
	info = gethostbyname(hostname);
	if (info == NULL)
	{
		perror("Host not found.");
		return -1;
	}
	// Get the server’s address.
	addr.sin_family = info->h_addrtype;
	addr.sin_port = htons(port);
	memcpy((void*) &addr.sin_addr, info->h_addr_list[0], info->h_length);
	// Create a socket.
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		perror("Could not open socket.");
		return -1;
	}
	// Try to establish connection to server.
	if (connect(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
	{
		perror("Could not connect to server.");
		close(fd);
		return -1;
	}
	return fd;
}