// BurningPort.cpp: implementation of the CBurningPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoFP.h"
#include "AutoFPDlg.h"
#include "comidaqctrl1.h"
#include "BurningPort.h"
#include "DlgPosition.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "ExcelDataDlg.h"
#include <afxmt.h>
#include <Math.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
const int MAX_OFFSET = 2;//最大匹配偏差，2mm
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int GetSpecialInfo(int sockIndex, LPCTSTR field)
{
	CCriticalSection cs;
	cs.Lock();
	CString section;
	section.Format("socket #%d", sockIndex);
	CIniFile inf;
	inf.SetIniFileName(gm_strSpecialInfo);
	int keyValue = inf.GetInt((LPCSTR)section, field, 0);
	cs.Unlock();
	return keyValue;
}
void SetSpecialInfo(int sockIndex, LPCTSTR field, int value)
{
	CCriticalSection cs;
	cs.Lock();
	CString section;
	section.Format("socket #%d", sockIndex);
	CIniFile inf;
	inf.SetIniFileName(gm_strSpecialInfo);
	inf.WriteIntNumber((LPCSTR)section, field, value);
	cs.Unlock();
}

//----------------------------------------------------------------------------------
//\brief  检查要放入的socket，返回放入优先级最高的socket编号
//\多吸嘴时，要在取芯片时就确定取出来的芯片有地方放
//也就是说，吸嘴从进料取芯片时要预定烧录座
//
//----------------------------------------------------------------------------------
int  CheckChipForMoveInSocket(int nNozzle)
{
	int index = -1;
	int maxpreference = -1;
	int nPutPreference[MAXNUM_OF_PROGRAMMER]{ 0,0,0,0,0, 0,0,0,0,0 };
	bool bEmpty[MAXNUM_OF_SOCKET]{ false,false,false,false },
		bUsed[MAXNUM_OF_SOCKET]{ false,false,false,false },
		bCanPut[MAXNUM_OF_PROGRAMMER][MAXNUM_OF_SOCKET]{ { false,false,false,false },{ false,false,false,false },{ false,false,false,false },{ false,false,false,false } };
	for (int nModel = 0; nModel < MAXNUM_OF_PROGRAMMER; nModel++)
	{
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			bEmpty[nSocket] = gm_sBurningStatus[nModel].bEmpty[nSocket];
			bUsed[nSocket] = gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket];
			if (bUsed[nSocket] && bEmpty[nSocket])
			{
				if (gm_sBurningStatus[nModel].nAdpStatus[0] == FP_PROGM
					|| gm_sBurningStatus[nModel].nAdpStatus[1] == FP_PROGM
					|| gm_sBurningStatus[nModel].nAdpStatus[2] == FP_PROGM
					|| gm_sBurningStatus[nModel].nAdpStatus[3] == FP_PROGM)
					break;//如果该模组中有正在烧录的芯片，则跳过该模组
				if (gm_nModelPutPreference[nModel] > maxpreference)
				{
					maxpreference = gm_nModelPutPreference[nModel];
					index = nModel;
					break;
				}
			}
		}
	}
	if ((TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)
		&&(!gm_bStock && gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn))
		return -1;
	return index;//返回模组号
}
//预定烧录座
int BookSocket(int nNozzle)
{
	int index = -1;
	int maxpreference = -1;
	if (index >= 0)
		gm_sBurningStatus[index / MAXNUMOFSOCKET].bBooking[index%MAXNUMOFSOCKET] = true;
	return index;
}

//----------------------------------------------------------------------------------
//\brief  检查需要从socket中取出的芯片，返回取出优先级最高的socket编号
//----------------------------------------------------------------------------------
int  CheckChipForMoveOutSocket(int nNozzle)
{
	int index = -1;
	int maxpreference = -1;
	int nGetPreference[MAXNUM_OF_PROGRAMMER]{ 0,0,0,0,0, 0,0,0,0,0 };
	bool bEmpty[MAXNUM_OF_SOCKET]{ false,false,false,false },
		bUsed[MAXNUM_OF_SOCKET]{ false,false,false,false },
		bCanGet[MAXNUM_OF_PROGRAMMER][MAXNUM_OF_SOCKET]{ { false,false,false,false },{ false,false,false,false },{ false,false,false,false },{ false,false,false,false } };
	int nStatus[MAXNUM_OF_SOCKET]{ 0,0,0,0 };
	for (int nModel = 0; nModel < MAXNUM_OF_PROGRAMMER; nModel++)
	{
		if ((gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + 0]&&(FP_PROGM == gm_sBurningStatus[nModel].nAdpStatus[0]))
			|| (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + 1] && (FP_PROGM == gm_sBurningStatus[nModel].nAdpStatus[1]))
			|| (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + 2] && (FP_PROGM == gm_sBurningStatus[nModel].nAdpStatus[2]))
			|| (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + 3] && (FP_PROGM == gm_sBurningStatus[nModel].nAdpStatus[3])))
			continue;
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			bEmpty[nSocket] = gm_sBurningStatus[nModel].bEmpty[nSocket];
			nStatus[nSocket] = gm_sBurningStatus[nModel].nAdpStatus[nSocket];
			bUsed[nSocket] = gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket];
			//芯片放好芯片， 一段时间以后还未开始烧录，则判定NG	
			if (bUsed[nSocket] && !bEmpty[nSocket] && FP_READY == nStatus[nSocket]
				&& gm_sBurningStatus[nModel].bStartBurnFlag
				&& (GetTickCount() - gm_sBurningStatus[nModel].dwStartTime > gm_uInfo.nElapsed))
			{
				//gm_sBurningStatus[nModel].bStartBurnFlag = false;
				gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_NG;
			}
			//芯片烧录时间超过一定时间，则判定NG
			if (bUsed[nSocket] && !bEmpty[nSocket] && FP_PROGM == nStatus[nSocket]
				&& gm_sBurningStatus[nModel].bStartBurnFlag
				&& (GetTickCount() - gm_sBurningStatus[nModel].dwStartTime > gm_uInfo.nElapsed*100))
			{
				//gm_sBurningStatus[nModel].bStartBurnFlag = false;
				gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_NG;
			}

		}
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			bEmpty[nSocket] = gm_sBurningStatus[nModel].bEmpty[nSocket];
			nStatus[nSocket] = gm_sBurningStatus[nModel].nAdpStatus[nSocket];
			bUsed[nSocket] = gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket];
			if ((bUsed[nSocket] && !bEmpty[nSocket] && (FP_OK == nStatus[nSocket] || FP_NG == nStatus[nSocket])))
			{
				gm_sBurningStatus[nModel].bStartBurnFlag = false;
				if (gm_nModelGetPreference[nModel] > maxpreference)
				{
					maxpreference = gm_nModelGetPreference[nModel];
					index = nModel;
					break;
				}
			}
		}
	}
	if (-1 == index)
	{
		for (int nModel = 0; nModel < MAXNUM_OF_PROGRAMMER; nModel++)
		{
			for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
			{
				if (!gm_sBurningStatus[nModel].bEmpty[nSocket])
					return -1;
			}
		}
		return -2;//烧录座中没有芯片了
	}

	return index;//返回模组号
}

//----------------------------------------------------------------------------------
//\brief  检查socket中芯片能否取出，如果能取出，返回true
//----------------------------------------------------------------------------------
BOOL IsChipCanBeMoveOutFromSocket(int sock)
{
	int bEmpty = gm_sBurningStatus[sock / MAXNUMOFSOCKET].bEmpty[sock%MAXNUMOFSOCKET];
	int count = gm_sBurningStatus[sock / MAXNUMOFSOCKET].nAdpStatus[sock%MAXNUMOFSOCKET];
	if (!bEmpty && (count == 2 || count == 3) && gm_bSocketUsed[sock])
		return TRUE;
	else
		return FALSE;
}

//----------------------------------------------------------------------------------
//\brief  检查socket是否为空，如果为空，返回true
//----------------------------------------------------------------------------------
BOOL CheckSocketIsEmpty(int sock)
{
	int emptyValue = gm_sBurningStatus[sock / MAXNUMOFSOCKET].bEmpty[sock%MAXNUMOFSOCKET];
	return emptyValue;
}

//----------------------------------------------------------------------------------
//\brief  增大取料优先级
//----------------------------------------------------------------------------------
void  AddGetPreference()
{
	bool bEmpty[MAXNUM_OF_SOCKET]{ false,false,false,false }, bUsed[MAXNUM_OF_SOCKET]{ false,false,false,false };
	int nStatus[MAXNUM_OF_SOCKET]{ 0,0,0,0 };
	for (int nModel = 0; nModel < MAXNUM_OF_PROGRAMMER; nModel++)
	{
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			bEmpty[nSocket] = gm_sBurningStatus[nModel].bEmpty[nSocket];
			nStatus[nSocket] = gm_sBurningStatus[nModel].nAdpStatus[nSocket];
			bUsed[nSocket] = gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket];
		}

		if (bUsed[0] && !bEmpty[0] && (FP_OK == nStatus[0] || FP_NG == nStatus[0]) ||
			bUsed[1] && !bEmpty[1] && (FP_OK == nStatus[1] || FP_NG == nStatus[1]) ||
			bUsed[2] && !bEmpty[2] && (FP_OK == nStatus[2] || FP_NG == nStatus[2]) ||
			bUsed[3] && !bEmpty[3] && (FP_OK == nStatus[3] || FP_NG == nStatus[3]))
		{
			gm_nModelGetPreference[nModel]++;
		}
	}
}

//----------------------------------------------------------------------------------
//\brief  增大放置优先级
//----------------------------------------------------------------------------------
void AddPutPreference()
{
	bool bEmpty[MAXNUM_OF_SOCKET]{ false,false,false,false }, bUsed[MAXNUM_OF_SOCKET]{ false,false,false,false };
	for (int nModel = 0; nModel < MAXNUM_OF_PROGRAMMER; nModel++)
	{
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			bEmpty[nSocket] = gm_sBurningStatus[nModel].bEmpty[nSocket];
			bUsed[nSocket] = gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket];
		}
		//模组中只要有一个已使用且为空的socket，则增加该模组的放入优先级
		if (bUsed[0] && bEmpty[0] ||
			bUsed[1] && bEmpty[1] ||
			bUsed[2] && bEmpty[2] ||
			bUsed[3] && bEmpty[3])
		{
			gm_nModelPutPreference[nModel]++;
		}
	}
}

