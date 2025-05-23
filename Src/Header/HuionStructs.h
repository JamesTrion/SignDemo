#pragma once
#include <windows.h>
#define RET_SUCCESS              (0)
#define RET_FAILD_NO_LINK        (1)
#define RET_FAILD_FUNCS_OVERFLOW (2)
#define RET_FAILD_FUNCS_NO_FIND  (3)
#define RET_FAILD_FUNCS_EXIST    (4)
#define RET_FAILD_ERROR_PARAM    (5)
#define RET_Disconnected        (-2)

// ansi�ַ��棬�豸��Ϣ
typedef struct _DEVICE_INFO 
{
	char VendorName[56];
	char ProductName[32];
	char DllVersion[12];
	RECT MonitorBounds;	//��Ļ�ʼ����귶Χ
	int	 pressureLevel;	//ѹ�м���512��1024��2048��
	int	 DeviceType;	//0��10����д��	//1�� 3/5��ǩ��������С��ǩ�ְ壩 //2����д��
	unsigned short vendorID;
	unsigned short productID;
	char serial[20];
}
DEVICE_INFO, *PDEVICE_INFO;

// ������Ϣ
typedef struct _TOUCH_INFO 
{
	int x;
	int y;
	int pressure;
	int btnId;		//3��/5�����ġ�ȷ�ϡ�������ǩ������ȡ������ť��	//10����д������д�壺btnId = -1
}
TOUCH_INFO, *PTOUCH_INFO;

typedef void(__stdcall * DEVICE_STATUS_FUNC)(int status);
typedef int(__stdcall * TOUCH_INFO_FUNC)(TOUCH_INFO info);
typedef void(__stdcall * KEY_PRESS_FUNC)(int keyID);


#pragma pack(push)
#pragma pack(1)
struct sTabletInfo
{
	LONG m_nMaxX;//x�����ֵ
	LONG m_nMaxY;//y�����ֵ
	LONG m_nMaxP;//ѹ�����ֵ
	LONG m_nPenKeyCount;//�ʰ����� ��������Ŵ�1��ʼ��
	LONG m_nKeyCount;//�����ϵİ����� ��������Ŵ�1��ʼ��

	WCHAR m_CustomerCode[10];//�ͻ�����
	WCHAR m_RDCode[10];//��������
	WCHAR m_FirmwareDate[12]; //�̼���������
};

enum emTabletDataType
{
	DataType_Pen, //������
	DataType_Key, //��������
	DataType_Knob,//��ť����
	DataType_PenLeaveTablet//���뿪�˰��� ���ݰ���Ч
};
enum emTabletEventType
{
	EventType_TabletConnected, //�豸�Ѿ�����
	EventType_TabletDisconnected //�豸�Ѿ��Ͽ�����
};
union uTabletData
{
	struct sPtOfPen
	{
		long m_nX;//x����
		long m_nY;//y����
		long m_nP;//ѹ��
		long m_nAngleX;//x�����
		long m_nAngleY;//y�����

		long m_nIndexOfPenKey;//�ʰ�������  ���Ͻ���1    ���¼���2
		INT64 m_nTime;
	} m_Pen;
	struct sKeyOfTablet
	{
		long m_nKeyIndex;//��������
		INT64 m_nTime;
	} m_Key;
	struct sKnobOfTablet
	{
		long m_nKnobIndex;//��ť����
		long m_nRotate;//������1   ������2
		INT64 m_nTime;
	} m_Knob;
};


enum emCmdToTablet
{
	Cmd_RunInDriverMode,    //����Ϊ����������ģʽ                               ����ģʽ��nParam=0     ����ģʽ��nParam=1
	Cmd_RemoveCholesterol,  //���������ϵĵ��̴��ۼ������� A5H ��Ч��   
	Cmd_EnableMultimedia,   //���ö�ý��                                         ���ã�nParam=1     �����ã�nParam=0
	Cmd_EnableWindowsWheel, //����windows����                                    ���ã�nParam=1     �����ã�nParam=0
	Cmd_EnableTouchMonitor, //������Ļ����                                       ���ã�nParam=1     �����ã�nParam=0
};
#pragma pack(pop)
typedef void(__stdcall * TABLETDATAPROC)(IN const sTabletInfo *pTablet, emTabletDataType DataType, uTabletData tbData);
typedef void(__stdcall * TABLEEVENTPROC)(emTabletEventType EventType, IN const sTabletInfo *pTablet);
