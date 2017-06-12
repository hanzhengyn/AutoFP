/*******************************************************************************
* [ Cmmsdk.h ]
* Header file for API fuctions in CMMSDK library
* - Update Data: 2006/11/15
* - Provider: COMIZOA Co., Ltd.
* - Phone: +82-42-936-6500~6
* - Fax  : +82-42-936-6507
* - URL  : http://www.comizoa.co.kr,  http://www.comizoa.com
********************************************************************************/

#ifndef _CMMSDK_API_H_
#define _CMMSDK_API_H_

#include "CmmsdkDef.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef _CMM_SDK_LOADER_C_
	#define	CMM_EXTERN
#else
	#define CMM_EXTERN extern
#endif

//*******************************************************************************************************
//								API FUNCTIONS                                                           *
//*******************************************************************************************************

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// SetBit() : 지정한 변수의 특정 비트의 값을 변경한다. 매크로이므로 함수명과 괄호가 떨어지면 않됨에 주의!
// v:대상 변수, n:bit번호, s:변경값(state)
#define cmmSetBit(v,n,s)	if(s) (v)|=(1<<(n)); else (v)&=~(1<<(n))
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// GetBit() : 지정한 변수의 특정 비트의 값을 반환한다. 매크로이므로 함수명과 괄호가 떨어지면 않됨에 주의!
// v:대상 변수, n:bit번호
#define cmmGetBit(v,n)		(((v)>>(n))&0x1)

//====================== DLL LOAD/UNLOAD FUNCTIONS ============================================//
CMM_EXTERN BOOL cmmLoadDll(void);
CMM_EXTERN void cmmUnloadDll(void);

//====================== General FUNCTIONS ====================================================//
CMM_EXTERN long (WINAPI *cmmGnDeviceLoad)		(long IsResetDevice, long* NumAxes);
CMM_EXTERN long (WINAPI *cmmGnDeviceUnload)		(void);
CMM_EXTERN long (WINAPI *cmmGnDeviceIsLoaded)	(long* IsLoaded);
CMM_EXTERN long (WINAPI *cmmGnDeviceReset)		(void);
CMM_EXTERN long (WINAPI *cmmGnInitFromFile)		(const char *szCmeFile);
CMM_EXTERN long (WINAPI *cmmGnInitFromFile_MapOnly) (const char *szCmeFile, long MapType);
CMM_EXTERN long (WINAPI *cmmGnSetServoOn)		(long Axis, long Enable);
CMM_EXTERN long (WINAPI *cmmGnGetServoOn)		(long Axis, long* Enable);
CMM_EXTERN long (WINAPI *cmmGnSetAlarmRes)		(long Channel, long IsOn);
CMM_EXTERN long (WINAPI *cmmGnGetAlarmRes)		(long Channel, long* IsOn);
CMM_EXTERN long (WINAPI *cmmGnPulseAlarmRes)	(long Channel, long IsOnPulse, long dwDuration, long IsWaitPulseEnd);
CMM_EXTERN long (WINAPI *cmmGnSetSimulMode)		(long Axis, long IsSimulMode);
CMM_EXTERN long (WINAPI *cmmGnGetSimulMode)		(long Axis, long* IsSimulMode);
CMM_EXTERN long (WINAPI *cmmGnPutInternalSTA)	(long AxesMask);
CMM_EXTERN long (WINAPI *cmmGnSetEmergency)   	(long IsEnable, long IsDecStop);
CMM_EXTERN long (WINAPI *cmmGnGetEmergency)   	(long *IsEnabled);
CMM_EXTERN long (WINAPI *cmmGnBitShift) 		(long Value, long ShiftOption, long* Result);

