#ifndef _SERVER_H__
#define _SERVER_H__

class Server
{
private:
	int fd;
	char labIp[20];
	unsigned short port;
public:
	Server(const char* ip,unsigned short port);
	~Server();
	void initSocket();
};



#endif // _SERVER_H__

