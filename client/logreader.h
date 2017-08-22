#ifndef _LOGREADER_H__
#define _LOGREADER_H__

#include "data.h"
#include "except.h"
#include <list>
using namespace std;
class LogReader
{
private:
	char logFileName[50];//需要读取的日志文件
	char backFileName[50];//备份的日志文件
	char failLoginsFileName[50];//匹配失败的登录记录存储的文件
	list<LogRec> logins;//登录记录
	list<LogRec> logouts;//退出记录
	list<MatchedLogRec> matches;//完整的登录记录
public:
	LogReader();
	LogReader(const char* logFile,const char* failLogFile);
	~LogReader();
	list<MatchedLogRec> readLogs()throw (DmsReadException);
	void backup()throw (BackupException);
	void readFailLogins()throw (ReadFailLoginException);
	void readBackupFile()throw (DmsReadException);
	void matchLogRec()throw (MatchLogRecException);
	void saveFailLogins()throw (SaveLoginException);
};


#endif
