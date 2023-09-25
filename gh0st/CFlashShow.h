// DlgFlash.h: interface for the CDlgFlash class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGFLASH_H__FC9C5DF7_7144_4F99_9B5A_0C5451DEA813__INCLUDED_)
#define AFX_DLGFLASH_H__FC9C5DF7_7144_4F99_9B5A_0C5451DEA813__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDlgFlash 
{
public:
	CDlgFlash(CDialog* Dlg);
	CDlgFlash(CDialog* Dlg,int Timer1_ID,int Timer2_ID);
	virtual ~CDlgFlash();
	void StartFlashDestroyDlg();			//开始启动销毁效果
	void OnTimer(UINT nIDEvent);			//处理对应定时器的任务，在后面设置时钟时为SetTimer(TimerID,10,NULL)
											//没有设置为自定义执行函数的原因是一定要在需要动画效果的窗口中重写从
											//CDialog中继承的OnClose()函数用于映射销毁窗口效果用之
											//

	void SetFlashCreateSpeed(int x,int y);	//设置创建窗口变化速度
											//参数x表示窗口宽度的变化速度
											//参数y表示窗口高度的变化速度
											
	void SetFlashDestroySpeed(int y);		//设置销毁窗口变化速度
											//参数y表示窗口高度的变化速度
protected:
	int xCreateSpeed;						//创建窗口时窗口宽度的变化速度
	int yCreateSpeed;						//创建窗口时窗口高度的变化速度
	int xDestorySpeed;						//销毁窗口时窗口宽度的变化速度			
	int yDestorySpeed;						//创建窗口时窗口宽度的变化速度
	int OriginalWidth,OriginalHeight;		//窗口的原始大小
	int DestoryPosX;						//销毁窗口时窗口的左上角x轴位置
	int DestoryPosY;						//销毁窗口时窗口的左上角y轴位置
	int Timer1_ID;							//定时器1ID，创建窗口时用之
	int Timer2_ID;							//定时器2ID，销毁窗口时用之	
	CDialog* flashDlg;						//指向需要动画效果的窗口指针
	void InitCDlg();						//初始化，内部使用
};

#endif // !defined(AFX_DLGFLASH_H__FC9C5DF7_7144_4F99_9B5A_0C5451DEA813__INCLUDED_)
