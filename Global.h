#ifndef GLOBAL_H
#define GLOBAL_H
#include "LogFile.h"
#include "datapos.h"
#include "HalconCpp.h"

using namespace HalconCpp;

//编带进料图像模型的位置数据
extern HTuple gm_hv_Row_In;
extern HTuple gm_hv_Column_In;
extern HTuple gm_hv_Angle_In;

//编带出料图像模型的位置数据
extern HTuple gm_hv_Row_Out;
extern HTuple gm_hv_Column_Out;
extern HTuple gm_hv_Angle_Out;

//下相机，芯片图像模型的位置数据
extern HTuple gm_hv_Row_Chip;
extern HTuple gm_hv_Column_Chip;
extern HTuple gm_hv_Angle_Chip;

//socket图像模型的位置数据
extern HTuple gm_hv_Row_Socket;
extern HTuple gm_hv_Column_Socket;
extern HTuple gm_hv_Angle_Socket;

//芯片模型的名称
extern CString gm_strChipModel;

//烧录座模型名称
extern CString gm_strSocketModel;

//程序启动路径
extern CString gm_strSetupPath;

//
extern BYTE gm_nBoardID;

//位置坐标文件路径
extern CString gm_strPositionFile;

//烧录状态文件路径
extern CString gm_strStatusLog;

//配置文件路径
extern CString gm_strConfigFile;

extern CString gm_strSpecialInfo;

//tray盘规格
extern TrayInfo gm_dataTray;

//备料盘规格
extern TrayInfo gm_dataStock;

//废料盘规格
extern TrayInfo gm_dataNG;

//主界面tray盘显示数据
extern BYTE *gm_pTray;

//模组的取放优先级
extern int gm_nModelPutPreference[MAXNUM_OF_PROGRAMMER];
extern int gm_nModelGetPreference[MAXNUM_OF_PROGRAMMER];

//socket放置优先级
extern int sockPutPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//socket取料优先级
extern int sockGetPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//socket是否使用
extern BOOL gm_bSocketUsed[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//吸嘴使用标志位
extern BOOL gm_bNozzleUsed[4];

//显示每个烧录座的OK数量
extern int gm_nDisplayGoodForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//显示每个烧录座的NG数量
extern int gm_nDisplayFailForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//停止标志位
extern BOOL gm_bStop;

//开始烧录标识
extern BOOL gm_bStart;

//tray盘PLC打开信号
extern BOOL gm_bTrayPLCStart;

//结束烧录标识
extern BOOL gm_bFinish;

//除补料外的进料总数量
extern int gm_nTotalInSize;
extern int gm_nTotalOutSize;

//当前进料数量
extern int gm_nCurrentInSize;

//当前出料数量
extern int gm_nCurrentOutSize;

//待烧录的芯片总数
extern int gm_nSum;

//吸嘴数量
extern int gm_nNozzleNum;

//当前烧录成功数量
extern int gm_nCurrentGoodSize;

//当前烧录失败数量
extern int gm_nCurrentFailSize;

//匹配失败的数量
extern int gm_nTotalCheckFail;

//连续匹配失败数量
extern int gm_nContinueCheckFail;

//烧录失败的数量
extern int gm_nTotalFPFail;

//总的烧录成功数量
extern int gm_nTotalGoodSize;

//总的烧录失败数量
extern int gm_nTotalFailSize;

//用户信息句柄
extern UserInfo gm_uInfo;

//日志文件句柄
extern CLogFile gm_logFile;
extern bool gm_bPushed[MAXNUMOFPROGRAMMER];

//位置信息结构体
extern CDataPos gm_dataPos;

//X拖动条长度
extern int gm_nXLength;

//Y拖动条长度
extern int gm_nYLength;
extern int gm_nHCount;

extern HWND gm_hArray[4];

//烧录状态，0为未烧录，1为正在烧录，2为烧录成功，3为烧录失败
extern BurningStatus gm_sBurningStatus[MAXNUMOFPROGRAMMER];

//是否使用自动tray盘机标识
extern BOOL gm_bAutoTray;

//是否使用补料过程标识
extern BOOL gm_bStock;

//socket测高时是否使用相机检查烧录座中芯片
extern BOOL gm_bSocketHeightCamera;

//是否使用编带进出料检测标识
extern BOOL gm_bTapeCheck;

//进料是否使用下相机标识
extern BOOL gm_bDownCameraIn;

//出料是否使用下相机标识
extern BOOL gm_bDownCameraOut;

//机械臂到达下相机标识符
extern BOOL gm_bArrivedDownCamera;

//蜂鸣器启动标志位
extern BOOL gm_bBeep;

//当前烧录总量
extern int gm_nCurrentTotalSize;

//补料盘总数量
extern int gm_nTotalStock;

//当前补料量
extern int gm_nCurrentStock;

//补料盘中烧录OK数量
extern int gm_nStockGoodSize;

//补料运行标识
extern BOOL gm_bStockRun;

//编带出料标准位置
extern Point gm_nOutOrigePos;

//编带出料偏差
extern Point gm_nTapeOutOffset;

//global Function
void InitGlobalMembers();

//编带机出料正常标志位
extern BOOL gm_bCheckTapeOut;

//tray盘到达标志位
extern BOOL gm_bTrayArrived;

//编带机状态标志
extern int gm_bTapeStatus;

//日志信息
extern sData gm_sdInfo;

//四个吸嘴的错误标识
extern UINT gm_bErrorFig[4];

//初始旋转角度,PI的几分频，如果等于1，则角度为PI，如果等于2，则角度为PI/2,以此类推
//如果为0，表示不旋转
//有正负之分
extern int gm_nRotateAngleIn;
extern int gm_nRotateAngleOut;

//当前吸嘴上有的芯片个数，针对多吸嘴
extern int gm_nCurrentGetChip;

//
extern BOOL gm_bComplete;

//保存进行打开信号
extern BOOL gm_bSaveConfig;

extern BOOL gm_bConvertChip;
extern BOOL gm_bDoorAlarm;
extern BOOL gm_bTapeOutCheck;
extern BOOL gm_bTapeCamera;
#endif