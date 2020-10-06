#pragma once

#include<string>
using namespace std;

class LoginInfo
{
public:
	LoginInfo();
	LoginInfo(string ip, string location);
	~LoginInfo();

private:
	string m_Ip;
	string m_Location;

};

