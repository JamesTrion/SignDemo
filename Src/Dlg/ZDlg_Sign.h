#pragma once

class ZDlg_Sign : public ZBaseDialog
{
public:
	static ZDlg_Sign *GetInstance();
	UINT ShowModal(HWND hParentWnd,RECT rcWnd);
protected:
	ZDlg_Sign(void);
	~ZDlg_Sign(void);
	static ZDlg_Sign *m_pInstance;
protected:
	virtual void InitSize();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual LRESULT OnClose(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);

	void OnResetSign();
	void OnOK();

	BOOL BtnHitTest(const uTabletData &tbData);
	void OnTabletData(WPARAM wParam, LPARAM lParam);
protected:
	virtual CContainerUI *BuildItems();

	CButtonUI *CreateBtn(CString strName, CString strText);
public:
	void Paint_Sign(ZCanvasUI *pCanvas, HDC hDC, const RECT& rcPaint);
protected:
	ZCanvasUI *m_CanvasUI;
	RECT m_rcWnd;
	Gdiplus::Pen *m_pPen = NULL;

	CButtonUI *m_pBtnReSign = NULL;
	CButtonUI *m_pBtnOK = NULL;

	SIZE m_szTable;
};

