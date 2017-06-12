/************************************************************************
  Copyright (C), 1988-2999, Guangzhou Zhi Yuan Electronics Co., Ltd.
  File name: Pxx_ATEInterface.h
  Author: zhoushiyou	Version:	v1.0   Date: 2015-9-14
  Description:   ��λ�� ATE�����ӿ�
  Others Description:  
  Function List:  
  History:                     
    1. Date: 2015-9-14
       Author:zhoushiyou
       Modification:create
************************************************************************/
#ifndef __P500_INTERFACE_PXX_ATE
#define __P500_INTERFACE_PXX_ATE

#include "Pxx_LastErrCode.h"

#ifdef PXX_ATE_EXPORT
#define PXX_ATE_API __declspec(dllexport)
#else
#define PXX_ATE_API __declspec(dllimport)
#endif

/*���нӿ��ַ������ö��ֽڱ���*/

/************************************************************************/
/* ���ò������Ͷ���
/* ��ҪӦ����API:ConnectDevice
/************************************************************************/
typedef enum _pxx_ate_type{PXX_ATE_TYPE_WLAN,PXX_ATE_TYPE_COM,PXX_ATE_TYPE_USB}PXX_ATE_TYPE; //ͨѶ����
typedef struct _pxx_ate_wlan	//��������ṹ��
{
	char 		ip[16];//IP��ַ
	int			port;//�豸�˿ں�
}PXX_ATE_WLAN,*PPXX_ATE_WLAN;
typedef struct _pxx_ate_com		//���ڲ����ṹ��
{
	int			port;//���ں�
	int			baudrate;//������
}PXX_ATE_COM,*PPXX_ATE_COM;
typedef struct _pxx_ate_usb		//USB�����ṹ�� 
{/*��ʱδʹ��*/
}PXX_ATE_USB,*PPXX_ATE_USB;

/************************************************************************/
/* ִ�в�����Ӧ����
/* ��ӦAPIΪ��RunDeviceOperator
/************************************************************************/
#define PXX_ATE_OP_RUN_ERASE								1//����
#define PXX_ATE_OP_RUN_CHECK								2//���
#define PXX_ATE_OP_RUN_PROGRAM								3//���
#define PXX_ATE_OP_RUN_VERIFY								4//У��
#define PXX_ATE_OP_RUN_READ									5//��ȡ
#define PXX_ATE_OP_RUN_COMBINATION							6//���
#define PXX_ATE_OP_RUN_AUTOBATCH							7//����
#define PXX_ATE_OP_RUN_PROGRAM_C							8//�������
#define PXX_ATE_OP_RUN_VERIFY_C								9//У������
#define PXX_ATE_OP_RUN_ERASE_C								10//��������
#define PXX_ATE_OP_RUN_CHECK_C								11//�������
#define PXX_ATE_OP_RUN_READ_C								12//������
#define PXX_ATE_OP_RUN_LOCK									13//����
#define PXX_ATE_OP_RUN_UNLOCK								14//����

/************************************************************************/
/* ͨ��״ֵ̬
/* ATE���ƹ����У�ͨ�����ýӿ�GetDeviceStatus��ȡÿ��ͨ����״ֵ̬
/************************************************************************/
#define SOFT_ATE_STATUS_ERR			0
#define SOFT_ATE_STATUS_OK			1

#define SOFT_ATE_STATUS_POWERON		0x100	//�ϵ���
#define SOFT_ATE_STATUS_INIT		0x101	//��ʼ��������
#define SOFT_ATE_STATUS_ERASE		0x102	//������
#define SOFT_ATE_STATUS_CHECK		0x103	//�����
#define SOFT_ATE_STATUS_PROGRAM		0x104	//�����
#define SOFT_ATE_STATUS_VERIFY		0x105	//У����
#define SOFT_ATE_STATUS_READ		0x106	//��ȡ��
#define SOFT_ATE_STATUS_C_PROGRAM	0x107	//���������
#define SOFT_ATE_STATUS_C_VERIFY	0x108	//У��������
#define SOFT_ATE_STATUS_C_ERASE		0x109	//����������
#define SOFT_ATE_STATUS_C_CHECK		0x10A	//���������
#define SOFT_ATE_STATUS_C_READ		0x10B	//��ȡ������
#define SOFT_ATE_STATUS_LOCK		0x10C	//����
#define SOFT_ATE_STATUS_UNLOCK		0x10D	//����

