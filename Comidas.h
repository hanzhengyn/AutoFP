#ifndef __COMIDAS_DLL_H_
#define __COMIDAS_DLL_H_

#include "ComidasCommon.h"

#ifdef __cplusplus
extern "C"{
#endif

// Device ID definition // 
#define COMI_CP101	0xC101
#define COMI_CP102	0xC102
#define COMI_CP201	0xC201
#define COMI_CP202	0xC202
#define COMI_CP301	0xC301
#define COMI_CP302	0xC302
#define COMI_CP401	0xC401
#define COMI_CP402	0xC402
#define COMI_CP501	0xC501
#define COMI_CP502	0xC502
#define COMI_SD101	0xB101
#define COMI_SD102	0xB102
#define COMI_SD103	0xB103
#define COMI_SD104	0xB104
#define COMI_SD201	0xB201
#define COMI_SD202	0xB202
#define COMI_SD203	0xB203
#define COMI_SD209	0xB209
#define COMI_SD301	0xB301
#define COMI_SD302	0xB302
#define COMI_SD401	0xB401
#define COMI_SD402	0xB402
#define COMI_SD403	0xB403
#define COMI_SD404	0xB404
#define COMI_SD414	0xB414
#define COMI_SD424	0xB424
#define COMI_SD501	0xB501
#define COMI_SD502	0xB502
#define COMI_SD503	0xB503

#ifdef COMIDASDLL_EXPORTS
 #define COMIDAS_API _declspec(dllexport)
#else
 #ifdef _COMIDAS_LOADER_C_
  #define EXTERN
 #else
  #define EXTERN extern
 #endif
 BOOL COMI_LoadDll(void);
 void COMI_UnloadDll(void);
 EXTERN HANDLE 	(*COMI_LoadDevice) (COMIDAS_DEVID deviceID, ULONG instance);
 EXTERN void	(*COMI_UnloadDevice) (HANDLE hDevice);
 EXTERN BOOL	(*COMI_GetAvailDevList) (TComiDevList *pDevList);
 EXTERN BOOL	(*COMI_GetDevInfo) (HANDLE hDevice, TComiDevInfo *pDevInfo);
 EXTERN void	(*COMI_Write8402) (HANDLE hDevice, int ch, int addr, int data);
 EXTERN void	(*COMI_WriteEEPR) (HANDLE hDevice, int addr, int data);
 EXTERN int		(*COMI_ReadEEPR) (HANDLE hDevice, int addr);
 //__________ A/D General Functions ________________________________________________//
 EXTERN BOOL	(*COMI_AD_SetRange) (HANDLE hDevice, int ch, float vmin, float vmax);
 EXTERN short	(*COMI_AD_GetDigit) (HANDLE hDevice, int ch);
 EXTERN float	(*COMI_AD_GetVolt) (HANDLE hDevice, int ch);
 //__________ A/D Unlimited Scan Functions _________________________________//
 EXTERN long	(*COMI_US_Start) (HANDLE hDevice, int numCh, int *chanList, UINT scanFreq,
			UINT msb, int trsMethod);
 EXTERN long	(*COMI_US_StartEx) (HANDLE hDevice, UINT dwScanFreq, UINT nFrameSize, UINT nBufSizeGain);
 EXTERN BOOL	(*COMI_US_Stop) (HANDLE hDevice, BOOL bReleaseBuf);
 EXTERN BOOL	(*COMI_US_SetPauseAtFull) (HANDLE hDevice, BOOL bPauseAtFull);
 EXTERN BOOL	(*COMI_US_Resume) (HANDLE hDevice);
 EXTERN long	(*COMI_US_ChangeScanFreq) (HANDLE hDevice, UINT dwScanFreq);
 EXTERN void	(*COMI_US_ResetCount) (HANDLE hDevice);
 EXTERN void	(*COMI_US_ChangeSampleFreq) (HANDLE hDevice, ULONG dwSampleFreq);
 EXTERN ULONG	(*COMI_US_CurCount) (HANDLE hDevice);
 EXTERN UINT	(*COMI_US_SBPos) (HANDLE hDevice, int chOrder, ULONG scanCount);
 EXTERN short*	(*COMI_US_GetBufPtr) (HANDLE hDevice);
 EXTERN BOOL	(*COMI_US_ReleaseBuf) (HANDLE hDevice);
 EXTERN short	(*COMI_US_RetrvOne) (HANDLE hDevice, int chOrder, ULONG scanCount);
 EXTERN ULONG	(*COMI_US_RetrvChannel) (HANDLE hDevice, int chOrder, ULONG startCount, 
			int maxNumData, void *pDestBuf, TComiVarType VarType);
 EXTERN UINT	(*COMI_US_RetrvBlock) (HANDLE hDevice, UINT startCount, int maxNumScan, 
			void *pDestBuf, TComiVarType VarType);
 EXTERN BOOL	(*COMI_US_FileSaveFirst) (HANDLE hDevice, char *szFilePath, BOOL bIsFromStart);
 EXTERN ULONG	(*COMI_US_FileSaveNext) (HANDLE hDevice);
 EXTERN BOOL	(*COMI_US_FileSaveStop) (HANDLE hDevice);
 EXTERN void	(*COMI_US_FileConvert) (char *szBinFilePath, char *szTextFilePath, ULONG nMaxDataRow);
 EXTERN float	(*COMI_US_CheckFileConvert) (void);
 EXTERN void	(*COMI_US_CancelFileConvert) (void);
 //___________ PID Functions _______________________________________________//
 EXTERN BOOL	(*COMI_PID_Enable) (HANDLE hDevice); 
 EXTERN BOOL	(*COMI_PID_SetParams) (HANDLE hDevice, int nNumCtrls, TPidParams *pPidParams); 
 EXTERN BOOL	(*COMI_PID_Disable) (HANDLE hDevice); 
 //___________ DIO Common __________________________________________________//
 EXTERN void	(*COMI_DIO_SetUsage) (HANDLE hDevice, int usage);
 EXTERN int		(*COMI_DIO_GetUsage) (HANDLE hDevice);
 //__________ D/I Functions ________________________________________________//
 EXTERN int		(*COMI_DI_GetOne) (HANDLE hDevice, int ch);
 EXTERN DWORD	(*COMI_DI_GetAll) (HANDLE hDevice);
 EXTERN DWORD	(*COMI_DI_GetAllEx) (HANDLE hDevice, int nGroupIdx);
 //__________ D/O Functions ________________________________________________//
 EXTERN BOOL	(*COMI_DO_PutOne) (HANDLE hDevice, int ch, int status);
 EXTERN BOOL	(*COMI_DO_PutAll) (HANDLE hDevice, DWORD dwStatuses);
 EXTERN void	(*COMI_DO_PutAllEx) (HANDLE hDevice, int nGroupIdx, DWORD dwStatuses);
 EXTERN int		(*COMI_DO_GetOne) (HANDLE hDevice, int ch);
 EXTERN DWORD	(*COMI_DO_GetAll) (HANDLE hDevice);
 EXTERN DWORD	(*COMI_DO_GetAllEx) (HANDLE hDevice, int nGroupIdx);
 //__________ D/A Functions ________________________________________________//
 EXTERN BOOL	(*COMI_DA_Out) (HANDLE hDevice, int ch, float volt);
 EXTERN void	(*COMI_DA_SetRange) (HANDLE hDevice, int ch, int VMin, int VMax);
 EXTERN long	(*COMI_WFM_Start) (HANDLE hDevice, int ch, float *pDataBuffer, 
						UINT nNumData, UINT nPPS, int nMaxLoops);
 EXTERN long	(*COMI_WFM_RateChange) (HANDLE hDevice, int ch, ULONG nPPS);
 EXTERN long	(*COMI_WFM_GetCurPos) (HANDLE hDevice, int ch);
 EXTERN long	(*COMI_WFM_GetCurLoops) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_WFM_Stop) (HANDLE hDevice, int ch);
 //__________ Counter Functions ____________________________________________//
 EXTERN void	(*COMI_SetCounter) (HANDLE hDevice, int ch, int rw, int op, int bcd_bin, USHORT load_value);
 EXTERN void	(*COMI_LoadCount) (HANDLE hDevice, int ch, USHORT load_value);
 EXTERN USHORT	(*COMI_ReadCount) (HANDLE hDevice, int ch);
 EXTERN ULONG 	(*COMI_ReadCounter32) (HANDLE hDevice, int ch);
 EXTERN void 	(*COMI_ClearCounter32) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_FC_SelectGate) (HANDLE hDevice, int ch, int nGateIndex);
 EXTERN ULONG	(*COMI_FC_GateTime) (HANDLE hDevice, int ch);
 EXTERN ULONG	(*COMI_FC_ReadCount) (HANDLE hDevice, int ch);
 EXTERN ULONG	(*COMI_FC_ReadFreq) (HANDLE hDevice, int ch);
 
 EXTERN void	(*COMI_ENC_Config) (HANDLE hDevice, int ch, int mode, BOOL bResetByZ);
 EXTERN void	(*COMI_ENC_Reset) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_ENC_Load) (HANDLE hDevice, int ch, long Count);
 EXTERN long	(*COMI_ENC_Read) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_ENC_ResetZ) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_ENC_LoadZ) (HANDLE hDevice, int ch, short count);
 EXTERN short	(*COMI_ENC_ReadZ) (HANDLE hDevice, int ch);
 EXTERN double	(*COMI_PG_Start) (HANDLE hDevice, int ch, double freq, UINT num_pulses);
 EXTERN double	(*COMI_PG_ChangeFreq) (HANDLE hDevice, int ch, double freq);
 EXTERN BOOL	(*COMI_PG_IsActive) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_PG_Stop) (HANDLE hDevice, int ch);

 EXTERN void	(*COMI_SD502_SetCounter) (HANDLE hDevice, int ch, int nMode,
				UINT nClkSource);
 EXTERN ULONG	(*COMI_SD502_ReadNowCount) (HANDLE hDevice, int ch);
 EXTERN ULONG	(*COMI_SD502_ReadOldCount) (HANDLE hDevice, int ch);
 EXTERN BOOL	(*COMI_SD502_GetGateState) (HANDLE hDevice, int ch);
 EXTERN double	(*COMI_SD502_GetClkFreq) (int nClkSrcIdx);
 EXTERN void	(*COMI_SD502_Clear) (HANDLE hDevice, int ch);
 EXTERN void	(*COMI_SD502_ClearMulti) (HANDLE hDevice, ULONG dwCtrlBits);

 //__________ Utility Functions ____________________________________________//
 EXTERN float	(*COMI_DigitToVolt) (short digit, float vmin, float vmax);
 EXTERN float	(*COMI_Digit14ToVolt) (short digit, float vmin, float vmax);
 EXTERN float	(*COMI_Digit16ToVolt) (short digit, float vmin, float vmax);
 EXTERN int		(*COMI_LastError) (void);
 EXTERN const char* (*COMI_ErrorString) (int nErrCode);
 EXTERN void	(*COMI_GetResources) (HANDLE hDevice, PULONG pdwIntVect, PULONG pdwIoPorts,
			 int nNumPorts, PULONG pdwMemPorts, int nNumMemPorts);
 EXTERN void	(*COMI_WriteIoPortDW) (HANDLE hDevice, ULONG dwPortBase, ULONG nOffset, DWORD dwOutVal);
 EXTERN DWORD	(*COMI_ReadIoPortDW) (HANDLE hDevice, ULONG dwPortBase, ULONG nOffset);
 EXTERN void	(*COMI_WriteMemPortDW) (HANDLE hDevice, ULONG dwPortBase, ULONG nOffset, DWORD dwOutVal);
 EXTERN DWORD	(*COMI_ReadMemPortDW) (HANDLE hDevice, ULONG dwPortBase, ULONG nOffset);
 EXTERN void	(*COMI_GotoURL) (const char *szUrl, BOOL bMaximize);
#undef extern
#endif

#ifdef __cplusplus
}
#endif

#endif
