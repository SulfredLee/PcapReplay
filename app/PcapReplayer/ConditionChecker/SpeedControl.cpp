#include "SpeedControl.h"
#include "NetworkOutput.h"
#include "PcapReplayCtrl.h"

SpeedControl::SpeedControl()
{
	m_FirstPcapTime = 0;
	m_RelativeTime = 0;
	m_speedFactor = 0;
	m_unTimerCount = 0;
	m_bFixSpeed = false;
	SetArrivalTime(0);

	m_myTimer.SetTime(100, Timer_100); // timer callback for every 0.1 sec
	//m_myTimer.m_lstObserver.insert(this);
	m_myTimer.m_pParent = this;
}


SpeedControl::~SpeedControl()
{
}

//Override
void SpeedControl::ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime)
{
	SetArrivalTime(arrivalTime);

	if (!m_bFixSpeed)
	{
		if (m_bRegularReplayEnable)
		{
			while (!IsCanSend_RegularReplay(arrivalTime))
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}
		else
		{
			while (!IsCanSend(arrivalTime))
			{
				if (GetFirstPcapTime() == 0)
					SetFirstPcapTime(arrivalTime);
				boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			}
		}
	}
	
	

	for (std::set<NetworkOutput*>::iterator it = m_observerList.begin(); it != m_observerList.end(); it++)
	{
		(*it)->ConditionChecker_DataLine(data, MsgLen, arrivalTime);
	}
}

void SpeedControl::SetFixSpeedEnable(const bool& inputbSpeedLimit)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bFixSpeed = inputbSpeedLimit;
}

void SpeedControl::TimerCallBack()
{
	if (GetCurStatus() == PLAY)
	{
		SetRelativeTime(GetRelativeTime() + 0.1 * GetSpeedFactor()); // every trigger we add 0.1 sec
		m_unTimerCount++;
		if (m_unTimerCount % 10 == 0) // every one sec trigger
		{
			CalculateNextPackageTimeDiff();
		}		
	}
}

//Override
void SpeedControl::TimerCallback(int nTimerID)
{
	switch (nTimerID)
	{
	case Timer_100:
		TimerCallBack();
		break;
	default:
		break;
	}
}

bool SpeedControl::IsCanSend(const double& arrivalTime)
{
	boost::mutex::scoped_lock lock(the_mutex);

	return (CalculatePlayerTime() > arrivalTime);
}

bool SpeedControl::IsCanSend_RegularReplay(const double& arrivalTime)
{
	time_t curTimeSec;
	curTimeSec = time(NULL);

	if ((0 <= (curTimeSec - arrivalTime)) && ((curTimeSec - arrivalTime) <= 2))
		return true;
	else
		return false;
}

void SpeedControl::ResetTimer()
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_FirstPcapTime = 0;
	m_RelativeTime = 0;
}

double SpeedControl::GetFirstPcapTime()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_FirstPcapTime;
}

double SpeedControl::GetRelativeTime()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_RelativeTime;
}

double SpeedControl::GetSpeedFactor()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_speedFactor;
}

double SpeedControl::CalculatePlayerTime()
{
	double dPlayerTime;
	if (m_bOffsetEnable)
		dPlayerTime = m_FirstPcapTime + m_RelativeTime + m_unOffset;
	else
		dPlayerTime = m_FirstPcapTime + m_RelativeTime;
	dPlayerTime = dPlayerTime < 0 ? std::numeric_limits<double>::max() : dPlayerTime;

	return dPlayerTime;
}

unsigned int SpeedControl::GetOffset()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_unOffset;
}

bool SpeedControl::GetOffsetEnable()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bOffsetEnable;
}

bool SpeedControl::GetRegularReplayEnable()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bRegularReplayEnable;
}

double SpeedControl::GetArrivalTime()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_dArrivalTime;
}

bool SpeedControl::GetSpeedLimit()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bFixSpeed;
}

void SpeedControl::SetFirstPcapTime(const double& inputTime)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_FirstPcapTime = inputTime;
}

void SpeedControl::SetRelativeTime(const double& inputTime)
{
	boost::mutex::scoped_lock lock(the_mutex);
	if (inputTime < 0)
		m_RelativeTime = std::numeric_limits<double>::max();
	else
		m_RelativeTime = inputTime;
}

void SpeedControl::SetSpeedFactor(const double& inputFactor)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_speedFactor = inputFactor;
}

void SpeedControl::SetOffset(const unsigned int& inputOffset)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_unOffset = inputOffset;
}

void SpeedControl::SetOffsetEnable(const bool& inputOffsetEnbale)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bOffsetEnable = inputOffsetEnbale;
}

void SpeedControl::SetRegularReplay(const bool& inputRegularReplayEnable)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bRegularReplayEnable = inputRegularReplayEnable;
}

void SpeedControl::SetArrivalTime(const double& inputArrivalTime)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_dArrivalTime = inputArrivalTime;
}

SpeedControl::Status SpeedControl::GetCurStatus()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_curStatus;
}

void SpeedControl::SetCurStatus(Status status)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_curStatus = status;
}

void SpeedControl::CalculateNextPackageTimeDiff()
{
	if (GetArrivalTime() == 0)
	{
		return;
	}

	boost::mutex::scoped_lock lock(the_mutex);
	tm* arrivalTimeTM = new tm;
	tm* relativeTimeTM = new tm;
	time_t nArrivalTime = (int)m_dArrivalTime;
	time_t nRelativeTime = (int)CalculatePlayerTime();

	double dDiffTime = difftime(nArrivalTime, nRelativeTime);
	if (dDiffTime < 0)
	{
		dDiffTime = 0;
	}
	int nDay = (int)(dDiffTime / (24 * 3600));
	int nHour = (int)(dDiffTime) % (24 * 3600) / 3600;
	int nMin = (int)(dDiffTime) % 3600 / 60;
	int nSec = (int)(dDiffTime) % 60;
	//*arrivalTimeTM = *localtime(&nArrivalTime);
	//*relativeTimeTM = *localtime(&nRelativeTime);
	//int nDay = arrivalTimeTM->tm_yday - relativeTimeTM->tm_yday;
	//int nHour = arrivalTimeTM->tm_hour - relativeTimeTM->tm_hour;
	//int nMin = arrivalTimeTM->tm_min - relativeTimeTM->tm_min;
	//int nSec = arrivalTimeTM->tm_sec - relativeTimeTM->tm_sec;
	//nHour = nHour < 0 ? 0 : nHour;
	//nMin = nMin < 0 ? 0 : nMin;
	//nSec = nSec < 0 ? 0 : nSec;
	
	std::string strTime;
	strTime = "Next Package: " +
		std::to_string(nDay) + " day "
		+ std::to_string(nHour) + " hr " 
		+ std::to_string(nMin) + " min " 
		+ std::to_string(nSec) + " sec";
	
	if (m_pPcapReplayCtrl != nullptr)
	{
		if (nDay == 0 && nHour == 0 && nMin == 0 && nSec == 0)
		{
			m_pPcapReplayCtrl->ShowNextSendingPktTime(false);
		}
		else if (dDiffTime >= 20)
		{
			m_pPcapReplayCtrl->ShowNextSendingPktTime(true);
		}
		m_pPcapReplayCtrl->SetNextSendingPktTime(strTime);
	}
}

void SpeedControl::Convert_Time2String(double dTime, std::string& strTime)
{
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64], buf[64];

	nowtime = (int)dTime;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d,%H:%M:%S", nowtm);
	sprintf_s(buf, sizeof buf, "%s.%06d", tmbuf, (long)((dTime - (UINT32)dTime) * 1000000));

	strTime = buf;
}