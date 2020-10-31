
#include "iocp_Server.h"

iocp_Server::iocp_Server(const char* ip, const int port, int num)
{
	this->m_ThreadCount = num;
	//https://docs.microsoft.com/en-us/windows/win32/fileio/createiocompletionport
	//매개변수에 대한 설명이 있음
	// I/O완료 포드에 대한 핸들이 반환
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_ThreadCount);

	m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);
	m_Endpoint.sin_port = htons(port);

	int bindresult = bind(m_ListenSocket, (sockaddr*)&m_Endpoint, sizeof(m_Endpoint));

	listen(m_ListenSocket, 5000);
	
	AddSocket(m_ListenSocket, nullptr);

	accept = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);


	DWORD bytes;
	WSAIoctl(m_ListenSocket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&UUID(WSAID_ACCEPTEX),
		sizeof(UUID),
		&AcceptEx,
		sizeof(AcceptEx),
		&bytes,
		NULL,
		NULL);

	char ignored[200];
	DWORD ignored2 = 0;

	bool ret = AcceptEx(m_ListenSocket,
		accept,
		&ignored,
		0,
		50,
		50,
		&ignored2,
		&m_readOverlappedStruct
	) == TRUE;

	//m_ListenSocket
	cout << "iocp Server Create" << endl;
}

iocp_Server::~iocp_Server()
{
	cout << "iocp Server Close" << endl;
	CloseHandle(m_hIocp);
}

void iocp_Server::AddSocket(SOCKET& socket, void* userPtr)
{
	HANDLE port = CreateIoCompletionPort((HANDLE)socket, m_hIocp, (ULONG_PTR)1233, m_ThreadCount);
	if (!port)
	{
		cout << "Exception !!" << endl;
		auto err = GetLastError();

		throw exception("IOCP add failed!");
	}
}
void iocp_Server::WaitEvent(int timeouts)
{
	// https://docs.microsoft.com/en-us/windows/win32/fileio/getqueuedcompletionstatusex-func

	
	BOOL check = GetQueuedCompletionStatusEx(m_hIocp, m_Events, MaxEventCount, (ULONG*)m_EventsCount, timeouts, FALSE);
	if (!check)
	{
		//이벤트가 없어서 0으로 초기화 
		m_EventsCount = 0;
	}
}

void iocp_Server::Worker()
{

	while (m_WorkingState)
	{
		WaitEvent(MaxWaitEventTime);

		for (size_t i = 0; i < MaxEventCount; i++)
		{
			OVERLAPPED_ENTRY readEvent = m_Events[i];
			if (readEvent.lpCompletionKey == 0) //리슨소켓 
			{
				//이그노어는 뭘까?
				sockaddr_in ignore1;
				sockaddr_in ignore3;
				INT ignore2, ignore4;

				char ignore[1000];

				GetAcceptExSockaddrs(ignore,
					0,
					50,
					50,
					(sockaddr**)&ignore1,
					&ignore2,
					(sockaddr**)&ignore3,
					&ignore4);

				int setresult = 0;/* setsockopt(accept, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
					(char*)&m_ListenSocket, sizeof(m_ListenSocket));*/
				

				if (setresult != 0) //setresult 이 에러 일 경우
				{
					closesocket(m_ListenSocket);
				}
				else
				{
					SOCKET* remote = new SOCKET(accept);

					AddSocket(*remote, &remote);
					char recevebuffer[MaxReceiveLength];

					WSABUF b;
					b.buf = recevebuffer;
					b.len = MaxReceiveLength;
					DWORD m_readFlags = 0;

					int wsarresult = WSARecv(*remote, &b, 1, NULL, &m_readFlags, &m_readOverlappedStruct, NULL);
					if (wsarresult == 0)
					{
						m_RemoteClients.push_back(*remote);

						cout << "Remote Push !" << endl;
					}
					accept = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

					DWORD bytes;
					WSAIoctl(m_ListenSocket,
						SIO_GET_EXTENSION_FUNCTION_POINTER,
						&UUID(WSAID_ACCEPTEX),
						sizeof(UUID),
						&AcceptEx,
						sizeof(AcceptEx),
						&bytes,
						NULL,
						NULL);

					char ignored[200];
					DWORD ignored2 = 0;

					bool ret = AcceptEx(m_ListenSocket,
						accept,
						&ignored,
						0,
						50,
						50,
						&ignored2,
						&m_readOverlappedStruct
					) == TRUE;
				}

			}
			else //tcp 소켓
			{

			}
		}
	}

	
}
