#include "PcapReaderNetMap.h"
#include "ConditionChecker.h"
#include "PcapReplayCtrl.h"


PcapReaderNetMap::PcapReaderNetMap()
{
	m_unNextPkt = 1;
}


PcapReaderNetMap::~PcapReaderNetMap()
{
	m_pPcapReplayCtrl = 0;
}

unsigned int PcapReaderNetMap::GetNextPktNo()
{
	boost::mutex::scoped_lock lock(the_mutex);
	return m_unNextPkt;
}

void PcapReaderNetMap::SetNextPktNo(const unsigned int unPkt)
{
	boost::mutex::scoped_lock lock(the_mutex);
	m_unNextPkt = unPkt;
}

void PcapReaderNetMap::StartReadOneFile()
{
	m_nProgress = -1;
	SetNextPktNo(1);
}

void PcapReaderNetMap::ParseOnePkt(pcap_t * pcap, struct pcap_pkthdr *header, const u_char *data)
{
	long m_packetArrivalSec = header->ts.tv_sec;
	long m_packetArrivalmicroseconds = header->ts.tv_usec;
	double arrivalTime = m_packetArrivalSec + m_packetArrivalmicroseconds*0.000001;

	// handle mapping
	u_char* pMyData = new u_char[header->len];
	memcpy(pMyData, data, header->len);
	std::string strDstIP = getDstIPFromPcapPkt(data);
	std::string stdScrIP = getSrcIPFromPcapPkt(data);
	std::map<std::string, std::string>::iterator dstIT = m_mapDstIP.find(strDstIP);
	std::map<std::string, std::string>::iterator scrIT = m_mapScrIP.find(stdScrIP);
	if (dstIT != m_mapDstIP.end())
		UpdateDstIP(pMyData, dstIT->second);
	if (scrIT != m_mapScrIP.end())
		UpdateScrIP(pMyData, scrIT->second);
	// handle recalculate checksum
	if (dstIT != m_mapDstIP.end() || scrIT != m_mapScrIP.end())
	{
		ReCalculateCheckSum_IPHeader(pMyData);
		ReCalculateCheckSum_UDP_Pkt(pMyData, header->len);
	}

	for (std::set<ConditionChecker*>::iterator it = m_observerList.begin(); it != m_observerList.end(); it++)
	{
		(*it)->ConditionChecker_DataLine(pMyData, header->len, arrivalTime); // header->len unit as byte
	}
	delete[] pMyData;

	if (myThread::IsEndThread())
		return;
	// handle process information
	m_ullCurSentSize += header->len + PCAPLOCALHEADERSIZE;
	float fProgress = (float)m_ullCurSentSize / (float)m_ullCurFileSize;
	if (m_nProgress != (int)(fProgress * 100))
	{
		m_nProgress = (int)(fProgress * 100);
		m_pPcapReplayCtrl->SetCurProcess(m_nProgress);
	}
	// handle next packet information
	unsigned int unNextPkt = GetNextPktNo();
	SetNextPktNo(++unNextPkt);
}

void PcapReaderNetMap::FinishOneFile()
{
	m_pPcapReplayCtrl->SetCurProcess(0);
	m_nProgress = -1;
	SetNextPktNo(1);
	if (m_pPcapReplayCtrl != 0)
		m_pPcapReplayCtrl->ReadNextPcapFile();
}

std::string PcapReaderNetMap::getDstIPFromPcapPkt(const u_char *& data)
{
	return std::to_string((_ULonglong)data[30]) + "." + 
		std::to_string((_ULonglong)data[31]) + "." + 
		std::to_string((_ULonglong)data[32]) + "." + 
		std::to_string((_ULonglong)data[33]);
}

std::string PcapReaderNetMap::getSrcIPFromPcapPkt(const u_char *& data)
{
	return std::to_string((_ULonglong)data[26]) + "." + 
		std::to_string((_ULonglong)data[27]) + "." + 
		std::to_string((_ULonglong)data[28]) + "." + 
		std::to_string((_ULonglong)data[29]);
}

unsigned short PcapReaderNetMap::getPortFromPcapPkt(const u_char *& data)
{
	unsigned short dstPort;
	memcpy(&dstPort, &data[36], 2);
	dstPort = _byteswap_ushort(dstPort);
	return dstPort;
}

void PcapReaderNetMap::UpdateDstIP(u_char *& data, std::string strDstIP)
{
	std::vector<std::string> parts = string_split(strDstIP, '.');
	data[30] = (byte)atoi(parts[0].c_str());
	data[31] = (byte)atoi(parts[1].c_str());
	data[32] = (byte)atoi(parts[2].c_str());
	data[33] = (byte)atoi(parts[3].c_str());
}

void PcapReaderNetMap::UpdateScrIP(u_char *& data, std::string strScrIP)
{
	std::vector<std::string> parts = string_split(strScrIP, '.');
	data[26] = (byte)atoi(parts[0].c_str());
	data[27] = (byte)atoi(parts[1].c_str());
	data[28] = (byte)atoi(parts[2].c_str());
	data[29] = (byte)atoi(parts[3].c_str());
}

std::vector<std::string> PcapReaderNetMap::string_split(const std::string& s, const char& delim){
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;

}
