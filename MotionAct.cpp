#include "stdafx.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include <windows.h>
#include <iostream>
#include "LogFile.h"
#include "BurningPort.h"
#include "advmotdrv.h"
#include "General.h"
#include "AdvMotApi.h"
#include "IoSd404.h"
#include "MotionAct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HAND	gm_Devhand;//�忨�豸���
HAND	gm_Axishand[4];//����
HAND    gm_GpHand;//����

//#define LIMIT_Z 500
HANDLE g_hEventXHome, g_hEventYHome;
#if BOARD_TYPE == COMI
BOOL OpenMotionBoard()
{
	OpenIOBoard();

	if (!cmmLoadDll())
	{
		AfxMessageBox("Can't load CMMSDK Library(DLL)",  MB_OK | MB_ICONERROR);
		return TRUE;
	}

	long nNumAxes = 0;
	if (cmmGnDeviceLoad(cmTRUE, &nNumAxes) != cmERR_NONE)
	{
		cmmErrShowLast(NULL);
		return FALSE;
	}

#define CME_FILE_NAME "Default.cme2"
	char szCmeFilePath[MAX_PATH], szSystemDir[MAX_PATH];
	strcpy(szSystemDir, gm_strSetupPath);
	sprintf(szCmeFilePath, "%s\\%s", szSystemDir, CME_FILE_NAME);

	long nNumAxes_install = 0, nNumAxes_define = 0;
	if (cmmGnInitFromFile(szCmeFilePath) != cmERR_NONE)
	{
		cmmErrShowLast(NULL);
		return FALSE;
	}
	else
	{
		cmmAdvGetNumAvailAxes(&nNumAxes_install);
		cmmAdvGetNumDefinedAxes(&nNumAxes_define);
		if (nNumAxes_define != nNumAxes_install)
		{
			CString str;
			str.Format("nNumAxes_install:%d!=nNumAxes_define:%d", nNumAxes_install, nNumAxes_define);
			MessageBox(NULL, "�����������\r\n" + str, "Warning", MB_OK | MB_ICONWARNING);
			return FALSE;
		}
	}
	//��ʼ���忨����
	cmmCfgSetOutMode(XAXIS, cmOMODE_CWCCW0);//
	cmmCfgSetOutMode(YAXIS, cmOMODE_CWCCW0);
	cmmCfgSetOutMode(ZAXIS, cmOMODE_CWCCW0);

	cmmCfgSetOutMode(RAXIS_1, cmOMODE_PDIR2);
	cmmCfgSetOutMode(RAXIS_2, cmOMODE_PDIR2);
	cmmCfgSetOutMode(RAXIS_3, cmOMODE_PDIR2);
	cmmCfgSetOutMode(RAXIS_4, cmOMODE_PDIR2);

	cmmHomeSetConfig(XAXIS, MODE2_Lmt, 0, 200, 0);
	cmmHomeSetConfig(YAXIS, MODE2_Lmt, 0, 200, 0);
	cmmHomeSetConfig(ZAXIS, MODE2_Lmt, 0, 200, 0);

	long home_speed = 10000;//��ԭ�ٶȣ�8000����һȦ��20mm
	cmmHomeSetSpeedPattern(XAXIS, cmSMODE_S, home_speed, home_speed, home_speed, home_speed);
	cmmHomeSetSpeedPattern(YAXIS, cmSMODE_S, home_speed, home_speed, home_speed, home_speed);
	cmmHomeSetSpeedPattern(ZAXIS, cmSMODE_S, home_speed, home_speed, home_speed, home_speed);

	for (int i = 0; i < nNumAxes_install; i++)
	{
		cmmGnSetServoOn(i, cmTRUE);
		Sleep(100);
	}
	return TRUE;
}

BOOL GoHome()
{
	cmmCfgSetSoftLimit(cmX1, false, -10, 9999999);
	cmmCfgSetSoftLimit(cmY1, false, -10, 9999999);
	cmmCfgSetSoftLimit(cmZ1, false, -10, 9999999);
	Sleep(100);

	int almx, almy, almz;
	long dwTemp;
	cmmStReadMioStatuses(cmX1, &dwTemp);
	almx = (dwTemp >> cmIOST_ALM) & 0x1;

	cmmStReadMioStatuses(cmY1, &dwTemp);
	almy = (dwTemp >> cmIOST_ALM) & 0x1;

	cmmStReadMioStatuses(cmZ1, &dwTemp);
	almz = (dwTemp >> cmIOST_ALM) & 0x1;
	if (almz || almx || almy)
	{
		AfxMessageBox("���ᶼ�����������Ƿ�ͣ��ť������!");
		return FALSE;
	}

	for (int i = 0; i < 3; i++)
	{
		cmmHomeSetConfig(i, 1, 0, 800, 0);//�������ݾ���Ϊ800
		//cmmHomeSetSpeedPattern(i, cmSMODE_S, 3000, 1000, 1000, 3000);
	}

	if (cmmHomeMove(cmZ1, cmDIR_N, cmFALSE) != cmERR_NONE)
	{
		AfxMessageBox("z���ԭ�쳣��");
		return FALSE;
	}
	long nAxis[2] = { cmX1, cmY1 };
	long nDirList[2] = { cmDIR_N, cmDIR_N };

	for (int i = 0; i < 4; i++)
	{
		if (gm_bNozzleUsed[i])
		{
			if (!CheckNozzleUpStatus(i))
			{
				AfxMessageBox("����δ����ԭ��λ�ã���ԭʧ�ܣ��������������Ƿ�����������");
				return FALSE;
			}
		}
	}

	if (cmmHomeMoveAll(2, nAxis, nDirList, cmFALSE) != cmERR_NONE)
	{
		cmmErrShowLast(NULL);
		return FALSE;
	}
	return TRUE;
}
double GetPosition(long axis)
{
	double position = 0;
	if (cmmStGetPosition(axis, cmCNT_COMM, &position) != cmERR_NONE)
	{
		cmmErrShowLast(NULL);
	}
	return position;
}

BOOL GetPosition(long axis,double &position)
{
	if (cmmStGetPosition(axis, cmCNT_COMM, &position) != cmERR_NONE)
	{
		cmmErrShowLast(NULL);
		return FALSE;
	}
	return TRUE;
}

void SetSingleSpeed(LONG axis, double dv, double ac, double del)
{
	cmmCfgSetSpeedPattern(axis, cmSMODE_S, dv, ac, del);
	cmmSxSetSpeedRatio(axis, cmSMODE_KEEP, 100, 100, 100);
}
//����z����ٶ�
void SetAxisSpeed(long axis, int speedtype)
{
	//double speedratio = (speedtype) * 0.5;
	//SetSingleSpeed(axis, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 5000, speedratio*XGUIDE * 5000);
	double speedratio = (speedtype) * 2;
	SetSingleSpeed(axis, speedratio*XGUIDE * 100, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 1000);
}
//����xy����ٶ�
void SetAxisSpeed(long axis, double moverange, int speedtype)
{
	double speedratio = (speedtype) * 2;
	if (moverange < XGUIDE * 1200)//1200mm
		SetSingleSpeed(axis, speedratio*XGUIDE * 100, speedratio*XGUIDE * 500, speedratio*XGUIDE * 500);
	else if (moverange >= XGUIDE * 1200)//����������1200mm,˵���˶���������
	{
		SetSingleSpeed(axis, speedratio*XGUIDE * 1, speedratio*XGUIDE * 30, speedratio*XGUIDE * 30);
		AfxMessageBox("�˶����볬�����ޣ������¼�����꣬�����Ƿ��˶����ϡ�");
	}
}

