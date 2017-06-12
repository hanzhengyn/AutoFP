/*******************************************************************************
* [ CmmsdkApi.cpp ]
* Source(C/C++) file for loading CMMSDK library
* - Update Data: 2006/09/11
* - Provider: COMIZOA Co., Ltd.
* - Phone: +82-42-936-6500~6
* - Fax  : +82-42-936-6507
* - URL  : http://www.comizoa.co.kr,  http://www.comizoa.com
********************************************************************************/

#include "stdafx.h" // VC++ 이 아닌 다른곳에서 컴파일할 때는 이 구문을 삭제하십시오.

#if defined(__BCPLUSPLUS__)  // Borland C++ Builder specific
 #include <vcl.h>
 #pragma hdrstop
#endif

#include <windows.h>
#define _CMM_SDK_LOADER_C_
#include "Cmmsdk.h"
#include <stdio.h>

#define __DLL_FILE_FULL_PATH__ "CmmSDK.DLL"

typedef struct{
    char *name;
    FARPROC* ptr;
}LIB_FUNCT;

static HINSTANCE g_hCmmLibrary=NULL;

BOOL cmmLoadDll(void)
{
	LIB_FUNCT Func[] = {
				{"cmmGnDeviceLoad", 	 (FARPROC*)&cmmGnDeviceLoad},
				{"cmmGnDeviceUnload",	 (FARPROC*)&cmmGnDeviceUnload},
				{"cmmGnDeviceIsLoaded",  (FARPROC*)&cmmGnDeviceIsLoaded},
				{"cmmGnDeviceReset",     (FARPROC*)&cmmGnDeviceReset},
				{"cmmGnInitFromFile",	 (FARPROC*)&cmmGnInitFromFile},
				{"cmmGnInitFromFile",	 (FARPROC*)&cmmGnInitFromFile_MapOnly},
				{"cmmGnSetServoOn",	 (FARPROC*)&cmmGnSetServoOn},
				{"cmmGnGetServoOn",	 (FARPROC*)&cmmGnGetServoOn},
				{"cmmGnSetAlarmRes",	 (FARPROC*)&cmmGnSetAlarmRes},
				{"cmmGnGetAlarmRes",	 (FARPROC*)&cmmGnGetAlarmRes},
				{"cmmGnPulseAlarmRes",	 (FARPROC*)&cmmGnPulseAlarmRes},
				{"cmmGnSetSimulMode",    (FARPROC*)&cmmGnSetSimulMode},
				{"cmmGnGetSimulMode",	 (FARPROC*)&cmmGnGetSimulMode},
				{"cmmGnPutInternalSTA",	 (FARPROC*)&cmmGnPutInternalSTA},
				{"cmmGnSetEmergency",    (FARPROC*)&cmmGnSetEmergency},
				{"cmmGnGetEmergency",    (FARPROC*)&cmmGnGetEmergency},
				{"cmmGnBitShift", 	 (FARPROC*)&cmmGnBitShift},
				{"cmmCfgSetMioProperty", (FARPROC*)&cmmCfgSetMioProperty}, 
				{"cmmCfgGetMioProperty", (FARPROC*)&cmmCfgGetMioProperty},
				{"cmmCfgSetFilter",	 (FARPROC*)&cmmCfgSetFilter},
				{"cmmCfgGetFilter",	 (FARPROC*)&cmmCfgGetFilter},
				{"cmmCfgSetFilterAB",	 (FARPROC*)&cmmCfgSetFilterAB},
				{"cmmCfgGetFilterAB",	 (FARPROC*)&cmmCfgGetFilterAB},
				{"cmmCfgSetInMode",	 (FARPROC*)&cmmCfgSetInMode},
				{"cmmCfgGetInMode",	 (FARPROC*)&cmmCfgGetInMode},
				{"cmmCfgSetOutMode",	 (FARPROC*)&cmmCfgSetOutMode},
				{"cmmCfgGetOutMode",	 (FARPROC*)&cmmCfgGetOutMode},
				{"cmmCfgSetCtrlMode",	 (FARPROC*)&cmmCfgSetCtrlMode},
				{"cmmCfgGetCtrlMode",	 (FARPROC*)&cmmCfgGetCtrlMode},
				{"cmmCfgSetInOutRatio",  (FARPROC*)&cmmCfgSetInOutRatio},
				{"cmmCfgSetUnitDist",	 (FARPROC*)&cmmCfgSetUnitDist},
				{"cmmCfgGetUnitDist",	 (FARPROC*)&cmmCfgGetUnitDist},
				{"cmmCfgSetUnitSpeed",	 (FARPROC*)&cmmCfgSetUnitSpeed},
				{"cmmCfgGetUnitSpeed",	 (FARPROC*)&cmmCfgGetUnitSpeed}, 
				{"cmmCfgSetSpeedRange",	 (FARPROC*)&cmmCfgSetSpeedRange}, 
				{"cmmCfgGetSpeedRange",	 (FARPROC*)&cmmCfgGetSpeedRange}, 
				{"cmmCfgSetSpeedPattern",(FARPROC*)&cmmCfgSetSpeedPattern},
				{"cmmCfgGetSpeedPattern",(FARPROC*)&cmmCfgGetSpeedPattern},
				{"cmmCfgSetActSpdCheck" ,(FARPROC*)&cmmCfgSetActSpdCheck},
				{"cmmCfgGetActSpdCheck" ,(FARPROC*)&cmmCfgGetActSpdCheck},
				{"cmmCfgSetSoftLimit",	 (FARPROC*)&cmmCfgSetSoftLimit},
				{"cmmCfgGetSoftLimit",	  (FARPROC*)&cmmCfgGetSoftLimit},
				{"cmmCfgSetVelCorrRatio", (FARPROC*)&cmmCfgSetVelCorrRatio},
				{"cmmCfgGetVelCorrRatio", (FARPROC*)&cmmCfgGetVelCorrRatio},
				{"cmmCfgSetSeqMode", (FARPROC*)&cmmCfgSetSeqMode},
				{"cmmCfgGetSeqMode", (FARPROC*)&cmmCfgGetSeqMode},				
				{"cmmCfgSetManExtLimit", (FARPROC*)&cmmCfgSetManExtLimit},
				{"cmmCfgGetManExtLimit", (FARPROC*)&cmmCfgGetManExtLimit},
				{"cmmHomeSetConfig",	  (FARPROC*)&cmmHomeSetConfig},
				{"cmmHomeGetConfig",	  (FARPROC*)&cmmHomeGetConfig},
				{"cmmHomeSetSpeedPattern",(FARPROC*)&cmmHomeSetSpeedPattern},
				{"cmmHomeGetSpeedPattern",(FARPROC*)&cmmHomeGetSpeedPattern},
				{"cmmHomeMoveStart",	  (FARPROC*)&cmmHomeMoveStart},
				{"cmmHomeMove",		  (FARPROC*)&cmmHomeMove},
				{"cmmHomeMoveAll",	  (FARPROC*)&cmmHomeMoveAll},
				{"cmmHomeMoveAllStart",	  (FARPROC*)&cmmHomeMoveAllStart},
				{"cmmHomeGetSuccess",	  (FARPROC*)&cmmHomeGetSuccess}, 
				{"cmmHomeSetSuccess",	  (FARPROC*)&cmmHomeSetSuccess}, 
				{"cmmHomeIsBusy",	  (FARPROC*)&cmmHomeIsBusy}, 
				{"cmmHomeWaitDone",	  (FARPROC*)&cmmHomeWaitDone}, 
				{"cmmSxSetSpeedRatio",	  (FARPROC*)&cmmSxSetSpeedRatio},
				{"cmmSxGetSpeedRatio",	  (FARPROC*)&cmmSxGetSpeedRatio},
				{"cmmSxMoveStart",	  (FARPROC*)&cmmSxMoveStart},
				{"cmmSxMove",		  (FARPROC*)&cmmSxMove},
				{"cmmSxMoveToStart",	  (FARPROC*)&cmmSxMoveToStart},
				{"cmmSxMoveTo",		  (FARPROC*)&cmmSxMoveTo},
				{"cmmSxVMoveStart",	  (FARPROC*)&cmmSxVMoveStart},
				{"cmmSxStop",		  (FARPROC*)&cmmSxStop},
				{"cmmSxStopEmg",	  (FARPROC*)&cmmSxStopEmg},
				{"cmmSxIsDone",		  (FARPROC*)&cmmSxIsDone},
				{"cmmSxWaitDone",	  (FARPROC*)&cmmSxWaitDone},
				{"cmmSxOptSetIniSpeed",   (FARPROC*)&cmmSxOptSetIniSpeed},
				{"cmmSxOptGetIniSpeed",   (FARPROC*)&cmmSxOptGetIniSpeed},
				{"cmmSxSetCorrection",	  (FARPROC*)&cmmSxSetCorrection},
				{"cmmSxGetCorrection",	  (FARPROC*)&cmmSxGetCorrection},
				{"cmmSxOptSetSyncMode",   (FARPROC*)&cmmSxOptSetSyncMode},
				{"cmmSxOptGetSyncMode",   (FARPROC*)&cmmSxOptGetSyncMode},
				{"cmmSxOptSetSyncOut",   (FARPROC*)&cmmSxOptSetSyncOut},
				{"cmmSxOptGetSyncOut",   (FARPROC*)&cmmSxOptGetSyncOut},
				{"cmmSxOptSetRdpOffset",  (FARPROC*)&cmmSxOptSetRdpOffset},
				{"cmmSxOptGetRdpOffset",  (FARPROC*)&cmmSxOptGetRdpOffset},
				{"cmmMxMove",	(FARPROC*)&cmmMxMove},
				{"cmmMxMoveStart",	(FARPROC*)&cmmMxMoveStart},
				{"cmmMxMoveTo",	(FARPROC*)&cmmMxMoveTo},
				{"cmmMxMoveToStart",	(FARPROC*)&cmmMxMoveToStart},
				{"cmmMxVMoveStart",	(FARPROC*)&cmmMxVMoveStart},
				{"cmmMxStop",	(FARPROC*)&cmmMxStop},
				{"cmmMxStopEmg",	(FARPROC*)&cmmMxStopEmg},
				{"cmmMxIsDone",	(FARPROC*)&cmmMxIsDone},
				{"cmmMxWaitDone",	(FARPROC*)&cmmMxWaitDone},
				{"cmmIxMapAxes",	(FARPROC*)&cmmIxMapAxes},
				{"cmmIxSetSpeedPattern", (FARPROC*)&cmmIxSetSpeedPattern},
				{"cmmIxGetSpeedPattern", (FARPROC*)&cmmIxGetSpeedPattern},
				{"cmmIxLine",	(FARPROC*)&cmmIxLine},
				{"cmmIxLineStart",	(FARPROC*)&cmmIxLineStart},
				{"cmmIxLineTo",	(FARPROC*)&cmmIxLineTo},
				{"cmmIxLineToStart",	(FARPROC*)&cmmIxLineToStart},
				{"cmmIxArcA",	(FARPROC*)&cmmIxArcA},
				{"cmmIxArcAStart",	(FARPROC*)&cmmIxArcAStart},
				{"cmmIxArcATo",	(FARPROC*)&cmmIxArcATo},
				{"cmmIxArcAToStart",	(FARPROC*)&cmmIxArcAToStart},
				{"cmmIxArcP",	(FARPROC*)&cmmIxArcP},
				{"cmmIxArcPStart",	(FARPROC*)&cmmIxArcPStart},
				{"cmmIxArcPTo",	(FARPROC*)&cmmIxArcPTo},
				{"cmmIxArcPToStart",	(FARPROC*)&cmmIxArcPToStart},
				{"cmmIxIsDone",	(FARPROC*)&cmmIxIsDone},
				{"cmmIxWaitDone",	(FARPROC*)&cmmIxWaitDone},
				{"cmmIxxHelOnceSetSpeed",	(FARPROC*)&cmmIxxHelOnceSetSpeed},
				{"cmmIxxHelOnce",	(FARPROC*)&cmmIxxHelOnce},
				{"cmmIxxHelOnceStart",	(FARPROC*)&cmmIxxHelOnceStart},
				{"cmmIxxSplineBuild",	(FARPROC*)&cmmIxxSplineBuild},
				{"cmmIxStop",	(FARPROC*)&cmmIxStop},
				{"cmmIxStopEmg",	(FARPROC*)&cmmIxStopEmg},
				{"cmmExVMoveStart",	(FARPROC*)&cmmExVMoveStart},
				{"cmmExMoveStart",	(FARPROC*)&cmmExMoveStart},
				{"cmmExMoveToStart",	(FARPROC*)&cmmExMoveToStart},
				{"cmmPlsrSetInMode",	(FARPROC*)&cmmPlsrSetInMode},
				{"cmmPlsrGetInMode",	(FARPROC*)&cmmPlsrGetInMode},
				{"cmmPlsrHomeMoveStart",	(FARPROC*)&cmmPlsrHomeMoveStart},
				{"cmmPlsrMoveStart",	(FARPROC*)&cmmPlsrMoveStart},
				{"cmmPlsrMove",	(FARPROC*)&cmmPlsrMove},
				{"cmmPlsrMoveToStart",	(FARPROC*)&cmmPlsrMoveToStart},
				{"cmmPlsrMoveTo",	(FARPROC*)&cmmPlsrMoveTo},
				{"cmmPlsrVMoveStart",	(FARPROC*)&cmmPlsrVMoveStart},
				{"cmmPlsrIsActive",	(FARPROC*)&cmmPlsrIsActive},
				{"cmmMsRegisterSlave",	(FARPROC*)&cmmMsRegisterSlave},
				{"cmmMsUnregisterSlave",	(FARPROC*)&cmmMsUnregisterSlave},
				{"cmmMsCheckSlaveState",	(FARPROC*)&cmmMsCheckSlaveState},
				{"cmmMsGetMasterAxis",	(FARPROC*)&cmmMsGetMasterAxis},
				//====================== Overriding FUNCTIONS =========================================//
				{"cmmOverrideSpeedSet", (FARPROC*)&cmmOverrideSpeedSet},
				{"cmmOverrideSpeedSetAll", (FARPROC*)&cmmOverrideSpeedSetAll},
				{"cmmOverrideMove", (FARPROC*)&cmmOverrideMove},
				{"cmmOverrideMoveTo", (FARPROC*)&cmmOverrideMoveTo},
				//====================== LIST-MOTION FUNCTIONS ========================================//
				{"cmmLmMapAxes", (FARPROC*)&cmmLmMapAxes},
				{"cmmLmBeginList", (FARPROC*)&cmmLmBeginList},
				{"cmmLmEndList", (FARPROC*)&cmmLmEndList},
				{"cmmLmIsDone", (FARPROC*)&cmmLmIsDone},
				{"cmmLmWaitDone", (FARPROC*)&cmmLmWaitDone},
				{"cmmLmStartMotion", (FARPROC*)&cmmLmStartMotion},
				{"cmmLmAbortMotion", (FARPROC*)&cmmLmAbortMotion},
				{"cmmLmCurSequence", (FARPROC*)&cmmLmCurSequence},
				{"cmmLmImmediacySet", (FARPROC*)&cmmLmImmediacySet},
				{"cmmLmDoPutOne", (FARPROC*)&cmmLmDoPutOne},
				{"cmmLmDoPutMulti", (FARPROC*)&cmmLmDoPutMulti},
				{"cmmLmDoPulseOne", (FARPROC*)&cmmLmDoPulseOne},
				{"cmmLmDoPulseMulti", (FARPROC*)&cmmLmDoPulseMulti},
				//====================== 상태감시 FUNCTIONS ===========================================//
				{"cmmStSetCount", (FARPROC*)&cmmStSetCount},
				{"cmmStGetCount", (FARPROC*)&cmmStGetCount},
				{"cmmStSetPosition", (FARPROC*)&cmmStSetPosition},
				{"cmmStGetPosition", (FARPROC*)&cmmStGetPosition},
				{"cmmStGetSpeed", (FARPROC*)&cmmStGetSpeed},
				{"cmmStReadMotionState", (FARPROC*)&cmmStReadMotionState},
				{"cmmStReadMioStatuses", (FARPROC*)&cmmStReadMioStatuses},
				{"cmmStGetMstString", (FARPROC*)&cmmStGetMstString},
				//====================== INTERRUPT FUNCTIONS ==========================================//
				{"cmmIntSetMask", (FARPROC*)&cmmIntSetMask},
				{"cmmIntGetMask", (FARPROC*)&cmmIntGetMask},
				{"cmmIntHandlerSetup", (FARPROC*)&cmmIntHandlerSetup},
				{"cmmIntHandlerEnable", (FARPROC*)&cmmIntHandlerEnable},
				{"cmmIntReadFlag", (FARPROC*)&cmmIntReadFlag},
				{"cmmIntReadErrorStatus", (FARPROC*)&cmmIntReadErrorStatus},
				{"cmmIntReadEventStatus", (FARPROC*)&cmmIntReadEventStatus},
				//====================== LATCH FUNCTIONS ==============================================//
				{"cmmLtcIsLatched", (FARPROC*)&cmmLtcIsLatched},
				{"cmmLtcReadLatch", (FARPROC*)&cmmLtcReadLatch},
				//====================== Position Compare FUNCTIONS ===================================//
				{"cmmCmpErrSetConfig", (FARPROC*)&cmmCmpErrSetConfig},
				{"cmmCmpErrGetConfig", (FARPROC*)&cmmCmpErrGetConfig},
				{"cmmCmpGenSetConfig", (FARPROC*)&cmmCmpGenSetConfig},
				{"cmmCmpGenGetConfig", (FARPROC*)&cmmCmpGenGetConfig},
				{"cmmCmpTrgSetConfig", (FARPROC*)&cmmCmpTrgSetConfig},
				{"cmmCmpTrgGetConfig", (FARPROC*)&cmmCmpTrgGetConfig},
				{"cmmCmpTrgSetOneData", (FARPROC*)&cmmCmpTrgSetOneData},
				{"cmmCmpTrgGetCurData", (FARPROC*)&cmmCmpTrgGetCurData},
				{"cmmCmpTrgContRegTable", (FARPROC*)&cmmCmpTrgContRegTable},
				{"cmmCmpTrgContBuildTable", (FARPROC*)&cmmCmpTrgContBuildTable},
				{"cmmCmpTrgContStart", (FARPROC*)&cmmCmpTrgContStart},
				{"cmmCmpTrgContStop", (FARPROC*)&cmmCmpTrgContStop},
				{"cmmCmpTrgContIsActive", (FARPROC*)&cmmCmpTrgContIsActive},
				{"cmmCmpTrgHigh_WriteData", (FARPROC*)&cmmCmpTrgHigh_WriteData},
				{"cmmCmpTrgHigh_ReadData", (FARPROC*)&cmmCmpTrgHigh_ReadData},
				{"cmmCmpTrgHigh_Start", (FARPROC*)&cmmCmpTrgHigh_Start},
				{"cmmCmpTrgHigh_Stop", (FARPROC*)&cmmCmpTrgHigh_Stop},
				{"cmmCmpTrgHigh_Check", (FARPROC*)&cmmCmpTrgHigh_Check},
				//====================== Digital In/Out FUNCTIONS =====================================//
				{"cmmDiSetInputLogic", (FARPROC*)&cmmDiSetInputLogic},
				{"cmmDiGetInputLogic", (FARPROC*)&cmmDiGetInputLogic},
				{"cmmDiGetOne", (FARPROC*)&cmmDiGetOne},
				{"cmmDiGetMulti", (FARPROC*)&cmmDiGetMulti},
				{"cmmDiGetOneF", (FARPROC*)&cmmDiGetOneF},
				{"cmmDiGetMultiF", (FARPROC*)&cmmDiGetMultiF},
				{"cmmDoSetOutputLogic", (FARPROC*)&cmmDoSetOutputLogic},
				{"cmmDoGetOutputLogic", (FARPROC*)&cmmDoGetOutputLogic},
				{"cmmDoPutOne", (FARPROC*)&cmmDoPutOne},
				{"cmmDoGetOne", (FARPROC*)&cmmDoGetOne},
				{"cmmDoPulseOne", (FARPROC*)&cmmDoPulseOne},
				{"cmmDoPutMulti", (FARPROC*)&cmmDoPutMulti},
				{"cmmDoGetMulti", (FARPROC*)&cmmDoGetMulti},
				{"cmmDoPulseMulti", (FARPROC*)&cmmDoPulseMulti},
				//====================== Advanced FUNCTIONS ===========================================//
				{"cmmAdvGetNumAvailAxes", (FARPROC*)&cmmAdvGetNumAvailAxes},
				{"cmmAdvGetNumDefinedAxes", (FARPROC*)&cmmAdvGetNumDefinedAxes},
				{"cmmAdvGetNumAvailDioChan", (FARPROC*)&cmmAdvGetNumAvailDioChan},
				{"cmmAdvGetNumDefinedDioChan", (FARPROC*)&cmmAdvGetNumDefinedDioChan},
				{"cmmAdvGetMotDeviceId", (FARPROC*)&cmmAdvGetMotDeviceId},
				{"cmmAdvGetMotDevInstance", (FARPROC*)&cmmAdvGetMotDevInstance},
				{"cmmAdvGetDioDeviceId", (FARPROC*)&cmmAdvGetDioDeviceId},
				{"cmmAdvGetDioDevInstance", (FARPROC*)&cmmAdvGetDioDevInstance},
				{"cmmAdvGetDeviceHandle", (FARPROC*)&cmmAdvGetDeviceHandle},
				{"cmmAdvWriteMainSpace", (FARPROC*)&cmmAdvWriteMainSpace},
				{"cmmAdvReadMainSpace", (FARPROC*)&cmmAdvReadMainSpace},
				{"cmmAdvWriteRegister", (FARPROC*)&cmmAdvWriteRegister},
				{"cmmAdvReadRegister", (FARPROC*)&cmmAdvReadRegister},
				{"cmmAdvGetMioCfg1Dword", (FARPROC*)&cmmAdvGetMioCfg1Dword},
				{"cmmAdvSetMioCfg1Dword", (FARPROC*)&cmmAdvSetMioCfg1Dword},
				{"cmmAdvSetToolboxMode", (FARPROC*)&cmmAdvSetToolboxMode},
				{"cmmAdvGetString", (FARPROC*)&cmmAdvGetString},
				{"cmmAdvErcOut", (FARPROC*)&cmmAdvErcOut},
				{"cmmAdvErcReset", (FARPROC*)&cmmAdvErcReset},
				{"cmmAdvSetExtOptions", (FARPROC*)&cmmAdvSetExtOptions},
				{"cmmAdvEnumMotDevices", (FARPROC*)&cmmAdvEnumMotDevices},
				{"cmmAdvGetMotDevMap", (FARPROC*)&cmmAdvGetMotDevMap},
				{"cmmAdvEnumDioDevices", (FARPROC*)&cmmAdvEnumDioDevices},
				{"cmmAdvGetDioDevMap", (FARPROC*)&cmmAdvGetDioDevMap},
				{"cmmAdvInitFromCmeBuffer", (FARPROC*)&cmmAdvInitFromCmeBuffer},
				{"cmmAdvInitFromCmeBuffer_MapOnly", (FARPROC*)&cmmAdvInitFromCmeBuffer_MapOnly},
				{"cmmAdvGetLatestCmeFile", (FARPROC*)&cmmAdvGetLatestCmeFile},
				{"cmmAdvGetAxisCapability", (FARPROC*)&cmmAdvGetAxisCapability},
				//====================== DEBUG-LOGGING FUNCTIONS ======================================//
				{"cmmDlogSetup", (FARPROC*)&cmmDlogSetup},
				{"cmmDlogAddComment", (FARPROC*)&cmmDlogAddComment},
				{"cmmDlogGetCurLevel", (FARPROC*)&cmmDlogGetCurLevel},
				{"cmmDlogGetCurFilePath", (FARPROC*)&cmmDlogGetCurFilePath},
				//====================== ERROR HANDLING FUNCTIONS =====================================//
				{"cmmErrGetLastCode", (FARPROC*)&cmmErrGetLastCode},
				{"cmmErrParseAxis", (FARPROC*)&cmmErrParseAxis},
				{"cmmErrParseReason", (FARPROC*)&cmmErrParseReason},
				{"cmmErrGetString", (FARPROC*)&cmmErrGetString},
				{"cmmErrShowLast", (FARPROC*)&cmmErrShowLast},
				{"cmmErrSetSkipShowMessage", (FARPROC*)&cmmErrSetSkipShowMessage},
				{"cmmErrGetSkipShowMessage", (FARPROC*)&cmmErrGetSkipShowMessage},
				{"cmmErrSetEnableAutoMessage", (FARPROC*)&cmmErrSetEnableAutoMessage},
				{"cmmErrGetEnableAutoMessage", (FARPROC*)&cmmErrGetEnableAutoMessage},
				//====================== ERROR HANDLING FUNCTIONS =====================================//
				{"cmmUtlProcessWndMsgS", (FARPROC*)&cmmUtlProcessWndMsgS},
				{"cmmUtlProcessWndMsgM", (FARPROC*)&cmmUtlProcessWndMsgM},
				{"cmmUtlDelayMicroSec", (FARPROC*)&cmmUtlDelayMicroSec},				
				{"cmmUtlReadUserTable", (FARPROC*)&cmmUtlReadUserTable},
				{"cmmUtlWriteUserTable", (FARPROC*)&cmmUtlWriteUserTable},
				
				// TODO 
				{NULL, NULL}
			   };
	LIB_FUNCT *f;
	
	if((g_hCmmLibrary = LoadLibrary (__DLL_FILE_FULL_PATH__)) == NULL)
    	return FALSE;
	
	f = &Func[0];
	while(f->ptr != NULL)
	{
		*(f->ptr) = GetProcAddress ((HMODULE)g_hCmmLibrary, f->name);
		if(*(f->ptr) == NULL){
			//printf("Failed to load \"%s\" Function\n", f->name);
		}
		f++;
	}
	return TRUE;
}

void cmmUnloadDll(void)
{
	if(g_hCmmLibrary){
		FreeLibrary(g_hCmmLibrary);
		g_hCmmLibrary = NULL;
	}
}

