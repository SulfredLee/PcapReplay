#pragma once
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <set>
#include <limits>
#include <time.h>

#include "ConditionChecker.h"
#include "myTimer.h"

class NetworkOutput;
class PcapReplayCtrl;
class SpeedControl :
	public ConditionChecker, myTimerInterface
{
public:
	enum TimerIDs
	{
		Timer_100
	};
	enum Status
	{
		PLAY,
		PAUSE,
		RESUME,
		STOP
	};
	std::set<NetworkOutput*> m_observerList;
	PcapReplayCtrl* m_pPcapReplayCtrl;
protected:
	mutable boost::mutex the_mutex;
	double m_FirstPcapTime;
	double m_RelativeTime;
	double m_speedFactor;
	double m_dArrivalTime;
	myTimer m_myTimer;
	unsigned int m_unOffset;
	bool m_bOffsetEnable;
	bool m_bRegularReplayEnable;
	bool m_bFixSpeed;
	Status m_curStatus;
	unsigned int m_unTimerCount;
public:
	SpeedControl();
	~SpeedControl();

	bool IsCanSend(const double& arrivalTime);
	bool IsCanSend_RegularReplay(const double& arrivalTime);
	void ResetTimer();
	double GetFirstPcapTime();
	double GetRelativeTime();
	double GetSpeedFactor();
	double CalculatePlayerTime();
	unsigned int GetOffset();
	bool GetOffsetEnable();
	bool GetRegularReplayEnable();
	double GetArrivalTime();
	bool GetSpeedLimit();
	void SetFirstPcapTime(const double& inputTime);
	void SetRelativeTime(const double& inputTime);
	void SetSpeedFactor(const double& inputFactor);
	void SetOffset(const unsigned int& inputOffset);
	void SetOffsetEnable(const bool& inputOffsetEnable);
	void SetRegularReplay(const bool& inputRegularReplayEnable);
	void SetArrivalTime(const double& inputArrivalTime);
	void SetFixSpeedEnable(const bool& inputbSpeedLimit);
	void TimerCallBack();
	Status GetCurStatus();
	void SetCurStatus(Status status);
	void CalculateNextPackageTimeDiff();
	void Convert_Time2String(double dTime, std::string& strTime);

	//Override
	void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime);
	//Override
	void TimerCallback(int nTimerID);
};