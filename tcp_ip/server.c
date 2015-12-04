#incldue "server.h"
const int MAXSIZE = 1024;
const int LISTENQ = 16;
typedef struct sockaddr SA;
void function(int operation);

int main(int argc, char* argv){
	unsigned short port = 12355;
	int listen_fg = CreateListenFg(port);
	while(true){
		socklne_t len;
		struct sockaddr_in client_addr;
		int connect_fd = accept( listen_fg,(SA*)&client_addr, &len);
		int pid = fork();
		if(pid == 0){
			// in child
			close(listen_fg);	
			HandleConnection(connect_fd);
			close(connect_fd);
			exit(0);	// exit child
		}
		else
			close(connect_fd);	// parent 
	}
}
int CreateListenFg(unsigned short port){
	int listen_fg;
	struct sockaddr_in sercaddr
	// user defined
	// appoint protocol type, 0 for default
	listen_fg = socket(AF_INET, SOCK_STREAM, 0);
	// init
	bzero(&sercaddr,sizeof(sercaddr));	
	sercaddr.sin_family = AF_INET;
	sercaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sercaddr.sin_port = htonl(port);
	// bind
	bind(listen_fg,(SA*)&sercaddr, sizeof(servaddr) );
	// listen
	listen(listen_fg, LISTENQ);
	return listen_fg;
}
void HandleConnection(int connect_fd){
	while(true){
		
	}
}