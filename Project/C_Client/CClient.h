#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <signal.h>

#include <iostream>
#include <stdio.h>
#include <string>


#pragma comment(lib, "ws2_32.lib")


#define PACKET_SIZE	1024


using namespace std;

class CClient
{
public:
	void Connect(const char* ip, const uint16_t port);
	void DisConnect();
	bool Send(string msg);

	CClient();
	~CClient();
private:
	SOCKET m_socket;
	int  m_WSAError;
};


