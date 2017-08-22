#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "data.h"
#include "logsender.h"
using namespace std;
int main()
{
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8888);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(sockfd,(struct sockaddr*)&addr,sizeof(addr));
	listen(sockfd,50);
	socklen_t len=sizeof(addr);
	int fd=accept(sockfd,(struct sockaddr*)&addr,&len);
	while(1)
	{
		MatchedLogRec log;
		if(0==recv(fd,(char*)&log,sizeof(log),0))
		{
			cout<<"end!"<<endl;
			break;
		}
		log.pid=htonl(log.pid);
		log.logintime=htonl(log.logintime);
		log.logouttime=htonl(log.logouttime);
		log.durations=htonl(log.durations);
		cout<<log;
	}
}
