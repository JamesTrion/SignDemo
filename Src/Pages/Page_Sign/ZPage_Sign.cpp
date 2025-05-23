#include "StdAfx.h"
#include "ZPage_Sign.h"
#include "Dlg/ZDlg_Sign.h"

#define BtnName_StartSign   _T("CButtonUI_StartSign")
#define BtnName_ResetSign   _T("CButtonUI_ResetSign")
#define BtnName_OpenFinger  _T("CButtonUI_OpenFinger")
#define BtnName_CloseFinger _T("CButtonUI_CloseFinger")

#define BtnName_MergeImg    _T("CButtonUI_MergeImg")
#define BtnName_CleanImg    _T("CButtonUI_CleanImg")

#define BtnName_Save        _T("CButtonUI_Save")
#define BtnName_SelectPath  _T("CButtonUI_SelectPath")

#define OptName_CleanImgAfterSave _T("COptionUI_CleanImgAfterSave")
#define OptName_FingerColor_Black _T("COptionUI_FingerColor_Black")
#define OptName_FingerColor_Red   _T("COptionUI_FingerColor_Red")

#define LabName_SignDeviceStatu   _T("CLabelUI_SignDeviceStatu")
#define LabName_FingerDeviceStatu _T("CLabelUI_FingerDeviceStatu")

#define EditName_SavePath _T("CEditUI_SavePath")

#define Req_GetFinger (1)
#define W_FingerImg (256)
#define H_FingerImg (360)

ZPage_Sign *g_pPage_Sign = NULL;
ZPage_Sign::ZPage_Sign(void)
{
	g_pPage_Sign = this;
	m_WorkThread.InitWorkThread(this);
	Build();
}

ZPage_Sign::~ZPage_Sign(void)
{
	if (g_hSignDlg) SendMessage(g_hSignDlg, WM_CLOSE, 0, 0);

	m_bGetFinger = FALSE;
	m_WorkThread.EndWork();
	m_FingerObj.CloseFingerDevice();

	if (m_pPen) ::delete m_pPen;
	g_pPage_Sign = NULL;

	if (m_pCanvasMergeDC) delete m_pCanvasMergeDC;

	if (g_pSignDC) delete g_pSignDC;
	
	m_pManager->RemoveNotifier(this);
	m_pManager->RemoveMessageFilter(this);
}

void __stdcall TabletDataProc(IN const sTabletInfo *pTablet, emTabletDataType DataType, uTabletData tbData)
{//由于涉及到UI操作 此处将数据交由UI线程处理
	sTabletData *pData = new sTabletData(DataType, tbData);
    PostMessage(g_pPage_Sign->GetWindow(),WM_TabletData, (WPARAM)g_DataDic.AddItem(pData), 0);
}


void __stdcall TabletEventProc(emTabletEventType EventType, IN const sTabletInfo *pTablet)
{//由于涉及到UI操作 此处将数据交由UI线程处理
	SendMessage(g_pPage_Sign->GetWindow(), WM_TabletEvent, (WPARAM)EventType, (LPARAM)pTablet);
}
void ZPage_Sign::DoInit()
{
	__super::DoInit();
	m_pManager->AddNotifier(this);
	m_pManager->AddMessageFilter(this);

	m_pPen = ::new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	m_pPen->SetStartCap(Gdiplus::LineCapRound);
	m_pPen->SetEndCap(Gdiplus::LineCapRound);

	if (m_SignDllObj.InitSignDllObj())//初始化签名DLL
	{
		m_SignDllObj.pFuncSetCallback_TabletDataProc(TabletDataProc);  //设置处理手写数据的回调函数
		m_SignDllObj.pFuncSetCallback_TabletEventProc(TabletEventProc);//设置处理数位板连接状态的回调函数
		m_SignDllObj.pFuncStartScan();
	}
 
	if (ZCfg::GetCfg_Red()) ((COptionUI *)FindSubControl(OptName_FingerColor_Red))->Selected(true);
	else ((COptionUI *)FindSubControl(OptName_FingerColor_Black))->Selected(true);

	((COptionUI *)FindSubControl(OptName_CleanImgAfterSave))->Selected(ZCfg::GetCfg_CleanImgAfterSave());

	CString strDir = ZCfg::GetCfg_SavePath();
	if(strDir.IsEmpty()) strDir = ZGlobal::GetAppPath() + _T("\\ExportedImage");
	ZDirObj::CreateDirectory(strDir);
	FindSubControl(EditName_SavePath)->SetText(strDir);

	FindSubControl(BtnName_StartSign)->SetEnabled(false);
	FindSubControl(BtnName_ResetSign)->SetEnabled(false);

	m_WorkThread.StartWork();
}


