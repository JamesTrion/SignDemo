#ifndef  _PROTOCOL_Optics
#define  _PROTOCOL_Optics

#ifdef _OT_CPP_
#undef _OT_CPP_
#endif


#define PS_MAXWAITTIME 2000
#define DELAY_TIME     150
///////////////////错误返回码////////////////////
// #define PS_OK                0x00
// #define PS_COMM_ERR          0x01
// #define PS_NO_FINGER         0x02
// #define PS_GET_IMG_ERR       0x03
// #define PS_FP_TOO_DRY        0x04
// #define PS_FP_TOO_WET        0x05
// #define PS_FP_DISORDER       0x06
// #define PS_LITTLE_FEATURE    0x07
// #define PS_NOT_MATCH         0x08
// #define PS_NOT_SEARCHED      0x09
// #define PS_MERGE_ERR         0x0a
// #define PS_ADDRESS_OVER      0x0b
// #define PS_READ_ERR          0x0c
// #define PS_UP_TEMP_ERR       0x0d
// #define PS_RECV_ERR          0x0e
// #define PS_UP_IMG_ERR        0x0f
// #define PS_DEL_TEMP_ERR      0x10
// #define PS_CLEAR_TEMP_ERR    0x11
// #define PS_SLEEP_ERR         0x12
// #define PS_INVALID_PASSWORD  0x13
// #define PS_RESET_ERR         0x14
// #define PS_INVALID_IMAGE     0x15
// #define PS_HANGOVER_UNREMOVE 0X17


/*******************************************************************************
* Error Code
********************************************************************************/
#define		ERR_SUCCESS								0
#define		ERR_FAIL								1
#define		ERR_VERIFY								0x10
#define		ERR_IDENTIFY							0x11
#define		ERR_TMPL_EMPTY							0x12
#define		ERR_TMPL_NOT_EMPTY						0x13
#define		ERR_ALL_TMPL_EMPTY						0x14
#define		ERR_EMPTY_ID_NOEXIST					0x15
#define		ERR_BROKEN_ID_NOEXIST					0x16
#define		ERR_INVALID_TMPL_DATA					0x17
#define		ERR_DUPLICATION_ID						0x18
#define		ERR_BAD_QUALITY							0x19
#define		ERR_MERGE_FAIL							0x1A
#define 	ERR_NOT_AUTHORIZED						0x1B
#define		ERR_MEMORY								0x1C
#define		ERR_INVALID_TMPL_NO						0x1D
#define		ERR_INVALID_PARAM						0x22
#define		ERR_GEN_COUNT							0x25
#define		ERR_INVALID_BUFFER_ID					0x26
#define		ERR_INVALID_OPERATION_MODE				0x27
#define		ERR_FP_NOT_DETECTED						0x28
#define     ERR_FP_CANCEL							0x41

#define PS_Error_Send		 -1
#define PS_Error_RESend		 -2
#define PS_Error_Verify		 -3
#define PS_Error_Lenvl		 -5
#define PS_Error_Value		 -10

#define DEVICE_USB		0
#define DEVICE_COM		1
#define DEVICE_UDisk	2
#define DEVICE_WD		3
#define DEVICE_070		4


