// CloseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "CloseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCloseDlg dialog


CCloseDlg::CCloseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCloseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCloseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	iCount = 0;
}


void CCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCloseDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Showing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCloseDlg, CDialog)
	//{{AFX_MSG_MAP(CCloseDlg)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCloseDlg message handlers

void CCloseDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x100)
	{
		if(iCount++ == 10)
		{
			KillTimer(0x100);
			OnOK();
		}
			switch (iCount)
		{
		case 2: 
			SetDlgItemText(IDC_STATIC_SHOW,"ÕýÔÚ¹Ø±Õ£¬ghost4.0");
		}
			

		m_Showing.SetPos(iCount);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CCloseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Showing.SetRange32(1,10);
	m_Showing.SetPos(1);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCloseDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	SetTimer(0x100,200,NULL);
	// TODO: Add your message handler code here
}
