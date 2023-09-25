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
	//获得窗口预设的大小
	CRect dlgRect;
	flashDlg->GetWindowRect(dlgRect);
	//获取屏幕大小
	CRect desktopRect;
	flashDlg->GetDesktopWindow()->GetWindowRect(desktopRect);
	//将窗口开始大小设为0，居中
	flashDlg->MoveWindow(dlgRect.left + dlgRect.Width() / 2,
						  dlgRect.top + dlgRect.Height() / 2,
						   0,
						   0 
						   );
	//数据成员初始化
	OriginalWidth=dlgRect.Width();
	OriginalHeight=dlgRect.Height();
	xCreateSpeed=2;
	yCreateSpeed=3;
	xDestorySpeed=2;
	yDestorySpeed=3;

	//设定定时器Timer1_ID	
	flashDlg->SetTimer(Timer1_ID,10,NULL);
	
}
void CDlgFlash::OnTimer(UINT nIDEvent){
	{
	//获得此时窗口的实际大小
	CRect dlgRect;
	flashDlg->GetWindowRect(&dlgRect);
	//获得桌面的大小
	CRect desktopRect;
	flashDlg->GetDesktopWindow()->GetWindowRect(desktopRect);
	//如果是窗口弹出过程，窗口居中，逐渐增大窗口
	if(nIDEvent ==Timer1_ID)
	{
		flashDlg->MoveWindow(
				  (desktopRect.Width() - dlgRect.Width()) / 2,
				  (desktopRect.Height() - dlgRect.Height()) / 2, 
				  xCreateSpeed+dlgRect.Width(),
				  yCreateSpeed+dlgRect.Height() 
				  );
		
		//不要超过窗口预设的宽度
		if(dlgRect.Width() >=OriginalWidth) 
			xCreateSpeed=0; 
		//不要超过窗口预设的高度
		if(dlgRect.Height() >=OriginalHeight)
			yCreateSpeed=0;
		//停止变化，关闭定时器Timer1_ID
		if((dlgRect.Width() >=OriginalWidth) && (dlgRect.Height() >=OriginalHeight))
			flashDlg->KillTimer(Timer1_ID); 
	}
	
	//如果是窗口关闭过程，则逐渐缩小窗口
	if(nIDEvent == Timer2_ID)
	{
		flashDlg->MoveWindow(DestoryPosX,
							DestoryPosY ,
							dlgRect.Width(),
							dlgRect.Height()-yDestorySpeed
							);
		
		//当高度等于零后高度就不在变化
		//if(dlgRect.Height() <=0)
			 
		//停止变化，关闭定时器Timer2_ID，并且关闭窗口
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
	//获取窗口当前的位置并保存
	flashDlg->GetWindowRect(&dlgRect);
	DestoryPosX=dlgRect.left;
	DestoryPosY=dlgRect.top;
	//开始启动定时器Timer2_ID
	flashDlg->SetTimer(Timer2_ID,10,NULL);
}

void CDlgFlash::SetFlashCreateSpeed(int x,int y){
	xCreateSpeed=x;
	xCreateSpeed=y;
}

void CDlgFlash::SetFlashDestroySpeed(int y){
	yDestorySpeed=y;
}