UINT XYMove(double xnextposition, double ynextposition)
{
	long XIsDone = 0, YIsDone = 0, ZIsDone = 0;
	cmmSxIsDone(cmX1, &XIsDone);
	cmmSxIsDone(cmY1, &YIsDone);
	cmmSxIsDone(cmZ1, &ZIsDone);
	if (!XIsDone | !YIsDone | !YIsDone)
		return ERROR_XYMOVE; 

//#if SINGLE_AXIS
//	int i = 0;
//	BOOL bZCome = ZComeBack();
//	while (!bZCome)
//	{
//		if (i++ > 1)
//		{
//			AfxMessageBox("z���ԭ�쳣");
//			return ERROR_XYMOVE_ZCOMEBACK;
//		}
//		Sleep(100);
//		bZCome = ZComeBack();
//	}
//	double zposition = GetPosition(cmZ1);
//	if (zposition > ZGUIDE * 3) 
//	{
//		return ERROR_XYMOVE_ZCOMEBACK;
//	}
//#endif // SINGLE_AXIS

	for (int i = 0; i < 4; i++)
	{
		if (gm_bNozzleUsed[i])//���������ʹ�ã�������λ�ź�δ��⵽���򷵻�FALSE
		{
			if (!CheckNozzleUpStatus(i))
			{
				SetNozzleDownOff(i);
				Sleep(200);
				if (!CheckNozzleUpStatus(i))
				{
					return ERROR_XYMOVE_NOZZLE_BACK;
				}
			}
		}
	}

#define MAP_0 0	//Ⱥ����
	cmmIxMapAxes(MAP_0, cmX1_MASK | cmY1_MASK, 0);//�����岹��

	long IxDone = 0;
	cmmIxIsDone(0, &IxDone);
	if (!IxDone)
		return ERROR_XYMOVE;

	double xcurrentposition = 0, ycurrentposition = 0, MaxMoverange = 0;
	GetPosition(cmX1, xcurrentposition);
	GetPosition(cmY1, ycurrentposition);

	double xmoverange = xnextposition - xcurrentposition;
	double ymoverange = ynextposition - ycurrentposition;

	//if (abs(xmoverange) <= abs(ymoverange))
	//	MaxMoverange = abs(ymoverange);
	//else
	//	MaxMoverange = abs(xmoverange);

	//int speedType = gm_uInfo.xySpeed;
	////change the speed accroding work speed type
	//SetAxisSpeed(cmX1, MaxMoverange, speedType);
	//SetAxisSpeed(cmY1, MaxMoverange, speedType);
	cmmIxSetSpeedPattern(MAP_0, cmFALSE, cmSMODE_S, 100, 100, 100);
	double fDistList[2] = { xmoverange, ymoverange };
	
	//if (cmmIxArcP(MAP_0,) != cmERR_NONE)
	if (cmmIxLine(MAP_0, fDistList, cmFALSE) != cmERR_NONE)
	{
		StopMotion();
		cmmErrShowLast(NULL);
		return ERROR_IXLINE;
	}
	return SUCCESS_BURNING;
}

BOOL SxMove(long axis, double nextposition)
{
	long XIsDone = 0;
	cmmSxIsDone(axis, &XIsDone);
	if (!XIsDone)
		return FALSE;

	float rate = 0.5;
	SetSingleSpeed(axis, rate*XGUIDE * 1000, rate*XGUIDE * 3000, rate*XGUIDE * 3000);

	if (cmmSxMove(axis, nextposition, cmFALSE) != cmERR_NONE)
	{
		cmmSxStopEmg(axis);
		cmmErrShowLast(NULL);
		return FALSE;
	}

	return TRUE;
}

BOOL SzMove(double nextposition)
{
	long XIsDone = 0;
	cmmSxIsDone(ZAXIS, &XIsDone);
	if (!XIsDone)
		return FALSE;

	int ZspeedType = gm_uInfo.zSpeed;
	SetAxisSpeed(ZAXIS, ZspeedType);
	double zcurrentposition = 0;
	GetPosition(ZAXIS, zcurrentposition);
	double moverange = nextposition - zcurrentposition;

	if (cmmSxMove(ZAXIS, moverange, cmFALSE) != cmERR_NONE)
	{
		//cmmErrShowLast(NULL);
		return FALSE;
	}
	return TRUE;
}
BOOL ZComeBack()
{
	long XIsDone = 0;
	cmmSxIsDone(ZAXIS, &XIsDone);
	if (!XIsDone)
		return FALSE;

	int ZspeedType = gm_uInfo.zSpeed;
	SetAxisSpeed(ZAXIS, ZspeedType);

	double currentposition = 0;
	double moverange = 0;
	GetPosition(cmZ1, currentposition);
	moverange = -currentposition;
	if (cmmSxMove(ZAXIS, moverange, cmFALSE) != cmERR_NONE)
	{
		return FALSE;
	}
	return TRUE;
}
//��ת��angle��λΪ����
BOOL SrMove(double angle, int axis)
{
	if (!SINGLE_AXIS)
		axis += RAXIS_1;
	else
		axis = RAXIS_1;//���Ῠ����������һ����ת��

	long XIsDone = 0;
	cmmSxIsDone(axis, &XIsDone);
	if (!XIsDone)
		return FALSE;

	float rate = 4;
	double offset;
	offset = angle * RGUIDE / PI / 2;
	SetSingleSpeed(axis, RGUIDE * rate, 5*RGUIDE * rate, 5*RGUIDE * rate);

	if (cmmSxMove(axis, offset, cmFALSE) != cmERR_NONE)
		return FALSE;
	return TRUE;
}

void StopMotion()
{
	long nAxisList[4] = { cmX1, cmY1, cmZ1, cmU1 };
	long nNumAxis;
	cmmAdvGetNumDefinedAxes(&nNumAxis);
	cmmMxStopEmg(nNumAxis, nAxisList);
}

BOOL MeasureHeight(double &height)
{
	if (!CheckPositiveMeter())
	{
		AfxMessageBox("��ѹ�ϵͣ�δ����Ԥ�谲ȫֵ!\r\n������ѹ��·������ѹ�������Ƿ�������");
		return FALSE;
	}
	long state=0;
	cmmCfgSetSpeedPattern(cmZ1, cmSMODE_T, 10000, 10000, 10000);
	SetNozzleDownOn(NOZZLE_0);
	SetNozzleVaccumOn(NOZZLE_0);
	cmmSxVMoveStart(cmZ1, cmDIR_P);
	state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	DWORD dwStart = GetTickCount();
	int nEp = 0;
	while (!state)
	{
		nEp = GetTickCount() - dwStart;
		if (nEp > gm_uInfo.nSocketTime)
		{
			cmmSxStopEmg(cmZ1);
			SetNozzleVaccumOff(NOZZLE_0);
			SetNozzleDownOff(NOZZLE_0);
			ZComeBack();
			AfxMessageBox("��߳�ʱ�����ʵ����Ӳ��ʱ�䡣");
			return FALSE;
		}
		state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	}
	cmmSxStopEmg(cmZ1);
	height = GetPosition(cmZ1);
	SetNozzleVaccumOff(NOZZLE_0);
	SetNozzleBlowOn(NOZZLE_0);
	Sleep(100);
	SetNozzleBlowOff(NOZZLE_0);
	SetNozzleDownOff(NOZZLE_0);
	ZComeBack();
	return TRUE;
}
void CloseMotionCard()
{
	CloseIOBoard();
	cmmGnDeviceUnload();
	cmmUnloadDll();
}

#elif BOARD_TYPE == ADVANTECH //�л��忨

