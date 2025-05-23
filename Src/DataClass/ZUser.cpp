#include "StdAfx.h"
#include "ZUser.h"
#include "lib/libpng.h"
#pragma comment(lib, "libpng.lib")

ZSignDllObj::ZSignDllObj()
{

}
ZSignDllObj::~ZSignDllObj()
{
	if (m_hSignDll != NULL)
	{
		FreeLibrary(m_hSignDll);
	}
}
BOOL ZSignDllObj::InitSignDllObj()
{
	m_hSignDll = ::LoadLibraryA("Sign.dll");

	if (NULL == m_hSignDll) return FALSE;

	pFuncStartScan = (Func_StartScan)GetProcAddress(m_hSignDll, "StartScan");
	pFuncGetErrorMessage = (Func_GetErrorMessage)GetProcAddress(m_hSignDll, "GetErrorMessage");
	pFuncSendCmdToTablet = (Func_SendCmdToTablet)GetProcAddress(m_hSignDll, "SendCmdToTablet");
	pFuncSetCallback_TabletDataProc = (Func_SetCallback_TabletDataProc)GetProcAddress(m_hSignDll, "SetCallback_TabletDataProc");
	pFuncSetCallback_TabletEventProc = (Func_SetCallback_TabletEventProc)GetProcAddress(m_hSignDll, "SetCallback_TabletEventProc");

	if (NULL == pFuncStartScan) return FALSE;
	if (NULL == pFuncGetErrorMessage) return FALSE;
	if (NULL == pFuncSendCmdToTablet) return FALSE;
	if (NULL == pFuncSetCallback_TabletDataProc) return FALSE;
	if (NULL == pFuncSetCallback_TabletEventProc) return FALSE;

	return TRUE;
}




void SetTransparentColor(Gdiplus::Bitmap *pBmp)
{
	if (NULL == pBmp) return;
	int nW = pBmp->GetWidth();
	int nH = pBmp->GetHeight();
	Gdiplus::Color colorT;
	for (int i = 0; i < nH; i++)
	{
		for (int j = 0; j < nW; j++)
		{
			pBmp->GetPixel(j, i, &colorT);

			BYTE nA = 255;
			BYTE nR = colorT.GetR();
			BYTE nG = colorT.GetG();
			BYTE nB = colorT.GetB();
			if (nR >= 1 && nG >= 1 && nB >= 1)
			{
				nA = 0;
			}
	
			colorT.SetValue(Gdiplus::Color::MakeARGB(nA, nR, nG, nB));
			pBmp->SetPixel(j, i, colorT);
		}
	}
}
void SetTransparentColor(Gdiplus::Bitmap *pBmp, COLORREF dwTColor)
{
	if (NULL == pBmp) return;
	int nW = pBmp->GetWidth();
	int nH = pBmp->GetHeight();
	Gdiplus::Color colorT;
	for (int i = 0; i < nH; i++)
	{
		for (int j = 0; j < nW; j++)
		{
			pBmp->GetPixel(j, i, &colorT);

			BYTE nA = 255;
			BYTE nR = colorT.GetR();
			BYTE nG = colorT.GetG();
			BYTE nB = colorT.GetB();
			if (RGB(nR, nG, nB) == dwTColor) nA = 0;
		
			colorT.SetValue(Gdiplus::Color::MakeARGB(nA, nR, nG, nB));
			pBmp->SetPixel(j, i, colorT);
		}
	}
}
bool ImageCopy(const CImage &srcImage, CImage &destImage)
{
	if (srcImage.IsNull()) return FALSE;

	int MaxColors = srcImage.GetMaxColorTableEntries();
	RGBQUAD* ColorTab;
	ColorTab = new RGBQUAD[MaxColors];

	CDC *pDCsrc, *pDCdrc;
	if (!destImage.IsNull())
	{
		destImage.Destroy();
	}
	destImage.Create(srcImage.GetWidth(), srcImage.GetHeight(), srcImage.GetBPP(), 0);

	if (srcImage.IsIndexed())
	{
		srcImage.GetColorTable(0, MaxColors, ColorTab);
		destImage.SetColorTable(0, MaxColors, ColorTab);
	}

	pDCsrc = CDC::FromHandle(srcImage.GetDC());
	pDCdrc = CDC::FromHandle(destImage.GetDC());
	pDCdrc->BitBlt(0, 0, srcImage.GetWidth(), srcImage.GetHeight(), pDCsrc, 0, 0, SRCCOPY);
	srcImage.ReleaseDC();
	destImage.ReleaseDC();
	delete ColorTab;

	return TRUE;
}
ZFingerObj::ZFingerObj()
{
}
ZFingerObj::~ZFingerObj()
{
	CloseFingerDevice();
}

emFingerDevType ZFingerObj::GetDevType()
{
	if (m_Finger5.IsOpen()) return DevType_Finger5;
	if (m_Finger10.IsOpen()) return DevType_Finger10;
	return DevType_NULL;
}

