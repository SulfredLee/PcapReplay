#include "PcapReplayCtrl.h"
#include "Common.h"

PcapReplayCtrl::PcapReplayCtrl()
{
}


PcapReplayCtrl::~PcapReplayCtrl()
{
	m_PcapReader.EndThread();

	m_PcapReader.m_pPcapReplayCtrl = 0;
	m_SpeedLimit.m_pPcapReplayCtrl = 0;
	m_NetworkOutput.m_pPcapReplayCtrl = 0;
	m_MainWindow.m_pMainCtrl = 0;
	m_ConfigurationMaker.m_pMainCtrl = 0;
	m_AutoDriver.m_pPcapReplayCtrl = 0;
}

void PcapReplayCtrl::Init()
{
	// init PcapReader
	m_PcapReader.m_listener = this;
	m_PcapReader.m_objID = PCAP_READER;
	m_PcapReader.m_inputFiles = g_Config.m_PcapFiles;
	m_PcapReader.m_observerList.insert(&m_StatusChecking);
	m_PcapReader.m_pPcapReplayCtrl = this;

	// init StatusChecking
	m_StatusChecking.SetCurStatus(StatusChecking::STOP);
	m_StatusChecking.m_observerList.insert(&m_SpeedLimit);

	// init SpeedLimit
	m_SpeedLimit.SetFixSpeedEnable(g_Config.m_bFixSpeed);
	m_SpeedLimit.m_BitPerSec_Limit = g_Config.m_unSpeedLimit;
	m_SpeedLimit.m_observerList.insert(&m_SpeedControl);
	m_SpeedLimit.m_pPcapReplayCtrl = this;

	// init SpeedControl
	m_SpeedControl.SetSpeedFactor(g_Config.m_dSpeedFactor);
	m_SpeedControl.SetFixSpeedEnable(g_Config.m_bFixSpeed);
	m_SpeedControl.m_observerList.insert(&m_NetworkOutput);
	m_SpeedControl.SetRegularReplay(g_Config.m_bRegularReplayEnable);
	m_SpeedControl.m_pPcapReplayCtrl = this;

	// init NetworkOutput
	m_NetworkOutput.m_Adapter = g_Config.m_Adapter;
	m_NetworkOutput.m_AdapterName = g_Config.m_AdapterName;
	m_NetworkOutput.m_pPcapReplayCtrl = this;

	// init Adapter information
	m_vecAdapterName = m_AdapterInfoCtrl.m_vecAdapterName;
	m_vecInterfaceInfo = m_AdapterInfoCtrl.m_vecInterfaceInfo;

	// init main window
	m_MainWindow.m_pMainCtrl = this;
	m_MainWindow.show();
	m_MainWindow.Serialize(false);
	emit m_MainWindow.StartInitialFirstScreen();

	// init configuration maker
	m_ConfigurationMaker.m_pMainCtrl = this;
	GenConfig_MainWin();
	GenConfig_SchDialog();

	// init status
	m_StatusChecking.SetCurStatus(StatusChecking::STOP);

	// init AutoDriver
	m_AutoDriver.SetSchedularDateTime(g_Config.m_schedularDateTime);
	m_AutoDriver.SetOneTimeOnly(g_Config.m_bOneTimeOnly);
	m_AutoDriver.SetSchularEnable(g_Config.m_bSchularEnable);
	m_AutoDriver.SetWeekDayChecked(g_Config.m_vecWeekDaysChecked);
	m_AutoDriver.m_pPcapReplayCtrl = this;
}

void PcapReplayCtrl::SetCurProcess(int nCurProcess)
{
	emit m_MainWindow.SetCurProcess(nCurProcess);
}

void PcapReplayCtrl::SetCurSendingSpeed(int nCurSpeed)
{
	if (m_MainWindow.isVisible())
	{
		emit m_MainWindow.SetCurSendingSpeed(nCurSpeed);
	}
	else
	{

	}
}

void PcapReplayCtrl::SetCurSendingPktTime(const std::string& strTime)
{
	char line[1024];
	strcpy_s(line, sizeof(line), strTime.c_str());
	emit m_MainWindow.SetCurSendingPktTime(strTime.c_str());
}

void PcapReplayCtrl::SetNextSendingPktTime(const std::string& strTime)
{
	char line[1024];
	strcpy_s(line, sizeof(line), strTime.c_str());
	emit m_MainWindow.SetNextSendingPktTime(strTime.c_str());
}

void PcapReplayCtrl::ShowNextSendingPktTime(bool bShow)
{
	emit m_MainWindow.ShowNextSendingPktTime(bShow);
}

void PcapReplayCtrl::StartPlay()
{
	if (m_StatusChecking.GetCurStatus() == StatusChecking::PLAY)
		return;
	SetConfigToOperators();
	m_StatusChecking.SetCurStatus(StatusChecking::PLAY);
	m_SpeedControl.SetCurStatus(SpeedControl::PLAY);
	m_PcapReader.StartThread();
}

