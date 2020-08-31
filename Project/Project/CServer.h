#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib") //없으면 외부 참조 에러 발생됨


#define PACKET_SIZE	1024


using namespace std;

struct Client;

	
class  CServer
{
public:
	void Listen(const char* ip, const uint16_t port);
	void Accept();
	void DisConnect();
	void ClientWorkThread(Client& client);
	void ShowData();
	string GetAcceptIP(const SOCKET& acceptsocket);

	 CServer();
	~CServer();
private:
	static const int MaxReceiveLength = 8192;
	bool m_WorkingSignal = false;
	SOCKET m_Socket;
	vector<Client> m_AcceptSocketList;
	thread* m_ListenThread;
	int m_ClientCount = 0;
};

struct Client
{
	int num;
	thread m_Thread;
	SOCKET m_Socket;
};