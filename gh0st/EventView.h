#if !defined(AFX_EVENTVIEW_H__F57156AA_B21C_4405_B272_9556D54CF78D__INCLUDED_)
#define AFX_EVENTVIEW_H__F57156AA_B21C_4405_B272_9556D54CF78D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventView.h : header file
//
#define CListView CCJListView		//加了这个让界面更平
/////////////////////////////////////////////////////////////////////////////
// CEventView view

class CEventView : public CListView
{
protected:
	CEventView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEventView)
		
		// Attributes
public:
	
	// Operations
public:
	void InsertItem(LPCTSTR Text,int Mode);
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CEventView();
	int m_nCount;
	CEventView		*pEventView;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CEventView)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteItem();
	afx_msg void OnEmptyItem();
	afx_msg void OnSaveLog();
	afx_msg void OnCopyToClipboard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl *m_pListCtrl;
	CImageList   I_ImageList;
	afx_msg LRESULT OnMyInitialUpdate(WPARAM /*wParam*/, LPARAM /*lParam*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTVIEW_H__F57156AA_B21C_4405_B272_9556D54CF78D__INCLUDED_)
