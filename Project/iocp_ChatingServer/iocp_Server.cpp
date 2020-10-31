
#include "iocp_Server.h"

iocp_Server::iocp_Server(int num)
{
	this->m_ThreadCount = num;
	//https://docs.microsoft.com/en-us/windows/win32/fileio/createiocompletionport
	//�Ű������� ���� ������ ����
	// I/O�Ϸ� ���忡 ���� �ڵ��� ��ȯ
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
		//�̺�Ʈ�� ��� 0���� �ʱ�ȭ 
		m_EventsCount = 0;
	}
}
