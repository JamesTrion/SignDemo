#include "stdafx.h"
#include "ZMainFrame2.h"
#include <Dbt.h>

#define W_SysBtn (16)
#define H_SysBtn (16)
#define H_TopBar (45)

#define BtnName_Max _T("CButtonUI_最大化")
#define BtnName_Min _T("CButtonUI_最小化")
#define BtnName_Restore _T("CButtonUI_还原")
#define BtnName_Close _T("CButtonUI_关闭")

#define OptName_LangItem   _T("ZMainFrame2_COptionUI_LangItem")

ZMainFrame2::ZMainFrame2(void)
{
	EnableMaximize(TRUE);
	m_bMinMaxFlag = true;
	m_bRestoreFlag = false;
}

ZMainFrame2::~ZMainFrame2(void)
{
	g_hMainWnd = NULL;
}

void ZMainFrame2::OnFinalMessage(HWND hWnd)
{
	delete this;
}

void ZMainFrame2::InitWindow()
{
	SetIcon(IDR_MAINFRAME);
	g_hMainWnd = m_hWnd;
	{
		std::vector<sLang> vectLang;
		ZTextConvert::GetLangList(vectLang);
		InitComb_Language(m_pCombLanguage, vectLang);
	}
}
void ZMainFrame2::InitComb_Language(ZComboUI *pComb, IN std::vector<sLang> &vectLang)
{
	if (NULL == pComb) return;
	pComb->RemoveAll();

	CString strGroupName = _T("Group_Language");
	CString strLangID = ZTextConvert::GetCurLangName().Trim();
	int nIndex = 0;
	for (size_t i = 0; i < vectLang.size(); i++)
	{
		CListContainerElementUI *pItem = new CListContainerElementUI;
		pItem->SetText(vectLang[i].m_strName);
		pItem->SetUserData(vectLang[i].m_strLangID);
		pItem->SetFixedHeight(41);
		pItem->SetBkColor(0xffffffff);
		pComb->Add(pItem);

		ZVerticalBox *pVBoxOpt = new ZVerticalBox;
		pItem->Add(pVBoxOpt);
		{
			pVBoxOpt->AddStretch();
			COptionUI *pOpt = UI_CreateOpt(OptName_LangItem, vectLang[i].m_strName, 0xff000000, FontSize_14, 0, 28, strGroupName);
			pOpt->SetHotBkColor(0x22000000);
			CString strT;
			strT.Format(_T("file='%s' corner='10,10,10,10'"), CPaintManagerUI::GetResourcePath() + _T("UI\\combItemBk.png"));
			pOpt->SetSelectedImage(strT);
			pOpt->SetSelectedTextColor(0xff0000ff);
			//pOpt->SetPadding({ 5,0,5,0 });
			pOpt->SetTag(i);
			pOpt->OnEvent += MakeDelegate(this, &ZMainFrame2::LanguageItemEvent);
			pVBoxOpt->Add(pOpt);

			if (strLangID.CompareNoCase(vectLang[i].m_strLangID) == 0)
			{
				nIndex = pComb->GetItemIndex(pItem);
				pOpt->Selected(true, false);
			}

			pVBoxOpt->AddStretch();
		}
	}
	pComb->SelectItem(-1, false);
	pComb->SelectItem(nIndex, false, true);
}
bool ZMainFrame2::LanguageItemEvent(void *pParam)
{
	TEventUI *pEvent = (TEventUI *)pParam;
	if (NULL == pEvent->pSender) return true;

	if (UIEVENT_BUTTONDOWN == pEvent->Type)
	{
		m_pCombLanguage->SelectItem(pEvent->pSender->GetTag(), false, true);
	}

	return true;
}

