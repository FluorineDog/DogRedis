#include <stdio.h>
#include <stdlib.h>
#include "unp.h"
#include "lazy_tcp.h"
void str_cli(FILE *fp, int sockfd){
	char	sendline[MAXLINE], recvline[MAXLINE];//serve as buf
	int 	maxfdp1;
	fd_set 	rset;
	FD_ZERO(&rset);
	for(;;){
		FD_SET(fileno(fp),&rset);
		FD_SET(sockfd, &rset);
		maxfdp1=max(fileno(fp),sockfd)+1;
		Select(maxfdp1,&rset,NULL,NULL,NULL);
		fprintf(stdout,"*");
		if(FD_ISSET(sockfd,&rset)){
			if(Readline(sockfd,recvline,MAXLINE) == 0){
				error_quit("str_cli: server terminated prematurely");		
			}
			fprintf(stdout,"\n<%s>\n",recvline);
			//Fputs(recvline,stdout);// print connect infomation
		}
		fprintf(stdout,"whatBUG!!!!\n");
		if(FD_ISSET(fileno(fp),&rset)){	//wait for input
			if(Fgets(sendline,MAXLINE,fp)==NULL){
				return;
			}
			Writen(sockfd,sendline,strlen(sendline));
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