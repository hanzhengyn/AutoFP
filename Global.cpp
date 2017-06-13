#include "stdafx.h"
#include "global.h"
#include "BurningPort.h"

CString gm_strSetupPath;
BYTE gm_nBoardID = 0;
CString gm_strPositionFile;
CString gm_strStatusLog;
CString gm_strConfigFile;
TrayInfo gm_dataTray;
TrayInfo gm_dataStock;
TrayInfo gm_dataNG;
BYTE *gm_pTray = NULL;
CString gm_strSpecialInfo;
int gm_nModelPutPreference[MAXNUM_OF_PROGRAMMER]{ 0,0,0,0,0, 0,0,0,0,0 };
int gm_nModelGetPreference[MAXNUM_OF_PROGRAMMER]{ 0,0,0,0,0, 0,0,0,0,0 };
int sockPutPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];
int sockGetPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];
BOOL gm_bSocketUsed[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];
BOOL gm_bNozzleUsed[4];				//����ʹ�ñ�־λ

//ֹͣ��־λ
BOOL gm_bStop = FALSE;

//���б�־λ
BOOL gm_bStart = FALSE;

//
BOOL gm_bTrayPLCStart = FALSE;

//������־λ
BOOL gm_bFinish = FALSE;

//��������Ľ���������
int gm_nTotalInSize = 0;

int gm_nTotalOutSize = 0;
int gm_nCurrentInSize = 0;
int gm_nCurrentOutSize = 0;
int gm_nSum = 0;
int gm_nCurrentGoodSize = 0;
int gm_nNozzleNum = 1;
//��ǰһ�̵���¼GN����
int gm_nCurrentFailSize = 0;
//ƥ��ʧ�ܵ�����
int gm_nTotalCheckFail = 0;
int gm_nContinueCheckFail = 0;
//��¼ʧ������
int gm_nTotalFPFail = 0;
int gm_nTotalGoodSize = 0;
int gm_nTotalFailSize = 0;

int gm_nCurrentTotalSize = 0;
//��ʾÿ����¼����OK����
int gm_nDisplayGoodForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];
//��ʾÿ����¼����NG����
int gm_nDisplayFailForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];
UserInfo gm_uInfo;
CLogFile gm_logFile;
bool gm_bPushed[MAXNUMOFPROGRAMMER];
CDataPos gm_dataPos;
int gm_nXLength;
int gm_nYLength;
int gm_nHCount;
HWND gm_hArray[4];
//����¼������
int gm_nEmptySocketNum = FALSE;

BurningStatus gm_sBurningStatus[MAXNUMOFPROGRAMMER];
BOOL gm_bAutoTray;
BOOL gm_bStock;
BOOL gm_bTapeCheck;

BOOL gm_bSocketHeightCamera;

BOOL gm_bDownCameraIn;

BOOL gm_bDownCameraOut;

BOOL gm_bArrivedDownCamera;
BOOL gm_bBeep;

//���������������־λ
BOOL gm_bCheckTapeOut;

//tray�̵����־λ
BOOL gm_bTrayArrived;

//�����״̬��־
int gm_bTapeStatus;

//�������е�����
int gm_nTotalStock;

//��ǰ������
int gm_nCurrentStock;

//����������¼OK����
int gm_nStockGoodSize = 0;

BOOL gm_bStockRun;
//globla func

//ת�ϱ�־
BOOL gm_bConvertChip = FALSE;
BOOL gm_bDoorAlarm = FALSE;
BOOL gm_bTapeOutCheck = TRUE;

Point gm_nOutOrigePos;
Point gm_nTapeOutOffset;

HTuple gm_hv_Row_In;
HTuple gm_hv_Column_In;
HTuple gm_hv_Angle_In;

HTuple gm_hv_Row_Out;
HTuple gm_hv_Column_Out;
HTuple gm_hv_Angle_Out;

HTuple gm_hv_Row_Chip;
HTuple gm_hv_Column_Chip;
HTuple gm_hv_Angle_Chip;

HTuple gm_hv_Row_Socket;
HTuple gm_hv_Column_Socket;
HTuple gm_hv_Angle_Socket;

CString gm_strChipModel;
CString gm_strSocketModel;

sData gm_sdInfo{ "Customer","WorkNo",0,"ChipModel","ChipBrand",0,0,0,0,0,0,0,"12:20","00/00/2016 00:00:00","00/00/2016 00:00:00","FFFFFFFF","s001" };//��־��Ϣ
//��¼���̴����ʶ
UINT gm_bErrorFig[4]{ 0,0,0,0 };