#define W_FRAME_MAX (1400)
#define H_FRAME_MAX (900)
#define W_FRAME_MIN (1400)
#define H_FRAME_MIN (900)
void ZMainFrame2::InitSize()
{
	RECT rectSize = { 4,4,4,4 };
	m_PaintManager.SetSizeBox(rectSize);
	m_PaintManager.SetInitSize(W_FRAME_MAX, H_FRAME_MAX);
	m_PaintManager.SetMinInfo(W_FRAME_MAX, H_FRAME_MAX);
	m_PaintManager.SetMaxInfo(W_FRAME_MIN, H_FRAME_MIN);

	RECT rcCaption = { 0,0,0,H_TopBar};
	m_PaintManager.SetCaptionRect(rcCaption);

		//original codes , marked by Trion on 2025/04/21
	m_PaintManager.SetDefaultFont(_T("微软雅黑"), 12, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 12, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 14, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 16, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 20, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 30, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 22, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 21, false, false, false);
	m_PaintManager.AddFont(_T("微软雅黑"), 40, true, false, false);
	

	/*	//added by Trion on 2025/04/21
	m_PaintManager.SetDefaultFont(_T("Arial"), 12, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 12, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 14, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 16, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 20, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 30, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 22, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 21, false, false, false);
	m_PaintManager.AddFont(_T("Arial"), 40, true, false, false);
	*/

	//一开始就最大化
	//PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	//::SetWindowPos(m_PaintManager.GetPaintWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

CContainerUI* ZMainFrame2::BuildItems()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("MainFrame\\");
	CContainerUI *pMainBox = new CContainerUI;

	ZVerticalBox *pVBox = new ZVerticalBox;
	{
		pVBox->Add(BuildTopBar());

		CControlUI *pLine = new CControlUI;
		pLine->SetFixedHeight(2);
		pLine->SetBkImage(strResPath + _T("line.png"));
		pVBox->Add(pLine);

		pVBox->Add(BuildBody()); //page 页面管理
	}
		//------------------------------------------------------------------//
	pMainBox->Add(pVBox);

	RECT rc = { 1,1,1,1 };
	pMainBox->SetInset(rc);
	pMainBox->SetBorderSize(1);
	pMainBox->SetBorderColor(0xff333333);
	pMainBox->SetBkColor(0xfff0f2f5);
	return pMainBox;
}

ZHorizontalBox *ZMainFrame2::BuildTopBar()
{
	CString strResPath = CPaintManagerUI::GetResourcePath() + _T("TitleBar\\");

	ZHorizontalBox *pTopBar = new ZHorizontalBox;
	pTopBar->SetBkColor(0xffffffff);
	pTopBar->SetFixedHeight(H_TopBar);
	pTopBar->AddSpace(10);
	//-----------------------------------------------------//
	{
		ZControlUI *pIcon = new ZControlUI;
		pIcon->SetFixedHeight(26);
		pIcon->SetFixedWidth(32);
		pIcon->SetBkImage(strResPath + _T("huion_logo.png"));
		pIcon->SetPadding({10,(H_TopBar - 26)/2,0,0});
		pTopBar->Add(pIcon);
	}
	pTopBar->AddSpace(10);
	{
		//std::string strTitle = getHuionStrCn();
		//CString strT = ZStrCoding::AnsiToUnicode(strTitle).c_str();
		CLabelUI *pLab = UI_CreateLab(NULL, _T("签批演示软件 V1.0.1"), 0xff000000, FontSize_16, 0, 0);
		pLab->SetTextStyle(DT_SINGLELINE | DT_VCENTER | DT_LEFT);
		pTopBar->Add(pLab);
	}
	//-------------------------------------------------------------------------------------------------//
	{
		ZComboUI *pCombLanguage = new ZComboUI;
		m_pCombLanguage = pCombLanguage;
		pTopBar->Add(pCombLanguage);
		pCombLanguage->SetFixedHeight(30);
		pCombLanguage->SetFixedWidth(200);
		pCombLanguage->SetSelectedItemTextColor(0xff333333);
		pCombLanguage->SetItemFont(FontSize_14);
		pCombLanguage->SetItemTextPadding({ 7,0,0,0 });
		pCombLanguage->SetTextPadding({ 10,0,15,0 });
		pCombLanguage->SetDropBoxSize({ 0,500 });
		pCombLanguage->SetBorderColor(0xffaaaaaa);
		pCombLanguage->SetBorderSize(1);
		pCombLanguage->SetPadding({0,(H_TopBar - pCombLanguage->GetFixedHeight())/2,0,0});
		CString strImg;
		strImg.Format(_T("file='%s' dest='174,11,190,20' "), strResPath + _T("downArrow2.png"));
		pCombLanguage->SetBkImage(strImg);
	}
	pTopBar->AddSpace(100);
	//-------------------------------------------------------------------------------------------------//
	{
		CString strTemp;
		CButtonUI *pMinBtn = UI_CreateBtn(BtnName_Min, NULL, 0xff000000, FontSize_14, W_SysBtn, H_SysBtn);
		strTemp.Format(_T("file='%s' source='0,0,16,16'"), strResPath + _T("min.png"));
		pMinBtn->SetNormalImage(strTemp);
		pMinBtn->SetHotImage(strTemp);
		pMinBtn->SetPushedImage(strTemp);
		pMinBtn->SetPadding({0,(H_TopBar - 16)/2,0,0});
		pTopBar->Add(pMinBtn);
        
		pTopBar->AddSpace(20);

		CButtonUI *pCloseBtn = UI_CreateBtn(BtnName_Close, NULL, 0xff000000, FontSize_14, W_SysBtn, H_SysBtn);
		strTemp.Format(_T("file='%s' source='0,0,16,16'"), strResPath + _T("close.png"));
		pCloseBtn->SetNormalImage(strTemp);
		pCloseBtn->SetHotImage(strTemp);
		pCloseBtn->SetPushedImage(strTemp);
		pCloseBtn->SetPadding({ 0,(H_TopBar - 16) / 2,0,0 });
		pTopBar->Add(pCloseBtn);
		pTopBar->AddSpace(10);
	}

	return pTopBar;
}

