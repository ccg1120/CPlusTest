#pragma once
#include "iocp_Server.h"

#include <csignal>
#include <iostream>


using namespace std;

void ProcessSignalAction(int );

volatile bool m_WorkingState = true;
//iocp_Server server = iocp_Server("127.0.0.1", 7272, 1);

int main()
{
	signal(SIGINT, ProcessSignalAction);
	
	iocp_Server server = iocp_Server("127.0.0.1", 7272, 1);
	//server.SetWorkingState(m_WorkingState);
	server.Worker();

	cout << "End..." << endl;
	return 0;
}

void ProcessSignalAction(int sig_number)
{
	cout << "Process End Signal" << endl;
	if (sig_number == SIGINT)
	{
		cout << "Process End Signal" << endl;
		m_WorkingState = false;
	}
}