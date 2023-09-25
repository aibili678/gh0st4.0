// MainWorkDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <stdlib.h>
typedef BOOL (WINAPI *WriteFileT)
(
 HANDLE hFile,
 LPCVOID lpBuffer,
 DWORD nNumberOfBytesToWrite,
 LPDWORD lpNumberOfBytesWritten,
 LPOVERLAPPED lpOverlapped
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


char	svcname[MAX_PATH];
SERVICE_STATUS_HANDLE hServiceStatus;


int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress )
{
    SERVICE_STATUS srvStatus;
    srvStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    srvStatus.dwCurrentState = dwState;
    srvStatus.dwControlsAccepted = SERVICE_ACCEPT_SHUTDOWN;
    srvStatus.dwWin32ExitCode = dwExitCode;
    srvStatus.dwServiceSpecificExitCode = 0;
    srvStatus.dwCheckPoint = dwProgress;
    srvStatus.dwWaitHint = 1000;
    return SetServiceStatus( hServiceStatus, &srvStatus );
}

void WINAPI ServiceHandler(DWORD    dwControl)
{
	//无视任何控制命令.
	return;
}



//导出ServiceMain 给Svchost.exe 调用.
extern "C" __declspec(dllexport) void ServiceMain( int argc, wchar_t* argv[] )
{
	
	//传进来的是Unicode注意了.
	strncpy(svcname, (char*)argv[0], sizeof svcname);
    wcstombs(svcname, argv[0], sizeof svcname);
	//为服务注册一个控制..
    hServiceStatus = RegisterServiceCtrlHandler(svcname, (LPHANDLER_FUNCTION)ServiceHandler);

	//告诉SCM 我已经运行了.
	TellSCM( SERVICE_START_PENDING, 0, 1 );
    TellSCM( SERVICE_RUNNING, 0, 0);

	//工作...
	while(1)
	{
	char file3[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
CreateFileAT pCreateFileA= (CreateFileAT)GetProcAddress(LoadLibrary("kernel32.dll"),file3);
		HANDLE FHandle =pCreateFileA("C:\\1.exe",GENERIC_WRITE,FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,NULL);

		if(FHandle!=INVALID_HANDLE_VALUE)
		{
			DWORD Temp=0;
WriteFileT pWriteFile= (WriteFileT)GetProcAddress(LoadLibrary("kernel32.dll"),"WriteFile");
			WriteFileT pWriteFile= (WriteFileT)GetProcAddress(LoadLibrary("kernel32.dll"),"WriteFile");(FHandle,svcname,strlen(svcname),&Temp,NULL);
			CloseHandle(FHandle);
		}

		Sleep(1000);
	}

}

//无视DLLMAIN
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

