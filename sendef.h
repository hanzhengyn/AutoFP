#ifndef SENDEF_H
#define SENDEF_H
//定义了程序中用到的一些宏，以及结构体。

#define MAXNUMOFPROGRAMMER 10	//模组数量
#define MAXNUMOFSOCKET	   4	//每个模组中socket数量
#define SOCKETS_OF_MODULE 4		//每个模组中socket数量
#define FAIL_XN 3	
#define FAIL_YN 5

#define TRAYTOTRAY 0	//
#define TAPETOTRAY 1	//
#define TAPETOTAPE 2	//
#define TRAYTOTAPE 3	//
#define MAXNUM_OF_PROGRAMMER	10		//最大模组数量
#define MAXNUM_OF_SOCKET		4		//每个模组有4个烧录座
//工作模式
#define TRAY_TO_TRAY	0	//tray盘进tray盘出
#define TAPE_TO_TRAY	1	//编带进tray盘出
#define TAPE_TO_TAPE	2	//编带进编带出
#define TRAY_TO_TAPE	3	//tray盘进编带出


//吸嘴编号
#define NOZZLE_0 0
#define NOZZLE_1 1
#define NOZZLE_2 2
#define NOZZLE_3 3

//模组编号
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

//烧录状态
#define FP_READY	0	//未烧录
#define FP_PROGM	1	//正在烧录
#define FP_OK		2	//烧录OK
#define FP_NG		3	//烧录NG

//芯片位置
#define SOCKET_NUM		4	//烧录座
#define SCRAP_TRAY		5	//废料盘
#define STOCK_TRAY		6	//备料盘
#define TRAY_OR_TAPE	7	//进料：tray盘或编带

//定时器
#define TIMER_GOHOME					1	//回原
#define TIMER_RUN_TIME					5	//记录运行时间
#define TIMER_SHOW_POS					7	//显示坐标值
#define TIMER_REFRESH_SOCKET_STATUS		8	//刷新烧录状态
#define TIMER_CHECK_TAPEOUT				9	//检查编带机出料故障
#define TIMER_CHECK_TRAY_ARRIVED		10	//检查tray盘到达信号
#define TIMER_CHECK_TAPE_STATUS			11	//检查编带机状态信号

//错误代号
#define SUCCESS_BURNING				0	//成功
#define ERROR_STOCKTRAY_POS			1	//备料盘取料错误
#define ERROR_SOCKET_PRESSED		2	//下拉气缸错误
#define ERROR_NOZZLE_NOT_BOUND		3	//吸嘴未成功复原错误
#define ERROR_GET_CHIP				4	//取料错误
#define ERROR_PUT_CHIP				5	//放料错误
#define ERROR_XYMOVE				6	//XY移动错误
#define ERROR_STOCKTRAY_EMPTY		7	//备料盘已空
#define ERROR_STOCKTRAY_FULL		8	//备料盘已满
#define ERROR_XYMOVE_ZCOMEBACK		9	//z轴回原错误
#define ERROR_XYMOVE_NOZZLE_BACK	10	//吸嘴未回原错误
#define ERROR_IXLINE				11	//插补运动失败
#define ERROR_CAMERA_CHECK			12	//相机匹配失败
#define ERROR_TAPE_OUT				13	//编带机出料故障
#define ERROR_SCRAYTRAY_EMPTY		14	//废料盘已空
#define PROGRAM_BUSY				15	//烧录忙
#define CHIP_MISS					16	//芯片掉落
#define PROGRAMER_ERROR				17	//编程器异常，烧录时间太短，可能空烧

//定位速度
#define POSITION_SPEED	1

//tray盘状态
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
    short nAdpStatus[4]; //0:未烧录状态  1:正在烧录  2:烧录完成，成功  3; 烧录完成，失败
    bool  bAdpUsed[4];
    int nContinueFailSize[4];
    DWORD dwStartTime;//记录模组的开始烧录时间
    DWORD dwNoEmptyTime[4];//记录烧录座放入芯片的时间
    bool bStartBurnFlag;//开始烧录标志
    bool bBooking[4];//对应座子的预定信息，针对多吸嘴
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

//报表数据
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