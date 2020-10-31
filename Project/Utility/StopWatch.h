#pragma once

#include <chrono>
#include <iostream>
#include <unordered_map>

using namespace std;
using namespace chrono;

typedef int KEY;

class Clock;

class StopWatch
{

public:
	StopWatch();
	~StopWatch();
	void Start_Clock(KEY key);
	void reset_Clock(KEY key);
	void Stop_Clock(KEY key);
private:
	unordered_map<KEY, Clock> m_ClockMap;
	bool CheckMapByKey(KEY key);
	Clock GetTimePoint(KEY key);
};

class Clock
{
public :
	Clock();
	~Clock();
	void Reset();
private:
	system_clock::time_point m_Start;
	//null 대입이 되지 않아 클래스로 한번 감싸서 사용
	
};