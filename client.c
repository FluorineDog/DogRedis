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
void str_cli(FILE *fp, int sockfd){
	int 	maxfdp1;
	fd_set 	rset;
	char	sendline[MAXLINE], recvline[MAXLINE];//serve as buf
	FD_ZERO(&rset);
	for(;;){
		FD_SET(fileno(fp),&reset);
		FD_SET(sockfd, &rset);
		maxfdp1=max(fileno(fp),sockfd)+1;
		select(max,&rset,NULL,NULL,NULL);
		if(FD_ISSET(sockfd,&rset)){
			if(readline(sockfd,recvline,MAXLINE) == 0){
				err_quit("str_cli: server terminated prematurely");		
			}
			fputs(recvline,stdout);// print connect infomation
		}
		if(FD_ISSET(fileno(fp),&rset)){
			if(FD_ISSET(fileno(fp),&rset)){
				return;
			}
			Writen(sockfd,sendline,strlen(sendline));
		}
	}
}
int main(int argc, char** argv){
	int i,sockfd[5];
	struct sockaddr_in servaddr;
	if(argc != 3){
		err_quit("usage: redis_cli <hostname> <port>");
	}
	int fd = ConnectToServer(argv[1],(unsigned short)atoi(argv[2]);
	str_cli(stdin, fd);
	exit(0);
}