//====================== Configuration FUNCTIONS ==============================================//
CMM_EXTERN long (WINAPI *cmmCfgSetMioProperty)	(long Axis, long PropId, long PropVal);
CMM_EXTERN long (WINAPI *cmmCfgGetMioProperty) (long Axis, long PropId, long *PropVal);
CMM_EXTERN long (WINAPI *cmmCfgSetFilter)		(long Axis, long IsEnable);
CMM_EXTERN long (WINAPI *cmmCfgGetFilter)		(long Axis, long* IsEnabled);
CMM_EXTERN long (WINAPI *cmmCfgSetFilterAB)		(long Channel, long Target, long IsEnable);
CMM_EXTERN long (WINAPI *cmmCfgGetFilterAB)		(long Channel, long Target, long *IsEnabled);
CMM_EXTERN long (WINAPI *cmmCfgSetInMode)		(long Axis, long InputMode, long IsReverse);
CMM_EXTERN long (WINAPI *cmmCfgGetInMode)		(long Axis, long* InputMode, long* IsReverse);
CMM_EXTERN long (WINAPI *cmmCfgSetOutMode)		(long Axis, long OutputMode);
CMM_EXTERN long (WINAPI *cmmCfgGetOutMode)		(long Axis, long* OutputMode);
CMM_EXTERN long (WINAPI *cmmCfgSetCtrlMode)		(long Axis, long CtrlMode);
CMM_EXTERN long (WINAPI *cmmCfgGetCtrlMode)		(long Axis, long* CtrlMode);
CMM_EXTERN long (WINAPI *cmmCfgSetInOutRatio)	(long Axis, double Ratio);
CMM_EXTERN long (WINAPI *cmmCfgGetInOutRatio)	(long Axis, double* Ratio);
CMM_EXTERN long (WINAPI *cmmCfgSetUnitDist)		(long Axis, double UnitDist);
CMM_EXTERN long (WINAPI *cmmCfgGetUnitDist)		(long Axis, double* UnitDist);
CMM_EXTERN long (WINAPI *cmmCfgSetUnitSpeed)	(long Axis, double UnitSpeed);
CMM_EXTERN long (WINAPI *cmmCfgGetUnitSpeed)	(long Axis, double* UnitSpeed);
CMM_EXTERN long (WINAPI *cmmCfgSetSpeedRange)	(long Axis, double MaxPPS);
CMM_EXTERN long	(WINAPI *cmmCfgGetSpeedRange)	(long Axis, double* MinPPS, double* MaxPPS);
CMM_EXTERN long (WINAPI *cmmCfgSetSpeedPattern)	(long Axis, long SpeedMode, double WorkSpeed, double Accel, double Decel);
CMM_EXTERN long (WINAPI *cmmCfgGetSpeedPattern)	(long Axis, long* SpeedMode, double* WorkSpeed, double* Accel, double* Decel);
CMM_EXTERN long (WINAPI *cmmCfgSetActSpdCheck)	(long IsEnable, long Interval);
CMM_EXTERN long (WINAPI *cmmCfgGetActSpdCheck)	(long *IsEnable, long* Interval);
CMM_EXTERN long (WINAPI *cmmCfgSetSoftLimit)	(long Axis, long IsEnable, double LimitN, double LimitP);
CMM_EXTERN long (WINAPI *cmmCfgGetSoftLimit)	(long Axis, long* IsEnable, double* LimitN, double* LimitP);
CMM_EXTERN long (WINAPI *cmmCfgSetVelCorrRatio)	(long Axis, double fCorrRatio);
CMM_EXTERN long	(WINAPI *cmmCfgGetVelCorrRatio) (long Channel, double* fCorrRatio);
CMM_EXTERN long	(WINAPI *cmmCfgSetSeqMode)		(long SeqMode);
CMM_EXTERN long	(WINAPI *cmmCfgGetSeqMode)		(long* SeqMode);
CMM_EXTERN long	(WINAPI *cmmCfgSetManExtLimit)	(long Channel, long IsSetELP, long IsEnable, long ManState);
CMM_EXTERN long	(WINAPI *cmmCfgGetManExtLimit)	(long Channel, long IsGetELP, long *IsEnable, long *ManState);