#ifdef _OT_CPP_
extern "C"
{
#endif 

	//new 
	//打开设备
	int WINAPI	KOpenDeviceEx(HANDLE* pHandle, int nDeviceType,int iCom,int iBaud,int nPackageSize,int iDevNum);
	int WINAPI	KCloseDeviceEx(HANDLE hHandle,int nAddr);
	char* WINAPI	KErr2Str(int nErrCode); 
	int WINAPI	KGetImage(HANDLE hHandle,int nAddr);	 
	int WINAPI	KGenChar(HANDLE hHandle,int nAddr,int iBufferID); 
	int WINAPI	KShowFingerData(HWND hWnd,unsigned char *pFingerData); 
	int	WINAPI	KRegModule(HANDLE hHandle,int nAddr);  
	int WINAPI	KMatch(HANDLE hHandle,int nAddr,int* iScore);	 
	int WINAPI	KSearch(HANDLE hHandle,int nAddr,int iBufferID, int iStartPage, int iPageNum, int *iMbAddress,int *iscore); 
	int WINAPI	KHighSpeedSearch(HANDLE hHandle,int nAddr,int iBufferID, int iStartPage, int iPageNum, int *iMbAddress,int *iscore=NULL);	 
	int WINAPI	KDelChar(HANDLE hHandle,int nAddr,int iStartPageID,int nDelPageNum); 
	int WINAPI	KEmpty(HANDLE hHandle,int nAddr); 
	int WINAPI	KReadParTable(HANDLE hHandle,int nAddr,unsigned char* pParTable); 
	int WINAPI	KReadInfPage(HANDLE hHandle,int nAddr, unsigned char* pInf); 
	int WINAPI	KTemplateNum(HANDLE hHandle,int nAddr,int *iMbNum);  
	int WINAPI	KWriteReg(HANDLE hHandle,int nAddr,int iRegAddr,int iRegValue); 
	int WINAPI	KSetBaud(HANDLE hHandle,int nAddr,int nBaudNum); 
	int WINAPI	KSetSecurLevel(HANDLE hHandle,int nAddr,int nLevel); 
	int WINAPI	KSetPacketSize(HANDLE hHandle,int nAddr,int nSize); 
	int WINAPI	KGetRandomData(HANDLE hHandle,int nAddr,unsigned char* pRandom); 
	int WINAPI	KSetChipAddr(HANDLE hHandle,int nAddr,unsigned char* pChipAddr); 
	int WINAPI	KReadIndexTable(HANDLE hHandle,int nAddr,int nPage,unsigned char* UserContent); 
	int WINAPI	KDoUserDefine(HANDLE hHandle,int nAddr,int GPIO,int STATE); 
	int WINAPI	KFingerData2BMPData(unsigned char *pFingerData, unsigned char *pBMPData,int* nBMPDataLen); 
	int WINAPI	KLoadChar(HANDLE hHandle,int nAddr,int iBufferID,int iPageID); 
	int WINAPI	KUpChar(HANDLE hHandle,int nAddr,int iBufferID, unsigned char* pTemplet, int* iTempletLength); 
	int WINAPI	KDownChar(HANDLE hHandle,int nAddr,int iBufferID, unsigned char* pTemplet, int iTempletLength); 
	int WINAPI	KUpImage(HANDLE hHandle,int nAddr,unsigned char* pImageData, int* iImageLength);
	int WINAPI	KDownImage(HANDLE hHandle,int nAddr,unsigned char *pImageData, int iLength); 
	int WINAPI	KImgData2BMP(unsigned char* pImgData,const char* pImageFile);
	int WINAPI	KGetImgDataFromBMP(HANDLE hHandle,const char *pImageFile,unsigned char *pImageData,int *pnImageLen); 
	int WINAPI	KReadInfo(HANDLE hHandle,int nAddr,int nPage,unsigned char* UserContent);
	int WINAPI	KWriteInfo(HANDLE hHandle,int nAddr,int nPage,unsigned char* UserContent); 
	int WINAPI	KSetPwd(HANDLE hHandle,int nAddr,unsigned char* pPassword);
	int WINAPI	KVfyPwd(HANDLE hHandle,int nAddr,unsigned char* pPassword); 
	int WINAPI	KUpChar2File(HANDLE hHandle,int nAddr,int iBufferID, const char* pFileName);
	int WINAPI	KDownCharFromFile(HANDLE hHandle,int nAddr,int iBufferID, const char* pFileName); 
	int WINAPI  KSetCharLen(int nLen);
	int WINAPI	KGetCharLen(int *pnLen); 
	int WINAPI KSetledsound(HANDLE hHandle,int nAddr,unsigned char red, unsigned char green, unsigned char sound, unsigned char moveflag,int timecount);
	int WINAPI KByteTOBase64(unsigned char* pInput, int inputLen,char * pOutput);
	int WINAPI KBase64TOByte( char* pInput, int *inputLen,unsigned char * pOutput);

	//1
	int WINAPI	KInitConnect(HANDLE hHandle,int nAddr);
	//2
	int WINAPI	KGetPara(HANDLE hHandle,int nAddr,int *devid,int *level,int *fprepeat,int *baudrate,int *selflearn);	
	//3
	int WINAPI	KSetPara(HANDLE hHandle,int nAddr,int devid,int level,int fprepeat,int baudrate,int selflearn);
	//4
	int WINAPI	KDEVICEINFO(HANDLE hHandle,int nAddr,char *deviceinfo);
	//5
	int WINAPI	KGETSN(HANDLE hHandle,int nAddr, char *SNstr);
	//6
	int WINAPI	KSETSN(HANDLE hHandle,int nAddr, char *SNstr);
	//7
	//int WINAPI	KSLED(HANDLE hHandle,int nAddr,int setvalue);
	int WINAPI	KSLED(HANDLE hHandle,int nAddr,int  g_status,unsigned char r_led,unsigned char g_led,unsigned char b_led,unsigned char g_time);

	int WINAPI	KIAPMODE(HANDLE hHandle,int nAddr);
	//9
	int WINAPI	KFINGERDETECT(HANDLE hHandle,int nAddr);
	//10
	int WINAPI	KGETIMAGE(HANDLE hHandle,int nAddr);
	//11
	//int WINAPI  KUPIMAGE(HANDLE hHandle,int nAddr,unsigned char *bbmpbuf,int *bmpwidth ,int *bmpheigh) ;
	//int WINAPI	KImgData2BMP(unsigned char* pImgData,const char* pImageFile);
	int WINAPI	KShowBmp(HWND hWnd,unsigned char *pFingerData);

	void SetBmplen(int w,int h);
	void GetBmplen(int *w,int *h);
	//12
	int WINAPI	KGenChar(HANDLE hHandle,int nAddr,int setvalue);
	//13
	int WINAPI	KMergeChar(HANDLE hHandle,int nAddr,int buffer,int mode);
	//14
	int WINAPI	KStoreChar(HANDLE hHandle,int nAddr,int buffer, int storeid);
	//15
	int WINAPI  KGETEMPTYID(HANDLE hHandle,int nAddr,int startid,int endid,int *getid);
	//16
//	int WINAPI	KLoadChar(HANDLE hHandle,int nAddr,int buffer, int storeid);
	//17
	//int WINAPI  KDelChar(HANDLE hHandle,int nAddr,int startid,int endid);
	//18
//	int WINAPI	KUpChar(HANDLE hHandle,int nAddr,int buffer, unsigned char *Template,int *tlen);
	//19
//	int WINAPI	KDownChar(HANDLE hHandle,int nAddr,int buffer, unsigned char *Template,int *tlen);
	//20
	int WINAPI  KGETENROLLCOUNT(HANDLE hHandle,int nAddr,int startid,int endid,int *allcount);
	//21
	//int WINAPI  KMatch(HANDLE hHandle,int nAddr,int buffer1,int buffer2);
	//22
	int WINAPI  KMatchDBID(HANDLE hHandle,int nAddr,int buffer,int fpno);
	//23
	//int WINAPI  KSearch(HANDLE hHandle,int nAddr,int startid,int endid,int buffer,int *fpno);
	//24
	int WINAPI  KGETENROLLIST(HANDLE hHandle,int nAddr,int fpcount,int *allcount,int *pnIDs);
	//25
	int WINAPI	KInterRead(HANDLE hHandle,int nAddr,  unsigned char *Template,int *tlen);
	//26
//	int WINAPI  KInterWrite(HANDLE hHandle,int nAddr, int newcmd, unsigned char *Template,int *tlen ) ;
	int WINAPI  KInterWrite(HANDLE hHandle,int nAddr,  unsigned char *Template,int *tlen) ;

	char* WINAPI	KErr2Str(int nErrCode);
	int WINAPI  KByteToHexstr(unsigned char* pInput, int inputLen,char * pOutput);
	int WINAPI  KHexstrToByte(char *pInput, int *inputLen,unsigned char * pOutput);
	int WINAPI KSetIMGLen( int img_x,int img_y);
	int WINAPI KGetIMGLen(int *img_x,int *img_y);
#ifdef _OT_CPP_
}
#endif

#endif  