void ZPage_Sign::Build()
{
	ZVerticalBox *pVBoxRoot = new ZVerticalBox;
	
	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pVBoxRoot->Add(pHBox);

		pHBox->Add(BuildBox_Sign());
		pHBox->AddStretch();
		pHBox->Add(BuildBox_Finger());
		pHBox->AddStretch();
		pHBox->Add(BuildBox_Merge());
	}
	pVBoxRoot->AddSpace(20);
	{
		pVBoxRoot->Add(BuildBox_Save());
	}

	Add(pVBoxRoot);

	SetInset({ 20,20,20,20 });
}

CButtonUI *ZPage_Sign::CreateBtn(CString strName, CString strText)
{
	CButtonUI *pBtn = UI_CreateBtn(strName, strText, 0xffffffff, FontSize_16, 185, 40);
	pBtn->SetBkColor(0xff00bed6);
	pBtn->SetHotBkColor(0xff00b1c8);
	pBtn->SetBorderRound({3,3});
	pBtn->SetDisabledImage(CPaintManagerUI::GetResourcePath() + _T("MainFrame\\disable.png"));
	return pBtn;
}

void PaintFunc_Sign(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint, void *pUserParam)
{
	((ZPage_Sign *)pUserParam)->Paint_Sign(pCanvas, hDC, rcPaint);
}
void ZPage_Sign::Paint_Sign(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint)
{
	if (pCanvas == m_pCanvasSign)
	{
		RECT rcUI = pCanvas->GetPos();
		int nW = rcUI.right - rcUI.left;
		int nH = rcUI.bottom - rcUI.top;

		InflateRect(&rcUI, -1, -1);

		if (NULL == g_pSignDC)
		{
			g_pSignDC = new ZMemDC(m_pManager->GetPaintDC(), nW, nH);
			RECT rcDC = { 0,0,nW, nH };
			::FillRect(g_pSignDC->DC(), &rcDC, GetStockBrush(WHITE_BRUSH));
		}
		else
		{
			if (g_SignData.m_vectPts.size() == 0)
			{
				SIZE szDC = g_pSignDC->GetSize();
				RECT rcDC = { 0,0,szDC.cx, szDC.cy };
				::FillRect(g_pSignDC->DC(), &rcDC, GetStockBrush(WHITE_BRUSH));
			}
			::BitBlt(hDC, rcUI.left, rcUI.top, nW, nH, g_pSignDC->DC(), 0, 0, SRCCOPY);
		}

		ZGDI::DrawRect(hDC, rcUI, RGB(220, 220, 220), 1, PS_SOLID);
	}
	else if (pCanvas == m_pCanvasFinger)
	{
		RECT rcImg = m_pCanvasFinger->GetPos();
		::FillRect(hDC, &rcImg, GetStockBrush(WHITE_BRUSH));
		::InflateRect(&rcImg, -1, -1);

		if (m_FingerImg.IsNull() == false)
		{
		    m_FingerImg.Draw(hDC, rcImg);
		}

		ZGDI::DrawRect(hDC, rcImg, RGB(220, 220, 220), 1, PS_SOLID);
	}
	else if (pCanvas == m_pCanvasMerge)
	{
		RECT rcUI = m_pCanvasMerge->GetPos();

		if (m_pCanvasMergeDC)
		{
			int nW = rcUI.right - rcUI.left;
			int nH = rcUI.bottom - rcUI.top;
			::BitBlt(hDC, rcUI.left, rcUI.top, nW, nH, m_pCanvasMergeDC->DC(), 0, 0, SRCCOPY);
		}
		
		::InflateRect(&rcUI, -1, -1);
		ZGDI::DrawRect(hDC, rcUI, RGB(220, 220, 220), 1, PS_SOLID);
	}
}
CControlUI *ZPage_Sign::BuildBox_Sign()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");
	ZVerticalBox *pVBoxRoot = new ZVerticalBox;

	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pHBox->SetFixedHeight(67);
		pVBoxRoot->Add(pHBox);

		CLabelUI *pImg = UI_CreateLab(NULL, _T("1"), 0xffffffff, FontSize_40, 57, 67);
		pImg->SetTextPadding({0,0,0,10});
		pImg->SetBkImage(strResPath + _T("num_bg.png"));
		pHBox->Add(pImg);

		ZVerticalBox *pVBox = new ZVerticalBox;
		pHBox->Add(pVBox);
		{
			pVBox->AddStretch();

			CString strTemp;
			CLabelUI *pLab = UI_CreateLab(NULL, _T("签名"),0xff000000,FontSize_16,0,28);
			pLab->SetTextStyle(DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			pLab->SetTextPadding({57,0,0,0});
			strTemp.Format(_T("file='%s' source='0,0,27,28' dest='20,0,47,28'"), strResPath + _T("sign_ic.png"));
			pLab->SetBkImage(strTemp);
			pVBox->Add(pLab);

			pVBox->AddStretch();
			pVBox->AddHLine(0xff00bed6);
			pVBox->AddSpace(10);
		}
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBox = new ZHorizontalBox;
		pVBoxRoot->Add(HBox);
		HBox->SetFixedHeight(240);
		HBox->SetInset({ 10,0,10,0 });

		ZCanvasUI *pUI = new ZCanvasUI;
		pUI->SetBorderColor(0xffe0e0e0);
		pUI->SetBorderSize(1);
		m_pCanvasSign = pUI;
		m_pCanvasSign->SetPaintFunc(PaintFunc_Sign,this);
		HBox->Add(pUI);
	}
	{
		CLabelUI *pLab = UI_CreateLab(LabName_SignDeviceStatu, _T("签名设备未连接"),0xffff0000,FontSize_14,0,30);
		pLab->SetTextStyle(DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
		pLab->SetTextPadding({0,0,10,0});
		pVBoxRoot->Add(pLab);
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBoxBtns = new ZHorizontalBox;
		pVBoxRoot->Add(HBoxBtns);
		HBoxBtns->SetInset({20,0,20,0});
		HBoxBtns->SetFixedHeight(40);
		HBoxBtns->Add(CreateBtn(BtnName_StartSign, _T("开始签名")));
		HBoxBtns->AddStretch();
		HBoxBtns->Add(CreateBtn(BtnName_ResetSign, _T("重新签名")));
	}
	pVBoxRoot->AddSpace(20);
	pVBoxRoot->SetBkColor(0xffffffff);
	pVBoxRoot->SetFixedWidth(435);
	return pVBoxRoot;
}