//----------------------------------------------------------------------------------
//\brief  获取tray盘中第presum个位置的坐标pos
//----------------------------------------------------------------------------------
BOOL GetChipPosInTray(int presum, Position &pos)
{
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	int xn = 0;
	int yn = 0;
	double height = 0.0;
	double height1 = 0.0;
	double height2 = 0.0;
	double heightPitchX = 0.0;
	double heightPitchY = 0.0;
	double firstx = 0.0;
	double firsty = 0.0;
	double cornerX = 0.0;
	double cornerY = 0.0;
	double lastX = 0.0;
	int trayXn = 0;
	int trayYn = 0;
	trayXn = gm_dataTray.xn;
	trayYn = gm_dataTray.yn;
	yn = presum / trayXn;
	xn = presum % trayXn;

	double xpitch = 0.0;
	double ypitch = 0.0;
	double x1, y1, x2, y2, x3, y3;

	Point pt1 = gm_dataPos.GetPlacePoint("TrayPos1");
	Point pt2 = gm_dataPos.GetPlacePoint("TrayPos2");
	Point pt3 = gm_dataPos.GetPlacePoint("TrayPos3");

	x1 = pt1.x;
	y1 = pt1.y;
	x2 = pt2.x;
	y2 = pt2.y;
	x3 = pt3.x;
	y3 = pt3.y;
	xpitch = (float)((x1 - x2) / (gm_dataTray.xn - 1));
	ypitch = (float)((y3 - y2) / (gm_dataTray.yn - 1));
	//xpitch = gm_dataPos.GetPitchX();
	//ypitch = gm_dataPos.GetPitchY();
	height = gm_dataPos.GetHeight("TrayHeight1");
	height1 = gm_dataPos.GetHeight("TrayHeight2");
	height2 = gm_dataPos.GetHeight("TrayHeight3");
	heightPitchX = (height1 - height) / (trayXn - 1);
	heightPitchY = (height2 - height1) / (trayYn - 1);

	firstx = pt1.x;
	firsty = pt1.y;

	cornerX = pt2.x;
	cornerY = pt2.y;

	lastX = pt3.x;
	pos.x = firstx - xn * xpitch + (lastX - cornerX) / (trayYn - 1) * yn;
	pos.y = firsty + yn * ypitch + (cornerY - firsty) / (trayXn - 1)*xn;
	pos.z = height + xn*heightPitchX + yn*heightPitchY;

	return TRUE;
}

//----------------------------------------------------------------------------------
//\brief  获取补料盘第presum个位置的坐标pos
//----------------------------------------------------------------------------------
UINT GetChipPosInStockTray(int presum, Position &pos)
{
	int nSupply = 0;
	//if (presum == gm_nTotalStock)
	//{
	//	//ShowErrorMessage("备料盘已空，请更换备料盘。");
	//	gm_nCurrentStock = 0;
	//	return ERROR_STOCKTRAY_EMPTY;
	//}
	//if (gm_bFinish)//结束时
	//{
	//	gm_nCurrentStock--;
	//	if (gm_nCurrentStock < 0)
	//	{
	//		//ShowErrorMessage("备料盘已满。");
	//		return ERROR_STOCKTRAY_FULL;
	//	}
	//}
	nSupply = presum;

	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	int xn = 0;
	int yn = 0;
	double height = 0.0;
	double height1 = 0.0;
	double height2 = 0.0;
	double heightPitchX = 0.0;
	double heightPitchY = 0.0;
	double firstx = 0.0;
	double firsty = 0.0;
	double cornerX = 0.0;
	double cornerY = 0.0;
	double lastX = 0.0;
	int trayXn = 0;
	int trayYn = 0;
	trayXn = gm_dataTray.xn;
	trayYn = gm_dataTray.yn;
	yn = nSupply / trayXn;
	xn = nSupply % trayXn;

	double xpitch = 0.0;
	double ypitch = 0.0;
	double x1, y1, x2, y2, x3, y3;

	Point pt1 = gm_dataPos.GetPlacePoint("SupplyPos1");
	Point pt2 = gm_dataPos.GetPlacePoint("SupplyPos2");
	Point pt3 = gm_dataPos.GetPlacePoint("SupplyPos3");

	x1 = pt1.x;
	y1 = pt1.y;
	x2 = pt2.x;
	y2 = pt2.y;
	x3 = pt3.x;
	y3 = pt3.y;
	xpitch = (float)((x1 - x2) / (gm_dataStock.xn - 1));
	ypitch = (float)((y3 - y2) / (gm_dataStock.yn - 1));
	//xpitch = gm_dataPos.GetPitchX();
	//ypitch = gm_dataPos.GetPitchY();

	height = gm_dataPos.GetHeight("SupplyHeight1");
	height1 = gm_dataPos.GetHeight("SupplyHeight2");
	height2 = gm_dataPos.GetHeight("SupplyHeight3");
	heightPitchX = (height1 - height) / (trayXn - 1);
	heightPitchY = (height2 - height1) / (trayYn - 1);

	firstx = pt1.x;
	firsty = pt1.y;

	cornerX = pt2.x;
	cornerY = pt2.y;

	lastX = pt3.x;
	pos.x = firstx - xn * xpitch + (lastX - cornerX) / (trayYn - 1) * yn;
	pos.y = firsty + yn * ypitch + (cornerY - firsty) / (trayXn - 1)*xn;
	pos.z = height + xn*heightPitchX + yn*heightPitchY;
	//if (gm_bStart&&!gm_bFinish)//运行时
	//	nSupply++;
	return SUCCESS_BURNING;
}
//----------------------------------------------------------------------------------
//\brief  获取废料盘第presum个位置的坐标pos
//----------------------------------------------------------------------------------
BOOL GetChipPosInScrap(int presum, Position &pos)
{
	//static int nNG = 0;
	//if ((gm_nCurrentFailSize % (gm_dataNG.xn*gm_dataNG.yn)) == (gm_dataNG.xn*gm_dataNG.yn - 1))
	//{
	//	//nNG = 0;
	//	gm_nCurrentFailSize = 0;
	//	return FALSE;
	//}
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	int xn = 0;
	int yn = 0;

	double firstx = 0.0;
	double firsty = 0.0;
	double cornerX = 0.0;
	double cornerY = 0.0;
	double lastX = 0.0;

	yn = presum / gm_dataNG.xn;
	xn = presum % gm_dataNG.xn;

	double xpitch = 0.0;
	double ypitch = 0.0;
	double x1, y1, x2, y2, x3, y3;

	Point pt1 = gm_dataPos.GetPlacePoint("NGBinPos1");
	Point pt2 = gm_dataPos.GetPlacePoint("NGBinPos2");
	Point pt3 = gm_dataPos.GetPlacePoint("NGBinPos3");

	x1 = pt1.x;
	y1 = pt1.y;
	x2 = pt2.x;
	y2 = pt2.y;
	x3 = pt3.x;
	y3 = pt3.y;
	xpitch = (float)((x1 - x2) / (gm_dataNG.xn - 1));
	ypitch = (float)((y3 - y2) / (gm_dataNG.yn - 1));
	//xpitch = gm_dataPos.GetPitchX();
	//ypitch = gm_dataPos.GetPitchY();

	firstx = pt1.x;
	firsty = pt1.y;

	cornerX = pt2.x;
	cornerY = pt2.y;

	lastX = pt3.x;
	pos.x = firstx - xn * xpitch + (lastX - cornerX) / (gm_dataNG.yn - 1) * yn;
	pos.y = firsty + yn * ypitch + (cornerY - firsty) / (gm_dataNG.xn - 1)* xn;

	pos.z = gm_dataPos.GetHeight("NGBinHeight");
	//nNG++;

	return TRUE;
}
//----------------------------------------------------------------------------------
//\brief  获取编带进料坐标
//----------------------------------------------------------------------------------
Position  GetPosForTapeIn()
{
	CString str;
	str = "TapeIn";
	Position curPos;
	Point temp;
	temp = gm_dataPos.GetPlacePoint((LPCSTR)str);
	curPos.x = temp.x;
	curPos.y = temp.y;
	curPos.z = gm_dataPos.GetHeight((LPCSTR)str);
	return curPos;
}

//----------------------------------------------------------------------------------
//\brief  获取编带出料坐标
//----------------------------------------------------------------------------------
Position  GetPosForTapeOut()
{
	CString str;
	str = "TapeOut";
	Position curPos;
	Point temp;
	temp = gm_dataPos.GetPlacePoint((LPCSTR)str);
	curPos.x = temp.x;
	curPos.y = temp.y;
	curPos.z = gm_dataPos.GetHeight((LPCSTR)str);
	return curPos;
}
BOOL  GetPosForTape(const char *inOut, Position &pos)
{
	Point temp;
	temp = gm_dataPos.GetPlacePoint(inOut);
	pos.x = temp.x;
	pos.y = temp.y;
	pos.z = gm_dataPos.GetHeight((LPCSTR)inOut);
	return TRUE;
}

//----------------------------------------------------------------------------------
//\brief  获取第sockInx个socket的坐标
//----------------------------------------------------------------------------------
Position  GetPosForSocket(int sockInx)
{
	CString str;
	str.Format("Socket%d", sockInx);
	Position curPos;
	Point temp;
	temp = gm_dataPos.GetPlacePoint((LPCSTR)str);
	curPos.x = temp.x;
	curPos.y = temp.y;
	curPos.z = gm_dataPos.GetHeight((LPCSTR)str);
	return curPos;
}
//获取第sockInx个socket的坐标
BOOL  GetPosForSocket(int sockInx, Position &pos)
{
	CString str;
	str.Format("Socket%d", sockInx);
	Point temp;
	temp = gm_dataPos.GetPlacePoint((LPCSTR)str);
	pos.x = temp.x;
	pos.y = temp.y;
	pos.z = gm_dataPos.GetHeight((LPCSTR)str);
	return TRUE;
}