//====================== HOME-RETURN FUNCTIONS ================================================//
CMM_EXTERN long (WINAPI *cmmHomeSetConfig)		(long Axis, long HomeMode, long EzCount, double EscDist, double Offset);
CMM_EXTERN long (WINAPI *cmmHomeGetConfig)		(long Axis, long* HomeMode, long* EzCount, double* EscDist, double* Offset);
CMM_EXTERN long (WINAPI *cmmHomeSetSpeedPattern)(long Axis, long SpeedMode, double Vel, double Accel, double Decel, double RevVel);
CMM_EXTERN long (WINAPI *cmmHomeGetSpeedPattern)(long Axis, long* SpeedMode, double* Vel, double* Accel, double* Decel, double* RevVel);
CMM_EXTERN long (WINAPI *cmmHomeMoveStart)		(long Axis, long Direction);
CMM_EXTERN long (WINAPI *cmmHomeMove)			(long Axis, long Direction, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmHomeMoveAll)		(long NumAxes, long* AxisList, long* DirList, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmHomeMoveAllStart)	(long NumAxes, long* AxisList, long* DirList);
CMM_EXTERN long (WINAPI *cmmHomeGetSuccess)		(long Axis, long *IsSuccess);
CMM_EXTERN long (WINAPI *cmmHomeSetSuccess)		(long Axis, long IsSuccess);
CMM_EXTERN long (WINAPI *cmmHomeIsBusy)			(long Axis, long *IsBusy);
CMM_EXTERN long (WINAPI *cmmHomeWaitDone)		(long Axis, long IsBlocking);

//====================== Single Axis Move FUNCTIONS ===========================================//
CMM_EXTERN long (WINAPI *cmmSxSetSpeedRatio)	(long Axis, long SpeedMode, double VelRatio, double AccRatio, double DecRatio);
CMM_EXTERN long (WINAPI *cmmSxGetSpeedRatio)	(long Axis, long* SpeedMode, double* VelRatio, double* AccRatio, double* DecRatio);
CMM_EXTERN long (WINAPI *cmmSxMoveStart)		(long Axis, double Distance);
CMM_EXTERN long (WINAPI *cmmSxMove)				(long Axis, double Distance, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmSxMoveToStart)		(long Axis, double Position);
CMM_EXTERN long (WINAPI *cmmSxMoveTo)			(long Axis, double Position, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmSxVMoveStart)		(long Axis, long Dir);
CMM_EXTERN long (WINAPI *cmmSxStop)				(long Axis, long IsWaitComplete, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmSxStopEmg)			(long Axis);
CMM_EXTERN long (WINAPI *cmmSxIsDone)			(long Axis, long *IsDone);
CMM_EXTERN long (WINAPI *cmmSxWaitDone)			(long Axis, long IsBlocking);
CMM_EXTERN long (WINAPI *cmmSxOptSetIniSpeed)	(long Axis, double IniSpeed);
CMM_EXTERN long (WINAPI *cmmSxOptGetIniSpeed)	(long Axis, double* IniSpeed);
CMM_EXTERN long (WINAPI *cmmSxSetCorrection)	(long Axis, long CorrMode, double CorrAmount, double CorrVel, long CntrMask);
CMM_EXTERN long (WINAPI *cmmSxGetCorrection)	(long Axis, long* CorrMode, double* CorrAmount, double* CorrVel, long* CntrMask);
CMM_EXTERN long (WINAPI *cmmSxOptSetSyncMode)	(long Axis, long Mode, long RefAxis, long Condition);
CMM_EXTERN long (WINAPI *cmmSxOptGetSyncMode)	(long Axis, long* Mode, long* RefAxis, long* Condition);
CMM_EXTERN long (WINAPI *cmmSxOptSetSyncOut)	(long Axis, long Mode, long DoChan_local, long DoLogic);
CMM_EXTERN long (WINAPI *cmmSxOptGetSyncOut)	(long Axis, long *Mode, long *DoChan_local, long *DoLogic);
CMM_EXTERN long (WINAPI *cmmSxOptSetRdpOffset)	(long Axis, double OffsetDist);
CMM_EXTERN long (WINAPI *cmmSxOptGetRdpOffset)	(long Axis, double* OffsetDist);

