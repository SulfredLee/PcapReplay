#ifndef ADAPTERINFORCTRL_H
#define ADAPTERINFORCTRL_H
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

#include "Common.h"

// #include "Packet32.h"
// #include <ntddndis.h>

class AdapterInfoCtrl
{
public:
	// AdapterName for an example \Device\NPF_{7485820D-FCE1-47FE-8D7B-E747D7853137}
	std::vector<std::string> m_vecAdapterName; 
	std::vector<std::string> m_vecInterfaceInfo; // with IP, AdapterName
	std::vector<std::string> m_vecIP;
	std::vector<std::string> m_vecMacAddress; // Mac address for those NIC, network interface control
public:
	AdapterInfoCtrl();
	~AdapterInfoCtrl();
private:
	std::string GetMacAddress(const std::string& strAdapterName);
};
#endif
