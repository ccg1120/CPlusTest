
#pragma comment(lib, "ws2_32.lib") //������ �ܺ� ���� ���� �߻���
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
	//�̺�Ʈ�� ���� ���ϸ� ��� �Ǵ°ɱ�?
	HANDLE m_hIocp; //iocp ������ ���� �ڵ鷯
	SOCKET m_ListenSocket;
	SOCKET accept;

	int m_ThreadCount; //������ ���� �� ����� �ִ°���?

	LPFN_ACCEPTEX AcceptEx = NULL;
	WSAOVERLAPPED m_readOverlappedStruct;

	bool m_WorkingState = false;

	vector<SOCKET> m_RemoteClients;

};