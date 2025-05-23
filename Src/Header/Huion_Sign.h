#pragma once

// Huion_Sign.cpp : 定义 DLL 应用程序的导出函数。
#include "HuionStructs.h"
#define DLL_EXPORT
#define STDCALL __stdcall


DLL_EXPORT void STDCALL GetErrorMessage(int errorCode, char* errorMsg);

DLL_EXPORT int STDCALL GetDeviceStatus();

DLL_EXPORT int STDCALL GetDeviceInfo(DEVICE_INFO* info);

DLL_EXPORT int STDCALL RegisterDeviceStatusCallBack(DEVICE_STATUS_FUNC func);

DLL_EXPORT int STDCALL UnregisterDeviceStatusCallBack(DEVICE_STATUS_FUNC func);

DLL_EXPORT int STDCALL RegisterTouchInfoCallBack(TOUCH_INFO_FUNC func);

DLL_EXPORT int STDCALL UnregisterTouchInfoCallBack(TOUCH_INFO_FUNC func);

DLL_EXPORT int STDCALL RegisterKeyPressCallback(KEY_PRESS_FUNC callback);
DLL_EXPORT int STDCALL UnregisterKeyPressCallback(KEY_PRESS_FUNC callback);



extern "C"
{
	DLL_EXPORT int STDCALL SetCallback_TabletDataProc(TABLETDATAPROC pFunc);
	DLL_EXPORT int STDCALL SetCallback_TabletEventProc(TABLEEVENTPROC pFunc);
	DLL_EXPORT int STDCALL SendCmdToTablet(emCmdToTablet nCmd,long nParam);
	DLL_EXPORT void STDCALL StartScan();

	typedef int(STDCALL *Func_GetErrorMessage)(int errorCode, char* errorMsg);
	typedef int(STDCALL *Func_SetCallback_TabletDataProc)(TABLETDATAPROC pFunc);
	typedef int(STDCALL *Func_SetCallback_TabletEventProc)(TABLEEVENTPROC pFunc);
	typedef int(STDCALL *Func_SendCmdToTablet)(emCmdToTablet nCmd, long nParam);
	typedef int(STDCALL *Func_StartScan)();
}

