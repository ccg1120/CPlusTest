#include "CClient.h"

CClient::CClient()
{
	WSADATA wsadata;
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	m_socket = NULL;

	m_WSAError = WSAStartup(wVersionRequested, &wsadata);
	if (m_WSAError != 0)
	{
		std::cout << "WSAStart Failed :"<< m_WSAError << std::endl;
	}
}

CClient::~CClient()
{

}

void CClient::Connect(const char* ip, const uint16_t port)
{
	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//const char* ip = SERVER_IP;

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	m_Endpoint.sin_port = htons(port);
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);

	connect(m_socket, (SOCKADDR*)&m_Endpoint, sizeof(m_Endpoint));
}

void CClient::DisConnect()
{
	if(m_socket != NULL)
	{ 
		closesocket(m_socket);
	}
	if (m_WSAError == 0)
	{
		WSACleanup();
	}
}

bool CClient::Send(string msg)
{
	int result = send(m_socket, msg.c_str(), msg.size() + 1, 0);
	
	if (result > 0)
	{
		return true;
	}
	return false;
}
bool CClient::Recive()
{
	char m_receiveBuffer[MaxReceiveLength];
	int result = recv(m_socket, m_receiveBuffer, MaxReceiveLength, 0);
	if (result == 0)
	{
		cout <<"Connection closed.\n";
		return false;
	}
	else if (result < 0)
	{
		cout << " : result zero.\n";
		return false;
	}
	string str = m_receiveBuffer;
	m_STR.push_back(str);
	cout << "MSG: " << m_receiveBuffer << endl;
	//system("cls");
	return true;
}
