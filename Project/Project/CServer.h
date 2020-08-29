#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <iostream>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib") //������ �ܺ� ���� ���� �߻���

#define PORT	7272
#define PACKET_SIZE	1024
#define SERVER_IP "127.0.0.1"

class  CServer
{
public:
	static const int MaxReceiveLength = 8192;

	 CServer();
	~CServer();
	void StartConnect();
private:

};
