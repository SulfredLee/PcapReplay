#include "myTimer.h"

myTimer::myTimer()
{
	m_nTimerDuration = 0;
	m_pParent = 0;
}

myTimer::~myTimer()
{
	EndThread();
}

void myTimer::SetTime(int nMsec, int nID) // msec
{
	m_nTimerDuration = nMsec;
	m_nTimerID = nID;

	myThread::StartThread();
}

void myTimer::TimerCallback()
{

}

void myTimer::Stop_Timer()
{
	myThread::EndThread();
}

void myTimer::ReStart_Timer()
{
	myThread::EndThread();
	myThread::StartThread();
}

//Override
void myTimer::ThreadMain()
{
	m_start = clock();
	while (!IsEndThread())
	{
		double dElapsed = (double)(clock() - m_start) * 1000.0 / CLOCKS_PER_SEC;
		if (dElapsed >= m_nTimerDuration)
		{
			if (m_pParent != 0)
				m_pParent->TimerCallback(m_nTimerID);
			m_start = clock();
		}
		if (dElapsed < 0)
			m_start = clock();
		boost::this_thread::sleep(boost::posix_time::milliseconds(m_nTimerDuration / 2));
	}
}

//myTimer::myTimer()
//{
//	m_timer = 0;
//	m_nTimerDuration = 0;
//}
//
//myTimer::~myTimer()
//{
//	EndThread();
//	m_io.stop();
//}
//
//void myTimer::SetTime(int nMsec, int nID) // msec
//{
//	if (m_timer != 0)
//		delete m_timer;
//	m_nTimerDuration = nMsec;
//	m_nTimerID = nID;
//	m_timer = new boost::asio::deadline_timer(m_io, boost::posix_time::milliseconds(m_nTimerDuration));
//	m_timer->async_wait(boost::bind(&myTimer::TimerCallback, this));
//	
//	myThread::StartThread();
//}
//int g_nCount = 0;
//void myTimer::TimerCallback()
//{
//	if (m_pParent != 0)
//		m_pParent->TimerCallback(m_nTimerID);
//	//m_timer->expires_from_now(boost::posix_time::milliseconds(m_nTimerDuration));
//	m_timer->expires_at(m_timer->expires_at() + boost::posix_time::milliseconds(m_nTimerDuration));
//	m_timer->async_wait(boost::bind(&myTimer::TimerCallback, this));
//	//m_io.run();
//}
//
//void myTimer::Stop_Timer()
//{
//	m_io.stop();
//}
//
//void myTimer::ReStart_Timer()
//{
//	if (m_timer == 0)
//		return;
//	//m_timer->expires_from_now(boost::posix_time::milliseconds(m_nTimerDuration));
//	m_timer->expires_at(m_timer->expires_at() + boost::posix_time::milliseconds(m_nTimerDuration));
//	m_timer->async_wait(boost::bind(&myTimer::TimerCallback, this));
//	myThread::StartThread();
//}
//
////Override
//void myTimer::ThreadMain()
//{
//	while (!IsEndThread())
//		m_io.run();
//}