UINT BeepThreadProc(LPVOID lParam)
{
	while (gm_bBeep)
	{
		SetBeepOn();
		Sleep(50);
		SetBeepOff();
		Sleep(200);
	}
	return 0;
}
UINT GetChip()
{
	SetNozzleVaccumOn(0);
	Sleep(10);
	if (!CheckNegtiveMeter(0))
		return ERROR_GET_CHIP;
	return SUCCESS_BURNING;
}

UINT GetChip(int nNozzleNum)
{
#ifdef FOUR_NUZZLE
	double currentposition = 0;
	GetPosition(ZAXIS, currentposition);

	SetNozzleDownOn(nNozzleNum);
	Sleep(60);
	SzMove(currentposition + ZGUIDE*0.6);

	SetNozzleVaccumOn(nNozzleNum);
	Sleep(gm_uInfo.nNozzleDelay);
	SzMove(currentposition);
	SetNozzleDownOff(nNozzleNum);
//#if SINGLE_AXIS
//#endif // SINGLE_AXIS
	Sleep(30);
#else
	SetNozzleVaccumOn(nNozzleNum);
	ZComeBack();
	Sleep(gm_uInfo.nNozzleDelay);
#endif // FOUR_NUZZLE
	return SUCCESS_BURNING;

}

UINT PutChip()
{
	SetNozzleVaccumOff(0);
	SetNozzleBlowOn(0);
	Sleep(50);
	SetNozzleBlowOff(0);

	return SUCCESS_BURNING;
}

UINT PutChip(int nNozzleNum)
{
#if FOUR_NUZZLE
	double currentposition = 0;
	GetPosition(ZAXIS, currentposition);

	SetNozzleDownOn(nNozzleNum);
	Sleep(50);
	SzMove(currentposition + ZGUIDE*0.6);

	SetNozzleVaccumOff(nNozzleNum);
	SetNozzleBlowOn(nNozzleNum);
	Sleep(gm_uInfo.nNozzleDelay);
	SzMove(currentposition);
	SetNozzleBlowOff(nNozzleNum);
	SetNozzleDownOff(nNozzleNum);
	
#else
	SetNozzleVaccumOff(nNozzleNum);
	Sleep(50);
	SetNozzleBlowOn(nNozzleNum);
	Sleep(50);
	SetNozzleBlowOff(nNozzleNum);
	ZComeBack();
#endif // FOUR_NUZZLE
	return SUCCESS_BURNING;
}

//----------------------------------------------------------------------------------
//\brief  取芯片过程
//\param  nGetNumber [in] 取出的位置编号
//\param  nNozzleNumber [in] 吸嘴编号
//\param  nWorkMode [in] 工作模式
//\//return
//----------------------------------------------------------------------------------
UINT GetChipProc(int nGetNumber, int nNozzleNumber, int nWorkMode)
{
	CString strNozzle;//offset_nozzle1
	strNozzle.Format("offset_nozzle%d", nNozzleNumber + 1);
	Point offset_nozzle{ 0, 0 };//吸嘴与上相机的偏差值
	offset_nozzle = gm_dataPos.GetPlacePoint(strNozzle);
	Position posCurrentChip{ 0, 0, 0 };//待取芯片的坐标值
	int getHeight = 0;//取片高度偏差值
	int nResult = 0;
	//进料
	if (TRAY_TO_TRAY == nWorkMode || TRAY_TO_TAPE == nWorkMode)//从tray盘取料
	{
		GetChipPosInTray(nGetNumber, posCurrentChip);
		offset_nozzle.x += gm_uInfo.nOffsetInX;
		offset_nozzle.y += gm_uInfo.nOffsetInY;
		getHeight = gm_uInfo.nGetTrayHeight;
	}
	else if (TAPE_TO_TRAY == nWorkMode || TAPE_TO_TAPE == nWorkMode)//从编带取料
	{
		GetPosForTape("TapeIn", posCurrentChip);
		offset_nozzle.x += gm_uInfo.nOffsetInX;
		offset_nozzle.y += gm_uInfo.nOffsetInY;
		getHeight = gm_uInfo.nGetTrayHeight;
	}
	else if (STOCK_TRAY == nWorkMode)//从补料盘取料
	{
		nResult = GetChipPosInStockTray(nGetNumber, posCurrentChip);
		if (SUCCESS_BURNING == nResult)
		{
			getHeight = gm_uInfo.nGetTrayHeight;
		}
		else
		{
			return nResult;
		}
	}
	//出料
	else if (SOCKET_NUM == nWorkMode)//从socket取料
	{
		GetPosForSocket(nGetNumber, posCurrentChip);
		offset_nozzle.x += gm_uInfo.nOffsetSocketX;
		offset_nozzle.y += gm_uInfo.nOffsetSocketY;
		getHeight = gm_uInfo.nGetSocketHeight;
	}
	SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
	SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
	nResult = XYMove(posCurrentChip.x + offset_nozzle.x, posCurrentChip.y + offset_nozzle.y);
	if (SUCCESS_BURNING != nResult)
		return nResult;
	SzMove(posCurrentChip.z + getHeight);
	GetChip(nNozzleNumber);
	Sleep(SLEEP_TIME_GET_CHIP);
	int n = 0;
	while (!CheckNegtiveMeter(nNozzleNumber))
	{
		SetNozzleVaccumOff(nNozzleNumber);

		if (SOCKET_NUM == nWorkMode)//如果从socket中取料，则不重复取料，以免压坏芯片
			return ERROR_GET_CHIP;

		if (n > 0)
		{
			//ShowErrorMessage("芯片粘在吸嘴上，无法正常放置。");
			return ERROR_GET_CHIP;
		}
		SzMove(posCurrentChip.z + getHeight + ZGUIDE*0.4);
		GetChip(nNozzleNumber);
		Sleep(SLEEP_TIME_GET_CHIP);
		n++;
	}
	return SUCCESS_BURNING;
}

