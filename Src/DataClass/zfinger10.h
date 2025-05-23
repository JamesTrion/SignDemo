#pragma once
//#include "D:\GaomonSVN\SignSDKAndDemo\Sign\Demo3\Src\StdAfx.h"				//original codes , marked by Trion on 2025/04/17
#include "C:\Git_Project\GaomonDemo\SignSDKAndDemo\Sign\Demo3\Src\StdAfx.h"		//added by Trion on 2025/04/17
class ZFinger10
{
public:
	ZFinger10();
	~ZFinger10();
public:
	BOOL OpenFingerDevice();
	BOOL CloseFingerDevice();
	int GetFingerImg(OUT CImage &bmpObj, BOOL bRedColor);

	BOOL IsOpen() { return (NULL != m_hDev); };
protected:
	HANDLE m_hDev = NULL;
	int m_nDevAddr = 0xffffffff;
public:
	unsigned char *m_pImgData = NULL;
	SIZE m_szImg = {0,0};
	int m_nImgDataSize = 0;
};