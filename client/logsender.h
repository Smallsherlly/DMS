#ifndef _LOGSENDER_H__
#define _LOGSENDER_H__

#include "data.h"
#include "except.h"
#include <list>

using namespace std;
class LogSender
{
private:
	char failSendFileName[50];//用于保存发送失败数据的文件
	int fd;//socket描述符，用于通信
	unsigned short port;//服务的的端口
	char serverIp[20];//服务器的Ip
public:
	LogSender(const char* fileName,
	unsigned short port,const char* ip);
	~LogSender();
	void sendLog(list<MatchedLogRec>& matches)
	throw (DmsSendException);
private:
	void sendMatches(list<MatchedLogRec>& matches)
	throw (SendDataException);
	void initNetWork()throw (SocketException);
	void readSendFailed(list<MatchedLogRec>& matches)
	throw (ReadSendException);
	void saveSendFailed(list<MatchedLogRec>& matches)
	throw (SaveSendException);
};


#endif