//----------------------------------------------------------------------------------
//brief  放芯片过程
//param  nPutNumber [in] 放入的位置编号,对于烧录座为座子编号，对于tray盘为tray盘中芯片序号，对于编带为编带出料位置
//param  nNozzleNumber [in] 吸嘴编号	0 1 2 3
//param  posOffset [in] 放片偏差
//param  nWorkMode [in] 工作模式
//return
//----------------------------------------------------------------------------------
UINT PutChipProc(int nPutNumber, int nNozzleNumber, Position2 &posOffset, int nWorkMode)
{
	CString strNozzle;
	strNozzle.Format("offset_nozzle%d", nNozzleNumber + 1);
	Point offset_nozzle{ 0,0 };
	offset_nozzle = gm_dataPos.GetPlacePoint(strNozzle);
	Position posCurrentPut{ 0,0,0 };
	double putHeight = 0;//保存取放高度偏差
	UINT nResult = 0;
	if (TRAY_TO_TRAY == nWorkMode || TAPE_TO_TRAY == nWorkMode)//放入tray盘
	{
		GetChipPosInTray(nPutNumber, posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetOutX;
		offset_nozzle.y += gm_uInfo.nOffsetOutY;
		putHeight = gm_uInfo.nPutTrayHeight;
	}
	else if (TRAY_TO_TAPE == nWorkMode || TAPE_TO_TAPE == nWorkMode)//放入编带
	{
		GetPosForTape("TapeOut", posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetOutX;
		offset_nozzle.y += gm_uInfo.nOffsetOutY;
		putHeight = gm_uInfo.nPutTrayHeight;
	}
	else if (SOCKET_NUM == nWorkMode)//放入socket
	{
		GetPosForSocket(nPutNumber, posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetSocketX;
		offset_nozzle.y += gm_uInfo.nOffsetSocketY;
		putHeight = gm_uInfo.nPutSocketHeight;
	}
	else if (SCRAP_TRAY == nWorkMode)//放入废料盘
	{
		ZComeBack();
		if (!GetChipPosInScrap(nPutNumber, posCurrentPut))
			return ERROR_SCRAYTRAY_EMPTY;
		putHeight = 0;
	}
	else if (STOCK_TRAY == nWorkMode)//放入补料盘
	{
		if (SUCCESS_BURNING != GetChipPosInStockTray(nPutNumber, posCurrentPut))
			return ERROR_STOCKTRAY_EMPTY;
		putHeight = gm_uInfo.nPutTrayHeight;
	}
	SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
	SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
	nResult = XYMove(
		posCurrentPut.x + offset_nozzle.x - (XGUIDE / gm_uInfo.nPixelsPM_down*posOffset.x),
		posCurrentPut.y + offset_nozzle.y - (YGUIDE / gm_uInfo.nPixelsPM_down*posOffset.y));
	if (SUCCESS_BURNING != nResult)
		return nResult;

	SrMove(posOffset.phi, nNozzleNumber);//固高

	SzMove(posCurrentPut.z + putHeight);
	PutChip(nNozzleNumber);
#ifndef FOUR_NUZZLE
	ZComeBack();
#endif
	Sleep(SLEEP_TIME_GET_CHIP);
	SetNozzleVaccumOn(nNozzleNumber);
	Sleep(SLEEP_TIME_GET_CHIP);
	int n = 0;
	while (CheckNegtiveMeter(nNozzleNumber))
	{
		SetNozzleVaccumOff(nNozzleNumber);
		if (n > 0)
		{
			//ShowErrorMessage("芯片粘在吸嘴上，无法正常放置。");
			return ERROR_PUT_CHIP;
		}
		SzMove(posCurrentPut.z + putHeight - ZGUIDE*0.4);
		PutChip(nNozzleNumber);
		Sleep(SLEEP_TIME_GET_CHIP);
		SetNozzleVaccumOn(nNozzleNumber);
		Sleep(SLEEP_TIME_GET_CHIP);
		n++;
	}
	SetNozzleVaccumOff(nNozzleNumber);

	SrMove(-posOffset.phi, nNozzleNumber);//转回原点

	return SUCCESS_BURNING;
}
//----------------------------------------------------------------------------------
//\brief  芯片移动到下相机进行坐标检测
//\param  nNozzleNumber			[in] 吸嘴编号
//\param  posOffset				[out] 偏差值
//\param  lParam				[in] 父窗口指针
//\return TRUE	--	匹配成功
//\return FALSE	--	匹配失败
//----------------------------------------------------------------------------------
UINT CheckChipProc(int nNozzleNumber, Position2 &posOffset, LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	UINT nResult = 0;
	CString strNozzle;
	strNozzle.Format("offset_nozzle%d", nNozzleNumber + 1);
	Point offset_nozzle = gm_dataPos.GetPlacePoint(strNozzle);
	Point ptDownCamera = gm_dataPos.GetPlacePoint("DownCamera");
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
	SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
	nResult = XYMove(ptDownCamera.x + offset_nozzle.x, ptDownCamera.y + offset_nozzle.y);
	if (SUCCESS_BURNING != nResult)
	{
		return nResult;
	}

	if (!pMainWnd->CheckModel(posOffset, "down_camera_in"))
		return ERROR_CAMERA_CHECK;

	double x, y;
	x = posOffset.x*cos(posOffset.phi) + posOffset.y*sin(posOffset.phi);
	y = posOffset.y*cos(posOffset.phi) - posOffset.x*sin(posOffset.phi);
	posOffset.x = x;
	posOffset.y = y;

	Sleep(20);
	return SUCCESS_BURNING;
}

//放满的座子开始烧录
void StartProgramProc(int nModel, LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	int nUsed = 0;//每个模组的座子使用数量
	int nTotalSocket = 0;//每个模组放入芯片的座子数量
	for (int k = 0; k < MAXNUM_OF_SOCKET; k++)//检查每个模组是否放满芯片
	{
		if (gm_bSocketUsed[nModel * MAXNUM_OF_SOCKET + k])
			nUsed++;
		if ((gm_sBurningStatus[nModel].bEmpty[k] == FALSE) &&
			(gm_sBurningStatus[nModel].nAdpStatus[k] == FP_READY))
		{
			nTotalSocket++;
		}
	}
	if (nTotalSocket == nUsed)//如果某个模组芯片放满，则上拉气缸，开始烧录
	{
		SetCylinderUp(nModel);
		Sleep(100);
		pMainWnd->StartOneProgrammer(nModel + 1);
		gm_sBurningStatus[nModel].dwStartTime = GetTickCount();
		gm_sBurningStatus[nModel].bStartBurnFlag = true;
	}
	else if (gm_nTotalInSize + gm_nCurrentStock >= gm_nSum
		|| ((TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode))
		&&(!gm_bStock && gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn))//如果无补料，则单盘进料完成时检查是否需要强制烧录
	{
		BOOL bHaveReadyChip = FALSE;
		BOOL bHaveFPChip = FALSE;
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			if (gm_bSocketUsed[nModel * MAXNUM_OF_SOCKET + nSocket] && (gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE) && (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_READY))
			{
				bHaveReadyChip = TRUE;
				break;
			}
		}
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			if (gm_bSocketUsed[nModel * MAXNUM_OF_SOCKET + nSocket] && (gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE)
				&& (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_OK || gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_NG))
			{
				bHaveFPChip = TRUE;
				break;
			}
		}
		if (bHaveReadyChip && !bHaveFPChip)
		{
			SetCylinderUp(nModel);
			Sleep(200);
			//CString str;
			//str.Format("烧录座未放满%d，需手动烧录。", nModel + 1);
			//ShowErrorMessage(str, pMainWnd);
			gm_sBurningStatus[nModel].nAdpStatus[0] == 5;
			gm_sBurningStatus[nModel].nAdpStatus[1] == 5;
			gm_sBurningStatus[nModel].nAdpStatus[2] == 5;
			gm_sBurningStatus[nModel].nAdpStatus[3] == 5;
			pMainWnd->StartOneProgrammer(nModel + 1);
			gm_sBurningStatus[nModel].dwStartTime = GetTickCount();
			gm_sBurningStatus[nModel].bStartBurnFlag = true;
		}
	}

}

DWORD WINAPI SrMoveProc(
	LPVOID lpParameter   // thread data
)
{
	//Position2 *posOffset = (Position2*)lpParameter;
	//SrMove(posOffset->phi, RAXIS_1);
	double angle = *(double*)lpParameter;
	SrMove(angle, RAXIS_1-3);
	return 0;
}
DWORD WINAPI SrMoveProc1(
	LPVOID lpParameter   // thread data
)
{
	//Position2 *posOffset = (Position2*)lpParameter;
	//SrMove(posOffset->phi, RAXIS_1);
	double *angle = (double*)lpParameter;
	SrMove(*angle, RAXIS_2-3);
	return 0;
}
DWORD WINAPI SrMoveProc2(
	LPVOID lpParameter   // thread data
)
{
	//Position2 *posOffset = (Position2*)lpParameter;
	//SrMove(posOffset->phi, RAXIS_1);
	double *angle = (double*)lpParameter;
	SrMove(*angle, RAXIS_3-3);
	return 0;
}
DWORD WINAPI SrMoveProc3(
	LPVOID lpParameter   // thread data
)
{
	//Position2 *posOffset = (Position2*)lpParameter;
	//SrMove(posOffset->phi, RAXIS_1);
	double *angle = (double*)lpParameter;
	SrMove(*angle, RAXIS_4-3);
	return 0;
}

//转料过程
UINT ConvertChipProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//吸嘴拿到芯片标识
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//下相机对每个吸嘴上芯片匹配是否成功标识，默认为成功
	UINT nResult = 0;

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	//从进料吸取芯片
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		//1.tray盘取料：正常运行，tray盘进料模式，tray盘中还有料，
		if (gm_bStart//运行时
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray盘进料
			&& (gm_nTotalInSize < gm_nSum - gm_nTotalStock)//tray盘进料总量未用完
			&& (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn))//当前tray盘未取完
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTrayStatus(gm_nCurrentInSize, EMPTY_STATUS);
				bNozzleHasChip[i] = TRUE;//i号吸嘴上有芯片
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//错误代码保存起来
			}

		}
		//2.tape取料：正常运行，tape进料模式，tape中还有料
		else if (gm_bStart//运行时
			&& (TAPE_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode)//tape进料
			&& (gm_nTotalInSize < gm_nSum))//烧录完成总数少于待烧录总数
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTapeIn(gm_nCurrentInSize);
				pMainWnd->TapeInOk();
				bNozzleHasChip[i] = TRUE;//i号吸嘴上有芯片
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//错误代码保存起来
			}
		}
		//3.备料盘取料：有补料模式，正常运行，tray盘进料模式，tray盘中无料，tray盘ok料未放满，stock盘中有料；
		//				有补料模式，结束时
		else if ((gm_bStart && gm_bStock//有补料模式，正常运行
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray盘进料模式
			&& ((gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn) //当前tray盘无料
				|| (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum - gm_nTotalStock))//总tray盘进料无料
			&& (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//tray盘ok料未放满
			&& gm_nCurrentStock < gm_nTotalStock)//stock盘中有料

			/*|| (gm_bStart && gm_bStock && gm_nCurrentStock < gm_nTotalStock)*/)
		{
			//if (!CheckChipCanBeGet(STOCK_TRAY, i))
			//	break;
			nResult = GetChipProc(gm_nCurrentStock, i, STOCK_TRAY);
			if (SUCCESS_BURNING == nResult)
			{
				bNozzleHasChip[i] = TRUE;
				gm_nCurrentStock++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}
		else if (gm_bStart && gm_bStock//有补料模式，正常运行
			&& gm_nTotalInSize < gm_nSum - gm_nTotalStock//进料总数未达到tray盘料总量
			&& gm_nCurrentStock >= gm_nTotalStock)//补料盘已空
		{
			gm_bErrorFig[i] = ERROR_STOCKTRAY_EMPTY;
		}
	}
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//吸嘴上没有芯片
	{
		return SUCCESS_BURNING;//跳出进料过程
	}

	//旋转90°
	if (gm_nRotateAngleIn != 0)
	{
		HANDLE hThread;
		static double angle = PI / (double)gm_nRotateAngleIn;
#if FOUR_NUZZLE
		hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
		CloseHandle(hThread);
#else
		if (gm_bNozzleUsed[0])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[1])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc1, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[2])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc2, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[3])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc3, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
