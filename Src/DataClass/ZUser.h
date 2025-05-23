#pragma once
//#include "D:\GaomonSVN\SignSDKAndDemo\Sign\Demo3\Src\Header\Huion_Sign.h"					//origial codes , marked by Trion on 2025/04/17
#include "C:\Git_Project\GaomonDemo\SignSDKAndDemo\Sign\Demo3\Src\Header\Huion_Sign.h"		//added by Trion on 2025/04/17
#include "ZFinger10.h"
#include "ZFinger5.h"

#define WM_TabletData      (WM_USER + 1024)
#define WM_TabletEvent     (WM_USER + 1025)
#define WM_UpdateFingerImg (WM_USER + 1026)
#define WM_Disconnected    (WM_USER + 1027)

#define WM_UpdataSign           (WM_USER + 888)
#define WM_TabletDataForSignDlg (WM_USER + 889)

#define USING_Finger5
#define USING_Finger10

void SetTransparentColor(Gdiplus::Bitmap *pBmp);
void SetTransparentColor(Gdiplus::Bitmap *pBmp, COLORREF dwTColor);
bool ImageCopy(const CImage &srcImage, CImage &destImage);
//Severity	Code	Description	Project	File	Line	Suppression State
//Error	C4430	missing type specifier - int assumed.Note: C++ does not support default - int	UITest	d : \gaomonsvn\signsdkanddemo\sign\demo3\src\dataclass\zuser.h	19


struct sTabletData
{
public:
	sTabletData(IN emTabletDataType &DataType, IN uTabletData &tbData)
	{
		m_DataType = DataType;
		m_tbData = tbData;
	};
public:
	emTabletDataType m_DataType;
	uTabletData m_tbData;
};
extern ZDictionary<sTabletData *> g_DataDic;

class ZSignDllObj
{
public:
	ZSignDllObj();
	~ZSignDllObj();
	BOOL InitSignDllObj();
public:
	Func_StartScan pFuncStartScan = NULL;
	Func_GetErrorMessage pFuncGetErrorMessage = NULL;
	Func_SendCmdToTablet pFuncSendCmdToTablet = NULL;
	Func_SetCallback_TabletDataProc pFuncSetCallback_TabletDataProc = NULL;
	Func_SetCallback_TabletEventProc pFuncSetCallback_TabletEventProc = NULL;
protected:
	HMODULE	 m_hSignDll = NULL;
};



enum emFingerDevType
{
	DevType_NULL,DevType_Finger5, DevType_Finger10
};
class ZFingerObj
{
public:
	ZFingerObj();
	~ZFingerObj();

	enum emErrCode
	{
		Err_OK, Err_Fail,Err_Disconnected
	};
public:
	emFingerDevType GetDevType();
	BOOL OpenFingerDevice();
	BOOL CloseFingerDevice();
	emErrCode GetFingerImg(OUT CImage &bmpObj, BOOL bRedColor);
protected:
	ZFinger10 m_Finger10;
	ZFinger5 m_Finger5;
};




struct sTabletPt
{
public:
	sTabletPt(POINT pt, int nPresure)
	{
		m_pt = pt;
		m_nPresure = nPresure;
	}
public:
	POINT m_pt;
	int m_nPresure;
};
class ZSignData
{
public:
	ZSignData();
	~ZSignData();
	void DrawToDC(HDC hDC, const RECT& rcPaint);
public:
	void AddPoint(POINT pt, int nPresure);
	void RemoveAllPoints();
	RECT GetExternalRect();//获取外接矩形
public:
	long m_nMaxX = 0;//签名设备X轴上最大值
	long m_nMaxY = 0;//签名设备Y轴上最大值
	long m_nMaxP = 0;//签名设备最大压感值
	std::vector<sTabletPt> m_vectPts;
};

class ZCfg
{
public:
	static void InitCfg();
	static void SaveCfg();
	static void SaveCfg_CurLang(CString strLangName);
	static void SaveCfg_SavePath(CString strSavePath);
	static void SaveCfg_Red(BOOL bRed);
	static void SaveCfg_CleanImgAfterSave(BOOL bClean);
	static CString GetCfg_CurLang();
	static CString GetCfg_SavePath();
	static BOOL GetCfg_Red();
	static BOOL GetCfg_CleanImgAfterSave();
protected:
	static Json::Value m_vCfg;
};

extern sTabletInfo g_TabletInfo;
extern ZSignData g_SignData;
extern ZMemDC *g_pSignDC;
extern HWND g_hMainWnd;
extern HWND g_hSignDlg;