
#pragma comment(lib, "ws2_32.lib") //없으면 외부 참조 에러 발생됨
#pragma comment(lib,"mswsock.lib")

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <winnt.h>
#include <iostream>
#include <mswsock.h>
#include <rpc.h>
#include <vector>

#include <chrono>

using namespace std;

class iocp_Server
{

public:
	static const int MaxWaitEventTime = 100;
	static const int MaxEventCount = 100;
	static const int MaxReceiveLength = 8192;

	iocp_Server(const char* ip, const int port, int num);
	~iocp_Server();

	void AddSocket(SOCKET& socket, void* userPtr);
	void WaitEvent(int timeouts);
	void Worker();

	OVERLAPPED_ENTRY m_Events[MaxEventCount];
	int m_EventsCount = 0;
private:
	//이벤트를 지정 안하면 어떻게 되는걸까?
	HANDLE m_hIocp; //iocp 생성을 위한 핸들러
	SOCKET m_ListenSocket;
	SOCKET accept;

	int m_ThreadCount; //스레드 수가 왜 상관이 있는거지?

	LPFN_ACCEPTEX AcceptEx = NULL;
	WSAOVERLAPPED m_readOverlappedStruct;

	bool m_WorkingState = false;

	vector<SOCKET> m_RemoteClients;

};