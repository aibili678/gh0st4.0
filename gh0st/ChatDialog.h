#if !defined(AFX_CHATDIALOG_H__5E21F3E0_8413_4F0F_AB62_A71195C1D621__INCLUDED_)
#define AFX_CHATDIALOG_H__5E21F3E0_8413_4F0F_AB62_A71195C1D621__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatDialog dialog
#include "gh0stView.h"

class CChatDialog : public CDialog
{
// Construction
public:
	CChatDialog(CWnd* pParent = NULL);   // standard constructor
	UINT m_msgButton;
	UINT m_msgIcon;
	CGh0stView *Point;
// Dialog Data
	//{{AFX_DATA(CChatDialog)
	enum { IDD = IDD_CHAT_DIALOG };
	CStatic	m_Icons4;
	CStatic	m_Icons3;
	CStatic	m_Icons2;
	CStatic	m_Icons1;
	CString	m_Title;
	CString	m_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChatDialog)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnButton3();
	afx_msg void OnButton1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio12();
	afx_msg void OnRadio10();
	afx_msg void OnRadio13();
	afx_msg void OnRadio7();
	afx_msg void OnRadio11();
	afx_msg void OnRadio5();
	afx_msg void OnRadio8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDIALOG_H__5E21F3E0_8413_4F0F_AB62_A71195C1D621__INCLUDED_)
