#ifndef SPEEDLIMIT_H
#define SPEEDLIMIT_H
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <set>
#include <string>

#include "ConditionChecker.h"

#include "myTimer.h"
class PcapReplayCtrl;
class SpeedLimit :
	public ConditionChecker, public myTimerInterface
{
public:
	std::set<ConditionChecker*> m_observerList;
	unsigned int m_BitPerSec_Limit;
	PcapReplayCtrl* m_pPcapReplayCtrl;
protected:
	enum TimerIDs
	{
		Timer_100
	};
	unsigned int m_CurBitPerSec;
	mutable boost::mutex the_mutex;
	bool m_bFixSpeedEnable;
	unsigned int m_nAccumulateBit;
	myTimer m_MyTimer;
public:
	SpeedLimit();
	~SpeedLimit();

	int GetCurBitPerSec();
	bool GetFixSpeedEnable();
	unsigned int GetAccumulateBit();
	void SetCurBitPerSec(int curBit);
	void SetFixSpeedEnable(bool bLimitEnable);
	void SetAccumulateBit(unsigned int accumulateBit);
	void TimerCallback();
	

	//Override
	void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime);
	//Override
	void TimerCallback(int nTimerID);
};
#endif