#define W_FingerUI (186)
#define H_FingerUI (240)
CControlUI *ZPage_Sign::BuildBox_Finger()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");
	ZVerticalBox *pVBoxRoot = new ZVerticalBox;

	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pHBox->SetFixedHeight(67);
		pVBoxRoot->Add(pHBox);

		CLabelUI *pImg = UI_CreateLab(NULL, _T("2"), 0xffffffff, FontSize_40, 57, 67);
		pImg->SetTextPadding({ 0,0,0,10 });
		pImg->SetBkImage(strResPath + _T("num_bg.png"));
		pHBox->Add(pImg);

		ZVerticalBox *pVBox = new ZVerticalBox;
		pHBox->Add(pVBox);
		{
			pVBox->AddStretch();

			CString strTemp;
			CLabelUI *pLab = UI_CreateLab(NULL, _T("指纹"), 0xff000000, FontSize_16, 0, 28);
			pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT);
			pLab->SetTextPadding({ 58,0,0,0 });
			strTemp.Format(_T("file='%s' source='0,0,28,28' dest='20,0,48,28'"), strResPath + _T("fingerprint_ic.png"));
			pLab->SetBkImage(strTemp);
			pVBox->Add(pLab);

			pVBox->AddStretch();
			pVBox->AddHLine(0xff00bed6);
			pVBox->AddSpace(10);
		}
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBox = new ZHorizontalBox;
		pVBoxRoot->Add(HBox);
		HBox->SetFixedHeight(H_FingerUI);
		
		HBox->AddStretch();

		ZCanvasUI *pUI = new ZCanvasUI;
		pUI->SetBorderColor(0xffe0e0e0);
		pUI->SetBorderSize(1);
		pUI->SetFixedWidth(W_FingerUI);
		pUI->SetFixedHeight(H_FingerUI);
		m_pCanvasFinger = pUI;
		m_pCanvasFinger->SetPaintFunc(PaintFunc_Sign, this);
		HBox->Add(pUI);

		HBox->AddStretch();
	}
	{
		CLabelUI *pLab = UI_CreateLab(LabName_FingerDeviceStatu, _T("指纹传感器未连接"), 0xffff0000, FontSize_14, 0, 30);
		pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
		pLab->SetTextPadding({ 0,0,10,0 });
		pVBoxRoot->Add(pLab);
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBoxOpt = new ZHorizontalBox;
		pVBoxRoot->Add(HBoxOpt);
		HBoxOpt->SetInset({ 20,0,20,0 });
		HBoxOpt->SetFixedHeight(30);

		HBoxOpt->Add(UI_CreateLab(NULL, _T("指纹颜色"),0xff000000,FontSize_14,100,0));
		HBoxOpt->Add(CreateOpt(OptName_FingerColor_Black, _T("黑色"),_T("Group_Color")));
		HBoxOpt->AddSpace(20);
		HBoxOpt->Add(CreateOpt(OptName_FingerColor_Red, _T("红色"), _T("Group_Color")));
	}
	{
		ZHorizontalBox *HBoxBtns = new ZHorizontalBox;
		pVBoxRoot->Add(HBoxBtns);
		HBoxBtns->SetInset({ 20,0,20,0 });
		HBoxBtns->SetFixedHeight(40);
		HBoxBtns->Add(CreateBtn(BtnName_OpenFinger, _T("打开指纹传感器")));
		HBoxBtns->AddStretch();
		HBoxBtns->Add(CreateBtn(BtnName_CloseFinger, _T("关闭指纹传感器")));
	}
	pVBoxRoot->AddSpace(20);
	pVBoxRoot->SetBkColor(0xffffffff);
	pVBoxRoot->SetFixedWidth(435);
	return pVBoxRoot;
}
CControlUI *ZPage_Sign::BuildBox_Merge()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");
	ZVerticalBox *pVBoxRoot = new ZVerticalBox;

	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pHBox->SetFixedHeight(67);
		pVBoxRoot->Add(pHBox);

		CLabelUI *pImg = UI_CreateLab(NULL, _T("3"), 0xffffffff, FontSize_40, 57, 67);
		pImg->SetTextPadding({ 0,0,0,10 });
		pImg->SetBkImage(strResPath + _T("num_bg.png"));
		pHBox->Add(pImg);

		ZVerticalBox *pVBox = new ZVerticalBox;
		pHBox->Add(pVBox);
		{
			pVBox->AddStretch();

			CString strTemp;
			CLabelUI *pLab = UI_CreateLab(NULL, _T("合并图像"), 0xff000000, FontSize_16, 0, 28);
			pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT);
			pLab->SetTextPadding({ 58,0,0,0 });
			strTemp.Format(_T("file='%s' source='0,0,28,28' dest='20,0,48,28'"), strResPath + _T("merge_ic.png"));
			pLab->SetBkImage(strTemp);
			pVBox->Add(pLab);

			pVBox->AddStretch();
			pVBox->AddHLine(0xff00bed6);
			pVBox->AddSpace(10);
		}
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBox = new ZHorizontalBox;
		pVBoxRoot->Add(HBox);
		HBox->SetFixedHeight(240);
		HBox->SetInset({ 10,0,10,0 });

		ZCanvasUI *pUI = new ZCanvasUI;
		pUI->SetBorderColor(0xffe0e0e0);
		pUI->SetBorderSize(1);
		m_pCanvasMerge = pUI;
		m_pCanvasMerge->SetPaintFunc(PaintFunc_Sign, this);
		HBox->Add(pUI);
	}
	{
		CLabelUI *pLab = UI_CreateLab(NULL, _T(" "), 0xffff0000, FontSize_14, 0, 30);
		pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
		pLab->SetTextPadding({ 0,0,10,0 });
		pVBoxRoot->Add(pLab);
	}
	pVBoxRoot->AddStretch();
	{
		ZHorizontalBox *HBoxBtns = new ZHorizontalBox;
		pVBoxRoot->Add(HBoxBtns);
		HBoxBtns->SetInset({ 20,0,20,0 });
		HBoxBtns->SetFixedHeight(40);
		HBoxBtns->Add(CreateBtn(BtnName_MergeImg, _T("合并图像")));
		HBoxBtns->AddStretch();
		HBoxBtns->Add(CreateBtn(BtnName_CleanImg, _T("清除")));
	}
	pVBoxRoot->AddSpace(20);
	pVBoxRoot->SetBkColor(0xffffffff);
	pVBoxRoot->SetFixedWidth(435);
	return pVBoxRoot;
}


