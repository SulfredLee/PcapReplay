#include "main.h"
//#include "Common.h"
#include "PcapReplayCtrl.h"
#include "LogMgr.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	Logging::CLogTargetDebugger traceLogger(Logging::LOG_LEVEL_TRACE);
	Logging::CLoggerFactory::getDefaultInstance()->AddTarget(&traceLogger);

	PcapReplayCtrl mainCtrl;
	mainCtrl.Init();
	mainCtrl.StartThread();
	
	bool rtn = a.exec();
	exit(0);
	return rtn;
}
