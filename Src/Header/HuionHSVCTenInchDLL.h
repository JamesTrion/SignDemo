// 基于VS 2015开发
// 2021-09-17
// 尹锋

#pragma once
#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport) 
#else
#define MATHFUNCSDLL_API __declspec(dllimport) 
#endif

//#include "lib\\Protocol.h"
#include <iostream>
#include <atlstr.h>

// #pragma comment(lib, "lib\\x86\\ZAZAPIt.lib")
// 32位编译
#ifdef _M_IX86
#pragma comment(lib, "lib\\x86\\ZAZAPIt.lib")
#endif // _M_IX86
// 64位编译
#ifdef _M_X64
// #pragma comment(lib, "lib\\x64\\ZAZAPIt.lib")
#endif // _M_X64

// MATHFUNCSDLL_API double add(double a, double b);

// 调试接口
extern "C" __declspec(dllexport) double add(double a, double b);

// 绘王DLL库 API接口	指示编译器这部分代码按C语言（而不是C++）的方式进行编译
extern "C" __declspec(dllexport) int HWOpenDeviceEx(int nProductType, HANDLE * pHandle, int nDeviceType, int iCom, int iBaud, int nPackageSize/*=2*/, int iDevNum/*=0*/);					// 打开5寸屏高拍仪
extern "C" __declspec(dllexport) int HWCloseDeviceEx(int nDeviceType, HANDLE hHandle);					// 关闭设备

extern "C" __declspec(dllexport) int HWSetIMGLen(int img_x, int img_y);
extern "C" __declspec(dllexport) int HWGetImage(HANDLE hHandle, int nAddr);
extern "C" __declspec(dllexport) int HWUpImage(HANDLE hHandle, int nAddr, unsigned char* pImageData, int* iImageLength);
extern "C" __declspec(dllexport) int HWImgData2BMP(unsigned char* pImgData, const char* pImageFile);
extern "C" __declspec(dllexport) int HWShowFingerData(HWND hWnd, unsigned char* pFingerData);
extern "C" __declspec(dllexport) int HWReadIndexTable(HANDLE hHandle, int nAddr, int nPage, unsigned char* UserContent);
extern "C" __declspec(dllexport) int HWVfyPwd(HANDLE hHandle, int nAddr, unsigned char* pPassword);
extern "C" __declspec(dllexport) CString HWErr2Str(int nErrCode);

// 2021-07-23 测试“打开” ，“获取图像” API
extern "C" __declspec(dllexport) int HWReadInfPage(HANDLE hHandle, int nAddr, unsigned char* pInf);
extern "C" __declspec(dllexport) int HWSetCharLen(int nLen);
extern "C" __declspec(dllexport) int HWReadInfo(HANDLE hHandle, int nAddr, int nPage, unsigned char* UserContent);
extern "C" __declspec(dllexport) int HWSetBaud(HANDLE hHandle, int nAddr, int nBaudNum);
extern "C" __declspec(dllexport) int HWSetSecurLevel(HANDLE hHandle, int nAddr, int nLevel);
extern "C" __declspec(dllexport) int HWSetPacketSize(HANDLE hHandle, int nAddr, int nSize);
extern "C" __declspec(dllexport) int HWSleep(HANDLE hHandle, int nAddr);

// 2021-07-23 其它API接口
extern "C" __declspec(dllexport) int HWGenChar(HANDLE hHandle, int nAddr, int iBufferID);
extern "C" __declspec(dllexport) int HWMatch(HANDLE hHandle, int nAddr, int* iScore);
extern "C" __declspec(dllexport) int HWSearch(HANDLE hHandle, int nAddr, int iBufferID, int iStartPage,
	int iPageNum, int *iMbAddress, int *iscore/*=NULL*/);
extern "C" __declspec(dllexport) int HWHighSpeedSearch(HANDLE hHandle, int nAddr, int iBufferID,
	int iStartPage, int iPageNum, int *iMbAddress, int *iscore/*=NULL*/);
