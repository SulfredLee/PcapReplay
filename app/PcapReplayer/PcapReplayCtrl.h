#ifndef PCAPREPLAYCTRL_H
#define PCAPREPLAYCTRL_H
#include "PcapReaderNetMap.h"
#include "SpeedLimit.h"
#include "StatusChecking.h"
#include "SpeedControl.h"
#include "NetworkOutput.h"
#include "AdapterInfoCtrl.h"
#include "ConfigurationMaker.h"
#include "mainwindow.h"
#include "AutoDriver.h"

#include "myThread.h"
#include "blockingQ.h"

class PcapReplayCtrl :
	public myThread, myThreadCallBack
{
public:
	enum PcapReplayCtrl_MsgType
	{
		PLAY,
		PAUSE,
		RESUME,
		STOP,
		GEN_CONFIG,
		GEN_CONFIG_SCHDIALOG,
		REGULAR_REPLAY
	};
	enum ObjID
	{
		PCAP_READER
	};
	blockingQ<PcapReplayCtrl_MsgType> m_MsgQ; // class MainWindow will send message to this queue
	std::vector<std::string> m_vecAdapterName; // with AdapterName only
	std::vector<std::string> m_vecInterfaceInfo; // with IP, AdapterName
protected:
	PcapReaderNetMap m_PcapReader;
	SpeedLimit m_SpeedLimit;
	StatusChecking m_StatusChecking;
	SpeedControl m_SpeedControl;
	NetworkOutput m_NetworkOutput;
	AdapterInfoCtrl m_AdapterInfoCtrl;
	ConfigurationMaker m_ConfigurationMaker;
	MainWindow m_MainWindow;
	AutoDriver m_AutoDriver;
public:
	PcapReplayCtrl();
	~PcapReplayCtrl();

	void Init();
	void StartPlay();
	void StartPause();
	void StartResume();
	void StartStop();
	void PcapReaderCallBack();

	//Override
	void ThreadMain();
	//Override
	void ThreadFinishCallBack(int objID);

	void OnTest();
	void GenConfig_MainWin();
	void GenConfig_SchDialog();
	PcapReplayCtrl::PcapReplayCtrl_MsgType Get_CurrentState();
	void SetCurProcess(int nCurProcess);
	void SetCurSendingSpeed(int nCurSpeed);
	void SetCurSendingPktTime(const std::string& strTime);
	void SetNextSendingPktTime(const std::string& strTime);
	void ShowNextSendingPktTime(bool bShow);
	void SetConfigToOperators();
	void StartPlayFromAutoDriver();
	void ReadNextPcapFile();
};
#endif
