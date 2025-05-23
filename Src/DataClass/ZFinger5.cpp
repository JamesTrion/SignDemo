#include "StdAfx.h"
#include "ZFinger5.h"

#include "Header/ZAZAPIK_Protocol.h"

#ifdef USING_Finger5

#ifdef _WIN64
//#pragma comment(lib, "x64/ZAZAPIK.lib")			//marked by Trion on 2025/04/17
#pragma comment(lib, "ZAZAPIK.lib")				// added by Trion on 2025/04/17
#else 
#pragma comment(lib, "x86/ZAZAPIK.lib")
#endif

#endif // USING_Finger5



ZFinger5::ZFinger5()
{
	
}
ZFinger5::~ZFinger5()
{
	CloseFingerDevice();
	if (m_pImgData)
	{
		delete m_pImgData;
		m_pImgData = NULL;
		m_nImgDataSize = 0;
	}
}


BOOL ZFinger5::OpenFingerDevice()
{
#ifdef USING_Finger5
	if (NULL != m_hDev) return TRUE;// KCloseDeviceEx(m_hDev, m_nDevAddr);
	m_hDev = NULL;

	int nRet = KOpenDeviceEx(&m_hDev, DEVICE_070, 0, 0, 0, 0);
	if (0 != nRet)
	{
		m_hDev = NULL;
		g_Log.WriteLog(std::string("ZAZOpenDeviceEx fail:") + KErr2Str(nRet));
	}

	return (0 == nRet);
#else
	return FALSE;
#endif // USING_Finger5
	return FALSE;
}
BOOL ZFinger5::CloseFingerDevice()
{
#ifdef USING_Finger5
	if (NULL != m_hDev) KCloseDeviceEx(m_hDev, m_nDevAddr);
	m_hDev = NULL;
	return TRUE;
#else
	return FALSE;
#endif // USING_Finger5
	return FALSE;
}
int ZFinger5::GetFingerImg(OUT CImage &bmpObj,BOOL bRedColor)
{
#ifdef USING_Finger5
	if (NULL == m_hDev) return ERR_FAIL;
	bmpObj.Destroy();

	SIZE szImg = { 256, 360 };
	if (m_szImg.cx != szImg.cx || m_szImg.cy != szImg.cy)
	{
		m_szImg = szImg;
		m_nImgDataSize = szImg.cx * szImg.cy;
		if (m_pImgData) delete m_pImgData;
		m_pImgData = new unsigned char[m_nImgDataSize + 1078];
	}

	int nRet = 2;
	int nTimeout = 0;
	while (2 == nRet)
	{
		nRet = KGetImage(m_hDev, m_nDevAddr); //获取图象
		nTimeout++;
		if (nTimeout > 100) break;
	}
	if (0 != nRet)
	{
		char Buf[200] = { 0 };
		sprintf(Buf, "KGetImage fail. Err:%d   %s", nRet, KErr2Str(nRet));
		g_Log.WriteLog(Buf);
		return nRet;
	}


	// Severity	Code	Description	Project	File	Line	Suppression State
	//	Error	C4996	'sprintf': This function or variable may be unsafe.Consider using sprintf_s instead.To disable deprecation, use _CRT_SECURE_NO_WARNINGS.See online help for details.SignDemo	d : \gaomonsvn\signsdkanddemo\sign\demo3\src\dataclass\zfinger5.cpp	90



	int  nImgW = 180;
	int  nImgH = 180;
	int nImageLength = 0;
	nRet = KUpImage(m_hDev, m_nDevAddr, m_pImgData, &nImageLength);  //上传图象
	if (nRet != 0)
	{
		g_Log.WriteLog(std::string("ZAZUPIMAGE fail:") + KErr2Str(nRet));
		return nRet;
	}

	CString strPath = ZGlobal::GetAppPath() + _T("\\Img\\Finger.bmp");
	CString strDir = ZDirObj::GetDirectoryName(strPath);
	ZDirObj::CreateDirectory(strDir);
	std::string strAnsi = ZStrCoding::UnicodeToAnsi(strPath.GetString());

	if (bRedColor)
	{
		bmpObj.Create(nImgW, nImgH, 32);
		for (int i = 0; i < nImgH; i++)
		{
			for (int j = 0; j < nImgW; j++)
			{
				BYTE nV = m_pImgData[i*nImgW + j];
				bmpObj.SetPixel(j, i, RGB(255, nV, nV));
			}
		}
		bmpObj.Save(strPath);
	}
	else
	{
		int nRet = KImgData2BMP(m_pImgData, strAnsi.data());
		if (0 != nRet) g_Log.WriteLog(std::string("ZAZImgData2BMP fail:") + KErr2Str(nRet));
		bmpObj.Load(strPath);
	}

	if (bmpObj.IsNull())
	{
		g_Log.WriteLog(_T("Finger.png is null"));
	}

	return ERR_SUCCESS;
#else
	return ERR_FAIL;
#endif // USING_Finger5
	return ERR_FAIL;
}