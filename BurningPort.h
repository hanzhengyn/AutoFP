// BurningPort.h: interface for the CBurningPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_)
#define AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//\brief  ���Ҫ�����socket�����ط������ȼ���ߵ�socket���
int CheckChipForMoveInSocket(int nNozzle);
//Ԥ����¼��
int BookSocket(int nNozzle);
//\brief  �����Ҫ��socket��ȡ����оƬ������ȡ�����ȼ���ߵ�socket���
int CheckChipForMoveOutSocket(int nNozzle);
//\brief  ���socket��оƬ�ܷ�ȡ���������ȡ��������true
BOOL IsChipCanBeMoveOutFromSocket(int sock);
//\brief  ���socket�Ƿ�Ϊ�գ����Ϊ�գ�����true
BOOL CheckSocketIsEmpty(int sock);
//\brief  ����ȡ�����ȼ�
void  AddGetPreference();
//\brief  ����������ȼ�
void AddPutPreference();
//\brief  ��ȡtray���е�presum��λ�õ�����pos
BOOL GetChipPosInTray(int presum, Position &pos);
//\brief  ��ȡ�����̵�presum��λ�õ�����pos
UINT GetChipPosInStockTray(int presum, Position &pos);
//\brief  ��ȡ�����̵�presum��λ�õ�����pos
BOOL GetChipPosInScrap(int presum, Position &pos);
//\brief  ��ȡ�����������
Position  GetPosForTapeIn();
//\brief  ��ȡ�����������
Position  GetPosForTapeOut();
BOOL  GetPosForTape(const char *inOut, Position &pos);
//\brief  ��ȡ��sockInx��socket������
Position  GetPosForSocket(int sockInx);
BOOL  GetPosForSocket(int sockInx, Position &pos);

UINT BeepThreadProc(LPVOID lParam);
UINT GetChip();
UINT PutChip();
UINT GetChip(int nNozzleNum);
UINT PutChip(int nNozzleNum);

//\brief  ȡоƬ����
//\param  nGetNumber [in] ȡ����λ�ñ��
//\param  nNozzleNumber [in] ������
//\param  nWorkMode [in] ����ģʽ
//\//return
UINT GetChipProc(int nGetNumber, int nNozzleNumber, int nWorkMode);

///brief  ��оƬ����
//param  nPutNumber [in] �����λ�ñ��,������¼��Ϊ���ӱ�ţ�����tray��Ϊtray����оƬ��ţ����ڱ��Ϊ�������λ��
//param  nNozzleNumber [in] ������	0 1 2 3
//param  posOffset [in] ��Ƭƫ��
//param  nWorkMode [in] ����ģʽ
//return
UINT PutChipProc(int nPutNumber, int nNozzleNumber, Position2 &posOffset, int nWorkMode);

//\brief  оƬ�ƶ������������������
//\param  nNozzleNumber			[in] ������
//\param  posOffset				[out] ƫ��ֵ
//\param  lParam					[in] ������ָ��
//\return TRUE	--	ƥ��ɹ�
//\return FALSE	--	ƥ��ʧ��
UINT CheckChipProc(int nNozzleNumber, Position2 &posOffset, LPVOID lParam);

//  ��תоƬ�߳�
//param  lpParameter			[in] ����ƫ����ϢposOffset
DWORD WINAPI SrMoveProc(
    LPVOID lpParameter   // thread data
);
DWORD WINAPI SrMoveProc1(
    LPVOID lpParameter   // thread data
);
DWORD WINAPI SrMoveProc2(
    LPVOID lpParameter   // thread data
);
DWORD WINAPI SrMoveProc3(
    LPVOID lpParameter   // thread data
);
//����������Ϣ
DWORD WINAPI SaveConfigProc(
    LPVOID lpParameter   // thread data
);
//���������ӿ�ʼ��¼
void StartProgramProc(int nSocketNum, LPVOID lParam);

//��������Ƿ���ȡ��
//ȡ��Ҫ�������������
//1.�ӽ���ȡ�ϣ������Ƿ����ϣ���¼���Ƿ��еط���
//2.����¼��ȡ�ϣ���¼���Ƿ�����¼�õ��ϣ����ϴ��Ƿ��еط���
//3.�Ӳ�����ȡ�ϣ��в��Ϲ��̣�tray���ѿգ�����δ����ֻ���tray�̽��ϣ�
BOOL CheckChipCanBeGet(int nWorkMode, int nNozzle);

//���Ϲ���
UINT InputProc(LPVOID lParam);

//���Ϲ���
UINT OutputProc(LPVOID lParam);

//�ж���¼�����Ƿ����
//��¼��ɵļ��������
//1.�޲��ϣ����Զ�tray�̣�	gm_nCurrentOutSize == gm_nSum
//2.�в��ϣ����Զ�tray�̣�	gm_nCurrentGoodSize == gm_nSum
//3.�޲��ϣ����Զ�tray�̣�	gm_nCurrentOutSize == gm_nSum && tray�̽���û�и�Ӧ��
//4.�в��ϣ����Զ�tray�̣�	gm_nCurrentGoodSize == gm_nSum && tray�̽���û�и�Ӧ��
BOOL CheckBurningProcComplete(LPVOID lParam);

//���tray�̵����ź�
BOOL CheckAutoTrayArrived();

//��¼����
UINT BurningProc(LPVOID lParam);

//������Ϣ��ʾ
void ShowErrorMessage(const char *chErrorMessage, LPVOID lParam);
void ShowErrorMessage(const char *chErrorMessage);
void ShowErrorMessage(UINT nErrorCode, int axis, LPVOID lParam);
#endif // !defined(AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_)
