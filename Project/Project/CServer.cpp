#include "CServer.h"

bool CServer::m_WorkingSignal;
void static TEST22()
{
	cout << "TEST..." << endl;
}
void CServer::Listen(const char* ip, const uint16_t port)
{
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_Socket == -1)
	{
		std::cout << "Server Socket Error" << std::endl;
		return;
	}
	std::cout << "Server Socket Succece" << std::endl;

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);
	m_Endpoint.sin_port = htons(port);

	int bindresult = bind(m_Socket, (sockaddr*)&m_Endpoint, sizeof(m_Endpoint));
	if (bindresult < 0)
	{
		std::cout << "Bind Failed" << std::endl;
		return;
	}

	listen(m_Socket, 5000);
	std::cout << "SERVER Listen" << std::endl;
	m_WorkingSignal = true;
	m_ListenThread = thread(
		[&]() {
		Accept();
		});
	m_ListenThread.join();

}

void CServer::Accept()
{
	std::cout << "Accept Start !!" << std::endl;
	while (m_WorkingSignal)
	{
		SOCKET acceptsocket = accept(m_Socket, NULL, 0);
		if (acceptsocket == -1)
		{
			std::cout << "Accept Socket Error" << std::endl;
			return;
		}
		m_ClientCount++;

		std::cout << "Accept !!!" << std::endl;
		Client* client = new  Client;
		client->m_Socket = acceptsocket;
		client->num = m_ClientCount;
		client->m_Thread = thread(ClientWorkThread, client);
		
		//client->m_Thread.join();
		m_AcceptSocketList.push_back(client);
	}	
}

void CServer::StaticTest(int num)
{
	cout << "StaticTest : " << num << endl;

}
void  CServer::ClientWorkThread(Client* client)
{
	char m_receiveBuffer[MaxReceiveLength];

	while (m_WorkingSignal)
	{
		string receivedData;
		int result = (int)recv(client->m_Socket, m_receiveBuffer, MaxReceiveLength, 0);

		if (result == 0)
		{
			cout << client->num << "Connection closed.\n";
			break;
		}
		else if (result < 0)
		{
			cout << client->num << " : result zero.\n";
			break;
		}

		cout << client->num << " : Received = " << m_receiveBuffer << endl;
	}
	closesocket(client->m_Socket);
}
string CServer::GetAcceptIP(const SOCKET& acceptsocket)
{
	sockaddr_in ret;
	ret.sin_family = AF_INET;
	socklen_t retLength = sizeof(ret);

	std::cout << "retLength  : " << retLength << std::endl;

	getpeername(acceptsocket, (sockaddr*)&ret, &retLength);

	char addrString[1000];
	addrString[0] = 0;
	inet_ntop(AF_INET, &ret.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];
	//sprintf(finalString, "%s:%d", addrString, htons(ret.sin_port));
	sprintf_s(finalString, "%s:%d", addrString, htons(ret.sin_port));
	std::cout << "Socket from  : " << finalString << std::endl;
	string result = finalString;

	return result;

}


void CServer::ShowData()
{
	//char m_receiveBuffer[MaxReceiveLength];

	//while (m_WorkingSignal)
	//{
	//	for (size_t i = 0; i < m_AcceptSocketList.size(); i++)
	//	{
	//		string receivedData;
	//		cout << "Receiving data...\n";
	//		int result = (int)recv(m_AcceptSocketList[i], m_receiveBuffer, MaxReceiveLength, 0);

	//		if (result == 0)
	//		{
	//			cout << "Connection closed.\n";
	//			break;
	//		}
	//		else if (result < 0)
	//		{
	//			cout << "Connect lost: " << endl;
	//			closesocket(m_AcceptSocketList[i]);
	//			m_AcceptSocketList.erase(m_AcceptSocketList.begin()+i);
	//			break;
	//		}
	//		// 수신된 데이터를 꺼내서 출력합니다. 송신자는 "hello\0"을 보냈으므로 sz가 있음을 그냥 믿고 출력합니다.
	//		// (실전에서는 클라이언트가 보낸 데이터는 그냥 믿으면 안됩니다. 그러나 여기는 독자의 이해를 위해 예외로 두겠습니다.)
	//		cout << "Received: " << m_receiveBuffer << endl;
	//	}
	//}
}


void CServer::DisConnect()
{
	cout << "Server DisConnect" << endl;
	closesocket(m_Socket);
	WSACleanup();
}
CServer::CServer()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err = 0;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//WSAStartup Error
		std::cout << "WSAStartup failed with error : " << err << std::endl;
		return;
	}
}
CServer::~CServer()
{
	
	//delete m_ListenThread;
}

