#include "CClient.h"
#include <thread>

int ShowMenu();

#define PORT	7272
#define SERVER_IP "127.0.0.1";

bool start = true;

void ProcessSignalAction(int );
void ReciveThread(CClient client);

int main()
{
	
	signal(SIGINT, ProcessSignalAction);

	int menu_index = 0;
	CClient client;
	const char* ip = SERVER_IP;
	const uint16_t port = (uint16_t)PORT;

	client.Connect(ip, port);
	string str = "";

	thread tt = thread(ReciveThread, client);

	while (start)
	{
		cin >> str;
		client.Send(str);
	}

	client.DisConnect();

	return 0;
}
void ReciveThread(CClient client)
{
	cout << "Start :ReciveThread" << endl;
	while (start)
	{
		bool result = client.Recive();
	}
}

void ProcessSignalAction(int sig_number)
{
	if (sig_number == SIGINT)
	{
		start = false;
		/*stopWorking = true;
		mainThreadWorkCount.Notify();*/
	}
}

int ShowMenu()
{
	int menu = 0;
	cout << "-----------Menu-----------" << endl;
	cout << "1. Send Message" << endl;
	cout << "2. Exit" << endl;
	cout << "ют╥б : "; cin >> menu;
	
	return menu;
}
