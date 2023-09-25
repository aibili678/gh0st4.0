#if !defined(AFX_COMMUNIQUEDLG_H__67D99916_9208_4A68_926F_1CDBE5B78D08__INCLUDED_)
#define AFX_COMMUNIQUEDLG_H__67D99916_9208_4A68_926F_1CDBE5B78D08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CommuniqueDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommuniqueDlg dialog

class CCommuniqueDlg : public CDialog
{
// Construction
public:
	CCommuniqueDlg(CWnd* pParent = NULL);   // standard constructor
	CString szMent;
// Dialog Data
	//{{AFX_DATA(CCommuniqueDlg)
	enum { IDD = IDD_COMMUNIQUE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommuniqueDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCommuniqueDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMUNIQUEDLG_H__67D99916_9208_4A68_926F_1CDBE5B78D08__INCLUDED_)
