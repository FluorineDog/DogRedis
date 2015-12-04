#incldue "server.h"
const int MAXSIZE = 1024;
int main(){
	int lisFg,connFg;
	socklne_t len;
	struct sockaddr_in sercaddr, cliaddr;
	char buff[MAXSIZE];
	time_t ticks;
	
	// user defined
	short port = 5321;
	
	// appoint protocol type, 0 for default
	lisFg = socket(AF_INET, SOCK_STREAM, 0);
	// init
	bzero(&sercaddr,sizeof(sercaddr));	
	sercaddr.sin_family = AF_INET;
	sercaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sercaddr.sin_port = htonl(port);
	// bind
	bind(lisFg, )
}