#endif // FOUR_NUZZLE

	}

	//吸嘴到下相机，进行芯片检测
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//用于保存4个吸嘴的偏差值
	if (gm_bDownCameraIn)
	{
		SetDownCameraLightOn();
		for (int i = 3; i >= 0; i--)//进料时逆序检查，出料时顺序检查
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i])
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);

				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>20 || abs(posOffset[i].y)>20))
				{
					//如果匹配失败，则放入废料盒
					gm_nTotalCheckFail++;
					gm_nContinueCheckFail++;
					bMatchSuccess[i] = FALSE;
					continue;
				}
				else if (SUCCESS_BURNING != nResult)
				{
					gm_bErrorFig[i] = nResult;
					continue;
				}
				gm_nContinueCheckFail = 0;
				//posOffset[i].x = 0;//因双吸嘴使用下相机时偏差过大，此为临时解决方案
				//posOffset[i].y = 0;
			}
		}
		SetDownCameraLightOff();
	}
	int nBurningSataus[4]{ 2, 2, 2, 2 };//0为未烧录状态，1为正在烧录，2为烧录成功，3为烧录失败

	//出料情况：
	//1.tray盘出料：正常运行，tray盘出料模式，tray盘进料数量大于当前ok数量，ok ng总量未达到工单数量，tray盘未放满
	//				结束时，无补料模式
	//2.tape出料：正常运行，tape出料模式，ok ng总量未达到工单数量
	//				结束时，
	//3.备料盘出料：正常运行，tray盘出料模式，tray盘进料数量小于等于当前ok数量，tray盘已满
	//				结束时，有补料模式
	//4.废料盘出料：烧录NG芯片，匹配失败芯片
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		if (!bNozzleHasChip[i])
			continue;

		if (!CheckNegtiveMeter(i))
		{
			gm_bErrorFig[i] = CHIP_MISS;
			continue;
		}

		if (FP_OK == nBurningSataus[i] && TRUE == bMatchSuccess[i])
		{//烧录成功，匹配成功
			if (gm_bStart)//运行时
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{//tray盘出料
					if (gm_nCurrentInSize > gm_nCurrentGoodSize//当前tray盘进料大于当前OK料，也就是tray盘有地方放
															   //&& gm_nTotalGoodSize/* + gm_nTotalFailSize*/ < gm_nSum
						&& gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//当前OK料小于tray盘规格
					{//放入tray盘
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
					else//放入备料盘
					{
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//放入补料盘
						{
							//gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nStockGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
				}
				else//OK料编带出料模式
				{//编带出料模式下，不用检查出料是否满，只要吸嘴上有芯片，就可以放入编带
				 //需要检查出料报警
					if (!CheckFeederOutError())
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTapeOut(gm_nCurrentGoodSize);
							pMainWnd->TapeOutOk();
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
					else
					{//飞达出料报警，放入废料盘
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//放入废料盘
						{
							gm_nCurrentFailSize++;
							gm_nTotalFailSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
						gm_bErrorFig[i] = ERROR_TAPE_OUT;
					}
				}
			}
			else if (gm_bFinish)//结束时
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentInSize > gm_nCurrentGoodSize)//结束，tray盘还有空位置，放入tray盘或tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
					else if (/*gm_bStock && */gm_nCurrentStock > gm_nStockGoodSize)
					{//tray盘没位置，则放入补料盘
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//放入补料盘
						{
							//gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nStockGoodSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
				}
				else//编带结束
				{
					if (!CheckFeederOutError())//结束，tray盘还有空位置，放入tray盘或tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTapeOut(gm_nCurrentGoodSize);
							pMainWnd->TapeOutOk();
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
					else
					{//飞达出料报警，放入废料盘
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//放入废料盘
						{
							gm_nCurrentFailSize++;
							gm_nTotalFailSize++;
							gm_nCurrentOutSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
						gm_bErrorFig[i] = ERROR_TAPE_OUT;
					}
				}

			}
		}
		else//烧录NG匹配失败芯片处理过程
		{
			nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
			if (SUCCESS_BURNING == nResult)//放入废料盘
			{
				gm_nCurrentFailSize++;
				gm_nTotalFailSize++;
				bNozzleHasChip[i] = FALSE;
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}

	}
	if (!ZComeBack())
	{
		//MessageBox(NULL, "ZComeBack Error of End Output proc", NULL, MB_ICONERROR);
		return ERROR_XYMOVE_ZCOMEBACK;
	}

	return SUCCESS_BURNING;
}
//进料过程
UINT InputProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//吸嘴拿到芯片标识
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//下相机对每个吸嘴上芯片匹配是否成功标识，默认为成功
	UINT nResult = 0;
	int nEmptyNum = 0;//获得该模组已使用且为空的座子数量
	int nModel = CheckChipForMoveInSocket(0);//获取放入优先级最高的模组
	if (nModel == -1)
		return PROGRAM_BUSY;
	//检查下拉气缸信号
		SetCylinderDown(nModel);
		DWORD dwStart = ::GetTickCount();
		while (!CheckModelCylinderPressed(nModel))
		{
			Sleep(100);
			DWORD dwElapse = ::GetTickCount() - dwStart;
			if (dwElapse > 3000)
			{
				CString str;
				str.Format("气缸%d下拉信号异常。", nModel + 1);
				ShowErrorMessage(str);
				return ERROR_SOCKET_PRESSED;
			}
		}

	//每次只操作一个模组，不跨模组取放料
	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{
		if (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket] && gm_sBurningStatus[nModel].bEmpty[nSocket] == TRUE)
		{
			nEmptyNum++;
		}
	}

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	//从进料吸取芯片
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		if (nEmptyNum <= 0)
			break;
		//1.tray盘取料：正常运行，tray盘进料模式，tray盘中还有料，
		if (gm_bStart//运行时
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray盘进料
			&& (gm_nTotalInSize < gm_nSum - gm_nTotalStock)//tray盘进料总量未用完
			&& (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn))//当前tray盘未取完
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTrayStatus(gm_nCurrentInSize, EMPTY_STATUS);
				bNozzleHasChip[i] = TRUE;//i号吸嘴上有芯片
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//错误代码保存起来
			}

		}
		//2.tape取料：正常运行，tape进料模式，tape中还有料
		else if (gm_bStart//运行时
			&& (TAPE_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode)//tape进料
			&& (gm_nTotalInSize < gm_nSum))//烧录完成总数少于待烧录总数
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTapeIn(gm_nCurrentInSize);
				pMainWnd->TapeInOk();
				bNozzleHasChip[i] = TRUE;//i号吸嘴上有芯片
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//错误代码保存起来
			}
		}
		//3.备料盘取料：有补料模式，正常运行，tray盘进料模式，tray盘中无料，tray盘ok料未放满，stock盘中有料；
		//				有补料模式，结束时
		else if ((gm_bStart && gm_bStock//有补料模式，正常运行
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray盘进料模式
			&& ((gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn) //当前tray盘无料
				|| (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum - gm_nTotalStock))//总tray盘进料无料
			&& (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//tray盘ok料未放满
			&& gm_nCurrentStock < gm_nTotalStock)//stock盘中有料

			/*|| (gm_bStart && gm_bStock && gm_nCurrentStock < gm_nTotalStock)*/)
		{
			//if (!CheckChipCanBeGet(STOCK_TRAY, i))
			//	break;
			nResult = GetChipProc(gm_nCurrentStock, i, STOCK_TRAY);
			if (SUCCESS_BURNING == nResult)
			{
				bNozzleHasChip[i] = TRUE;
				gm_nCurrentStock++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}
		else if (gm_bStart && gm_bStock//有补料模式，正常运行
			&& gm_nTotalInSize < gm_nSum - gm_nTotalStock//进料总数未达到tray盘料总量
			&& gm_nCurrentStock >= gm_nTotalStock)//补料盘已空
		{
			gm_bErrorFig[i] = ERROR_STOCKTRAY_EMPTY;
		}
		if (SUCCESS_BURNING == nResult)
			nEmptyNum--;
	}
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//吸嘴上没有芯片
	{
		return SUCCESS_BURNING;//跳出进料过程
	}

	//旋转90°
	if (gm_nRotateAngleIn != 0)
	{
		HANDLE hThread;
		static double angle = PI / (double)gm_nRotateAngleIn;
#if FOUR_NUZZLE
		SrMove(angle, RAXIS_1 - 3);
		//使用线程时，感觉传过去的参数没有负号了，回头有机会，调试运行看看具体什么原因
		//hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
		//CloseHandle(hThread);
#else
		if (gm_bNozzleUsed[0])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[1])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc1, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[2])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc2, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[3])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc3, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
#endif // FOUR_NUZZLE
	}

	//吸嘴到下相机，进行芯片检测
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//用于保存4个吸嘴的偏差值
	if (gm_bDownCameraIn)
	{
		SetDownCameraLightOn();
		for (int i = 3; i >=0; i--)//进料时逆序检查，出料时顺序检查
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i])
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);
				
				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>gm_uInfo.nPixelsPM_down*MAX_OFFSET || abs(posOffset[i].y)>gm_uInfo.nPixelsPM_down*MAX_OFFSET))
				{
					//如果匹配失败，则放入废料盒
					gm_nTotalCheckFail++;
					gm_nContinueCheckFail++;
					bMatchSuccess[i] = FALSE;
					continue;
				}
				else if (SUCCESS_BURNING != nResult)
				{
					gm_bErrorFig[i] = nResult;
					continue;
				}
				gm_nContinueCheckFail = 0;
				//posOffset[i].x = 0;//因双吸嘴使用下相机时偏差过大，此为临时解决方案
				//posOffset[i].y = 0;
			}
		}
		SetDownCameraLightOff();
	}

	//将芯片放入烧录座
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		if (!bNozzleHasChip[i])
			continue;

		if (!CheckNegtiveMeter(i))
		{
			gm_bErrorFig[i] = CHIP_MISS;
			continue;
		}

		if (gm_bNozzleUsed[i] && bNozzleHasChip[i] && bMatchSuccess[i])
		{
			for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
			{
				if (gm_sBurningStatus[nModel].bEmpty[nSocket] == TRUE
					&& gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket])
				{
					nResult = PutChipProc(nModel*MAXNUM_OF_SOCKET + nSocket, i, posOffset[i], SOCKET_NUM);
					if (SUCCESS_BURNING == nResult)//放入socket
					{
						gm_sBurningStatus[nModel].bEmpty[nSocket] = FALSE;
						gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_READY;//未烧录状态
						gm_sBurningStatus[nModel].dwNoEmptyTime[nSocket] = GetTickCount();
						break;
					}
					else
					{
						gm_bErrorFig[i] = nResult;
						break;
					}
				}
				else
				{
					continue;
				}
			}
		}
		else if (!bMatchSuccess[i])
		{
			nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
			ZComeBack();
			if (SUCCESS_BURNING == nResult)//匹配失败，放入废料盘
			{
				gm_nTotalFailSize++;
				gm_nCurrentFailSize++;
				gm_nCurrentOutSize++;//这也是出料
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}
	}

	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{//如果当前模组中还有已使用，但为空的socket，则跳过该模组的放入优先级置零语句
		if (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket]
			&& gm_sBurningStatus[nModel].bEmpty[nSocket] == TRUE)
		{
			gm_nModelPutPreference[nModel]++;
			goto lablePut;
		}
	}
	gm_nModelPutPreference[nModel] = 0;
lablePut:
	AddPutPreference();
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	StartProgramProc(nModel, lParam);
	//gm_sBurningStatus[nModel].dwStartTime = GetTickCount();

	//旋转90°
	if (gm_nRotateAngleIn != 0)
	{
		HANDLE hThread;
		static double angle = -PI / (double)gm_nRotateAngleIn;
#if FOUR_NUZZLE
		SrMove(angle, RAXIS_1 - 3);

		//hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
		//CloseHandle(hThread);
#else
		if (gm_bNozzleUsed[0])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[1])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc1, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[2])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc2, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[3])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc3, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
#endif // FOUR_NUZZLE
	}


	return SUCCESS_BURNING;
}

