#include "AutoDriver.h"
#include "PcapReplayCtrl.h"

AutoDriver::AutoDriver()
{
	m_pPcapReplayCtrl = 0;
	m_pmyDailyTimer = 0;
}

AutoDriver::~AutoDriver()
{
}


void AutoDriver::SetSchedularDateTime(const tm& schedularDateTime)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_schedularDateTime = schedularDateTime;
}

void AutoDriver::SetOneTimeOnly(const bool& bOneTimeOnly)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bOneTimeOnly = bOneTimeOnly;
}

void AutoDriver::SetSchularEnable(const bool& bSchularEnable)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bSchularEnable = bSchularEnable;
}

void AutoDriver::SetWeekDayChecked(const std::vector<bool>& vecWeekDaysChecked)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_vecWeekDaysChecked = vecWeekDaysChecked;
}

tm AutoDriver::GetSchedularDateTime()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_schedularDateTime;
}

bool AutoDriver::GetOneTimeOnly()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bOneTimeOnly;
}

bool AutoDriver::GetSchularEnable()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bSchularEnable;
}

std::vector<bool> AutoDriver::GetWeekDayChecked()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_vecWeekDaysChecked;
}

// override
void AutoDriver::TimerCallback(int nTimerID)
{
	switch (nTimerID)
	{
	case DAILY_TIMER:
		DailyTimeOut();
		break;
	default:
		break;
	}
}

void AutoDriver::DailyTimeOut()
{
	if (m_bOneTimeOnly)
	{
		if (IsTimeExpired())
		{
			//if (m_pmyDailyTimer != 0)
			//{
			//	delete m_pmyDailyTimer;
			//	m_pmyDailyTimer = 0;
			//}
			//boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			if (m_pPcapReplayCtrl != 0)
			{
				m_pPcapReplayCtrl->StartPlayFromAutoDriver();
			}
		}
		
		//if (m_pmyDailyTimer != 0)
		//{
		//	m_pmyDailyTimer->StopTimer();
		//	delete m_pmyDailyTimer;
		//	m_pmyDailyTimer = 0;
		//}
	}
	else
	{
		time_t nowtime;
		struct tm *nowtm;
		time(&nowtime);
		nowtm = localtime(&nowtime);
		std::vector<bool> vecTempWeekCheck = GetWeekDayChecked();
		int nIdx = (nowtm->tm_wday + 6) % 7; // convert to since Monday [0,6]
		if (vecTempWeekCheck[nIdx])
		{
			if (m_pPcapReplayCtrl != 0)
			{
				m_pPcapReplayCtrl->StartPlayFromAutoDriver();
			}
			m_pmyDailyTimer->StopTimer();
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			m_pmyDailyTimer->RestartTimer();
		}
	}
	
}

void AutoDriver::StartAutoDrive()
{
	if (m_pmyDailyTimer != 0)
	{
		//delete m_pmyDailyTimer;
		//m_pmyDailyTimer = 0;
		
	}
	else
	{
		m_pmyDailyTimer = new myDailyTimer(DAILY_TIMER, this);
		
	}
	m_pmyDailyTimer->SetTime(m_schedularDateTime);
}

bool AutoDriver::IsTimeExpired()
{
	time_t nowtime, schedularTime;
	struct tm *nowtm;
	time(&nowtime);
	nowtm = localtime(&nowtime);
	schedularTime = mktime(&m_schedularDateTime); // time is GMT local
	//schedularTime += 10; // add 10 sec for a buffer
	//if (difftime(nowtime, schedularTime) < 0)
	//	return false;
	//else
	//	return true;

	if (difftime(nowtime, schedularTime) == 0)
		return true;
	else
		return false;
}