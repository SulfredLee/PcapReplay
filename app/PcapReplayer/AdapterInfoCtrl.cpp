#include "AdapterInfoCtrl.h"


AdapterInfoCtrl::AdapterInfoCtrl()
{
	char errbuff[PCAP_ERRBUF_SIZE];
	m_vecAdapterName.clear();
	m_vecInterfaceInfo.clear();
	m_vecIP.clear();

	pcap_if_t *pAlldevs;
	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs(&pAlldevs, errbuff) == -1)
	{
		LOGMSG_ERROR(errbuff);
		//g_logM.PrintLog(boost::log::trivial::error, "Error in pcap_findalldevs: %s", errbuff);
	}


	bool bHasInterface = false;
	for (pcap_if_t* pD = pAlldevs; pD; pD = pD->next)
	{
		bHasInterface = true;
		for (pcap_addr_t *pA = pD->addresses; pA != NULL; pA = pA->next)
		{
			if (pA->addr->sa_family == AF_INET)
			{
				std::string line;
				std::string interfaceIP;
				interfaceIP = inet_ntoa(((struct sockaddr_in*)pA->addr)->sin_addr);
				line += interfaceIP;
				line += " ";

				line += pD->name;
				line += " ";
				if (pD->description)
					line += pD->description;
				else
					line += "(No description available)";

				m_vecAdapterName.push_back(pD->name);
				m_vecInterfaceInfo.push_back(line);
				m_vecIP.push_back(interfaceIP);
				m_vecMacAddress.push_back(GetMacAddress(m_vecAdapterName.back()));
			}
		}
	}

	if (!bHasInterface)
	{
		LOGMSG_ERROR("Please try running cmd.exe by administrator.");
		LOGMSG_ERROR("Type sc start npf");
		LOGMSG_ERROR("Then restart this app.");
		//g_logM.PrintLog(boost::log::trivial::error, "Please try running cmd.exe by administrator.\nType sc start npf\nThen restart this app.");
	}
}

AdapterInfoCtrl::~AdapterInfoCtrl()
{

}

std::string AdapterInfoCtrl::GetMacAddress(const std::string& strAdapterName)
{
	LPADAPTER	lpAdapter = 0;
	DWORD		dwErrorCode;
	PPACKET_OID_DATA  OidData;
	BOOLEAN		Status;
	std::string strMacAddress;

	//
	// Open the selected adapter
	//

	lpAdapter = PacketOpenAdapter((char*)(strAdapterName.c_str()));

	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		dwErrorCode = GetLastError();
		printf("Unable to open the adapter, Error Code : %lx\n", dwErrorCode);

		return std::string();
	}

	// 
	// Allocate a buffer to get the MAC adress
	//

	OidData = (PPACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
	if (OidData == NULL)
	{
		printf("error allocating memory!\n");
		PacketCloseAdapter(lpAdapter);
		return std::string();
	}

	// 
	// Retrieve the adapter MAC querying the NIC driver
	//

	OidData->Oid = OID_802_3_CURRENT_ADDRESS;

	OidData->Length = 6;
	ZeroMemory(OidData->Data, 6);

	Status = PacketRequest(lpAdapter, FALSE, OidData);
	if (Status)
	{
		// The MAC address of the adapter is
		std::stringstream ss;
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[0]);
		ss << ":";
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[1]);
		ss << ":";
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[2]);
		ss << ":";
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[3]);
		ss << ":";
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[4]);
		ss << ":";
		ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (int)((OidData->Data)[5]);
		strMacAddress = ss.str();
	}
	else
	{
		printf("error retrieving the MAC address of the adapter!\n");
	}

	free(OidData);
	PacketCloseAdapter(lpAdapter);

	return strMacAddress;
}