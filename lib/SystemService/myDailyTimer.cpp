#include "myDailyTimer.h"

myDailyTimer::myDailyTimer(int nTimerID, myDailyTimerInterface* pParent)
{
	m_nTimerID = nTimerID;
	m_pParent = pParent;
	StartThread();
}

myDailyTimer::~myDailyTimer()
{
	m_pParent = 0;
	EndThread();
}

void myDailyTimer::SetTime(tm strIN)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_tmSpecificTime = strIN;
	m_strSpecificTime = MakeStringSpecificTime(m_tmSpecificTime);
	if(IsEndThread())
		StartThread();
}

void myDailyTimer::StopTimer()
{
	EndThread();
}

//Override
void myDailyTimer::ThreadMain()
{
	while (!IsEndThread())
	{
		if (IsTimeOut())
		{
			if (m_pParent != 0)
				m_pParent->TimerCallback(m_nTimerID);
			//the_mutex.lock();
			//AddOneDay(m_tmSpecificTime); // we don't need to add one day, since we only compare the hour min sec
			//the_mutex.unlock();
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
	}
}

bool myDailyTimer::IsTimeOut()
{
	boost::mutex::scoped_lock lock(the_mutex);
	time_t nowtime, schedularTime;
	struct tm *nowtm;
	time(&nowtime);
	nowtm = localtime(&nowtime);
	// updat nowtm, since we only compare hour min sec
	nowtm->tm_isdst = m_tmSpecificTime.tm_isdst;
	nowtm->tm_mday = m_tmSpecificTime.tm_mday;
	nowtm->tm_mon = m_tmSpecificTime.tm_mon;
	nowtm->tm_wday = m_tmSpecificTime.tm_wday;
	nowtm->tm_yday = m_tmSpecificTime.tm_yday;
	nowtm->tm_year = m_tmSpecificTime.tm_year;
	nowtime = mktime(nowtm);
	schedularTime = mktime(&m_tmSpecificTime); // time is GMT local
	//schedularTime += 10; // add 10 sec for a buffer
	if (difftime(nowtime, schedularTime) == 0)
		return true;
	else
		return false;
}

std::string myDailyTimer::MakeStringSpecificTime(tm tmIN)
{
	time_t localTime;
	localTime = mktime(&tmIN); // time is GMT local
	struct tm * ptm = gmtime(&localTime); // convert time to GMT +0
	char veccNextTime[64];
	memset(veccNextTime, 0, sizeof(veccNextTime));
	sprintf(veccNextTime, "%d-%02d-%02d %02d:%02d:%02d.000",
		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	std::string strTemp(veccNextTime);
	return strTemp;
}

void myDailyTimer::RestartTimer()
{
	if (IsEndThread())
		StartThread();
}

void myDailyTimer::AddOneDay(tm& tmIN)
{
	tmIN.tm_mday += 1;
	mktime(&tmIN); /* normalize result */
}
//myDailyTimer::myDailyTimer()
//{
//	m_pTimer = 0;
//	m_strSpecificTime = "";
//}
//
//myDailyTimer::~myDailyTimer()
//{
//	EndThread();
//	if (m_pTimer != 0)
//	{
//		m_pTimer->cancel();
//		delete m_pTimer;
//	}
//	m_IO.reset();
//	//boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//	m_IO.stop();
//	m_IO.~io_service();
//
//	int i = 0;
//	i++;
//}
//
//void myDailyTimer::SetTime(tm tmIN, int nID) // msec
//{
//	m_tmSpecificTime = tmIN;
//	m_strSpecificTime = MakeStringSpecificTime();
//	m_nTimerID = nID;
//	if (m_pTimer != 0)
//	{
//		//m_pTimer->cancel();
//		//delete m_pTimer;
//		m_pTimer->expires_at(boost::posix_time::time_from_string(m_strSpecificTime));
//	}
//	else
//	{
//		m_pTimer = new boost::asio::deadline_timer(m_IO, boost::posix_time::time_from_string(m_strSpecificTime));
//	}
//	m_pTimer->async_wait(boost::bind(&myDailyTimer::TimerCallback, this));
//	
//	myThread::StartThread();
//}
//
//std::string myDailyTimer::MakeStringSpecificTime()
//{
//	time_t localTime;
//	localTime = mktime(&m_tmSpecificTime); // time is GMT local
//	struct tm * ptm = gmtime(&localTime); // convert time to GMT +0
//	char veccNextTime[64];
//	memset(veccNextTime, 0, sizeof(veccNextTime));
//	sprintf(veccNextTime, "%d-%02d-%02d %02d:%02d:%02d.000",
//		ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
//		ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
//	std::string strTemp(veccNextTime);
//	return strTemp;
//}
//
//void myDailyTimer::AddOneDay()
//{
//	m_tmSpecificTime.tm_mday += 1;
//	mktime(&m_tmSpecificTime); /* normalize result */
//}
//
//void myDailyTimer::TimerCallback()
//{
//	if (m_pParent != 0)
//		m_pParent->TimerCallback(m_nTimerID);
//	//m_timer->expires_from_now(boost::posix_time::milliseconds(m_nTimerDuration));
//	AddOneDay();
//	m_strSpecificTime = MakeStringSpecificTime();
//	m_pTimer->expires_at(boost::posix_time::time_from_string(m_strSpecificTime));
//	m_pTimer->async_wait(boost::bind(&myDailyTimer::TimerCallback, this));
//}
//
////Override
//void myDailyTimer::ThreadMain()
//{
//	while (!IsEndThread())
//		m_IO.run();
//	int j = 0;
//	j++;
//}
//
//void myDailyTimer::StopTimer()
//{
//	if (m_pTimer != 0)
//	{
//		m_pTimer->cancel();
//	}
//}