#if !defined(AFX_BUILDDIALOG_H__88AF1EDE_B24A_4F4A_ADAA_816ABCB1B8A6__INCLUDED_)
#define AFX_BUILDDIALOG_H__88AF1EDE_B24A_4F4A_ADAA_816ABCB1B8A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuildDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog dialog

static struct SERVERINFO
{
	CHAR ServiceName[128];
	CHAR ServiceShow[200];
	CHAR ServiceDescribe[300];
	CHAR ReleasePath[MAX_PATH];
	WORD FileAttribute;
}m_ServiceInfo;


static struct ONLINEINFO
{
	CHAR DNS[300];     //上线DNS
	CHAR URL[300];     //HTPP上线
	WORD Port;         //上线端口
	WORD Type;         //上线类型
	CHAR szMark[100];       //上线分组
}m_OnlineInfo;


class CBuildDialog : public CDialog
{
// Construction
public:
	CBuildDialog(CWnd* pParent = NULL);   // standard constructor
	CString szIniPath;
	static DWORD WINAPI WorkingThread(LPVOID lParam);
	static WORD ShowPosting;
	VOID SetShowTips(LPSTR szStr);
	BOOL InitializationOnlineInfo(LPSTR);
	BOOL CreateServer(SERVERINFO *lpData,ONLINEINFO *lpOnline,LPSTR szPath);
// Dialog Data
	//{{AFX_DATA(CBuildDialog)
	enum { IDD = IDD_BUILDDIALOG };
	CComboBox	m_FileSuffiex;
	CComboBox	m_Directory;
	CComboBox	m_FileAttribute;
	CProgressCtrl	m_Showing;
	CString	m_ServerName;
	CString	m_ServerDisplay;
	CString	m_ServerDescribe;
	CString	m_FileName;
	CString	m_Dns;
	BOOL	m_CheckUrl;
	CString	m_UrlAddress;
	UINT	m_Port;
	CString	m_ShowTips;
	CString	m_OnlineMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuildDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBuildDialog)
	afx_msg void OnBuild();
	virtual BOOL OnInitDialog();
	afx_msg void OnTest();
	afx_msg void OnCheckUrl();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRandom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDDIALOG_H__88AF1EDE_B24A_4F4A_ADAA_816ABCB1B8A6__INCLUDED_)
