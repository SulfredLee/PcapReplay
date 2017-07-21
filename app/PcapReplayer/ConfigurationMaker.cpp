#include "ConfigurationMaker.h"
#include "Common.h"
#include "mainwindow.h"
#include "PcapReplayCtrl.h"
#include "dialogscheduler.h"

ConfigurationMaker::ConfigurationMaker()
{
	g_Config.m_Adapter = 0;
}

ConfigurationMaker::~ConfigurationMaker()
{

}

void ConfigurationMaker::GenConfig_SchDialog(DialogScheduler* pDiaScheduler)
{
	// todo
	// get schedular enable
	g_Config.m_bSchularEnable = pDiaScheduler->m_bSchularEnable;
	// get date and time
	g_Config.m_schedularDateTime.tm_mday = pDiaScheduler->m_schedularDate.day();
	g_Config.m_schedularDateTime.tm_wday = pDiaScheduler->m_schedularDate.dayOfWeek() % 7;
	g_Config.m_schedularDateTime.tm_yday = pDiaScheduler->m_schedularDate.dayOfYear() - 1;
	g_Config.m_schedularDateTime.tm_mon = pDiaScheduler->m_schedularDate.month() - 1;
	g_Config.m_schedularDateTime.tm_year = pDiaScheduler->m_schedularDate.year() - 1900;
	g_Config.m_schedularDateTime.tm_sec = pDiaScheduler->m_schedularTime.second();
	g_Config.m_schedularDateTime.tm_min = pDiaScheduler->m_schedularTime.minute();
	g_Config.m_schedularDateTime.tm_hour = pDiaScheduler->m_schedularTime.hour();
	// get one time only bool
	g_Config.m_bOneTimeOnly = pDiaScheduler->m_bOneTimeOnly;
	// get week day
	g_Config.m_vecWeekDaysChecked.clear();
	g_Config.m_vecWeekDaysChecked.resize(7);
	for (int i = 0; i < g_Config.m_vecWeekDaysChecked.size(); i++)
	{
		g_Config.m_vecWeekDaysChecked[i] = pDiaScheduler->m_vecWeekDaysChecked[i];
	}
}

void ConfigurationMaker::GenConfig_MainWin(MainWindow* pMainWindow)
{
	// init output adapter
	g_Config.m_AdapterName = m_pMainCtrl->m_vecAdapterName[pMainWindow->m_nAdapterIdx];
	SetAdapter_Output();

	// convert pcap file list
	g_Config.m_PcapFiles.clear();
	for (int i = pMainWindow->m_nCurPcapFileIdx; i < pMainWindow->m_fileNames.size(); i++)
	{
		g_Config.m_PcapFiles.push_back(pMainWindow->m_fileNames[i].toUtf8().constData());
	}

	// handle line edit
	g_Config.m_dSpeedFactor = pMainWindow->Get_SpeedFactor();
	g_Config.m_unSpeedLimit = pMainWindow->Get_SpeedLimit();
	g_Config.m_unOffSet = pMainWindow->Get_OffSet();
	g_Config.m_nLoopCount = pMainWindow->Get_LoopCount();

	// handle checked box
	g_Config.m_bFixSpeed = pMainWindow->Get_Check_SpeedLimit();
	g_Config.m_bOffsetEnable = pMainWindow->Get_Check_OffsetEnable();
	g_Config.m_bRegularReplayEnable = pMainWindow->Get_Check_RegularReplayEnable();
	g_Config.m_bLoopPlay = pMainWindow->Get_Check_LoopPlay();

	// handle table widget
	g_Config.m_mapDstIP = pMainWindow->m_mapDstIP;
	g_Config.m_mapScrIP = pMainWindow->m_mapScrIP;
}

void ConfigurationMaker::SetAdapter_Output()
{
	if (g_Config.m_Adapter != 0)
	{
		pcap_close(g_Config.m_Adapter);
		g_Config.m_Adapter = 0;
	}
	char errbuff[PCAP_ERRBUF_SIZE];
	/* Open the adapter */
	if ((g_Config.m_Adapter = pcap_open_live(g_Config.m_AdapterName.c_str(),		// name of the device
		65536,			// portion of the packet to capture. It doesn't matter in this case 
		1,				// promiscuous mode (nonzero means promiscuous)
		1000,			// read timeout
		errbuff			// error buffer
		)) == NULL)
	{
		g_logM.PrintLog(boost::log::trivial::error, "Unable to open the adapter. %s is not supported by WinPcap\n", g_Config.m_AdapterName.c_str());
	}
}