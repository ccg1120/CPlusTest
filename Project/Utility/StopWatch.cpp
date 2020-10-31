#include "StopWatch.h"

StopWatch::StopWatch()
{

}

void StopWatch::Start_Clock(KEY key)
{
	if (CheckMapByKey(key))
	{
		Clock c;
		m_ClockMap.insert(make_pair(key, c));
		cout << "Start Clock :" << key << endl;
	}
}
void StopWatch::reset_Clock(KEY key)
{

}
void StopWatch::Stop_Clock(KEY key)
{

}
StopWatch::~StopWatch()
{
	for (auto i : m_ClockMap)
	{
		Stop_Clock(i.first);
	}
	m_ClockMap.clear();
	cout << "Destroy Stopwatch" << endl;
}

bool StopWatch::CheckMapByKey(KEY key)
{
	auto item = m_ClockMap.find(key);

	if (item == m_ClockMap.end())
	{
		return true;
	}
	return false;	
}
Clock StopWatch::GetTimePoint(KEY key)
{	
	auto item = m_ClockMap.find(key);

	if (item == m_ClockMap.end())
	{
		return;
	}

	return item->second;
}

Clock::Clock()
{
	cout << "Clock On" << endl;
	Reset();
}

Clock::~Clock()
{
}
void Clock::Reset()
{
	m_Start = system_clock::now();
}
