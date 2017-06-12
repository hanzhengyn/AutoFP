#pragma once

#define RES_LIMIT                       8
#define RES_ALARM                       8
#define RES_HOME                        8
#define RES_GPI                         16
#define RES_ARRIVE                      8
#define RES_MPG                         7

#define RES_ENABLE                      8
#define RES_CLEAR                       8
#define RES_GPO                         16

#define RES_DAC                         12
#define RES_STEP                        8
#define RES_PULSE                       8
#define RES_ENCODER                     11
#define RES_LASER                       2  

#define AXIS_MAX                        8
#define PROFILE_MAX                     8
#define CONTROL_MAX                     8

#define PRF_MAP_MAX                     2
#define ENC_MAP_MAX                     2

#define STEP_DIR                        0
#define STEP_PULSE                      1

#define USER_VAR_LENGTH                 16

#define WATCH_LIST_MAX                  8
#define WATCH_LENGTH_MAX                32
#define WATCH_FIFO_SIZE                 0x1E0       // 480 word

#define WATCH_ADDRESS_TYPE_PRF_POS      (0)
#define WATCH_ADDRESS_TYPE_PRF_VEL      (1)
#define WATCH_ADDRESS_TYPE_ENC_POS      (2)
#define WATCH_ADDRESS_TYPE_ENC_VEL      (3)
#define WATCH_ADDRESS_TYPE_AXIS_PRF_POS (4)
#define WATCH_ADDRESS_TYPE_AXIS_PRF_VEL (5)
#define WATCH_ADDRESS_TYPE_AXIS_ENC_POS (6)
#define WATCH_ADDRESS_TYPE_AXIS_ENC_VEL (7)
#define WATCH_ADDRESS_TYPE_DAC          (8)
#define WATCH_ADDRESS_TYPE_LMT_POS      (9)
#define WATCH_ADDRESS_TYPE_LMT_NEG      (10)
#define WATCH_ADDRESS_TYPE_ALARM        (11)
#define WATCH_ADDRESS_TYPE_HOME         (12)
#define WATCH_ADDRESS_TYPE_GPI          (13)
#define WATCH_ADDRESS_TYPE_ENABLE       (14)
#define WATCH_ADDRESS_TYPE_CLEAR        (15)
#define WATCH_ADDRESS_TYPE_GPO          (16)
#define WATCH_ADDRESS_TYPE_CLOCK        (17)
#define WATCH_ADDRESS_TYPE_FLASH_BUFFER (18)
#define WATCH_ADDRESS_TYPE_CRD_VEL      (19)
#define WATCH_ADDRESS_TYPE_ARRIVE       (20)
#define WATCH_ADDRESS_TYPE_POS_LOOP_ERROR (21)
#define WATCH_ADDRESS_TYPE_POS_LOOP_OUTPUT (22)
#define WATCH_ADDRESS_TYPE_POS_LOOP_INTERGRAL (23)
#define WATCH_ADDRESS_TYPE_VEL_LOOP_ERROR (24)
#define WATCH_ADDRESS_TYPE_VEL_LOOP_INTERGRAL (25)
#define WATCH_ADDRESS_TYPE_SPRING_OFFSET (26)
#define WATCH_ADDRESS_TYPE_BEFORE_SPRING (27)
#define WATCH_ADDRESS_TYPE_AFTER_SPRING (28)
#define WATCH_ADDRESS_TYPE_OBSERVER_LOWPASS_OUTPUT (29)
#define WATCH_ADDRESS_TYPE_OBSERVER_OUTPUT_PRE (30)
#define WATCH_ADDRESS_TYPE_OBSERVER_TORQUE_CMD_PRE (31)
#define WATCH_ADDRESS_TYPE_OBSERVER_VEL_FEEDBACK_INTERGRAL (32)
#define WATCH_ADDRESS_TYPE_OBSERVER_VEL_ERR_FEEDBACK_INTERGRAL (33)
#define WATCH_ADDRESS_TYPE_POS_LOOP_REF_POS (34)
#define WATCH_ADDRESS_TYPE_ADC                   (35)               //16.0

typedef struct DiConfig
{
    short active;
    short reverse;
    short filterTime;
} TDiConfig;

typedef struct CountConfig
{
    short active;
    short reverse;
    short filterType;

    short captureSource;
    short captureHomeSense;
    short captureIndexSense;
} TCountConfig;

typedef struct DoConfig
{
    short active;
    short axis;
    short axisItem;
    short reverse;
} TDoConfig;

typedef struct StepConfig
{
    short active;
    short axis;
    short mode;
    short parameter;
    short reverse;
} TStepConfig;

