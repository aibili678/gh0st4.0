// LogonDialog.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "LogonDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDialog dialog

WORD CLogonDialog::iCount = 1;
BOOL CLogonDialog::Success = FALSE;
CHAR CLogonDialog::szSend[500]={NULL};

CLogonDialog::CLogonDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDialog)
	m_UserName = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "UserName","");
	m_Pass ="";
	m_Remeber = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Remember",false);
	m_AutoLogon = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "AutoLogon",false);
	//}}AFX_DATA_INIT

	ZeroMemory(szSend,500);
}


void CLogonDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDialog)
	DDX_Control(pDX, IDC_PROGRESS1, m_Showing);
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	DDV_MaxChars(pDX, m_UserName, 20);
	DDX_Text(pDX, IDC_PASSWORD, m_Pass);
	DDV_MaxChars(pDX, m_Pass, 20);
	DDX_Check(pDX, IDC_REMEMBER, m_Remeber);
	DDX_Check(pDX, IDC_AUTO_LOGON, m_AutoLogon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDialog, CDialog)
	//{{AFX_MSG_MAP(CLogonDialog)
	ON_BN_CLICKED(IDC_LOGON, OnLogon)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_REMEMBER, OnRemember)
	ON_BN_CLICKED(IDC_AUTO_LOGON, OnAutoLogon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogonDialog message handlers

BOOL CLogonDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);

	m_Showing.SetRange(1,10);
	m_Showing.SetPos(1);

	SetTimer(0x101,1000,NULL);

	if(m_Remeber)
		m_Pass = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "PassWord","");
	if(m_AutoLogon)
	{
		SetTimer(0x102,500,NULL);
		GetDlgItem(IDC_LOGON)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	m_wndTaskbarNotifier.Create(this);
	m_wndTaskbarNotifier.SetSkin(IDB_BITMAP_INFO);
	m_wndTaskbarNotifier.SetTextFont("宋体",100,TN_TEXT_NORMAL,TN_TEXT_UNDERLINE);
	m_wndTaskbarNotifier.SetTextColor(RGB(255,255,255),RGB(255,0,0));
	m_wndTaskbarNotifier.SetTextRect(CRect(10,40,m_wndTaskbarNotifier.m_nSkinWidth-10,
	m_wndTaskbarNotifier.m_nSkinHeight-25));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



//发送数据
BOOL SendData(SOCKET s, char *data, int len)
{
	char* pData   = data;
	int iHasSend  = 0;
	int iLeftSend = len;
	int iRet      = 0;
	
	if(len <= 0) 
		return TRUE;
	
	while(1)
	{
		iRet = send(s, pData, iLeftSend, 0);
		if(iRet == 0 || iRet == SOCKET_ERROR)
			return FALSE;
		
		iHasSend  += iRet;
		pData     += iRet;
		iLeftSend -= iRet;
		if(iHasSend >= len) 
			break;
	}
	
	return TRUE;
}

//接收数据
BOOL RecvData(SOCKET s, char *data, int len)
{
	char * pData  = data;
	int iHasRecv  = 0;
	int iLeftRecv = len;
	int iRet      = 0;
	
	if(len <= 0) 
		return TRUE;
	while(1)
	{
		iRet = recv(s,pData,iLeftRecv,0);
		if(iRet == 0 || iRet == SOCKET_ERROR)
			return FALSE;
		
		iHasRecv  += iRet;
		pData     += iRet;
		iLeftRecv -= iRet;
		if(iHasRecv >= len) 
			break;
	}
	
	return TRUE;
}

//请求头..
enum 
{
	SOCKET_ADDDATA, //增加数据
	SOCKET_MODIFY,  //修改数据
	SOCKET_SELECT,  //查询数据
	SOCKET_LOGON    //登录
};

//数据头.
struct REQUSET
{
	WORD Type;        //请求类型
	CHAR Name[100];   //用户名
	CHAR Pass[100];   //密码
	CHAR Permissions[100];//开号人..
}m_Request={0};


DWORD WINAPI CLogonDialog::WorkThread(LPVOID lParam)
{
	REQUSET m_SendData={NULL};
	memcpy(&m_SendData,lParam,sizeof(REQUSET));


	CLogonDialog::iCount=1;
	CLogonDialog::Success = FALSE;

	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

	SOCKADDR_IN m_Addr={NULL};
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(SERVERPORT);

	CString szDns = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "ServerAddress","");
	if(szDns.IsEmpty())
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(OFFICIALADDRESS);
	else
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(szDns.GetBuffer(0));
	iCount = 3;

	CHAR szTemp[5]={NULL};

	if(connect(m_Socket,(SOCKADDR *)&m_Addr,sizeof(SOCKADDR))!=SOCKET_ERROR)
	{
		CLogonDialog::iCount = 6;

		if(SendData(m_Socket,(char *)&m_SendData,sizeof(REQUSET)))
		{
			CLogonDialog::iCount = 8;
			RecvData(m_Socket,szTemp,4);
			Sleep(100);
			ZeroMemory(szSend,500);
			RecvData(m_Socket,CLogonDialog::szSend,500);
		}

	}

	if(szTemp[0]=='O' && szTemp[1]=='K')
		CLogonDialog::Success = TRUE;
	if(szTemp[2]=='O' && szTemp[3] == 'K')
	{
		CGh0stApp::bUsePlugin = FALSE;
	}
	CLogonDialog::iCount = 10;
	closesocket(m_Socket);
	return 1;
}

