#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <map>

#include "pcap.h"

class Configuration
{
public:
	// Pcap reader
	std::vector<std::string> m_PcapFiles;
	std::map<std::string, std::string> m_mapDstIP;
	std::map<std::string, std::string> m_mapScrIP;
	// Speed limit
	unsigned int m_unSpeedLimit; // Bit per Sec
	bool m_bFixSpeed;
	// Speed control
	double m_dSpeedFactor; // Bit per Sec
	bool m_bOffsetEnable;
	bool m_bRegularReplayEnable;
	unsigned int m_unOffSet; // Sec
	// Network output
	pcap_t * m_Adapter;
	std::string m_AdapterName;
	// Scheduler auto driver
	tm m_schedularDateTime;
	bool m_bOneTimeOnly;
	bool m_bSchularEnable;
	std::vector<bool> m_vecWeekDaysChecked; // from monday to sunday
	// Other
	bool m_bLoopPlay;
	int m_nLoopCount;
public:
	Configuration();
	~Configuration();
};