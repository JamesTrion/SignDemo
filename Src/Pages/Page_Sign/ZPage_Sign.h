#pragma once
#include "../PageHeaderData.h"

class ZPage_Sign : public CContainerUI, INotifyUI,IMessageFilterUI,CPackHandle
{
public:
	ZPage_Sign(void);
	~ZPage_Sign(void);
public:
	HWND GetWindow() { return m_pManager->GetPaintWindow(); };
	void Paint_Sign(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint);
protected:
	virtual void DoInit();
	virtual void Notify(TNotifyUI& msg);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	virtual void HandlePack(CReqPack &ReqPack);
protected:
	void Build();
	CButtonUI *CreateBtn(CString strName,CString strText);
	COptionUI *CreateOpt(CString strName, CString strText, CString strGroupName = NULL);

	CControlUI *BuildBox_Sign();
	CControlUI *BuildBox_Finger();
	CControlUI *BuildBox_Merge();
	CControlUI *BuildBox_Save();

	
	void OnStartSign();
	void OnResetSign();

	void OnOpenFinger();
	void OnCloseFinger();

	void OnMergeImg();
	void OnCleanImg();

	void OnSelectPath();
	void OnSave();

	void OnTabletEventProc(emTabletEventType EventType, IN const sTabletInfo *pTablet);
	LRESULT OnTabletData(WPARAM wParam, LPARAM lParam);
protected:
	BOOL m_bStartSign = FALSE;
	BOOL m_bMergeDCIsValid = FALSE;
protected:
	Gdiplus::Pen *m_pPen = NULL;
	ZCanvasUI *m_pCanvasSign = NULL;
	ZCanvasUI *m_pCanvasFinger = NULL;
	ZCanvasUI *m_pCanvasMerge = NULL;
	ZMemDC *m_pCanvasMergeDC = NULL;
	
	ZSignDllObj m_SignDllObj;
	
	ZFingerObj m_FingerObj;
	CImage m_FingerImg;
	BOOL m_bRedColor = FALSE;

	BOOL m_bGetFinger = FALSE;
	CWorkThread m_WorkThread;
};

