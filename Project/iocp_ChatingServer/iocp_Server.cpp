
#include "iocp_Server.h"

iocp_Server::iocp_Server(const char* ip, const uint16_t port, int num)
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

	//WSASTARTUP을 안해서 실행이 안되던 문제 기본에 충실할것 

	this->m_ThreadCount = num;
	//https://docs.microsoft.com/en-us/windows/win32/fileio/createiocompletionport
	//매개변수에 대한 설명이 있음
	// I/O완료 포드에 대한 핸들이 반환
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_ThreadCount);

	//m_ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in m_Endpoint;
	m_Endpoint.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &m_Endpoint.sin_addr);
	m_Endpoint.sin_port = htons(port);

	int bindresult = bind(m_ListenSocket, (sockaddr*)&m_Endpoint, sizeof(m_Endpoint));
	cout << "bind Shcek :" << bindresult << endl;

	listen(m_ListenSocket, 5000);
	
	AddSocket(m_ListenSocket, nullptr);

	//accept = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	accept = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (AcceptEx == NULL)
	{
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
	}

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
	int errcode = WSAGetLastError();

	if (!ret && (errcode  != ERROR_IO_PENDING))
	{
		cout << "Error ??" << endl;
	}
	cout << "iocp Server Create : " << ret << endl;
}

iocp_Server::~iocp_Server()
{
	cout << "iocp Server Close" << endl;
	closesocket(accept);
	closesocket(m_ListenSocket);
	CloseHandle(m_hIocp);
	WSACleanup();
}

void iocp_Server::AddSocket(SOCKET& socket, void* userPtr)
{
	HANDLE port = CreateIoCompletionPort((HANDLE)socket, m_hIocp, (ULONG_PTR)0, m_ThreadCount);
	
	if (!port)
	{
		cout << "Exception !!" << endl;
		auto err = GetLastError();

		if (err != ERROR_SUCCESS)
		{
			//fprintf(err, "err %d\n", err);
			cout << err << endl;
		}
		throw exception("IOCP add failed!");
	}
}
void iocp_Server::WaitEvent(int timeouts)
{
	// https://docs.microsoft.com/en-us/windows/win32/fileio/getqueuedcompletionstatusex-func

	
	BOOL check = GetQueuedCompletionStatusEx(m_hIocp, m_Events, MaxEventCount, (ULONG*)m_EventsCount, timeouts, FALSE);
	if (!check)
	{
		//이벤트가 없어서 0으로 초기화 v
		m_EventsCount = 0;
	}
	else
	{
		cout << "wait event Event count : " << m_EventsCount << endl;
	}
}
void iocp_Server::WaitEvent(OVERLAPPED_ENTRY* array ,int timeouts)
{
	// https://docs.microsoft.com/en-us/windows/win32/fileio/getqueuedcompletionstatusex-func


	BOOL check = GetQueuedCompletionStatusEx(m_hIocp, array, MaxEventCount, (ULONG*)m_EventsCount, timeouts, FALSE);
	if (!check)
	{
		//이벤트가 없어서 0으로 초기화 v
		m_EventsCount = 0;
	}
	else
	{
		cout << "wait event Event count : " << m_EventsCount << endl;
	}
}

void iocp_Server::SetWorkingState(bool ison)
{
	m_WorkingState = ison;
	cout << "Worker State Change : " << m_WorkingState << endl;	
}
void iocp_Server::Worker()
{

	while (m_WorkingState)
	{
		OVERLAPPED_ENTRY events[MaxEventCount];
		cout << "working..." << endl;
		//WaitEvent(MaxWaitEventTime);
		WaitEvent(events, MaxWaitEventTime);
		cout << "working...end wait :" << m_EventsCount << endl;

		for (size_t i = 0; i < m_EventsCount; i++)
		{
			OVERLAPPED_ENTRY readEvent = events[i];
			if (readEvent.lpCompletionKey == 0) //리슨소켓 
			{
				cout << "listen Socket" << endl;
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

				int setresult = setsockopt(accept, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
					(char*)&m_ListenSocket, sizeof(m_ListenSocket));
				

				if (setresult != 0) //setresult 이 에러 일 경우
				{
					closesocket(m_ListenSocket);
				}
				else
				{
					cout << "accept process..." << endl;
					SOCKET remote = SOCKET(accept);

					AddSocket(remote, &remote);
					char recevebuffer[MaxReceiveLength];

					WSABUF b;
					b.buf = recevebuffer;
					b.len = MaxReceiveLength;
					DWORD m_readFlags = 0;

					int wsarresult = WSARecv(remote, &b, 1, NULL, &m_readFlags, &m_readOverlappedStruct, NULL);
					if (wsarresult == 0)
					{
						m_RemoteClients.push_back(remote);

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

					cout << "accept On!" << endl;
				}

			}
			else //tcp 소켓
			{

			}
		}
	}

	
}
