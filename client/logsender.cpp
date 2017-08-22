#include "logsender.h"
#include "except.h"
#include <cstring>
#include <istream>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
using namespace std;

	char failSendFileName[50];//用于保存发送失败数据的文件
	int fd;//socket描述符，用于通信
	unsigned short port;//服务的的端口
	char serverIp[20];//服务器的Ip

LogSender::LogSender(const char* fileName,
	unsigned short port,const char* ip)
{
	strcpy(failSendFileName,fileName);
	this->port=port;
	strcpy(serverIp,ip);
}
LogSender::~LogSender(){}
void LogSender::sendLog(list<MatchedLogRec>& matches)
throw (DmsSendException)
{
	try
	{
		initNetWork();
		try{
			readSendFailed(matches);
		}catch(ReadSendException& e)
		{
			cout<<e.what()<<endl;
		}
		sendMatches(matches);
		saveSendFailed(matches);
	}
	catch(SocketException& e)
	{
		saveSendFailed(matches);
	}
}
void LogSender::sendMatches(list<MatchedLogRec>& matches)
throw(SendDataException)
{
	cout<<"发送数据"<<endl;
	while(!matches.empty())
	{
		MatchedLogRec log=matches.front();
		log.pid=htonl(log.pid);
		log.logintime=htonl(log.logintime);
		log.logouttime=htonl(log.logouttime);
		log.durations=htonl(log.durations);
		if(-1==send(fd,(char*)&log,sizeof(log),0))
		{
			throw SendDataException("发送数据异常");
		}
		matches.pop_front();
	}
}
void LogSender::initNetWork()throw(SocketException)
{
	cout<<"初始化网络"<<endl;
	fd=socket(AF_INET,SOCK_STREAM,0);
	if (fd==-1)
	{
		throw SocketException("创建网络套接字异常");
	}
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	addr.sin_addr.s_addr=inet_addr(serverIp);
	if(-1==connect(fd,(struct sockaddr*)&addr,sizeof(addr)))
	{
		throw SocketException("连接服务器异常");
	}
}
void LogSender::readSendFailed(list<MatchedLogRec>& matches)
throw (ReadSendException)
{
    cout<<"读取上一次发送失败的记录"<<endl;
    ifstream ifs(failSendFileName,ios::binary);
    if(!ifs)
    {
    	throw ReadSendException("读取上次发送失败记录的异常");
    }
    MatchedLogRec log;
    while(ifs.read((char*)&log,sizeof(log)))
    {
    	matches.push_front(log);
    }
    ifs.close();
    unlink(failSendFileName);
}
void LogSender::saveSendFailed(list<MatchedLogRec>& matches)
throw (SaveSendException)
{
    cout<<"如果发送失败，则保存记录到文件中去"<<endl;
    ofstream ofs(failSendFileName,ios::binary);
    if(!ofs)
    {
    	throw SaveSendException("打开保存发送失败数据的文件的异常");
    }
    list<MatchedLogRec>::iterator it=matches.begin();
    for(;it!=matches.end();it++)
    {
    	if(!ofs.write((char*)&(*it),sizeof(*it)))
    	{
    		throw SaveSendException("发送失败数据时异常");
    	}
    }
    ofs.close();
}

