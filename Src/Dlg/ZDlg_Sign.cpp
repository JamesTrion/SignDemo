#include "StdAfx.h"
#include "ZDlg_Sign.h"

#define BtnName_ResetSign _T("CButtonUI_ZDlg_Sign_ResetSign")
#define BtnName_OK        _T("CButtonUI_ZDlg_Sign_OK")

#define H_BtnBar (60)
ZDlg_Sign *ZDlg_Sign::m_pInstance = NULL;
ZDlg_Sign *ZDlg_Sign::GetInstance()
{
	if (NULL != m_pInstance) return m_pInstance;
	else
	{
		m_pInstance = new ZDlg_Sign;
	}
	return m_pInstance;
}

ZDlg_Sign::ZDlg_Sign(void)
{
	m_CanvasUI = NULL;
	m_rcWnd = {0,0,800,500};
}

ZDlg_Sign::~ZDlg_Sign(void)
{
	if (m_pPen) ::delete m_pPen;
	m_pInstance = NULL;
	g_hSignDlg = NULL;
}

UINT ZDlg_Sign::ShowModal(HWND hParentWnd,RECT rcWnd)
{
	//added by Trion on 2025/05/27
	this->m_bVertical=this->CheckForVerticalMonitor(rcWnd);
	//added by Trion on 2025/05/27

	if (NULL != m_hWnd)
	{
		Close(0);
		return 0;
	}
	m_rcWnd = rcWnd;

	if (NULL == GetHWND())
	{
		Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_NOACTIVATE, m_rcWnd);//WS_EX_PALETTEWINDOW UI_WNDSTYLE_FRAME
	}

	//ShowWindow(true);
#ifdef _DEBUG
	::SetWindowPos(m_hWnd, HWND_TOP, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, SWP_SHOWWINDOW);
#else 
	::SetWindowPos(m_hWnd, HWND_TOPMOST, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, SWP_SHOWWINDOW);
	::SetForegroundWindow(m_hWnd);
#endif
	g_hSignDlg = m_hWnd;
	return __super::ShowModal();
}

#define FontSize_12 (0)
#define FontSize_14 (1)
#define FontSize_18 (2)

void ZDlg_Sign::InitSize()
{
	m_PaintManager.SetInitSize(m_rcWnd.right - m_rcWnd.left, m_rcWnd.bottom - m_rcWnd.top);
	m_PaintManager.SetDefaultFont(_T("Microsoft YaHei"), 13, false, false, false);

	m_PaintManager.AddFont(_T("峚捇窪"), 12, false, false, false);
	m_PaintManager.AddFont(_T("峚捇窪"), 14, false, false, false);
	m_PaintManager.AddFont(_T("峚捇窪"), 18, true, false, false);

	m_pPen = ::new Gdiplus::Pen(Gdiplus::Color(0, 0, 0), 2);
	m_pPen->SetStartCap(Gdiplus::LineCapRound);
	m_pPen->SetEndCap(Gdiplus::LineCapRound);
}
void ZDlg_Sign::InitWindow()
{
	DWORD style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	style &= ~WS_EX_APPWINDOW;
	style = style | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, style);

	style = GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_SIZEBOX);
	SetWindowLong(m_hWnd, GWL_STYLE, style | WS_POPUP);

	RECT CaptionRect = { 0,0,0,0 };
	m_PaintManager.SetCaptionRect(CaptionRect);

	{
		INT64 nH = m_rcWnd.bottom - m_rcWnd.top;
		m_szTable = { g_TabletInfo.m_nMaxX,g_TabletInfo.m_nMaxY };
		m_szTable.cy = g_TabletInfo.m_nMaxY * (nH - H_BtnBar)/nH;
	}

	::SetForegroundWindow(m_hWnd);
}
void ZDlg_Sign::OnFinalMessage(HWND hWnd)
{
	delete this;
}
LRESULT ZDlg_Sign::OnClose(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(nMsg,  wParam,  lParam,  bHandled);
}

