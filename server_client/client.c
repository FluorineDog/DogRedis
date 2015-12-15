#include <stdio.h>
#include <stdlib.h>
#include "unp.h"
#include "lazy_tcp.h"
void str_cli(FILE *fp, int sockfd){
	char	buf[MAXLINE];
	int 	maxfdp1, stdineof = 0;
	fd_set 	rset;
	int n;
	FD_ZERO(&rset);
	for(;;){
		if(stdineof == 0)
			FD_SET(fileno(fp),&rset);
		FD_SET(sockfd, &rset);
		maxfdp1=max(fileno(fp),sockfd)+1;
		Select(maxfdp1,&rset,NULL,NULL,NULL);
		//fprintf(stdout,">");
		if(FD_ISSET(sockfd,&rset)){
			int n = Read(sockfd,buf,MAXLINE);
			if(n == 0){
				if(stdineof == 1)
					return;
				else
					error_quit("str_cli: server terminated prematurely");		
			}
			buf[n] =  '\0';
			fprintf(stderr,"%s",buf);
			//Fputs(recvline,stdout);// print connect infomation
		}
		//fprintf(stdout,"whatBUG!!!!\n");
		if(FD_ISSET(fileno(fp),&rset)){	//wait for input
			int n = Read(fileno(fp),buf,MAXLINE);
			if(n == 0){
				Shutdown(sockfd, SHUT_WR);
				FD_CLR(fileno(fp),&rset);
				continue;
			}
			buf[n] =  '\0';
			Writen(sockfd,buf,n);
		}
	}
	
}
int main(int argc, char** argv){

	if(argc > 3){
		error_quit("usage: %s [hostname port]\nOR %s <port>", \
			argv[0], argv[0]);
	}
	unsigned short port = (argc > 1 ? atoi(argv[2]) : SERV_PORT);
	char* hostname = (argc == 3 ? argv[1] : NULL);
	
	int sockfd = ConnectToServer(hostname, port);
	str_cli(stdin, sockfd);
	exit(0);
}