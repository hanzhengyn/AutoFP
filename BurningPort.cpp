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
const int MAX_OFFSET = 2;//���ƥ��ƫ�2mm
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
//\brief  ���Ҫ�����socket�����ط������ȼ���ߵ�socket���
//\������ʱ��Ҫ��ȡоƬʱ��ȷ��ȡ������оƬ�еط���
//Ҳ����˵������ӽ���ȡоƬʱҪԤ����¼��
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
					break;//�����ģ������������¼��оƬ����������ģ��
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
	return index;//����ģ���
}
//Ԥ����¼��
int BookSocket(int nNozzle)
{
	int index = -1;
	int maxpreference = -1;
	if (index >= 0)
		gm_sBurningStatus[index / MAXNUMOFSOCKET].bBooking[index%MAXNUMOFSOCKET] = true;
	return index;
}

//----------------------------------------------------------------------------------
//\brief  �����Ҫ��socket��ȡ����оƬ������ȡ�����ȼ���ߵ�socket���
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
			//оƬ�ź�оƬ�� һ��ʱ���Ժ�δ��ʼ��¼�����ж�NG	
			if (bUsed[nSocket] && !bEmpty[nSocket] && FP_READY == nStatus[nSocket]
				&& gm_sBurningStatus[nModel].bStartBurnFlag
				&& (GetTickCount() - gm_sBurningStatus[nModel].dwStartTime > gm_uInfo.nElapsed))
			{
				//gm_sBurningStatus[nModel].bStartBurnFlag = false;
				gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_NG;
			}
			//оƬ��¼ʱ�䳬��һ��ʱ�䣬���ж�NG
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
		return -2;//��¼����û��оƬ��
	}

	return index;//����ģ���
}

//----------------------------------------------------------------------------------
//\brief  ���socket��оƬ�ܷ�ȡ���������ȡ��������true
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
//\brief  ���socket�Ƿ�Ϊ�գ����Ϊ�գ�����true
//----------------------------------------------------------------------------------
BOOL CheckSocketIsEmpty(int sock)
{
	int emptyValue = gm_sBurningStatus[sock / MAXNUMOFSOCKET].bEmpty[sock%MAXNUMOFSOCKET];
	return emptyValue;
}

