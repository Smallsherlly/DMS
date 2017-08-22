#include "client.h"
#include "except.h"

void Client::sendLogs()
{
	try
	{	
		list<MatchedLogRec> match=logReader.readLogs();
		logSender.sendLog(match);
	}catch(DmsClientException& e)
	{
		cout << e.what() << endl;
	}
}
