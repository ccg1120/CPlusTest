#include "CServer.h"

using namespace std;

CServer::CServer()
{
	WSADATA wsa_data;
	int startupstate = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	
	cout << "START State " << startupstate << endl;

	SOCKET m_listen;
	m_listen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN m_listenaddr = {};
	m_listenaddr.sin_family = AF_INET;
	m_listenaddr.sin_port = htons(PORT);
	m_listenaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(m_listen, (SOCKADDR*)&m_listenaddr, sizeof(m_listenaddr));
	listen(m_listen, SOMAXCONN);

	SOCKADDR_IN m_Clientaddr = {};

	int iclintsize = sizeof(m_Clientaddr);
	SOCKET m_client = accept(m_listen, (SOCKADDR*)&m_Clientaddr, &iclintsize);



	WSACleanup();

}

CServer::~CServer()
{
}