#define SOFT_ATE_STATUS_WAITCONTROL	0x200	//�ȴ�����
#define SOFT_ATE_STATUS_OP_END		0x201	//��������

#define SOFT_ATE_ERR_TIMES_END		0x400	//���������ù�
#define SOFT_ATE_ERR_BEG_BATCH		0x401	//������ʼʧ��
#define SOFT_ATE_ERR_POWER_ON		0x402	//�ϵ����ʧ��
#define SOFT_ATE_ERR_INIT			0x403	//��ʼ��ʧ�ܣ�ϵͳ�ϵ�+��ID��
#define SOFT_ATE_ERR_OP_FILE		0x404	//��д�ļ�����ʧ�ܣ��´����ݴ���
#define SOFT_ATE_ERR_PROGRAM		0x405	//���ʧ�ܡ�����ϸ����־��Ϣ
#define SOFT_ATE_ERR_VERIFY			0x406	//У��ʧ�ܡ�����ϸ����־��Ϣ
#define SOFT_ATE_ERR_ERASE			0x407	//����ʧ�ܡ�����ϸ����־��Ϣ
#define SOFT_ATE_ERR_CHECK			0x408	//���ʧ�ܡ�����ϸ����־��Ϣ
#define SOFT_ATE_ERR_READ			0x409	//��ȡʧ�ܡ�����ϸ����־��Ϣ
#define SOFT_ATE_ERR_C_PROGRAM		0x40A	//�������ʧ��
#define SOFT_ATE_ERR_C_VERIFY		0x40B	//У������ʧ��
#define SOFT_ATE_ERR_C_ERASE		0x40C	//��������ʧ��
#define SOFT_ATE_ERR_C_CHECK		0x40D	//�������ʧ��
#define SOFT_ATE_ERR_C_READ			0x40E	//��ȡ����ʧ��
#define SOFT_ATE_ERR_LOCK			0x40F	//����ʧ��
#define SOFT_ATE_ERR_UNLOCK			0x410	//����ʧ��

