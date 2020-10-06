#pragma once

#include <time.h>
#include <string>
using namespace std;

class MessageInfo
{
public:
	MessageInfo();
	~MessageInfo();

	void StampRecive();
	void StampSend(time_t t);

	string GetReciveTime2String();
	string GetSendTiem2String();
private:
	time_t m_SendTime;
	time_t m_ReciveTime;
};