//出料过程
UINT OutputProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//吸嘴上是否有芯片
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//匹配是否成功
	int nBurningSataus[4]{ 0, 0, 0, 0 };//0为未烧录状态，1为正在烧录，2为烧录成功，3为烧录失败
	int nModel = -1;
	UINT nResult = 0;
	int nGetNum = 0;//当前模组中待取出的芯片的数量
	nModel = CheckChipForMoveOutSocket(0);
	if (nModel == -1 || nModel == -2)
		return PROGRAM_BUSY;
	//检查下拉气缸信号
	SetCylinderDown(nModel);
	DWORD dwStart = ::GetTickCount();
	while (!CheckModelCylinderPressed(nModel))
	{
		Sleep(100);
		DWORD dwElapse = ::GetTickCount() - dwStart;
		if (dwElapse > 3000)
		{
			CString str;
			str.Format("气缸%d下拉信号异常。", nModel + 1);
			ShowErrorMessage(str);
			return ERROR_SOCKET_PRESSED;
		}
	}
	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{
		if (gm_uInfo.nWorkMode == TAPE_TO_TRAY)//编带进tray盘出时，直到把tray盘放满
		{
			if (((nGetNum + gm_nCurrentGoodSize) < (gm_dataTray.xn*gm_dataTray.yn))
				&& gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket]
				&& gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE
				&& (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_OK
					|| gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_NG))
			{
				nGetNum++;
			}

		}
		else
		{
			if (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket]
				&& gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE
				&& (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_OK
					|| gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_NG))
			{
				nGetNum++;
			}
		}
	}

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		//if (!CheckChipCanBeGet(SOCKET_NUM, i))
		//	break;
		if (nGetNum == 0)
			break;
		for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
		{
			if (gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE
				&& gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket]
				&& (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_OK
					|| gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_NG))
			{
				//烧录时间过短，则报警提示
				if (GetTickCount() - gm_sBurningStatus[nModel].dwNoEmptyTime[nSocket] < gm_uInfo.nElapsed)
					return PROGRAMER_ERROR;

				nResult = GetChipProc(nModel*MAXNUM_OF_SOCKET + nSocket, i, SOCKET_NUM);
				if (SUCCESS_BURNING == nResult)
				{
					bNozzleHasChip[i] = TRUE;
					gm_sBurningStatus[nModel].bEmpty[nSocket] = TRUE;
					nBurningSataus[i] = gm_sBurningStatus[nModel].nAdpStatus[nSocket];
					gm_nCurrentOutSize++;//吸嘴每取一个，出料就增加一个
					if (nBurningSataus[i] == FP_OK)
					{
						gm_sBurningStatus[nModel].nContinueFailSize[nSocket] = 0;
						gm_nDisplayGoodForSocket[nModel*MAXNUM_OF_SOCKET + nSocket]++;
						pMainWnd->DisplayGood(nModel*MAXNUM_OF_SOCKET + nSocket, gm_nDisplayGoodForSocket[nModel*MAXNUM_OF_SOCKET + nSocket]);
					}
					else
					{
						gm_sBurningStatus[nModel].nContinueFailSize[nSocket]++;
						if (gm_sBurningStatus[nModel].nContinueFailSize[nSocket] >= 3)
						{
							gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket] = FALSE;//连续NG3次，禁用该烧录座
							pMainWnd->InitSockStatus(nModel*MAXNUM_OF_SOCKET + nSocket);
							//StartProgramProc(nModel, lParam);
							CString str;
							str.Format("烧录座%d-%d连续抛料3次，已禁用", nModel+1, nSocket+1);
							pMainWnd->AppendLogMessage(str);
						}
						gm_nTotalFPFail++;
						gm_nDisplayFailForSocket[nModel*MAXNUM_OF_SOCKET + nSocket]++;
						pMainWnd->DisplayFail(nModel*MAXNUM_OF_SOCKET + nSocket, gm_nDisplayFailForSocket[nModel*MAXNUM_OF_SOCKET + nSocket]);
					}
					nGetNum--;
					break;
				}
				else
				{
					//
					//gm_sBurningStatus[nModel].bEmpty[nSocket] = TRUE; 
					//gm_nCurrentOutSize++;//吸取失败就算取出
					nBurningSataus[i] = FP_NG;
					gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_NG;

					CString str;
					str.Format("烧录座#%d-%d芯片未取出。", nModel + 1, nSocket + 1);
					pMainWnd->AppendLogMessage(str);
					gm_bErrorFig[i] = nResult;
					BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
					if (!bHasGetChip)//吸嘴上没有芯片
					{
						if (!ZComeBack())
							return ERROR_XYMOVE_ZCOMEBACK;
						return SUCCESS_BURNING;//跳出出料过程
					}
					break;
				}
			}
			else
			{
				continue;
			}
		}
	}

	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{
		if (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket]
			&& gm_sBurningStatus[nModel].bEmpty[nSocket] == FALSE
			&& (gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_OK || gm_sBurningStatus[nModel].nAdpStatus[nSocket] == FP_NG))
		{
			gm_nModelGetPreference[nModel]++;
			goto lableGet;//只要模组中还有一个烧录座里面有烧录好的芯片，就跳过该模组的取优先级置零
		}
	}
	gm_nModelGetPreference[nModel] = 0;
lableGet:
	AddGetPreference();

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//吸嘴上没有芯片
	{
		return SUCCESS_BURNING;//跳出出料过程
	}
	//if (CheckNegtiveMeter(0))
	//	return ERROR_GET_CHIP;
	//吸嘴到下相机，进行芯片检测
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//用于保存4个吸嘴的偏差值
	if (/*!gm_bFinish&&*/gm_bDownCameraOut)//结束时不进行检测，直接放入补料盘
	{
		SetDownCameraLightOn();
		for (int i = 0; i < 4; i++)
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i] && nBurningSataus[i] == FP_OK)
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);
				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>gm_uInfo.nPixelsPM_down * MAX_OFFSET || abs(posOffset[i].y)>gm_uInfo.nPixelsPM_down * MAX_OFFSET))
				{
					//如果匹配失败，则放入废料盒
					gm_nTotalCheckFail++;
					bMatchSuccess[i] = FALSE;
					gm_nContinueCheckFail++;
					continue;
				}
				else if (SUCCESS_BURNING != nResult)
				{
					gm_bErrorFig[i] = nResult;
					gm_nContinueCheckFail++;
					continue;
				}
				gm_nContinueCheckFail = 0;
			}
		}
		SetDownCameraLightOff();
	}

	//旋转90°
	if (gm_nRotateAngleOut != 0)
	{
		HANDLE hThread;
		static double angle = PI / (double)gm_nRotateAngleOut;//进出料方向要相反
#if FOUR_NUZZLE
		SrMove(angle, RAXIS_1 - 3);

		//hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
		//CloseHandle(hThread);
#else
		if (gm_bNozzleUsed[0])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[1])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc1, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[2])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc2, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[3])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc3, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
