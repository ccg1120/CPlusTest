#include "UserInfo.h"


UserInfo::UserInfo()
{

}
UserInfo::UserInfo(string name, bool push) : m_Name(name), m_PushRegist(push)
{}

UserInfo::UserInfo(string name, bool push, string ip, string location) : m_Name(name), m_PushRegist(push), m_LastLoginInfo(ip, location)
{
}

UserInfo::~UserInfo()
{
	
}