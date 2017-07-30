#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <boost/thread.hpp>

class myThreadCallBack
{
public:
	myThreadCallBack(){}
	~myThreadCallBack(){}

	virtual void ThreadFinishCallBack(int objID) = 0;
};
class myThread
{
public:
	boost::thread * m_t;
	myThreadCallBack * m_listener;
	int m_objID;
private:
	bool m_bThreadExit;
	boost::mutex m_mtx;
public:
	myThread();
	~myThread();

	void StartThread();
	virtual void ThreadMain() = 0;
	void EndThread();
	bool IsEndThread();
};
#endif