COptionUI *ZPage_Sign::CreateOpt(CString strName, CString strText, CString strGroupName)
{
	CString strT;
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");

	COptionUI *pOpt = UI_CreateOpt(strName, strText, 0xff000000, FontSize_14, 0, 30, strGroupName);
	pOpt->SetTextPadding({ 25,0,0,0 });
	pOpt->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	strT.Format(_T("file='%s' dest='0,8,16,24'"), strResPath + _T("\\choicebox_unsel.png"));
	pOpt->SetNormalImage(strT);
	strT.Format(_T("file='%s' dest='0,8,16,24'"), strResPath + _T("\\choicebox_sel.png"));
	pOpt->SetSelectedImage(strT);

	return pOpt;
}
CControlUI *ZPage_Sign::BuildBox_Save()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");
	ZVerticalBox *pVBoxRoot = new ZVerticalBox;

	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pHBox->SetFixedHeight(67);
		pVBoxRoot->Add(pHBox);

		CLabelUI *pImg = UI_CreateLab(NULL, _T("4"), 0xffffffff, FontSize_40, 57, 67);
		pImg->SetTextPadding({ 0,0,0,10 });
		pImg->SetBkImage(strResPath + _T("export_bg.png"));
		pHBox->Add(pImg);

		ZVerticalBox *pVBox = new ZVerticalBox;
		pHBox->Add(pVBox);
		{
			pVBox->AddStretch();

			CString strTemp;
			CLabelUI *pLab = UI_CreateLab(NULL, _T("保存"), 0xff000000, FontSize_16, 0, 28);
			pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT);
			pLab->SetTextPadding({ 58,0,0,0 });
			strTemp.Format(_T("file='%s' source='0,0,28,28' dest='20,0,48,28'"), strResPath + _T("export_ic.png"));
			pLab->SetBkImage(strTemp);
			pVBox->Add(pLab);

			pVBox->AddStretch();
			pVBox->AddHLine(0xffff9200);
			pVBox->AddSpace(10);
		}
	}
	pVBoxRoot->AddStretch();

	ZHorizontalBox *pHBoxEdit = new ZHorizontalBox;
	pHBoxEdit->SetFixedHeight(40);
	pVBoxRoot->Add(pHBoxEdit);
	pHBoxEdit->AddSpace(20);
	{
		ZHorizontalBox *pHBox = new ZHorizontalBox;
		pHBox->SetBorderColor(0xffaaaaaa);
		pHBox->SetBorderSize(1);
		pHBoxEdit->Add(pHBox);

		{
			ZEditUI *pEdit = new ZEditUI;
			pEdit->SetName(EditName_SavePath);
			pEdit->SetFont(FontSize_14);
			pEdit->SetPadding({10,2,2,2});
			pHBox->Add(pEdit);
		}
		pHBox->AddSpace(5);
		{
			CButtonUI *pBtn = UI_CreateBtn(BtnName_SelectPath,NULL,0xffffffff,FontSize_14,24,20);
			pHBox->Add(pBtn);
			pBtn->SetBkImage(strResPath + _T("choice_file_nor.png"));
			pBtn->SetPadding({0,10,0,0});
		}
		pHBox->AddSpace(20);
	}
	pHBoxEdit->AddSpace(20);
	pHBoxEdit->Add(CreateBtn(BtnName_Save, _T("保存")));
	pHBoxEdit->AddSpace(20);

	pVBoxRoot->AddStretch();
	{
		COptionUI *pOpt = CreateOpt(OptName_CleanImgAfterSave, _T("保存完成后清除显示的图像"));
		pOpt->SetPadding({20,0,0,0});
		pVBoxRoot->Add(pOpt);
	}
	pVBoxRoot->AddSpace(20);
	pVBoxRoot->SetBkColor(0xffffffff);
	pVBoxRoot->SetFixedHeight(200);
	return pVBoxRoot;
}

