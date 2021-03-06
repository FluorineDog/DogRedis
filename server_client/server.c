#include "unp.h"
#include "../cmd/exec_func.h"
int SetUp(unsigned short port){
	int listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	memset(&servaddr,0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	Bind(listenfd, (SA*)&servaddr, sizeof(servaddr));
	Listen(listenfd,LISTENQ);
	fprintf(stderr,"set up server. Port %d",(int)port);
	return listenfd;
}

int main(int argc,char *argv[]){
	char input_buf[MAXLINE];
	char output_buf[MAXLINE];
	unsigned short port;
	fd_set rset, allset;
	int client[FD_SETSIZE];
	if(argc == 1)
		port = SERV_PORT;
	else if(argc == 2)
		port = (unsigned short)atoi(argv[1]);
	else error_quit("usage: redis_server [port]");
	//set up listenfd
	int listenfd = SetUp(port);
	// initialize
	int maxfd = listenfd;
	int maxi = -1;
	for(int i=0; i < FD_SETSIZE; ++i){
		client[i]= -1;
	}
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);	// add listenfd to allset
	for(;;){
		// round begin : set up if new client request connection
		rset = allset;
		int nready = Select(maxfd + 1, &rset, NULL,NULL,NULL);
		if(FD_ISSET(listenfd, &rset)){	// new client connection
			struct sockaddr_in cliaddr;
			socklen_t clilen = sizeof(cliaddr);
			int connfd = Accept(listenfd, (SA*)&cliaddr, &clilen);
			int i=0;
			for(; i<FD_SETSIZE;++i){
				if(client[i] < 0){		// find empty slot for new client
					client[i]=connfd;
					break;
				}
			}
			if(i == FD_SETSIZE){
				error_quit("too many clients");
			}
			FD_SET(connfd, &allset);	// add new descriptor to set
			if(connfd>maxfd)			// for select
				maxfd = connfd;			
			if(i>maxi)
				maxi = i; 				// max index in client[]
			if(--nready<=0)				// exclude listenfd and check
				continue;				// no client
		}
		// second half of the round
		// deal with all cilent
		for(int i = 0; i<= maxi; ++i){
			int sockfd = client[i];
			if(sockfd < 0)continue;
			
			if(FD_ISSET(sockfd, &rset)){
				int nStr = Read(sockfd, input_buf, MAXLINE);
				input_buf[nStr]='\0';
				if(nStr == 0){			// connection closed by client
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				else{
					int n = exec(output_buf, input_buf);
					fprintf(stdout,"%s",input_buf,output_buf);
					Write(sockfd, output_buf, n);
				}
				if(--nready<=0){
					break;
				}
			}
		}
	}
}