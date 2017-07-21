#pragma once
#include "Common.h"

#include "myDailyTimer.h"

#include <vector>
#include <string>
#include <time.h>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class PcapReplayCtrl;
class AutoDriver :
	public myDailyTimerInterface
{
public:
	enum TimerType
	{
		DAILY_TIMER
	};
	PcapReplayCtrl* m_pPcapReplayCtrl;
protected:
	mutable boost::mutex the_mutex;
	tm m_schedularDateTime;
	bool m_bOneTimeOnly;
	bool m_bSchularEnable;
	std::vector<bool> m_vecWeekDaysChecked; // from monday to sunday
	bool m_bStartTimer;
	myDailyTimer* m_pmyDailyTimer;
public:
	AutoDriver();
	~AutoDriver();

	// Get and Set
	void SetSchedularDateTime(const tm& schedularDateTime);
	void SetOneTimeOnly(const bool& bOneTimeOnly);
	void SetSchularEnable(const bool& bSchularEnable);
	void SetWeekDayChecked(const std::vector<bool>& vecWeekDaysChecked);
	tm GetSchedularDateTime();
	bool GetOneTimeOnly();
	bool GetSchularEnable();
	std::vector<bool> GetWeekDayChecked();

	void StartAutoDrive();
	
	
	// override
	void TimerCallback(int nTimerID);
protected:
	void DailyTimeOut();
	bool IsTimeExpired();
};