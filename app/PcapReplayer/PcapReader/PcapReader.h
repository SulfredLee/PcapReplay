#ifndef PCAPREADER_H
#define PCAPREADER_H
#include "Common.h"

#include <set>
#include <vector>
#include <string>
#include <fstream>

#include <boost/thread/mutex.hpp>

#include "PcapReaderBase.h"

class PcapReplayCtrl;
class PcapReader :
	public PcapReaderBase
{
public:
	PcapReplayCtrl* m_pPcapReplayCtrl;
protected:
	mutable boost::mutex the_mutex;
	unsigned int m_unNextPkt;
	int m_nProgress;
public:
	PcapReader();
	virtual ~PcapReader();
	
	unsigned int GetNextPktNo();
	void SetNextPktNo(const unsigned int unPkt);

	// override
	void StartReadOneFile();
	void ParseOnePkt(pcap_t * pcap, struct pcap_pkthdr *header, const u_char *data);
	void FinishOneFile();
};
#endif
