// CommuniqueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "CommuniqueDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommuniqueDlg dialog


CCommuniqueDlg::CCommuniqueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommuniqueDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommuniqueDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCommuniqueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommuniqueDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommuniqueDlg, CDialog)
	//{{AFX_MSG_MAP(CCommuniqueDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommuniqueDlg message handlers

void CCommuniqueDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	SetDlgItemText(IDC_STATIC_SHOW,szMent);
}