static void PaintFunc_CanvasUI(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint, void * pUserParam)
{
	((ZDlg_Sign *)pUserParam)->Paint_Sign(pCanvas, hDC, rcPaint);
}
void ZDlg_Sign::Paint_Sign(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint)
{
	if (pCanvas)
	{
		RECT rcUI = pCanvas->GetPos();
		g_SignData.DrawToDC(hDC, rcUI);
	}
}
CContainerUI *ZDlg_Sign::BuildItems()
{
	CContainerUI *pRoot = new CContainerUI;
	{
		m_CanvasUI = new ZCanvasUI;
		m_CanvasUI->SetBkColor(0xffffffff);
		m_CanvasUI->SetPaintFunc(PaintFunc_CanvasUI, (void *)this);
		pRoot->Add(m_CanvasUI);
	}

	{
		ZHorizontalBox *HBoxBtns = new ZHorizontalBox;
		pRoot->Add(HBoxBtns);
		HBoxBtns->SetFixedHeight(H_BtnBar);
		HBoxBtns->SetFixedWidth(m_rcWnd.right - m_rcWnd.left);
		HBoxBtns->SetTopBorderSize(1);
		HBoxBtns->SetBorderColor(0xffaaaaaa);
		HBoxBtns->AddStretch();
		HBoxBtns->Add(m_pBtnReSign = CreateBtn(BtnName_ResetSign, _T("重新签名"))); 
		HBoxBtns->AddSpace(20);
		HBoxBtns->Add(m_pBtnOK = CreateBtn(BtnName_OK, _T("确认"))); 
		HBoxBtns->AddSpace(20);

		HBoxBtns->SetFloat(true);
		HBoxBtns->SetFixedXY({ 0, -1 });
	}
	
	pRoot->SetBkColor(0xffffffff);
	return pRoot;
}
CButtonUI *ZDlg_Sign::CreateBtn(CString strName, CString strText)
{
	CButtonUI *pBtn = UI_CreateBtn(strName, strText, 0xffffffff, FontSize_16, 200, 0);
	pBtn->SetBkColor(0xff00bed6);
	pBtn->SetHotBkColor(0xff00b1c8);
	pBtn->SetBorderRound({ 3,3 });
	return pBtn;
}

void ZDlg_Sign::OnResetSign()
{
	g_SignData.RemoveAllPoints();
	::PostMessage(g_hMainWnd, WM_UpdataSign, 0, 0);
	m_CanvasUI->Invalidate();
}
void ZDlg_Sign::OnOK()
{
	Close(IDCANCEL);
}
void ZDlg_Sign::Notify(TNotifyUI& msg)
{
	if (NULL == msg.pSender) return;
	CString strSenderName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if      (BtnName_OK == strSenderName) OnOK();
		else if (BtnName_ResetSign == strSenderName) OnResetSign();
	}
}


