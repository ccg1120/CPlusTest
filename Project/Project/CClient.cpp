#include "CClient.h"



CClient::CClient()
{
	WSADATA wsadata;
	int startstate = WSAStartup(MAKEWORD(2,2), &wsadata);

	SOCKET m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN m_add = {};
	m_add.sin_family = AF_INET;
	m_add.sin_port = htons(PORT);
	m_add.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

	connect(m_socket, (SOCKADDR*)&m_add, sizeof(m_add));

	char msg[] = "Client Send";
	send(m_socket, msg, strlen(msg),0);

	char buffer[PACKET_SIZE] = {};

	recv(m_socket, buffer, PACKET_SIZE, 0);

	std::cout << "Recv MSG : " << buffer << std::endl;
	closesocket(m_socket);
		
	WSACleanup();
}

CClient::~CClient()
{
}