extern "C" __declspec(dllexport) int HWRegModule(HANDLE hHandle, int nAddr);
extern "C" __declspec(dllexport) int HWStoreChar(HANDLE hHandle, int nAddr, int iBufferID, int iPageID);
extern "C" __declspec(dllexport) int HWDelChar(HANDLE hHandle, int nAddr, int iStartPageID, int nDelPageNum);
extern "C" __declspec(dllexport) int HWEmpty(HANDLE hHandle, int nAddr);
extern "C" __declspec(dllexport) int HWReadParTable(HANDLE hHandle, int nAddr, unsigned char* pParTable);
extern "C" __declspec(dllexport) int HWTemplateNum(HANDLE hHandle, int nAddr, int* iMbNum);
extern "C" __declspec(dllexport) int HWGetRandomData(HANDLE hHandle, int nAddr, unsigned char* pRandom);
extern "C" __declspec(dllexport) int HWDoUserDefine(HANDLE hHandle, int nAddr, int GPIO, int STATE);
extern "C" __declspec(dllexport) int HWSetledsound(HANDLE hHandle, int nAddr, unsigned char red, unsigned char green,
	unsigned char sound, unsigned char moveflag, int timecount);
extern "C" __declspec(dllexport) int HWGetCharLen(int *pnLen);
extern "C" __declspec(dllexport) int HWLoadChar(HANDLE hHandle, int nAddr, int iBufferID, int iPageID);
extern "C" __declspec(dllexport) int HWUpChar(HANDLE hHandle, int nAddr, int iBufferID, unsigned char* pTemplet, int* iTempletLength);
extern "C" __declspec(dllexport) int HWDownChar(HANDLE hHandle, int nAddr, int iBufferID, unsigned char* pTemplet, int iTempletLength);
extern "C" __declspec(dllexport) int HWGetIMGLen(int *img_x, int *img_y);
extern "C" __declspec(dllexport) int HWDownImage(HANDLE hHandle, int nAddr, unsigned char *pImageData, int iLength);
extern "C" __declspec(dllexport) int HWGetImgDataFromBMP(HANDLE hHandle, const char *pImageFile, unsigned char *pImageData, int *pnImageLen);
extern "C" __declspec(dllexport) int HWWriteInfo(HANDLE hHandle, int nAddr, int nPage, unsigned char* UserContent);
extern "C" __declspec(dllexport) int HWSetBLS(HANDLE hHandle, int nAddr, int nBaudNum, int nLevel, int nSize);
extern "C" __declspec(dllexport) int HWWriteFile(unsigned char* pTemplate, int  iTempletLength, const char* pFileName);
extern "C" __declspec(dllexport) int HWReadFile(unsigned char* pTemplate, int iTempletLength, const char* pFileName);
extern "C" __declspec(dllexport) int HWByteTOBase64(unsigned char* pInput, int inputLen, char * pOutput);
extern "C" __declspec(dllexport) int HWBase64TOByte(char* pInput, int *inputLen, unsigned char * pOutput);
extern "C" __declspec(dllexport) int HWByteToHexstr(unsigned char* pInput, int inputLen, char * pOutput);
extern "C" __declspec(dllexport) int HWHexstrToByte(char *pInput, int *inputLen, unsigned char * pOutput);
extern "C" __declspec(dllexport) int HWSetPwd(HANDLE hHandle, int nAddr, unsigned char* pPassword);
extern "C" __declspec(dllexport) int HWBurnCode(HANDLE hHandle, int nAddr, int nType, unsigned char *pImageData, int iLength);
extern "C" __declspec(dllexport) int HWClear5xxFLAG(HANDLE hHandle, int nAddr);
extern "C" __declspec(dllexport) int HWUpChar2File(HANDLE hHandle, int nAddr, int iBufferID, const char* pFileName);
extern "C" __declspec(dllexport) int HWDownCharFromFile(HANDLE hHandle, int nAddr, int iBufferID, const char* pFileName);
extern "C" __declspec(dllexport) int HWGetUDiskNum(int* iNums);	//获取 UDISK 设备数
extern "C" __declspec(dllexport) int HWGetTestbmp(HANDLE hHandle, int nAddr);
extern "C" __declspec(dllexport) int HWImgData2TestBMP(unsigned char* pImgData, const char* pImageFile);
extern "C" __declspec(dllexport) int HWShowTestFingerData(HWND hWnd, unsigned char *pFingerData, int bmpx, int bmpy);
extern "C" __declspec(dllexport) int HWSetChipAddr(HANDLE hHandle, int nAddr, unsigned char* pChipAddr);

extern "C" __declspec(dllexport) int HWEncoding(HANDLE hHandle, int nAddr, unsigned char* Randomdata);
extern "C" __declspec(dllexport) int HWSetColor(int wh);

//extern "C" __declspec(dllexport) int HWEncoding(HANDLE hHandle,int nAddr,unsigned char* Randomdata);
extern "C" __declspec(dllexport) int HWDecoding(HANDLE hHandle, int nAddr, unsigned char* Randomdata);
extern "C" __declspec(dllexport) int HWAuthorization(HANDLE hHandle, int nAddr);
