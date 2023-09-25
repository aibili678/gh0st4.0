#if !defined(AFX_SKIN_H__7E2C5ECF_E1BE_401B_A07B_A4E66DBC4FBB__INCLUDED_)
#define AFX_SKIN_H__7E2C5ECF_E1BE_401B_A07B_A4E66DBC4FBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Skin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkin dialog

class CSkin : public CDialog
{
// Construction
public:
	CSkin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSkin)
	enum { IDD = IDD_SKIN_COLOUR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSkin)
	afx_msg void OnSetSkin();
	afx_msg void OnExit();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKIN_H__7E2C5ECF_E1BE_401B_A07B_A4E66DBC4FBB__INCLUDED_)
