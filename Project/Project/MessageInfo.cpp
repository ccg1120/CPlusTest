#include "MessageInfo.h"

string MessageInfo::GetReciveTime2String()
{
	//TODO :: return tiem to string
	return "";
}
string MessageInfo::GetSendTiem2String()
{
	//TODO :: return tiem to string
	return "";
}

void MessageInfo::StampSend(time_t t)
{
	m_SendTime = t;
}
void MessageInfo::StampRecive()
{
	//TODO :: Get current server time
}

MessageInfo::MessageInfo()
{
	//TODO :: Time prame set
}

MessageInfo::~MessageInfo()
{
	//TODO :: Time prame delete
}
