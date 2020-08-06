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

	char m_buffer[PACKET_SIZE] = {};
	recv(m_client, m_buffer, PACKET_SIZE, 0);

	string str(m_buffer);

	cout << "Recv MSG : " << str << endl;


	char m_msg[] = "Server Send";
	send(m_client, m_msg, strlen(m_msg),0);

	closesocket(m_client);
	closesocket(m_listen);


	WSACleanup();

}

CServer::~CServer()
{
}