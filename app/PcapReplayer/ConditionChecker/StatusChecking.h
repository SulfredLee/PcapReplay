#pragma once
#include <boost/thread/mutex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp> 

#include <set>

#include "ConditionChecker.h"
class StatusChecking :
	public ConditionChecker
{
public:
	enum Status
	{
		PLAY,
		PAUSE,
		RESUME,
		STOP
	};
	std::set<ConditionChecker*> m_observerList;
protected:
	Status m_curStatus;
	mutable boost::mutex the_mutex;
public:
	StatusChecking();
	~StatusChecking();

	Status GetCurStatus();
	void SetCurStatus(Status status);

	//Override
	void ConditionChecker_DataLine(const unsigned char * data, unsigned int MsgLen, const double& arrivalTime);
};