//====================== Multiple Axes Move FUNCTIONS =========================================//
CMM_EXTERN long	(WINAPI *cmmMxMove)				(long NumAxes, long* AxisList, double* DistList, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmMxVMoveStart)		(long NumAxes, long* AxisList, long* DirList);
CMM_EXTERN long	(WINAPI *cmmMxMoveStart) 		(long NumAxes, long* AxisList, double* DistList);
CMM_EXTERN long	(WINAPI *cmmMxMoveTo) 			(long NumAxes, long* AxisList, double* PosList, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmMxMoveToStart)		(long NumAxes, long* AxisList, double* PosList);
CMM_EXTERN long	(WINAPI *cmmMxStop) 			(long NumAxes, long* AxisList, long IsWaitComplete, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmMxStopEmg) 			(long NumAxes, long* AxisList);
CMM_EXTERN long	(WINAPI *cmmMxIsDone) 			(long NumAxes, long* AxisList, long *IsDone);
CMM_EXTERN long	(WINAPI *cmmMxWaitDone) 		(long NumAxes, long* AxisList, long IsBlocking);

//====================== Interpolation Move FUNCTIONS =========================================//
CMM_EXTERN long (WINAPI *cmmIxMapAxes) 			(long MapIndex, long MapMask1, long MapMask2);
CMM_EXTERN long	(WINAPI *cmmIxSetSpeedPattern)	(long MapIndex, long IsVectorSpeed, long SpeedMode, double Vel, double Acc, double Dec);
CMM_EXTERN long (WINAPI *cmmIxGetSpeedPattern)	(long MapIndex, long* IsVectorSpeed, long* SpeedMode, double* Vel, double* Acc, double* Dec);
CMM_EXTERN long	(WINAPI *cmmIxLine) 			(long MapIndex, double* DistList, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxLineStart) 		(long MapIndex, double* DistList);
CMM_EXTERN long	(WINAPI *cmmIxLineTo) 			(long MapIndex, double* PosList, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxLineToStart)		(long MapIndex, double* PosList);
CMM_EXTERN long	(WINAPI *cmmIxArcA) 			(long MapIndex, double XCentOffset, double YCentOffset, double EndAngle, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxArcAStart) 		(long MapIndex, double XCentOffset, double YCentOffset, double EndAngle);
CMM_EXTERN long	(WINAPI *cmmIxArcATo) 			(long MapIndex, double XCent, double YCent, double EndAngle, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxArcAToStart)		(long MapIndex, double XCent, double YCent, double EndAngle);
CMM_EXTERN long	(WINAPI *cmmIxArcP) 			(long MapIndex, double XCentOffset, double YCentOffset, double XEndPointDist, double YEndPointDist, long Direction, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxArcPStart) 		(long MapIndex, double XCentOffset, double YCentOffset, double XEndPointDist, double YEndPointDist, long Direction);
CMM_EXTERN long	(WINAPI *cmmIxArcPTo) 			(long MapIndex, double XCent, double YCent, double XEndPos, double YEndPos, long Direction, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxArcPToStart)		(long MapIndex, double XCent, double YCent, double XEndPos, double YEndPos, long Direction);
CMM_EXTERN long	(WINAPI *cmmIxIsDone) 			(long MapIndex, long *IsDone);
CMM_EXTERN long	(WINAPI *cmmIxWaitDone) 		(long MapIndex, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxxHelOnceSetSpeed)	(long HelId, long Master, long SpeedMode, double WorkSpeed, double Acc, double Dec);
CMM_EXTERN long	(WINAPI *cmmIxxHelOnce) 		(long HelId, long NumAxes, long* AxisList, double* CoordList, double ArcAngle, double *DistU, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxxHelOnceStart)	(long HelId, long NumAxes, long* AxisList, double* CoordList, double ArcAngle, double *DistU);
CMM_EXTERN long	(WINAPI *cmmIxxSplineBuild)		(double* InArray, long NumInArray, double* OutArray, long NumOutArray);
CMM_EXTERN long	(WINAPI *cmmIxStop) 			(long MapIndex, long IsWaitComplete, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmIxStopEmg) 			(long MapIndex);

//====================== External Switch Move FUNCTIONS =======================================//
CMM_EXTERN long	(WINAPI *cmmExVMoveStart)		(long Axis);
CMM_EXTERN long	(WINAPI *cmmExMoveStart) 		(long Axis, double Distance);
CMM_EXTERN long	(WINAPI *cmmExMoveToStart)		(long Axis, double Position);

