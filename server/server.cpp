#include "server.h"
#include "data.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

Server::Server(const char* ip,unsigned short port)
{
	strcpy(labIp,ip);
	this->port = port;
}
Server::~Server(){}

void Server::initSocket()
{
	fd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(labIp);
	int ret = bind(fd,(struct sockaddr*)&addr,sizeof(addr));
	if(-1 == ret)
	{
		cout << "bind failed" << endl;
	}
	listen(fd,10);
//	struct sockaddr_in addr2;
	socklen_t len = sizeof(addr);
	fd = accept(fd,(struct sockaddr*)&addr,&len);
	MatchedLogRec log;
	while(recv(fd,(char*)&log,sizeof(log),0)>0)
	{
	log.pid = ntohs(log.pid);
	log.logintime = ntohl(log.logintime);
	log.logouttime = ntohl(log.logouttime);
	log.durations = ntohl(log.durations);
	cout << log << endl;
	}
}

int main()
{
	Server server("127.0.0.1",10087);
	server.initSocket();
}

