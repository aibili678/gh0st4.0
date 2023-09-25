#if !defined(AFX_CLOSEDLG_H__D1DFD7B1_4D44_483C_8B4D_EA5E8D6F61BB__INCLUDED_)
#define AFX_CLOSEDLG_H__D1DFD7B1_4D44_483C_8B4D_EA5E8D6F61BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CloseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCloseDlg dialog

class CCloseDlg : public CDialog
{
// Construction
public:
	CCloseDlg(CWnd* pParent = NULL);   // standard constructor
	WORD iCount;
// Dialog Data
	//{{AFX_DATA(CCloseDlg)
	enum { IDD = IDD_CLOSE_DIALOG };
	CProgressCtrl	m_Showing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCloseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCloseDlg)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOSEDLG_H__D1DFD7B1_4D44_483C_8B4D_EA5E8D6F61BB__INCLUDED_)
