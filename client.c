#include "unp.h"
#include "lazy_tcp.h"
void str_cli(FILE *fp, int sockfd){
	int 	maxfdp1;
	fd_set 	rset;
	char	sendline[MAXLINE], recvline[MAXLINE];//serve as buf
	FD_ZERO(&rset);
	for(;;){
		FD_SET(fileno(fp),&rset);
		FD_SET(sockfd, &rset);
		maxfdp1=max(fileno(fp),sockfd)+1;
		Select(maxfdp1,&rset,NULL,NULL,NULL);
		if(FD_ISSET(sockfd,&rset)){
			if(Readline(sockfd,recvline,MAXLINE) == 0){
				error_quit("str_cli: server terminated prematurely");		
			}
			Fputs(recvline,stdout);// print connect infomation
		}
		if(FD_ISSET(fileno(fp),&rset)){	//wait for input
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
		error_quit("usage: %s <hostname> <port>", argv[0]);
	}
	int fd = ConnectToServer(argv[1],(unsigned short)atoi(argv[2]));
	str_cli(stdin, fd);
	exit(0);
}