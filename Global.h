#ifndef GLOBAL_H
#define GLOBAL_H
#include "LogFile.h"
#include "datapos.h"
#include "HalconCpp.h"

using namespace HalconCpp;

//�������ͼ��ģ�͵�λ������
extern HTuple gm_hv_Row_In;
extern HTuple gm_hv_Column_In;
extern HTuple gm_hv_Angle_In;

//�������ͼ��ģ�͵�λ������
extern HTuple gm_hv_Row_Out;
extern HTuple gm_hv_Column_Out;
extern HTuple gm_hv_Angle_Out;

//�������оƬͼ��ģ�͵�λ������
extern HTuple gm_hv_Row_Chip;
extern HTuple gm_hv_Column_Chip;
extern HTuple gm_hv_Angle_Chip;

//socketͼ��ģ�͵�λ������
extern HTuple gm_hv_Row_Socket;
extern HTuple gm_hv_Column_Socket;
extern HTuple gm_hv_Angle_Socket;

//оƬģ�͵�����
extern CString gm_strChipModel;

//��¼��ģ������
extern CString gm_strSocketModel;

//��������·��
extern CString gm_strSetupPath;

//
extern BYTE gm_nBoardID;

//λ�������ļ�·��
extern CString gm_strPositionFile;

//��¼״̬�ļ�·��
extern CString gm_strStatusLog;

//�����ļ�·��
extern CString gm_strConfigFile;

extern CString gm_strSpecialInfo;

//tray�̹��
extern TrayInfo gm_dataTray;

//�����̹��
extern TrayInfo gm_dataStock;

//�����̹��
extern TrayInfo gm_dataNG;

//������tray����ʾ����
extern BYTE *gm_pTray;

//ģ���ȡ�����ȼ�
extern int gm_nModelPutPreference[MAXNUM_OF_PROGRAMMER];
extern int gm_nModelGetPreference[MAXNUM_OF_PROGRAMMER];

//socket�������ȼ�
extern int sockPutPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//socketȡ�����ȼ�
extern int sockGetPreference[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//socket�Ƿ�ʹ��
extern BOOL gm_bSocketUsed[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//����ʹ�ñ�־λ
extern BOOL gm_bNozzleUsed[4];

//��ʾÿ����¼����OK����
extern int gm_nDisplayGoodForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//��ʾÿ����¼����NG����
extern int gm_nDisplayFailForSocket[MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET];

//ֹͣ��־λ
extern BOOL gm_bStop;

//��ʼ��¼��ʶ
extern BOOL gm_bStart;

//tray��PLC���ź�
extern BOOL gm_bTrayPLCStart;

//������¼��ʶ
extern BOOL gm_bFinish;

//��������Ľ���������
extern int gm_nTotalInSize;
extern int gm_nTotalOutSize;

//��ǰ��������
extern int gm_nCurrentInSize;

//��ǰ��������
extern int gm_nCurrentOutSize;

//����¼��оƬ����
extern int gm_nSum;

//��������
extern int gm_nNozzleNum;

//��ǰ��¼�ɹ�����
extern int gm_nCurrentGoodSize;

//��ǰ��¼ʧ������
extern int gm_nCurrentFailSize;

//ƥ��ʧ�ܵ�����
extern int gm_nTotalCheckFail;

//����ƥ��ʧ������
extern int gm_nContinueCheckFail;

//��¼ʧ�ܵ�����
extern int gm_nTotalFPFail;

//�ܵ���¼�ɹ�����
extern int gm_nTotalGoodSize;

//�ܵ���¼ʧ������
extern int gm_nTotalFailSize;

//�û���Ϣ���
extern UserInfo gm_uInfo;

//��־�ļ����
extern CLogFile gm_logFile;
extern bool gm_bPushed[MAXNUMOFPROGRAMMER];

//λ����Ϣ�ṹ��
extern CDataPos gm_dataPos;

//X�϶�������
extern int gm_nXLength;

//Y�϶�������
extern int gm_nYLength;
extern int gm_nHCount;

extern HWND gm_hArray[4];

//��¼״̬��0Ϊδ��¼��1Ϊ������¼��2Ϊ��¼�ɹ���3Ϊ��¼ʧ��
extern BurningStatus gm_sBurningStatus[MAXNUMOFPROGRAMMER];

//�Ƿ�ʹ���Զ�tray�̻���ʶ
extern BOOL gm_bAutoTray;

//�Ƿ�ʹ�ò��Ϲ��̱�ʶ
extern BOOL gm_bStock;

//socket���ʱ�Ƿ�ʹ����������¼����оƬ
extern BOOL gm_bSocketHeightCamera;

//�Ƿ�ʹ�ñ�������ϼ���ʶ
extern BOOL gm_bTapeCheck;

//�����Ƿ�ʹ���������ʶ
extern BOOL gm_bDownCameraIn;

//�����Ƿ�ʹ���������ʶ
extern BOOL gm_bDownCameraOut;

//��е�۵����������ʶ��
extern BOOL gm_bArrivedDownCamera;

//������������־λ
extern BOOL gm_bBeep;

//��ǰ��¼����
extern int gm_nCurrentTotalSize;

//������������
extern int gm_nTotalStock;

//��ǰ������
extern int gm_nCurrentStock;

//����������¼OK����
extern int gm_nStockGoodSize;

//�������б�ʶ
extern BOOL gm_bStockRun;

//������ϱ�׼λ��
extern Point gm_nOutOrigePos;

//�������ƫ��
extern Point gm_nTapeOutOffset;

//global Function
void InitGlobalMembers();

//���������������־λ
extern BOOL gm_bCheckTapeOut;

//tray�̵����־λ
extern BOOL gm_bTrayArrived;

//�����״̬��־
extern int gm_bTapeStatus;

//��־��Ϣ
extern sData gm_sdInfo;

//�ĸ�����Ĵ����ʶ
extern UINT gm_bErrorFig[4];

//��ʼ��ת�Ƕ�,PI�ļ���Ƶ���������1����Ƕ�ΪPI���������2����Ƕ�ΪPI/2,�Դ�����
//���Ϊ0����ʾ����ת
//������֮��
extern int gm_nRotateAngleIn;
extern int gm_nRotateAngleOut;

//��ǰ�������е�оƬ��������Զ�����
extern int gm_nCurrentGetChip;

//
extern BOOL gm_bComplete;

//������д��ź�
extern BOOL gm_bSaveConfig;

extern BOOL gm_bConvertChip;
extern BOOL gm_bDoorAlarm;
extern BOOL gm_bTapeOutCheck;
extern BOOL gm_bTapeCamera;
#endif