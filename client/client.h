#ifndef _CLIENT_H__
#define _CLIENT_H__

#include "logreader.h"
#include "logsender.h"


class Client
{
private:
	LogReader logReader;
	LogSender logSender;
public:
	Client(const char* wtm,const char* failfile,const char* sendfail,unsigned short port,const char* ip):logReader(wtm,failfile),logSender(sendfail,port,ip){}
	void sendLogs();

};



#endif // _CLIENT_H__