static COptionUI *CreateOpt2(CString strName, CString strText, DWORD dwTColor, int nFont, int nW, int nH, CString strGroupName)
{
	COptionUI *pOpt = UI_CreateOpt(strName, strText, dwTColor, nFont, nW, nH, strGroupName);
	pOpt->SetBorderSize(1);
	pOpt->SetBorderColor(0xffaaaaaa);
	pOpt->SetSelectedTextColor(0xff00ff00);
	pOpt->SetSelectedBkColor(0);
	return pOpt;
}

CControlUI* ZMainFrame2::BuildBody()
{
	CTabLayoutUI *pTab = new CTabLayoutUI;
	{
		pTab->Add(new ZPage_Sign);
	}
	return pTab;
}


void ZMainFrame2::OnLanguageChanged()
{
	int nIndex = m_pCombLanguage->GetCurSel();
	if (-1 == nIndex) return;
	CString strLangID = m_pCombLanguage->GetItemAt(nIndex)->GetUserData();

	{
		CListContainerElementUI *pItem = (CListContainerElementUI *)m_pCombLanguage->GetItemAt(nIndex);
		COptionUI *pOpt = (COptionUI *)pItem->FindSubControl(OptName_LangItem);
		if (pOpt) pOpt->Selected(true, false);
	}

	ZTextConvert::SetCurLangName(strLangID);
	ZLangMng::ResetText(strLangID);
	ZCfg::SaveCfg_CurLang(strLangID);
}
void ZMainFrame2::Notify(TNotifyUI& msg)
{
	if (NULL == msg.pSender) return;
	CString strSenderName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if     (BtnName_Close == strSenderName) OnClose();
		else if(BtnName_Min   == strSenderName) OnMinimize();
	}
	else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if (msg.pSender == m_pCombLanguage) OnLanguageChanged();
	}
	else __super::Notify(msg);
}

LRESULT ZMainFrame2::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ZMainFrame2::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

void ZMainFrame2::WndZoomedAdjust()
{
	if (::IsZoomed(*this))
	{
		CRect rcWorkArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
		MoveWindow(GetHWND(), rcWorkArea.left, rcWorkArea.top, rcWorkArea.Width(), rcWorkArea.Height(), TRUE);

		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(BtnName_Max));
		if (pControl) pControl->SetVisible(FALSE);
		pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(BtnName_Restore));
		if (pControl) pControl->SetVisible(TRUE);
	}
	else
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(BtnName_Max));
		if (pControl) pControl->SetVisible(TRUE);
		pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(BtnName_Restore));
		if (pControl) pControl->SetVisible(FALSE);
	}
}
LRESULT ZMainFrame2::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
#if defined(WIN32) && !defined(UNDER_CE)
	switch (wParam)
	{
	case SC_CLOSE:
		SendMessage(WM_CLOSE);
		return 0;
	}

	BOOL bZoomedOld = ::IsZoomed(*this);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	BOOL bZoomedNew = ::IsZoomed(*this);
	if (bZoomedNew != bZoomedOld) WndZoomedAdjust();

#else
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
	return lRes;//__super::OnSysCommand(uMsg, wParam, lParam,bHandled);
}

void ZMainFrame2::OnClose()
{
	Close();
}
void ZMainFrame2::OnMinimize()
{
	m_bMinMaxFlag = false;
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void ZMainFrame2::OnMaxSize()
{
	m_bMinMaxFlag = true;
	m_bRestoreFlag = false;
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void ZMainFrame2::OnRestoreSize()
{
	m_bRestoreFlag = true;
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

LRESULT ZMainFrame2::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right;
	lpMMI->ptMaxSize.y = rcWork.bottom;

	bHandled = FALSE;
	return 0;
}