typedef struct DacConfig
{
    short active;
    short control;
    short reverse;
    short bias;
    short limit;
} TDacConfig;

typedef struct ControlConfig
{
    short active;
    short axis;
    short encoder1;
    short encoder2;
    long  errorLimit;
    short filterType[3];
    short encoderSmooth;
    short controlSmooth;
} TControlConfig;

typedef struct ProfileConfig
{
    short  active;
    double decSmoothStop;
    double decAbruptStop;
} TProfileConfig;

typedef struct AxisConfig
{
    short active;
    short alarmType;
    short alarmIndex;
    short limitPositiveType;
    short limitPositiveIndex;
    short limitNegativeType;
    short limitNegativeIndex;
    short smoothStopType;
    short smoothStopIndex;
    short abruptStopType;
    short abruptStopIndex;
    long  prfMap;
    long  encMap;
    short prfMapAlpha[PRF_MAP_MAX];
    short prfMapBeta[PRF_MAP_MAX];
    short encMapAlpha[ENC_MAP_MAX];
    short encMapBeta[ENC_MAP_MAX];
} TAxisConfig;

typedef struct McConfig
{
    TProfileConfig profile[PROFILE_MAX];
    TAxisConfig    axis[AXIS_MAX];
    TControlConfig control[CONTROL_MAX];
    TDacConfig     dac[RES_DAC];
    TStepConfig    step[RES_STEP];
    TCountConfig   encoder[RES_ENCODER];
    TCountConfig   pulse[RES_PULSE];
    TDoConfig      enable[RES_ENABLE];
    TDoConfig      clear[RES_CLEAR];
    TDoConfig      gpo[RES_GPO];
    TDiConfig      limitPositive[RES_LIMIT];
    TDiConfig      limitNegative[RES_LIMIT];
    TDiConfig      alarm[RES_ALARM];
    TDiConfig      home[RES_HOME];
    TDiConfig      gpi[RES_GPI];
    TDiConfig      arrive[RES_ARRIVE];
	TDiConfig      mpg[RES_MPG];
} TMcConfig;

typedef struct ThreadStatus
{
    short link;
    unsigned long  address;
    short size;
    unsigned long  page;
    short delay;
    short priority;
    short ptr;
    short status;
    short error;
    short result[4];
    short resultType;
    short breakpoint;
    short period;
    short count;
    short function;
} TThreadStatus;

typedef struct Watch
{
    short count;
    short interval;
    unsigned long address[WATCH_LIST_MAX];
    short length[WATCH_LIST_MAX];
    unsigned long threshold;
} TWatch;

typedef struct ControlInfo
{
	double refPos;
	double refPosFilter;
	double refPosFilter2;
	double cntPos;
	double cntPosFilter;

	double error;
	double refVel;
	double refAcc;

	short value;
	short valueFilter;

	short offset;
} TControlInfo;

typedef struct RtcTime
{
    short century;
    short year;
    short month;
    short date;
    short day;
    short hour;
    short minutes;
    short seconds;
}TRtcTime;

GT_API GT_SetDiConfig(short diType,short diIndex,TDiConfig *pDi);
GT_API GT_GetDiConfig(short diType,short diIndex,TDiConfig *pDi);
GT_API GT_SetDoConfig(short doType,short doIndex,TDoConfig *pDo);
GT_API GT_GetDoConfig(short doType,short doIndex,TDoConfig *pDo);
GT_API GT_SetStepConfig(short step,TStepConfig *pStep);
GT_API GT_GetStepConfig(short step,TStepConfig *pStep);
GT_API GT_SetDacConfig(short dac,TDacConfig *pDac);
GT_API GT_GetDacConfig(short dac,TDacConfig *pDac);
GT_API GT_SetCountConfig(short countType,short countIndex,TCountConfig *pCount);
GT_API GT_GetCountConfig(short countType,short countIndex,TCountConfig *pCount);
GT_API GT_SetControlConfig(short control,TControlConfig *pControl);
GT_API GT_GetControlConfig(short control,TControlConfig *pControl);
GT_API GT_SetProfileConfig(short profile,TProfileConfig *pProfile);
GT_API GT_GetProfileConfig(short profile,TProfileConfig *pProfile);
GT_API GT_SetAxisConfig(short axis,TAxisConfig *pAxis);
GT_API GT_GetAxisConfig(short axis,TAxisConfig *pAxis);
GT_API GT_GetConfigTable(short type,short *pCount);
GT_API GT_GetConfigTableAll();

GT_API GT_SetMcConfig(TMcConfig *pMc);
GT_API GT_GetMcConfig(TMcConfig *pMc);

