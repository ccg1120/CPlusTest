#pragma once

#include <string>
#include "LoginInfo.h"

using namespace std;

class UserInfo
{
public:

	UserInfo();

	UserInfo(string name, bool push);
	UserInfo(string name, bool push, string ip, string location);
	~UserInfo();
private:
	string m_Name;
	bool m_PushRegist = false;
	LoginInfo m_LastLoginInfo;
};

