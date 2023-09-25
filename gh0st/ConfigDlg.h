#if !defined(AFX_CONFIGDLG_H__35DDE668_6CD1_4F5B_8946_E8E6D6201705__INCLUDED_)
#define AFX_CONFIGDLG_H__35DDE668_6CD1_4F5B_8946_E8E6D6201705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG_DIALOG };
	UINT	m_listen_port;
	UINT	m_max_connections;
	BOOL	m_bIsDisablePopTips;
	BOOL	m_Tray;
	BOOL	m_Animate;
	BOOL	m_connect_auto;
	BOOL	m_Sound;
	UINT	m_check_time;
	BOOL	m_Appment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	afx_msg void OnButtonApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__35DDE668_6CD1_4F5B_8946_E8E6D6201705__INCLUDED_)
