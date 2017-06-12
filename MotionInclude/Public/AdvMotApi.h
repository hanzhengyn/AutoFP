//
// Copyright (c) Advantech Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Advantech end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#ifdef __cplusplus
extern "C"
{
#endif

#include "AdvMotDev.h"
#include "AdvMotDrv.h"
#include "AdvMotPropID.h"
#include "AdvMotErr.h"


//Add for other Corp.           //  [6/8/2011 dan.yang]
#ifndef tagPT_DEVLIST
#define tagPT_DEVLIST
typedef struct tagPT_DEVLIST
{
    DWORD   dwDeviceNum;
    char	szDeviceName[50];    	
    SHORT   nNumOfSubdevices;	
} DEVLIST, *LPDEVLIST;

#endif

//  [11/26/2014 dan]
#ifndef _DEV_IO_MAP_INFO
#define _DEV_IO_MAP_INFO
typedef struct _DEV_IO_MAP_INFO
{
	char Name[50];  
	ULONG Index;
	ULONG Offset;
	ULONG ByteLength;
	ULONG SlotID;

	ULONG PortChanID;
	ULONG ModuleID;
	char ModuleName[16];
	char Discription[100];
} DEV_IO_MAP_INFO, *PDEV_IO_MAP_INFO;
#endif

#ifndef  ADVCMNAPI
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define ADVCMNAPI      __stdcall
#endif
#endif

/*
+-----------------------------------------------------------------------------+
| ************ Advantech Motion Master Device Type ID************************ |
+-----------------------------------------------------------------------------+
*/
//U32	ADVCMNAPI Acm_GetAvailableDevNum(U32 *DeviceNum); //Add for other corp. [dan.yang 2011.06.08]
U32 ADVCMNAPI Acm_GetAvailableDevs(DEVLIST *DeviceList, U32 MaxEntries, PU32 OutEntries); //Add for other corp. [dan.yang 2011.06.08]
void ADVCMNAPI GetAvailalbe_AMONet(DWORD		dwMasDevNum, //Add for other corp. [dan.yang 2011.07.13]
						 DEVLIST    *DeviceList,
						 ULONG      *devIndex,
						 ULONG		*OutEntries);
BOOL ADVCMNAPI Acm_GetErrorMessage(U32 ErrorCode, PI8 lpszError,  U32 nMaxError);//  [9/5/2011 dan.yang]

U32 ADVCMNAPI Acm_GpMoveCircularRel_Angle(HAND  GroupHandle,  //  [8/15/2012 dan.yang]
								PF64 CenterArray, 
								U16 Degree,
								PU32 ArrayElements,
								I16  Direction);

U32 ADVCMNAPI Acm_GpMoveCircularAbs_Angle(HAND  GroupHandle,  //  [8/15/2012 dan.yang]
								PF64 CenterArray, 
								U16 Degree,
                                PU32 ArrayElements,
								I16  Direction);
//Device operation
 U32 ADVCMNAPI Acm_DevOpen(U32 DeviceNumber, PHAND DeviceHandle);
 U32 ADVCMNAPI Acm_DevReOpen(HAND DeviceHandle); //  [11/25/2014 dan]
 U32 ADVCMNAPI Acm_DevClose(PHAND DeviceHandle);
 U32 ADVCMNAPI Acm_DevReadEEPROM(HAND DeviceHandle, U16 EEPROMAddr, PU16 readValue);
 U32 ADVCMNAPI Acm_DevWriteEEPROM(HAND DeviceHandle, U16 EEPROMAddr, U16 writeValue);

 U32 ADVCMNAPI Acm_DevReadEEPROM_Ex(HAND DeviceHandle, U16 PrivateID, PU32 PassWordArray, U32 PassArrayCnt, PU32 ReadArray, U32 BufferLength);//  [6/15/2012 dan.yang]
 U32 ADVCMNAPI Acm_DevWriteEEPROM_Ex(HAND DeviceHandle, U16 PrivateID, PU32 PassWordArray, U32 PassArrayCnt, PU32 WriteArray, U32 BufferLength); //  [6/15/2012 dan.yang]

 U32 ADVCMNAPI Acm_GetProperty(HAND Handle, U32 ProperyID, PVOID Buffer, PU32 BufferLength);
 U32 ADVCMNAPI Acm_SetProperty(HAND Handle, U32 ProperyID, PVOID Buffer, U32 BufferLength);
 U32 ADVCMNAPI Acm_EnableMotionEvent(HAND DeviceHandle,
						  PU32 AxEnableEvtArray,
						  PU32 GpEnableEvtArray,
						  U32 AxArrayElements,
						  U32 GpArrayElements);
 U32 ADVCMNAPI Acm_CheckMotionEvent(HAND DeviceHandle,
						 PU32 AxEvtStatusArray,
						 PU32 GpEvtStatusArray,
						 U32 AxArrayElements,
						 U32 GpArrayElements,
						 U32 Millisecond);
 U32 ADVCMNAPI Acm_CancelCheckEvent (HAND	ObjectHandle);
 U32 ADVCMNAPI Acm_DevLoadConfig(HAND DeviceHandle, PI8 ConfigPath);
 U32 ADVCMNAPI Acm_DevFwDownload(HAND DeviceHandle, U32 Data, U32 DataID); //internal function don't release.
 U32 ADVCMNAPI Acm_DevDownloadCAMTable (HAND DeviceHandle, 
							U32 CamTableID, 
							PF64 pMasterArray, 
							PF64 pSlaveArray, 
							PF64 pPointRangeArray, 
							PF64 pPointSlopeArray,    
							U32 ArrayElements);
 U32 ADVCMNAPI Acm_DevLoadCAMTableFile(HAND DeviceHandle, PI8 FilePath, U32 CamTableID, PU32 Range, PU32 PointsCount); // [dan.yang 2011.08.22]
 U32 ADVCMNAPI Acm_DevConfigCAMTable(HAND DeviceHandle,  //Add for pci1265 and pci1245 [dan.yang 2011.06.23]
						  U32 CamTableID,
						  U32 Periodic,
						  U32 MasterAbsolute,
						  U32 SlaveAbsolute);
 U32 ADVCMNAPI Acm_DevReadMailBox(HAND Handle, U16 par_id, U32 data_index, U32 data_count, PU32 DataBuffer); //[dan.yang 2012.12.28]
 U32 ADVCMNAPI Acm_DevWriteMailBox(HAND Handle, U16 par_id, U32 data_index, U32 data_count, PU32 DataBuffer); //[dan.yang 2012.12.28]
 U32 ADVCMNAPI Acm_LoadENI(HAND DeviceHandle, PI8 FilePath); //  [11/12/2014 dan]
 U32 ADVCMNAPI Acm_DevGetComStatus(HAND DeviceHandle, U16 RingNo, PU16 pStatus); //  [11/20/2014 dan]
 U32 ADVCMNAPI Acm_DevGetSlaveStates(HAND DeviceHandle, U16 RingNo, U16 SlaveIP, PU16 pStatus);
 U32 ADVCMNAPI Acm_DevGetErrorTable(HAND DeviceHandle, U16 RingNo, PU32 ErrorTableArray, PU32 ArrayElements);
 U32 ADVCMNAPI Acm_DevEnableEvent(HAND DeviceHandle, U32 MasEnableEvt); //  [11/20/2014 dan]
 U32 ADVCMNAPI Acm_DevCheckEvent(HAND DeviceHandle, PU32 MasCheckEvt, U32 Millisecond); //  [11/20/2014 dan]
 U32 ADVCMNAPI Acm_DevGetSlaveInfo(HAND DeviceHandle, U16 RingNo, U16 SlaveIP, PVOID pInfo);
 U32 ADVCMNAPI Acm_DevGetMasInfo(HAND DeviceHandle, PVOID pMasInfo, PU16 SlaveIPArray, PU32 SlvCnt);
 U32 ADVCMNAPI Acm_DevGetSlaveDataCnt(HAND DeviceHandle, U16 RingNo, U16 SlaveIP, U8 DataType, PU32 DataCnt);
 U32 ADVCMNAPI Acm_WriteRingBuffer(HAND Handle, U32 cmd_id, U32 data_index, U32 data_cnt, PU32 dataBuffer);
 U32 ADVCMNAPI Acm_DevGetModuleInfo(HAND DeviceHandle, U16 RingNo, U16 SlaveIP, PU32 ModIDArray, PU32 ModCnt);
 //  [2/28/2014 dan.yang]
 U32 ADVCMNAPI Acm_GetU32Property(HAND Handle, U32 ProperyID, PU32 Value);
 U32 ADVCMNAPI Acm_GetI32Property(HAND Handle, U32 ProperyID, PI32 Value);
 U32 ADVCMNAPI Acm_GetF64Property(HAND Handle, U32 ProperyID, PF64 Value);
 U32 ADVCMNAPI Acm_GetStringProperty (HAND Handle, U32 ProperyID, PU8 Value);
 U32 ADVCMNAPI Acm_SetU32Property (HAND Handle, U32 ProperyID, U32 Value);
 U32 ADVCMNAPI Acm_SetI32Property (HAND Handle, U32 ProperyID, I32 Value);
 U32 ADVCMNAPI Acm_SetF64Property (HAND Handle, U32 ProperyID, F64 Value);
 U32 ADVCMNAPI Acm_SetStringProperty (HAND Handle, U32 ProperyID, PU8 Value);

 //  [11/20/2014 dan]
 U32 ADVCMNAPI Acm_GetChannelProperty(HAND Handle, U32 ChannelID, U32 ProperyID,  PF64 Value);
 U32 ADVCMNAPI Acm_SetChannelProperty(HAND Handle, U32 ChannelID, U32 ProperyID,  F64 Value);
 U32 ADVCMNAPI Acm_GetMultiChannelProperty(HAND Handle, U32 ProperyID, U32 StartChID, U32 ChCount, PF64 ValueArray);
 U32 ADVCMNAPI Acm_SetMultiChannelProperty(HAND Handle, U32 ProperyID, U32 StartChID, U32 ChCount, PF64 ValueArray);
 //Master device operation
 U32 ADVCMNAPI Acm_MasStartRing(HAND DeviceHandle, U16 RingNo);
 U32 ADVCMNAPI Acm_MasStopRing(HAND DeviceHandle, U16 RingNo);
 U32 ADVCMNAPI Acm_MasGetComStatus(HAND DeviceHandle, U16 RingNo, PU16 pStatus);
 U32 ADVCMNAPI Acm_MasGetComCyclicTime(HAND DeviceHandle, U16 RingNo, PF64 pTime);
 U32 ADVCMNAPI Acm_MasGetDataCyclicTime(HAND DeviceHandle, U16 RingNo, PF64 DataCyclicTime);
 U32 ADVCMNAPI Acm_MasGetActiveTable(HAND DeviceHandle, U16 RingNo, PU32 ActiveTableArray, PU32 ArrayElements);
 U32 ADVCMNAPI Acm_MasGetErrorTable(HAND DeviceHandle, U16 RingNo, PU32 ErrorTableArray, PU32 ArrayElements);
 U32 ADVCMNAPI Acm_MasGetSlaveInfo(HAND DeviceHandle, U16 RingNo, U16 SlaveIP, PU32 pInfo);
 U32 ADVCMNAPI Acm_MasLogComStatus(HAND DeviceHandle, U16 RingNo);
 U32 ADVCMNAPI Acm_MasTrigOut(HAND DeviceHandle, U16 RingNo);//Engineer function, don't release
 U32 ADVCMNAPI Acm_MasGetRingStatus(HAND DeviceHandle, U16 RingNo, PU16 pStatus);//internal function don't release.

//Axis operation
U32 ADVCMNAPI Acm_AxOpen(HAND DeviceHandle, U16 PhyAxis, PHAND AxisHandle);
U32 ADVCMNAPI Acm_AxClose(PHAND AxisHandle);
U32 ADVCMNAPI Acm_AxSetSvOn(HAND AxisHandle, U32 OnOff);
U32 ADVCMNAPI Acm_AxSetErcOn(HAND AxisHandle, U32 OnOff);
U32 ADVCMNAPI Acm_AxResetAlm(HAND AxisHandle, U32 OnOff);
U32 ADVCMNAPI Acm_AxMoveRel(HAND AxisHandle, F64 Distance);
U32 ADVCMNAPI Acm_AxMoveAbs(HAND AxisHandle, F64 Position);
U32 ADVCMNAPI Acm_AxMoveRel_EC(HAND AxisHandle, F64 Position);//zhaocui add 20131203 for hongjin
U32 ADVCMNAPI Acm_AxMoveVel(HAND AxisHandle, U16 Direction);
U32 ADVCMNAPI Acm_AxStopDec(HAND AxisHandle);
U32 ADVCMNAPI Acm_AxStopEmg(HAND AxisHandle);
U32 ADVCMNAPI Acm_AxMoveImpose(HAND AxisHandle, F64 Position, F64 NewVel); //Add for pci1265 and pci1245 [dan.yang 2011.04.20]
U32 ADVCMNAPI Acm_AxHomeEx(HAND AxisHandle, U32 DirMode);
U32 ADVCMNAPI Acm_AxHome(HAND AxisHandle, U32 HomeMode, U32 DirMode);
// U32 Acm_AxPauseMotion(U32 AxisHandle);
// U32 Acm_AxResumeMotion(U32 AxisHandle);
U32 ADVCMNAPI Acm_AxChangeVel(HAND AxisHandle, F64 NewVel);
U32 ADVCMNAPI Acm_AxChangePos(HAND AxisHandle, F64 NewPos);
U32 ADVCMNAPI Acm_AxChangeVelByRate(HAND AxisHandle, U32 Rate); //  [11/21/2012 dan.yang]
U32 ADVCMNAPI Acm_AxResetError(HAND AxisHandle);
U32 ADVCMNAPI Acm_AxGetState(HAND AxisHandle, PU16 State);
U32 ADVCMNAPI Acm_AxGetMotionIO(HAND AxisHandle, PU32 Status);
U32 ADVCMNAPI Acm_AxGetMotionStatus(HAND AxisHandle, PU32 Status);
U32 ADVCMNAPI Acm_GetLastError(HAND Handle);
U32 ADVCMNAPI Acm_AxGetCmdPosition(HAND AxisHandle, PF64 Position);
U32 ADVCMNAPI Acm_AxGetMachPosition(HAND AxisHandle, PF64 Position);
U32 ADVCMNAPI Acm_AxSetCmdPosition(HAND AxisHandle, F64 Position);
U32 ADVCMNAPI Acm_AxGetActualPosition(HAND AxisHandle, PF64 Position);
U32 ADVCMNAPI Acm_AxSetActualPosition(HAND AxisHandle, F64 Position);
U32 ADVCMNAPI Acm_AxGetCmdVelocity(HAND AxisHandle, PF64 Velocity);
U32 ADVCMNAPI Acm_AxGetLagCounter(HAND AxisHandle, PF64 Position);
U32 ADVCMNAPI Acm_AxGetLatchData(HAND AxisHandle, U32 PositionNo, PF64 Position); //  [5/30/2011 dan.yang]
U32 ADVCMNAPI Acm_AxStartSoftLatch(U32 AxisHandle);
U32 ADVCMNAPI Acm_AxSetExtDrive(HAND AxisHandle, U16 ExtDrvMode);
U32 ADVCMNAPI Acm_AxDoSetBit(HAND AxisHandle, U16	DoChannel, U8 BitData);
U32 ADVCMNAPI Acm_AxDoGetBit(HAND AxisHandle, U16	DoChannel, PU8 BitData);
U32 ADVCMNAPI Acm_AxDiGetBit(HAND AxisHandle, U16	DiChannel, PU8 BitData);
U32 ADVCMNAPI Acm_AxSimStartSuspendVel(HAND AxisHandle, U16 DriDir);
U32 ADVCMNAPI Acm_AxSimStartSuspendRel(HAND AxisHandle,F64 Distance);
U32 ADVCMNAPI Acm_AxSimStartSuspendAbs(HAND AxisHandle,F64 EndPoint);
U32 ADVCMNAPI Acm_AxSimStart(HAND AxisHandle);
U32 ADVCMNAPI Acm_AxSimStop(HAND AxisHandle);
U32 ADVCMNAPI Acm_AxResetLatch(HAND AxisHandle);//Add for pci1245 and pci1265 [dan.yang 2011.05.05]
U32 ADVCMNAPI Acm_AxGetLatchFlag(HAND AxisHandle, PU8 LatchFlag);//Add for pci1245 and pci1265 [dan.yang 2011.05.05]
U32 ADVCMNAPI Acm_AxTriggerLatch(HAND AxisHandle); //Add for pci1245 and pci1265 [dan.yang 2011.05.10]
U32 ADVCMNAPI Acm_AxCamInAx (HAND AxisHandle,   //Add for pci1245 and pci1265 [dan.yang 2011.05.10]
				   HAND MasAxisHandle, 
				   F64 MasterOffset,
				   F64 SlaveOffset, 
				   F64 MasterScaling, 
				   F64 SlaveScaling, 
				   U32 CamTableID, 
				   U32 RefSrc);
U32 ADVCMNAPI Acm_AxGearInAx(HAND AxisHandle,  //Add for pci1245 and pci1265 [dan.yang 2011.06.23]
				   HAND MasAxisHandle,
				   I32 Numerator, 
				   I32 Denominator, 
				   U32 RefSrc, 
				   U32 Absolute);
U32	ADVCMNAPI Acm_AxTangentInGp (HAND 	AxisHandle, //Add for pci1245 and pci1265 [dan.yang 2011.06.24]
					   HAND 	MasGroupHandle,
					   PI16		StartVectorArray,  //must three dimension
					   U8		Working_plane,		// 0:  XY plane (default),  1: YZ plane,  2: XZ plane
					   I16		Direction); //0: same with master group. 1:opposite
U32	ADVCMNAPI Acm_AxGantryInAx (HAND 	AxisHandle, //Add for pci1245 and pci1265 [dan.yang 2011.06.24]
					  HAND 	MasAxisHandle,
					  I16		RefMasterSrc,
					  I16		direction); //0: same with master axis. 1:opposite
U32 ADVCMNAPI Acm_AxPhaseAx(HAND AxisHandle, F64 Acc, F64 Dec, F64 PhaseSpeed, F64 PhaseDist);
//  [12/14/2012 dan.yang]
U32 ADVCMNAPI Acm_AxChangeVelEx(HAND AxisHandle,
					  F64  NewVel,
					  F64  NewAcc,
					  F64  NewDec);
U32 ADVCMNAPI Acm_AxChangeVelExByRate(HAND AxisHandle,
							U32  Rate,               //The percentage of velocity to be changed.
							F64  NewAcc,             // New acceleration.
							F64  NewDec);            // New deceleration.
//  [12/14/2012 dan.yang]
U32 ADVCMNAPI Acm_AxStopDecEx(HAND AxisHandle,
					 F64 NewDec);

//  [10/30/2013 dan.yang]: Add compare channel
U32 ADVCMNAPI Acm_AxSetChannelCmpSetting(HAND AxisHandle, U16 ChannelID, U32 CmpSrc, U32 CmpMethod, U32 CmpPulseMode, U32 CmpPulseWidth);
U32 ADVCMNAPI Acm_AxGetChannelCmpSetting(HAND AxisHandle, U16 ChannelID, PU32 CmpSrc, PU32 CmpMethod, PU32 CmpPulseMode, PU32 CmpPulseWidth);
U32 ADVCMNAPI Acm_AxResetChannelCmp (HAND AxisHandle, U16 ChannelID);
U32 ADVCMNAPI Acm_AxAddChannelCmpDatas(HAND AxisHandle, U16 ChannelID, PF64 TableArray, U32 ArrayCount);
U32 ADVCMNAPI Acm_AxGetChannelCmpData(HAND AxisHandle, U16 ChannelID, PF64 CmpData);
U32 ADVCMNAPI Acm_AxLoadChannelNextData(HAND AxisHandle, U16 ChannelID);
U32 ADVCMNAPI Acm_AxGetCmpbufferRemainCount(HAND AxisHandle, U16 ChannelID,PU32 DataCount);

//Group operation
U32 ADVCMNAPI Acm_GpAddAxis(PHAND GpHandle,HAND AxHandle);
U32 ADVCMNAPI Acm_GpRemAxis(HAND GroupHandle, HAND AxisHandle);
U32 ADVCMNAPI Acm_GpClose(PHAND GroupHandle);
U32 ADVCMNAPI Acm_GpGetState(HAND GroupHandle, PU16 State);
U32 ADVCMNAPI Acm_GpResetError(HAND GroupHandle);

U32 ADVCMNAPI Acm_GpMoveLinearRel(HAND GroupHandle, PF64 DistanceArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_GpMoveLinearAbs(HAND GroupHandle, PF64 PositionArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_GpMoveDirectRel(HAND GroupHandle, PF64 DistanceArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_GpMoveDirectAbs(HAND GroupHandle, PF64 PositionArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_GpMoveCircularRel(HAND  GroupHandle, PF64 CenterArray, PF64 EndArray,
                          PU32 ArrayElements, I16 Direction);
U32 ADVCMNAPI Acm_GpMoveCircularAbs(HAND  GroupHandle, PF64 CenterArray, PF64 EndArray,
                          PU32 ArrayElements, I16 Direction);
U32 ADVCMNAPI Acm_GpMoveCircularRel_3P(HAND GroupHandle, 
								 PF64	RefArray,
                                 PF64    EndArray,
                                 PU32	pArrayElements,
                                 I16     Direction );
U32 ADVCMNAPI Acm_GpMoveCircularAbs_3P(HAND GroupHandle, 
								 PF64	RefArray,
                                 PF64    EndArray,
                                 PU32	pArrayElements,
                                 I16     Direction );   
U32 ADVCMNAPI Acm_GpMoveHelixAbs(HAND GroupHandle, 
						PF64	CenterArray,
                        PF64    EndArray,
                        PU32	pArrayElements,
                        I16     Direction);   //  [6/21/2011 dan.yang]
U32 ADVCMNAPI Acm_GpMoveHelixRel(HAND GroupHandle, 
						PF64	CenterArray,
                        PF64    EndArray,
                        PU32	pArrayElements,
                        I16     Direction);   //  [6/21/2011 dan.yang]
U32 ADVCMNAPI Acm_GpMoveHelixAbs_3P(HAND GroupHandle, 
								 PF64	RefArray,
                                 PF64    EndArray,
                                 PU32	pArrayElements,
                                 I16     Direction );  //  [6/21/2011 dan.yang]
U32 ADVCMNAPI Acm_GpMoveHelixRel_3P(HAND GroupHandle, 
								 PF64	RefArray,
                                 PF64    EndArray,
                                 PU32	pArrayElements,
                                 I16     Direction );  //  [6/21/2011 dan.yang]
U32 ADVCMNAPI Acm_GpLoadPath(HAND GroupHandle, PI8 FilePath, PHAND PathHandle, PU32 pTotalCount);
U32 ADVCMNAPI Acm_GpUnloadPath(HAND GroupHandle, PHAND PathHandle);
U32 ADVCMNAPI Acm_GpMovePath(HAND GroupHandle, HAND PathHandle);
U32 ADVCMNAPI Acm_GpAddPath (HAND GroupHandle,U16 MoveCmd,U16 MoveMode,F64 FH,F64 FL,
				   PF64 EndPoint_DataArray,PF64 CenPoint_DataArray,PU32 ArrayElements);
U32 ADVCMNAPI Acm_GpResetPath (PHAND GroupHandle);
U32 ADVCMNAPI Acm_GpGetPathStatus (HAND GroupHandle, // IN, Group Handle
						 PU32 pCurIndex, // IN, OUT, Current index of path data in path buffer
						 PU32 pCurCmdFunc, // IN, OUT, Return current command function in executing
						 PU32 pRemainCount, // IN, OUT, Number of unexecuted path data in path
						 PU32 pFreeSpaceCount );
U32 ADVCMNAPI Acm_GpStopDec(HAND GroupHandle);
U32 ADVCMNAPI Acm_GpStopEmg(HAND GroupHandle);
U32 ADVCMNAPI Acm_GpChangeVel(HAND GroupHandle, F64 NewVelocity);
U32 ADVCMNAPI Acm_GpChangeVelByRate(HAND GroupHandle, U32 Rate);//  [11/21/2012 dan.yang]
U32 ADVCMNAPI Acm_GpGetCmdVel(HAND GroupHandle, PF64 CmdVel); //Add for PCI1245 and PCI1265.[Dan.Yang 20110607]
U32 ADVCMNAPI Acm_GpMoveSelPath(HAND	GroupHandle, //  [6/22/2011 dan.yang]
					  HAND  PathHandle,
					  U32	StartIndex,				// range: 0~9999
                      U32	EndIndex,				// range: 0~9999
                      U8	Repeat);
U32 ADVCMNAPI Acm_GpGetPathIndexStatus(HAND       	GroupHandle,  //  [6/22/2011 dan.yang]
								U32 		Index, 		// index of path
								PU16		CmdFunc, 
								PU16		MoveMode,
								PF64 	    FH,	
								PF64 	    FL, 
								PF64 	    EndPoint_DataArray, 
								PF64 	    CenPoint_DataArray,
								PU32 	    ArrayElements);
U32 ADVCMNAPI Acm_GpResumeMotion(HAND GroupHandle); //  [10/12/2012 dan.yang]
U32 ADVCMNAPI Acm_GpPauseMotion(HAND GroupHandle); //  [10/12/2012 dan.yang]
U32 ADVCMNAPI Acm_GpMove3DArcAbs( HAND GroupHandle,
					    PF64 CenterArray,
						PF64 EndArray,
						PU32 pArrayElements,
						I16  Direction);//zhaocui add 201308

U32 ADVCMNAPI Acm_GpMove3DArcRel( HAND GroupHandle,
					       PF64 CenterArray,
						   PF64 EndArray,
						   PU32 pArrayElements,
						   I16  Direction);//zhaocui add 201308

U32 ADVCMNAPI Acm_GpMove3DArcAbs_V( HAND GroupHandle,
					    PF64 CenterArray,
						PF64 NVectorArray,
						F64  Degree,
						PU32 pArrayElements,
						I16  Direction);//zhaocui add 201308

U32 ADVCMNAPI Acm_GpMove3DArcRel_V( HAND GroupHandle,
					    PF64 CenterArray,
						PF64 NVectorArray,
						F64  Degree,
						PU32 pArrayElements,
						I16  Direction);//zhaocui add 201308

// DIO
U32 ADVCMNAPI Acm_DaqDiGetByte(HAND DeviceHandle, U16 DiPort,PU8 ByteData);
U32 ADVCMNAPI Acm_DaqDiGetBit(HAND DeviceHandle, U16 DiChannel, PU8	BitData);
U32 ADVCMNAPI Acm_DaqDoSetByte(HAND DeviceHandle, U16 DoPort, U8	ByteData);
U32 ADVCMNAPI Acm_DaqDoSetBit(HAND	DeviceHandle, U16	DoChannel, U8 BitData);
U32 ADVCMNAPI Acm_DaqDoGetByte(HAND DeviceHandle, U16 DoPort, PU8 ByteData);
U32 ADVCMNAPI Acm_DaqDoGetBit(HAND	DeviceHandle, U16	DoChannel, PU8	BitData);
U32 ADVCMNAPI Acm_DaqDiGetBytes(HAND DeviceHandle, U16 StartPort, U16 NumPort, PU8 ByteDataArray);
U32 ADVCMNAPI Acm_DaqDoSetBytes(HAND DeviceHandle, U16 StartPort, U16 NumPort, PU8 ByteDataArray);
U32 ADVCMNAPI Acm_DaqDoGetBytes(HAND DeviceHandle, U16 StartPort, U16 NumPort, PU8 ByteDataArray);

//AIO [Dan.Yang 2011.07.19]
U32 ADVCMNAPI Acm_DaqAiGetRawData(HAND DeviceHandle, U16 AiChannel, PU16 AiData);
U32 ADVCMNAPI Acm_DaqAiGetVoltData(HAND DeviceHandle, U16	AiChannel, PF32	AiData);
U32 ADVCMNAPI Acm_DaqAiGetCurrData(HAND DeviceHandle, U16	AiChannel, PF32	AiData);
U32 ADVCMNAPI Acm_DaqAiZeroCalibration(HAND DeviceHandle, U16	AiChannel);
U32 ADVCMNAPI Acm_DaqAiSpanCalibration(HAND DeviceHandle, U16	AiChannel);
U32 ADVCMNAPI Acm_DaqAiGetChannelStatus(HAND DeviceHandle, U16	AiChannel, PU32 ChanStatus);
U32 ADVCMNAPI Acm_DaqAoSetRawData(HAND DeviceHandle, U16	AoChannel, U16 AoData);
U32 ADVCMNAPI Acm_DaqAoSetVoltData(HAND DeviceHandle, U16	AoChannel, F32 AoData);
U32 ADVCMNAPI Acm_DaqAoSetCurrData(HAND DeviceHandle, U16	AoChannel, F32 AoData);
U32 ADVCMNAPI Acm_DaqAoGetRawData(HAND DeviceHandle, U16	AoChannel, PU16 AoData);
U32 ADVCMNAPI Acm_DaqAoGetVoltData(HAND DeviceHandle, U16	AoChannel, PF32 AoData);
U32 ADVCMNAPI Acm_DaqAoGetCurrData(HAND DeviceHandle, U16	AoChannel, PF32 AoData);
U32 ADVCMNAPI Acm_DaqAoSetCaliType(HAND DeviceHandle, U16	AoChannel, U16 TrimType);
U32 ADVCMNAPI Acm_DaqAoSetCaliValue(HAND DeviceHandle, U16	AoChannel, U16 CaliData);
U32 ADVCMNAPI Acm_DaqAoCaliDone(HAND DeviceHandle, U16	AoChannel);
//mining: Compare functions
U32 ADVCMNAPI Acm_AxSetCmpAuto(HAND AxisHandle, F64 Start, F64 End, F64 Interval);
U32 ADVCMNAPI Acm_AxGetCmpData(HAND AxisHandle, PF64 CmpPosition);
U32 ADVCMNAPI Acm_AxSetCmpData(HAND AxisHandle, F64	CmpPosition);
U32 ADVCMNAPI Acm_AxSetCmpTable(HAND AxisHandle, PF64 TableArray, I32	ArrayCount);

//Dan.Yang 2011.07.12 :Add for download Dsp Firmware program.
U32 ADVCMNAPI Acm_GetDSPFrmWareDwnLoadRate(HAND DeviceHandle, PF64 Percentage);
U32 ADVCMNAPI Acm_DevDownLoadDSPFrmWare_STP1(HAND DeviceHandle);
U32 ADVCMNAPI Acm_DevDownLoadDSPFrmWare_STP2(HAND DeviceHandle, PI8 FilePath);
U32 ADVCMNAPI Acm_DevDownLoadDSPFrmWare_STP3(HAND DeviceHandle);

U32 ADVCMNAPI Acm_DownLoadMCU_ST2(HAND DeviceHandle, U16 CPLDNo, PI8 FilePath);
U32 ADVCMNAPI Acm_DownLoadMCU_ST1(HAND DeviceHandle, U16 CPLDNo);
U32 ADVCMNAPI Acm_GetMCUDownLoadRate(HAND DeviceHandle, PF64 Percentage);

U32 ADVCMNAPI Acm_AxPWMOut(HAND AxisHandle,U32 OFForON,U32 PulseCount);
U32 ADVCMNAPI Acm_AxGetPWMOutState(HAND AxisHandle,PU32 OFForON);



U32 ADVCMNAPI Acm_DownLoadCPLD_ST2(HAND DeviceHandle, U16 CPLDNo, PI8 FilePath);
U32 ADVCMNAPI Acm_DownLoadCPLD_ST1(HAND DeviceHandle, U16 CPLDNo);
U32 ADVCMNAPI Acm_GetCPLDDownLoadRate(HAND DeviceHandle, PF64 Percentage);
U32 ADVCMNAPI Acm_DevMDaqConfig(HAND DeviceHandle, U16 ChannelID, U32 Period, U32 AxisNo, U32 Method, U32 ChanType, U32 Count);
U32 ADVCMNAPI Acm_DevMDaqStart(HAND DeviceHandle);
U32 ADVCMNAPI Acm_DevMDaqStop(HAND DeviceHandle);
U32 ADVCMNAPI Acm_DevMDaqReset(HAND DeviceHandle, U16 ChannelID);
U32 ADVCMNAPI Acm_DevMDaqGetStatus(HAND DeviceHandle, U16 ChannelID, PU16 CurrentCnt, PU8 Status);
U32 ADVCMNAPI Acm_DevMDaqGetData(HAND DeviceHandle, U16 ChannelID, U16 StartIndex, U16 MaxCount, PI32 DataBuffer);
U32 ADVCMNAPI Acm_DevMDaqGetConfig(HAND DeviceHandle, U16 ChannelID, PU32 Period, PU32 AxisNo, PU32 Method, PU32 ChanType, PU32 Count);

U32	ADVCMNAPI Acm_AxReadLatchBuffer(HAND AxisHandle, PF64 LatchDataArray, PU32 DataCnt);
U32	ADVCMNAPI Acm_AxResetLatchBuffer(HAND AxisHandle);
U32	ADVCMNAPI Acm_AxGetLatchBufferStatus(HAND AxisHandle, PU32 RemainCnt, PU32 SpaceCnt);

//******************SCARA Robot*************************************************/
U32 Acm_GpRbSetMode(HAND GroupHandle, U16 Mode, PI32 CurActPosiArray, U32 ArrayElement);
U32 Acm_GpRbGetCmdPosition(HAND GroupHandle, PF64 PositionArray, U32 ArrayElement);
U32 Acm_GpRbGetActualPosition(HAND GroupHandle, PF64 PositionArray, U32 ArrayElement);
U32 Acm_GpRbGetArmCmdPosition(HAND GroupHandle, U16 ArmID, PF64 CurPosi);
U32 Acm_GpRbGetArmActualPosition(HAND GroupHandle, U16 ArmID, PF64 CurPosi);
//******************************************************************************/

//add by dujunling on 2012.11.22,for GM code function ,
U32 ADVCMNAPI Acm_GmOpen( IN HAND DeviceHandle, OUT PHAND GMSHandle );
U32 ADVCMNAPI Acm_GmClose(IN HAND GMSHandle);

U32 ADVCMNAPI Acm_GmLoadJob(IN HAND GMSHandle, IN PI8 JobFilePath, OUT PU32 ErrorRow);
U32 ADVCMNAPI Acm_GmUploadJob( IN HAND GMSHandle, IN OUT PI8 GCodeBuffer, IN OUT PU32 GCodeLength);
U32 ADVCMNAPI Acm_GmResetJob( IN HAND GMSHandle);

U32 ADVCMNAPI Acm_GmCommand(IN HAND GMSHandle, IN PI8 InputCmd);

U32 ADVCMNAPI Acm_GmRemoveAxisFromSystem(IN HAND GMSHandle, IN U32 uAxisId);

U32 ADVCMNAPI Acm_GmSetCompensationRadius(IN HAND GMSHandle, IN U32 DValue,IN F64 Radius);
U32 ADVCMNAPI Acm_GmGetCompensationRadius(IN HAND GMSHandle,  OUT PU32 pCRArray,IN OUT PU32 pCRArrayLength);
U32 ADVCMNAPI Acm_GmSetCurrentCompensationRadius(IN HAND GMSHandle, IN F64 Radius);
U32 ADVCMNAPI Acm_GmGetCurrentCompensationRadius(IN HAND GMSHandle,  IN OUT PF64 pCRadius);
U32 ADVCMNAPI Acm_GmSetToolLengthOffset(IN HAND GMSHandle, IN U32 HValue,IN F64 ToolLengthOffset);
U32 ADVCMNAPI Acm_GmGetToolLengthOffset(IN HAND GMSHandle, OUT PU32 pTLOArray,IN OUT PU32 pTLOArrayLength);
U32 ADVCMNAPI Acm_GmSetCurrentToolLengthOffset(IN HAND GMSHandle,IN F64 ToolLengthOffset);
U32 ADVCMNAPI Acm_GmGetCurrentToolLengthOffset(IN HAND GMSHandle, IN OUT PF64 pToolLengthOffset);

U32 ADVCMNAPI Acm_GmSetMacro(IN HAND GMSHandle, IN U32 uMacroCmd,IN const PI8 MacroCmdContent);
U32 ADVCMNAPI Acm_GmGetMacro(IN HAND GMSHandle, IN U32 uMacroCmd,OUT PI8 pMacroCmdContent,IN OUT PU32 pMacroCmdContentLength);
U32 ADVCMNAPI Acm_GmGetMacroArray(IN HAND GMSHandle,OUT PU32 pMacroArray,IN OUT PU32 pMacroArrayLength);

U32 ADVCMNAPI Acm_GmSetPreCustomFunction(IN HAND GMSHandle, IN U32 (*ptrPreCustomFunction)(const U32 CustomMCommand,U32 TWordValue,bool &bDone));
U32 ADVCMNAPI Acm_GmSetFinalCustomFunction(IN HAND GMSHandle, IN U32 (*ptrFinalCustomFunction)(const U32 CustomMCommand,U32 TWordValue,bool &bDone));

U32 ADVCMNAPI Acm_GmSetExtDoFunction(IN HAND GMSHandle, IN U32 (*ptrExtDoFunction)(U32 uDoCh,U32 uDoLevel));
U32 ADVCMNAPI Acm_GmSetExtDiFunction(IN HAND GMSHandle, IN U32 (*ptrExtDiFunction)(U32 uDiCh));

U32 ADVCMNAPI Acm_GmSetBreakPoint(IN HAND GMSHandle, IN U32 uBreakPoint);
U32 ADVCMNAPI Acm_GmAddBreakPoint(IN HAND GMSHandle, IN U32 uBreakPoint);
U32 ADVCMNAPI Acm_GmRemoveBreakPoint(IN HAND GMSHandle, IN U32 uBreakPoint);
U32 ADVCMNAPI Acm_GmClearBreakPoint(IN HAND GMSHandle);

U32 ADVCMNAPI Acm_GmSetSpindleRatio(IN HAND GMSHandle, IN F64 fSpindleRatio);
U32 ADVCMNAPI Acm_GmSetJogRatio(IN HAND GMSHandle, IN F64 fJogRatio);
U32 ADVCMNAPI Acm_GmSetFastFeedRateRatio(IN HAND GMSHandle, IN U32 uAxis,IN F64 fFastFeedRateRatio);
U32 ADVCMNAPI Acm_GmGetFastFeedRateRatio(IN HAND GMSHandle, IN U32 uAxis,OUT PF64 pFastFeedRateRatio);
U32 ADVCMNAPI Acm_GmSetFeedRateRatio(IN HAND GMSHandle, IN F64 fFeedRateRatio);
U32 ADVCMNAPI Acm_GmGetFeedRateRatio(IN HAND GMSHandle, OUT PF64 pFeedRateRatio);
U32 ADVCMNAPI Acm_GmSetFeedRate(IN HAND GMSHandle, IN F64 fFeedRate);
U32 ADVCMNAPI Acm_GmGetFeedRate(IN HAND GMSHandle, IN PF64 pFeedRate);
U32 ADVCMNAPI Acm_GmSetFastFeedRate(IN HAND GMSHandle, IN F64 fFastFeedRate);
U32 ADVCMNAPI Acm_GmGetFastFeedRate(IN HAND GMSHandle, OUT PF64 pFastFeedRate);
U32 ADVCMNAPI Acm_GmSetOptionPause(IN HAND GMSHandle, IN U32 uOptionPause);
U32 ADVCMNAPI Acm_GmGetOptionPause(IN HAND GMSHandle, OUT PU32 pOptionPause);
U32 ADVCMNAPI Acm_GmSetOptionSkip(IN HAND GMSHandle, IN U32 uOptionSkip);
U32 ADVCMNAPI Acm_GmGetOptionSkip(IN HAND GMSHandle, OUT PU32 pOptionSkip);
U32 ADVCMNAPI Acm_GmSetModuleRange(IN HAND GMSHandle, IN U32 ModuleRange);
U32 ADVCMNAPI Acm_GmGetModuleRange(IN HAND GMSHandle, IN PU32 pModuleRange);

U32 ADVCMNAPI Acm_GmGetCurrentFeedRate(IN HAND GMSHandle, OUT PF64 pCurrentFeedRate);
U32 ADVCMNAPI Acm_GmGetCurrentRow(IN HAND GMSHandle, OUT PU32 CurrentRow);
U32 ADVCMNAPI Acm_GmGetErrorRow(IN HAND GMSHandle, OUT PU32 ErrorRow);
U32 ADVCMNAPI Acm_GmGetCurrentCommand(IN HAND GMSHandle, OUT PI8 CurrentCmd, IN OUT PU32 CurrentCmdLength);
U32 ADVCMNAPI Acm_GmGetState(IN HAND GMSHandle, OUT PU32 GmsState);
U32 ADVCMNAPI Acm_GmGetPattern(IN HAND GMSHandle, IN U32 GmsPatternId, PVOID Buffer, PU32 BufferLength);
U32 ADVCMNAPI Acm_GmGetCurrentCoordinateSystem(IN HAND GMSHandle, OUT PU32 pCurrentCoordinateIndex,OUT PF64 pCoordinateArray,IN OUT PU32 pArrayLength);
U32 ADVCMNAPI Acm_GmGetError(IN HAND GMSHandle, OUT PU32 ErrorCode, OUT PU32 LastError);
U32 ADVCMNAPI Acm_GmDxfToGCodeTextEx(IN PI8 DxfFilePath, IN F64 DxfProportion, IN U32 Ellipse_Cut_Min_Angel , IN U32 Spline_Cut_Step ,OUT PI8 GCodeBuffer, IN OUT PU32 Length);
U32 ADVCMNAPI Acm_GmDxfToGCodeText(IN PI8 DxfFilePath, IN F64 DxfProportion, OUT PI8 GCodeBuffer, IN OUT PU32  Length);
U32 ADVCMNAPI Acm_GmDxfToGCodeFileEx(IN PI8 DxfFilePath, IN F64 DxfProportion,IN U32 Ellipse_Cut_Min_Angel , IN U32 Spline_Cut_Step ,IN PI8 GcodeFilePath,OUT PU32 Length);
U32 ADVCMNAPI Acm_GmDxfToGCodeFile(IN PI8 DxfFilePath, IN F64 DxfProportion,IN PI8 GcodeFilePath,PU32 Length);

//Added by W.Y.Z on 2014.08.28 for P point function
U32 ADVCMNAPI Acm_GmSetPoint(IN HAND GMSHandle, IN U32 id, IN PF64 fPoint);
U32 ADVCMNAPI Acm_GmGetPoint(IN HAND GMSHandle, IN U32 id,OUT PF64 fPoint);
U32 ADVCMNAPI Acm_GmLoadPoint(IN HAND GMSHandle, IN PI8 FilePath);
U32 ADVCMNAPI Acm_GmSavePoint(IN HAND GMSHandle, IN PI8 FilePath);
U32 ADVCMNAPI Acm_GmResetPoint(IN HAND GMSHandle);

U32 ADVCMNAPI Acm_GmSetPointF(IN HAND GMSHandle, IN U32 id, IN PF64 fPoint,IN U32 filterFlag);
U32 ADVCMNAPI Acm_GmGetPointF(IN HAND GMSHandle, IN U32 id,OUT PF64 fPoint,OUT PU32 filterflag);
//U32 ADVCMNAPI Acm_GmLoadPointF(IN HAND GMSHandle, IN PI8 FilePath);
//U32 ADVCMNAPI Acm_GmSavePointF(IN HAND GMSHandle, IN PI8 FilePath);
//U32 ADVCMNAPI Acm_GmResetPointF(IN HAND GMSHandle);

U32 ADVCMNAPI Acm_GpMoveHelixRel_Angle(HAND GroupHandle, 
						PF64	CenterArray,
                        PF64   EndArray,
                        PU32	pArrayElements,
                        I16    Direction);//added by zhaocui 201305
U32 ADVCMNAPI Acm_GpMoveHelixAbs_Angle(HAND GroupHandle, 
						PF64	CenterArray,
                        PF64   EndArray,
                        PU32	pArrayElements,
                        I16    Direction);//added by zhaocui 201305

U32 ADVCMNAPI Acm_AxSetPWMTableOnTime(HAND AxisHandle, PU32 TimeTableArray, I32 ArrayCount);//zhaocui add


U32 ADVCMNAPI Acm_AxGetINxStopStatus(HAND AxisHandle,PU32 Stop_Flag);//  [2/28/2014 deng]
U32 ADVCMNAPI Acm_AxResetINxStopStatus(HAND AxisHandle);//  [2/28/2014 deng]
U32 ADVCMNAPI Acm_GpGetINxStopStatus(HAND GroupHandle,PU32 Stop_Flag);//  [2/28/2014 deng]
U32 ADVCMNAPI Acm_GpResetINxStopStatus(HAND GroupHandle);//  [2/28/2014 deng]

U32 ADVCMNAPI Acm_AxJog(HAND AxisHandle,U16 Direction); //[5/22/2014 kai.yang]


//zhao cui add for robot 201307
U32 ADVCMNAPI Acm_RbGetActualPosition(HAND RbHandle, PF64 PositionArray, U32 ArrayElement);
U32 ADVCMNAPI Acm_RbGetCmdPosition(HAND RbHandle, PF64 PositionArray, U32 ArrayElement);
U32 ADVCMNAPI Acm_RbGetArmActualPosition(HAND RbHandle, U16 ArmID, PF64 CurPosi);
U32 ADVCMNAPI Acm_RbGetArmCmdPosition(HAND RbHandle, U16 ArmID, PF64 CurPosi);
U32 ADVCMNAPI Acm_RbOpen (HAND DeviceHandle,PHAND RbHandle);
U32 ADVCMNAPI Acm_RbClose (PHAND pRbHandle);
U32 ADVCMNAPI Acm_RbResetError (HAND RbHandle);
U32 ADVCMNAPI Acm_RbGetState (HAND RbHandle, PU16 PStates);
U32 ADVCMNAPI Acm_RbSetActPosition (HAND RbHandle, PF64 PositionArray, U32 ArrayElement);
U32 ADVCMNAPI Acm_RbMoveRel(HAND RbHandle, PF64 PosArray, PU32 ArrayElement);
U32 ADVCMNAPI Acm_RbMoveAbs(HAND RbHandle, PF64 PosArray, PU32 ArrayElement);
U32 ADVCMNAPI Acm_RbMoveDirectRel (HAND RbHandle, PF64 PositionArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_RbMoveDirectAbs (HAND RbHandle, PF64 PositionArray, PU32 ArrayElements);
U32 ADVCMNAPI Acm_RbMoveLinearRel( HAND RbHandle,PF64 PositionArray,PU32 pArrayElements);
U32 ADVCMNAPI Acm_RbMoveLinearAbs( HAND RbHandle,PF64 PositionArray,PU32 pArrayElements);
U32 ADVCMNAPI Acm_RbMoveArcAbs( HAND RbHandle,PF64 CenterArray,PF64 EndArray,PU32 pArrayElements, I16  Direction);
U32 ADVCMNAPI Acm_RbMoveArcRel( HAND RbHandle,PF64 CenterArray,PF64 EndArray,PU32 pArrayElements, I16  Direction);
U32 ADVCMNAPI Acm_RbMoveArcRel_3P ( HAND RbHandle,PF64 RefArray,PF64 EndArray,PU32 pArrayElements, I16  Direction);
U32 ADVCMNAPI Acm_RbMoveArcAbs_3P ( HAND RbHandle,PF64 RefArray,PF64 EndArray,PU32 pArrayElements, I16  Direction);
U32 ADVCMNAPI Acm_RbAddPath (HAND RbHandle,U16 MoveCmd,U16 MoveMode,F64 FH,F64 FL,
				   PF64 EndPoint_DataArray,PF64 CenPoint_DataArray,PU32 ArrayElements);
U32 ADVCMNAPI Acm_RbResetPath (PHAND RbHandle);
U32 ADVCMNAPI Acm_RbGetPathStatus (HAND RbHandle, // IN, Group Handle
						 PU32 pCurIndex, // IN, OUT, Current index of path data in path buffer
						 PU32 pCurCmdFunc, // IN, OUT, Return current command function in executing
						 PU32 pRemainCount, // IN, OUT, Number of unexecuted path data in path
						 PU32 pFreeSpaceCount );
U32 ADVCMNAPI Acm_RbMovePath(HAND RbHandle, HAND PathHandle);
U32 ADVCMNAPI Acm_RbChangeVel(HAND RbHandle, F64 NewVelocity);
U32 ADVCMNAPI Acm_RbChangeVelByRate(HAND RbHandle, U32 Rate);
U32 ADVCMNAPI Acm_RbGetCmdVel(HAND RbHandle, PF64 CmdVel);
U32 ADVCMNAPI Acm_RbStopDec ( HAND RbHandle);
U32 ADVCMNAPI Acm_RbStopEmg(HAND RbHandle);
U32 ADVCMNAPI Acm_RbPauseMotion(HAND RbHandle);
U32 ADVCMNAPI Acm_RbResumeMotion(HAND RbHandle);
bool IsGMSHandleValid(HAND GMSHandle);
U32 ADVCMNAPI Acm_RbLoadPath(HAND RbHandle, PI8 FilePath, PHAND PathHandle, PU32 pTotalCount);//  [6/26/2014 deng]
U32 ADVCMNAPI Acm_RbUnloadPath(HAND RbHandle, PHAND PathHandle);//  [6/26/2014 deng]
U32 ADVCMNAPI Acm_RbMoveSelPath(HAND	RbHandle,
					  HAND  PathHandle,
					  U32	StartIndex,				// range: 0~9999
					  U32	EndIndex,				// range: 0~9999
					  U8	Repeat);//  [6/26/2014 deng]
U32 ADVCMNAPI Acm_RbGetPathIndexStatus(HAND       	RbHandle, 
							 U32 		Index, 				// index of path
							 PU16		CmdFunc, 
							 PU16		MoveMode,
							 PF64 	    FH,	
							 PF64 	    FL, 
							 PF64 	    EndPoint_DataArray, 
							 PF64 	    CenPoint_DataArray,
							 PU32 	    ArrayElements);//  [6/26/2014 deng]
U32 ADVCMNAPI Acm_RbSetExtDrive(HAND RbHandle, U16 ExtDrvMode);//  [7/3/2014 deng]
U32 ADVCMNAPI Acm_RbJog(HAND RbHandle,U16 Direction);//  [7/3/2014 deng]

U32 ADVCMNAPI Acm_ServoSetCom(U32 ComPortID, U32 Baudrate, U32 Timeout);//  [7/17/2014 yang.kai]
U32 ADVCMNAPI Acm_ServoGetAbsPosition(
							U32 ComPortID,  
							U32 ServoType,
							U32 ServoID, 
							U32 ServoAbsResolution,  
							U32 ServoCmdResolution,
							U32 EncoderDir,
							PF64 AbsPosition
							);                                           //  [7/17/2014 yang.kai]
U32 ADVCMNAPI Acm_AxSetCmdPosi_Pulse(HAND AxisHandle,F64 Position);      //  [7/17/2014 yang.kai]
U32 ADVCMNAPI Acm_AxMoveAbs_EC(HAND AxisHandle, F64 Position);//  [8/18/2014 deng]
U32 ADVCMNAPI Acm_RbGetWorldPosFromJoint(HAND RbHandle,PF64 JointPosArray, PF64 WorldPosArray);  //  [8/12/2014 yang.kai]
U32 ADVCMNAPI Acm_GetDevNum(U32 DevType,U32 BoardID,PU32 DeviceNumber);            //  [9/2/2014 yang.kai]
U32 ADVCMNAPI Acm_GpMoveArcRel_Angle(HAND  GroupHandle,  //  [9/22/2014 yang.kai]
										  PF64 CenterArray, 
										  F64 Degree,
										  PU32 ArrayElements,
										  I16  Direction);

U32 ADVCMNAPI Acm_GpMoveArcAbs_Angle(HAND  GroupHandle,  //  [9/22/2014 yang.kai]
										  PF64 CenterArray, 
										  F64 Degree,
										  PU32 ArrayElements,
										  I16  Direction);

U32 ADVCMNAPI Acm_AxChangeCmpIndex(HAND AxisHandle,    //  [9/22/2014 yang.kai]
								   U32 CmpIndex);

//mapping 
U32 ADVCMNAPI Acm_DevSaveMapFile(HAND DeviceHandle, PI8 FilePath);
U32 ADVCMNAPI Acm_DevLoadMapFile(HAND DeviceHandle, PI8 FilePath);
U32 ADVCMNAPI Acm_DevUpLoadMapInfo(HAND DeviceHandle, U16 MapType, PDEV_IO_MAP_INFO MapInfoArray, PU32 ArrayLength);
U32 ADVCMNAPI Acm_DevDownLoadMapInfo(HAND DeviceHandle, U16 MapType, PDEV_IO_MAP_INFO MapInfoArray, U32 ArrayLength);
#ifdef __cplusplus
}
#endif
