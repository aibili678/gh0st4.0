#if !defined(AFX_MIANZESHENGMING_H__99D6CC0D_0C13_4D42_B447_A7201811A80F__INCLUDED_)
#define AFX_MIANZESHENGMING_H__99D6CC0D_0C13_4D42_B447_A7201811A80F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MianZeShengMing.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMianZeShengMing dialog

class CMianZeShengMing : public CDialog
{
// Construction
public:
	CMianZeShengMing(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMianZeShengMing)
	enum { IDD = IDD_MIANZESHENGMING };
	CEdit	m_Edit;
	CString	m_Text;
	BOOL	m_CloseAnimate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMianZeShengMing)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMianZeShengMing)
	afx_msg void OnClose();
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIANZESHENGMING_H__99D6CC0D_0C13_4D42_B447_A7201811A80F__INCLUDED_)