void ZPage_Sign::OnSelectPath()
{
	
	BROWSEINFO  BrowInfo;
	memset(&BrowInfo, 0, sizeof(BrowInfo));
	LPCITEMIDLIST pList = SHBrowseForFolder(&BrowInfo);
	if (NULL == pList) return;

	TCHAR  szDisplayName[255];
	SHGetPathFromIDList(pList, szDisplayName);
	CString strDir(szDisplayName);
	
	CEditUI *pEdit = (CEditUI *)FindSubControl(EditName_SavePath);
	if (pEdit) pEdit->SetText(strDir);
	ZCfg::SaveCfg_SavePath(strDir);
	/*
	CFileDialog dlg(false, _T("png File(*.png;*.png)|*.png;*.png"), NULL, NULL, _T("png File(*.png;*.png)|*.png;*.png"), NULL);
	if (IDOK != dlg.DoModal()) return;

	CString strPath = dlg.GetPathName();

	CEditUI *pEdit = (CEditUI *)FindSubControl(EditName_SavePath);
	if (pEdit) pEdit->SetText(strPath);
	*/
}

void ZPage_Sign::OnStartSign()
{
	m_bStartSign = TRUE;
	g_SignData.RemoveAllPoints();
	m_pCanvasSign->Invalidate();

	RECT rcWnd = {0,0,0,0};
	{
		ZMultiMonitorEnumerator Enumerator;
		Enumerator.EnumDisplayMonitors();
		for (int i = 0; i < Enumerator.m_vectMonitors.size(); i++)
		{
			if (Enumerator.m_vectMonitors[i].m_bPrimary) continue;
			rcWnd = Enumerator.m_vectMonitors[i].m_rcMonitor;
			break;
		}
	}
	
	if (rcWnd.right - rcWnd.left > 0)
	{
		ZDlg_Sign::GetInstance()->ShowModal(GetWindow(), rcWnd);
		if (g_hSignDlg) ::PostMessage(g_hSignDlg, WM_UpdataSign, 0, 0);
	}
}
void ZPage_Sign::OnResetSign()
{
	OnStartSign();
	OnCleanImg();
}