void CLogonDialog::OnLogon() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if(m_UserName.IsEmpty())
	{
		MessageBox("用户名不能为空!");
		return;
	}

	if(m_Pass.IsEmpty())
	{
		MessageBox("密码不能为空!");
		return;
	}
	m_Showing.SetPos(1);
	CLogonDialog::Success = FALSE;
	CLogonDialog::iCount =1;

	m_Request.Type = SOCKET_LOGON;

	strcpy(m_Request.Name,m_UserName.GetBuffer(0));
	strcpy(m_Request.Pass,m_Pass.GetBuffer(0));
	strcpy(m_Request.Permissions,PERMISSIONS);

	SetTimer(0x100,500,NULL);

	CloseHandle(CreateThread(NULL,NULL,WorkThread,&m_Request,NULL,NULL));

}

void CLogonDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x100)
	{
		m_Showing.SetPos(CLogonDialog::iCount);
		if(Success)
		{
			CString szShowTip;
			szShowTip.Format("%s \r\n\r\n祝您使用愉快!\r\n感谢支持 甲壳虫!",CLogonDialog::szSend);

			if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false))
				m_wndTaskbarNotifier.Show(szShowTip.GetBuffer(0),200,200000);
			
			KillTimer(0x100);
			UpdateData();
			((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Settings", "PassWord",m_Pass);
			((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Settings", "UserName",m_UserName);
			MessageBox("登录成功!","提示",MB_OK|MB_ICONINFORMATION);
			OnOK();
		}
		if(CLogonDialog::iCount == 10 && Success == FALSE)
		{
			KillTimer(0x100);
			MessageBox("登录失败!","提示",MB_OK|MB_ICONINFORMATION);
			GetDlgItem(IDC_LOGON)->EnableWindow(TRUE);
		}
	}
	if(nIDEvent == 0x101)
	{
		CString szShow;
		szShow.Format("登录成功后 可以获取账号相关信息 按ESC关闭登录");
		SetDlgItemText(IDC_STATIC_TIP,szShow);
	}
	if(nIDEvent == 0x102)
	{
		OnLogon();
		KillTimer(nIDEvent);
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CLogonDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	//要设置的static颜色
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_TIP)  
	{
		CFont font;
		font.CreatePointFont(100,"宋体");
		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CLogonDialog::OnRemember() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Remember",m_Remeber);
}

void CLogonDialog::OnAutoLogon() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "AutoLogon",m_AutoLogon);
}
