#if !defined(AFX_VIDEOCODEC_H_INCLUDED)
#define AFX_VIDEOCODEC_H_INCLUDED

/*
常用的编码器压缩比,程序使用M263编码
0x64697663,1684633187,Cinepak Codec by Radius			76032 >> 1860
0x32335649,842225225,Intel Indeo(R) Video R3.2			76032 >> 475
0x56555949,1448433993,Intel IYUV codec					76032 >> 38016 放弃这个，压缩比太低
0x4356534d,1129730893,Microsoft Video 1					76032 >> 3202
0x3336324d,859189837,Microsoft H.263 Video Codec		76032 >> 663 对分辩率有要求
0x33564d57,861293911,Microsoft Windows Media Video 9	76032 >> 196
0x3234504d,842289229,Microsoft MPEG-4 Video Codec V2	76032 >> 349

M263只支持176*144 352*288 (352*288 24彩的试验只支持biPlanes = 1)


一段可以用的上的代码

  // 列举本机安装的解码器(CODEC)
int EnumCodecs(int *fccHandler, char *strName)
{
	static int	i = 0;
	int			nRet = 1;
	HIC			hIC;
	ICINFO		icInfo;

	if (fccHandler == NULL)
		return 0;

	if(!ICInfo(ICTYPE_VIDEO, i, &icInfo)) 
	{
		i = 0;
		return 0;
	}
	hIC = ICOpen(icInfo.fccType, icInfo.fccHandler, ICMODE_QUERY);

	if (hIC)
	{
		ICGetInfo(hIC, &icInfo, sizeof(icInfo)); 
		*fccHandler = icInfo.fccHandler;
		//由于得到的szDescription是UNICODE双字节字串，所以要转换为ASCII的
		if (strName != NULL)
			wcstombs(strName, icInfo.szDescription, 256);
	}
	else nRet = -1;

	ICClose(hIC);
	i++;
	return nRet;
}

Usage:
	int	fccHandler;
	char strName[MAX_PATH];
	while(EnumCodecs(&fccHandler, strName))
	{
		printf("0x%x,%s\n", fccHandler, strName);
	}
*/



class CVideoCodec  
{
	COMPVARS	m_cv;
	HIC			m_hIC;
	BITMAPINFO*	m_lpbmiInput;
	BITMAPINFO	m_bmiOutput;
public:
	HMODULE VideoHmodule;
public:
	
	bool InitCompressor(BITMAPINFO*	lpbmi, DWORD fccHandler)
	{
		if (lpbmi == NULL)
			return false;

		m_lpbmiInput = lpbmi;
		
		ZeroMemory(&m_cv, sizeof(m_cv));
		m_cv.cbSize		= sizeof(m_cv);
		m_cv.dwFlags	= ICMF_COMPVARS_VALID;
		m_cv.hic		= m_hIC;
		m_cv.fccType	= ICTYPE_VIDEO;
		m_cv.fccHandler	= fccHandler;
		m_cv.lpbiOut	= NULL;
		m_cv.lKey		= 10;
		m_cv.lDataRate	= 6;
		m_cv.lQ			= ICQUALITY_HIGH;		
		
		typedef HIC (VFWAPI *ICOpenT)(DWORD,DWORD,UINT);
		ICOpenT pICOpen=(ICOpenT)GetProcAddress(VideoHmodule,"ICOpen");
		m_hIC = pICOpen(ICTYPE_VIDEO, m_cv.fccHandler, ICMODE_COMPRESS | ICMODE_DECOMPRESS);
		
		if (m_hIC == NULL)
		{
			return false;
		}
//#define ICCompressGetFormat(hic, lpbiInput, lpbiOutput) 
//ICSendMessage(hic, ICM_COMPRESS_GET_FORMAT, (DWORD_PTR)(LPVOID)(lpbiInput), (DWORD_PTR)(LPVOID)(lpbiOutput))
//ICCompressGetFormat(m_hIC, m_lpbmiInput, &m_bmiOutput);

		// 向编码器发送验证
		typedef LRESULT (VFWAPI *ICSendMessageT)(HIC,UINT,DWORD_PTR,DWORD_PTR);

		ICSendMessageT pICSendMessage=(ICSendMessageT)GetProcAddress(VideoHmodule,"ICSendMessage");
	
		pICSendMessage(m_hIC,ICM_COMPRESS_GET_FORMAT, (DWORD_PTR)(LPVOID)(m_lpbmiInput),(DWORD_PTR)(LPVOID)&m_bmiOutput);

		pICSendMessage(m_hIC, 0x60c9, 0xf7329ace, 0xacdeaea2);
		
		m_cv.hic = m_hIC;
		m_cv.dwFlags = ICMF_COMPVARS_VALID;
		

		typedef BOOL (VFWAPI *ICSeqCompressFrameStartT)(PCOMPVARS,LPBITMAPINFO);
		ICSeqCompressFrameStartT pICSeqCompressFrameStart=(ICSeqCompressFrameStartT)GetProcAddress(VideoHmodule,"ICSeqCompressFrameStart");

		if (!pICSeqCompressFrameStart(&m_cv, m_lpbmiInput))
		{
			return false;
		}
//#define ICDecompressBegin(hic, lpbiInput, lpbiOutput) 
//ICSendMessage(hic, ICM_DECOMPRESS_BEGIN, (DWORD_PTR)(LPVOID)(lpbiInput), (DWORD_PTR)(LPVOID)(lpbiOutput))
		
		pICSendMessage(m_hIC,ICM_DECOMPRESS_BEGIN,(DWORD_PTR)(LPVOID)&m_bmiOutput,(DWORD_PTR)(LPVOID)m_lpbmiInput);

//ICDecompressBegin(m_hIC, &m_bmiOutput , m_lpbmiInput);
		
		return true;
	}
	
