// ChatDialog.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "ChatDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatDialog dialog


CChatDialog::CChatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatDialog)
	m_Title = _T("世界你好..");
	m_Text = _T("注意: 消息是针对选中的主机..\r\n也就是复选框已经勾选的主机\r\n多选可能产生群发的效果.\r\n");
	//}}AFX_DATA_INIT
	m_msgButton = MB_OK;
	m_msgIcon = MB_ICONWARNING;
	Point = NULL;
}


void CChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatDialog)
	DDX_Control(pDX, IDC_STATIC_ICON4, m_Icons4);
	DDX_Control(pDX, IDC_STATIC_ICON3, m_Icons3);
	DDX_Control(pDX, IDC_STATIC_ICON2, m_Icons2);
	DDX_Control(pDX, IDC_STATIC_ICON1, m_Icons1);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_Title);
	DDV_MaxChars(pDX, m_Title, 200);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_Text);
	DDV_MaxChars(pDX, m_Text, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatDialog, CDialog)
	//{{AFX_MSG_MAP(CChatDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO12, OnRadio12)
	ON_BN_CLICKED(IDC_RADIO10, OnRadio10)
	ON_BN_CLICKED(IDC_RADIO13, OnRadio13)
	ON_BN_CLICKED(IDC_RADIO7, OnRadio7)
	ON_BN_CLICKED(IDC_RADIO11, OnRadio11)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO8, OnRadio8)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDialog message handlers

void CChatDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CChatDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_Icons1.SetIcon(LoadIcon(NULL,IDI_WARNING));
	m_Icons2.SetIcon(LoadIcon(NULL,IDI_ERROR));
	m_Icons3.SetIcon(LoadIcon(NULL,IDI_INFORMATION));
	m_Icons4.SetIcon(LoadIcon(NULL,IDI_QUESTION));
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	((CButton *)GetDlgItem(IDC_RADIO12))->SetCheck(TRUE);

	if(Point==NULL)
		SetWindowText("初始化失败！");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


struct CHAT
{
	CHAR Title[200];
	CHAR szText[1000];
	UINT Type;
}m_ChatSurt;


void CChatDialog::OnButton3() 
{
	// TODO: Add your control notification handler code here
	ZeroMemory(&m_ChatSurt,sizeof(CHAT));
	strcpy(m_ChatSurt.Title,m_Title.GetBuffer(0));
	strcpy(m_ChatSurt.szText,m_Text.GetBuffer(0));
	m_ChatSurt.Type = m_msgButton|m_msgIcon;

	BYTE	*bToken = new BYTE[sizeof(CHAT)+1];
	bToken[0]= COMMAND_CHAT_TEXT;
	memcpy(&bToken[1],&m_ChatSurt,sizeof(CHAT));

	Point->SendChatCommand(bToken, sizeof(CHAT)+1);

	delete bToken;
}

void CChatDialog::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MessageBox(m_Text.GetBuffer(0),m_Title.GetBuffer(0),m_msgIcon|m_msgButton);
}


void CChatDialog::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONWARNING;
}

void CChatDialog::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONERROR;
}

void CChatDialog::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONINFORMATION;
}

void CChatDialog::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_msgIcon = MB_ICONQUESTION;
}

void CChatDialog::OnRadio12() 
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_OK;	
}

void CChatDialog::OnRadio10() 
{
	// TODO: Add your control notification handler code here
	m_msgButton =MB_HELP;
}

void CChatDialog::OnRadio13() 
{
	// TODO: Add your control notification handler code here
	m_msgButton =MB_YESNOCANCEL;
}

void CChatDialog::OnRadio7() 
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_YESNO;
}

void CChatDialog::OnRadio11() 
{
	// TODO: Add your control notification handler code here
	m_msgButton = MB_CANCELTRYCONTINUE;
}

void CChatDialog::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_msgButton =MB_OKCANCEL;
}

void CChatDialog::OnRadio8() 
{
	// TODO: Add your control notification handler code here
	m_msgButton =MB_ABORTRETRYIGNORE;
}
