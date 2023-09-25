// EventView.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "EventView.h"
#include "MainFrm.h"
#include "gh0stView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WM_MYINITIALUPDATE (WM_USER + 101)

typedef struct
{
	char	*title;
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT e_Column_Data[] = 
{
	{"��־��Ϣ",		160},
	{"IP/����λ��",		686}
};

int e_Column_Width = 0;
int	e_Column_Count = 2;

/////////////////////////////////////////////////////////////////////////////
// CEventView

IMPLEMENT_DYNCREATE(CEventView, CListView)

CEventView::CEventView()
{
	//((CGh0stApp *)AfxGetApp())->m_pConnectView = this;//�����仰���˲�ǳ������Ĳ������ˣ�������һ�죨���ܼ������䣩
	//m_pListCtrl = NULL;
}

CEventView::~CEventView()
{
}

BEGIN_MESSAGE_MAP(CEventView, CListView)
	//{{AFX_MSG_MAP(CEventView)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_DELETE_ITEM, OnDeleteItem)
	ON_COMMAND(ID_EMPTY_ITEM, OnEmptyItem)
	ON_MESSAGE(WM_MYINITIALUPDATE, OnMyInitialUpdate)
	ON_COMMAND(ID_SAVE_LOG, OnSaveLog)
	ON_COMMAND(ID_COPY, OnCopyToClipboard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventView drawing
/////////////////////////////////////////////////////////////////////////////
// CEventView diagnostics

#ifdef _DEBUG
void CEventView::AssertValid() const
{
	CListView::AssertValid();
}

void CEventView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEventView message handlers

void CEventView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}
BOOL CEventView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}
LRESULT CEventView::OnMyInitialUpdate(WPARAM, LPARAM)
{
	CEventView* pView = this;
 	((CMainFrame*)AfxGetApp()->m_pMainWnd)->SetCurView(0);
	return 0;
}

void CEventView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_LOG_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}
void CEventView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	/**************************************************************************/
	//�ý����Ư��
	NONCLIENTMETRICS ncm;
	memset(&ncm, 0, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS), &ncm, 0));
	ncm.lfMessageFont.lfWeight = 500;
	m_headerCtrl.m_HeaderFont.CreateFontIndirect(&ncm.lfMessageFont);
	SetFont(&(m_headerCtrl.m_HeaderFont));

	/**************************************************************************/
	m_pListCtrl = &GetListCtrl();

	I_ImageList.Create(16, 16, ILC_COLOR32,10, 0);
	HICON hIcon = NULL;//����Ϊ����3��ͼ����Դ
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_EVENT), IMAGE_ICON, 16, 16, 0);
	I_ImageList.Add(hIcon);
	ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL);

	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB|
			 LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);//LVS_EX_ONECLICKACTIVATE |  ��URL�»���

	for (int i = 0; i < e_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, e_Column_Data[i].title,LVCFMT_LEFT,-1);
		m_pListCtrl->SetColumnWidth(i, e_Column_Data[i].nWidth);
		e_Column_Width += e_Column_Data[i].nWidth; // �ܿ��
	}

	/*CRect   rc;
	GetWindowRect(   &rc   );
	int   width   =   rc.Width();
	int   height=   rc.Height();
	m_pListCtrl->InsertColumn(0,"�¼�����ʱ��",LVCFMT_LEFT,160);
	m_pListCtrl->InsertColumn(1,"�¼������б�",LVCFMT_LEFT,850);
	//m_pListCtrl->SetColumnWidth(1, width + 138); */
	// �������ݶ�
	SetColumnNumeric(0);
	SetColumnNumeric(2);
	PostMessage(WM_MYINITIALUPDATE);
}

