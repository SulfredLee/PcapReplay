#pragma once
#include "myThread.h"

#include <time.h>
#include <string>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class myDailyTimerInterface
{
public:
	myDailyTimerInterface(){}
	~myDailyTimerInterface(){}

	virtual void TimerCallback(int nTimerID) = 0;
};
class myDailyTimer :
	public myThread
{
public:
	
protected:
	int m_nTimerID;
	myDailyTimerInterface* m_pParent;
	tm m_tmSpecificTime;
	std::string m_strSpecificTime;
	mutable boost::mutex the_mutex;
public:
	myDailyTimer(int nTimerID, myDailyTimerInterface* pParent);
	~myDailyTimer();

	void SetTime(tm strIN);
	void RestartTimer();
	void StopTimer();
	//Override
	void ThreadMain();
protected:
	std::string MakeStringSpecificTime(tm tmIN);
	void AddOneDay(tm& tmIN);
	bool IsTimeOut();
};
//#include <vector>
//#include <set>
//#include <string>
//#include <time.h>
//
//#include "myThread.h"
//
//#include <boost/asio.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//
//class myDailyTimerInterface
//{
//public:
//	myDailyTimerInterface(){}
//	~myDailyTimerInterface(){}
//
//	virtual void TimerCallback(int nTimerID) = 0;
//};
//class myDailyTimer :
//	public myThread
//{
//public:
//	boost::asio::io_service m_IO;
//	boost::asio::deadline_timer * m_pTimer;
//	tm m_tmSpecificTime;
//	std::string m_strSpecificTime;
//	int m_nTimerID;
//	myDailyTimerInterface* m_pParent;
//public:
//	myDailyTimer();
//	~myDailyTimer();
//
//	void SetTime(tm strIN, int nID); // msec
//	void StopTimer();
//	void TimerCallback();
//	
//	//Override
//	void ThreadMain();
//protected:
//	std::string MakeStringSpecificTime();
//	void AddOneDay();
//};