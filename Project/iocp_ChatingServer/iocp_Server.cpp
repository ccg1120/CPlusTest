
#include "iocp_Server.h"

iocp_Server::iocp_Server(int num)
{
	this->m_ThreadCount = num;
	//https://docs.microsoft.com/en-us/windows/win32/fileio/createiocompletionport
	//매개변수에 대한 설명이 있음
	// I/O완료 포드에 대한 핸들이 반환
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_ThreadCount);

	cout << "iocp Server Create" << endl;
}

iocp_Server::~iocp_Server()
{
	cout << "iocp Server Close" << endl;
	CloseHandle(m_hIocp);
}

void iocp_Server::AddSocket(SOCKET& socket, void* userPtr)
{
	if (!CreateIoCompletionPort((HANDLE)socket, m_hIocp, (ULONG_PTR)userPtr, m_ThreadCount))
		throw exception("IOCP add failed!");
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
