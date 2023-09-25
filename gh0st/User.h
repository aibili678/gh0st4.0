#if !defined(AFX_USER_H__C044AD89_F07C_4F92_957B_568AE8ED61FC__INCLUDED_)
#define AFX_USER_H__C044AD89_F07C_4F92_957B_568AE8ED61FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// User.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUser dialog

class CUser : public CDialog
{
	// Construction
public:
	CUser(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CUser)
	enum { IDD = IDD_ADD_USER };
	CString	m_admin;
	CString	m_net_pass;
	CString	m_net_user;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUser)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CUser)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USER_H__C044AD89_F07C_4F92_957B_568AE8ED61FC__INCLUDED_)