//====================== Manual Pulsar FUNCTIONS ==============================================//
CMM_EXTERN long	(WINAPI *cmmPlsrSetInMode)		(long Axis, long InputMode, long IsInverse);
CMM_EXTERN long	(WINAPI *cmmPlsrGetInMode)		(long Axis, long* InputMode, long* IsInverse);
CMM_EXTERN long	(WINAPI *cmmPlsrHomeMoveStart)	(long Axis, long HomeType);
CMM_EXTERN long	(WINAPI *cmmPlsrMoveStart)		(long Axis, double Distance);
CMM_EXTERN long	(WINAPI *cmmPlsrMove) 			(long Axis, double Distance, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmPlsrMoveToStart)	(long Axis, double Position);
CMM_EXTERN long	(WINAPI *cmmPlsrMoveTo) 		(long Axis, double Position, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmPlsrVMoveStart)		(long Axis);
CMM_EXTERN long	(WINAPI *cmmPlsrIsActive)		(long Channel, long *IsActive);

//====================== MASTER/SLAVE FUNCTIONS ===============================================//
CMM_EXTERN long	(WINAPI *cmmMsRegisterSlave) 	(long Axis, double MaxSpeed, long IsInverse);
CMM_EXTERN long	(WINAPI *cmmMsUnregisterSlave)	(long Axis);
CMM_EXTERN long (WINAPI *cmmMsCheckSlaveState)	(long SlaveAxis, long* SlaveState);
CMM_EXTERN long (WINAPI *cmmMsGetMasterAxis) 	(long SlaveAxis, long* MasterAxis);

//====================== Overriding FUNCTIONS =================================================//
CMM_EXTERN long	(WINAPI *cmmOverrideSpeedSet) 	(long Axis);
CMM_EXTERN long	(WINAPI *cmmOverrideSpeedSetAll)(long NumAxes, long* AxisList);
CMM_EXTERN long	(WINAPI *cmmOverrideMove) 		(long Axis, double NewDistance, long* IsIgnored);
CMM_EXTERN long	(WINAPI *cmmOverrideMoveTo) 	(long Axis, double NewPosition, long* IsIgnored);

//====================== LIST-MOTION FUNCTIONS ================================================//
CMM_EXTERN long	(WINAPI *cmmLmMapAxes) 			(long LmIndex, long MapMask1, long MapMask2);
CMM_EXTERN long	(WINAPI *cmmLmBeginList) 		(long LmIndex);
CMM_EXTERN long	(WINAPI *cmmLmEndList) 			(long LmIndex);
CMM_EXTERN long	(WINAPI *cmmLmStartMotion)		(long LmIndex);
CMM_EXTERN long	(WINAPI *cmmLmAbortMotion)		(long LmIndex);
CMM_EXTERN long	(WINAPI *cmmLmIsDone) 			(long LmIndex, long *IsDone);
CMM_EXTERN long	(WINAPI *cmmLmWaitDone)			(long LmIndex, long IsBlocking);
CMM_EXTERN long	(WINAPI *cmmLmCurSequence)		(long LmIndex, long *SeqIndex);
CMM_EXTERN long	(WINAPI *cmmLmImmediacySet)		(long LmIndex);
CMM_EXTERN long (WINAPI *cmmLmDoPutOne) 		(long LmIndex, HANDLE hDoDevice, long Channel, long OutState);
CMM_EXTERN long (WINAPI *cmmLmDoPutMulti)		(long LmIndex, HANDLE hDoDevice, long ChannelGroup, long Mask, long OutStatess);
CMM_EXTERN long (WINAPI *cmmLmDoPulseOne)		(long LmIndex, HANDLE hDoDevice, long Channel, long OutState, long Duration);
CMM_EXTERN long (WINAPI *cmmLmDoPulseMulti)		(long LmIndex, HANDLE hDoDevice, long ChannelGroup, 
				long Mask, long OutStatess, long Duration);