#endif // FOUR_NUZZLE
	}

	//出料情况：
	//1.tray盘出料：正常运行，tray盘出料模式，tray盘进料数量大于当前ok数量，ok ng总量未达到工单数量，tray盘未放满
	//				结束时，无补料模式
	//2.tape出料：正常运行，tape出料模式，ok ng总量未达到工单数量
	//				结束时，
	//3.备料盘出料：正常运行，tray盘出料模式，tray盘进料数量小于等于当前ok数量，tray盘已满
	//				结束时，有补料模式
	//4.废料盘出料：烧录NG芯片，匹配失败芯片
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		if (!bNozzleHasChip[i])
			continue;

		if (!CheckNegtiveMeter(i))
		{
			gm_bErrorFig[i] = CHIP_MISS;
			bNozzleHasChip[i] = false;
			continue;
		}

		if (FP_OK == nBurningSataus[i] && TRUE == bMatchSuccess[i])
		{//烧录成功，匹配成功
			if (gm_bStart)//运行时
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode)
				{//tray盘出料
					if (gm_nCurrentInSize > gm_nCurrentGoodSize//当前tray盘进料大于当前OK料，也就是tray盘有地方放
						//&& gm_nTotalGoodSize/* + gm_nTotalFailSize*/ < gm_nSum
						&& gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//当前OK料小于tray盘规格
					{//放入tray盘
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
					else//放入备料盘
					{
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//放入补料盘
						{
							//gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nStockGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
				}
				else if (TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//当前OK料小于tray盘规格
					{//放入tray盘
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
				}
				else//OK料编带出料模式
				{//编带出料模式下，不用检查出料是否满，只要吸嘴上有芯片，就可以放入编带
				//需要检查出料报警
					if (!CheckFeederOutError())
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTapeOut(gm_nCurrentGoodSize);
							pMainWnd->TapeOutOk();
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;

							if (gm_bTapeCheck)
							{
								SetUpCameraLightOn();
								//在这里进行出料检查，如果芯片放偏，则报警
								Position2 posOffset{ 0,0 };
								Position posCurrentPut{ 0,0,0 };
								GetPosForTape("TapeOut", posCurrentPut);
								nResult = XYMove(
									posCurrentPut.x/* + gm_uInfo.nOffsetSocketX - (XGUIDE / gm_uInfo.nPixelsPM_down*posOffset.x)*/,
									posCurrentPut.y/* + gm_uInfo.nOffsetSocketY - (YGUIDE / gm_uInfo.nPixelsPM_down*posOffset.y)*/);

								if (!pMainWnd->CheckModel(posOffset, "tape_out"))
								{
									gm_bErrorFig[i] = ERROR_TAPE_OUT;
								}
								SetUpCameraLightOff();
							}
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
					else
					{//飞达出料报警，放入废料盘
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//放入废料盘
						{
							gm_nCurrentFailSize++;
							gm_nTotalFailSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
						gm_bErrorFig[i] = ERROR_TAPE_OUT;
					}
				}
			}
			else if (gm_bFinish)//结束时
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentInSize > gm_nCurrentGoodSize)//结束，tray盘还有空位置，放入tray盘或tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
					else if (/*gm_bStock && */gm_nCurrentStock > gm_nStockGoodSize)
					{//tray盘没位置，则放入补料盘
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//放入补料盘
						{
							//gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							gm_nStockGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
				}
				else if (TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//当前OK料小于tray盘规格
					{//放入tray盘
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTrayStatus(gm_nCurrentGoodSize, 1);
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}

					}
				}

				else//编带结束
				{
					if (!CheckFeederOutError())//结束，tray盘还有空位置，放入tray盘或tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//放入出料
						{
							pMainWnd->ChangeTapeOut(gm_nCurrentGoodSize);
							pMainWnd->TapeOutOk();
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
					}
					else
					{//飞达出料报警，放入废料盘
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//放入废料盘
						{
							gm_nCurrentFailSize++;
							gm_nTotalFailSize++;
							bNozzleHasChip[i] = FALSE;
						}
						else
						{
							gm_bErrorFig[i] = nResult;
						}
						gm_bErrorFig[i] = ERROR_TAPE_OUT;
					}
				}

			}
		}
		else//烧录NG 、匹配失败芯片处理过程
		{
			nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
			ZComeBack();
			if (SUCCESS_BURNING == nResult)//放入废料盘
			{
				gm_nCurrentFailSize++;
				gm_nTotalFailSize++;
				bNozzleHasChip[i] = FALSE;
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}

	}
	if (!ZComeBack())
	{
		//MessageBox(NULL, "ZComeBack Error of End Output proc", NULL, MB_ICONERROR);
		return ERROR_XYMOVE_ZCOMEBACK;
	}
	StartProgramProc(nModel,pMainWnd);
	//旋转90°
	if (gm_nRotateAngleOut != 0)
	{
		HANDLE hThread;
		static double angle = -PI / (double)gm_nRotateAngleOut;//进出料方向要相反
#if FOUR_NUZZLE
		SrMove(angle, RAXIS_1 - 3);

		//hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
		//CloseHandle(hThread);
#else
		if (gm_bNozzleUsed[0])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[1])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc1, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[2])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc2, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
		if (gm_bNozzleUsed[3])
		{
			hThread = CreateThread(NULL, 0, SrMoveProc3, (LPVOID)&angle, 0, NULL);
			CloseHandle(hThread);
		}
#endif // FOUR_NUZZLE
	}
	return SUCCESS_BURNING;
}
//判断烧录过程是否完成
//烧录完成的几种情况：
//1.无补料，无自动tray盘：	gm_nCurrentOutSize == gm_nSum
//2.有补料，无自动tray盘：	gm_nCurrentGoodSize == gm_nSum
//3.无补料，有自动tray盘：	gm_nCurrentOutSize == gm_nSum && tray盘进料没有感应到
//4.有补料，有自动tray盘：	gm_nCurrentGoodSize == gm_nSum && tray盘进料没有感应到
BOOL CheckBurningProcComplete(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	switch (gm_uInfo.nWorkMode)
	{
	case TRAY_TO_TRAY:
		if (!gm_bStock && !gm_bAutoTray)//无补料、无自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum 
				|| gm_nCurrentOutSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//有补料、无自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum 
				|| gm_nCurrentGoodSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				//if (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn)
				//	return FALSE;
				return TRUE;
			}
		}
		else if (!gm_bStock&&gm_bAutoTray)//无补料、有自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentOutSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
				TrayComplete();
				gm_bComplete = FALSE;
				gm_nCurrentTotalSize = 0;
				gm_nCurrentOutSize = 0;
				gm_nCurrentInSize = 0;
				gm_nCurrentGoodSize = 0;
				//gm_nCurrentFailSize = 0;
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				//XYMove(0, 0);
				pMainWnd->SetRunStatus("换盘中…");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//有补料、有自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentGoodSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
				TrayComplete();
				gm_bComplete = FALSE;
				gm_nCurrentTotalSize = 0;
				gm_nCurrentOutSize = 0;
				gm_nCurrentInSize = 0;
				gm_nCurrentGoodSize = 0;
				//gm_nCurrentFailSize = 0;
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				//XYMove(0, 0);
				pMainWnd->SetRunStatus("换盘中…");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		break;
	case TRAY_TO_TAPE:
		if (!gm_bStock && !gm_bAutoTray)//无补料、无自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum || gm_nCurrentOutSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//有补料、无自动tray盘
		{
			if (gm_nTotalGoodSize /*+ gm_nTotalFailSize*/ == gm_nSum || gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (!gm_bStock&&gm_bAutoTray)//无补料、有自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentInSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
				TrayComplete();
				gm_bComplete = FALSE;
				gm_nCurrentTotalSize = 0;
				gm_nCurrentOutSize = 0;
				gm_nCurrentInSize = 0;
				gm_nCurrentGoodSize = 0;
				//gm_nCurrentFailSize = 0;
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				//XYMove(0, 0);
				pMainWnd->SetRunStatus("换盘中…");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//有补料、有自动tray盘
		{
			if (gm_nTotalGoodSize == gm_nSum /*&& tray盘进料没有感应到*/)
			{
				return TRUE;
			}
			else if (gm_nCurrentInSize == gm_dataTray.xn*gm_dataTray.yn /*&& tray盘进料感应到*/)
			{
				pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
				TrayComplete();
				gm_bComplete = FALSE;
				gm_nCurrentTotalSize = 0;
				gm_nCurrentOutSize = 0;
				gm_nCurrentInSize = 0;
				gm_nCurrentGoodSize = 0;
				//gm_nCurrentFailSize = 0;
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				//XYMove(0, 0);
				pMainWnd->SetRunStatus("换盘中…");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		break;
	case TAPE_TO_TRAY:
		if (!gm_bStock && !gm_bAutoTray)//无补料、无自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum || gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//有补料、无自动tray盘
		{
			//tape进料不需要补料
		}
		else if (!gm_bStock&&gm_bAutoTray)//无补料、有自动tray盘
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
				TrayComplete();
				gm_bComplete = FALSE;
				gm_nCurrentTotalSize = 0;
				gm_nCurrentOutSize = 0;
				gm_nCurrentInSize = 0;
				gm_nCurrentGoodSize = 0;
				//gm_nCurrentFailSize = 0;
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				//XYMove(0, 0);
				pMainWnd->SetRunStatus("换盘中…");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//有补料、有自动tray盘
		{
			//tape进料不需要补料盘
		}
		break;
	case TAPE_TO_TAPE:
		if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)
		{
			return TRUE;
		}
		break;
	default:
		break;
	}
	return FALSE;
}

//检查tray盘到达信号
BOOL CheckAutoTrayArrived()
{
	DWORD dwStart = ::GetTickCount();
	while (!gm_bTrayArrived)
	{
		if (gm_bStop)
			return TRUE;
		DWORD nEp = ::GetTickCount() - dwStart;
		if (nEp > 1000 * 60 * 1)//换盘时间大于1分钟，则报警
		{
			return FALSE;
		}
		Sleep(1000);
	}
	return TRUE;
}
//烧录过程
UINT BurningProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	//BOOL bComplete = FALSE;
	HANDLE hThread;
	gm_bSaveConfig = FALSE;
	UINT nResult = 0, nResultA = 0, nResultB = 0;
	if (gm_bAutoTray)
	{
		pMainWnd->SetTimer(TIMER_CHECK_TRAY_ARRIVED, 1000, NULL);//开启检查Tray盘到达定时器
	}

	//执行过程循环
	while (TRUE)
	{
		if (gm_bTapeOutCheck)
		{
			pMainWnd->SetTimer(TIMER_CHECK_TAPE_STATUS, 1000, NULL);//检查编带机出料状态
		}

		SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
		SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
		//如果有自动tray盘机，则每次进料之前都检查是否tray盘机到位
		if (gm_bAutoTray && gm_bStart && (gm_nTotalInSize < gm_nSum - gm_nTotalStock))
		{
			if (!CheckAutoTrayArrived())
			{
				//pMainWnd->TrayComplete();
				TrayComplete();
				ShowErrorMessage("tray盘到达超时.", pMainWnd);
				break;
			}
			//pMainWnd->SetRunStatus("Running...");
		}
		if (!CheckDoorOpen())
		{
			ShowErrorMessage("请关闭前门", pMainWnd);
			break;
		}
		if (!CheckPositiveMeter())
		{
			ShowErrorMessage("气压过低", pMainWnd);
			break;
		}
		//暂停时，退出循环
		if (gm_bStop)
		{
			break;
		}

		//只有运行时，需要进料------------------------------------------------
		//执行InputProc的集中情况
		//1.Tray盘进料，有补料：
		if (gm_bStart && gm_nTotalInSize + gm_nCurrentStock < gm_nSum)//进料全部用完，不再执行进料过程
		{
			if (gm_bConvertChip)
				nResultA = ConvertChipProc(pMainWnd);
			else
				nResultA = InputProc(pMainWnd);
			if (SUCCESS_BURNING != nResultA && PROGRAM_BUSY != nResultA)
			{
				ShowErrorMessage(nResultA, 0, pMainWnd);
				break;
			}
			for (int i = 0; i < 4; i++)
			{
				if (gm_bNozzleUsed[i])
				{
					if (SUCCESS_BURNING != gm_bErrorFig[i])
					{
						ShowErrorMessage(gm_bErrorFig[i], i, pMainWnd);
						for (int j = 0; j < 4; j++)
						{
							gm_bErrorFig[j] = 0;
						}
						goto endofwhile;
					}
				}
			}
			//-------------------------------------------------------------
			if (gm_nContinueCheckFail >= 2)
			{
				gm_nContinueCheckFail = 0;
				ShowErrorMessage("连续匹配失败，请检查相机是否正常工作。", pMainWnd);
				break;
			}
		}

		//如果有自动tray盘机，则每次进料之前都检查是否tray盘机到位
		if (gm_bAutoTray && gm_bStart && (gm_nTotalInSize < gm_nSum - gm_nTotalStock))
		{
			if (!CheckAutoTrayArrived())
			{
				//pMainWnd->TrayComplete();
				TrayComplete();
				ShowErrorMessage("tray盘到达超时.", pMainWnd);
				break;
			}
			//pMainWnd->SetRunStatus("Running...");
		}
		if (!CheckDoorOpen())
		{
			ShowErrorMessage("请关闭前门", pMainWnd);
			break;
		}
		if (!CheckPositiveMeter())
		{
			ShowErrorMessage("气压过低", pMainWnd);
			break;
		}
		//暂停时，退出循环
		if (gm_bStop)
		{
			break;
		}

		if (gm_bTapeOutCheck)
		{
			if ((TRAY_TO_TAPE == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode))//出料为编带时，进行检测	
			{
				if (FEEDER == 1)
				{
					if (CheckOutTapeIO())
					{
						ShowErrorMessage("出料料带未检测到，查看是否用完。", pMainWnd);
						break;
					}
					if (CheckOutTapeIOFilm())
					{
						ShowErrorMessage("出料料膜未检测到，查看是否用完。", pMainWnd);
						break;
					}
				}
				else
				{
					if (gm_bTapeStatus != 0)
					{
						Sleep(200);
					}
					switch (gm_bTapeStatus)
					{
					case 1:
						ShowErrorMessage("出料料带用完，请更换料带", pMainWnd);
						break;
					case 2:
						ShowErrorMessage("编带机原点感应错误，请检查原点光电。", pMainWnd);
						break;
					case 3:
						ShowErrorMessage("料膜用完，请更换。", pMainWnd);
						break;
					case -1:
						ShowErrorMessage("与编带机通信异常。", pMainWnd);
						break;
					default:
						break;
					}
					if (gm_bTapeStatus != 0)
					{
						break;
					}
				}
			}
		}


		if(!gm_bConvertChip)
		//运行时或者结束时执行出料------------------------------------------
			nResultB = OutputProc(pMainWnd);
		if (SUCCESS_BURNING != nResultB && PROGRAM_BUSY != nResultB)
		{
			ShowErrorMessage(nResultB, 0, pMainWnd);
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			if (gm_bNozzleUsed[i])
			{
				if (SUCCESS_BURNING != gm_bErrorFig[i])
				{
					ShowErrorMessage(gm_bErrorFig[i], i, pMainWnd);
					for (int j = 0; j < 4; j++)
					{
						gm_bErrorFig[j] = 0;
					}
					goto endofwhile;//跳出while
				}
			}
		}
		//------------------------------------------------------------------
		if (gm_nContinueCheckFail >= 2)
		{
			gm_nContinueCheckFail = 0;
			ShowErrorMessage("连续匹配失败，请检查相机是否正常工作。", pMainWnd);
			break;
		}

		//保存烧录信息
		if (!gm_bSaveConfig)
		{
			hThread = CreateThread(NULL, 0, SaveConfigProc, (LPVOID)pMainWnd, 0, NULL);
			CloseHandle(hThread);
		}
		float yield = 0.0;
		pMainWnd->ShowGoodSize(gm_nTotalGoodSize);
		pMainWnd->ShowFailSize(gm_nTotalFPFail, gm_nTotalCheckFail);
		yield = (gm_nTotalGoodSize + gm_nTotalFailSize + gm_nStockGoodSize == 0) ? 0 : float(gm_nStockGoodSize + gm_nTotalGoodSize) / float(gm_nStockGoodSize + gm_nTotalGoodSize + gm_nTotalFailSize);
		pMainWnd->ShowYield(yield);
		gm_sdInfo.nUPH = gm_nTotalGoodSize / (pMainWnd->m_nTime / float(3600));
		CString str;
		str.Format("%d", gm_sdInfo.nUPH);
		pMainWnd->SetDlgItemTextA(IDC_MAIN_UPH, str);
		if (gm_nCurrentFailSize >= gm_dataNG.xn*gm_dataNG.yn - 2)
		{
			ShowErrorMessage("废料盘已满，请暂停，清空废料盘后再进行烧录。");
			gm_nCurrentFailSize = 0;
			break;
		}

		if (gm_bFinish)//结束时，如果烧录座中没有芯片，则表示结束过程完成
		{
			if (-2 == CheckChipForMoveOutSocket(0))
			{
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				pMainWnd->CreateLogfile();//结束时生成烧录日志
				SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
				SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
				XYMove(0, 0);
				break;
			}
		}
		//出料完成时，判断是否烧录完成
		if (gm_bStart)
		{
			if (CheckBurningProcComplete(lParam))
			{
				gm_bComplete = TRUE;
				break;
			}
		}

		if (PROGRAM_BUSY == nResultA && PROGRAM_BUSY == nResultB)
		{
			Sleep(100);
		}
		pMainWnd->SetRunStatus("Running...");
	}
endofwhile:
	gm_bStart = FALSE;
	gm_bFinish = FALSE;
	pMainWnd->StopPLC();
	pMainWnd->GetDlgItem(IDC_BTN_RUN)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_FINISH)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_EXIT)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_GOHOME)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_BTN_FP_NG)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM1)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM2)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM3)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM4)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM5)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM6)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM7)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM8)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM9)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_CHECK_PROGRAM10)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK11)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK12)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK13)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK14)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK21)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK22)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK23)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK24)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK31)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK32)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK33)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK34)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK41)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK42)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK43)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK44)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK51)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK52)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK53)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK54)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK61)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK62)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK63)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK64)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK71)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK72)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK73)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK74)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK81)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK82)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK83)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK84)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK91)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK92)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK93)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK94)->EnableWindow();

	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK101)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK102)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK103)->EnableWindow();
	pMainWnd->GetDlgItem(IDC_SOCKET_CHECK104)->EnableWindow();

	if (gm_bComplete)
	{
		gm_bComplete = FALSE;
		if (gm_bAutoTray)
		{
			pMainWnd->TrayComplete();//如果还有盘，则发送换盘信号
			TrayComplete();//IO通信
		}
		gm_nCurrentTotalSize = 0;
		gm_nCurrentOutSize = 0;
		gm_nCurrentInSize = 0;
		gm_nCurrentGoodSize = 0;
		//gm_nCurrentFailSize = 0;
		if (!ZComeBack())
			return ERROR_XYMOVE_ZCOMEBACK;
		SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
		SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
		XYMove(0, 0);
		ShowErrorMessage("烧录完成", lParam);
		pMainWnd->SetRunStatus("Complete!");
		if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)//烧录完成时才插入数据
		{
			pMainWnd->m_dlgExcel.InSertFPData();
			pMainWnd->CreateLogfile();
		}
		if (TAPE_TO_TAPE != gm_uInfo.nWorkMode)
			pMainWnd->WholeTrayRfresh();
	
		for (int i = 0; i < MAXNUMOFPROGRAMMER; i++)
		{
			if (CheckModelCylinderPressed(i))
				SetCylinderUp(i);
		}
	}

	if (gm_bAutoTray)
	{
		pMainWnd->KillTimer(TIMER_CHECK_TRAY_ARRIVED);//检查Tray盘到达信号
	}
	//if (gm_bTapeOutCheck && (FEEDER == 0))
	//{
	//	pMainWnd->KillTimer(TIMER_CHECK_TAPE_STATUS);//
	//}

	pMainWnd->KillTimer(TIMER_RUN_TIME);//关闭烧录时间定时器
	return 0;
}