LRESULT ZDlg_Sign::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_UpdataSign == uMsg)
	{
		m_CanvasUI->Invalidate();
	}
	else if (WM_TabletDataForSignDlg == uMsg) OnTabletData( wParam,  lParam);
	else return __super::HandleMessage(uMsg, wParam, lParam);
}
BOOL ZDlg_Sign::BtnHitTest(const uTabletData &tbData)
{
	static int OldP = 0;

	RECT rcWnd;
	::GetClientRect(m_hWnd, &rcWnd);

	INT64 nW = rcWnd.right - rcWnd.left;
	INT64 nH = rcWnd.bottom - rcWnd.top;

	POINT ptIn;
	ptIn.x = rcWnd.left + tbData.m_Pen.m_nX * nW / g_TabletInfo.m_nMaxX;
	ptIn.y = rcWnd.top  + tbData.m_Pen.m_nY * nH / g_TabletInfo.m_nMaxY;

	RECT rcReSign = m_pBtnReSign->GetPos();
	RECT rcOK = m_pBtnOK->GetPos();
	if (OldP > 0 && tbData.m_Pen.m_nP == 0)
	{
		if (::PtInRect(&rcReSign, ptIn))  OnResetSign();
		else if (::PtInRect(&rcOK, ptIn)) OnOK();
	}

	OldP = tbData.m_Pen.m_nP;

	if (ptIn.y >= rcOK.top) return TRUE;
	return FALSE;
}
void ZDlg_Sign::OnTabletData(WPARAM wParam, LPARAM lParam)
{
	return this->OnTabletDataEx(wParam, lParam);

	emTabletDataType DataType = (emTabletDataType)wParam;
	uTabletData tbData = *(uTabletData *)lParam;
	
	//***********added by Trion on 2025/05/26***********
	BOOL bVertical = this->IsVerticalMonitor();
	//***********added by Trion on 2025/05/26***********

	if (emTabletDataType::DataType_Pen == DataType)
	{
		static Gdiplus::Point ptS = { 0,0 };

		if (BtnHitTest(tbData))
		{
			tbData.m_Pen.m_nP = 0;
			return;
		}

		

		RECT rcCanvas = m_CanvasUI->GetPos();
		INT64 nW = rcCanvas.right - rcCanvas.left;
		INT64 nH = rcCanvas.bottom - rcCanvas.top;
		//nW = m_CanvasUI->GetWidth();				//added by Trion on 2025/05/27
		//nH = m_CanvasUI->GetHeight();				//added by Trion on 2025/05/27
		{
			g_SignData.AddPoint({ tbData.m_Pen.m_nX ,tbData.m_Pen.m_nY }, tbData.m_Pen.m_nP);
			
			SIZE szDC = g_pSignDC->GetSize();

			CDebugWriter::OutputDBGStringMultipleAutoCloseToAnsi
			(
				_T("Vertical is %d , original:( %d , %d ) , Table:( %d , %d ) , TabletMaxInfo:( %d , %d ) , Canvas( %d, %d ) , DC( %d , %d ) , Canvas( Left:%d , Top:%d , Right:%d , Bottom:%d ) , Canvas W/H( %d , %d )"),
				bVertical, tbData.m_Pen.m_nX, tbData.m_Pen.m_nY,
				m_szTable.cx, m_szTable.cy,							//drawing area dimension
				g_TabletInfo.m_nMaxX, g_TabletInfo.m_nMaxY,			//total tablet dimension
				nW, nH,												//canvas dimension (width and height)
				szDC.cx,szDC.cy,										//DC dimension (width and height)
				rcCanvas.left,rcCanvas.top,rcCanvas.right,rcCanvas.bottom,
				m_CanvasUI->GetWidth(),m_CanvasUI->GetHeight()
			);

			//draw on memory dc
			Gdiplus::Point ptIn;
			ptIn.X = tbData.m_Pen.m_nX * szDC.cx / m_szTable.cx;
			ptIn.Y = tbData.m_Pen.m_nY * szDC.cy / m_szTable.cy;

			static Gdiplus::Point ptS = { 0,0 };
			if (tbData.m_Pen.m_nP == 0) ptS = ptIn;
			else
			{
				Gdiplus::Graphics Graph(g_pSignDC->DC());
				m_pPen->SetWidth(tbData.m_Pen.m_nP / 1000 + 1);
				Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

				Graph.DrawLine(m_pPen, ptS, ptIn);

				ptS = ptIn;
			}
		}

		if (g_hMainWnd) ::PostMessage(g_hMainWnd,WM_UpdataSign,0,0);

		//draw on current window
		Gdiplus::Point ptIn;
		ptIn.X = rcCanvas.left + tbData.m_Pen.m_nX * nW / g_TabletInfo.m_nMaxX;
		ptIn.Y = rcCanvas.top  + tbData.m_Pen.m_nY * nH / g_TabletInfo.m_nMaxY;
		if (bVertical == TRUE)
		{
			double dbPercentX = (double)tbData.m_Pen.m_nX / (double)g_TabletInfo.m_nMaxX;
			double dbPercentY = (double)tbData.m_Pen.m_nY / (double)g_TabletInfo.m_nMaxY;
			long lnNewX = (long)(dbPercentX * (double)g_TabletInfo.m_nMaxY);
			long lnNewY = (long)(dbPercentY * (double)g_TabletInfo.m_nMaxX);

			ptIn.X = rcCanvas.left + lnNewX * nW / g_TabletInfo.m_nMaxY;
			ptIn.Y = rcCanvas.top +  lnNewY * nH / g_TabletInfo.m_nMaxX;
			CDebugWriter::OutputDBGStringMultipleAutoCloseToAnsi
			(
				_T("New XY( %d , %d ) , NewDimensin( %d , %d ) , NewPoint( %d , %d ) , Percent( %f , %f ) , CurrentTabletPos( %d , %d )"),
				lnNewX,lnNewY, g_TabletInfo.m_nMaxY, g_TabletInfo.m_nMaxX, ptIn.X,ptIn.Y,dbPercentX,dbPercentY,
				tbData.m_Pen.m_nX, tbData.m_Pen.m_nY
			);
		}

		if (tbData.m_Pen.m_nP == 0) ptS = ptIn;
		else
		{
			Gdiplus::Graphics Graph(m_hWnd);
			m_pPen->SetWidth(tbData.m_Pen.m_nP / 1000 + 1);
			Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

			Graph.DrawLine(m_pPen, ptS, ptIn);

			ptS = ptIn;
		}
	}

}

