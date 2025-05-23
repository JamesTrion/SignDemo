#pragma once
#include "Pages/PagesHeader.h"

class ZMainFrame2 :public ZBaseDialog
{
public:
	ZMainFrame2(void);
	~ZMainFrame2(void);
protected:
	virtual void InitSize();
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual CContainerUI* BuildItems();
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("ZMainFrame"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	
	virtual LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
protected://内部消息响应函数
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	void OnClose();
	void OnMinimize();
	void OnMaxSize();
	void OnRestoreSize();
	void WndZoomedAdjust();

	void OnLanguageChanged();
	void InitComb_Language(ZComboUI *pComb, IN std::vector<sLang> &vectLang);
	bool LanguageItemEvent(void *pParam);
protected:
	ZHorizontalBox *BuildTopBar();

	CControlUI* BuildBody();

	ZComboUI *m_pCombLanguage = NULL;
protected:
	bool m_bMinMaxFlag;
	bool m_bRestoreFlag;
};