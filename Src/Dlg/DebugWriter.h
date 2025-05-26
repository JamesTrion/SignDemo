#ifndef _DebugWriter_H_
#define _DebugWriter_H_

#pragma once

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>



#ifdef WIN32 
    static char gstrBit[]="32 bit" ;
#else 
    static char gstrBit[]=_T("64 bit") ;
#endif 


static int gnSystemBitFlag=1;         //0: don't write bit flag, 1: write bit flag
static BOOL gbDebugWriteFlag=TRUE;

class CDebugWriter
{
private:
	CDebugWriter(void);
	
protected:
	BOOL m_bOutput;

	virtual void Init();
public:
	
	~CDebugWriter(void);

	static CDebugWriter * GetDebugWriter();
	void Terminate();
	
	BOOL OutputString(TCHAR * pszMsg);	
	BOOL ConfigOutputFlag(BOOL bGet=TRUE,BOOL bOutput=TRUE);
	

	
	static void EnableDebugWrite(BOOL bEnabled); 
	static BOOL GetWriteFlag();
	static int GetWriteBitFlag();
	static BOOL OutputDBGStringAutoClose(TCHAR * pszMsg);
	
	static BOOL OutputDBGStringMultipleAutoClose(TCHAR * pszFormat,...);
	static char* ConvertStringToChar(wchar_t* szString);
	static BOOL OutputDBGStringMultipleAutoCloseToAnsi(TCHAR* pszFormat, ...);
};

#endif