void ZPage_Sign::HandlePack(CReqPack &ReqPack)
{
	if (Req_GetFinger == ReqPack.m_Type && m_bGetFinger)
	{
		static CImage ImgTemp;
		ZFingerObj::emErrCode nErr = m_FingerObj.GetFingerImg(ImgTemp, m_bRedColor);
		if (ZFingerObj::Err_OK == nErr)
		{
			ImageCopy(ImgTemp, m_FingerImg);
			::SendMessage(GetWindow(),WM_UpdateFingerImg,0,0);
		}
		else if (ZFingerObj::Err_Disconnected == nErr)
		{
			::SendMessage(GetWindow(), WM_Disconnected, 0, 0);
		}
		else m_WorkThread.AddReqPack(ReqPack);
	}
}
void ZPage_Sign::OnOpenFinger()
{
	m_FingerImg.Destroy();
	m_pCanvasFinger->Invalidate();

	BOOL bRet = m_FingerObj.OpenFingerDevice();
	if (bRet)
	{
		m_bGetFinger = TRUE;
		m_WorkThread.AddReqPack(CReqPack(Req_GetFinger));
	}
	
	CLabelUI *pLab = (CLabelUI *)FindSubControl(LabName_FingerDeviceStatu);
	if (pLab)
	{
		if (bRet)
		{
			pLab->SetText(_T("指纹传感器已连接"));
			pLab->SetTextColor(0xff00d197);
		}
		else
		{
			pLab->SetText(_T("指纹传感器未连接"));
			pLab->SetTextColor(0xffff0000);
		}
	}
}
void ZPage_Sign::OnCloseFinger()
{
	m_bGetFinger = FALSE;
	m_FingerObj.CloseFingerDevice();
	
	m_FingerImg.Destroy();
	m_pCanvasFinger->Invalidate();
	CLabelUI *pLab = (CLabelUI *)FindSubControl(LabName_FingerDeviceStatu);
	if (pLab)
	{
		pLab->SetText(_T("指纹传感器未连接"));
		pLab->SetTextColor(0xffff0000);
	}

	OnCleanImg();
}
void ZPage_Sign::OnMergeImg()
{
	if (NULL == m_pCanvasMergeDC)
	{
		HDC hDC = m_pManager->GetPaintDC();
		RECT rcUI = m_pCanvasMerge->GetPos();
		int nW = rcUI.right - rcUI.left;
		int nH = rcUI.bottom - rcUI.top;
		m_pCanvasMergeDC = new ZMemDC(hDC, nW, nH);
	}

	SIZE szDC = m_pCanvasMergeDC->GetSize();
	RECT rcDC = {0,0, szDC.cx,szDC.cy};
	::FillRect(m_pCanvasMergeDC->DC(), &rcDC, GetStockBrush(WHITE_BRUSH));

	if(m_FingerImg.IsNull() == false)
	{
		RECT rcFinger = m_pCanvasFinger->GetPos();
		int nW = rcFinger.right - rcFinger.left;
		int nH = rcFinger.bottom - rcFinger.top;
		int nL = (szDC.cx - nW) / 2;
		int nT = (szDC.cy - nH) / 2;
		RECT rcImg = { nL , nT , nL + nW, nT + nH};
		m_FingerImg.Draw(m_pCanvasMergeDC->DC(), rcImg);
	}
	
	g_SignData.DrawToDC(m_pCanvasMergeDC->DC(), rcDC);
	m_pCanvasMerge->Invalidate();

	if (g_SignData.m_vectPts.size() == 0 || m_FingerImg.IsNull()) m_bMergeDCIsValid = FALSE;
	else m_bMergeDCIsValid = TRUE;
}
void ZPage_Sign::OnCleanImg()
{
	m_bMergeDCIsValid = FALSE;
	if (m_pCanvasMergeDC)
	{
		SIZE szDC = m_pCanvasMergeDC->GetSize();
		RECT rcDC = {0,0,szDC.cx,szDC.cy};
		::FillRect(m_pCanvasMergeDC->DC(), &rcDC, GetStockBrush(WHITE_BRUSH));
		m_pCanvasMerge->Invalidate();
	}
	else
	{
		RECT rcUI = m_pCanvasMerge->GetPos();
		::InflateRect(&rcUI, -1, -1);
		::FillRect(m_pManager->GetPaintDC(), &rcUI, GetStockBrush(WHITE_BRUSH));
	}
}


static CString TimeToSting(INT64 nTime)
{
	CTime cTime(nTime);
	CString strTime;

	int nYear = cTime.GetYear();
	int nMonth = cTime.GetMonth();
	int nDay = cTime.GetDay();
	int nH = cTime.GetHour();
	int nM = cTime.GetMinute();
	int nS = cTime.GetSecond();
	
	strTime.Format(_T("%4d%02d%02d_%02d_%02d_%02d"), nYear, nMonth, nDay, nH, nM, nS);

	return strTime;
}


