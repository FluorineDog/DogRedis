#incldue "server.h"
const int MAXSIZE = 1024;
const int LISTENQ = 16;
typedef struct sockaddr SA;
void function(int operation);

int main(int argc, char* argv){
	unsigned short port = 12355;
	int listen_fd = CreateListenfd(port);
	while(true){
		socklne_t len;
		struct sockaddr_in client_addr;
		int connect_fd = accept( listen_fd,(SA*)&client_addr, &len);
		int pid = fork();
		if(pid == 0){
			// in child
			close(listen_fd);	
			HandleConnection(connect_fd);
			close(connect_fd);
			exit(0);	// exit child
		}
		else
			close(connect_fd);	// parent 
	}
}
int CreateListenfd(unsigned short port){
	int listen_fd;
	struct sockaddr_in sercaddr
	// user defined
	// appoint protocol type, 0 for default
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	// init
	bzero(&sercaddr,sizeof(sercaddr));	
	sercaddr.sin_family = AF_INET;
	sercaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sercaddr.sin_port = htonl(port);
	// bind
	bind(listen_fd,(SA*)&sercaddr, sizeof(servaddr) );
	// listen
	listen(listen_fd, LISTENQ);
	return listen_fd;
}

char msg[30];
void HandleConnection(int connect_fd){
	int op_id;
	while(true){
		if(read(connect_fd, (char*)&op_id, 4)>=0)
		{
			
			break;
		}
		op_id = htonl(op_id);
		if( op_id > sizeof(operationList))
		while(enableLock())
			delay(5);
		operationList[op_id].func();	//
		unlock();
	}
}