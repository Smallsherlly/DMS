#include "client.h"
int main()
{
	Client client("wtmpx","loginfail.dat","sendfail.dat",10087,"127.0.0.1");
	client.sendLogs();
	return 0;
}
