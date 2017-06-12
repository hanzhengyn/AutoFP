#pragma once
#include "Pxx_LastErrCode.h"
#include "Pxx_ATEInterface.h"
class CPxxATE
{
public:
	CPxxATE();
	~CPxxATE();
public:
	//�����豸�ķ���
	BOOL ConnectWithWlan();//ͨ����������
	BOOL ConnectWithUlart();//ͨ����������
	BOOL ConnectWithUSB();//ͨ��USB����

	BOOL OpenProjectWithIdex();//ͨ����Ŵ򿪹���
	BOOL OpenProjectWithName();//ͨ���������򿪹���
	BOOL StartProgramm(int channel);//��ʼ��¼
	BOOL OperateDevice(int op);//����¼��ִ�в���
	static DWORD WINAPI GetStatusThread(LPVOID lpvoid);//��ȡ��¼״̬

public:
	bool m_bGetStatus;//״̬����ʶ��
	int m_deviceID;//�豸ID
	CString m_strIP;//�豸IP
	DWORD m_dwPort;//�豸�˿�
	DWORD m_dwCom;//�豸����
	DWORD m_dwBaudrate;//�豸������
	int m_nProjectIndex;//�������
	CString m_strProjectName;//������
};

