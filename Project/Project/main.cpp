#include "main.h"
#include "CServer.h"

using namespace std;

void Test()
{
	while (true)
	{
		cout << "Thread TEST...." << endl;
	}
}

class TestThread
{
private:
	thread* tt;
	int num;
	static void Show()
	{
		cout << "Static Class Thread..." << endl;
	}
	void Show2()
	{
		while (true)
		{
			cout << num <<" : Class Thread..." << endl;
		}
	}

public:
	TestThread(int num)
	{
		cout << "init!!" << endl;
		this->num = num; 
		tt = new thread([&] {
			Show2();
			});

		tt->join();	
	}

	~TestThread()
	{
		delete tt;
	}
		
	
};
void TestThreadMember(int num)
{
	cout << "Thread Test : " << num << endl;
}
int main()
{
	int m_MaxUser = 2;

	cout << "START SERVER" << endl;

	CServer server;
	const char* ip = SERVER_IP;
	const uint16_t port = (uint16_t)PORT;
	server.Listen(ip, port);

	//for (size_t i = 0; i < m_MaxUser; i++)
	//{
	//	server.Accept();
	//}
	//server.ShowData();

	return 0;
}

