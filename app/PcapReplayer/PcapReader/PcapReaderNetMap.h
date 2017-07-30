#ifndef PCAPREADERNETMAP_H
#define PCAPREADERNETMAP_H
#include "Common.h"

#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <map>

#include <boost/thread/mutex.hpp>

#include "PcapReaderBase.h"

class PcapReplayCtrl;
class PcapReaderNetMap :
	public PcapReaderBase
{
public:
	PcapReplayCtrl* m_pPcapReplayCtrl;
	std::map<std::string, std::string> m_mapDstIP;
	std::map<std::string, std::string> m_mapScrIP;
protected:
	mutable boost::mutex the_mutex;
	unsigned int m_unNextPkt;
	int m_nProgress;
public:
	PcapReaderNetMap();
	virtual ~PcapReaderNetMap();
	
	unsigned int GetNextPktNo();
	void SetNextPktNo(const unsigned int unPkt);

	// override
	void StartReadOneFile();
	void ParseOnePkt(pcap_t * pcap, struct pcap_pkthdr *header, const u_char *data);
	void FinishOneFile();
protected:
	std::string getDstIPFromPcapPkt(const u_char *& data);
	std::string getSrcIPFromPcapPkt(const u_char *& data);
	unsigned short getPortFromPcapPkt(const u_char *& data);
	void UpdateDstIP(u_char *& data, std::string strDstIP);
	void UpdateScrIP(u_char *& data, std::string strScrIP);
	std::vector<std::string> string_split(const std::string& s, const char& delim);
};
#endif