BOOL OpenMotionBoard()
{
	gm_Devhand = 0;
	memset(gm_Axishand, 0, sizeof(gm_Axishand));
	gm_GpHand = NULL;
	DEVLIST avaDevs[MAX_DEVICES];
	U32		Result = 0;
	ULONG	deviceNumber = 0;
	CString strTemp;
	Result = Acm_GetAvailableDevs(avaDevs, MAX_DEVICES, &deviceNumber);
	if (Result != SUCCESS)
	{
		strTemp.Format("Get Device Numbers Failed With Error Code: %x", Result);
		AfxMessageBox(strTemp);
		return FALSE;
	}
	DWORD dwDevNum = avaDevs[0].dwDeviceNum;

	Result = Acm_DevOpen(dwDevNum, &gm_Devhand);
	if (Result != SUCCESS)
	{
		strTemp.Format("Open Device Failed With Error Code:%x", Result);
		AfxMessageBox(strTemp);
		return FALSE;
	}
	for (int i = 0; i < 4; i++)
	{
		Acm_AxResetError(gm_Axishand[i]);
	}

	for (int AxisNumber = 0; AxisNumber < 4; AxisNumber++)
	{
		//Open every Axis and get the each Axis Handle
		//And Initial property for each Axis 		
		//Open Axis 
		Result = Acm_AxOpen(gm_Devhand, (USHORT)AxisNumber, &gm_Axishand[AxisNumber]);
		if (Result != SUCCESS)
		{
			strTemp.Format("Open Axis Failed With Error Code:%x", Result);
			AfxMessageBox(strTemp);
			return FALSE;
		}
		//Reset Command Counter
		Acm_AxSetCmdPosition(gm_Axishand[AxisNumber], 0);
		//Reset Actual Counter
		Acm_AxSetActualPosition(gm_Axishand[AxisNumber], 0);
	}
	for (int i = 0; i < 4; i++)
	{
		Result = Acm_AxSetSvOn(gm_Axishand[i], 1);
		if (Result != SUCCESS)
		{
			strTemp.Format("Open Servo Failed With Error Code:%x", Result);
			AfxMessageBox(strTemp);
			return FALSE;
		}
		Sleep(100);
	}

	Result = Acm_GpAddAxis(&gm_GpHand, gm_Axishand[0]);
	if (Result != SUCCESS)
	{
		strTemp.Format("Group add axis-0 failed with error code:%x", Result);
		AfxMessageBox(strTemp);
		return FALSE;
	}
	Result = Acm_GpAddAxis(&gm_GpHand, gm_Axishand[1]);
	if (Result != SUCCESS)
	{
		strTemp.Format("Group add axis-1 failed with error code:%x", Result);
		AfxMessageBox(strTemp);
		return FALSE;
	}
#define CME_FILE_NAME "Default.cfg"
	char szCmeFilePath[MAX_PATH], szSystemDir[MAX_PATH];
	strcpy(szSystemDir, gm_strSetupPath);
	sprintf(szCmeFilePath, "%s\\%s", szSystemDir, CME_FILE_NAME);

	Result = Acm_DevLoadConfig(gm_Devhand, szCmeFilePath);
	if (Result != SUCCESS)
	{
		CString strString;
		strString.Format("Load Config Failed With Error Code: %x", Result);
		AfxMessageBox(strString);
		return FALSE;
	}
	return TRUE;
}

BOOL SingleHome(long axis)
{
	int nRaio = 1;
	if (ZAXIS == axis)
	{
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeVelLow, 2000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeVelHigh, 8000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeAcc, 10000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeDec, 10000 * nRaio);

		Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelLow, 2000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelHigh, 8000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxAcc, 10000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxDec, 10000 * nRaio);
	}
	else
	{
		nRaio = 2;
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeVelLow, 2000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeVelHigh, 8000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeAcc, 10000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxHomeDec, 10000 * nRaio);

		Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelLow, 2000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelHigh, 8000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxAcc, 10000 * nRaio);
		Acm_SetF64Property(gm_Axishand[axis], PAR_AxDec, 10000 * nRaio);
	}

	Acm_AxHome(gm_Axishand[axis], MODE7_AbsSearch, Home_PosDir);
	return TRUE;
}
BOOL GoHome()
{
	for (int i = 0; i < 4; i++)
	{
		Acm_AxResetError(gm_Axishand[i]);
	}
	BOOL bOK = FALSE;
	U32 Result, ResultX, ResultY;
	U32 lState, lStateX, lStateY;
	SingleHome(ZAXIS);

	U16 nZState;
	Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	int n = 0;
	while (STA_AX_READY != nZState)
	{
		if (n++ > 100)
		{
			MessageBox(NULL, "Z���ԭ�쳣��", "��ԭ", MB_ICONINFORMATION);
			return FALSE;
		}
		Sleep(100);
		Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	}

	SingleHome(XAXIS);
	SingleHome(YAXIS);

	U16 nXState, nYState;
	Acm_AxGetState(gm_Axishand[XAXIS], &nXState);
	Acm_AxGetState(gm_Axishand[YAXIS], &nYState);
	n = 0;
	while (STA_AX_READY != nXState || STA_AX_READY != nYState)
	{
		if (n++ > 100)
		{
			CString str = "";
			if (STA_AX_READY != nXState)
				str += "x���ԭ�쳣\r\n";
			else
				str += "y���ԭ�쳣\r\n";
			MessageBox(NULL, str, "��ԭ", MB_ICONINFORMATION);
			return FALSE;
		}
		Sleep(500);
		Acm_AxGetState(gm_Axishand[XAXIS], &nXState);
		Acm_AxGetState(gm_Axishand[YAXIS], &nYState);
	}

	//bOK = FALSE;
	//while (!bOK)
	//{
	//	ResultX = Acm_AxGetMotionStatus(gm_Axishand[XAXIS], &lStateX);
	//	ResultY = Acm_AxGetMotionStatus(gm_Axishand[YAXIS], &lStateY);
	//	//if (ResultX == SUCCESS && ResultY == SUCCESS)
	//	{
	//		if (lStateX && lStateY)
	//		{
	//			bOK = TRUE;
	//		}
	//	}
	//}
	return TRUE;
}
double GetPosition(long axis)
{
	double position = 0;
	U32 Result=0;
	F64 ActPos=0;
	Result = Acm_AxGetCmdPosition(gm_Axishand[axis], &ActPos);
	if (Result == SUCCESS)
	{
		position = ActPos;
	}
	return position;
}

BOOL GetPosition(long axis, double &position)
{
	U32 Result = 0;
	Result = Acm_AxGetCmdPosition(gm_Axishand[axis], &position);
	if (Result != SUCCESS)
	{
		return FALSE;
	}
	return TRUE;
}

