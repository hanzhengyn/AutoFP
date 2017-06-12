#include "stdafx.h" // Visual C++에서는 이 주석을 해제한다(단 Precomipiled header를 사용하지 않는 경우에는 예외). 
#include <windows.h>
#define _COMIDAS_LOADER_C_
#include "Comidas.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
    char *name;
    FARPROC* ptr;
}LIB_FUNCT;

HINSTANCE hLibrary=NULL;

BOOL COMI_LoadDll(void)
{
	LIB_FUNCT Func[] = {
						{"COMI_LoadDevice", 	(FARPROC*)&COMI_LoadDevice},
						{"COMI_UnloadDevice", 	(FARPROC*)&COMI_UnloadDevice},
						{"COMI_GetAvailDevList", 	(FARPROC*)&COMI_GetAvailDevList},
						{"COMI_GetDevInfo", 	(FARPROC*)&COMI_GetDevInfo},
						{"COMI_Write8402", 	(FARPROC*)&COMI_Write8402},
						{"COMI_WriteEEPR", 	(FARPROC*)&COMI_WriteEEPR},
						{"COMI_ReadEEPR", 	(FARPROC*)&COMI_ReadEEPR},
						{"COMI_AD_SetRange", 	(FARPROC*)&COMI_AD_SetRange},
						{"COMI_AD_GetDigit", 	(FARPROC*)&COMI_AD_GetDigit},
						{"COMI_AD_GetVolt", 	(FARPROC*)&COMI_AD_GetVolt},
						{"COMI_US_Start", 	(FARPROC*)&COMI_US_Start},
						{"COMI_US_StartEx", 	(FARPROC*)&COMI_US_StartEx},
						{"COMI_US_Stop", 	(FARPROC*)&COMI_US_Stop},
						{"COMI_US_ChangeScanFreq", 	(FARPROC*)&COMI_US_ChangeScanFreq},
						{"COMI_US_ResetCount", 	(FARPROC*)&COMI_US_ResetCount},
						{"COMI_US_ChangeSampleFreq", 	(FARPROC*)&COMI_US_ChangeSampleFreq},
						{"COMI_US_CurCount", 	(FARPROC*)&COMI_US_CurCount},
						{"COMI_US_SBPos", 	(FARPROC*)&COMI_US_SBPos},
						{"COMI_US_GetBufPtr", 	(FARPROC*)&COMI_US_GetBufPtr},
						{"COMI_US_ReleaseBuf", 	(FARPROC*)&COMI_US_ReleaseBuf},
						{"COMI_US_RetrvOne", 	(FARPROC*)&COMI_US_RetrvOne},
						{"COMI_US_RetrvChannel", 	(FARPROC*)&COMI_US_RetrvChannel},
						{"COMI_US_RetrvBlock", 	(FARPROC*)&COMI_US_RetrvBlock},
						{"COMI_US_FileSaveFirst", 	(FARPROC*)&COMI_US_FileSaveFirst},
						{"COMI_US_FileSaveNext", 	(FARPROC*)&COMI_US_FileSaveNext},
						{"COMI_US_FileSaveStop", 	(FARPROC*)&COMI_US_FileSaveStop},
						{"COMI_US_FileConvert", 	(FARPROC*)&COMI_US_FileConvert},
						{"COMI_US_CheckFileConvert",(FARPROC*)&COMI_US_CheckFileConvert},
						{"COMI_US_CancelFileConvert", 	(FARPROC*)&COMI_US_CancelFileConvert},
						{"COMI_PID_SetParams", 	(FARPROC*)&COMI_PID_SetParams},
						{"COMI_PID_Enable", 	(FARPROC*)&COMI_PID_Enable},
						{"COMI_PID_Disable", 	(FARPROC*)&COMI_PID_Disable},
						{"COMI_DIO_SetUsage", 	(FARPROC*)&COMI_DIO_SetUsage},
						{"COMI_DIO_GetUsage", 	(FARPROC*)&COMI_DIO_GetUsage},
						{"COMI_DI_GetOne", 	(FARPROC*)&COMI_DI_GetOne},
						{"COMI_DI_GetAll", 	(FARPROC*)&COMI_DI_GetAll},
						{"COMI_DI_GetAllEx", 	(FARPROC*)&COMI_DI_GetAllEx},
						{"COMI_DO_PutOne", 	(FARPROC*)&COMI_DO_PutOne},
						{"COMI_DO_PutAll", 	(FARPROC*)&COMI_DO_PutAll},
						{"COMI_DO_PutAllEx", 	(FARPROC*)&COMI_DO_PutAllEx},
						{"COMI_DO_GetOne", 	(FARPROC*)&COMI_DO_GetOne},
						{"COMI_DO_GetAll", 	(FARPROC*)&COMI_DO_GetAll},
						{"COMI_DO_GetAllEx", 	(FARPROC*)&COMI_DO_GetAllEx},
						{"COMI_DA_Out", 	(FARPROC*)&COMI_DA_Out},
						{"COMI_DA_SetRange", 	(FARPROC*)&COMI_DA_SetRange},
						{"COMI_WFM_Start", 	(FARPROC*)&COMI_WFM_Start},
						{"COMI_WFM_RateChange", 	(FARPROC*)&COMI_WFM_RateChange},
						{"COMI_WFM_GetCurPos", 	(FARPROC*)&COMI_WFM_GetCurPos},
						{"COMI_WFM_GetCurLoops", 	(FARPROC*)&COMI_WFM_GetCurLoops},
						{"COMI_WFM_Stop", 	(FARPROC*)&COMI_WFM_Stop},
						{"COMI_SetCounter", 	(FARPROC*)&COMI_SetCounter},
						{"COMI_LoadCount", 	(FARPROC*)&COMI_LoadCount},
						{"COMI_ReadCount", 	(FARPROC*)&COMI_ReadCount},
						{"COMI_ReadCounter32", 	(FARPROC*)&COMI_ReadCounter32},
						{"COMI_ClearCounter32", 	(FARPROC*)&COMI_ClearCounter32},
						{"COMI_FC_SelectGate", 	(FARPROC*)&COMI_FC_SelectGate},
						{"COMI_FC_GateTime", 	(FARPROC*)&COMI_FC_GateTime},
						{"COMI_FC_ReadCount", 	(FARPROC*)&COMI_FC_ReadCount},
						{"COMI_FC_ReadFreq", 	(FARPROC*)&COMI_FC_ReadFreq},
						{"COMI_ENC_Config", 	(FARPROC*)&COMI_ENC_Config},
						{"COMI_ENC_Reset", 	(FARPROC*)&COMI_ENC_Reset},
						{"COMI_ENC_Load", 	(FARPROC*)&COMI_ENC_Load},
						{"COMI_ENC_Read", 	(FARPROC*)&COMI_ENC_Read},
						{"COMI_ENC_ResetZ", 	(FARPROC*)&COMI_ENC_ResetZ},
						{"COMI_ENC_LoadZ", 	(FARPROC*)&COMI_ENC_LoadZ},
						{"COMI_ENC_ReadZ", 	(FARPROC*)&COMI_ENC_ReadZ},
						{"COMI_PG_Start", 	(FARPROC*)&COMI_PG_Start},
						{"COMI_PG_ChangeFreq", 	(FARPROC*)&COMI_PG_ChangeFreq},
						{"COMI_PG_IsActive", 	(FARPROC*)&COMI_PG_IsActive},
						{"COMI_PG_Stop", 	(FARPROC*)&COMI_PG_Stop},
						{"COMI_SD502_SetCounter", 	(FARPROC*)&COMI_SD502_SetCounter},
						{"COMI_SD502_ReadNowCount", 	(FARPROC*)&COMI_SD502_ReadNowCount},
						{"COMI_SD502_ReadOldCount", 	(FARPROC*)&COMI_SD502_ReadOldCount},
						{"COMI_SD502_GetGateState", 	(FARPROC*)&COMI_SD502_GetGateState},
						{"COMI_SD502_GetClkFreq", 	(FARPROC*)&COMI_SD502_GetClkFreq},
						{"COMI_SD502_Clear", 	(FARPROC*)&COMI_SD502_Clear},
						{"COMI_SD502_ClearMulti", 	(FARPROC*)&COMI_SD502_ClearMulti},
						{"COMI_DigitToVolt", 	(FARPROC*)&COMI_DigitToVolt},
						{"COMI_Digit14ToVolt", 	(FARPROC*)&COMI_Digit14ToVolt},
						{"COMI_Digit16ToVolt", 	(FARPROC*)&COMI_Digit16ToVolt},
						{"COMI_LastError", 	(FARPROC*)&COMI_LastError},
						{"COMI_ErrorString", 	(FARPROC*)&COMI_ErrorString},
						{"COMI_GetResources", 	(FARPROC*)&COMI_GetResources},
						{"COMI_WriteIoPortDW", 	(FARPROC*)&COMI_WriteIoPortDW},
						{"COMI_ReadIoPortDW", 	(FARPROC*)&COMI_ReadIoPortDW},
						{"COMI_WriteMemPortDW", 	(FARPROC*)&COMI_WriteMemPortDW},
						{"COMI_ReadMemPortDW", 	(FARPROC*)&COMI_ReadMemPortDW},
						{"COMI_GotoURL", 	(FARPROC*)&COMI_GotoURL},
						{NULL, NULL}
					   };
	LIB_FUNCT *f;
	
	if((hLibrary = LoadLibrary ("Comidll.dll")) == NULL)
    	return FALSE;
	
	f = &Func[0];
	while(f->ptr != NULL)
	{
		*(f->ptr) = GetProcAddress ((HMODULE)hLibrary, f->name);
		f++;
	}
	return TRUE;
}

void COMI_UnloadDll(void)
{
	if(hLibrary)
		FreeLibrary(hLibrary);
}

#ifdef __cplusplus
}
#endif