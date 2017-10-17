#pragma once
#include "Common.h"

#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include <boost/filesystem.hpp>

#include "myThread.h"

class ConditionChecker;
class PcapReaderBase :
	public myThread
{
public:
	std::vector<std::string> m_inputFiles;
	std::set<ConditionChecker*> m_observerList;
protected:
	std::string m_inputFile;
	const unsigned int PCAPGOBLEHEADERSIZE = 24;
	const unsigned int PCAPLOCALHEADERSIZE = 16;
	const unsigned int DATAGRAME_DATA_OFFSET = 42;
	boost::uintmax_t m_ullCurSentSize;
	boost::uintmax_t m_ullCurFileSize;
public:
	PcapReaderBase();
	virtual ~PcapReaderBase();

	virtual void StartReadOneFile() = 0;
	virtual void ParseOnePkt(pcap_t * pcap, struct pcap_pkthdr *header, const u_char *data) = 0;
	virtual void FinishOneFile() = 0;

	void StartReading();

	//Override
	void ThreadMain();
protected:
	//std::ifstream::pos_type FileSize(const std::string& filename);
	boost::uintmax_t FileSize(const std::string& filename);
	void ReCalculateCheckSum(u_char* pData, unsigned int unDataLen);
};