void ZPage_Sign::OnSave()
{
	if (g_SignData.m_vectPts.size() == 0 && m_FingerImg.IsNull()) return;

	CString strTime = TimeToSting(ZGlobal::Time_GetTime(FALSE));
	CString strPath = FindSubControl(EditName_SavePath)->GetText();

	HRESULT nRet = -1;
	if (g_SignData.m_vectPts.size() > 0)
	{
		SIZE szDC = g_pSignDC->GetSize();
		Gdiplus::Bitmap *pBmp = ZGDI::GetBitmapOfDC(g_pSignDC->DC(), Gdiplus::Rect(0, 0, szDC.cx, szDC.cy));
		SetTransparentColor(pBmp);
		
		CString strSaveDir = strPath + _T("\\") + strTime;
		ZDirObj::CreateDirectory(strSaveDir);

		CLSID pngClsid;
		ZMemImage::GetEncoderClsid(_T("image/png"), &pngClsid);
		nRet = pBmp->Save(strSaveDir + _T("\\sign_") + strTime + _T(".png"), &pngClsid);
		delete pBmp;
	}
	if (m_FingerImg.IsNull() == false)
	{
		RECT rcUI = m_pCanvasFinger->GetPos();
		::InsetRect(&rcUI,-2,-2);
		int nW = rcUI.right - rcUI.left;
		int nH = rcUI.bottom - rcUI.top;
		Gdiplus::Bitmap *pBmp = ZGDI::GetBitmapOfDC(m_pManager->GetPaintDC(), Gdiplus::Rect(rcUI.left, rcUI.top, nW, nH));

		if (m_FingerObj.GetDevType() != DevType_Finger5) SetTransparentColor(pBmp);
		else SetTransparentColor(pBmp, RGB(255, 255, 255));

		CString strSaveDir = strPath + _T("\\") + strTime;
		ZDirObj::CreateDirectory(strSaveDir);

		CLSID pngClsid;
		ZMemImage::GetEncoderClsid(_T("image/png"), &pngClsid);
		nRet = pBmp->Save(strSaveDir + _T("\\fingerprint_") + strTime + _T(".png"),&pngClsid);
		delete pBmp;
	}
	if (g_SignData.m_vectPts.size() > 0 && m_FingerImg.IsNull() == false && m_bMergeDCIsValid)
	{
		SIZE szDC = m_pCanvasMergeDC->GetSize();
		Gdiplus::Bitmap *pBmp = ZGDI::GetBitmapOfDC(m_pCanvasMergeDC->DC(), Gdiplus::Rect(0, 0, szDC.cx, szDC.cy));
		
		if (m_FingerObj.GetDevType() != DevType_Finger5) SetTransparentColor(pBmp);
		//else SetTransparentColor(pBmp, RGB(255, 255, 255));

		CString strSaveDir = strPath + _T("\\") + strTime;
		ZDirObj::CreateDirectory(strSaveDir);

		CLSID pngClsid;
		ZMemImage::GetEncoderClsid(_T("image/png"), &pngClsid);
		nRet = pBmp->Save(strSaveDir + _T("\\merge_") + strTime + _T(".png"), &pngClsid);
		delete pBmp;
	}
	
	if (0 == nRet)
	{
		::AfxMessageBox(ZTextConvert::Text(_T("保存成功")));
		ZCfg::SaveCfg_SavePath(strPath);
	}
	else
	{
		::AfxMessageBox(ZTextConvert::Text(_T("请输入有效的路径名")));
		return;
	}

	if (((COptionUI *)FindSubControl(OptName_CleanImgAfterSave))->IsSelected())
	{
		g_SignData.RemoveAllPoints();
		m_FingerImg.Destroy();
		m_pCanvasFinger->Invalidate();
		m_pCanvasSign->Invalidate();
		OnCleanImg();

		Invalidate();
		if (g_hSignDlg) ::PostMessage(g_hSignDlg, WM_UpdataSign, 0, 0);
	}
}
void ZPage_Sign::Notify(TNotifyUI& msg)
{
	if (NULL == msg.pSender) return;
	CString strName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if      (BtnName_StartSign   == strName) OnStartSign();
		else if (BtnName_ResetSign   == strName) OnResetSign();
		else if (BtnName_OpenFinger  == strName) OnOpenFinger();
		else if (BtnName_CloseFinger == strName) OnCloseFinger();
		else if (BtnName_MergeImg    == strName) OnMergeImg();
		else if (BtnName_CleanImg    == strName) OnCleanImg();
		else if (BtnName_SelectPath  == strName) OnSelectPath();
		else if (BtnName_Save        == strName) OnSave();
	}
	else if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (OptName_FingerColor_Black == strName)
		{
			m_bRedColor = FALSE;
			OnCleanImg();
			ZCfg::SaveCfg_Red(m_bRedColor);
		}
		else if (OptName_FingerColor_Red == strName)
		{
			m_bRedColor = TRUE;
			OnCleanImg();
			ZCfg::SaveCfg_Red(m_bRedColor);
		}
		else if (OptName_CleanImgAfterSave == strName)
		{
			BOOL bSelected = ((COptionUI *)msg.pSender)->IsSelected();
			ZCfg::SaveCfg_CleanImgAfterSave(bSelected);
		}
	}
}

