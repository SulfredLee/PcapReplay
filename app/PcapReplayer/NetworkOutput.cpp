#include "NetworkOutput.h"
#include "PcapReplayCtrl.h"

NetworkOutput::NetworkOutput()
{
	// init timer
	m_nTimerWait = 1000;
	m_MyTimer.SetTime(m_nTimerWait, Timer_1000);
	m_MyTimer.m_pParent = this;
	// init number
	m_CurBitPerSec = 0;
	SetArrivalTime(0);
}

NetworkOutput::~NetworkOutput()
{
	m_MyTimer.m_pParent = nullptr;
}

void NetworkOutput::ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime)
{
	if (m_Adapter == 0)
	{
		LOGMSG_ERROR("Error Adapter is empty");
		//g_logM.PrintLog(boost::log::trivial::error, "Error Adapter is empty\n");
		return;
	}
	if (pcap_sendpacket(m_Adapter,
		data,
		MsgLen
		) != 0)
	{
		std::ostringstream ss;
		ss << "Error sending the packet: " << pcap_geterr(m_Adapter);
		LOGMSG_ERROR(ss.str());
		//g_logM.PrintLog(boost::log::trivial::error, "Error sending the packet: %s\n", pcap_geterr(m_Adapter));
	}
	SetArrivalTime(arrivalTime);
	SetCurBitPerSec(GetCurBitPerSec() + MsgLen * 8);
}

//Override
void NetworkOutput::TimerCallback(int nTimerID)
{
	switch (nTimerID)
	{
	case Timer_1000:
		TimerCallback();
		break;
	default:
		break;
	}
}

void NetworkOutput::TimerCallback()
{
	if (m_pPcapReplayCtrl != 0)
	{
		m_pPcapReplayCtrl->SetCurSendingSpeed(GetCurBitPerSec());
		std::string strTime;
		Convert_Time2String(GetArrivalTime(), strTime);
		m_pPcapReplayCtrl->SetCurSendingPktTime(strTime);
	}
	SetCurBitPerSec(0);
}

void NetworkOutput::Convert_Time2String(double dTime, std::string& strTime)
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

int NetworkOutput::GetCurBitPerSec()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_CurBitPerSec;
}

double NetworkOutput::GetArrivalTime()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_dArrivalTime;
}

void NetworkOutput::SetCurBitPerSec(int curBit)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_CurBitPerSec = curBit;
}

void NetworkOutput::SetArrivalTime(const double& dArrivalTime)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_dArrivalTime = dArrivalTime;
}