//====================== 상태감시 FUNCTIONS ===================================================//
CMM_EXTERN long	(WINAPI *cmmStSetCount) 		(long Axis, long Target, long Count);
CMM_EXTERN long	(WINAPI *cmmStGetCount) 		(long Axis, long Source, long* Count);
CMM_EXTERN long	(WINAPI *cmmStSetPosition)		(long Axis, long Target, double Position);
CMM_EXTERN long	(WINAPI *cmmStGetPosition)		(long Axis, long Source, double* Position);
CMM_EXTERN long	(WINAPI *cmmStGetSpeed) 		(long Axis, long Source, double* Speed);
CMM_EXTERN long	(WINAPI *cmmStReadMotionState)	(long Axis, long* MotStates);
CMM_EXTERN long	(WINAPI *cmmStReadMioStatuses)	(long Axis, long* MioStates);
CMM_EXTERN long	(WINAPI *cmmStGetMstString)		(long MstCode, char *Buffer, long BufferLen);

//====================== INTERRUPT FUNCTIONS ==================================================//
CMM_EXTERN long	(WINAPI *cmmIntSetMask)			(long Axis, long Mask);
CMM_EXTERN long	(WINAPI *cmmIntGetMask)			(long Axis, long* Mask);
CMM_EXTERN long (WINAPI *cmmIntHandlerSetup)	(long HandlerType, HANDLE Handler, UINT nMessage, LPVOID lParam); 
CMM_EXTERN long (WINAPI *cmmIntHandlerEnable)	(long IsEnable);
CMM_EXTERN long	(WINAPI *cmmIntReadFlag)		(long *IntFlag1, long *IntFlag2);
CMM_EXTERN long	(WINAPI *cmmIntReadErrorStatus)	(long Axis, long *ErrState);
CMM_EXTERN long	(WINAPI *cmmIntReadEventStatus)	(long Axis, long *EventState);

//====================== LATCH FUNCTIONS =======================================================//
CMM_EXTERN long	(WINAPI *cmmLtcIsLatched)		(long Axis, long *IsLatched);
CMM_EXTERN long	(WINAPI *cmmLtcReadLatch)		(long Axis, long Counter, double* LatchedPos);

//====================== Position Compare FUNCTIONS ===========================================//
CMM_EXTERN long	(WINAPI *cmmCmpErrSetConfig) 	(long Axis, double Tolerance, long IsEnable);
CMM_EXTERN long	(WINAPI *cmmCmpErrGetConfig) 	(long Axis, double* Tolerance, long* IsEnabled);
CMM_EXTERN long	(WINAPI *cmmCmpGenSetConfig)	(long Axis, long CmpSrc, long CmpMethod, long CmpAction, double CmpData);
CMM_EXTERN long	(WINAPI *cmmCmpGenGetConfig) 	(long Axis, long* CmpSrc, long* CmpMethod, long* CmpAction, long* CmpData);
CMM_EXTERN long	(WINAPI *cmmCmpTrgSetConfig) 	(long Axis, long CmpSrc, long CmpMethod);
CMM_EXTERN long	(WINAPI *cmmCmpTrgGetConfig) 	(long Axis, long* CmpSrc, long* CmpMethod);
CMM_EXTERN long	(WINAPI *cmmCmpTrgSetOneData) 	(long Axis, double Data);
CMM_EXTERN long	(WINAPI *cmmCmpTrgGetCurData) 	(long Axis, double* Data);
CMM_EXTERN long	(WINAPI *cmmCmpTrgContRegTable)	(long Axis, double* Buffer, long NumData);
CMM_EXTERN long	(WINAPI *cmmCmpTrgContBuildTable) (long Axis, double StartData, double Interval, long NumData);
CMM_EXTERN long	(WINAPI *cmmCmpTrgContStart) 	(long Axis);
CMM_EXTERN long	(WINAPI *cmmCmpTrgContStop)  	(long Axis);
CMM_EXTERN long	(WINAPI *cmmCmpTrgContIsActive)	(long Channel, long *IsActive);
CMM_EXTERN long	(WINAPI *cmmCmpTrgHigh_WriteData)	(long Channel, long CMPH_No, double IniPos, double Interval);
CMM_EXTERN long	(WINAPI *cmmCmpTrgHigh_ReadData)	(long Channel, long *CMPH_No, double *IniPos, double *Interval);
CMM_EXTERN long	(WINAPI *cmmCmpTrgHigh_Start)	(long Channel);
CMM_EXTERN long	(WINAPI *cmmCmpTrgHigh_Stop)	(long Channel);
CMM_EXTERN long	(WINAPI *cmmCmpTrgHigh_Check)	(long Channel, long *IsActive, long *OutCount);

