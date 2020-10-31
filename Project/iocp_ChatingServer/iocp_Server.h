#define WIN32_LEAN_AND_MEAN

#pragma once
#pragma comment(lib, "ws2_32.lib") //������ �ܺ� ���� ���� �߻���
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <winnt.h>
#include <iostream>

#include <chrono>

using namespace std;

class iocp_Server
{

public :
	iocp_Server(int num);
	~iocp_Server();

	void AddSocket(SOCKET& socket, void* userPtr);
	void WaitEvent(int timeouts);
private :
	//�̺�Ʈ�� ���� ���ϸ� ��� �Ǵ°ɱ�?
	static const int MaxEventCount = 100;

	HANDLE m_hIocp; //iocp ������ ���� �ڵ鷯

	int m_ThreadCount; //������ ���� �� ����� �ִ°���?
	int m_EventsCount = 0;
	OVERLAPPED_ENTRY m_Events[MaxEventCount];
};