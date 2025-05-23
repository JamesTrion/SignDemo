#include "StdAfx.h"
#include "ZFinger10.h"

#ifdef USING_Finger10

#include "Header/HuionHSVCTenInchDLL.h"
#pragma comment(lib, "HuionHSVCTenInchDLL.lib")
#endif // USING_Finger10


ZFinger10::ZFinger10()
{

}
ZFinger10::~ZFinger10()
{
	CloseFingerDevice();
	if (m_pImgData)
	{
		delete m_pImgData;
		m_pImgData = NULL;
		m_nImgDataSize = 0;
	}
}

#define ProductType_5  (0) //5寸屏
#define ProductType_10 (1) //10寸屏

//设备类型（0：有驱动USB 设备，1：串口设备，2：无驱 UDISK 设备）
#define DeviceType_USB   (0)
#define DeviceType_COM   (1)
#define DeviceType_UDISK (2)

#define ProductType ProductType_10
BOOL ZFinger10::OpenFingerDevice()
{
#ifdef USING_Finger10
	if (NULL != m_hDev) return TRUE;// HWCloseDeviceEx(ProductType, m_hDev);
	m_hDev = NULL;

	int nRet = HWOpenDeviceEx(ProductType, &m_hDev, DeviceType_UDISK, 0, 0, 2, 0);
	if (0 != nRet)
	{
		m_hDev = NULL;
		g_Log.WriteLog(_T("HWOpenDeviceEx fail:") + HWErr2Str(nRet));
	}

	return (0 == nRet);
#else
	return FALSE;
#endif // USING_Finger10
	return FALSE;
}
BOOL ZFinger10::CloseFingerDevice()
{
#ifdef USING_Finger10
	if (NULL != m_hDev) HWCloseDeviceEx(ProductType, m_hDev);
	m_hDev = NULL;
	return TRUE;
#else
	return FALSE;
#endif // USING_Finger10
	return FALSE;
	
}
int ZFinger10::GetFingerImg(OUT CImage &bmpObj,BOOL bRedColor)
{
#ifdef USING_Finger10
	if (NULL == m_hDev) return 1;
	bmpObj.Destroy();

	SIZE szImg = { 256,360 };
	if (m_szImg.cx != szImg.cx || m_szImg.cy != szImg.cy)
	{
		m_szImg = szImg;

		int nRet = HWSetIMGLen(szImg.cx, szImg.cy);
		if (0 != nRet)
		{
			g_Log.WriteLog(_T("HWSetIMGLen fail:") + HWErr2Str(nRet));
		}

		m_nImgDataSize = szImg.cx * szImg.cy;
		if (m_pImgData) delete m_pImgData;
		m_pImgData = new unsigned char[m_nImgDataSize + 1078];
	}

	int nRet = 2;
	int nTimeout = 0;
	while (2 == nRet)
	{
		nRet = HWGetImage(m_hDev, m_nDevAddr); //获取图象
		nTimeout++;
		if (nTimeout > 100) break;
	}
	if (0 != nRet)
	{
		g_Log.WriteLog(_T("HWGetImage fail:") + HWErr2Str(nRet));
		return nRet;
	}

	nRet = HWUpImage(m_hDev, m_nDevAddr, m_pImgData, &m_nImgDataSize);  //上传图象
	if (nRet != 0)
	{
		g_Log.WriteLog(_T("HWUpImage fail:") + HWErr2Str(nRet));
		return nRet;
	}

	CString strPath = ZGlobal::GetAppPath() + _T("\\Img\\Finger.bmp");
	CString strDir = ZDirObj::GetDirectoryName(strPath);
	ZDirObj::CreateDirectory(strDir);
	if (bRedColor)
	{
		int nImgW = m_szImg.cx;
		int nImgH = m_szImg.cy;
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
		std::string strAnsi = ZStrCoding::UnicodeToAnsi(strPath.GetString());
		int nRet = HWImgData2BMP(m_pImgData, strAnsi.data());
		if (0 != nRet) g_Log.WriteLog(_T("ZAZImgData2BMP fail:") + HWErr2Str(nRet));
		bmpObj.Load(strPath);
	}

	if (bmpObj.IsNull()) g_Log.WriteLog(_T("Finger.png is null"));

	return 0;
#else
	return 1;
#endif // USING_Finger10
	return 1;
}