//====================== Digital In/Out FUNCTIONS =============================================//
CMM_EXTERN long (WINAPI *cmmDiSetInputLogic)	(long Channel, long InputLogic);
CMM_EXTERN long (WINAPI *cmmDiGetInputLogic)	(long Channel, long* InputLogic);
CMM_EXTERN long	(WINAPI *cmmDiGetOne) 			(long Channel, long* InputState);
CMM_EXTERN long	(WINAPI *cmmDiGetMulti) 		(long IniChannel, long NumChannels, long* InputStates);
CMM_EXTERN long	(WINAPI *cmmDiGetOneF) 			(long Channel, long CutoffTime_us, long* InputState);
CMM_EXTERN long	(WINAPI *cmmDiGetMultiF) 		(long IniChannel, long NumChannels, long CutoffTime_us, long* InputStates);
CMM_EXTERN long (WINAPI *cmmDoSetOutputLogic)	(long Channel, long OutputLogic);
CMM_EXTERN long (WINAPI *cmmDoGetOutputLogic)	(long Channel, long* OutputLogic);
CMM_EXTERN long	(WINAPI *cmmDoPutOne) 			(long Channel, long OutState);
CMM_EXTERN long	(WINAPI *cmmDoGetOne) 			(long Channel, long* OutState);
CMM_EXTERN long	(WINAPI *cmmDoPulseOne)			(long Channel, long IsOnPulse, long dwDuration, long IsWaitPulseEnd);
CMM_EXTERN long	(WINAPI *cmmDoPutMulti) 		(long IniChannel, long NumChannels, long OutStates);
CMM_EXTERN long	(WINAPI *cmmDoGetMulti) 		(long IniChannel, long NumChannels, long* InputStates);
CMM_EXTERN long	(WINAPI *cmmDoPulseMulti)		(long IniChannel, long NumChannels, long OutStates, long dwDuration, long IsWaitPulseEnd);

//====================== Advanced FUNCTIONS ===================================================//

CMM_EXTERN long (WINAPI *cmmAdvGetNumAvailAxes)	(long *NumAxes);
CMM_EXTERN long (WINAPI *cmmAdvGetNumDefinedAxes)	(long *NumAxes);
CMM_EXTERN long (WINAPI *cmmAdvGetNumAvailDioChan) (long IsInputChannel, long* NumChannels);
CMM_EXTERN long (WINAPI *cmmAdvGetNumDefinedDioChan) (long IsInputChannel, long* NumChannels);
CMM_EXTERN long	(WINAPI *cmmAdvGetMotDeviceId) 	(long Axis, long *DeviceId);
CMM_EXTERN long	(WINAPI *cmmAdvGetMotDevInstance)  (long Axis, long *DevInstance);
CMM_EXTERN long	(WINAPI *cmmAdvGetDioDeviceId) 	(long Axis, long IsInputChannel, long *DeviceId);
CMM_EXTERN long	(WINAPI *cmmAdvGetDioDevInstance)  (long Axis, long IsInputChannel, long *DevInstance);
CMM_EXTERN long	(WINAPI *cmmAdvGetDeviceHandle)	(long DeviceId, long DevInstance, HANDLE *DevHandle);
CMM_EXTERN long	(WINAPI *cmmAdvWriteMainSpace)	(long Axis, long Addr, long Value);
CMM_EXTERN long	(WINAPI *cmmAdvReadMainSpace)	(long Axis, long Addr, long *Value);
CMM_EXTERN long	(WINAPI *cmmAdvWriteRegister)	(long Axis, long RegisterNo, long RegVal);
CMM_EXTERN long	(WINAPI *cmmAdvReadRegister) 	(long Axis, long RegisterNo, long* RegVal);
CMM_EXTERN long (WINAPI *cmmAdvGetMioCfg1Dword) (long Axis, long* Mio1Dword);
CMM_EXTERN long (WINAPI *cmmAdvSetMioCfg1Dword) (long Axis, long Mio1Dword);
CMM_EXTERN long (WINAPI *cmmAdvSetToolboxMode) 	(long EnInterrupt);
CMM_EXTERN long (WINAPI *cmmAdvGetString) 		(long Axis, long StringID, char* szBuffer);
CMM_EXTERN long (WINAPI *cmmAdvErcOut) 			(long Axis, long IsWaitOff);
CMM_EXTERN long (WINAPI *cmmAdvErcReset) 		(long Axis);
CMM_EXTERN long (WINAPI *cmmAdvSetExtOptions)  	(long OptionId, long lParam1, long lParam2, double fParam1, double fParam2);
CMM_EXTERN long (WINAPI *cmmAdvEnumMotDevices)	(TMotDevEnum *EnumBuffer);
CMM_EXTERN long (WINAPI *cmmAdvGetMotDevMap)	(TMotDevMap *MapBuffer);
CMM_EXTERN long (WINAPI *cmmAdvEnumDioDevices)	(TDioDevEnum *EnumBuffer);
CMM_EXTERN long (WINAPI *cmmAdvGetDioDevMap)	(TDioDevMap *MapBuffer);
CMM_EXTERN long (WINAPI *cmmAdvInitFromCmeBuffer) (TCmeData_V2 *pCmeBuffer);
CMM_EXTERN long (WINAPI *cmmAdvInitFromCmeBuffer_MapOnly) (TCmeData_V2 *pCmeBuffer, int nMapType);
CMM_EXTERN long (WINAPI *cmmAdvGetLatestCmeFile) (/*[out] */char *szCmeFile);
CMM_EXTERN long (WINAPI *cmmAdvGetAxisCapability) (long Channel, long CapId, long *CapBuffer);

