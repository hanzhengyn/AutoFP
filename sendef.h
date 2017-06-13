#ifndef SENDEF_H
#define SENDEF_H
//�����˳������õ���һЩ�꣬�Լ��ṹ�塣

#define MAXNUMOFPROGRAMMER 10	//ģ������
#define MAXNUMOFSOCKET	   4	//ÿ��ģ����socket����
#define SOCKETS_OF_MODULE 4		//ÿ��ģ����socket����
#define FAIL_XN 3	
#define FAIL_YN 5

#define TRAYTOTRAY 0	//
#define TAPETOTRAY 1	//
#define TAPETOTAPE 2	//
#define TRAYTOTAPE 3	//
#define MAXNUM_OF_PROGRAMMER	10		//���ģ������
#define MAXNUM_OF_SOCKET		4		//ÿ��ģ����4����¼��
//����ģʽ
#define TRAY_TO_TRAY	0	//tray�̽�tray�̳�
#define TAPE_TO_TRAY	1	//�����tray�̳�
#define TAPE_TO_TAPE	2	//����������
#define TRAY_TO_TAPE	3	//tray�̽������


//������
#define NOZZLE_0 0
#define NOZZLE_1 1
#define NOZZLE_2 2
#define NOZZLE_3 3

//ģ����
#define MODEL_0 0
#define MODEL_1 1
#define MODEL_2 2
#define MODEL_3 3
#define MODEL_4 4
#define MODEL_5 5
#define MODEL_6 6
#define MODEL_7 7
#define MODEL_8 8
#define MODEL_9 9

//��¼״̬
#define FP_READY	0	//δ��¼
#define FP_PROGM	1	//������¼
#define FP_OK		2	//��¼OK
#define FP_NG		3	//��¼NG

//оƬλ��
#define SOCKET_NUM		4	//��¼��
#define SCRAP_TRAY		5	//������
#define STOCK_TRAY		6	//������
#define TRAY_OR_TAPE	7	//���ϣ�tray�̻���

//��ʱ��
#define TIMER_GOHOME					1	//��ԭ
#define TIMER_RUN_TIME					5	//��¼����ʱ��
#define TIMER_SHOW_POS					7	//��ʾ����ֵ
#define TIMER_REFRESH_SOCKET_STATUS		8	//ˢ����¼״̬
#define TIMER_CHECK_TAPEOUT				9	//����������Ϲ���
#define TIMER_CHECK_TRAY_ARRIVED		10	//���tray�̵����ź�
#define TIMER_CHECK_TAPE_STATUS			11	//�������״̬�ź�

//�������
#define SUCCESS_BURNING				0	//�ɹ�
#define ERROR_STOCKTRAY_POS			1	//������ȡ�ϴ���
#define ERROR_SOCKET_PRESSED		2	//�������״���
#define ERROR_NOZZLE_NOT_BOUND		3	//����δ�ɹ���ԭ����
#define ERROR_GET_CHIP				4	//ȡ�ϴ���
#define ERROR_PUT_CHIP				5	//���ϴ���
#define ERROR_XYMOVE				6	//XY�ƶ�����
#define ERROR_STOCKTRAY_EMPTY		7	//�������ѿ�
#define ERROR_STOCKTRAY_FULL		8	//����������
#define ERROR_XYMOVE_ZCOMEBACK		9	//z���ԭ����
#define ERROR_XYMOVE_NOZZLE_BACK	10	//����δ��ԭ����
#define ERROR_IXLINE				11	//�岹�˶�ʧ��
#define ERROR_CAMERA_CHECK			12	//���ƥ��ʧ��
#define ERROR_TAPE_OUT				13	//��������Ϲ���
#define ERROR_SCRAYTRAY_EMPTY		14	//�������ѿ�
#define PROGRAM_BUSY				15	//��¼æ
#define CHIP_MISS					16	//оƬ����
#define PROGRAMER_ERROR				17	//������쳣����¼ʱ��̫�̣����ܿ���

//��λ�ٶ�
#define POSITION_SPEED	1

//tray��״̬
#define EMPTY_STATUS	0
#define OK_STATUS		1
#define READY_STATUS	2

#define SLEEP_TIME_GET_CHIP 100
#define SLEEP_TIME_PUT_CHIP 70

typedef struct _point
{
    double x;
    double y;
}Point;

typedef struct _TrayInfo
{
    USHORT xn;
    USHORT yn;
}TrayInfo;

typedef struct
{
    double x;
    double y;
    double z;
}Position;
typedef struct
{
    double x;
    double y;
    double phi;
}Position2;
typedef struct _uInfo
{
    short xySpeed;
    short zSpeed;
    int	  nGetTrayHeight;
    int	  nPutTrayHeight;
    int   nGetSocketHeight;
    int   nPutSocketHeight;
    int   xn;
    int   yn;
    int   nTapeIn;
    int   nTapeOut;
    int   nTrayTime;
    int   nSocketTime;
    float fCheckScore;
    float fCheckScoreDown;
    int nNozzleDelay;
    int   nWorkMode;//Tray,0;tape2tray,1
    float fTapeInSpeed;
    float fTapeOutSpeed;
    float fTapeInStep;
    float fTapeOutStep;
    int   nCoverTime;
    int   nElapsed;
    float	nPixelsPM_up;
    float	nPixelsPM_down;
    float	nChipLength;
    float	nChipWidth;
    float	nSocketLength;
    float	nSocketWidth;
    float	nTapeLength;
    float	nTapeWidth;
    float	nOffsetInX;
    float	nOffsetInY;
    float	nOffsetOutX;
    float	nOffsetOutY;
    float	nOffsetSocketX;
    float	nOffsetSocketY;
    CString strCameraNum;
    CString strCameraVersion;
    CString strDownCameraNum;
    CString strDownCameraVersion;

    CString strCheckSum;
    CString strProcName;
}UserInfo;

typedef struct _tagBurningStatus
{
    short  nBusy;
    short  nPutChips;
    short  nGetChips;
    bool  bUsed;
    bool  bEmpty[4];
    short nAdpStatus[4]; //0:δ��¼״̬  1:������¼  2:��¼��ɣ��ɹ�  3; ��¼��ɣ�ʧ��
    bool  bAdpUsed[4];
    int nContinueFailSize[4];
    DWORD dwStartTime;//��¼ģ��Ŀ�ʼ��¼ʱ��
    DWORD dwNoEmptyTime[4];//��¼��¼������оƬ��ʱ��
    bool bStartBurnFlag;//��ʼ��¼��־
    bool bBooking[4];//��Ӧ���ӵ�Ԥ����Ϣ����Զ�����
}BurningStatus;

typedef struct
{
    BYTE MsgType;
    BYTE Index;
    BYTE Action;
    BYTE AdpStatus;
    int  Step;
    int  Max;
    int  Position;
}TPbInfo;

typedef struct
{
    char Serial[16];  // SerialNumber
    int  DrvIndex;    // -1 = not present
    int  DevStatus;  // Reserved
}DevInfo;

//��������
typedef struct
{
    CString strCustomer;
    CString  strWorkNo;
    int nLotNum;
    CString  strICBrand;
    CString  strICModel;
    int nPass;
    float fYield;
    int nProgNG;
    int nSysNG;
    float fFailRate;
    int nUPH;
    int nInput;
    CString  strRunTime;
    CString  strStartTime;
    CString  strEndTime;
    CString  strCheckSum;
    CString  strOperator;
    CString  strDate;
}sData;

#endif