void PcapReplayCtrl::SetConfigToOperators()
{
	if (m_StatusChecking.GetCurStatus() == StatusChecking::STOP)
	{
		// set Speed Control
		m_SpeedControl.ResetTimer();
	}

	// set Pcap Reader
	m_PcapReader.m_inputFiles = g_Config.m_PcapFiles;
	m_PcapReader.m_mapDstIP = g_Config.m_mapDstIP;
	m_PcapReader.m_mapScrIP = g_Config.m_mapScrIP;
	// set Speed Limit
	m_SpeedLimit.m_BitPerSec_Limit = g_Config.m_unSpeedLimit;
	m_SpeedLimit.SetFixSpeedEnable(g_Config.m_bFixSpeed);
	// set Speed Control
	m_SpeedControl.SetSpeedFactor(g_Config.m_dSpeedFactor);
	m_SpeedControl.SetFixSpeedEnable(g_Config.m_bFixSpeed);
	m_SpeedControl.SetOffset(g_Config.m_unOffSet);
	m_SpeedControl.SetOffsetEnable(g_Config.m_bOffsetEnable);
	m_SpeedControl.SetRegularReplay(g_Config.m_bRegularReplayEnable);
	// set Network Output
	m_NetworkOutput.m_Adapter = g_Config.m_Adapter;
	m_NetworkOutput.m_AdapterName = g_Config.m_AdapterName;
}

void PcapReplayCtrl::StartPause()
{
	if (m_StatusChecking.GetCurStatus() == StatusChecking::PLAY)
	{
		m_StatusChecking.SetCurStatus(StatusChecking::PAUSE);
		m_SpeedControl.SetCurStatus(SpeedControl::PAUSE);
	}
	emit m_MainWindow.SetNextPktNo(m_PcapReader.GetNextPktNo());
}

void PcapReplayCtrl::StartResume()
{
	SetConfigToOperators();
	if (m_StatusChecking.GetCurStatus() == StatusChecking::PAUSE)
	{
		m_StatusChecking.SetCurStatus(StatusChecking::PLAY);
		m_SpeedControl.SetFirstPcapTime(0);
		m_SpeedControl.SetRelativeTime(0);
		m_SpeedControl.SetCurStatus(SpeedControl::PLAY);
	}
}

void PcapReplayCtrl::StartStop()
{
	if (m_StatusChecking.GetCurStatus() == StatusChecking::STOP)
		return;
	m_StatusChecking.SetCurStatus(StatusChecking::STOP);
	m_PcapReader.EndThread();
	emit m_MainWindow.Signal_ToUIState_Stop();
}

// Callback when all files are replayed
void PcapReplayCtrl::PcapReaderCallBack()
{
	StartStop();
	//emit m_MainWindow.Signal_ToUIState_Stop();
	if (g_Config.m_bLoopPlay && (g_Config.m_nLoopCount == -1 || g_Config.m_nLoopCount > 0))
	{
		if (g_Config.m_nLoopCount > 0)
			emit m_MainWindow.SetLoopCount(g_Config.m_nLoopCount - 1);
		StartPlayFromAutoDriver();
	}
}

//Override
void PcapReplayCtrl::ThreadMain()
{
	while (!myThread::IsEndThread())
	{
		PcapReplayCtrl_MsgType curMsgType;
		m_MsgQ.wait_and_pop(curMsgType);
		switch (curMsgType)
		{
		case PLAY:
		case REGULAR_REPLAY:
			StartPlay();
			break;
		case PAUSE:
			StartPause();
			break;
		case RESUME:
			StartResume();
			break;
		case STOP:
			StartStop();
			break;
		case GEN_CONFIG:
			GenConfig_MainWin();
			break;
		case GEN_CONFIG_SCHDIALOG:
			GenConfig_SchDialog();
			break;
		default:
			break;
		}
	}
}

//Override
void PcapReplayCtrl::ThreadFinishCallBack(int objID)
{
	switch (objID)
	{
	case PCAP_READER:
		PcapReaderCallBack();
		break;
	default:
		break;
	}
}

void PcapReplayCtrl::OnTest()
{
	int i = 0;
	i++;
}

void PcapReplayCtrl::GenConfig_MainWin()
{
	m_ConfigurationMaker.GenConfig_MainWin(&m_MainWindow);
}

void PcapReplayCtrl::GenConfig_SchDialog()
{
	m_ConfigurationMaker.GenConfig_SchDialog(m_MainWindow.m_pDialogScheduler);

	m_AutoDriver.SetSchedularDateTime(g_Config.m_schedularDateTime);
	m_AutoDriver.SetOneTimeOnly(g_Config.m_bOneTimeOnly);
	m_AutoDriver.SetSchularEnable(g_Config.m_bSchularEnable);
	m_AutoDriver.SetWeekDayChecked(g_Config.m_vecWeekDaysChecked);
	if (g_Config.m_bSchularEnable)
		m_AutoDriver.StartAutoDrive();
	emit m_MainWindow.StartInitialFirstScreen();
}

PcapReplayCtrl::PcapReplayCtrl_MsgType PcapReplayCtrl::Get_CurrentState()
{
	return (PcapReplayCtrl_MsgType)m_StatusChecking.GetCurStatus();
}

void PcapReplayCtrl::StartPlayFromAutoDriver()
{
	emit m_MainWindow.StartPlayFromAutoDriver();
}

void PcapReplayCtrl::ReadNextPcapFile()
{
	// m_SpeedControl.ResetTimer(); // to make sure we read the first pcap as a time reference for every file
	emit m_MainWindow.ReadNextPcapFile();
}