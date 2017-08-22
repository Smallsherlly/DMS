#ifndef _DATA_H__
#define _DATA_H__

#include <iostream>
using namespace std;
struct LogRec
{
	char  logname[32];
	int   pid;
	short logtype;
	int   logtime;
	char  logip[257];

	friend ostream& operator<<(ostream& os,const LogRec& log)
	{
		os << log.logname << " " << log.pid << " " << log.logtype << " "
		<< log.logtime << " " << log.logip << endl;
	}
};

struct MatchedLogRec
{
	char logname[32];
	int  pid;
	int  logintime;
	int  logouttime;
	int  durations;
	char logip[257];

	friend ostream& operator<<(ostream& os,const MatchedLogRec& log)
	{
		os << log.logname << " " << log.pid << " " <<log.logintime << " "
		<< log.logouttime << " " << log.durations << " " 
		<< log.logip << endl;
	}
};


#endif // _DATA_H__
