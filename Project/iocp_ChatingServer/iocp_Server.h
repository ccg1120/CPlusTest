#define WIN32_LEAN_AND_MEAN

#pragma once
#pragma comment(lib, "ws2_32.lib") //없으면 외부 참조 에러 발생됨
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
	//이벤트를 지정 안하면 어떻게 되는걸까?
	static const int MaxEventCount = 100;

	HANDLE m_hIocp; //iocp 생성을 위한 핸들러

	int m_ThreadCount; //스레드 수가 왜 상관이 있는거지?
	int m_EventsCount = 0;
	OVERLAPPED_ENTRY m_Events[MaxEventCount];
};