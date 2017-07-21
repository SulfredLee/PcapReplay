#include "StatusChecking.h"

StatusChecking::StatusChecking()
{
	m_curStatus = STOP;
}


StatusChecking::~StatusChecking()
{
}

void StatusChecking::ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime)
{
	while (GetCurStatus() != PLAY)
	{
		if (GetCurStatus() == STOP)
			return;
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}

	for (std::set<ConditionChecker*>::iterator it = m_observerList.begin(); it != m_observerList.end(); it++)
	{
		(*it)->ConditionChecker_DataLine(data, MsgLen, arrivalTime);
	}
}

StatusChecking::Status StatusChecking::GetCurStatus()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_curStatus;
}

void StatusChecking::SetCurStatus(Status status)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_curStatus = status;
}