LRESULT ZPage_Sign::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (WM_TabletEvent == uMsg)
	{
		OnTabletEventProc((emTabletEventType)wParam, (sTabletInfo *)lParam);
		bHandled = true;
	}
	else if (WM_TabletData == uMsg)
	{
		OnTabletData( wParam, lParam);
		bHandled = true;
	}
	else if (WM_UpdateFingerImg == uMsg)
	{
		m_pCanvasFinger->Invalidate();

		m_WorkThread.AddReqPack(CReqPack(Req_GetFinger));//继续采集指纹
	}
	else if (WM_Disconnected == uMsg)
	{
		OnCloseFinger();
	}
	else if (WM_UpdataSign == uMsg)
	{
		m_pCanvasSign->Invalidate();
	}
	return 0;
}


LRESULT ZPage_Sign::OnTabletData(WPARAM wParam, LPARAM lParam)
{
	ULONG nID = (ULONG)wParam;
	sTabletData *pTabletData = NULL;
	if (FALSE == g_DataDic.GetItem(nID, pTabletData, TRUE)) return 0;

	emTabletDataType DataType = pTabletData->m_DataType;
	uTabletData tbData = pTabletData->m_tbData;
	delete pTabletData;

	if (g_hSignDlg)
	{
		SendMessage(g_hSignDlg, WM_TabletDataForSignDlg, DataType, (LPARAM)&tbData);
		return 0;
	}

	if (emTabletDataType::DataType_Key == DataType)
	{
	}
	else if (emTabletDataType::DataType_Knob == DataType)
	{
	}
	else if (emTabletDataType::DataType_Pen == DataType && m_bStartSign)
	{
		g_SignData.AddPoint({ tbData.m_Pen.m_nX ,tbData.m_Pen.m_nY }, tbData.m_Pen.m_nP);

		if (g_pSignDC)
		{
			SIZE szDC = g_pSignDC->GetSize();
			Gdiplus::Point ptIn;
			ptIn.X = tbData.m_Pen.m_nX * szDC.cx / g_TabletInfo.m_nMaxX;
			ptIn.Y = tbData.m_Pen.m_nY * szDC.cy / g_TabletInfo.m_nMaxY;

			static Gdiplus::Point ptS = { 0,0 };
			if (tbData.m_Pen.m_nP == 0) ptS = ptIn;
			else
			{
				Gdiplus::Graphics Graph(g_pSignDC->DC());
				m_pPen->SetWidth(tbData.m_Pen.m_nP / 1000 + 1);
				Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

				Graph.DrawLine(m_pPen, ptS, ptIn);
				m_pCanvasSign->Invalidate();

				ptS = ptIn;
			}
		}
		
	}
	return 0;
}
void ZPage_Sign::OnTabletEventProc(emTabletEventType EventType, IN const sTabletInfo *pTablet)
{
	if (emTabletEventType::EventType_TabletConnected == EventType)
	{
		g_TabletInfo = *pTablet;
		g_SignData.m_nMaxP = g_TabletInfo.m_nMaxP;
		g_SignData.m_nMaxX = g_TabletInfo.m_nMaxX;
		g_SignData.m_nMaxY = g_TabletInfo.m_nMaxY;
		CLabelUI *pLab = (CLabelUI *)FindSubControl(LabName_SignDeviceStatu);
		if (pLab)
		{
			pLab->SetText(_T("签名设备已连接"));
			pLab->SetTextColor(0xff00d197);
		}

		FindSubControl(BtnName_StartSign)->SetEnabled(true);
		FindSubControl(BtnName_ResetSign)->SetEnabled(true);
	}
	else if (emTabletEventType::EventType_TabletDisconnected == EventType)
	{
		memset(&g_TabletInfo,0,sizeof(g_TabletInfo));
		CLabelUI *pLab = (CLabelUI *)FindSubControl(LabName_SignDeviceStatu);
		if (pLab)
		{
			pLab->SetText(_T("签名设备未连接"));
			pLab->SetTextColor(0xffff0000);
		}
		FindSubControl(BtnName_StartSign)->SetEnabled(false);
		FindSubControl(BtnName_ResetSign)->SetEnabled(false);
	}
}
