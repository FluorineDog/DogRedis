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
}