//----------------------------------------------------------------------------------
//\brief  ����ȡ�����ȼ�
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
//\brief  ����������ȼ�
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
		//ģ����ֻҪ��һ����ʹ����Ϊ�յ�socket�������Ӹ�ģ��ķ������ȼ�
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
//\brief  ��ȡtray���е�presum��λ�õ�����pos
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
//\brief  ��ȡ�����̵�presum��λ�õ�����pos
//----------------------------------------------------------------------------------
UINT GetChipPosInStockTray(int presum, Position &pos)
{
	int nSupply = 0;
	//if (presum == gm_nTotalStock)
	//{
	//	//ShowErrorMessage("�������ѿգ�����������̡�");
	//	gm_nCurrentStock = 0;
	//	return ERROR_STOCKTRAY_EMPTY;
	//}
	//if (gm_bFinish)//����ʱ
	//{
	//	gm_nCurrentStock--;
	//	if (gm_nCurrentStock < 0)
	//	{
	//		//ShowErrorMessage("������������");
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
	//if (gm_bStart&&!gm_bFinish)//����ʱ
	//	nSupply++;
	return SUCCESS_BURNING;
}
//----------------------------------------------------------------------------------
//\brief  ��ȡ�����̵�presum��λ�õ�����pos
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
//\brief  ��ȡ�����������
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
//\brief  ��ȡ�����������
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
//\brief  ��ȡ��sockInx��socket������
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
//��ȡ��sockInx��socket������
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
//\brief  ȡоƬ����
//\param  nGetNumber [in] ȡ����λ�ñ��
//\param  nNozzleNumber [in] ������
//\param  nWorkMode [in] ����ģʽ
//\//return
//----------------------------------------------------------------------------------
UINT GetChipProc(int nGetNumber, int nNozzleNumber, int nWorkMode)
{
	CString strNozzle;//offset_nozzle1
	strNozzle.Format("offset_nozzle%d", nNozzleNumber + 1);
	Point offset_nozzle{ 0, 0 };//�������������ƫ��ֵ
	offset_nozzle = gm_dataPos.GetPlacePoint(strNozzle);
	Position posCurrentChip{ 0, 0, 0 };//��ȡоƬ������ֵ
	int getHeight = 0;//ȡƬ�߶�ƫ��ֵ
	int nResult = 0;
	//����
	if (TRAY_TO_TRAY == nWorkMode || TRAY_TO_TAPE == nWorkMode)//��tray��ȡ��
	{
		GetChipPosInTray(nGetNumber, posCurrentChip);
		offset_nozzle.x += gm_uInfo.nOffsetInX;
		offset_nozzle.y += gm_uInfo.nOffsetInY;
		getHeight = gm_uInfo.nGetTrayHeight;
	}
	else if (TAPE_TO_TRAY == nWorkMode || TAPE_TO_TAPE == nWorkMode)//�ӱ��ȡ��
	{
		GetPosForTape("TapeIn", posCurrentChip);
		offset_nozzle.x += gm_uInfo.nOffsetInX;
		offset_nozzle.y += gm_uInfo.nOffsetInY;
		getHeight = gm_uInfo.nGetTrayHeight;
	}
	else if (STOCK_TRAY == nWorkMode)//�Ӳ�����ȡ��
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
	//����
	else if (SOCKET_NUM == nWorkMode)//��socketȡ��
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

		if (SOCKET_NUM == nWorkMode)//�����socket��ȡ�ϣ����ظ�ȡ�ϣ�����ѹ��оƬ
			return ERROR_GET_CHIP;

		if (n > 0)
		{
			//ShowErrorMessage("оƬճ�������ϣ��޷��������á�");
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
//brief  ��оƬ����
//param  nPutNumber [in] �����λ�ñ��,������¼��Ϊ���ӱ�ţ�����tray��Ϊtray����оƬ��ţ����ڱ��Ϊ�������λ��
//param  nNozzleNumber [in] ������	0 1 2 3
//param  posOffset [in] ��Ƭƫ��
//param  nWorkMode [in] ����ģʽ
//return
//----------------------------------------------------------------------------------
UINT PutChipProc(int nPutNumber, int nNozzleNumber, Position2 &posOffset, int nWorkMode)
{
	CString strNozzle;
	strNozzle.Format("offset_nozzle%d", nNozzleNumber + 1);
	Point offset_nozzle{ 0,0 };
	offset_nozzle = gm_dataPos.GetPlacePoint(strNozzle);
	Position posCurrentPut{ 0,0,0 };
	double putHeight = 0;//����ȡ�Ÿ߶�ƫ��
	UINT nResult = 0;
	if (TRAY_TO_TRAY == nWorkMode || TAPE_TO_TRAY == nWorkMode)//����tray��
	{
		GetChipPosInTray(nPutNumber, posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetOutX;
		offset_nozzle.y += gm_uInfo.nOffsetOutY;
		putHeight = gm_uInfo.nPutTrayHeight;
	}
	else if (TRAY_TO_TAPE == nWorkMode || TAPE_TO_TAPE == nWorkMode)//������
	{
		GetPosForTape("TapeOut", posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetOutX;
		offset_nozzle.y += gm_uInfo.nOffsetOutY;
		putHeight = gm_uInfo.nPutTrayHeight;
	}
	else if (SOCKET_NUM == nWorkMode)//����socket
	{
		GetPosForSocket(nPutNumber, posCurrentPut);
		offset_nozzle.x += gm_uInfo.nOffsetSocketX;
		offset_nozzle.y += gm_uInfo.nOffsetSocketY;
		putHeight = gm_uInfo.nPutSocketHeight;
	}
	else if (SCRAP_TRAY == nWorkMode)//���������
	{
		ZComeBack();
		if (!GetChipPosInScrap(nPutNumber, posCurrentPut))
			return ERROR_SCRAYTRAY_EMPTY;
		putHeight = 0;
	}
	else if (STOCK_TRAY == nWorkMode)//���벹����
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

	SrMove(posOffset.phi, nNozzleNumber);//�̸�

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
			//ShowErrorMessage("оƬճ�������ϣ��޷��������á�");
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

	SrMove(-posOffset.phi, nNozzleNumber);//ת��ԭ��

	return SUCCESS_BURNING;
}
//----------------------------------------------------------------------------------
//\brief  оƬ�ƶ������������������
//\param  nNozzleNumber			[in] ������
//\param  posOffset				[out] ƫ��ֵ
//\param  lParam				[in] ������ָ��
//\return TRUE	--	ƥ��ɹ�
//\return FALSE	--	ƥ��ʧ��
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

//���������ӿ�ʼ��¼
void StartProgramProc(int nModel, LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	int nUsed = 0;//ÿ��ģ�������ʹ������
	int nTotalSocket = 0;//ÿ��ģ�����оƬ����������
	for (int k = 0; k < MAXNUM_OF_SOCKET; k++)//���ÿ��ģ���Ƿ����оƬ
	{
		if (gm_bSocketUsed[nModel * MAXNUM_OF_SOCKET + k])
			nUsed++;
		if ((gm_sBurningStatus[nModel].bEmpty[k] == FALSE) &&
			(gm_sBurningStatus[nModel].nAdpStatus[k] == FP_READY))
		{
			nTotalSocket++;
		}
	}
	if (nTotalSocket == nUsed)//���ĳ��ģ��оƬ���������������ף���ʼ��¼
	{
		SetCylinderUp(nModel);
		Sleep(100);
		pMainWnd->StartOneProgrammer(nModel + 1);
		gm_sBurningStatus[nModel].dwStartTime = GetTickCount();
		gm_sBurningStatus[nModel].bStartBurnFlag = true;
	}
	else if (gm_nTotalInSize + gm_nCurrentStock >= gm_nSum
		|| ((TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode))
		&&(!gm_bStock && gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn))//����޲��ϣ����̽������ʱ����Ƿ���Ҫǿ����¼
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
			//str.Format("��¼��δ����%d�����ֶ���¼��", nModel + 1);
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

//ת�Ϲ���
UINT ConvertChipProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//�����õ�оƬ��ʶ
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//�������ÿ��������оƬƥ���Ƿ�ɹ���ʶ��Ĭ��Ϊ�ɹ�
	UINT nResult = 0;

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	//�ӽ�����ȡоƬ
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		//1.tray��ȡ�ϣ��������У�tray�̽���ģʽ��tray���л����ϣ�
		if (gm_bStart//����ʱ
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray�̽���
			&& (gm_nTotalInSize < gm_nSum - gm_nTotalStock)//tray�̽�������δ����
			&& (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn))//��ǰtray��δȡ��
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTrayStatus(gm_nCurrentInSize, EMPTY_STATUS);
				bNozzleHasChip[i] = TRUE;//i����������оƬ
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//������뱣������
			}

		}
		//2.tapeȡ�ϣ��������У�tape����ģʽ��tape�л�����
		else if (gm_bStart//����ʱ
			&& (TAPE_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode)//tape����
			&& (gm_nTotalInSize < gm_nSum))//��¼����������ڴ���¼����
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTapeIn(gm_nCurrentInSize);
				pMainWnd->TapeInOk();
				bNozzleHasChip[i] = TRUE;//i����������оƬ
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//������뱣������
			}
		}
		//3.������ȡ�ϣ��в���ģʽ���������У�tray�̽���ģʽ��tray�������ϣ�tray��ok��δ������stock�������ϣ�
		//				�в���ģʽ������ʱ
		else if ((gm_bStart && gm_bStock//�в���ģʽ����������
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray�̽���ģʽ
			&& ((gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn) //��ǰtray������
				|| (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum - gm_nTotalStock))//��tray�̽�������
			&& (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//tray��ok��δ����
			&& gm_nCurrentStock < gm_nTotalStock)//stock��������

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
		else if (gm_bStart && gm_bStock//�в���ģʽ����������
			&& gm_nTotalInSize < gm_nSum - gm_nTotalStock//��������δ�ﵽtray��������
			&& gm_nCurrentStock >= gm_nTotalStock)//�������ѿ�
		{
			gm_bErrorFig[i] = ERROR_STOCKTRAY_EMPTY;
		}
	}
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//������û��оƬ
	{
		return SUCCESS_BURNING;//�������Ϲ���
	}

	//��ת90��
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

	//���쵽�����������оƬ���
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//���ڱ���4�������ƫ��ֵ
	if (gm_bDownCameraIn)
	{
		SetDownCameraLightOn();
		for (int i = 3; i >= 0; i--)//����ʱ�����飬����ʱ˳����
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i])
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);

				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>20 || abs(posOffset[i].y)>20))
				{
					//���ƥ��ʧ�ܣ��������Ϻ�
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
				//posOffset[i].x = 0;//��˫����ʹ�������ʱƫ����󣬴�Ϊ��ʱ�������
				//posOffset[i].y = 0;
			}
		}
		SetDownCameraLightOff();
	}
	int nBurningSataus[4]{ 2, 2, 2, 2 };//0Ϊδ��¼״̬��1Ϊ������¼��2Ϊ��¼�ɹ���3Ϊ��¼ʧ��

	//���������
	//1.tray�̳��ϣ��������У�tray�̳���ģʽ��tray�̽����������ڵ�ǰok������ok ng����δ�ﵽ����������tray��δ����
	//				����ʱ���޲���ģʽ
	//2.tape���ϣ��������У�tape����ģʽ��ok ng����δ�ﵽ��������
	//				����ʱ��
	//3.�����̳��ϣ��������У�tray�̳���ģʽ��tray�̽�������С�ڵ��ڵ�ǰok������tray������
	//				����ʱ���в���ģʽ
	//4.�����̳��ϣ���¼NGоƬ��ƥ��ʧ��оƬ
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
		{//��¼�ɹ���ƥ��ɹ�
			if (gm_bStart)//����ʱ
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{//tray�̳���
					if (gm_nCurrentInSize > gm_nCurrentGoodSize//��ǰtray�̽��ϴ��ڵ�ǰOK�ϣ�Ҳ����tray���еط���
															   //&& gm_nTotalGoodSize/* + gm_nTotalFailSize*/ < gm_nSum
						&& gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//��ǰOK��С��tray�̹��
					{//����tray��
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					else//���뱸����
					{
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//���벹����
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
				else//OK�ϱ������ģʽ
				{//�������ģʽ�£����ü������Ƿ�����ֻҪ��������оƬ���Ϳ��Է�����
				 //��Ҫ�����ϱ���
					if (!CheckFeederOutError())
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					{//�ɴ���ϱ��������������
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//���������
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
			else if (gm_bFinish)//����ʱ
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentInSize > gm_nCurrentGoodSize)//������tray�̻��п�λ�ã�����tray�̻�tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					{//tray��ûλ�ã�����벹����
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//���벹����
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
				else//�������
				{
					if (!CheckFeederOutError())//������tray�̻��п�λ�ã�����tray�̻�tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					{//�ɴ���ϱ��������������
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//���������
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
		else//��¼NGƥ��ʧ��оƬ�������
		{
			nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
			if (SUCCESS_BURNING == nResult)//���������
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
//���Ϲ���
UINT InputProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//�����õ�оƬ��ʶ
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//�������ÿ��������оƬƥ���Ƿ�ɹ���ʶ��Ĭ��Ϊ�ɹ�
	UINT nResult = 0;
	int nEmptyNum = 0;//��ø�ģ����ʹ����Ϊ�յ���������
	int nModel = CheckChipForMoveInSocket(0);//��ȡ�������ȼ���ߵ�ģ��
	if (nModel == -1)
		return PROGRAM_BUSY;
	//������������ź�
		SetCylinderDown(nModel);
		DWORD dwStart = ::GetTickCount();
		while (!CheckModelCylinderPressed(nModel))
		{
			Sleep(100);
			DWORD dwElapse = ::GetTickCount() - dwStart;
			if (dwElapse > 3000)
			{
				CString str;
				str.Format("����%d�����ź��쳣��", nModel + 1);
				ShowErrorMessage(str);
				return ERROR_SOCKET_PRESSED;
			}
		}

	//ÿ��ֻ����һ��ģ�飬����ģ��ȡ����
	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{
		if (gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket] && gm_sBurningStatus[nModel].bEmpty[nSocket] == TRUE)
		{
			nEmptyNum++;
		}
	}

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;
	//�ӽ�����ȡоƬ
	for (int i = 0; i < 4; i++)
	{
		if (!gm_bNozzleUsed[i])
			continue;
		if (nEmptyNum <= 0)
			break;
		//1.tray��ȡ�ϣ��������У�tray�̽���ģʽ��tray���л����ϣ�
		if (gm_bStart//����ʱ
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray�̽���
			&& (gm_nTotalInSize < gm_nSum - gm_nTotalStock)//tray�̽�������δ����
			&& (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn))//��ǰtray��δȡ��
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTrayStatus(gm_nCurrentInSize, EMPTY_STATUS);
				bNozzleHasChip[i] = TRUE;//i����������оƬ
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//������뱣������
			}

		}
		//2.tapeȡ�ϣ��������У�tape����ģʽ��tape�л�����
		else if (gm_bStart//����ʱ
			&& (TAPE_TO_TRAY == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode)//tape����
			&& (gm_nTotalInSize < gm_nSum))//��¼����������ڴ���¼����
		{
			nResult = GetChipProc(gm_nCurrentInSize, i, gm_uInfo.nWorkMode);
			if (SUCCESS_BURNING == nResult)
			{
				pMainWnd->ChangeTapeIn(gm_nCurrentInSize);
				pMainWnd->TapeInOk();
				bNozzleHasChip[i] = TRUE;//i����������оƬ
				gm_nCurrentInSize++;
				gm_nTotalInSize++;
			}
			else
			{
				gm_bErrorFig[i] = nResult;//������뱣������
			}
		}
		//3.������ȡ�ϣ��в���ģʽ���������У�tray�̽���ģʽ��tray�������ϣ�tray��ok��δ������stock�������ϣ�
		//				�в���ģʽ������ʱ
		else if ((gm_bStart && gm_bStock//�в���ģʽ����������
			&& (TRAY_TO_TRAY == gm_uInfo.nWorkMode || TRAY_TO_TAPE == gm_uInfo.nWorkMode)//tray�̽���ģʽ
			&& ((gm_nCurrentInSize >= gm_dataTray.xn*gm_dataTray.yn) //��ǰtray������
				|| (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum - gm_nTotalStock))//��tray�̽�������
			&& (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//tray��ok��δ����
			&& gm_nCurrentStock < gm_nTotalStock)//stock��������

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
		else if (gm_bStart && gm_bStock//�в���ģʽ����������
			&& gm_nTotalInSize < gm_nSum - gm_nTotalStock//��������δ�ﵽtray��������
			&& gm_nCurrentStock >= gm_nTotalStock)//�������ѿ�
		{
			gm_bErrorFig[i] = ERROR_STOCKTRAY_EMPTY;
		}
		if (SUCCESS_BURNING == nResult)
			nEmptyNum--;
	}
	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//������û��оƬ
	{
		return SUCCESS_BURNING;//�������Ϲ���
	}

	//��ת90��
	if (gm_nRotateAngleIn != 0)
	{
		HANDLE hThread;
		static double angle = PI / (double)gm_nRotateAngleIn;
#if FOUR_NUZZLE
		SrMove(angle, RAXIS_1 - 3);
		//ʹ���߳�ʱ���о�����ȥ�Ĳ���û�и����ˣ���ͷ�л��ᣬ�������п�������ʲôԭ��
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

	//���쵽�����������оƬ���
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//���ڱ���4�������ƫ��ֵ
	if (gm_bDownCameraIn)
	{
		SetDownCameraLightOn();
		for (int i = 3; i >=0; i--)//����ʱ�����飬����ʱ˳����
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i])
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);
				
				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>gm_uInfo.nPixelsPM_down*MAX_OFFSET || abs(posOffset[i].y)>gm_uInfo.nPixelsPM_down*MAX_OFFSET))
				{
					//���ƥ��ʧ�ܣ��������Ϻ�
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
				//posOffset[i].x = 0;//��˫����ʹ�������ʱƫ����󣬴�Ϊ��ʱ�������
				//posOffset[i].y = 0;
			}
		}
		SetDownCameraLightOff();
	}

	//��оƬ������¼��
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
					if (SUCCESS_BURNING == nResult)//����socket
					{
						gm_sBurningStatus[nModel].bEmpty[nSocket] = FALSE;
						gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_READY;//δ��¼״̬
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
			if (SUCCESS_BURNING == nResult)//ƥ��ʧ�ܣ����������
			{
				gm_nTotalFailSize++;
				gm_nCurrentFailSize++;
				gm_nCurrentOutSize++;//��Ҳ�ǳ���
			}
			else
			{
				gm_bErrorFig[i] = nResult;
			}
		}
	}

	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{//�����ǰģ���л�����ʹ�ã���Ϊ�յ�socket����������ģ��ķ������ȼ��������
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

	//��ת90��
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

//���Ϲ���
UINT OutputProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	BOOL bNozzleHasChip[4]{ FALSE, FALSE, FALSE, FALSE };//�������Ƿ���оƬ
	BOOL bMatchSuccess[4]{ TRUE, TRUE, TRUE, TRUE };//ƥ���Ƿ�ɹ�
	int nBurningSataus[4]{ 0, 0, 0, 0 };//0Ϊδ��¼״̬��1Ϊ������¼��2Ϊ��¼�ɹ���3Ϊ��¼ʧ��
	int nModel = -1;
	UINT nResult = 0;
	int nGetNum = 0;//��ǰģ���д�ȡ����оƬ������
	nModel = CheckChipForMoveOutSocket(0);
	if (nModel == -1 || nModel == -2)
		return PROGRAM_BUSY;
	//������������ź�
	SetCylinderDown(nModel);
	DWORD dwStart = ::GetTickCount();
	while (!CheckModelCylinderPressed(nModel))
	{
		Sleep(100);
		DWORD dwElapse = ::GetTickCount() - dwStart;
		if (dwElapse > 3000)
		{
			CString str;
			str.Format("����%d�����ź��쳣��", nModel + 1);
			ShowErrorMessage(str);
			return ERROR_SOCKET_PRESSED;
		}
	}
	for (int nSocket = 0; nSocket < MAXNUM_OF_SOCKET; nSocket++)
	{
		if (gm_uInfo.nWorkMode == TAPE_TO_TRAY)//�����tray�̳�ʱ��ֱ����tray�̷���
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
				//��¼ʱ����̣��򱨾���ʾ
				if (GetTickCount() - gm_sBurningStatus[nModel].dwNoEmptyTime[nSocket] < gm_uInfo.nElapsed)
					return PROGRAMER_ERROR;

				nResult = GetChipProc(nModel*MAXNUM_OF_SOCKET + nSocket, i, SOCKET_NUM);
				if (SUCCESS_BURNING == nResult)
				{
					bNozzleHasChip[i] = TRUE;
					gm_sBurningStatus[nModel].bEmpty[nSocket] = TRUE;
					nBurningSataus[i] = gm_sBurningStatus[nModel].nAdpStatus[nSocket];
					gm_nCurrentOutSize++;//����ÿȡһ�������Ͼ�����һ��
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
							gm_bSocketUsed[nModel*MAXNUM_OF_SOCKET + nSocket] = FALSE;//����NG3�Σ����ø���¼��
							pMainWnd->InitSockStatus(nModel*MAXNUM_OF_SOCKET + nSocket);
							//StartProgramProc(nModel, lParam);
							CString str;
							str.Format("��¼��%d-%d��������3�Σ��ѽ���", nModel+1, nSocket+1);
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
					//gm_nCurrentOutSize++;//��ȡʧ�ܾ���ȡ��
					nBurningSataus[i] = FP_NG;
					gm_sBurningStatus[nModel].nAdpStatus[nSocket] = FP_NG;

					CString str;
					str.Format("��¼��#%d-%dоƬδȡ����", nModel + 1, nSocket + 1);
					pMainWnd->AppendLogMessage(str);
					gm_bErrorFig[i] = nResult;
					BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
					if (!bHasGetChip)//������û��оƬ
					{
						if (!ZComeBack())
							return ERROR_XYMOVE_ZCOMEBACK;
						return SUCCESS_BURNING;//�������Ϲ���
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
			goto lableGet;//ֻҪģ���л���һ����¼����������¼�õ�оƬ����������ģ���ȡ���ȼ�����
		}
	}
	gm_nModelGetPreference[nModel] = 0;
lableGet:
	AddGetPreference();

	if (!ZComeBack())
		return ERROR_XYMOVE_ZCOMEBACK;

	BOOL bHasGetChip = bNozzleHasChip[0] || bNozzleHasChip[1] || bNozzleHasChip[2] || bNozzleHasChip[3];
	if (!bHasGetChip)//������û��оƬ
	{
		return SUCCESS_BURNING;//�������Ϲ���
	}
	//if (CheckNegtiveMeter(0))
	//	return ERROR_GET_CHIP;
	//���쵽�����������оƬ���
	Position2 posOffset[4]{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } };//���ڱ���4�������ƫ��ֵ
	if (/*!gm_bFinish&&*/gm_bDownCameraOut)//����ʱ�����м�⣬ֱ�ӷ��벹����
	{
		SetDownCameraLightOn();
		for (int i = 0; i < 4; i++)
		{
			if (gm_bNozzleUsed[i] && bNozzleHasChip[i] && nBurningSataus[i] == FP_OK)
			{
				nResult = CheckChipProc(i, posOffset[i], lParam);
				if (ERROR_CAMERA_CHECK == nResult || (abs(posOffset[i].x)>gm_uInfo.nPixelsPM_down * MAX_OFFSET || abs(posOffset[i].y)>gm_uInfo.nPixelsPM_down * MAX_OFFSET))
				{
					//���ƥ��ʧ�ܣ��������Ϻ�
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

	//��ת90��
	if (gm_nRotateAngleOut != 0)
	{
		HANDLE hThread;
		static double angle = PI / (double)gm_nRotateAngleOut;//�����Ϸ���Ҫ�෴
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

	//���������
	//1.tray�̳��ϣ��������У�tray�̳���ģʽ��tray�̽����������ڵ�ǰok������ok ng����δ�ﵽ����������tray��δ����
	//				����ʱ���޲���ģʽ
	//2.tape���ϣ��������У�tape����ģʽ��ok ng����δ�ﵽ��������
	//				����ʱ��
	//3.�����̳��ϣ��������У�tray�̳���ģʽ��tray�̽�������С�ڵ��ڵ�ǰok������tray������
	//				����ʱ���в���ģʽ
	//4.�����̳��ϣ���¼NGоƬ��ƥ��ʧ��оƬ
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
		{//��¼�ɹ���ƥ��ɹ�
			if (gm_bStart)//����ʱ
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode)
				{//tray�̳���
					if (gm_nCurrentInSize > gm_nCurrentGoodSize//��ǰtray�̽��ϴ��ڵ�ǰOK�ϣ�Ҳ����tray���еط���
						//&& gm_nTotalGoodSize/* + gm_nTotalFailSize*/ < gm_nSum
						&& gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//��ǰOK��С��tray�̹��
					{//����tray��
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					else//���뱸����
					{
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//���벹����
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
					if (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//��ǰOK��С��tray�̹��
					{//����tray��
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
				else//OK�ϱ������ģʽ
				{//�������ģʽ�£����ü������Ƿ�����ֻҪ��������оƬ���Ϳ��Է�����
				//��Ҫ�����ϱ���
					if (!CheckFeederOutError())
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
						{
							pMainWnd->ChangeTapeOut(gm_nCurrentGoodSize);
							pMainWnd->TapeOutOk();
							gm_nCurrentGoodSize++;
							gm_nTotalGoodSize++;
							bNozzleHasChip[i] = FALSE;

							if (gm_bTapeCheck)
							{
								SetUpCameraLightOn();
								//��������г��ϼ�飬���оƬ��ƫ���򱨾�
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
					{//�ɴ���ϱ��������������
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//���������
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
			else if (gm_bFinish)//����ʱ
			{
				if (TRAY_TO_TRAY == gm_uInfo.nWorkMode)
				{
					if (gm_nCurrentInSize > gm_nCurrentGoodSize)//������tray�̻��п�λ�ã�����tray�̻�tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					{//tray��ûλ�ã�����벹����
						nResult = PutChipProc(gm_nStockGoodSize, i, posOffset[i], STOCK_TRAY);
						if (SUCCESS_BURNING == nResult)//���벹����
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
					if (gm_nCurrentGoodSize < gm_dataTray.xn*gm_dataTray.yn)//��ǰOK��С��tray�̹��
					{//����tray��
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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

				else//�������
				{
					if (!CheckFeederOutError())//������tray�̻��п�λ�ã�����tray�̻�tape
					{
						nResult = PutChipProc(gm_nCurrentGoodSize, i, posOffset[i], gm_uInfo.nWorkMode);
						if (SUCCESS_BURNING == nResult)//�������
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
					{//�ɴ���ϱ��������������
						nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
						if (SUCCESS_BURNING == nResult)//���������
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
		else//��¼NG ��ƥ��ʧ��оƬ�������
		{
			nResult = PutChipProc(gm_nCurrentFailSize, i, posOffset[i], SCRAP_TRAY);
			ZComeBack();
			if (SUCCESS_BURNING == nResult)//���������
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
	//��ת90��
	if (gm_nRotateAngleOut != 0)
	{
		HANDLE hThread;
		static double angle = -PI / (double)gm_nRotateAngleOut;//�����Ϸ���Ҫ�෴
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
//�ж���¼�����Ƿ����
//��¼��ɵļ��������
//1.�޲��ϣ����Զ�tray�̣�	gm_nCurrentOutSize == gm_nSum
//2.�в��ϣ����Զ�tray�̣�	gm_nCurrentGoodSize == gm_nSum
//3.�޲��ϣ����Զ�tray�̣�	gm_nCurrentOutSize == gm_nSum && tray�̽���û�и�Ӧ��
//4.�в��ϣ����Զ�tray�̣�	gm_nCurrentGoodSize == gm_nSum && tray�̽���û�и�Ӧ��
BOOL CheckBurningProcComplete(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	switch (gm_uInfo.nWorkMode)
	{
	case TRAY_TO_TRAY:
		if (!gm_bStock && !gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum 
				|| gm_nCurrentOutSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum 
				|| gm_nCurrentGoodSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				//if (gm_nCurrentInSize < gm_dataTray.xn*gm_dataTray.yn)
				//	return FALSE;
				return TRUE;
			}
		}
		else if (!gm_bStock&&gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentOutSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//��������̣����ͻ����ź�
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
				pMainWnd->SetRunStatus("�����С�");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize >= gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentGoodSize >= gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//��������̣����ͻ����ź�
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
				pMainWnd->SetRunStatus("�����С�");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		break;
	case TRAY_TO_TAPE:
		if (!gm_bStock && !gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum || gm_nCurrentOutSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize /*+ gm_nTotalFailSize*/ == gm_nSum || gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (!gm_bStock&&gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentInSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//��������̣����ͻ����ź�
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
				pMainWnd->SetRunStatus("�����С�");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize == gm_nSum /*&& tray�̽���û�и�Ӧ��*/)
			{
				return TRUE;
			}
			else if (gm_nCurrentInSize == gm_dataTray.xn*gm_dataTray.yn /*&& tray�̽��ϸ�Ӧ��*/)
			{
				pMainWnd->TrayComplete();//��������̣����ͻ����ź�
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
				pMainWnd->SetRunStatus("�����С�");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		break;
	case TAPE_TO_TRAY:
		if (!gm_bStock && !gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum || gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				return TRUE;
			}
		}
		else if (gm_bStock && !gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			//tape���ϲ���Ҫ����
		}
		else if (!gm_bStock&&gm_bAutoTray)//�޲��ϡ����Զ�tray��
		{
			if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)
			{
				return TRUE;
			}
			else if (gm_nCurrentGoodSize == gm_dataTray.xn*gm_dataTray.yn)
			{
				pMainWnd->TrayComplete();//��������̣����ͻ����ź�
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
				pMainWnd->SetRunStatus("�����С�");
				pMainWnd->WholeTrayRfresh();
				return FALSE;
			}
		}
		else if (gm_bStock&&gm_bAutoTray)//�в��ϡ����Զ�tray��
		{
			//tape���ϲ���Ҫ������
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

//���tray�̵����ź�
BOOL CheckAutoTrayArrived()
{
	DWORD dwStart = ::GetTickCount();
	while (!gm_bTrayArrived)
	{
		if (gm_bStop)
			return TRUE;
		DWORD nEp = ::GetTickCount() - dwStart;
		if (nEp > 1000 * 60 * 1)//����ʱ�����1���ӣ��򱨾�
		{
			return FALSE;
		}
		Sleep(1000);
	}
	return TRUE;
}
//��¼����
UINT BurningProc(LPVOID lParam)
{
	CAutoFPDlg *pMainWnd = (CAutoFPDlg*)lParam;
	//BOOL bComplete = FALSE;
	HANDLE hThread;
	gm_bSaveConfig = FALSE;
	UINT nResult = 0, nResultA = 0, nResultB = 0;
	if (gm_bAutoTray)
	{
		pMainWnd->SetTimer(TIMER_CHECK_TRAY_ARRIVED, 1000, NULL);//�������Tray�̵��ﶨʱ��
	}

	//ִ�й���ѭ��
	while (TRUE)
	{
		if (gm_bTapeOutCheck)
		{
			pMainWnd->SetTimer(TIMER_CHECK_TAPE_STATUS, 1000, NULL);//�����������״̬
		}

		SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
		SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
		//������Զ�tray�̻�����ÿ�ν���֮ǰ������Ƿ�tray�̻���λ
		if (gm_bAutoTray && gm_bStart && (gm_nTotalInSize < gm_nSum - gm_nTotalStock))
		{
			if (!CheckAutoTrayArrived())
			{
				//pMainWnd->TrayComplete();
				TrayComplete();
				ShowErrorMessage("tray�̵��ﳬʱ.", pMainWnd);
				break;
			}
			//pMainWnd->SetRunStatus("Running...");
		}
		if (!CheckDoorOpen())
		{
			ShowErrorMessage("��ر�ǰ��", pMainWnd);
			break;
		}
		if (!CheckPositiveMeter())
		{
			ShowErrorMessage("��ѹ����", pMainWnd);
			break;
		}
		//��ͣʱ���˳�ѭ��
		if (gm_bStop)
		{
			break;
		}

		//ֻ������ʱ����Ҫ����------------------------------------------------
		//ִ��InputProc�ļ������
		//1.Tray�̽��ϣ��в��ϣ�
		if (gm_bStart && gm_nTotalInSize + gm_nCurrentStock < gm_nSum)//����ȫ�����꣬����ִ�н��Ϲ���
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
				ShowErrorMessage("����ƥ��ʧ�ܣ���������Ƿ�����������", pMainWnd);
				break;
			}
		}

		//������Զ�tray�̻�����ÿ�ν���֮ǰ������Ƿ�tray�̻���λ
		if (gm_bAutoTray && gm_bStart && (gm_nTotalInSize < gm_nSum - gm_nTotalStock))
		{
			if (!CheckAutoTrayArrived())
			{
				//pMainWnd->TrayComplete();
				TrayComplete();
				ShowErrorMessage("tray�̵��ﳬʱ.", pMainWnd);
				break;
			}
			//pMainWnd->SetRunStatus("Running...");
		}
		if (!CheckDoorOpen())
		{
			ShowErrorMessage("��ر�ǰ��", pMainWnd);
			break;
		}
		if (!CheckPositiveMeter())
		{
			ShowErrorMessage("��ѹ����", pMainWnd);
			break;
		}
		//��ͣʱ���˳�ѭ��
		if (gm_bStop)
		{
			break;
		}

		if (gm_bTapeOutCheck)
		{
			if ((TRAY_TO_TAPE == gm_uInfo.nWorkMode || TAPE_TO_TAPE == gm_uInfo.nWorkMode))//����Ϊ���ʱ�����м��	
			{
				if (FEEDER == 1)
				{
					if (CheckOutTapeIO())
					{
						ShowErrorMessage("�����ϴ�δ��⵽���鿴�Ƿ����ꡣ", pMainWnd);
						break;
					}
					if (CheckOutTapeIOFilm())
					{
						ShowErrorMessage("������Ĥδ��⵽���鿴�Ƿ����ꡣ", pMainWnd);
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
						ShowErrorMessage("�����ϴ����꣬������ϴ�", pMainWnd);
						break;
					case 2:
						ShowErrorMessage("�����ԭ���Ӧ��������ԭ���硣", pMainWnd);
						break;
					case 3:
						ShowErrorMessage("��Ĥ���꣬�������", pMainWnd);
						break;
					case -1:
						ShowErrorMessage("������ͨ���쳣��", pMainWnd);
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
		//����ʱ���߽���ʱִ�г���------------------------------------------
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
					goto endofwhile;//����while
				}
			}
		}
		//------------------------------------------------------------------
		if (gm_nContinueCheckFail >= 2)
		{
			gm_nContinueCheckFail = 0;
			ShowErrorMessage("����ƥ��ʧ�ܣ���������Ƿ�����������", pMainWnd);
			break;
		}

		//������¼��Ϣ
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
			ShowErrorMessage("����������������ͣ����շ����̺��ٽ�����¼��");
			gm_nCurrentFailSize = 0;
			break;
		}

		if (gm_bFinish)//����ʱ�������¼����û��оƬ�����ʾ�����������
		{
			if (-2 == CheckChipForMoveOutSocket(0))
			{
				if (!ZComeBack())
					return ERROR_XYMOVE_ZCOMEBACK;
				pMainWnd->CreateLogfile();//����ʱ������¼��־
				SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
				SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
				XYMove(0, 0);
				break;
			}
		}
		//�������ʱ���ж��Ƿ���¼���
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
			pMainWnd->TrayComplete();//��������̣����ͻ����ź�
			TrayComplete();//IOͨ��
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
		ShowErrorMessage("��¼���", lParam);
		pMainWnd->SetRunStatus("Complete!");
		if (gm_nTotalGoodSize + gm_nTotalFailSize == gm_nSum)//��¼���ʱ�Ų�������
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
		pMainWnd->KillTimer(TIMER_CHECK_TRAY_ARRIVED);//���Tray�̵����ź�
	}
	//if (gm_bTapeOutCheck && (FEEDER == 0))
	//{
	//	pMainWnd->KillTimer(TIMER_CHECK_TAPE_STATUS);//
	//}

	pMainWnd->KillTimer(TIMER_RUN_TIME);//�ر���¼ʱ�䶨ʱ��
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
	gm_logFile.Write("�û�ȷ�ϣ�");
}

//������Ϣ��ʾ
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
	gm_logFile.Write("�û�ȷ�ϣ�");
}

void ShowErrorMessage(UINT nErrorCode, int axis, LPVOID lParam)
{
	CString str;
	switch (nErrorCode)
	{
	case ERROR_STOCKTRAY_POS:
		str.Format("��ȡ������λ��ʧ��.");
		break;
	case ERROR_SOCKET_PRESSED:
		str.Format("��������ʧ��.");
		break;
	case ERROR_NOZZLE_NOT_BOUND:
		str.Format("����%d,����δ�ɹ�����.", axis);
		break;
	case ERROR_GET_CHIP:
		str.Format("����%d,δ�ɹ�ȡ��оƬ.", axis);
		break;
	case ERROR_PUT_CHIP:
		str.Format("����%d,δ�ɹ�����оƬ.", axis);
		break;
	case ERROR_XYMOVE:
		str.Format("XY���˶�����.");
		break;
	case ERROR_STOCKTRAY_EMPTY:
		str.Format("�������ѿգ�����������̣���������ò����̰�ť��");
		break;
	case ERROR_STOCKTRAY_FULL:
		str.Format("������������");
		break;
	case ERROR_XYMOVE_ZCOMEBACK:
		str.Format("z���ԭ�쳣");
		break;
	case ERROR_XYMOVE_NOZZLE_BACK:
		str.Format("����%d��ԭ�쳣��", axis);
		break;
	case ERROR_IXLINE:
		str.Format("�岹�˶�ʧ�ܡ�");
		break;
	case ERROR_CAMERA_CHECK:
		str.Format("���ƥ��ʧ�ܡ�");
		break;
	case ERROR_TAPE_OUT:
		str.Format("��������Ϲ��ϡ�");
		break;
	case ERROR_SCRAYTRAY_EMPTY:
		str.Format("�������ѿա�");
		break;
	case CHIP_MISS:
		str.Format("оƬ���䡣");
		gm_nTotalCheckFail++;
		gm_nTotalFailSize++;
		break;
	case PROGRAMER_ERROR:
		str.Format("�������¼ʱ��̫�̣���������Ƿ�����������");
		break;

	default:
		break;
	}
	ShowErrorMessage(str, lParam);
}



