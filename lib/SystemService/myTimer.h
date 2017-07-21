#pragma once
#include <vector>
#include <set>
#include <time.h>

#include "myThread.h"

//#include <boost/asio.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>

class myTimerInterface
{
public:
	myTimerInterface(){}
	~myTimerInterface(){}

	virtual void TimerCallback(int nTimerID) = 0;
};
class myTimer :
	public myThread
{
public:
	int m_nTimerDuration; // msec
	int m_nTimerID;
	myTimerInterface* m_pParent;
protected:
	clock_t m_start;
public:
	myTimer();
	~myTimer();

	void SetTime(int nMsec, int nID); // msec
	void TimerCallback();

	void Stop_Timer();
	void ReStart_Timer();
	//Override
	void ThreadMain();
};

//class myTimer :
//	public myThread
//{
//public:
//	boost::asio::io_service m_io;
//	boost::asio::deadline_timer * m_timer;
//	int m_nTimerDuration;
//	int m_nTimerID;
//	//std::set<myTimerInterface*> m_lstObserver;
//	myTimerInterface* m_pParent;
//public:
//	myTimer();
//	~myTimer();
//
//	void SetTime(int nMsec, int nID); // msec
//	void TimerCallback();
//
//	void Stop_Timer();
//	void ReStart_Timer();
//	//Override
//	void ThreadMain();
//};