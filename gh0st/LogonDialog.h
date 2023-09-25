#if !defined(AFX_LOGONDIALOG_H__59CDBBF2_1D5B_4A49_8DBB_13BD15711A1C__INCLUDED_)
#define AFX_LOGONDIALOG_H__59CDBBF2_1D5B_4A49_8DBB_13BD15711A1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogonDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogonDialog dialog
#include "TaskbarNotifier.h"

class CLogonDialog : public CDialog
{
// Construction
public:
	CLogonDialog(CWnd* pParent = NULL);   // standard constructor
	static DWORD WINAPI WorkThread(LPVOID lParam);
	static WORD iCount;
	static BOOL Success;
	static CHAR szSend[500];
// Dialog Data
	//{{AFX_DATA(CLogonDialog)
	enum { IDD = IDD_LOGON_DIALOG };
	CProgressCtrl	m_Showing;
	CString	m_UserName;
	CString	m_Pass;
	BOOL	m_Remeber;
	BOOL	m_AutoLogon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTaskbarNotifier m_wndTaskbarNotifier;
	// Generated message map functions
	//{{AFX_MSG(CLogonDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRemember();
	afx_msg void OnAutoLogon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGONDIALOG_H__59CDBBF2_1D5B_4A49_8DBB_13BD15711A1C__INCLUDED_)
