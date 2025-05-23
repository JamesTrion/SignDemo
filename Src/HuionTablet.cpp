
// HuionTablet.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HuionTablet.h"
#include "HuionTabletDlg.h"
#include "ZMainFrame2.h"
#include <algorithm>  


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString CreateLogFileFullPathName()
{
	SYSTEMTIME SystemTime;
	GetSystemTime(&SystemTime);
	CString strTime;
	strTime.Format(_T("%d-%02d-%02d-%02d-%02d-%02d"),
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour + 8, SystemTime.wMinute, SystemTime.wSecond);

	CString strLogFileName = ZGlobal::GetAppPath() + _T("\\Log\\") + _T("\\") + strTime + _T(".txt");
	return strLogFileName;
}

BEGIN_MESSAGE_MAP(CHuionTabletApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CHuionTabletApp::CHuionTabletApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


CHuionTabletApp theApp;

BOOL CHuionTabletApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (ZNameObj::IsExistName(_T("SignDemoInstance"))) return FALSE;

	ZNameObj NameObj;
	NameObj.CreateName(_T("SignDemoInstance"));

	AfxEnableControlContainer();
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	ZGDIInit InitObj;
	ZCfg::InitCfg();
	ZTextConvert::Init(m_hInstance, MAKEINTRESOURCE(IDR_JSON2));

	CString strLangName = ZCfg::GetCfg_CurLang();
	if (strLangName.IsEmpty()) strLangName = LangName_English;
	ZTextConvert::SetCurLangName(strLangName);

	{//创建日志文件对象
		CString strLogFileName = CreateLogFileFullPathName();
		g_Log.CreateLogFile(strLogFileName, FALSE);

		CString strAppName = ZGlobal::GetAppFullPath();
		CString strVersion = ZGlobal::GetModuleVersion(strAppName, FALSE);
		g_Log.WriteLog(CString(_T("版本号:")) + strVersion);
	}

	SetRegistryKey(_T(" "));

	if (0) ShowMainFrame_MFC();
	else ShowMainFrame_DuiLib();

	
	::CoUninitialize();

	return FALSE;
}


void CHuionTabletApp::ShowMainFrame_MFC()
{
	CHuionTabletDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}
	else if (nResponse == IDCANCEL)
	{
		
	}
}

void CHuionTabletApp::ShowMainFrame_DuiLib()
{
	CControlUI::SetHook_Destruct(ZLangMng::HookFunc_UIDestruct);
	CControlUI::SetHook_SetText(ZLangMng::HookFunc_SetText);

	CPaintManagerUI::SetInstance(m_hInstance);
	CString strInstance = CPaintManagerUI::GetInstancePath();
	CPaintManagerUI::SetResourcePath(strInstance + _T("skin\\Images\\"));

	ZBaseDialog *pMainFrame = new ZMainFrame2;

	pMainFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pMainFrame->CenterWindow();
	pMainFrame->ShowModal();

	CPaintManagerUI::Term();
}
