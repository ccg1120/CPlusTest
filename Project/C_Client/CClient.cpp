#include "CClient.h"

CClient::CClient()
{
	WSADATA wsadata;
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0)
	{
		std::cout << "WSAStart Failed :"<< err << std::endl;
	}

	SOCKET m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	const char* ip = SERVER_IP;

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	m_Endpoint.sin_port = htons((uint16_t)PORT);
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);

	connect(m_socket, (SOCKADDR*)&m_Endpoint, sizeof(m_Endpoint));

	char msg[] = "Client Send";
	send(m_socket, msg, strlen(msg)+ 1,0);

	char buffer[PACKET_SIZE] = {};

	//recv(m_socket, buffer, PACKET_SIZE, 0);

	std::cout << "Recv MSG : " << buffer << std::endl;
	closesocket(m_socket);
		
	WSACleanup();
}

CClient::~CClient()
{

}