void CEventView::InsertItem(LPCTSTR Text,int Mode)
{
	char m_Text[512] = {0};
	CTime time = CTime::GetCurrentTime();		//����CTime���� 
	CString strTime = time.Format("[%Y-%m-%d %H:%M:%S]");
	int nCount = GetListCtrl().GetItemCount();
	m_pListCtrl->InsertItem(nCount, strTime, 0);//int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	m_pListCtrl->SetItemText(nCount, 1, Text);  //BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );
	switch(Mode)
	{
	case 0:
		{    
			m_pListCtrl->SetTextColor(RGB(246,45,244));//��ɫ��ʾ
			//m_pListCtrl->SetTextColor(RGB(255,0,0));//��ɫ
			//m_pListCtrl->SetTextColor(RGB(0,20,100));//ò�ƺ�ɫ
			//m_pListCtrl->SetTextBkColor(RGB(0,247,255));
			//m_pListCtrl->SetBkColor(RGB(0,247,255));
		}
		break;
	/*
	case 1:
		m_pListCtrl->SetTextColor(RGB(0,180,250));//����ɫ
		break;
	case 2:
		m_pListCtrl->SetTextColor(RGB(255,0,0));//��ɫ
		break;
	case 3:
		m_pListCtrl->SetTextColor(RGB(200,0,200));//��ɫ
		break;
	case 4:
		m_pListCtrl->SetTextColor(RGB(0,20,100));//ò�ƺ�ɫ
		break;
	*/
	default:
			m_pListCtrl->SetTextColor(RGB(0,20,100));//ò�ƺ�ɫ
		break;
	}
}

void CEventView::OnDeleteItem() 
{	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if  (pos  ==  NULL)
	{
		::MessageBox(NULL,"����ѡ��Ҫɾ�����¼���¼ ...","��ʾ",MB_ICONINFORMATION);
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_pListCtrl->GetNextSelectedItem(pos);
			m_pListCtrl->DeleteItem(nItem);
			pos = m_pListCtrl->GetFirstSelectedItemPosition();
		}
	}
}

void CEventView::OnEmptyItem() 
{
	m_pListCtrl->DeleteAllItems();

	/*//���Ҳ�У������鷳�˵�
	int nCnt = m_pListCtrl->GetItemCount();
	for(int Tmpi = nCnt ; Tmpi >= 0 ; Tmpi-- )
	{
	m_pListCtrl->DeleteItem(Tmpi);
	}*/
}
void CEventView::OnSaveLog() 
{
	CTime time = CTime::GetCurrentTime(); ///����CTime���� 
	CString strTime = time.Format("%Y-%m-%d %H-%M-%S");

    CFileDialog dlg(FALSE, _T("log"), strTime, OFN_OVERWRITEPROMPT, _T("*.log|*.log|*.txt|*.log"), NULL);
    if (dlg.DoModal() != IDOK)
        return;
    CFile file;
    char strip[50] = {0};
    if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
    {
        POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
        while (pos)
        {
            int nItem = m_pListCtrl->GetNextSelectedItem(pos);
            CString strTitle;
            strTitle.Format(_T("ʱ��:%s �¼�:%s"), m_pListCtrl->GetItemText(nItem, 0),m_pListCtrl->GetItemText(nItem, 1));
			char *strip = strTitle.GetBuffer(strTitle.GetLength() + 1);	//CStringת��Ϊchar*
			strTitle.ReleaseBuffer();
            strcat(strip, "\r\n");
            file.Write(strip, strlen(strip));
            memset(strip, 0, 50);
        }
        file.Close();
		strTime = time.Format("[%Y-%m-%d %H:%M:%S]") + " ��־�����ɹ� ...";
        ::MessageBox(0,strTime,"��ʾ",MB_ICONINFORMATION);
    }
}
/*
�����ݸ��Ƶ�������:   
1.����OpenClipboard()�������ݵ�Դ����.   
2.����EmptyClipboard()��ռ���������ǰ������.   
3.����SetClipboardData()�����ݴ�ŵ���������.   
4.����CloseClipboard()ʹ��Ĵ����ܷ��ʼ�����. 
*/
/*
#include <Winbase.h>
#include <Windows.h>
#pragma comment(lib, "Kernel32.lib")
*/
void CEventView::OnCopyToClipboard() 
{
    int nItem;
    CString strText(_T(""));
    POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	
    //��ȡ����ѡ����Ŀ�����ݡ�
    while (pos)
    {
        nItem = m_pListCtrl->GetNextSelectedItem(pos);
        strText += m_pListCtrl->GetItemText(nItem, 0) + " ";
        strText += m_pListCtrl->GetItemText(nItem, 1) + _T("\r\n");
    }
    //�����ݱ��浽�����塣
    if (!strText.IsEmpty())
    {
        if (OpenClipboard())
        {
            EmptyClipboard();
            HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
            if (hClipboardData)
            {
                TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
                _tcscpy(pszData, strText);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_TEXT, hClipboardData);
            }
            CloseClipboard();
        }
    }
}
