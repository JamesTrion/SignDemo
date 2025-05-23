
// HuionTablet.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

class CHuionTabletApp : public CWinApp
{
public:
	CHuionTabletApp();
public:
	virtual BOOL InitInstance();

	void ShowMainFrame_MFC();
	void ShowMainFrame_DuiLib();
protected:
	DECLARE_MESSAGE_MAP()
};

extern CHuionTabletApp theApp;