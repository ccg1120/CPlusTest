#include "main.h"
#include "CServer.h"

using namespace std;

int main()
{
	int m_MaxUser = 2;
	//printf("TEST");

	cout << "START SERVER" << endl;

	CServer server;
	const char* ip = SERVER_IP;
	const uint16_t port = (uint16_t)PORT;
	server.Listen(ip, port);

	for (size_t i = 0; i < m_MaxUser; i++)
	{
		server.Accept();
	}
	server.ShowData();

	return 0;
}

