#pragma once
#include <windows.h>
#define RET_SUCCESS              (0)
#define RET_FAILD_NO_LINK        (1)
#define RET_FAILD_FUNCS_OVERFLOW (2)
#define RET_FAILD_FUNCS_NO_FIND  (3)
#define RET_FAILD_FUNCS_EXIST    (4)
#define RET_FAILD_ERROR_PARAM    (5)
#define RET_Disconnected        (-2)

// ansi字符版，设备信息
typedef struct _DEVICE_INFO 
{
	char VendorName[56];
	char ProductName[32];
	char DllVersion[12];
	RECT MonitorBounds;	//屏幕笔迹坐标范围
	int	 pressureLevel;	//压感级别（512、1024、2048）
	int	 DeviceType;	//0：10寸手写屏	//1： 3/5寸签名屏（带小屏签字板） //2：手写板
	unsigned short vendorID;
	unsigned short productID;
	char serial[20];
}
DEVICE_INFO, *PDEVICE_INFO;

// 数据信息
typedef struct _TOUCH_INFO 
{
	int x;
	int y;
	int pressure;
	int btnId;		//3寸/5寸屏的“确认”、“重签”、“取消”按钮。	//10寸手写屏和手写板：btnId = -1
}
TOUCH_INFO, *PTOUCH_INFO;

typedef void(__stdcall * DEVICE_STATUS_FUNC)(int status);
typedef int(__stdcall * TOUCH_INFO_FUNC)(TOUCH_INFO info);
typedef void(__stdcall * KEY_PRESS_FUNC)(int keyID);


#pragma pack(push)
#pragma pack(1)
struct sTabletInfo
{
	LONG m_nMaxX;//x轴最大值
	LONG m_nMaxY;//y轴最大值
	LONG m_nMaxP;//压感最大值
	LONG m_nPenKeyCount;//笔按键数 （按键编号从1开始）
	LONG m_nKeyCount;//板子上的按键数 （按键编号从1开始）

	WCHAR m_CustomerCode[10];//客户代码
	WCHAR m_RDCode[10];//开发代号
	WCHAR m_FirmwareDate[12]; //固件生成日期
};

enum emTabletDataType
{
	DataType_Pen, //笔数据
	DataType_Key, //按键数据
	DataType_Knob,//旋钮数据
	DataType_PenLeaveTablet//笔离开了板子 数据包无效
};
enum emTabletEventType
{
	EventType_TabletConnected, //设备已经连接
	EventType_TabletDisconnected //设备已经断开连接
};
union uTabletData
{
	struct sPtOfPen
	{
		long m_nX;//x坐标
		long m_nY;//y坐标
		long m_nP;//压感
		long m_nAngleX;//x轴倾角
		long m_nAngleY;//y轴倾角

		long m_nIndexOfPenKey;//笔按键索引  笔上建：1    笔下键：2
		INT64 m_nTime;
	} m_Pen;
	struct sKeyOfTablet
	{
		long m_nKeyIndex;//按键索引
		INT64 m_nTime;
	} m_Key;
	struct sKnobOfTablet
	{
		long m_nKnobIndex;//旋钮索引
		long m_nRotate;//左旋：1   右旋：2
		INT64 m_nTime;
	} m_Knob;
};


enum emCmdToTablet
{
	Cmd_RunInDriverMode,    //设置为有驱或无驱模式                               无驱模式：nParam=0     有驱模式：nParam=1
	Cmd_RemoveCholesterol,  //擦除板子上的胆固醇痕迹（仅对 A5H 有效）   
	Cmd_EnableMultimedia,   //启用多媒体                                         启用：nParam=1     不启用：nParam=0
	Cmd_EnableWindowsWheel, //启用windows轮盘                                    启用：nParam=1     不启用：nParam=0
	Cmd_EnableTouchMonitor, //启用屏幕触摸                                       启用：nParam=1     不启用：nParam=0
};
#pragma pack(pop)
typedef void(__stdcall * TABLETDATAPROC)(IN const sTabletInfo *pTablet, emTabletDataType DataType, uTabletData tbData);
typedef void(__stdcall * TABLEEVENTPROC)(emTabletEventType EventType, IN const sTabletInfo *pTablet);
