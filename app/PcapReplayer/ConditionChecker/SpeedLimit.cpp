#include "SpeedLimit.h"
#include "PcapReplayCtrl.h"

SpeedLimit::SpeedLimit()
{
	m_BitPerSec_Limit = 0;
	m_bFixSpeedEnable = false;
	m_CurBitPerSec = 0;
	m_nAccumulateBit = 0;
	m_MyTimer.SetTime(100, Timer_100);
	m_MyTimer.m_pParent = this;
}


SpeedLimit::~SpeedLimit()
{
	m_MyTimer.m_pParent = nullptr;
}

void SpeedLimit::ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime)
{
	if (GetFixSpeedEnable())
	{
		SetAccumulateBit(0);
		while ((GetCurBitPerSec() + MsgLen * 8) > (m_BitPerSec_Limit / 10 + GetAccumulateBit()))
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			if (!GetFixSpeedEnable())
			{
				break;
			}
		}
		SetAccumulateBit(0);
	}
	SetCurBitPerSec(GetCurBitPerSec() + MsgLen * 8);
	for (std::set<ConditionChecker*>::iterator it = m_observerList.begin(); it != m_observerList.end(); it++)
	{
		(*it)->ConditionChecker_DataLine(data, MsgLen, arrivalTime);
	}
}

int SpeedLimit::GetCurBitPerSec()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_CurBitPerSec;
}

bool SpeedLimit::GetFixSpeedEnable()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_bFixSpeedEnable;
}


unsigned int SpeedLimit::GetAccumulateBit()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_nAccumulateBit;
}

void SpeedLimit::SetCurBitPerSec(int curBit)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_CurBitPerSec = curBit;
}

void SpeedLimit::SetFixSpeedEnable(bool bLimitEnable)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_bFixSpeedEnable = bLimitEnable;
}

void SpeedLimit::SetAccumulateBit(unsigned int accumulateBit)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_nAccumulateBit = accumulateBit;
}

//Override
void SpeedLimit::TimerCallback(int nTimerID)
{
	switch (nTimerID)
	{
	case Timer_100:
		TimerCallback();
		break;
	default:
		break;
	}
}

void SpeedLimit::TimerCallback()
{
	SetCurBitPerSec(0);
	// If packet size is 100 bit, limit size is 10 bit
	// we will send the packet after 10 duration
	// by following code, we are able to do this
	SetAccumulateBit(GetAccumulateBit() + (m_BitPerSec_Limit / 10));
}