void SetSingleSpeed(LONG axis, double dv, double ac, double del)
{
	Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelLow, 200);
	Acm_SetF64Property(gm_Axishand[axis], PAR_AxVelHigh, dv);
	Acm_SetF64Property(gm_Axishand[axis], PAR_AxAcc, ac);
	Acm_SetF64Property(gm_Axishand[axis], PAR_AxDec, del);
	Acm_SetF64Property(gm_Axishand[axis], PAR_AxJerk, 0.0);
}
//����z����ٶ�
void SetAxisSpeed(long axis, int speedtype)
{
	double speedratio = (speedtype) * 0.5;
	SetSingleSpeed(axis, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 3000, speedratio*XGUIDE * 3000);
}
BOOL SetLinearSpeed(double AxVelHigh, double AxAcc, double AxDec)
{

	Acm_SetF64Property(gm_GpHand, PAR_GpVelLow, 200);//PAR_GpVelLow
	Acm_SetF64Property(gm_GpHand, PAR_GpVelHigh, AxVelHigh);
	Acm_SetF64Property(gm_GpHand, PAR_GpAcc, AxAcc);
	Acm_SetF64Property(gm_GpHand, PAR_GpDec, AxDec);
	Acm_SetF64Property(gm_GpHand, PAR_GpJerk, 0.0);
	return TRUE;
}
//����xy����ٶ�
void SetAxisSpeed(long axis, double moverange, int speedtype)
{
	double speedratio = (speedtype) * 2;
	SetLinearSpeed(speedratio*XGUIDE * 140, speedratio*XGUIDE * 450, speedratio*XGUIDE * 450);
	if (moverange < XGUIDE * 1200)//1200mm
		SetSingleSpeed(axis, speedratio*XGUIDE * 140, speedratio*XGUIDE * 450, speedratio*XGUIDE * 450);
	else if (moverange >= XGUIDE * 1200)//����������1200mm,˵���˶���������
	{
		SetSingleSpeed(axis, speedratio*XGUIDE * 1, speedratio*XGUIDE * 30, speedratio*XGUIDE * 30);
		AfxMessageBox("�˶����볬�����ޣ������¼�����꣬�����Ƿ��˶����ϡ�");
	}
}
//���ò岹�ٶ�

UINT XYMove(double xnextposition, double ynextposition)
{
	U16 nGpState, nXState, nYState, nZState;
	//Acm_AxGetState(gm_Axishand[XAXIS], &nXState);
	//if (STA_AX_READY != nXState)
	//{
	//	return ERROR_XYMOVE;
	//}
	//Acm_AxGetState(gm_Axishand[YAXIS], &nYState);
	//if (STA_AX_READY != nYState)
	//{
	//	return ERROR_XYMOVE;
	//}
	Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	if (STA_AX_READY != nZState)
	{
		return ERROR_XYMOVE;
	}
	//Acm_GpGetState(gm_GpHand, &nGpState);
	//if (STA_GP_READY != nGpState)
	//{
	//	return ERROR_XYMOVE;
	//}
	double height = GetPosition(ZAXIS);
	if (height > 500)//��ֹz��������ʱ��Χ�ƶ�xy��
	{
		return ERROR_XYMOVE;
	}

	for (int i = 0; i < 4; i++)
	{
		if (gm_bNozzleUsed[i])//���������ʹ�ã�������λ�ź�δ��⵽���򷵻�FALSE
		{
			if (!CheckNozzleUpStatus(i))
			{
				SetNozzleDownOff(i);
				Sleep(200);
				if (!CheckNozzleUpStatus(i))
				{
					return ERROR_XYMOVE_NOZZLE_BACK;
				}
			}
		}
	}

	double xcurrentposition, ycurrentposition, MaxMoverange;
	xcurrentposition = GetPosition(XAXIS);
	ycurrentposition = GetPosition(YAXIS);
	double xmoverange = xnextposition - xcurrentposition;
	double ymoverange = ynextposition - ycurrentposition;
	if (abs(xmoverange) <= abs(ymoverange))
		MaxMoverange = abs(ymoverange);
	else
		MaxMoverange = abs(xmoverange);

	double fDistList[2] = { xmoverange,ymoverange };
	U32 ArrayElementCnt = 2;
	U32 Result = Acm_GpMoveLinearRel(gm_GpHand, fDistList, &ArrayElementCnt);

	if (Result != SUCCESS)
	{
		if (!(Result & Warning))
		{
			CString strTemp;
			strTemp.Format("�ƶ����󣬴������: %x", Result);
			//AfxMessageBox(strTemp);
			return ERROR_XYMOVE;
		}
	}
	BOOL bOK = FALSE;
	U16 lState;
	while (!bOK)
	{
		Result = Acm_GpGetState(gm_GpHand, &lState);
		if (Result == SUCCESS)
		{
			if (lState & 1)
			{
				bOK = TRUE;
			}
		}
	}
	return SUCCESS_BURNING;
}
//�ƶ���Ծ���
BOOL SxMove(long axis, double nextposition)
{
	U16 nXState;
	Acm_AxGetState(gm_Axishand[axis], &nXState);
	if (STA_AX_READY != nXState)
	{
		return ERROR_XYMOVE;
	}
	
	float rate = 0.5;
	SetSingleSpeed(axis, rate*XGUIDE * 1000, rate*XGUIDE * 3000, rate*XGUIDE * 3000);

	U32 Result = Acm_AxMoveRel(gm_Axishand[axis], nextposition);
	if (Result != SUCCESS)
		return FALSE;
	return TRUE;
}
//�ƶ����Ծ���
BOOL SzMove(double nextposition)
{
	U16 nXState;
	Acm_AxGetState(gm_Axishand[ZAXIS], &nXState);
	if (STA_AX_READY != nXState)
	{
		return ERROR_XYMOVE;
	}

	float rate = 0.5;
	SetSingleSpeed(ZAXIS, rate*XGUIDE * 1000, rate*XGUIDE * 3000, rate*XGUIDE * 3000);

	double currentposition;
	currentposition = GetPosition(ZAXIS);
	double moverange = nextposition - currentposition;
	U32 Result = Acm_AxMoveRel(gm_Axishand[ZAXIS], moverange);
	U16 nZState;
	Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	int n = 0;
	while (STA_AX_READY != nZState)
	{
		if (n++ > 500)
			return FALSE;
		Sleep(10);
		Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	}
	return TRUE;
}
BOOL ZComeBack()
{
	int ZspeedType = gm_uInfo.zSpeed;
	SetAxisSpeed(ZAXIS, ZspeedType);

	double currentposition = 0;
	double moverange = 0;
	GetPosition(ZAXIS, currentposition);
	moverange = -currentposition;
	U32 Result = Acm_AxMoveRel(gm_Axishand[ZAXIS], moverange);
	if (Result != SUCCESS)
		return FALSE;
	U16 nZState;
	Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	int n = 0;
	while (STA_AX_READY != nZState)
	{
		if (n++ > 500)
			return FALSE;
		Sleep(10);
		Acm_AxGetState(gm_Axishand[ZAXIS], &nZState);
	}

	return TRUE;
}
//��ת��angle��λΪ����
BOOL SrMove(double angle, int axis)
{
	float rate = 4;
	double offset;
	offset = angle * RGUIDE / PI / 2;
	SetSingleSpeed(axis, RGUIDE * rate, RGUIDE * rate * 5, RGUIDE * rate * 5);

	U32 Result = Acm_AxMoveRel(gm_Axishand[3], offset);
	if (Result != SUCCESS)
		return FALSE;
	return TRUE;
}

void StopMotion()
{
	Acm_AxStopEmg(gm_Axishand[XAXIS]);
	Acm_AxStopEmg(gm_Axishand[YAXIS]);
	Acm_AxStopEmg(gm_Axishand[ZAXIS]);
}

