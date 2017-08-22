#ifndef _EXCEPT_H__
#define _EXCEPT_H__

#include <exception>
#include <cstring>
using namespace std;

class DmsClientException:public exception
{
public:
	DmsClientException(const char* str="客户端异常")
	{
		strcpy(msg,str);
	}
	virtual const char* what()
	{
		return msg;
	}
	~DmsClientException()throw(){}
private:
	char msg[200];
};


class DmsReadException:public DmsClientException
{
public:
	DmsReadException(const char* str="读文件异常")
	:DmsClientException(str){}
	//~DmsReadException();
	
};

class BackupException:public DmsReadException
{
public:
	BackupException(const char* str="备份异常")
	:DmsReadException(str){}
	//~BackupException();
	
};

class ReadFailLoginException:public DmsReadException
{
public:
	ReadFailLoginException(const char* str="读取为匹配登录记录异常")
	:DmsReadException(str){}
	//~ReadFailLoginException();
	
};

class MatchLogRecException:public DmsReadException
{
public:
	MatchLogRecException(const char* str="备份异常")
	:DmsReadException(str){}
	//~MatchLogRecException();
	
};

class DmsSendException:public DmsClientException
{
public:
	DmsSendException(const char* str="发送文件异常")
	:DmsClientException(str){}
	//~DmsSendException();
	
};

class SendDataException:public DmsSendException
{
public:
	SendDataException(const char* str="发送数据异常")
	:DmsSendException(str){}
	//~SendDataException();
	
};

class SaveLoginException:public DmsReadException
{
public:
	SaveLoginException(const char* str="保存为匹配登录的记录异常")
	:DmsReadException(str){}
	//~SaveLoginException();
	
};

class ReadSendException:public DmsSendException
{
public:
	ReadSendException(const char* str="读取未发送文件异常")
	:DmsSendException(str){}
	//~ReadSendException();
	
};

class SaveSendException:public DmsSendException
{
public:
	SaveSendException(const char* str="保存未发送文件异常")
	:DmsSendException(str){}
	//~SaveSendException();
	
};

class SocketException:public DmsSendException
{
public:
	SocketException(const char* str="网络异常")
	:DmsSendException(str){}
	//~SocketException();
	
};

#endif
