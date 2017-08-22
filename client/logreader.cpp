#include "logreader.h"
#include <time.h>
#include <cstring>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define LOG_REC_SIZE 372
using namespace std;
LogReader::LogReader(){}
LogReader::LogReader(const char* logFile,const char* failLogFile)
{
    strcpy(logFileName,logFile);
    strcpy(failLoginsFileName,failLogFile);
}
LogReader::~LogReader(){}
list<MatchedLogRec>& LogReader::readLogs()throw (DmsReadException)
{
    try
    {
        backup();
		try{
        	readFailLogins();
		}catch(ReadFailLoginException & e)
		{
			cout << e.what() << endl;
		}
        readBackupFile();
        matchLogRec();
        saveFailLogins();
    }catch(SaveLoginException &e)
    {
 //       strcpy(failLoginsFileName,"fileLogin.dat");
        saveFailLogins();
    }
    catch(DmsReadException &e)
    {
        cout<<e.what()<<endl;
    }
    return matches;
}
void LogReader::backup()throw (BackupException)
{
    cout<<"备份日志文件"<<endl;
    //system("cp file1 file2");
    //rename();
    time_t now=time(NULL);
    struct tm *local=localtime(&now);
    sprintf(backFileName,"%04d-%02d-%02d-%02d-%02d-%02d.dat",
        local->tm_year+1900,local->tm_mon+1,local->tm_mday,
        local->tm_hour,local->tm_min,local->tm_sec);
    if(rename(logFileName,backFileName)!=0)
    {
        throw BackupException("备份日志文件异常");
    }

}
void LogReader::readFailLogins()throw (ReadFailLoginException)
{
    cout<<"读取上一次未匹配成功的日志"<<endl;
    ifstream ifs(failLoginsFileName,ios::binary);
    LogRec log;
    if (!ifs)  
    {
        throw ReadFailLoginException("读取上一次未匹配成功的日志异常");
        return;
    }
    while(ifs.read((char*)&log,sizeof(log)))
    {
        logins.push_back(log);
    }
    ifs.close();
}
void LogReader::readBackupFile()throw (DmsReadException)
{
    cout<<"读取备份日志文件"<<endl;
    struct stat st;
    stat(backFileName,&st);
    size_t recoders=st.st_size/LOG_REC_SIZE;
    ifstream ifs(backFileName,ios::binary);
    if (!ifs)  
    {
        throw DmsReadException("读取备份日志文件异常");
        return;
    }
    for (size_t i = 0; i < recoders; i++)
    {
        ifs.seekg(i*LOG_REC_SIZE,ios::beg);

        char logname[32]="";
        ifs.read(logname,sizeof(logname));
        ifs.seekg(36,ios::cur);

        int pid;
        ifs.read((char*)&pid,sizeof(pid));

        short type;
        ifs.read((char*)&type,sizeof(type));
        ifs.seekg(6,ios::cur);

        int logtime;
        ifs.read((char*)&logtime,sizeof(logtime));
        ifs.seekg(30,ios::cur);

        char ip[257];
        ifs.read(ip,sizeof(ip));

        if(logname[0]!='.')
        {
            LogRec log;
            strcpy(log.logname,logname);
            log.pid=ntohl(pid);
            log.logtype=ntohs(type);
            log.logtime=ntohl(logtime);
            strcpy(log.logip,ip);
            //cout<<log;
            if (log.logtype==7)
            {
                logins.push_back(log);
            }
            else if (log.logtype==8)
            {
                logouts.push_back(log);
            }
        }
    }
    ifs.close();
}
void LogReader::matchLogRec()throw (MatchLogRecException)
{
    cout<<"匹配登录记录"<<endl;
    list<LogRec>::const_iterator it=logouts.begin();
    for (;it!=logouts.end();it++)
    {
        list<LogRec>::iterator iter=logins.begin();
        for (;iter!=logins.end();iter++)
        {
            if (strcmp(it->logname,iter->logname)==0&&
                it->pid==iter->pid&&
                strcmp(it->logip,iter->logip)==0)
            {
                MatchedLogRec matchlog;
                strcpy(matchlog.logname,it->logname);
                strcpy(matchlog.logip,it->logip);
                matchlog.pid=it->pid;
                matchlog.logintime=iter->logtime;//iter登录的时间
                matchlog.logouttime=it->logtime;
                matchlog.durations=it->logtime-iter->logtime;

                //cout<<matchlog;
                matches.push_back(matchlog);
                logins.erase(iter);
                break;
            }
        }
    }
}
void LogReader::saveFailLogins()throw (SaveLoginException)
{
    cout<<"保存未能匹配成功的登录记录"<<endl;
    ofstream ofs(failLoginsFileName,ios::binary);
    if(!ofs)
    {
        throw SaveLoginException("保存未能匹配成功的登录记录异常");
    }
    list<LogRec>::iterator it=logins.begin();
    for(;it!=logins.end();it++)
    {
        ofs.write((char*)&(*it),sizeof(*it));
    }
    ofs.close();
}


