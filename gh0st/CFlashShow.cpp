// DlgFlash.cpp: implementation of the CDlgFlash class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFlashShow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgFlash::CDlgFlash(CDialog* Dlg)
{
	flashDlg=Dlg;
	Timer1_ID=9;
	Timer2_ID=10;
	InitCDlg();
}
CDlgFlash::CDlgFlash(CDialog* Dlg,int Timer1_ID,int Timer2_ID)
{
	flashDlg=Dlg;
	this->Timer1_ID=Timer1_ID;
	this->Timer2_ID=Timer2_ID;
	InitCDlg();
}
CDlgFlash::~CDlgFlash()
{
}

void CDlgFlash::InitCDlg(){
	//��ô���Ԥ��Ĵ�С
	CRect dlgRect;
	flashDlg->GetWindowRect(dlgRect);
	//��ȡ��Ļ��С
	CRect desktopRect;
	flashDlg->GetDesktopWindow()->GetWindowRect(desktopRect);
	//�����ڿ�ʼ��С��Ϊ0������
	flashDlg->MoveWindow(dlgRect.left + dlgRect.Width() / 2,
						  dlgRect.top + dlgRect.Height() / 2,
						   0,
						   0 
						   );
	//���ݳ�Ա��ʼ��
	OriginalWidth=dlgRect.Width();
	OriginalHeight=dlgRect.Height();
	xCreateSpeed=2;
	yCreateSpeed=3;
	xDestorySpeed=2;
	yDestorySpeed=3;

	//�趨��ʱ��Timer1_ID	
	flashDlg->SetTimer(Timer1_ID,10,NULL);
	
}
void CDlgFlash::OnTimer(UINT nIDEvent){
	{
	//��ô�ʱ���ڵ�ʵ�ʴ�С
	CRect dlgRect;
	flashDlg->GetWindowRect(&dlgRect);
	//�������Ĵ�С
	CRect desktopRect;
	flashDlg->GetDesktopWindow()->GetWindowRect(desktopRect);
	//����Ǵ��ڵ������̣����ھ��У������󴰿�
	if(nIDEvent ==Timer1_ID)
	{
		flashDlg->MoveWindow(
				  (desktopRect.Width() - dlgRect.Width()) / 2,
				  (desktopRect.Height() - dlgRect.Height()) / 2, 
				  xCreateSpeed+dlgRect.Width(),
				  yCreateSpeed+dlgRect.Height() 
				  );
		
		//��Ҫ��������Ԥ��Ŀ��
		if(dlgRect.Width() >=OriginalWidth) 
			xCreateSpeed=0; 
		//��Ҫ��������Ԥ��ĸ߶�
		if(dlgRect.Height() >=OriginalHeight)
			yCreateSpeed=0;
		//ֹͣ�仯���رն�ʱ��Timer1_ID
		if((dlgRect.Width() >=OriginalWidth) && (dlgRect.Height() >=OriginalHeight))
			flashDlg->KillTimer(Timer1_ID); 
	}
	
	//����Ǵ��ڹرչ��̣�������С����
	if(nIDEvent == Timer2_ID)
	{
		flashDlg->MoveWindow(DestoryPosX,
							DestoryPosY ,
							dlgRect.Width(),
							dlgRect.Height()-yDestorySpeed
							);
		
		//���߶ȵ������߶ȾͲ��ڱ仯
		//if(dlgRect.Height() <=0)
			 
		//ֹͣ�仯���رն�ʱ��Timer2_ID�����ҹرմ���
		if(dlgRect.Height() <=0)
		{
			yDestorySpeed=0;
			flashDlg->KillTimer(Timer1_ID);
			flashDlg->DestroyWindow();
		}
		
	}
}
}
void CDlgFlash::StartFlashDestroyDlg(){
	CRect dlgRect;
	//��ȡ���ڵ�ǰ��λ�ò�����
	flashDlg->GetWindowRect(&dlgRect);
	DestoryPosX=dlgRect.left;
	DestoryPosY=dlgRect.top;
	//��ʼ������ʱ��Timer2_ID
	flashDlg->SetTimer(Timer2_ID,10,NULL);
}

void CDlgFlash::SetFlashCreateSpeed(int x,int y){
	xCreateSpeed=x;
	xCreateSpeed=y;
}

void CDlgFlash::SetFlashDestroySpeed(int y){
	yDestorySpeed=y;
}