GT_API GT_SetMcConfigToFile(TMcConfig *pMc,char *pFile);
GT_API GT_GetMcConfigFromFile(TMcConfig *pMc,char *pFile);

GT_API GT_SaveConfig(char *pFile);

GT_API GT_Serialize(void);
GT_API GT_ResetOrigin(void);

GT_API GT_Retain(void);

GT_API GT_SetMemory(unsigned long address,short count,short *pData);
GT_API GT_GetMemory(unsigned long address,short count,short *pData);

GT_API GT_FlashToSdram(long *pRetry=NULL);
GT_API GT_SdramToFlash(long *pRetry=NULL);
GT_API GT_SdramToFlash2(long *pRetry=NULL);
GT_API GT_TestSdram();

GT_API GT_GetPlsPos(short pulse,double *pValue,short count,unsigned long *pClock);
GT_API GT_GetPlsVel(short pulse,double *pValue,short count,unsigned long *pClock);

GT_API GT_ClearPlc(void);
GT_API GT_LoadPlc(short id,short returnType);
GT_API GT_LoadPlcCommand(short id,short count,short *pData);
GT_API GT_StepThread(short thread);
GT_API GT_RunThreadToBreakpoint(short thread,short line);
GT_API GT_GetThread(short thread,TThreadStatus *pThread);

GT_API GT_SetWatch(TWatch *pWatch);
GT_API GT_StartWatch(void);
GT_API GT_StartWatchStatic(void);
GT_API GT_StopWatch(void);
GT_API GT_StopWatchStatic(void);
GT_API GT_LoadWatchFifo(short fifo,short *pBuf);
GT_API GT_GetWatchFifo(short *pFifo);
GT_API GT_GetWatchAddress(short type,short index,unsigned long *pAddress,short count=1);
GT_API GT_GetWatchSts(short *pRun,unsigned long *pCount);
GT_API GT_GetWatchData(unsigned long offset,short *pData,short count=1);

GT_API GT_GetControlInfo(short control,TControlInfo *pControlInfo);

GT_API GT_SetRtcTime(TRtcTime *pRtcTime);
GT_API GT_GetRtcTime(TRtcTime *pRtcTime);

GT_API GT_GetFlashPassword(unsigned short *pPassword);
GT_API GT_CheckFlashPassword(void);

GT_API GT_CheckPassword(char *pPassword);
GT_API GT_ModifyPassword(char *pOldPassword,char *pNewPassword);

GT_API GT_LinkCaptureOffset(short encoder,short source);
GT_API GT_SetCaptureOffset(short encoder,long *pOffset,short count=1,long loop=1);
GT_API GT_GetCaptureOffset(short encoder,long *pOffset,short *pCount,long *pLoop);
GT_API GT_GetCaptureOffsetStatus(short encoder,short *pCount,long *pLoop,long *pCapturePos);

GT_API GT_GetHandle(short cardNum,HANDLE *pHandle);

GT_API GT_SetServoTime(long servoTime,long delay,long stepCoef);
GT_API GT_GetServoTime(long *pServoTime,long *pDelay,long *pStepCoef);

GT_API GT_SetFollowRegist(short profile,short segment,short fifo);
GT_API GT_GetFollowRegist(short profile,short *pSegment,short fifo);
GT_API GT_GetFollowRegistCount(short profile,unsigned long *pCount);

GT_API GT_AutoCaptureOn(short encoder);
GT_API GT_AutoCaptureOff(short encoder);

GT_API GT_SetCaptureCount(short encoder,unsigned long count);
GT_API GT_GetCaptureCount(short encoder,unsigned long *pCount,short count=1,unsigned long *pClock=NULL);

GT_API GT_GetEncPosModulo(short encoder,double *pValue,short count=1,unsigned long *pClock=NULL);
GT_API GT_GetEncPosRegist(short encoder,double *pValue,short count=1,unsigned long *pClock=NULL);

GT_API GT_SetFollowVirtualSeg(short profile,short segment,short axis,short fifo);
GT_API GT_GetFollowVirtualSeg(short profile,short *pSegment,short *pAxis,short fifo);

GT_API GT_GetFollowVirtualErr(short profile,double *pVirtualErr);
GT_API GT_ClearFollowVirtualErr(short profile);

GT_API GT_AutoSetWatch(long *pWatchInfo,short interval);
GT_API GT_PrintWatchStaticData(char *pFileName);

GT_API GT_GetInterruptTime(double *pServoRunTime,double *pProfileRunTime);
GT_API GT_GetInterruptTimeMax(double *pServoRunTimeMax,double *pProfileRunTimeMax);

GT_API GT_Reserve(long mask);

