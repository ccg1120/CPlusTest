#include "CServer.h"


using namespace std;

CServer::CServer()
{
	
}

void CServer::StartConnect()
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

	SOCKET socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket == -1)
	{
		std::cout << "Server Socket Error" << std::endl;
		return;
	}
	std::cout << "Server Socket Succece" << std::endl;
	sockaddr_in m_Endpoint;

	m_Endpoint.sin_family = AF_INET;
	const char* ip = SERVER_IP;
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);
	m_Endpoint.sin_port = htons((uint16_t)PORT);

	int bindresult = bind(socket, (sockaddr*)&m_Endpoint, sizeof(m_Endpoint));
	if (bindresult < 0)
	{
		std::cout << "Bind Failed" << std::endl;
		return;
	}

	listen(socket, 5000);

	SOCKET acceptsocket = accept(socket, NULL, 0);
	if (acceptsocket == -1)
	{
		std::cout << "Accept Socket Error" << std::endl;
		return;
	}

	sockaddr_in ret;
	ret.sin_family = AF_INET;

	socklen_t retLength = sizeof(ret);

	std::cout << "retLength  : " << retLength << std::endl;

	getpeername(acceptsocket, (sockaddr*)&ret, &retLength);

	char addrString[1000];
	addrString[0] = 0;
	inet_ntop(AF_INET, &ret.sin_addr, addrString, sizeof(addrString) - 1);

	char finalString[1000];
	sprintf(finalString, "%s:%d", addrString, htons(ret.sin_port));

	std::cout << "Socket from  : " << finalString << std::endl;

	char m_receiveBuffer[MaxReceiveLength];

	while (true)
	{
		string receivedData;
		cout << "Receiving data...\n";
		int result = (int)recv(acceptsocket, m_receiveBuffer, MaxReceiveLength, 0);

		if (result == 0)
		{
			cout << "Connection closed.\n";
			break;
		}
		else if (result < 0)
		{
			//cout << "Connect lost: " << GetLastErrorAsString() << endl;
		}

		// 수신된 데이터를 꺼내서 출력합니다. 송신자는 "hello\0"을 보냈으므로 sz가 있음을 그냥 믿고 출력합니다.
		// (실전에서는 클라이언트가 보낸 데이터는 그냥 믿으면 안됩니다. 그러나 여기는 독자의 이해를 위해 예외로 두겠습니다.)
		cout << "Received: " << m_receiveBuffer << endl;
	}

	closesocket(socket);

	WSACleanup();
}

CServer::~CServer()
{
}