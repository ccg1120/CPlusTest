#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define PORT	7272

#define PACKET_SIZE	1024
#define SERVER_IP "127.0.0.1";

class CClient
{
public:
	CClient();
	~CClient();
private:

};


