#include "main.h"
//#include "Common.h"
#include "PcapReplayCtrl.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	PcapReplayCtrl mainCtrl;
	mainCtrl.Init();
	mainCtrl.StartThread();
	
	bool rtn = a.exec();
	exit(0);
	return rtn;
}
