#include "StdAfx.h"
#include "DebugWriter.h"

CDebugWriter * gpDebugWriter=NULL;


CDebugWriter::CDebugWriter(void)
{
	this->Init();
}


CDebugWriter::~CDebugWriter(void)
{
}

CDebugWriter * CDebugWriter::GetDebugWriter()
{
	if (!gpDebugWriter){
		gpDebugWriter=new CDebugWriter();
	}
	return gpDebugWriter;
}

BOOL CDebugWriter::ConfigOutputFlag(BOOL bGet,BOOL bOutput)
{
	if (!bGet) this->m_bOutput=bOutput;
	return this->m_bOutput;
}

void CDebugWriter::Init()
{
	this->ConfigOutputFlag(FALSE);
}

BOOL CDebugWriter::OutputString(TCHAR * pszMsg)
{
	//Purpose: output string to debug window
	BOOL bRet=this->ConfigOutputFlag();
	if (!bRet) return bRet;

	//to do: output string
	OutputDebugString(pszMsg);
	

	return TRUE;
}

void CDebugWriter::Terminate()
{
	if (gpDebugWriter){
		delete gpDebugWriter;
		gpDebugWriter=NULL;
	}
}

BOOL CDebugWriter::OutputDBGStringAutoClose(TCHAR * pszMsg)
{
	if (!CDebugWriter::GetWriteFlag()) return FALSE;   //won't write because write flag is off   
	BOOL bRet=CDebugWriter::GetDebugWriter()->OutputString(pszMsg);
	CDebugWriter::GetDebugWriter()->Terminate();
	return bRet;
}


BOOL CDebugWriter::OutputDBGStringMultipleAutoClose(TCHAR * pszFormat,...)
{
 
	TCHAR buf[4096];
	TCHAR* p = buf;
	va_list args;
    va_start(args, pszFormat);
    _vstprintf(p, pszFormat, args);
    va_end(args);

	const int nSize=4096 + 24;
	TCHAR bufNow[nSize];
	ZeroMemory(bufNow,nSize*sizeof(TCHAR));
	
	int nType=CDebugWriter::GetWriteBitFlag();
	switch(nType)
	{
	case 1:
		//sprintf(bufNow,"%s %s",gstrBit,p);				//original codes.
		sprintf((char *)bufNow, "%s %s", gstrBit, p);		//modified by Trion on 2025/05/26
		break;

	default:
		//sprintf(bufNow,"%s",p);							//original codes.
		sprintf((char *)bufNow, "%s", p);					//modified by Trion on 2025/05/26
		break;
	}
	
	return CDebugWriter::OutputDBGStringAutoClose(bufNow);

	//return CDebugWriter::OutputDBGStringAutoClose(p);

}


void CDebugWriter::EnableDebugWrite(BOOL bEnabled)
{
	gbDebugWriteFlag=bEnabled;
}

BOOL CDebugWriter::GetWriteFlag()
{
	return gbDebugWriteFlag;
}

int CDebugWriter::GetWriteBitFlag()
{
	return gnSystemBitFlag;
}

char* CDebugWriter::ConvertStringToChar(wchar_t* szString)
{
	//Purpose: Convert string from WideChar to multibyte
	char* pszString = NULL;
	//int nSize=(int)(_tcslen(szString) +1)*2; 
	int nSize = (int)(wcslen(szString) + 1) * 2;
	pszString = new char[nSize];
	memset(pszString, 0, nSize);
	pszString[nSize - 1] = '\0';

	WideCharToMultiByte(CP_ACP, 0, szString, -1, pszString, nSize, NULL, NULL);

	return pszString;
}

BOOL CDebugWriter::OutputDBGStringMultipleAutoCloseToAnsi(TCHAR* pszFormat, ...)
{

	TCHAR buf[4096];
	TCHAR* p = buf;
	va_list args;
	va_start(args, pszFormat);
	_vstprintf(p, pszFormat, args);
	va_end(args);

	const int nSize = 4096 + 24;
	TCHAR bufNow[nSize];
	ZeroMemory(bufNow, nSize * sizeof(TCHAR));

	/*
	int nType = CDebugWriter::GetWriteBitFlag();
	switch (nType)
	{
	case 1:
		//sprintf(bufNow,"%s %s",gstrBit,p);				//original codes.
		sprintf((char*)bufNow, "%s %s", gstrBit, p);		//modified by Trion on 2025/05/26
		break;

	default:
		//sprintf(bufNow,"%s",p);							//original codes.
		sprintf((char*)bufNow, "%s", p);					//modified by Trion on 2025/05/26
		break;
	}
	*/
	
	char* szFinal = CDebugWriter::ConvertStringToChar(p);
	OutputDebugStringA(szFinal);
	CDebugWriter::GetDebugWriter()->Terminate();
	return TRUE;
	//return CDebugWriter::OutputDBGStringAutoClose(p);

	//return CDebugWriter::OutputDBGStringAutoClose(bufNow);

	//return CDebugWriter::OutputDBGStringAutoClose(p);

}