BOOL ZFingerObj::OpenFingerDevice()
{
	BOOL bRet = FALSE;
	if (FALSE == bRet) bRet = m_Finger10.OpenFingerDevice();
	if (FALSE == bRet) bRet = m_Finger5.OpenFingerDevice();
	return bRet;
}
BOOL ZFingerObj::CloseFingerDevice()
{
	m_Finger10.CloseFingerDevice();
	m_Finger5.CloseFingerDevice();
	return TRUE;
}
ZFingerObj::emErrCode ZFingerObj::GetFingerImg(OUT CImage &bmpObj,BOOL bRedColor)
{
	if (m_Finger5.IsOpen())
	{
		int nRet = m_Finger5.GetFingerImg(bmpObj, bRedColor);
		if (-2 == nRet)
		{
			m_Finger5.CloseFingerDevice();
			return Err_Disconnected;
		}
		if (0 == nRet) return Err_OK;
		return Err_Fail;
	}
	else if (m_Finger10.IsOpen())
	{
		int nRet = m_Finger10.GetFingerImg(bmpObj, bRedColor);
		if (-2 == nRet)
		{
			m_Finger10.CloseFingerDevice();
			return Err_Disconnected;
		}
		if (0 == nRet) return Err_OK;
		return Err_Fail;
	}
	return emErrCode::Err_Fail;
}







ZSignData::ZSignData()
{

}
ZSignData::~ZSignData()
{

}
void ZSignData::AddPoint(POINT pt, int nPresure)
{
	m_vectPts.push_back({ pt,nPresure });
}
void ZSignData::RemoveAllPoints()
{
	m_vectPts.clear();
}
RECT ZSignData::GetExternalRect()
{
	RECT rcRet = { 0,0,0,0 };
	for (int i = 0, nSize = m_vectPts.size(); i < nSize; i++)
	{
		rcRet.left = min(rcRet.left, m_vectPts[i].m_pt.x);
		rcRet.top = min(rcRet.top, m_vectPts[i].m_pt.y);

		rcRet.right = max(rcRet.right, m_vectPts[i].m_pt.x);
		rcRet.bottom = max(rcRet.bottom, m_vectPts[i].m_pt.y);
	}
	return rcRet;
}
void ZSignData::DrawToDC(HDC hDC, const RECT& rcPaint)
{
	if (0 == m_nMaxX || 0 == m_nMaxY)
		return;

	Gdiplus::Pen *pPen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	pPen->SetStartCap(Gdiplus::LineCapRound);
	pPen->SetEndCap(Gdiplus::LineCapRound);

	Gdiplus::Graphics Graph(hDC);
	Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	INT64 nDCW = abs(rcPaint.right - rcPaint.left);
	INT64 nDCH = abs(rcPaint.bottom - rcPaint.top);

	Gdiplus::Point ptS = {0,0};
	int nSize = m_vectPts.size();
	for (int i = 0; i < nSize; i++)
	{
		Gdiplus::Point ptInDC;
		ptInDC.X = nDCW * m_vectPts[i].m_pt.x / m_nMaxX + rcPaint.left;
		ptInDC.Y = nDCH * m_vectPts[i].m_pt.y / m_nMaxY + rcPaint.top;

		pPen->SetWidth(m_vectPts[i].m_nPresure / 1000 + 1);
		
		if (0 == i) ptS = ptInDC;
		else if (m_vectPts[i].m_nPresure > 0) Graph.DrawLine((const Pen *)pPen, ptS, ptInDC);
		ptS = ptInDC;
	}

	delete pPen;
}

//-------------------------------------------------------------------------------------------//
Json::Value ZCfg::m_vCfg;
void ZCfg::InitCfg()
{
	CString strFilePath = ZGlobal::GetAppPath() + _T("//cfg.js");
	ZGlobal::JsonFromFile(strFilePath, m_vCfg, STR_TYPE_UTF8);
}
void ZCfg::SaveCfg()
{
	CString strFilePath = ZGlobal::GetAppPath() + _T("//cfg.js");
	std::string strF8 = m_vCfg.toStyledString();
	ZMemery memObj(strF8.length());
	memcpy(memObj.GetData(), strF8.data(), strF8.length());
	memObj.Save(strFilePath);
}
void ZCfg::SaveCfg_CurLang(CString strLangName)
{
	m_vCfg["CurLang"] = ZStrCoding::UnicodeToUTF8(strLangName.GetString());
	SaveCfg();
}
void ZCfg::SaveCfg_SavePath(CString strSavePath)
{
	m_vCfg["SavePath"] = ZStrCoding::UnicodeToUTF8(strSavePath.GetString());
	SaveCfg();
}
void ZCfg::SaveCfg_Red(BOOL bRed)
{
	m_vCfg["Red"] = bRed;
	SaveCfg();
}
void ZCfg::SaveCfg_CleanImgAfterSave(BOOL bClean)
{
	m_vCfg["CleanImgAfterSave"] = bClean;
	SaveCfg();
}
CString ZCfg::GetCfg_CurLang()
{
	CString strLangName = ZStrCoding::Utf8ToUnicode(m_vCfg["CurLang"].asString()).c_str();
	return strLangName;
}
CString ZCfg::GetCfg_SavePath()
{
	CString strSavePath = ZStrCoding::Utf8ToUnicode(m_vCfg["SavePath"].asString()).c_str();
	return strSavePath;
}
BOOL ZCfg::GetCfg_Red()
{
	return m_vCfg["Red"].asBool();
}
BOOL ZCfg::GetCfg_CleanImgAfterSave()
{
	return m_vCfg["CleanImgAfterSave"].asBool();
}
//--------------------------------------------------------------------------------------------------//




ZDictionary<sTabletData *> g_DataDic;
sTabletInfo g_TabletInfo;
ZSignData g_SignData;
ZMemDC *g_pSignDC = NULL;
HWND g_hMainWnd = NULL;
HWND g_hSignDlg = NULL;