//��ʼ��ת�Ƕ�,PI�ļ���Ƶ���������1����Ƕ�ΪPI���������2����Ƕ�ΪPI/2,�Դ�����
//���Ϊ0����ʾ����ת
int gm_nRotateAngleIn = 0;
int gm_nRotateAngleOut = 0;
int gm_nCurrentGetChip = 0;
BOOL gm_bComplete = FALSE;
BOOL gm_bSaveConfig = FALSE;//������д��ź�
void InitGlobalMembers()
{

    gm_nOutOrigePos.x = 0;
    gm_nTapeOutOffset.x = 0;
    gm_nOutOrigePos.y = 0;
    gm_nTapeOutOffset.y = 0;

    gm_bStockRun = FALSE;
    gm_nTotalStock = 0;
    gm_nCurrentStock = 0;
    gm_nXLength = 200;
    gm_nYLength = 200;
    gm_strSpecialInfo = "c:\\SpecialInfo.ini";
    CIniFile inf;
    inf.SetIniFileName(gm_strPositionFile);

    gm_dataTray.xn = inf.GetInt("Tray", "XN", 8);
    gm_dataTray.yn = inf.GetInt("Tray", "YN", 12);

    gm_dataStock.xn = inf.GetInt("Stock", "XN", 8);
    gm_dataStock.yn = inf.GetInt("Stock", "YN", 12);

    gm_dataNG.xn = inf.GetInt("NG", "XN", 8);
    gm_dataNG.yn = inf.GetInt("NG", "YN", 12);

    gm_pTray = new BYTE[gm_dataTray.xn*gm_dataTray.yn];
    int total = gm_dataTray.xn * gm_dataTray.yn;
    for (int i = 0; i < total; i++)
        gm_pTray[i] = 2;

    gm_bAutoTray = FALSE;
    gm_bStock = FALSE;
    gm_bTapeCheck = FALSE;
    gm_bDownCameraIn = FALSE;
    gm_bDownCameraOut = FALSE;
    gm_bArrivedDownCamera = FALSE;
    gm_bCheckTapeOut = FALSE;
    gm_bTrayArrived = FALSE;
    gm_bBeep = TRUE;
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER; i++)
    {
        gm_bPushed[i] = false;

        gm_sBurningStatus[i].nBusy = 0;
        gm_sBurningStatus[i].bUsed = true;
        gm_sBurningStatus[i].nGetChips = 0;
        gm_sBurningStatus[i].nPutChips = 0;
        for (int j = 0; j < MAXNUM_OF_SOCKET; j++)
        {
            gm_sBurningStatus[i].bAdpUsed[j] = true;
            gm_sBurningStatus[i].bEmpty[j] = true;
            gm_sBurningStatus[i].nAdpStatus[j] = 0;
            gm_sBurningStatus[i].bBooking[j] = false;
            gm_sBurningStatus[i].nContinueFailSize[j] = 0;
        }
    }

    for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    {
        gm_nDisplayFailForSocket[i] = 0;
        gm_nDisplayGoodForSocket[i] = 0;
        sockPutPreference[i] = 0;
        sockGetPreference[i] = 0;
        //gm_bSocketUsed[i] = TRUE;
    }

    gm_bStart = FALSE;
    gm_bFinish = FALSE;
    gm_nCurrentInSize = 0;
    gm_nCurrentOutSize = 0;
    gm_nSum = 0;
    gm_nCurrentGoodSize = 0;
    gm_nCurrentFailSize = 0;

    gm_nTotalGoodSize = 0;
    gm_nTotalFailSize = 0;
    gm_uInfo.xySpeed = 3;
    gm_uInfo.zSpeed = 3;
    gm_uInfo.nWorkMode = 0;
    gm_uInfo.nChipLength = 0;
    gm_uInfo.nChipWidth = 0;
    gm_uInfo.nSocketLength = 0;
    gm_uInfo.nSocketWidth = 0;
    gm_uInfo.nPixelsPM_down = 0;
    gm_uInfo.nPixelsPM_up = 0;
    gm_uInfo.nOffsetInX = 0;
    gm_uInfo.nOffsetInY = 0;
    gm_uInfo.nOffsetOutX = 0;
    gm_uInfo.nOffsetOutY = 0;
    gm_uInfo.nOffsetSocketX = 0;
    gm_uInfo.nOffsetSocketY = 0;
}