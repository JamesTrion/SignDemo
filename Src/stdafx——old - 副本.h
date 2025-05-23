
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

//#ifndef VC_EXTRALEAN
//#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
//#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS



#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

//#include <iostream>
//#include <fstream>

//typedef CSize MFCSize;
//#include <windows.h>
//#include <objbase.h>
//#include <comutil.h>

#include "Resource.h"
#include "PageMessageHeader.h"




#include "D:\GaomonSVN\DuiLib\DuiLib\UIlib.h"
//#include "DuiLib\DuiLib\UIlib.h"

#include ".\..\..\..\..\DuiLib\DuiLib\UIlib.h"


#ifdef _DEBUG
       #pragma comment(lib, ".\\..\\..\\..\\..\\lib\\DuiLib_d.lib")
#else
       #pragma comment(lib, ".\\..\\..\\..\\..\\lib\\DuiLib.lib")
#endif

#include ".\..\..\..\../lib/StrLib/HuionStrLib.h"
//#include "lib/StrLib/HuionStrLib.h"

#ifdef _DEBUG
//#pragma comment(lib, ".\..\..\..\..\lib/StrLib/HuionStrLib_MDd.lib")
#pragma comment(lib, ".\\..\\..\\..\\..\\lib\\StrLib\\HuionStrLib_MDd.lib")
#else
#pragma comment(lib, ".\\..\\..\\..\\..\\lib\\StrLib\\HuionStrLib_MD.lib")
#endif

#include <vector>
#include <algorithm>
#include <set>
#include ".\..\..\..\../Components/Json/json.h"
#include ".\..\..\..\../Components/StrCoding/ZStrCoding.h"
#include ".\..\..\..\../Components/DirObj/DirObj.h"

//#include "Components/Json/json.h"
//#include "Components/StrCoding/ZStrCoding.h"
//#include "Components/DirObj/DirObj.h"


//#include "Components/Json/ZJsonHelp.h"
//#include "Components/MD5Codec/ZMD5.h"
#include ".\..\..\..\../Components/MemClass/ZMemery.h"
#include ".\..\..\..\../Components/Device/DisplayDevices/ZDisplayDevices.h"


//#include "Components/MemClass/ZMemery.h"
//#include "Components/Device/DisplayDevices/ZDisplayDevices.h"


//#include "Components/xml/Markup.h"
//#include "Components/xml/XmlHelp.h"
#include ".\..\..\..\../Components/WorkThread/WorkThreadHead.h"
#include ".\..\..\..\../Components/GDI/ZGDI.h"
#include ".\..\..\..\../Components/GDI/ZMemDC.h"
#include ".\..\..\..\../Components/GDI/ZMemImage.h"
#include ".\..\..\..\../Components/Timer/ZTimer.h"
#include ".\..\..\..\../Components/UIClass/UIClassHeader.h"
#include ".\..\..\..\../Components/Dialog/DialogHeader.h"

#include ".\..\..\..\../Components/CommnuFunc.h"

#include ".\..\..\..\../Components/LogClass/LogFile.h"
#include ".\..\..\..\../Components/NameObj/ZNameObj.h"


//#include "Components/WorkThread/WorkThreadHead.h"
//#include "Components/GDI/ZGDI.h"
//#include "Components/GDI/ZMemDC.h"
//#include "Components/GDI/ZMemImage.h"
//#include "Components/Timer/ZTimer.h"
//#include "Components/UIClass/UIClassHeader.h"
//#include "Components/Dialog/DialogHeader.h"

//#include "Components/CommnuFunc.h"

//#include "Components/LogClass/LogFile.h"
//#include "Components/NameObj/ZNameObj.h"
//#include "Components/Zip/ZZip.h"

//#include "Components/HotKeyMng/ZHotKeyMng.h"
//#include "Components/KeyBoardHook/ZKeyBoardHook.h"
//#include "Components/Sys/ZSys.h"



#include ".\..\..\..\../Components/TextConvert/ZTextConvert.h"
//#include "Components/TextConvert/ZTextConvert.h"




#include "DataClass/DataClassHeader.h"

extern HWND g_MainWnd;
extern CLogFile g_Log;

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#if 0
#define MS_OFFICE_2003

#pragma warning(disable:4259)
#pragma warning(disable:4786)

#import "./officeItems/MSO.DLL" rename("RGB","_RGB")
#import "./officeItems/VBE6EXT.OLB"
//#import "C:\\Program Files (x86)\\Microsoft Office\\Office12\\MSWORD.OLB" no_namespace
#import "./officeItems/MSWORD.OLB" \
	rename("ExitWindows","_ExitWindows") rename("Fields","wordFields")
#pragma warning(default:4146)

#include "./officeItems/msword.h"
#endif