	bool DecodeVideoData(BYTE *pin, int len, BYTE* pout, int *lenr,DWORD flag)
	{
		if(!pin || !pout ||!m_hIC)		
			return false;
 		if (ICDecompress(m_hIC, flag, &m_bmiOutput.bmiHeader, pin, &m_lpbmiInput->bmiHeader, pout) != ICERR_OK)
 			return false;
		
		if (lenr) *lenr = m_lpbmiInput->bmiHeader.biSizeImage;
		
		return true;	
	}
	
	bool EncodeVideoData(BYTE* pin, int len, BYTE* pout, int* lenr, bool* pKey)
	{
		BYTE	*p;
		long	s = 1;
		BOOL	k = true;
		if ( !pin || !pout || len != (int)m_lpbmiInput->bmiHeader.biSizeImage || !m_hIC)
			return false;

		typedef LPVOID (VFWAPI *ICSeqCompressFrameT)( IN PCOMPVARS pc, IN UINT uiFlags, IN LPVOID lpBits, OUT BOOL FAR *pfKey, IN OUT LONG FAR *plSize );
		ICSeqCompressFrameT pICSeqCompressFrame=(ICSeqCompressFrameT)GetProcAddress(VideoHmodule,"ICSeqCompressFrame");


		p = (BYTE*)pICSeqCompressFrame(&m_cv, 0, pin, &k, &s);

		if (!p)		return false;
		if (lenr)	*lenr = s;
		if (pKey)	*pKey = k;
		
		CopyMemory(pout, p, s);
		
		return true;
	}
	
	CVideoCodec()
	{
		VideoHmodule = LoadLibrary("MSVFW32.dll");
		m_lpbmiInput = NULL;
	}
	
	virtual ~CVideoCodec()
	{
		// No init yet or init error
		if (m_hIC == NULL)
			return;

		typedef LRESULT (VFWAPI *ICSendMessageT)(HIC,UINT,DWORD_PTR,DWORD_PTR);
		ICSendMessageT pICSendMessage=(ICSendMessageT)GetProcAddress(VideoHmodule,"ICSendMessage");
//#define ICDecompressEnd(hic) ICSendMessage(hic, ICM_DECOMPRESS_END, 0, 0)
//		ICDecompressEnd(m_hIC);

		pICSendMessage(m_hIC,ICM_DECOMPRESS_END, 0, 0);

		typedef void (VFWAPI *ICSeqCompressFrameEndT)(PCOMPVARS);
		ICSeqCompressFrameEndT pICSeqCompressFrameEnd=(ICSeqCompressFrameEndT)GetProcAddress(VideoHmodule,"ICSeqCompressFrameEnd");
		pICSeqCompressFrameEnd(&m_cv);


		typedef void (VFWAPI *ICCompressorFreeT)( PCOMPVARS);
		ICCompressorFreeT pICCompressorFree=(ICCompressorFreeT)GetProcAddress(VideoHmodule,"ICCompressorFree");
		pICCompressorFree(&m_cv);
		
		
		typedef LRESULT (VFWAPI *ICCloseT)(HIC);
		ICCloseT pICClose=(ICCloseT)GetProcAddress(VideoHmodule,"ICClose");
		pICClose(m_hIC);
		
		FreeLibrary(VideoHmodule);
	}
	
};
#endif // !defined(AFX_VIDEOCODEC_H_INCLUDED)