DWORD WINAPI SaveConfigProc(
	LPVOID lpParameter   // thread data
)
{
	gm_bSaveConfig = TRUE;
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lpParameter;
	pMainWnd->SaveConfig();
	gm_bSaveConfig = FALSE;
	return 0;
}

void ShowErrorMessage(const char * chErrorMessage, LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	gm_logFile.Write(chErrorMessage);
	pMainWnd->AppendLogMessage(chErrorMessage);
	gm_bBeep = TRUE;
	AfxBeginThread(BeepThreadProc, NULL);
	pMainWnd->SetRunStatus("Error!", 1);
	AfxMessageBox(chErrorMessage);
	gm_bBeep = FALSE;
	pMainWnd->SetRunStatus("Checking...", 2);
	gm_logFile.Write("用户确认！");
}

//错误信息显示
void ShowErrorMessage(const char *chErrorMessage)
{
	//	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	gm_logFile.Write(chErrorMessage);
	gm_bBeep = TRUE;
	AfxBeginThread(BeepThreadProc, NULL);
	//	pMainWnd->SetRunStatus("Error!", 1);
	AfxMessageBox(chErrorMessage);
	gm_bBeep = FALSE;
	//	pMainWnd->SetRunStatus("Checking...", 2);
	gm_logFile.Write("用户确认！");
}

void ShowErrorMessage(UINT nErrorCode, int axis, LPVOID lParam)
{
	CString str;
	switch (nErrorCode)
	{
	case ERROR_STOCKTRAY_POS:
		str.Format("获取补料盘位置失败.");
		break;
	case ERROR_SOCKET_PRESSED:
		str.Format("下拉气缸失败.");
		break;
	case ERROR_NOZZLE_NOT_BOUND:
		str.Format("吸嘴%d,吸嘴未成功弹起.", axis);
		break;
	case ERROR_GET_CHIP:
		str.Format("吸嘴%d,未成功取到芯片.", axis);
		break;
	case ERROR_PUT_CHIP:
		str.Format("吸嘴%d,未成功放置芯片.", axis);
		break;
	case ERROR_XYMOVE:
		str.Format("XY轴运动错误.");
		break;
	case ERROR_STOCKTRAY_EMPTY:
		str.Format("补料盘已空，请更换补料盘，并点击重置补料盘按钮。");
		break;
	case ERROR_STOCKTRAY_FULL:
		str.Format("备料盘已满。");
		break;
	case ERROR_XYMOVE_ZCOMEBACK:
		str.Format("z轴回原异常");
		break;
	case ERROR_XYMOVE_NOZZLE_BACK:
		str.Format("吸嘴%d回原异常。", axis);
		break;
	case ERROR_IXLINE:
		str.Format("插补运动失败。");
		break;
	case ERROR_CAMERA_CHECK:
		str.Format("相机匹配失败。");
		break;
	case ERROR_TAPE_OUT:
		str.Format("编带机出料故障。");
		break;
	case ERROR_SCRAYTRAY_EMPTY:
		str.Format("废料盘已空。");
		break;
	case CHIP_MISS:
		str.Format("芯片掉落。");
		gm_nTotalCheckFail++;
		gm_nTotalFailSize++;
		break;
	case PROGRAMER_ERROR:
		str.Format("编程器烧录时间太短，检查编程器是否正常工作。");
		break;

	default:
		break;
	}
	ShowErrorMessage(str, lParam);
}



