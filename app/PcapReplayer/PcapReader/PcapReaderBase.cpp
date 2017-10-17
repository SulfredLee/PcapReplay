#include "PcapReaderBase.h"

PcapReaderBase::PcapReaderBase()
{
}


PcapReaderBase::~PcapReaderBase()
{
}

void PcapReaderBase::StartReading()
{
	char errbuff[PCAP_ERRBUF_SIZE];
	pcap_t * pcap = pcap_open_offline(m_inputFile.c_str(), errbuff);
	{
		std::ostringstream ss;
		ss << "Reading: " << m_inputFile.c_str();
		LOGMSG_ERROR(ss.str());
		//g_logM.PrintLog(boost::log::trivial::info, "Reading: %s\n", m_inputFile.c_str());
	}
	if (pcap == NULL)
	{
		std::ostringstream ss;
		ss << "Error: " << errbuff;
		LOGMSG_ERROR(ss.str());
		//g_logM.PrintLog(boost::log::trivial::error, "Error: %s\n", errbuff);
		return;
	}

	struct pcap_pkthdr *header;
	const u_char *data;
	u_int packetCount = 0;
	m_ullCurFileSize = FileSize(m_inputFile);
	m_ullCurSentSize = PCAPGOBLEHEADERSIZE;

	StartReadOneFile();
	while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
	{
		ParseOnePkt(pcap, header, data);
		if (myThread::IsEndThread())
			return;
	}
	FinishOneFile();
}

//Override
void PcapReaderBase::ThreadMain()
{
	for (int i = 0; i < m_inputFiles.size(); i++)
	{
		m_inputFile = m_inputFiles[i];
		StartReading();
		if (myThread::IsEndThread())
			return;
	}

	if (m_listener != NULL)
		m_listener->ThreadFinishCallBack(m_objID);
}

boost::uintmax_t PcapReaderBase::FileSize(const std::string& filename)
//std::ifstream::pos_type PcapReaderBase::FileSize(const std::string& filename)
{
	return boost::filesystem::file_size(filename);
	//std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	//return in.tellg();
}

void PcapReaderBase::ReCalculateCheckSum(u_char* pData, unsigned int unDataLen)
{
	// reset checksum
	pData[40] = 0;
	pData[41] = 0;
	
	uint32_t unChecksum = 0;
	const uint16_t* pD16;
	const uint8_t* pD8;
	// handle IP layer
	//		handle src IP
	pD16 = reinterpret_cast<const uint16_t*>(pData + 26);
	unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
	unChecksum += _byteswap_ushort(*pD16);
	//		handle dst IP
	pD16 = reinterpret_cast<const uint16_t*>(pData + 30);
	unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
	unChecksum += _byteswap_ushort(*pD16);
	//		handle portocol
	pD8 = reinterpret_cast<const uint8_t*>(pData + 23);
	unChecksum += *pD8;
	//		handle data lenght, from IP layer to udp data layer
	pD16 = reinterpret_cast<const uint16_t*>(pData + 38);
	unChecksum += _byteswap_ushort(*pD16);


	// handle UDP layer
	//		handle src Port
	pD16 = reinterpret_cast<const uint16_t*>(pData + 34);
	unChecksum += _byteswap_ushort(*pD16);
	//		handle dst Port
	pD16 = reinterpret_cast<const uint16_t*>(pData + 36);
	unChecksum += _byteswap_ushort(*pD16);	
	//		handle data lenght, from IP layer to udp data layer
	pD16 = reinterpret_cast<const uint16_t*>(pData + 38);
	unChecksum += _byteswap_ushort(*pD16);
	
	// handle udp data
	size_t len = unDataLen - DATAGRAME_DATA_OFFSET; // 42 is the Ethernet header
	pD16 = reinterpret_cast<const uint16_t*>(pData + DATAGRAME_DATA_OFFSET);
	while (len > 1)
	{
		unChecksum += _byteswap_ushort(*pD16++); // since wireshark file is big-endian, but c++ in PC is little-endian
		len -= sizeof(uint16_t);
	}
	if (len) // if total lenght the data is odd
	{
		unChecksum += *reinterpret_cast<const uint8_t*>(pD16);
	}

	
	while (unChecksum >> 16)
	{
		unChecksum = (unChecksum & 0xffff) + (unChecksum >> 16);
	}
	
	// set checksum back to data
	uint16_t un16TempChecksum = static_cast<uint16_t>(~unChecksum);
	un16TempChecksum = _byteswap_ushort(un16TempChecksum);
	memcpy((char*)(pData + 40), &un16TempChecksum, sizeof(uint16_t));
}