#define SOFT_ATE_STATUS_UNUSE		0xFFFFFFFF//δʹ��



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	/*ͨ��ATE����*/
	/************************************************************************/
	/* Description:
		���ӱ�����豸
		ATE���ػ��������豸ǰ������øýӿڻ�ȡ�����豸��ʶ��API����ֵ��
		ConnectDevice��ͨ�ýӿڣ�ͨ�����ݲ�ͨ��������ִ�в�ͬ�Ĵ򿪲���
		���������������������磺
		PXX_ATE_WLAN wlanInfo;
		strcpy_s(wlanInfo.ip,16,"192.168.9.140");
		wlanInfo.port = 8800;//�㶨ֵ
		m_deviceID = ConnectDevice(PXX_ATE_TYPE_WLAN,&wlanInfo);
		
		ConnectDeviceWlanֱ��ͨ�����緽ʽ�����豸
		ConnectDeviceUartֱ��ͨ�����ڷ�ʽ�����豸
		ConnectDeviceUSBֱ��ͨ��USB��ʽ�����豸
	/* Param:
		ConnectDevice::type:ͨ��typeģʽ�򿪱�����豸
		ConnectDevice::pParam:ģʽ��Ӧ�Ľṹ��ָ��

		ConnectDeviceWlan::ip:�������豸��IP��ַ����"192.168.9.140"
		ConnectDeviceWlan::port:�������豸�˿ڣ�������д�㶨ֵ8800

		ConnectDeviceUart::port:���ػ����ӵ��豸�ϵ�COM�˿ںţ�ȡֵΪ0.1.2.3...
		ConnectDeviceUart::baudrate:Ϊƥ���豸�����ʣ��������ú㶨ֵ115200

	/* Result:
		0:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		��0:�����豸��ʶ���ñ�ʶ����Ҫ����ʹ�ã�����API����Ҫ���ݸñ�ʶ�����в���������ƥ����ȷ�Ĳ�������
	/************************************************************************/
	PXX_ATE_API int ConnectDevice(PXX_ATE_TYPE type,void * pParam);
	PXX_ATE_API int ConnectDeviceWlan(char ip[16],DWORD port);
	PXX_ATE_API int ConnectDeviceUart(int port,int baudrate);
	PXX_ATE_API int ConnectDeviceUSB();

	/************************************************************************/
	/* Description:
		�Ͽ����豸�����ӣ������ػ�����Ҫ�����Ӹ��豸��ʱ���ã��Ͽ����Ӻ���豸���ܱ����п��ơ�
		��ϵ�к�����ConnectDeviceϵ�к����ɶԵ���
		�磺����ر�ʱ���ã��ԶϿ�ATE���ӿ��ơ�
		UnConnectDeviceΪͨ�öϿ��ӿ�
		����̫���Ͽ�Ϊ���������磺
		UnConnectDevice(PXX_ATE_TYPE_WLAN,m_deviceID);

		UnConnectDeviceWlan�Ͽ��������ӵ��豸
		UnConnectDeviceUart�Ͽ��������ӵ��豸
		UnConnectDeviceUSB�Ͽ�USB���ӵ��豸
	/* Param:
		UnConnectDevice::type:�Ͽ����ӵ�����
		UnConnectDevice::deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		UnConnectDevice::bForce:�Ƿ�ǿ�ƶϿ����������Ϊtrue����Ͽ�����ʧ��ʱҲǿ�ƹر����ض˵����ӣ��������ɹ���

		UnConnectDeviceWlan::deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		UnConnectDeviceWlan::bForce:�Ƿ�ǿ�ƶϿ����������Ϊtrue����Ͽ�����ʧ��ʱҲǿ�ƹر����ض˵����ӣ��������ɹ���

		UnConnectDeviceUart::deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		UnConnectDeviceUart::bForce:�Ƿ�ǿ�ƶϿ����������Ϊtrue����Ͽ�����ʧ��ʱҲǿ�ƹر����ض˵����ӣ��������ɹ���

		UnConnectDeviceUSB::deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		UnConnectDeviceUSB::bForce:�Ƿ�ǿ�ƶϿ����������Ϊtrue����Ͽ�����ʧ��ʱҲǿ�ƹر����ض˵����ӣ��������ɹ���
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool UnConnectDevice(PXX_ATE_TYPE type,int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceWlan(int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceUart(int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceUSB(int deviceID,bool bForce = true);

	/************************************************************************/
	/* Description:
		ͨ����������Զ�̴��豸�ϵĹ���
		����������ָ�豸��SD������Ŀ¼�¹����ļ�����������ţ���ǰ�Ѵ򿪹���Ĭ��ռ�����1��
		���ýӿ��磺
		OpenDeviceProject(m_deviceID,m_proIdex);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		proIdex:�豸�Ϲ����б������,��Χ(1-N)
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool OpenDeviceProject(int deviceID,int proIdex);

	/************************************************************************/
	/* Description:
		ִ�в���������������̡�У�顢��ϡ������ȡ�
		��ִ�в����б���ձ��ļ��к궨�����ͣ�ѡ����Ҫִ�еĲ�������øýӿڼ��ɣ����Ӵ����磺
		RunDeviceOperator(m_deviceID,PXX_ATE_OP_RUN_ERASE);
		�ò�����Ϊ�������������ػ����øò�������Ҫִ��ATE��صĻ�е�������������оƬ������Ŀ���Ķ�����
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		op:�������ͣ����ļ������еĺ궨��
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool RunDeviceOperator(int deviceID,int op);

	/************************************************************************/
	/* Description:
		��ʼִ�е��β���
		�ú���Ϊ���RunDeviceOperatorʹ�ã���RunDeviceOperator֮��ִ�����е������Ӧ�õ��øýӿ����������Ĳ������̡�
		�ò����Ƿ�ͨ��������ģ���˼�ǵ���ͨ���������ƣ�Ҳ����ֱ�����������ʽͬʱ��������������û�����ͬ��ִ�в�ͬ�ĵ��÷�ʽ
		���Ӵ����磺
		StartDeviceOperater(m_deviceID,1);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		channel:Ҫ������ͨ������
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool StartDeviceOperater(int deviceID,int channel);
	
	/************************************************************************/
	/* Description:
		��ȡ�豸����ͨ���ĵ�ǰ״̬
		�ڳɹ����������豸��API�󼴿ɿ�����̨�߳������øýӿڻ�ȡÿ��ͨ����״ֵ̬��״ֵ̬�����ļ������еú궨��
		�ɸ��ݷ�����ͬ��״ֵ̬�������UI�Ͻ�����ʾ��
		���ô����磺
		DWORD statusArray[16];//16��Ϊ�������ͨ������
		GetDeviceStatus(pDlg->m_deviceID,statusArray);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		pbuffer:����16��DWORD���ȵĻ�����
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool GetDeviceStatus(int deviceID,DWORD* pBuffer);

	/************************************************************************/
	/* Description:
		��ȡ������log��Ϣ���ڲ�������ʱ���ü��ɣ���������ϵͳ������
		ÿ�β�������Ҫ���øýӿ�����ȡ��־��Ϣ��������־��Ϣ�ᱻ��һ�β������ǵ���
		���ô����磺
		char pBuffer[4096];
		memset(pBuffer,0,4096);
		GetCurOPLogs(m_deviceID,1,pBuffer);
		��־��Ϣ�ı��뷽ʽΪ�����ֽڱ���
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		channel:һ��ͨ��һ��ͨ����ȡ��ͬ��ģʽֱ�ӻ�ȡһ��ͨ��
		pLog:����������־����С��֤4096���ϳ���
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool GetCurOPLogs(int deviceID,DWORD channel,char * pLog);

	/************************************************************************/
	/* Description:
		ֹͣ����ִ��
		�ýӿڸ�RunDeviceOperator�ɶ�Ӧ��ϵ��������ֹ������ִ��
		���ô����磺
		StopRunning(m_deviceID);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool StopRunning(int deviceID);

	/************************************************************************/
	/* Description:
		��λϵͳ����Pxxϵ�л����������豸
		������δ֧�֣�����
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool ResetSystem(int deviceID);
	
	/************************************************************************/
	/* Description:
		��ȡ���ӿ��ļ��ṩ�Ľӿڳ�����ú�Ĵ�����
	/* Result:
		����ֵΪ�ļ�Pxx_LastErrCode.h�еô�����궨��ֵ
	/************************************************************************/
	PXX_ATE_API int Pxx_GetLastErr();

	/*���½ӿ�Ϊ�߼�ATE����*/
	/************************************************************************/
	/* Description:
		���������Ż�ȡ�豸�еĹ�������
		�����Ŵ��ݸ��򿪹��̵�������һ��
		���ô����磺
		char pName[MAX_PATH] = {0};
		GetProjectName(m_deviceID,m_proIdex,pName,MAX_PATH);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		idex:��������
		pBuffer:�����洢����������Ϣ�����ֽڱ��뷽ʽ
		dwSize:pBuffer��С
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool GetProjectName(int deviceID,int idex,char * pBuffer,int dwSize);

	/************************************************************************/
	/* Description:
		ͨ���������ƴ��豸���̣��ú������ܸ����������򿪹��̵�API��OpenDeviceProject��ͬ
		�û����Դ�������֪���Ĺ�������Ҳ���Ը���GetProjectName��ȡ�豸�Ĺ�������ȥ��
		���ô���Ϊ��
		char pName[MAX_PATH] = {0};
		GetProjectName(m_deviceID,m_proIdex,pName,MAX_PATH);
		OpenDeviceProjectName(m_deviceID,pName);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		name:��������,Ϊ���ֽڱ��뷽ʽ���ַ���
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool OpenDeviceProjectName(int deviceID,char * name);

	/************************************************************************/
	/* Description:
		ɾ���豸�ϵĹ���
		֧������ɾ��ģʽ��
		ͨ�����ɾ��DelProjectIdex
		ͨ����������ɾ��DelProjectName
		�����ɾ��Ϊ�������Ӵ����磺
		DelProjectIdex(m_deviceID,m_proIdex);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		DelProjectIdex::idex:Ҫɾ�����̵�����
		DelProjectName::name:Ҫɾ�����̵����ƣ����ö��ֽڱ��뷽ʽ����
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool DelProjectIdex(int deviceID,int idex);
	PXX_ATE_API bool DelProjectName(int deviceID,char * name);

	/************************************************************************/
	/* Description:
		���ͱ��ع����ļ��е��豸�ϣ�SD��P500_Projects����Ϊ������û�ѹ���㷨��һ�£�����ֻ֧���ļ��еķ���
		ֻ�贫�ݱ��صĹ���Ŀ¼���ӿ��м�������·�����Ҫ��֤�豸�����㹻�Ŀռ�洢�ù��̣����豸�в����ڶ�Ӧ��ͬ�����̣����������ɾ��
		���Ӵ����磺
		SendProjectForder(m_deviceID,_T("C:\\TestProject"));
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		path:Ҫ���͵ı���·�������ö��ֽڱ���
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool SendProjectForder(int deviceID,char * path);

	
	/************************************************************************/
	/* Description:
		��ȡ������豸����汾�������û����������汾�͹��̰汾��ƥ�䣬������ְ汾��ƥ������Ҫ���¶�Ӧ�İ汾
		���Ӵ����磺
		char pName[MAX_PATH] = {0};
		GetSoftVersion(m_deviceID,pName,MAX_PATH);
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		pBuffer:��������汾��Ϣ�����صĽ��Ϊ���ֽڱ��뷽ʽ���ַ���
		dwSize:�����С��>=MAX_PATH)
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool GetSoftVersion(int deviceID,char * pBuffer,int dwSize);

	/************************************************************************/
	/* Description:
		���±�����е����
		�û��ɽ��������Ŀ¼���µ��豸�У�����ʱ��ȽϾã����³���3-5�������ң��û�����������߳���ȥ����
		�ú�����ȷ���غ��û��ɵ���ConnectDeviceϵ�к���ִ�����Ӳ�����ֱ���ɹ���Ϊ���������ɣ���ʱʱ������Ϊ10���ӣ�
		���Ӵ����磺
		UpdateSoft(m_deviceID,_T("C:\\P500"));
		DWORD beg = GetTickCount();
		while(GetTickCount()-beg<1000*60*10)
		{
			if(ConnectDevice(...))
				break;//�ɹ�
		}
	/* Param:
		deviceID:�����豸��Ӧ��ID��ʶ����ֵΪConnectDeviceϵ�к����ķ���ֵ��
		path:Ҫ���µı������·��
	/* Result:
		false:ʧ��,ʧ�ܺ����ͨ�����ýӿ�Pxx_GetLastErr()��ȡʧ����Ϣ��ʧ�ܾ�����������Pxx_LastErrCode.h�ж���
		true:�ɹ�
	/************************************************************************/
	PXX_ATE_API bool UpdateSoft(int deviceID,char * path);


#ifdef __cplusplus
}
#endif

#endif