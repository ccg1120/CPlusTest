#include "main.h"
#include "CServer.h"

using namespace std;

int main()
{
	//printf("TEST");

	cout << "START SERVER" << endl;

	CServer server;
	cout << "Connect SERVER" << endl;
	server.StartConnect();
	
	int a;

	return 0;
}

