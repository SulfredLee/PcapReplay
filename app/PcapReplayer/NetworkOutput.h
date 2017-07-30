#ifndef NETWORKOUTPUT_H
#define NETWORKOUTPUT_H
#include "Common.h"

#include "myTimer.h"

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

class PcapReplayCtrl;
class NetworkOutput :
	public myTimerInterface
{
public:
	pcap_t * m_Adapter;
	std::string m_AdapterName;
	PcapReplayCtrl* m_pPcapReplayCtrl;
protected:
	enum TimerIDs
	{
		Timer_1000
	};
	int m_nTimerWait;
	double m_dArrivalTime;
	unsigned int m_CurBitPerSec;
	mutable boost::mutex the_mutex;
	myTimer m_MyTimer;
public:
	NetworkOutput();
	~NetworkOutput();

	void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime);
	void TimerCallback();
	int GetCurBitPerSec();
	double GetArrivalTime();
	void SetCurBitPerSec(int curBit);
	void SetArrivalTime(const double& dArrivalTime);

	//Override
	void TimerCallback(int nTimerID);
protected:
	void Convert_Time2String(double dTime, std::string& strTime);
};
#endif
