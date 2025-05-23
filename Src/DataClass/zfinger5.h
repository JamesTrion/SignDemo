#pragma once

class ZFinger5
{
public:
	ZFinger5();
	~ZFinger5();

	enum emErrCode {OK,NotCon};
public:
	BOOL OpenFingerDevice();
	BOOL CloseFingerDevice();
//	int GetFingerImg( CImage &bmpObj, BOOL bRedColor);
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