BOOL MeasureHeight(double &height)
{
	BOOL bOK = FALSE;
	U32 Result;
	U16 lState;
	while (bOK == FALSE)
	{
		Result = Acm_GpGetState(gm_GpHand, &lState);//����xy���Ƿ����ƶ�
		if (Result == SUCCESS)
		{
			if (lState & 1)
			{
				bOK = TRUE;
			}
		}
	}
	if (!CheckPositiveMeter())
	{
		AfxMessageBox("��ѹ�ϵͣ�δ����Ԥ�谲ȫֵ!\r\n������ѹ��·������ѹ�������Ƿ�������");
		return FALSE;
	}

	long state = 0;
	SetSingleSpeed(ZAXIS, 10000, 10000, 10000);
	SetNozzleDownOn(NOZZLE_0);
	SetNozzleVaccumOn(NOZZLE_0);
	Acm_AxMoveRel(gm_Axishand[ZAXIS], 200000);
	state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	DWORD dwStart = GetTickCount();
	int nEp = 0;
	while (!state)
	{
		nEp = GetTickCount() - dwStart;
		if (nEp > gm_uInfo.nSocketTime)
		{
			Acm_AxStopEmg(gm_Axishand[ZAXIS]);
			SetNozzleVaccumOff(NOZZLE_0);
			SetNozzleDownOff(NOZZLE_0);
			ZComeBack();
			AfxMessageBox("��߳�ʱ�����ʵ����߲��ʱ�䡣");
			return FALSE;
		}
		state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	}
	Acm_AxStopEmg(gm_Axishand[ZAXIS]);
	height = GetPosition(ZAXIS);
	SetNozzleVaccumOff(NOZZLE_0);
	SetNozzleBlowOn(NOZZLE_0);
	Sleep(100);
	SetNozzleBlowOff(NOZZLE_0);
	SetNozzleDownOff(NOZZLE_0);
	ZComeBack();
	return TRUE;
}
//����
BOOL CheckDoorOpen()
{
	if (gm_bDoorAlarm)
		return TRUE;
	U16		Channel = 0;
	U8		BitIn = 0;
	Acm_AxDiGetBit(gm_Axishand[XAXIS], Channel, &BitIn);
	return BitIn;
}
//��ѹ��
BOOL CheckNegtiveMeter(INT nNozzleNum)
{
	U16		Channel = 2;
	U8		BitIn = 0;
	Acm_AxDiGetBit(gm_Axishand[XAXIS], Channel, &BitIn);
	return BitIn;
}
//�����ϴ����
BOOL CheckOutTape()
{
	return TRUE;//������ϴ�
}

//�����ϴ����
BOOL CheckOutTapeIO()
{
	//long status = 0;
	//cmmDiGetOne(2, &status);
	return 0;//1��ʾ���ϴ�
}
//������Ĥ���
BOOL CheckOutTapeIOFilm()
{
	return 0;//1��ʾ����Ĥ
}

//tray�̵����ź�
BOOL CheckTrayArrive()
{
	U16		Channel = 2;
	U8		BitIn = 0;
	Acm_AxDiGetBit(gm_Axishand[XAXIS], Channel, &BitIn);
	return BitIn;
}
//��ѹ��
BOOL CheckPositiveMeter()
{
	U16		Channel = 3;
	U8		BitIn = 0;
	Acm_AxDiGetBit(gm_Axishand[XAXIS], Channel, &BitIn);
	return BitIn;
}
//�ɴ���Ϲ���
BOOL CheckFeederOutError()
{
	return FALSE;
}
//���������ź�
BOOL CheckModelCylinderPressed(INT nModelNum)
{
	U16		channel = 0;
	U8		BitIn = 0;
	int axis = XAXIS;
	switch (nModelNum)
	{
	case 0:
		channel = 0;
		axis = YAXIS; break;
	case 1:
		channel = 2;
		axis = YAXIS; break;
	case 2:
		channel = 3;
		axis = YAXIS; break;
	case 3:
		channel = 0;
		axis = ZAXIS; break;
	case 4:
		channel = 2;
		axis = ZAXIS; break;
	case 5:
		channel = 3;
		axis = ZAXIS; break;
	case 6:
		break;
	case 7:
		break;
	default:
		break;
	}
	Acm_AxDiGetBit(gm_Axishand[axis], channel, &BitIn);
	return BitIn;
}
//����̧���ź�
BOOL CheckNozzleUpStatus(INT nNozzleNum)
{
	return TRUE;
}

