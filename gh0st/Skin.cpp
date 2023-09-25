// Skin.cpp : implementation file
//

#include "stdafx.h"
#include "gh0st.h"
#include "Skin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkin dialog


CSkin::CSkin(CWnd* pParent /*=NULL*/)
	: CDialog(CSkin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSkin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkin, CDialog)
	//{{AFX_MSG_MAP(CSkin)
	ON_BN_CLICKED(IDC_SET_SKIN, OnSetSkin)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnReleasedcaptureSlider2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, OnReleasedcaptureSlider3)
	//}}AFX_MSG_MAP
	END_MESSAGE_MAP()
	
	/////////////////////////////////////////////////////////////////////////////
	// CSkin message handlers
	
	void CSkin::OnSetSkin() 
{
	// TODO: Add your control notification handler code here
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Æ¤·ô", "É«µ÷",((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos());
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Æ¤·ô", "±¥ºÍ",((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos());
	((CGh0stApp *)AfxGetApp())->m_IniFile.SetInt("Æ¤·ô", "ÁÁ¶È",((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos());
}

void CSkin::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CSkin::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SkinH_AdjustHSV( ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos(),
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER2))->GetPos(), 
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER3))->GetPos()
		);
	
	*pResult = 0;
}

void CSkin::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnReleasedcaptureSlider1(pNMHDR, pResult); 	
	*pResult = 0;
}

void CSkin::OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnReleasedcaptureSlider1(pNMHDR, pResult);
	*pResult = 0;
}

BOOL CSkin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CSliderCtrl* pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	pSlider->SetRange(-180,180,TRUE);
	pSlider->SetPos(0);
	
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER2);
	pSlider->SetRange(-100,100,TRUE);
	pSlider->SetPos(0);
	
	pSlider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER3);
	pSlider->SetRange(-100,100,TRUE);
	pSlider->SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