void ZDlg_Sign::OnTabletDataEx(WPARAM wParam, LPARAM lParam)
{
	emTabletDataType DataType = (emTabletDataType)wParam;
	uTabletData tbData = *(uTabletData*)lParam;

	//***********added by Trion on 2025/05/26***********
	BOOL bVertical = this->IsVerticalMonitor();
	//***********added by Trion on 2025/05/26***********

	if (emTabletDataType::DataType_Pen == DataType)
	{
		static Gdiplus::Point ptS = { 0,0 };

		if (BtnHitTest(tbData))
		{
			tbData.m_Pen.m_nP = 0;
			return;
		}


		RECT rcCanvas = m_CanvasUI->GetPos();
		INT64 nW = rcCanvas.right - rcCanvas.left;
		INT64 nH = rcCanvas.bottom - rcCanvas.top;
		//nW = m_CanvasUI->GetWidth();				//added by Trion on 2025/05/27
		//nH = m_CanvasUI->GetHeight();				//added by Trion on 2025/05/27
		{
			g_SignData.AddPoint({ tbData.m_Pen.m_nX ,tbData.m_Pen.m_nY }, tbData.m_Pen.m_nP);

			SIZE szDC = g_pSignDC->GetSize();
			//szDC.cx = (long)nW;
			//szDC.cy = (long)nH;

			CDebugWriter::OutputDBGStringMultipleAutoCloseToAnsi
			(
				_T("Vertical is %d , original:( %d , %d ) , Table:( %d , %d ) , TabletMaxInfo:( %d , %d ) , Canvas( %d, %d ) , DC( %d , %d ) , Canvas( Left:%d , Top:%d , Right:%d , Bottom:%d ) , Canvas W/H( %d , %d )"),
				bVertical, tbData.m_Pen.m_nX, tbData.m_Pen.m_nY,
				m_szTable.cx, m_szTable.cy,							//drawing area dimension
				g_TabletInfo.m_nMaxX, g_TabletInfo.m_nMaxY,			//total tablet dimension
				nW, nH,												//canvas dimension (width and height)
				szDC.cx, szDC.cy,										//DC dimension (width and height)
				rcCanvas.left, rcCanvas.top, rcCanvas.right, rcCanvas.bottom,
				m_CanvasUI->GetWidth(), m_CanvasUI->GetHeight()
			);

			//draw on memory dc for updating it on the main dialog
			Gdiplus::Point ptIn;
			ptIn.X = tbData.m_Pen.m_nX * szDC.cx / m_szTable.cx;
			ptIn.Y = tbData.m_Pen.m_nY * szDC.cy / m_szTable.cy;

			static Gdiplus::Point ptS = { 0,0 };
			if (tbData.m_Pen.m_nP == 0) ptS = ptIn;
			else
			{
				Gdiplus::Graphics Graph(g_pSignDC->DC());
				m_pPen->SetWidth(tbData.m_Pen.m_nP / 1000 + 1);
				Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

				Graph.DrawLine(m_pPen, ptS, ptIn);

				ptS = ptIn;
			}
		}

		if (g_hMainWnd) ::PostMessage(g_hMainWnd, WM_UpdataSign, 0, 0);

		//draw on current window
		Gdiplus::Point ptIn;
		ptIn.X = rcCanvas.left + tbData.m_Pen.m_nX * nW / g_TabletInfo.m_nMaxX;
		ptIn.Y = rcCanvas.top + tbData.m_Pen.m_nY * nH / g_TabletInfo.m_nMaxY;
		if (bVertical == TRUE)
		{
			double dbPercentX = (double)tbData.m_Pen.m_nX / (double)g_TabletInfo.m_nMaxX;
			double dbPercentY = (double)tbData.m_Pen.m_nY / (double)g_TabletInfo.m_nMaxY;
			long lnNewX = (long)(dbPercentX * (double)g_TabletInfo.m_nMaxY);
			long lnNewY = (long)(dbPercentY * (double)g_TabletInfo.m_nMaxX);

			ptIn.X = rcCanvas.left + lnNewX * nW / g_TabletInfo.m_nMaxY;
			ptIn.Y = rcCanvas.top + lnNewY * nH / g_TabletInfo.m_nMaxX;
			CDebugWriter::OutputDBGStringMultipleAutoCloseToAnsi
			(
				_T("New XY( %d , %d ) , NewDimensin( %d , %d ) , NewPoint( %d , %d ) , Percent( %f , %f ) , CurrentTabletPos( %d , %d )"),
				lnNewX, lnNewY, g_TabletInfo.m_nMaxY, g_TabletInfo.m_nMaxX, ptIn.X, ptIn.Y, dbPercentX, dbPercentY,
				tbData.m_Pen.m_nX, tbData.m_Pen.m_nY
			);
		}

		if (tbData.m_Pen.m_nP == 0) ptS = ptIn;
		else
		{
			Gdiplus::Graphics Graph(m_hWnd);
			m_pPen->SetWidth(tbData.m_Pen.m_nP / 1000 + 1);
			Graph.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

			Graph.DrawLine(m_pPen, ptS, ptIn);

			ptS = ptIn;
		}
	}

}

//***********added by Trion on 2025/05/27***********
BOOL ZDlg_Sign::IsVerticalMonitor()
{
	return this->m_bVertical;
}

BOOL ZDlg_Sign::CheckForVerticalMonitor(RECT rcWnd)
{
	int nWidth =  abs(rcWnd.right - rcWnd.left);
	int nHeight = abs(rcWnd.bottom - rcWnd.top);
	if (nHeight > nWidth) return TRUE;
	return FALSE;
}
//***********added by Trion on 2025/05/27***********