//====================== DEBUG-LOGGING FUNCTIONS ==============================================//
CMM_EXTERN long	(WINAPI *cmmDlogSetup) 			(long Level, const char* szLogFile);
CMM_EXTERN long	(WINAPI *cmmDlogAddComment)		(const char* szComment);
CMM_EXTERN long	(WINAPI *cmmDlogGetCurLevel)	(long* CurLevel);
CMM_EXTERN long	(WINAPI *cmmDlogGetCurFilePath) (char* szFilePath);

//====================== ERROR HANDLING FUNCTIONS =============================================//
CMM_EXTERN long	(WINAPI *cmmErrGetLastCode) 	(long *ErrorCode);
CMM_EXTERN short (WINAPI *cmmErrParseAxis) 		(long ErrorCode);
CMM_EXTERN short (WINAPI *cmmErrParseReason) 	(long ErrorCode);
CMM_EXTERN long	(WINAPI *cmmErrGetString) 		(long ErrorCode, char *Buffer, long BufferLen);
CMM_EXTERN long	(WINAPI *cmmErrShowLast)		(HWND ParentWnd);
CMM_EXTERN long	(WINAPI *cmmErrSetSkipShowMessage) (long IsSkip);
CMM_EXTERN long	(WINAPI *cmmErrGetSkipShowMessage) (long *IsSkip);
CMM_EXTERN long	(WINAPI *cmmErrSetEnableAutoMessage) (long Enable);
CMM_EXTERN long	(WINAPI *cmmErrGetEnableAutoMessage) (long *Enabled);

//====================== Utility FUNCTIONS ===================================================//
CMM_EXTERN long (WINAPI *cmmUtlProcessWndMsgS)	(HWND WndHandle, long *IsEmpty);
CMM_EXTERN long (WINAPI *cmmUtlProcessWndMsgM)	(HWND WndHandle, long TimeOut, long *IsTimeOuted);
CMM_EXTERN long (WINAPI *cmmUtlReadUserTable)	(HANDLE hDevice, long nAddress, long nSize, UCHAR* pBuffer);
CMM_EXTERN long (WINAPI *cmmUtlWriteUserTable)	(HANDLE hDevice, long nAddress, long nSize, UCHAR* pBuffer);
CMM_EXTERN long (WINAPI *cmmUtlDelayMicroSec)	(long Delay_us);


#undef CMM_EXTERN

#ifdef __cplusplus
}
#endif


#endif