//--------------------------------------------------------------------------------
//DO
//tray����¼����ź�
void TrayComplete()
{
	short Channel = 5;
	int axis = XAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//�ɴ����
void SetFeederOut()
{
	//cmmDoPutOne(0, 1);
	//Sleep(50);
	//cmmDoPutOne(0, 0);
}
//�ɴ����
void SetFeedIn()
{
	//cmmDoPutOne(11, 1);
	//Sleep(50);
	//cmmDoPutOne(11, 0);
}
//��������
void SetCylinderDown(INT nModelNum)
{
	short channel = 0;
	int axis = XAXIS;
	switch (nModelNum)
	{
	case 0:
		channel = 5;
		axis = RAXIS_1;
		break;
	case 1:
		channel = 7;
		axis = XAXIS;
		break;
	case 2:
		channel = 7;
		axis = YAXIS;
		break;
	case 3:
		channel = 7;
		axis = ZAXIS;
		break;
	case 4:
		channel = 7;
		axis = RAXIS_1;
		break;
	case 5:
		channel = 6;
		axis = RAXIS_1;
		break;
	default:
		return;
	}
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], channel, 1);
}
//��������
void SetCylinderUp(INT nModelNum)
{
	short channel = 0;
	int axis = XAXIS;
	switch (nModelNum)
	{
	case 0:
		channel = 5;
		axis = RAXIS_1;
		break;
	case 1:
		channel = 7;
		axis = XAXIS;
		break;
	case 2:
		channel = 7;
		axis = YAXIS;
		break;
	case 3:
		channel = 7;
		axis = ZAXIS;
		break;
	case 4:
		channel = 7;
		axis = RAXIS_1;
		break;
	case 5:
		channel = 6;
		axis = RAXIS_1;
		break;
	default:
		return;
	}
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], channel, 0);
}
//����
void SetNozzleVaccumOn(INT nNozzleNum)
{
	short Channel = 4;
	int axis = XAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//ֹͣ����
void SetNozzleVaccumOff(INT nNozzleNum)
{
	short Channel = 4;
	int axis = XAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//����
void SetNozzleBlowOn(INT nNozzleNum)
{
	short Channel = 5;
	int axis = XAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//ֹͣ����
void SetNozzleBlowOff(INT nNozzleNum)
{
	short Channel = 5;
	int axis = XAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//�����
void SetRedLightOn()
{
	short Channel = 4;
	int axis = ZAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//�����
void SetRedLightOff()
{
	short Channel = 4;
	int axis = ZAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//��������
void SetBeepOn()
{
	short Channel = 4;
	int axis = RAXIS_1;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//��������
void SetBeepOff()
{
	short Channel = 4;
	int axis = RAXIS_1;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//�̵���
void SetGreenLightOn()
{
	short Channel = 5;
	int axis = ZAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//�̵���
void SetGreenLightOff()
{
	short Channel = 5;
	int axis = ZAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//����Ϲ�Դ��
void SetUpCameraLightOn()
{
	short Channel = 4;
	int axis = YAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//����Ϲ�Դ��
void SetUpCameraLightOff()
{
	short Channel = 4;
	int axis = YAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//����¹�Դ��
void SetDownCameraLightOn()
{
	short Channel = 5;
	int axis = YAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 1);
}
//����¹�Դ��
void SetDownCameraLightOff()
{
	short Channel = 5;
	int axis = YAXIS;
	Acm_SetU32Property(gm_Axishand[axis], CFG_AxGenDoEnable, 1);
	Acm_AxDoSetBit(gm_Axishand[axis], Channel, 0);
}
//��������
void SetNozzleDownOn(INT nNozzleNum)
{

}
//��������
void SetNozzleDownOff(INT nNozzleNum)
{

}
void CloseMotionCard()
{
	Acm_DevClose(&gm_Devhand);
}

#elif BOARD_TYPE == GOOGOL//�̸߰忨
#include "Lib\googol\gts.h"
#include "ExtMdl.h"
#pragma comment(lib,"Lib\\googol\\gts.lib")
// �ú������ĳ��GTָ���ִ�н���� commandΪָ�����ƣ� errorΪָ��ִ�з���ֵ
void commandhandler(char *command, short error)
{
	// ���ָ��ִ�з���ֵΪ��0��˵��ָ��ִ�д�������Ļ���������
	if (error)
	{
		CString str;
		str.Format("%s = %d\n", command, error);
		AfxMessageBox(str);
	}
}

BOOL OpenMotionBoard()
{
	short sRtn;
	HANDLE ShowT;

	sRtn = GT_Open();                             // ���˶�������
	commandhandler("GT_Open", sRtn);
	sRtn = GT_Reset();// ��λ�˶�������
	commandhandler("GT_Reset", sRtn);

	//sRtn = GT_LoadConfig("G:\\myproject\\AutoFP\\Debug\\GTS800.cfg");             // ���ؿ����������ļ�
	sRtn = GT_LoadConfig("GTS800.cfg");             // ���ؿ����������ļ�
	commandhandler("GT_LoadConfig", sRtn);
	GT_SetDo(MC_GPO, 0x00);//�������������

	sRtn = GT_OpenExtMdl("gts.dll");//IO��չģ����Ҫ���ذ忨�Ķ�̬���ӿ�
	commandhandler("GT_OpenExtMdl", sRtn);

	GT_ResetExtMdl();

	//sRtn = GT_LoadExtConfig("G:\\myproject\\AutoFP\\Debug\\ExtMdl.cfg");
	sRtn = GT_LoadExtConfig("ExtMdl.cfg");
	commandhandler("GT_LoadExtConfig", sRtn);


	//GT_SetExtIoValue(0, 0xffff);


	for (int i = 1; i <= 8; i++)
	{
		sRtn = GT_ClrSts(i);  // ���1~8�����״̬
		commandhandler("GT_ClrSts", sRtn);
		sRtn = GT_AxisOn(i); // ʹ���˶���		
		commandhandler("GT_AxisOn", sRtn);
		sRtn = GT_ZeroPos(i);// // ����滮λ�ú�ʵ��λ��
		commandhandler("GT_ZeroPos", sRtn);
	}

	return TRUE;
}
BOOL SingleHome(int AXIS)
{
#define SEARCH_HOME	-400000
#define HOME_OFFSET	2000
	short sRtn, capture;
	TTrapPrm trapPrm;
	long status, pos;
	double prfPos, encPos, axisPrfPos, axisEncPos;
	//// ���˶�������
	//sRtn = GT_Open();
	//commandhandler("GT_Open", sRtn);
	//// ��λ�˶�������
	//sRtn = GT_Reset();
	//commandhandler("GT_Reset", sRtn);
	//// �����˶�������
	//// ע�⣺�����ļ�test.cfgȡ���˸���ı�������λ
	//sRtn = GT_LoadConfig("test.cfg");
	//commandhandler("GT_LoadConfig", sRtn);
	//// ���ָ����ı�������λ
	//sRtn = GT_ClrSts(AXIS);
	//commandhandler("GT_ClrSts", sRtn);
	//// λ������
	//sRtn = GT_ZeroPos(AXIS);

	//// ������ʹ��
	//sRtn = GT_AxisOn(AXIS);
	//commandhandler("GT_AxisOn", sRtn);

	// �л�����λ�˶�ģʽ
	sRtn = GT_PrfTrap(AXIS);
	commandhandler("GT_PrfTrap", sRtn);
	// ��ȡ��λģʽ�˶�����
	sRtn = GT_GetTrapPrm(AXIS, &trapPrm);
	commandhandler("GT_GetTrapPrm", sRtn);
	trapPrm.acc = 0.25;
	trapPrm.dec = 0.25;
	// ���õ�λģʽ�˶�����
	sRtn = GT_SetTrapPrm(AXIS, &trapPrm);
	commandhandler("GT_SetTrapPrm", sRtn);
	// ���õ�λģʽĿ���ٶȣ�����ԭ���ٶ�
	sRtn = GT_SetVel(AXIS, 10);
	commandhandler("GT_SetVel", sRtn);

	//�����������ƶ�8000�����򸺷�����ԭ��
	sRtn = GT_SetPos(AXIS, 8000);
	commandhandler("GT_SetPos", sRtn);
	// �����˶�
	sRtn = GT_Update(1 << (AXIS - 1));
	commandhandler("GT_Update", sRtn);
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(AXIS, &status);
		// �ȴ��˶�ֹͣ
	} while (status & 0x400);
	// ����Home����
	sRtn = GT_SetCaptureMode(AXIS, CAPTURE_HOME);

	// ��ȡ����״̬
	sRtn = GT_GetCaptureStatus(AXIS, &capture, &pos);
	if (0 != capture)//�����ʼλ����ԭ�㣬�������ƶ�8000�����壬Ȼ��������Home����
	{
		sRtn = GT_SetPos(AXIS, 8000);
		commandhandler("GT_SetPos", sRtn);
		// �����˶�
		sRtn = GT_Update(1 << (AXIS - 1));
		commandhandler("GT_Update", sRtn);
		do
		{
			// ��ȡ��״̬
			sRtn = GT_GetSts(AXIS, &status);
			// �ȴ��˶�ֹͣ
		} while (status & 0x400);
		// ����Home����
		sRtn = GT_SetCaptureMode(AXIS, CAPTURE_HOME);
	}

	// ���õ�λģʽĿ��λ�ã���ԭ����������
	sRtn = GT_SetPos(AXIS, SEARCH_HOME);
	commandhandler("GT_SetPos", sRtn);
	// �����˶�
	sRtn = GT_Update(1 << (AXIS - 1));
	commandhandler("GT_Update", sRtn);
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(AXIS, &status);
		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(AXIS, &capture, &pos);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(AXIS, &prfPos);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(AXIS, &encPos);
		//printf("capture=%d prfPos=%.2lf encPos=%.2lf\r", capture, prfPos, encPos);
		// ����˶�ֹͣ�����س�����Ϣ
		if (0 == (status & 0x400))
		{
			return FALSE;
		}
		// �ȴ����񴥷�
	} while (0 == capture);
	// ��ʾ����λ��
	// �˶���"����λ��+ƫ����"
	sRtn = GT_SetPos(AXIS, prfPos + HOME_OFFSET);
	commandhandler("GT_SetPos", sRtn);
	// ���˶�״̬�¸���Ŀ��λ��
	sRtn = GT_Update(1 << (AXIS - 1));
	commandhandler("GT_Update", sRtn);

	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(AXIS, &status);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(AXIS, &prfPos);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(AXIS, &encPos);
		// �ȴ��˶�ֹͣ
	} while (status & 0x400);
	// ����Ƿ񵽴�"Home����λ��+ƫ����"
	// ��ʱһ��ʱ�䣬�ȴ����ͣ��
	Sleep(200);
	// λ������
	sRtn = GT_ZeroPos(AXIS);
	commandhandler("GT_ZeroPos", sRtn);
	// ��ȡ�滮λ��
	sRtn = GT_GetPrfPos(AXIS, &prfPos);
	// ��ȡ������λ��
	sRtn = GT_GetEncPos(AXIS, &encPos);
	// ��ȡaxis�滮λ��
	sRtn = GT_GetAxisPrfPos(AXIS, &axisPrfPos);
	// ��ȡaxis������λ��
	sRtn = GT_GetAxisEncPos(AXIS, &axisEncPos);
	// �ŷ��ر�
	//sRtn = GT_AxisOff(AXIS);

	// ��������λ
	sRtn = GT_SetSoftLimit(AXIS, 600000, -1000);

	return TRUE;
}
DWORD WINAPI XHomeProc(
	LPVOID lpParameter   // thread data
)
{
	if (!SingleHome(XAXIS))
	{
		AfxMessageBox("X���ԭ�쳣��");
	}
	SetEvent(g_hEventXHome);
	return 0;
}
DWORD WINAPI YHomeProc(
	LPVOID lpParameter   // thread data
)
{
	if (!SingleHome(YAXIS))
	{
		AfxMessageBox("Y���ԭ�쳣��");
	}
	SetEvent(g_hEventYHome);
	return 0;
}

BOOL GoHome()
{
	CloseMotionCard();
	
	OpenMotionBoard();
	if (!SingleHome(ZAXIS))
	{
		AfxMessageBox("Z���ԭ�쳣��");
		return FALSE;
	}
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, XHomeProc, NULL, 0, NULL);
	CloseHandle(hThread);
	hThread = CreateThread(NULL, 0, YHomeProc, NULL, 0, NULL);
	CloseHandle(hThread);
	g_hEventXHome = CreateEventA(NULL, FALSE, FALSE, NULL);
	g_hEventYHome = CreateEventA(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(g_hEventXHome, INFINITE);
	WaitForSingleObject(g_hEventYHome, INFINITE);
	return TRUE;
}
double GetPosition(long axis)
{
	double position = 0;
	// ��ȡ�滮λ��
	GT_GetPrfPos(axis, &position);
	return position;
}

BOOL GetPosition(long axis, double &position)
{
	// ��ȡ�滮λ��
	GT_GetPrfPos(axis, &position);
	return TRUE;
}

void SetSingleSpeed(LONG axis, double dv, double ac, double del)
{
	TTrapPrm trapPrm;
	GT_GetTrapPrm(axis, &trapPrm);
	trapPrm.acc = 0.25;
	trapPrm.dec = 0.25;
	trapPrm.smoothTime = 25;
	// ���õ�λ�˶�����
	GT_SetTrapPrm(axis, &trapPrm);
	// ����AXIS���Ŀ���ٶ�
	GT_SetVel(axis, (double)dv/1000);
}
//����z����ٶ�
void SetAxisSpeed(long axis, int speedtype)
{
	//double speedratio = (speedtype) * 0.5;
	//SetSingleSpeed(axis, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 5000, speedratio*XGUIDE * 5000);
	double speedratio = (speedtype) * 2;
	SetSingleSpeed(axis, speedratio*XGUIDE * 100, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 1000);
}
//����xy����ٶ�
void SetAxisSpeed(long axis, double moverange, int speedtype)
{
	double speedratio = (speedtype) * 2;
	if (moverange < XGUIDE * 1200)//1200mm
		SetSingleSpeed(axis, speedratio*XGUIDE * 100, speedratio*XGUIDE * 1000, speedratio*XGUIDE * 1000);
	else if (moverange >= XGUIDE * 1200)//����������1200mm,˵���˶���������
	{
		SetSingleSpeed(axis, speedratio*XGUIDE * 1, speedratio*XGUIDE * 30, speedratio*XGUIDE * 30);
		AfxMessageBox("�˶����볬�����ޣ������¼�����꣬�����Ƿ��˶����ϡ�");
	}
}

UINT XYMove(double xnextposition, double ynextposition)
{
	//�ƶ�ǰ������������Ƿ�������λ
	for (int i = 0; i < 4; i++)
	{
		if (gm_bNozzleUsed[i])//���������ʹ�ã�������λ�ź�δ��⵽���򷵻�FALSE
		{
			if (!CheckNozzleUpStatus(i))
			{
				SetNozzleDownOff(i);
				Sleep(200);
				if (!CheckNozzleUpStatus(i))
				{
					return ERROR_XYMOVE_NOZZLE_BACK;
				}
			}
		}
	}

	//��������ϵ------------------------------------
	short sRtn;
	TCrdPrm crdPrm;
	// ����ϵ�˶�״̬��ѯ����
	short run;
	// ����ϵ�˶���ɶβ�ѯ����
	long segment;
	// ����ϵ�Ļ�����ʣ��ռ��ѯ����
	long space;

	memset(&crdPrm, 0, sizeof(crdPrm));
	crdPrm.dimension = 2;	// ������ά������ϵ
	crdPrm.synVelMax = 500;	// ����ϵ�����ϳ��ٶ���: 500 pulse/ms
	crdPrm.synAccMax = 2;	// ����ϵ�����ϳɼ��ٶ���: 2 pulse/ms^2
	crdPrm.evenTime = 0;	// ����ϵ����С����ʱ��Ϊ0
	crdPrm.profile[0] = XAXIS;	// �滮��1��Ӧ��X��                       
	crdPrm.profile[1] = YAXIS;	// �滮��2��Ӧ��Y��
	crdPrm.profile[2] = 0;
	crdPrm.profile[3] = 0;
	crdPrm.profile[4] = 0;
	crdPrm.profile[5] = 0;
	crdPrm.profile[6] = 0;
	crdPrm.profile[7] = 0;
	crdPrm.setOriginFlag = 1;	// ��Ҫ���üӹ�����ϵԭ��λ��
	crdPrm.originPos[0] = 0;	// �ӹ�����ϵԭ��λ����(0,0)�������������ϵԭ���غ�
	crdPrm.originPos[1] = 0;
	crdPrm.originPos[2] = 0;
	crdPrm.originPos[3] = 0;
	crdPrm.originPos[4] = 0;
	crdPrm.originPos[5] = 0;
	crdPrm.originPos[6] = 0;
	crdPrm.originPos[7] = 0;

	sRtn = GT_SetCrdPrm(1, &crdPrm);
	//---------------------------------------------------------

	// ���������ݴ�������ϵ1��FIFO0�У�����Ҫ��������˻������е�����
	sRtn = GT_CrdClear(1, 0);

	// �򻺴���д���һ�β岹����
	sRtn = GT_LnXY(
		1,				// �ò岹�ε�����ϵ������ϵ1
		xnextposition, ynextposition,// �ò岹�ε��յ�����
		gm_uInfo.xySpeed * 2 * XGUIDE * 0.1,			// �ò岹�ε�Ŀ���ٶ�
		1,			// �岹�εļ��ٶȣ�0.1pulse/ms^2
		0,				// �յ��ٶ�Ϊ0
		0);			// ������ϵ1��FIFO0���������ݸ�ֱ�߲岹����

	// ��������ϵ1��FIFO0�Ĳ岹�˶�
	sRtn = GT_CrdStart(1, 0);
	// �ȴ��˶����
	sRtn = GT_CrdStatus(1, &run, &segment, 0);
	do
	{
		// ��ѯ����ϵ1��FIFO�Ĳ岹�˶�״̬
		sRtn = GT_CrdStatus(
			1,			// ����ϵ������ϵ1
			&run,		// ��ȡ�岹�˶�״̬
			&segment,	// ��ȡ��ǰ�Ѿ���ɵĲ岹����
			0);			// ��ѯ����ϵ1��FIFO0������
	} while (run == 1);  // ����ϵ���˶�,��ѯ����run��ֵΪ1
	commandhandler("GT_CrdStatus", sRtn);

	return SUCCESS_BURNING;
}
//���ᵥ���ƶ�
BOOL SxMove(long axis, double distance)
{
	short sRtn, capture;
	TTrapPrm trapPrm;
	long status, pos;
	long sts;
	double prfPos, encPos, axisPrfPos, axisEncPos;

	GT_PrfTrap(axis);//�л�����λ�˶�
	GT_GetTrapPrm(axis, &trapPrm);
	trapPrm.acc = 1;
	trapPrm.dec = 1;
	trapPrm.smoothTime = 25;


	// ���õ�λ�˶�����
	GT_SetTrapPrm(axis, &trapPrm);//���õ�λģʽ�˶�����
	GT_SetVel(axis, 20);//���õ�λ�˶��ٶ�
	// ��ȡ�滮λ��
	sRtn = GT_GetPrfPos(axis, &prfPos);

	GT_SetPos(axis, prfPos + distance);//����Ŀ��λ��

	GT_Update(1 << (axis - 1));
	do
	{
		// ��ȡAXIS���״̬
		sRtn = GT_GetSts(axis, &sts);
		//// ��ȡAXIS��Ĺ滮λ��
		//sRtn = GT_GetPrfPos(axis, &prfPos);
	} while (sts & 0x400);	// �ȴ�AXIS��滮ֹͣ
							// ����Ƿ񵽴�
	if (prfPos != prfPos + distance)
		return FALSE;
	return TRUE;
}

BOOL SzMove(double nextposition)
{
	short sRtn, capture;
	TTrapPrm trapPrm;
	long status, pos;
	double prfPos, encPos, axisPrfPos, axisEncPos;

	GT_PrfTrap(ZAXIS);//�л�����λ�˶�
	GT_GetTrapPrm(ZAXIS, &trapPrm);
	trapPrm.acc = 1;
	trapPrm.dec = 1;
	trapPrm.smoothTime = 25;
	// ���õ�λ�˶�����
	GT_SetTrapPrm(ZAXIS, &trapPrm);
	GT_SetVel(ZAXIS, gm_uInfo.zSpeed*XGUIDE * 0.1);//���õ�λ�˶��ٶ�
						// ��ȡ�滮λ��
	sRtn = GT_GetPrfPos(ZAXIS, &prfPos);
	
	GT_SetPos(ZAXIS, nextposition);//����Ŀ��λ��

	GT_Update(1 << (ZAXIS - 1));
	do
	{
		// ��ȡAXIS���״̬
		sRtn = GT_GetSts(ZAXIS, &status);
		// ��ȡAXIS��Ĺ滮λ��
		sRtn = GT_GetPrfPos(ZAXIS, &prfPos);
	} while (status & 0x400);	// �ȴ�AXIS��滮ֹͣ
	// ����Ƿ񵽴�
	if (prfPos != nextposition)
		return FALSE;
	return TRUE;
}
BOOL ZComeBack()
{
	short sRtn, capture;
	TTrapPrm trapPrm;
	long status, pos;
	double prfPos, encPos, axisPrfPos, axisEncPos;

	GT_PrfTrap(ZAXIS);//�л�����λ�˶�
	GT_GetTrapPrm(ZAXIS, &trapPrm);
	trapPrm.acc = 1;
	trapPrm.dec = 1;
	trapPrm.smoothTime = 25;
	// ���õ�λ�˶�����
	GT_SetTrapPrm(ZAXIS, &trapPrm);
	GT_SetVel(ZAXIS, gm_uInfo.zSpeed *XGUIDE * 0.1);//���õ�λ�˶��ٶ�

	GT_SetPos(ZAXIS, 0);//����Ŀ��λ��

	GT_Update(1 << (ZAXIS - 1));
	do
	{
		// ��ȡAXIS���״̬
		sRtn = GT_GetSts(ZAXIS, &status);
		// ��ȡAXIS��Ĺ滮λ��
		sRtn = GT_GetPrfPos(ZAXIS, &prfPos);
	} while (status & 0x400);	// �ȴ�AXIS��滮ֹͣ
	// ����Ƿ񵽴�
	if (prfPos > 100)
		return FALSE;
	return TRUE;
}
//��ת��angle��λΪ����
BOOL SrMove(double angle, int axis)
{
	if (!SINGLE_AXIS)
		axis += RAXIS_1;
	short sRtn, capture;
	float rate = 4;
	long status, pos;
	TTrapPrm trapPrm;
	double offset;
	double prfPos, encPos, axisPrfPos, axisEncPos;
	offset = angle * RGUIDE / PI / 2;

	GT_PrfTrap(axis);//�л�����λ�˶�
	GT_GetTrapPrm(axis, &trapPrm);
	trapPrm.acc = 1;
	trapPrm.dec = 1;
	trapPrm.smoothTime = 25;
	// ���õ�λ�˶�����
	GT_SetTrapPrm(axis, &trapPrm);

	GT_SetVel(axis, 20);//���õ�λ�˶��ٶ�
						 // ��ȡ�滮λ��
	sRtn = GT_GetPrfPos(axis, &prfPos);

	GT_SetPos(axis, prfPos + offset);//����Ŀ��λ��

	GT_Update(1 << (axis - 1));
	do
	{
		// ��ȡAXIS���״̬
		sRtn = GT_GetSts(axis, &status);
		//// ��ȡAXIS��Ĺ滮λ��
		//sRtn = GT_GetPrfPos(axis, &prfPos);
	} while (status & 0x400);	// �ȴ�AXIS��滮ֹͣ
	return TRUE;
}

void StopMotion()
{
	GT_Stop(0xFF, 0xFF);
}

BOOL MeasureHeight(double &height)
{
	if (!CheckPositiveMeter())
	{
		AfxMessageBox("��ѹ�ϵͣ�δ����Ԥ�谲ȫֵ!\r\n������ѹ��·������ѹ�������Ƿ�������");
		return FALSE;
	}
	long state = 0;
	SetNozzleDownOn(NOZZLE_0);
	SetNozzleVaccumOn(NOZZLE_0);
	short sRtn;
	double prfPos;
	TTrapPrm trapPrm;

	GT_PrfTrap(ZAXIS);//�л�����λ�˶�
	GT_GetTrapPrm(ZAXIS, &trapPrm);
	trapPrm.acc = 0.25;
	trapPrm.dec = 0.25;
	trapPrm.smoothTime = 25;
	// ���õ�λ�˶�����
	GT_SetTrapPrm(ZAXIS, &trapPrm);
	GT_SetVel(ZAXIS, 10);//���õ�λ�˶��ٶ�
											 // ��ȡ�滮λ��
	sRtn = GT_GetPrfPos(ZAXIS, &prfPos);

	GT_SetPos(ZAXIS, 50000);//����Ŀ��λ�ã���ԭ����������
	GT_Update(1 << (ZAXIS - 1));
	state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	DWORD dwStart = GetTickCount();
	int nEp = 0;
	while (!state)
	{
		nEp = GetTickCount() - dwStart;
		if (nEp > gm_uInfo.nSocketTime)
		{
			GT_Stop(1 << (ZAXIS - 1), 1 << (ZAXIS - 1));
			SetNozzleVaccumOff(NOZZLE_0);
			SetNozzleDownOff(NOZZLE_0);
			ZComeBack();
			AfxMessageBox("��߳�ʱ�����ʵ����Ӳ��ʱ�䡣");
			return FALSE;
		}
		state = CheckNegtiveMeter(NOZZLE_0);//���0������ĸ�ѹ��
	}
	GT_Stop(1 << (ZAXIS - 1), 1 << (ZAXIS - 1));
	height = GetPosition(ZAXIS);
	SetNozzleVaccumOff(NOZZLE_0);
	SetNozzleBlowOn(NOZZLE_0);
	Sleep(100);
	SetNozzleBlowOff(NOZZLE_0);
	SetNozzleDownOff(NOZZLE_0);
	ZComeBack();
	return TRUE;
}
void CloseMotionCard()
{
	GT_Stop(0xFF, 0xFF);
	GT_SetDo(MC_GPO, 0x00);//�������������

	for (int i = 1; i <= 8; i++)
	{
		GT_AxisOff(i);
	}
	GT_Close();
}
#endif // BOARD_TYPE == ADVANTECH

