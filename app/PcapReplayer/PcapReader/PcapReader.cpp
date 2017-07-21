#include "PcapReader.h"
#include "ConditionChecker.h"
#include "PcapReplayCtrl.h"


PcapReader::PcapReader()
{
	m_unNextPkt = 1;
}


PcapReader::~PcapReader()
{
	m_pPcapReplayCtrl = 0;
}

unsigned int PcapReader::GetNextPktNo()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_unNextPkt;
}

void PcapReader::SetNextPktNo(const unsigned int unPkt)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_unNextPkt = unPkt;
}

void PcapReader::StartReadOneFile()
{
	m_nProgress = -1;
	SetNextPktNo(1);
}

void PcapReader::ParseOnePkt(pcap_t * pcap, struct pcap_pkthdr *header, const u_char *data)
{
	long m_packetArrivalSec = header->ts.tv_sec;
	long m_packetArrivalmicroseconds = header->ts.tv_usec;
	double arrivalTime = m_packetArrivalSec + m_packetArrivalmicroseconds*0.000001;

	for (std::set<ConditionChecker*>::iterator it = m_observerList.begin(); it != m_observerList.end(); it++)
	{
		(*it)->ConditionChecker_DataLine(data, header->len, arrivalTime); // header->len unit as byte
	}
	if (myThread::IsEndThread())
		return;
	// handle process information
	m_ullCurSentSize += header->len + PCAPLOCALHEADERSIZE;
	float fProgress = (float)m_ullCurSentSize / (float)m_ullCurFileSize;
	if (m_nProgress != (int)(fProgress * 100))
	{
		m_pPcapReplayCtrl->SetCurProcess(m_nProgress);
		m_nProgress++;
	}
	// handle next packet information
	unsigned int unNextPkt = GetNextPktNo();
	SetNextPktNo(++unNextPkt);
}

void PcapReader::FinishOneFile()
{
	m_pPcapReplayCtrl->SetCurProcess(0);
	m_nProgress = -1;
	SetNextPktNo(1);
	if (m_pPcapReplayCtrl != 0)
		m_pPcapReplayCtrl->ReadNextPcapFile();
}