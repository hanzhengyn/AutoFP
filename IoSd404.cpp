#include "stdafx.h"
//Ҫ����ʹ�ã���Ҫ��� comidas.h �� comidas.cpp

//----------------------------------------------------------------------------------
/**
\brief  �л��忨������ź�
------------------------------------------------------
|	out		4			5			6			7
|	x��		����			����			serv_on		v2
|	y��		�Ϲ�Դ		�¹�Դ		serv_on		v3
|	z��		���			�̵�			serv_on		v4
|	r��		������		v1			v6			v5
-------------------------------------------------------
\brief  �л��忨�������ź�
------------------------------------------------------
|	in		0		1			2			3
|	x��		����					��ѹ��		��ѹ���
|	y��		v1					v2			v3
|	z��		v4					v5			v6
|	r��
-------------------------------------------------------
v1~v6 �������������ź�
*/
//----------------------------------------------------------------------------------

//���Ͱ忨��IO
/*
DI_00	����						DO_00	tray����¼����ź�/Feeder�����ź�
DI_01	��ѹ��1					DO_01	��������7
DI_02	�����ϴ����				DO_02	����1��������
DI_03	��Ĥ���					DO_03	����1��������
DI_04	���̵����ź�				DO_04	��������5
DI_05	��ѹ��					DO_05	��������2
DI_06							DO_06	�������Դ
DI_07	Feeder���Ϲ���			DO_07	��������6
DI_08	���������ź�7				DO_08	��������8
DI_09	���������ź�8				DO_09	���/������
DI_10	���������ź�5				DO_10	�̵�
DI_11	���������ź�6				DO_11	Feeder����
DI_12	���������ź�1				DO_12	��������1
DI_13	���������ź�2				DO_13	��������4
DI_14	���������ź�3				DO_14	��������3
DI_15	���������ź�4				DO_15	�������Դ
DI_16	����1�����ź�				DO_16	����2��������
DI_17	����2�����ź�				DO_17	����2��������
DI_18	����3�����ź�				DO_18	����1��������
DI_19	����4�����ź�				DO_19	����2��������
DI_20	��ѹ��2					DO_20	����3��������
DI_21	��ѹ��3					DO_21	����3��������
DI_22	��ѹ��4					DO_22	����3��������
DI_23							DO_23	����4��������
DI_24							DO_24	����4��������
DI_25							DO_25	����4��������
*/


#if SINGLE_AXIS//���Ῠ
#if BOARD_TYPE == COMI

#if FOUR_NUZZLE//������
#include "IoSd404.h"
//#include "ComiDaqApi.h"
//#include "comidaq.h"
#include "Comidas.h"
//#include "ComiIoSd404.h"
#define DEV_ID	COMI_SD404
#define DO_CH	0
#define DI_CH	0

HANDLE hDevice;
int do_state = 0, di_state;

void OpenIOBoard()
{
    if (!COMI_LoadDll()) {
    }

    hDevice = COMI_LoadDevice(DEV_ID, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
    }
    COMI_DIO_SetUsage(hDevice, DO_DI);


    //do_state ^= 1;  // state ���� 
    //COMI_DO_PutOne(hDevice, DO_CH, do_state); // Put D/O  

    //										  /* Get D/I and print on screen */
    //di_state = COMI_DI_GetOne(hDevice, DI_CH);

}
void CloseIOBoard()
{
    COMI_UnloadDevice(hDevice);
    COMI_UnloadDll();
}

//------------------------------------------------------------------------------
//DI

BOOL CheckDoorOpen()
{
    if (gm_bDoorAlarm)
        return TRUE;
    return COMI_DI_GetOne(hDevice, 0);
    //return m_ComiDaq.DiGetOne(0);
}

BOOL CheckNegtiveMeter(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 1);
        //return COMI_DI_GetOne(hDevice, 3);//��ʱ����
        //return m_ComiDaq.DiGetOne(1);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 20);
        //return m_ComiDaq.DiGetOne(20);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 21);
        //return m_ComiDaq.DiGetOne(21);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 22);
        //return m_ComiDaq.DiGetOne(22);
        break;
    default:
        break;
    }
    return FALSE;
}
//�����ϴ����
BOOL CheckOutTape()
{
    return COMI_DI_GetOne(hDevice, 2);
}
//�����ϴ����
BOOL CheckOutTapeIO()
{
    return COMI_DI_GetOne(hDevice, 2);//1��ʾ���ϴ�
}
//������Ĥ���
BOOL CheckOutTapeIOFilm()
{
    return COMI_DI_GetOne(hDevice, 3);//1��ʾ����Ĥ
}

BOOL CheckTrayArrive()
{
    return COMI_DI_GetOne(hDevice, 4);
    //return m_ComiDaq.DiGetOne(4);
}

BOOL CheckPositiveMeter()
{
    return COMI_DI_GetOne(hDevice, 5);
    //return m_ComiDaq.DiGetOne(5);
}

BOOL CheckFeederOutError()
{
    return COMI_DI_GetOne(hDevice, 7);
    //return m_ComiDaq.DiGetOne(7);
}

BOOL CheckModelCylinderPressed(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 12);
        //return m_ComiDaq.DiGetOne(12);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 13);
        //return m_ComiDaq.DiGetOne(13);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 14);
        //return m_ComiDaq.DiGetOne(14);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 15);
        //return m_ComiDaq.DiGetOne(15);
        break;
    case 4:
        return COMI_DI_GetOne(hDevice, 10);
        //return m_ComiDaq.DiGetOne(10);
        break;
    case 5:
        return COMI_DI_GetOne(hDevice, 11);
        //return m_ComiDaq.DiGetOne(11);
        break;
    case 6:
        return COMI_DI_GetOne(hDevice, 8);
        //return m_ComiDaq.DiGetOne(8);
        break;
    case 7:
        return COMI_DI_GetOne(hDevice, 9);
        //return m_ComiDaq.DiGetOne(9);
        break;
    default:
        break;
    }
    return FALSE;
}

BOOL CheckNozzleUpStatus(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 16);
        //return m_ComiDaq.DiGetOne(16);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 17);
        //return m_ComiDaq.DiGetOne(17);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 18);
        //return m_ComiDaq.DiGetOne(18);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 19);
        //return m_ComiDaq.DiGetOne(19);
        break;
    default:
        break;
    }
    return FALSE;
}

//--------------------------------------------------------------------------------
//DO

void TrayComplete()
{
    COMI_DO_PutOne(hDevice, 0, 1);
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(500);
    COMI_DO_PutOne(hDevice, 0, 0);
    //m_ComiDaq.DoPutOne(0, 0);
    Sleep(5000);
}

void SetFeederOut()
{
    COMI_DO_PutOne(hDevice, 0, 1);
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(100);
    COMI_DO_PutOne(hDevice, 0, 0);
    //m_ComiDaq.DoPutOne(0, 0);
}

void SetFeedIn()
{
    COMI_DO_PutOne(hDevice, 11, 1);
    //m_ComiDaq.DoPutOne(11, 1);
    Sleep(100);
    COMI_DO_PutOne(hDevice, 11, 0);
    //m_ComiDaq.DoPutOne(11, 0);
}
void SetCylinderDown(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 12, 1);
        //m_ComiDaq.DoPutOne(12, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 5, 1);
        //m_ComiDaq.DoPutOne(5, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 14, 1);
        //m_ComiDaq.DoPutOne(14, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 13, 1);
        //m_ComiDaq.DoPutOne(13, 1);
        break;
    case 4:
        COMI_DO_PutOne(hDevice, 4, 1);
        //m_ComiDaq.DoPutOne(4, 1);
        break;
    case 5:
        COMI_DO_PutOne(hDevice, 7, 1);
        //m_ComiDaq.DoPutOne(7, 1);
        break;
    case 6:
        COMI_DO_PutOne(hDevice, 1, 1);
        //m_ComiDaq.DoPutOne(1, 1);
        break;
    case 7:
        COMI_DO_PutOne(hDevice, 8, 1);
        //m_ComiDaq.DoPutOne(8, 1);
        break;
    default:
        break;
    }
}

void SetCylinderUp(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 12, 0);
        //m_ComiDaq.DoPutOne(12, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 5, 0);
        //m_ComiDaq.DoPutOne(5, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 14, 0);
        //m_ComiDaq.DoPutOne(14, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 13, 0);
        //m_ComiDaq.DoPutOne(13, 0);
        break;
    case 4:
        COMI_DO_PutOne(hDevice, 4, 0);
        //m_ComiDaq.DoPutOne(4, 0);
        break;
    case 5:
        COMI_DO_PutOne(hDevice, 7, 0);
        //m_ComiDaq.DoPutOne(7, 0);
        break;
    case 6:
        COMI_DO_PutOne(hDevice, 1, 0);
        //m_ComiDaq.DoPutOne(1, 0);
        break;
    case 7:
        COMI_DO_PutOne(hDevice, 8, 0);
        //m_ComiDaq.DoPutOne(8, 0);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 2, 1);
        //m_ComiDaq.DoPutOne(2, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 16, 1);
        //m_ComiDaq.DoPutOne(16, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 20, 1);
        //m_ComiDaq.DoPutOne(20, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 24, 1);
        //m_ComiDaq.DoPutOne(24, 1);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 2, 0);
        //m_ComiDaq.DoPutOne(2, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 16, 0);
        //m_ComiDaq.DoPutOne(16, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 20, 0);
        //m_ComiDaq.DoPutOne(20, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 24, 0);
        //m_ComiDaq.DoPutOne(24, 0);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 3, 1);
        //m_ComiDaq.DoPutOne(3, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 17, 1);
        //m_ComiDaq.DoPutOne(17, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 21, 1);
        //m_ComiDaq.DoPutOne(21, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 25, 1);
        //m_ComiDaq.DoPutOne(25, 1);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 3, 0);
        //m_ComiDaq.DoPutOne(3, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 17, 0);
        //m_ComiDaq.DoPutOne(17, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 21, 0);
        //m_ComiDaq.DoPutOne(21, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 25, 0);
        //m_ComiDaq.DoPutOne(25, 0);
        break;
    default:
        break;
    }
}

void SetRedLightOn()
{
    COMI_DO_PutOne(hDevice, 9, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetRedLightOff()
{
    COMI_DO_PutOne(hDevice, 9, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetBeepOn()
{
    COMI_DO_PutOne(hDevice, 9, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetBeepOff()
{
    COMI_DO_PutOne(hDevice, 9, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetGreenLightOn()
{
    COMI_DO_PutOne(hDevice, 10, 1);
    //m_ComiDaq.DoPutOne(10, 1);
}

void SetGreenLightOff()
{
    COMI_DO_PutOne(hDevice, 10, 0);
    //m_ComiDaq.DoPutOne(10, 0);
}

void SetUpCameraLightOn()
{
    COMI_DO_PutOne(hDevice, 6, 1);
    //m_ComiDaq.DoPutOne(6, 1);
}

void SetUpCameraLightOff()
{
    COMI_DO_PutOne(hDevice, 6, 0);
    //m_ComiDaq.DoPutOne(6, 0);
}

void SetDownCameraLightOn()
{
    COMI_DO_PutOne(hDevice, 15, 1);
    //m_ComiDaq.DoPutOne(15, 1);
}

void SetDownCameraLightOff()
{
    COMI_DO_PutOne(hDevice, 15, 0);
    //m_ComiDaq.DoPutOne(15, 0);
}

void SetNozzleDownOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 18, 1);
        //m_ComiDaq.DoPutOne(18, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 19, 1);
        //m_ComiDaq.DoPutOne(19, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 22, 1);
        //m_ComiDaq.DoPutOne(22, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 23, 1);
        //m_ComiDaq.DoPutOne(23, 1);
        break;
    default:
        break;
    }
}

void SetNozzleDownOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 18, 0);
        //m_ComiDaq.DoPutOne(18, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 19, 0);
        //m_ComiDaq.DoPutOne(19, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 22, 0);
        //m_ComiDaq.DoPutOne(22, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 23, 0);
        //m_ComiDaq.DoPutOne(23, 0);
        break;
    default:
        break;
    }
}

#else
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"

void OpenIOBoard()
{

}

void CloseIOBoard()
{
}
//����
BOOL CheckDoorOpen()
{
    if (gm_bDoorAlarm)
        return TRUE;
    long status = 0;
    cmmDiGetOne(0, &status);
    return status;
}
//��ѹ��
BOOL CheckNegtiveMeter(INT nNozzleNum)
{
    long status = 0;
    cmmDiGetOne(1, &status);
    return status;
}
//�����ϴ����
BOOL CheckOutTapeIO()
{
    long status = 0;
    cmmDiGetOne(2, &status);
    return status;//1��ʾ���ϴ�
}
//������Ĥ���
BOOL CheckOutTapeIOFilm()
{
    long status = 0;
    cmmDiGetOne(3, &status);
    return status;//1��ʾ����Ĥ
}
//tray�̵����ź�
BOOL CheckTrayArrive()
{
    long status = 0;
    cmmDiGetOne(4, &status);
    return status;
}
//��ѹ��
BOOL CheckPositiveMeter()
{
    long status = 0;
    cmmDiGetOne(5, &status);
    return status;
}
//�ɴ���Ϲ���
BOOL CheckFeederOutError()
{
    long status = 0;
    cmmDiGetOne(7, &status);
    //cmmDiGetOne(4, &status);//������ɴﱨ���ӵ�DI04
    return status;
}
//���������ź�
BOOL CheckModelCylinderPressed(INT nModelNum)
{
    long status = 0;
    long channel = 0;
    switch (nModelNum)
    {
    case 0:
        channel = (12);
        break;
    case 1:
        channel = (13);
        break;
    case 2:
        channel = (14);
        break;
    case 3:
        channel = (15);
        break;
    case 4:
        channel = (10);
        break;
    case 5:
        channel = (11);
        break;
    case 6:
        channel = (8);
        break;
    case 7:
        channel = (9);
        break;
    default:
        break;
    }
    cmmDiGetOne(channel, &status);
    return status;
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
    cmmDoPutOne(0, 1);
    Sleep(100);
    cmmDoPutOne(0, 0);
    Sleep(5000);
}
//�ɴ����
void SetFeederOut()
{
    cmmDoPutOne(0, 1);
    Sleep(50);
    cmmDoPutOne(0, 0);
}
//�ɴ����
void SetFeedIn()
{
    cmmDoPutOne(11, 1);
    Sleep(50);
    cmmDoPutOne(11, 0);
}
//��������
void SetCylinderDown(INT nModelNum)
{
    long status = 0;
    long channel = 0;
    switch (nModelNum)
    {
    case 0:
        channel = (12);
        break;
    case 1:
        channel = (5);
        break;
    case 2:
        channel = (14);
        break;
    case 3:
        channel = (13);
        break;
    case 4:
        channel = (4);
        break;
    case 5:
        channel = (7);
        break;
    case 6:
        channel = (1);
        break;
    case 7:
        channel = (8);
        break;
    case 8:
        channel = (22);
        break;
    case 9:
        channel = (23);
        break;
    default:
        break;
    }
    cmmDoPutOne(channel, 1);
}
//��������
void SetCylinderUp(INT nModelNum)
{
    long status = 0;
    long channel = 0;
    switch (nModelNum)
    {
    case 0:
        channel = (12);
        break;
    case 1:
        channel = (5);
        break;
    case 2:
        channel = (14);
        break;
    case 3:
        channel = (13);
        break;
    case 4:
        channel = (4);
        break;
    case 5:
        channel = (7);
        break;
    case 6:
        channel = (1);
        break;
    case 7:
        channel = (8);
        break;
    case 8:
        channel = (22);
        break;
    case 9:
        channel = (23);
        break;
    default:
        break;
    }
    cmmDoPutOne(channel, 0);
}
//����
void SetNozzleVaccumOn(INT nNozzleNum)
{
    cmmDoPutOne(2, 1);
}
//ֹͣ����
void SetNozzleVaccumOff(INT nNozzleNum)
{
    cmmDoPutOne(2, 0);
}
//����
void SetNozzleBlowOn(INT nNozzleNum)
{
    cmmDoPutOne(3, 1);
}
//ֹͣ����
void SetNozzleBlowOff(INT nNozzleNum)
{
    cmmDoPutOne(3, 0);
}
//�����
void SetRedLightOn()
{
    cmmDoPutOne(9, 1);
}
//�����
void SetRedLightOff()
{
    cmmDoPutOne(9, 0);
}
//��������
void SetBeepOn()
{
    cmmDoPutOne(9, 1);
}
//��������
void SetBeepOff()
{
    cmmDoPutOne(9, 0);
}
//�̵���
void SetGreenLightOn()
{
    cmmDoPutOne(10, 1);
}
//�̵���
void SetGreenLightOff()
{
    cmmDoPutOne(10, 0);
}
//����Ϲ�Դ��
void SetUpCameraLightOn()
{
    cmmDoPutOne(6, 1);
}
//����Ϲ�Դ��
void SetUpCameraLightOff()
{
    cmmDoPutOne(6, 0);
}
//����¹�Դ��
void SetDownCameraLightOn()
{
    cmmDoPutOne(15, 1);
}
//����¹�Դ��
void SetDownCameraLightOff()
{
    cmmDoPutOne(15, 0);
}
//��������
void SetNozzleDownOn(INT nNozzleNum)
{

}
//��������
void SetNozzleDownOff(INT nNozzleNum)
{

}




#endif // FOUR_NUZZLE


#elif BOARD_TYPE == ADVANTECH
#include "AdvMotApi.h"
#include "MotionAct.h"
void OpenIOBoard()
{

}

void CloseIOBoard()
{
}


#endif // BOARD_TYPE == COMI



#else//������
#if BOARD_TYPE == COMI
#include "IoSd404.h"
//#include "ComiDaqApi.h"
//#include "comidaq.h"
#include "Comidas.h"
//#include "ComiIoSd404.h"
#define DEV_ID	COMI_SD404
#define DO_CH	0
#define DI_CH	0

HANDLE hDevice;
int do_state = 0, di_state;

void OpenIOBoard()
{
    if (!COMI_LoadDll()) {
    }

    hDevice = COMI_LoadDevice(DEV_ID, 0);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
    }
    COMI_DIO_SetUsage(hDevice, DO_DI);


    //do_state ^= 1;  // state ���� 
    //COMI_DO_PutOne(hDevice, DO_CH, do_state); // Put D/O  

    //										  /* Get D/I and print on screen */
    //di_state = COMI_DI_GetOne(hDevice, DI_CH);

}
void CloseIOBoard()
{
    COMI_UnloadDevice(hDevice);
    COMI_UnloadDll();
}

//------------------------------------------------------------------------------
//DI

BOOL CheckDoorOpen()
{
    if (gm_bDoorAlarm)
        return TRUE;
    return COMI_DI_GetOne(hDevice, 0);
    //return m_ComiDaq.DiGetOne(0);
}

BOOL CheckNegtiveMeter(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 1);
        //return COMI_DI_GetOne(hDevice, 3);//��ʱ����
        //return m_ComiDaq.DiGetOne(1);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 20);
        //return m_ComiDaq.DiGetOne(20);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 21);
        //return m_ComiDaq.DiGetOne(21);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 22);
        //return m_ComiDaq.DiGetOne(22);
        break;
    default:
        break;
    }
    return FALSE;
}
//�����ϴ����
BOOL CheckOutTape()
{
    return COMI_DI_GetOne(hDevice, 2);
}

BOOL CheckTrayArrive()
{
    return COMI_DI_GetOne(hDevice, 4);
    //return m_ComiDaq.DiGetOne(4);
}

BOOL CheckPositiveMeter()
{
    return COMI_DI_GetOne(hDevice, 5);
    //return m_ComiDaq.DiGetOne(5);
}

BOOL CheckFeederOutError()
{
    return COMI_DI_GetOne(hDevice, 7);
    //return m_ComiDaq.DiGetOne(7);
}

BOOL CheckModelCylinderPressed(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 12);
        //return m_ComiDaq.DiGetOne(12);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 13);
        //return m_ComiDaq.DiGetOne(13);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 14);
        //return m_ComiDaq.DiGetOne(14);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 15);
        //return m_ComiDaq.DiGetOne(15);
        break;
    case 4:
        return COMI_DI_GetOne(hDevice, 10);
        //return m_ComiDaq.DiGetOne(10);
        break;
    case 5:
        return COMI_DI_GetOne(hDevice, 11);
        //return m_ComiDaq.DiGetOne(11);
        break;
    case 6:
        return COMI_DI_GetOne(hDevice, 8);
        //return m_ComiDaq.DiGetOne(8);
        break;
    case 7:
        return COMI_DI_GetOne(hDevice, 9);
        //return m_ComiDaq.DiGetOne(9);
        break;
    default:
        break;
    }
    return FALSE;
}

BOOL CheckNozzleUpStatus(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        return COMI_DI_GetOne(hDevice, 16);
        //return m_ComiDaq.DiGetOne(16);
        break;
    case 1:
        return COMI_DI_GetOne(hDevice, 17);
        //return m_ComiDaq.DiGetOne(17);
        break;
    case 2:
        return COMI_DI_GetOne(hDevice, 18);
        //return m_ComiDaq.DiGetOne(18);
        break;
    case 3:
        return COMI_DI_GetOne(hDevice, 19);
        //return m_ComiDaq.DiGetOne(19);
        break;
    default:
        break;
    }
    return FALSE;
}

//--------------------------------------------------------------------------------
//DO

void TrayComplete()
{
    COMI_DO_PutOne(hDevice, 0, 1);
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(500);
    COMI_DO_PutOne(hDevice, 0, 0);
    //m_ComiDaq.DoPutOne(0, 0);
    Sleep(5000);
}

void SetFeederOut()
{
    COMI_DO_PutOne(hDevice, 0, 1);
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(500);
    COMI_DO_PutOne(hDevice, 0, 0);
    //m_ComiDaq.DoPutOne(0, 0);
}

void SetFeedIn()
{
    COMI_DO_PutOne(hDevice, 11, 1);
    //m_ComiDaq.DoPutOne(11, 1);
    Sleep(500);
    COMI_DO_PutOne(hDevice, 11, 0);
    //m_ComiDaq.DoPutOne(11, 0);
}
void SetCylinderDown(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 12, 1);
        //m_ComiDaq.DoPutOne(12, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 5, 1);
        //m_ComiDaq.DoPutOne(5, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 14, 1);
        //m_ComiDaq.DoPutOne(14, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 13, 1);
        //m_ComiDaq.DoPutOne(13, 1);
        break;
    case 4:
        COMI_DO_PutOne(hDevice, 4, 1);
        //m_ComiDaq.DoPutOne(4, 1);
        break;
    case 5:
        COMI_DO_PutOne(hDevice, 7, 1);
        //m_ComiDaq.DoPutOne(7, 1);
        break;
    case 6:
        COMI_DO_PutOne(hDevice, 1, 1);
        //m_ComiDaq.DoPutOne(1, 1);
        break;
    case 7:
        COMI_DO_PutOne(hDevice, 8, 1);
        //m_ComiDaq.DoPutOne(8, 1);
        break;
    default:
        break;
    }
}

void SetCylinderUp(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 12, 0);
        //m_ComiDaq.DoPutOne(12, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 5, 0);
        //m_ComiDaq.DoPutOne(5, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 14, 0);
        //m_ComiDaq.DoPutOne(14, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 13, 0);
        //m_ComiDaq.DoPutOne(13, 0);
        break;
    case 4:
        COMI_DO_PutOne(hDevice, 4, 0);
        //m_ComiDaq.DoPutOne(4, 0);
        break;
    case 5:
        COMI_DO_PutOne(hDevice, 7, 0);
        //m_ComiDaq.DoPutOne(7, 0);
        break;
    case 6:
        COMI_DO_PutOne(hDevice, 1, 0);
        //m_ComiDaq.DoPutOne(1, 0);
        break;
    case 7:
        COMI_DO_PutOne(hDevice, 8, 0);
        //m_ComiDaq.DoPutOne(8, 0);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 2, 1);
        //m_ComiDaq.DoPutOne(2, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 16, 1);
        //m_ComiDaq.DoPutOne(16, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 20, 1);
        //m_ComiDaq.DoPutOne(20, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 24, 1);
        //m_ComiDaq.DoPutOne(24, 1);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 2, 0);
        //m_ComiDaq.DoPutOne(2, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 16, 0);
        //m_ComiDaq.DoPutOne(16, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 20, 0);
        //m_ComiDaq.DoPutOne(20, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 24, 0);
        //m_ComiDaq.DoPutOne(24, 0);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 3, 1);
        //m_ComiDaq.DoPutOne(3, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 17, 1);
        //m_ComiDaq.DoPutOne(17, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 21, 1);
        //m_ComiDaq.DoPutOne(21, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 25, 1);
        //m_ComiDaq.DoPutOne(25, 1);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 3, 0);
        //m_ComiDaq.DoPutOne(3, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 17, 0);
        //m_ComiDaq.DoPutOne(17, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 21, 0);
        //m_ComiDaq.DoPutOne(21, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 25, 0);
        //m_ComiDaq.DoPutOne(25, 0);
        break;
    default:
        break;
    }
}

void SetRedLightOn()
{
    COMI_DO_PutOne(hDevice, 9, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetRedLightOff()
{
    COMI_DO_PutOne(hDevice, 9, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetBeepOn()
{
    COMI_DO_PutOne(hDevice, 9, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetBeepOff()
{
    COMI_DO_PutOne(hDevice, 9, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetGreenLightOn()
{
    COMI_DO_PutOne(hDevice, 10, 1);
    //m_ComiDaq.DoPutOne(10, 1);
}

void SetGreenLightOff()
{
    COMI_DO_PutOne(hDevice, 10, 0);
    //m_ComiDaq.DoPutOne(10, 0);
}

void SetUpCameraLightOn()
{
    COMI_DO_PutOne(hDevice, 6, 1);
    //m_ComiDaq.DoPutOne(6, 1);
}

void SetUpCameraLightOff()
{
    COMI_DO_PutOne(hDevice, 6, 0);
    //m_ComiDaq.DoPutOne(6, 0);
}

void SetDownCameraLightOn()
{
    COMI_DO_PutOne(hDevice, 15, 1);
    //m_ComiDaq.DoPutOne(15, 1);
}

void SetDownCameraLightOff()
{
    COMI_DO_PutOne(hDevice, 15, 0);
    //m_ComiDaq.DoPutOne(15, 0);
}

void SetNozzleDownOn(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 18, 1);
        //m_ComiDaq.DoPutOne(18, 1);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 19, 1);
        //m_ComiDaq.DoPutOne(19, 1);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 22, 1);
        //m_ComiDaq.DoPutOne(22, 1);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 23, 1);
        //m_ComiDaq.DoPutOne(23, 1);
        break;
    default:
        break;
    }
}

void SetNozzleDownOff(INT nNozzleNum)
{
    switch (nNozzleNum)
    {
    case 0:
        COMI_DO_PutOne(hDevice, 18, 0);
        //m_ComiDaq.DoPutOne(18, 0);
        break;
    case 1:
        COMI_DO_PutOne(hDevice, 19, 0);
        //m_ComiDaq.DoPutOne(19, 0);
        break;
    case 2:
        COMI_DO_PutOne(hDevice, 22, 0);
        //m_ComiDaq.DoPutOne(22, 0);
        break;
    case 3:
        COMI_DO_PutOne(hDevice, 23, 0);
        //m_ComiDaq.DoPutOne(23, 0);
        break;
    default:
        break;
    }
}


//CComiIoSd404 m_ComiDaq;//�������ݲɼ�������
//
//void OpenIOBoard()
//{
//	m_ComiDaq.OpenIOBoard();
//}
//
////------------------------------------------------------------------------------
////DI
//
//BOOL CheckDoorOpen()
//{
//	return m_ComiDaq.CheckDoorOpen();
//}
//
//BOOL CheckNegtiveMeter(INT nNozzleNum)
//{
//	return CheckNegtiveMeter(nNozzleNum);
//}
//
//BOOL CheckTrayArrive()
//{
//	return m_ComiDaq.CheckTrayArrive();
//}
//
//BOOL CheckPositiveMeter()
//{
//	return m_ComiDaq.CheckPositiveMeter();
//}
//
//BOOL CheckFeederOutError()
//{
//	return m_ComiDaq.CheckFeederOutError();
//}
//
//BOOL CheckModelCylinderPressed(INT nModelNum)
//{
//	return m_ComiDaq.CheckModelCylinderPressed(nModelNum);
//}
//
//BOOL CheckNozzleUpStatus(INT nNozzleNum)
//{
//	return m_ComiDaq.CheckNozzleUpStatus(nNozzleNum);
//}
//
////--------------------------------------------------------------------------------
////DO
//
//void TrayComplete()
//{
//	m_ComiDaq.TrayComplete();
//}
//
//void SetFeederOut()
//{
//	m_ComiDaq.SetFeederOut();
//}
//
//void SetFeedIn()
//{
//	m_ComiDaq.SetFeedIn();
//}
//void SetCylinderDown(INT nModelNum)
//{
//	m_ComiDaq.SetCylinderDown(nModelNum);
//}
//
//void SetCylinderUp(INT nModelNum)
//{
//	m_ComiDaq.SetCylinderUp(nModelNum);
//}
//
//void SetNozzleVaccumOn(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleVaccumOn(nNozzleNum);
//}
//
//void SetNozzleVaccumOff(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleVaccumOff(nNozzleNum);
//}
//
//void SetNozzleBlowOn(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleBlowOn(nNozzleNum);
//}
//
//void SetNozzleBlowOff(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleBlowOff(nNozzleNum);
//}
//
//void SetRedLightOn()
//{
//	m_ComiDaq.SetRedLightOn();
//}
//
//void SetRedLightOff()
//{
//	m_ComiDaq.SetRedLightOff();
//}
//
//void SetBeepOn()
//{
//	m_ComiDaq.SetBeepOn();
//}
//
//void SetBeepOff()
//{
//	m_ComiDaq.SetBeepOff();
//}
//
//void SetGreenLightOn()
//{
//	m_ComiDaq.SetGreenLightOn();
//}
//
//void SetGreenLightOff()
//{
//	m_ComiDaq.SetGreenLightOff();
//}
//
//void SetUpCameraLightOn()
//{
//	m_ComiDaq.SetUpCameraLightOn();
//}
//
//void SetUpCameraLightOff()
//{
//	m_ComiDaq.SetUpCameraLightOff();
//}
//
//void SetDownCameraLightOn()
//{
//	m_ComiDaq.SetDownCameraLightOn();
//}
//
//void SetDownCameraLightOff()
//{
//	m_ComiDaq.SetDownCameraLightOff();
//}
//
//void SetNozzleDownOn(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleDownOn(nNozzleNum);
//}
//
//void SetNozzleDownOff(INT nNozzleNum)
//{
//	m_ComiDaq.SetNozzleDownOff(nNozzleNum);
//}

//void OpenIOBoard()
//{
//	m_ComiDaq.GnDeviceLoad();
//	m_ComiDaq.SetDeviceId(COMI_SD404);
//	m_ComiDaq.SetDevInstance(0);
//	if (m_ComiDaq.GnGetErrorCode() < 0)
//	{
//		m_ComiDaq.GnShowLastError();
//	}
//	else
//	{
//		m_ComiDaq.SetDlogLevel(0);
//		m_ComiDaq.SetDlogFile("c:\\S100\\DaqLog\\ComiDAQ.log");
//		m_ComiDaq.SetEnableErrorMsgDlg(1);
//		m_ComiDaq.DioSetUsage(cmDI_DO);
//	}
//
//}
//
////------------------------------------------------------------------------------
////DI
//
//BOOL CheckDoorOpen()
//{
//	return m_ComiDaq.DiGetOne(0);
//}
//
//BOOL CheckNegtiveMeter(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		return m_ComiDaq.DiGetOne(1);
//		break;
//	case 1:
//		return m_ComiDaq.DiGetOne(20);
//		break;
//	case 2:
//		return m_ComiDaq.DiGetOne(21);
//		break;
//	case 3:
//		return m_ComiDaq.DiGetOne(22);
//		break;
//	default:
//		break;
//	}
//	return FALSE;
//}
//
//BOOL CheckTrayArrive()
//{
//	return m_ComiDaq.DiGetOne(4);
//}
//
//BOOL CheckPositiveMeter()
//{
//	return m_ComiDaq.DiGetOne(5);
//}
//
//BOOL CheckFeederOutError()
//{
//	return m_ComiDaq.DiGetOne(7);
//}
//
//BOOL CheckModelCylinderPressed(INT nModelNum)
//{
//	switch (nModelNum)
//	{
//	case 0:
//		return m_ComiDaq.DiGetOne(12);
//		break;
//	case 1:
//		return m_ComiDaq.DiGetOne(13);
//		break;
//	case 2:
//		return m_ComiDaq.DiGetOne(14);
//		break;
//	case 3:
//		return m_ComiDaq.DiGetOne(15);
//		break;
//	case 4:
//		return m_ComiDaq.DiGetOne(10);
//		break;
//	case 5:
//		return m_ComiDaq.DiGetOne(11);
//		break;
//	case 6:
//		return m_ComiDaq.DiGetOne(8);
//		break;
//	case 7:
//		return m_ComiDaq.DiGetOne(9);
//		break;
//	default:
//		break;
//	}
//	return FALSE;
//}
//
//BOOL CheckNozzleUpStatus(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		return m_ComiDaq.DiGetOne(16);
//		break;
//	case 1:
//		return m_ComiDaq.DiGetOne(17);
//		break;
//	case 2:
//		return m_ComiDaq.DiGetOne(18);
//		break;
//	case 3:
//		return m_ComiDaq.DiGetOne(19);
//		break;
//	default:
//		break;
//	}
//	return FALSE;
//}
//
////--------------------------------------------------------------------------------
////DO
//
//void TrayComplete()
//{
//	m_ComiDaq.DoPutOne(0, 1);
//	Sleep(500);
//	m_ComiDaq.DoPutOne(0, 0);
//}
//
//void SetFeederOut()
//{
//	m_ComiDaq.DoPutOne(0, 1);
//	Sleep(500);
//	m_ComiDaq.DoPutOne(0, 0);
//}
//
//void SetFeedIn()
//{
//	m_ComiDaq.DoPutOne(11, 1);
//	Sleep(500);
//	m_ComiDaq.DoPutOne(11, 0);
//}
//void SetCylinderDown(INT nModelNum)
//{
//	switch (nModelNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(12, 1);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(5, 1);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(14, 1);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(13, 1);
//		break;
//	case 4:
//		m_ComiDaq.DoPutOne(4, 1);
//		break;
//	case 5:
//		m_ComiDaq.DoPutOne(7, 1);
//		break;
//	case 6:
//		m_ComiDaq.DoPutOne(1, 1);
//		break;
//	case 7:
//		m_ComiDaq.DoPutOne(8, 1);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetCylinderUp(INT nModelNum)
//{
//	switch (nModelNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(12, 0);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(5, 0);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(14, 0);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(13, 0);
//		break;
//	case 4:
//		m_ComiDaq.DoPutOne(4, 0);
//		break;
//	case 5:
//		m_ComiDaq.DoPutOne(7, 0);
//		break;
//	case 6:
//		m_ComiDaq.DoPutOne(1, 0);
//		break;
//	case 7:
//		m_ComiDaq.DoPutOne(8, 0);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetNozzleVaccumOn(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(2, 1);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(16, 1);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(20, 1);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(24, 1);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetNozzleVaccumOff(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(2, 0);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(16, 0);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(20, 0);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(24, 0);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetNozzleBlowOn(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(3, 1);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(17, 1);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(21, 1);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(25, 1);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetNozzleBlowOff(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(3, 0);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(17, 0);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(21, 0);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(25, 0);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetRedLightOn()
//{
//	m_ComiDaq.DoPutOne(9, 1);
//}
//
//void SetRedLightOff()
//{
//	m_ComiDaq.DoPutOne(9, 0);
//}
//
//void SetBeepOn()
//{
//	m_ComiDaq.DoPutOne(9, 1);
//}
//
//void SetBeepOff()
//{
//	m_ComiDaq.DoPutOne(9, 0);
//}
//
//void SetGreenLightOn()
//{
//	m_ComiDaq.DoPutOne(10, 1);
//}
//
//void SetGreenLightOff()
//{
//	m_ComiDaq.DoPutOne(10, 0);
//}
//
//void SetUpCameraLightOn()
//{
//	m_ComiDaq.DoPutOne(6, 1);
//}
//
//void SetUpCameraLightOff()
//{
//	m_ComiDaq.DoPutOne(6, 0);
//}
//
//void SetDownCameraLightOn()
//{
//	m_ComiDaq.DoPutOne(15, 1);
//}
//
//void SetDownCameraLightOff()
//{
//	m_ComiDaq.DoPutOne(15, 0);
//}
//
//void SetNozzleDownOn(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(18, 1);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(19, 1);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(22, 1);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(23, 1);
//		break;
//	default:
//		break;
//	}
//}
//
//void SetNozzleDownOff(INT nNozzleNum)
//{
//	switch (nNozzleNum)
//	{
//	case 0:
//		m_ComiDaq.DoPutOne(18, 0);
//		break;
//	case 1:
//		m_ComiDaq.DoPutOne(19, 0);
//		break;
//	case 2:
//		m_ComiDaq.DoPutOne(22, 0);
//		break;
//	case 3:
//		m_ComiDaq.DoPutOne(23, 0);
//		break;
//	default:
//		break;
//	}
//}
#elif BOARD_TYPE == GOOGOL

#include "Lib\googol\gts.h"
#include "ExtMdl.h"
#pragma comment(lib,"ExtMdl.lib")
void OpenIOBoard()
{
    short sRtn;
    //GT_OpenExtMdl();
    //GT_ResetExtMdl();
    sRtn = GT_OpenExtMdl(/*"G:\\myproject\\AutoFP\\Debug\\ExtMdl.dll"*/);
    if (sRtn)
    {
        CString str;
        str.Format("%s = %d\n", "GT_OpenExtMdl", sRtn);
        AfxMessageBox(str);
    }
    sRtn = GT_LoadExtConfig("G:\\myproject\\AutoFP\\Debug\\ExtMdl.cfg");
    if (sRtn)
    {
        CString str;
        str.Format("%s = %d\n", "GT_LoadExtConfig", sRtn);
        AfxMessageBox(str);
    }
}
void CloseIOBoard()
{
    GT_CloseExtMdl();
}

//------------------------------------------------------------------------------
//DI

BOOL CheckDoorOpen()
{
    if (gm_bDoorAlarm)
        return TRUE;
    // ָ���ֵ
    short sRtn;
    // ͨ�������ȡֵ
    long lGpiValue;
    // ��ȡEXI3����ֵ
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    // �˺���Ϊ���ָ���ֵ�������ο����� 2 1���GTָ���Ƿ�����ִ��
    //commandhandler("GT_GetDi", sRtn);
    // ���Ϊ�ߵ�ƽ��˵���������ڱ����£��򲻼�⣬����1
    if (lGpiValue & (1 << 0))
        return TRUE;
    return FALSE;
}
#define BASE_IO 16
#define EXT_IO 0
BOOL CheckNegtiveMeter(INT nNozzleNum)
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    unsigned short value;

    switch (nNozzleNum)
    {
    case 0:
        return lGpiValue & (1 << 1);
        break;
    case 1:
        GT_GetExtIoBit(EXT_IO, 20 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 20);
        //return m_ComiDaq.DiGetOne(20);
        break;
    case 2:
        GT_GetExtIoBit(EXT_IO, 21 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 21);
        //return m_ComiDaq.DiGetOne(21);
        break;
    case 3:
        GT_GetExtIoBit(EXT_IO, 22 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 22);
        //return m_ComiDaq.DiGetOne(22);
        break;
    default:
        break;
    }
    return FALSE;
}
//�����ϴ����
BOOL CheckOutTape()
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    return lGpiValue & (1 << 2);
}

BOOL CheckTrayArrive()
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    return lGpiValue & (1 << 4);
    //return m_ComiDaq.DiGetOne(4);
}

BOOL CheckPositiveMeter()
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    return lGpiValue & (1 << 5);
    //return m_ComiDaq.DiGetOne(5);
}

BOOL CheckFeederOutError()
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    return lGpiValue & (1 << 7);
    //return m_ComiDaq.DiGetOne(7);
}

BOOL CheckModelCylinderPressed(INT nModelNum)
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    //return lGpiValue & (1 << 2);
    switch (nModelNum)
    {
    case 0:
        return lGpiValue & (1 << 12);
        //return m_ComiDaq.DiGetOne(12);
        break;
    case 1:
        return lGpiValue & (1 << 13);
        //return m_ComiDaq.DiGetOne(13);
        break;
    case 2:
        return lGpiValue & (1 << 14);
        //return m_ComiDaq.DiGetOne(14);
        break;
    case 3:
        return lGpiValue & (1 << 15);
        //return m_ComiDaq.DiGetOne(15);
        break;
    case 4:
        return lGpiValue & (1 << 10);
        //return m_ComiDaq.DiGetOne(10);
        break;
    case 5:
        return lGpiValue & (1 << 11);
        //return m_ComiDaq.DiGetOne(11);
        break;
    case 6:
        return lGpiValue & (1 << 8);
        //return m_ComiDaq.DiGetOne(8);
        break;
    case 7:
        return lGpiValue & (1 << 9);
        //return m_ComiDaq.DiGetOne(9);
        break;
    default:
        break;
    }
    return FALSE;
}

BOOL CheckNozzleUpStatus(INT nNozzleNum)
{
    short sRtn;
    long lGpiValue;
    sRtn = GT_GetDi(MC_GPI, &lGpiValue);
    //commandhandler("GT_GetDi", sRtn);
    //return lGpiValue & (1 << 2);
    unsigned short value;

    switch (nNozzleNum)
    {
    case 0:
        GT_GetExtIoBit(EXT_IO, 16 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 16);
        //return m_ComiDaq.DiGetOne(16);
        break;
    case 1:
        GT_GetExtIoBit(EXT_IO, 17 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 17);
        //return m_ComiDaq.DiGetOne(17);
        break;
    case 2:
        GT_GetExtIoBit(EXT_IO, 18 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 18);
        //return m_ComiDaq.DiGetOne(18);
        break;
    case 3:
        GT_GetExtIoBit(EXT_IO, 19 - BASE_IO, &value);
        return !value;
        //return lGpiValue & (1 << 19);
        //return m_ComiDaq.DiGetOne(19);
        break;
    default:
        break;
    }
    return FALSE;
}

//--------------------------------------------------------------------------------
//DO

void TrayComplete()
{
    GT_SetDoBit(
        MC_GPO,				// ָ������IO������ͨ�����
        0 + 1,						// ָ����7��ͨ���������EXO6
        1);						// ����͵�ƽ
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(500);
    GT_SetDoBit(
        MC_GPO,				// ָ������IO������ͨ�����
        0 + 1,						// ָ����7��ͨ���������EXO6
        0);						// ����͵�ƽ
    //m_ComiDaq.DoPutOne(0, 0);
    Sleep(5000);
}

void SetFeederOut()
{
    GT_SetDoBit(MC_GPO, 0 + 1, 1);
    //m_ComiDaq.DoPutOne(0, 1);
    Sleep(100);
    GT_SetDoBit(MC_GPO, 0 + 1, 0);
    //m_ComiDaq.DoPutOne(0, 0);
}

void SetFeedIn()
{
    GT_SetDoBit(MC_GPO, 11 + 1, 1);
    //m_ComiDaq.DoPutOne(11, 1);
    Sleep(100);
    GT_SetDoBit(MC_GPO, 11 + 1, 0);
    //m_ComiDaq.DoPutOne(11, 0);
}
void SetCylinderDown(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 12 + 1, 1);
        //m_ComiDaq.DoPutOne(12, 1);
        break;
    case 1:
        GT_SetDoBit(MC_GPO, 5 + 1, 1);
        //m_ComiDaq.DoPutOne(5, 1);
        break;
    case 2:
        GT_SetDoBit(MC_GPO, 14 + 1, 1);
        //m_ComiDaq.DoPutOne(14, 1);
        break;
    case 3:
        GT_SetDoBit(MC_GPO, 13 + 1, 1);
        //m_ComiDaq.DoPutOne(13, 1);
        break;
    case 4:
        GT_SetDoBit(MC_GPO, 4 + 1, 1);
        //m_ComiDaq.DoPutOne(4, 1);
        break;
    case 5:
        GT_SetDoBit(MC_GPO, 7 + 1, 1);
        //m_ComiDaq.DoPutOne(7, 1);
        break;
    case 6:
        GT_SetDoBit(MC_GPO, 1 + 1, 1);
        //m_ComiDaq.DoPutOne(1, 1);
        break;
    case 7:
        GT_SetDoBit(MC_GPO, 8 + 1, 1);
        //m_ComiDaq.DoPutOne(8, 1);
        break;
    default:
        break;
    }
}

void SetCylinderUp(INT nModelNum)
{
    switch (nModelNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 12 + 1, 0);
        //m_ComiDaq.DoPutOne(12, 0);
        break;
    case 1:
        GT_SetDoBit(MC_GPO, 5 + 1, 0);
        //m_ComiDaq.DoPutOne(5, 0);
        break;
    case 2:
        GT_SetDoBit(MC_GPO, 14 + 1, 0);
        //m_ComiDaq.DoPutOne(14, 0);
        break;
    case 3:
        GT_SetDoBit(MC_GPO, 13 + 1, 0);
        //m_ComiDaq.DoPutOne(13, 0);
        break;
    case 4:
        GT_SetDoBit(MC_GPO, 4 + 1, 0);
        //m_ComiDaq.DoPutOne(4, 0);
        break;
    case 5:
        GT_SetDoBit(MC_GPO, 7 + 1, 0);
        //m_ComiDaq.DoPutOne(7, 0);
        break;
    case 6:
        GT_SetDoBit(MC_GPO, 1 + 1, 0);
        //m_ComiDaq.DoPutOne(1, 0);
        break;
    case 7:
        GT_SetDoBit(MC_GPO, 8 + 1, 0);
        //m_ComiDaq.DoPutOne(8, 0);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOn(INT nNozzleNum)
{
    unsigned short value;
    value = 0;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 2 + 1, 1);
        //m_ComiDaq.DoPutOne(2, 1);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 16 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 16 + 1, 1);
        //m_ComiDaq.DoPutOne(16, 1);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 20 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 20 + 1, 1);
        //m_ComiDaq.DoPutOne(20, 1);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 24 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 24 + 1, 1);
        //m_ComiDaq.DoPutOne(24, 1);
        break;
    default:
        break;
    }
}

void SetNozzleVaccumOff(INT nNozzleNum)
{
    unsigned short value;
    value = 1;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 2 + 1, 0);
        //m_ComiDaq.DoPutOne(2, 0);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 16 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 16 + 1, 0);
        //m_ComiDaq.DoPutOne(16, 0);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 20 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 20 + 1, 0);
        //m_ComiDaq.DoPutOne(20, 0);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 24 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 24 + 1, 0);
        //m_ComiDaq.DoPutOne(24, 0);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOn(INT nNozzleNum)
{
    unsigned short value;
    value = 0;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 3 + 1, 1);
        //m_ComiDaq.DoPutOne(3, 1);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 17 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 17 + 1, 1);
        //m_ComiDaq.DoPutOne(17, 1);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 21 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 21 + 1, 1);
        //m_ComiDaq.DoPutOne(21, 1);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 25 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 25 + 1, 1);
        //m_ComiDaq.DoPutOne(25, 1);
        break;
    default:
        break;
    }
}

void SetNozzleBlowOff(INT nNozzleNum)
{
    unsigned short value;
    value = 1;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetDoBit(MC_GPO, 3 + 1, 0);
        //m_ComiDaq.DoPutOne(3, 0);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 17 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 17 + 1, 0);
        //m_ComiDaq.DoPutOne(17, 0);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 21 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 21 + 1, 0);
        //m_ComiDaq.DoPutOne(21, 0);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 25 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 25 + 1, 0);
        //m_ComiDaq.DoPutOne(25, 0);
        break;
    default:
        break;
    }
}

void SetRedLightOn()
{
    GT_SetDoBit(MC_GPO, 9 + 1, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetRedLightOff()
{
    GT_SetDoBit(MC_GPO, 9 + 1, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetBeepOn()
{
    GT_SetDoBit(MC_GPO, 9 + 1, 1);
    //m_ComiDaq.DoPutOne(9, 1);
}

void SetBeepOff()
{
    GT_SetDoBit(MC_GPO, 9 + 1, 0);
    //m_ComiDaq.DoPutOne(9, 0);
}

void SetGreenLightOn()
{
    GT_SetDoBit(MC_GPO, 10 + 1, 1);
    //m_ComiDaq.DoPutOne(10, 1);
}

void SetGreenLightOff()
{
    GT_SetDoBit(MC_GPO, 10 + 1, 0);
    //m_ComiDaq.DoPutOne(10, 0);
}

void SetUpCameraLightOn()
{
    GT_SetDoBit(MC_GPO, 6 + 1, 1);
    //m_ComiDaq.DoPutOne(6, 1);
}

void SetUpCameraLightOff()
{
    GT_SetDoBit(MC_GPO, 6 + 1, 0);
    //m_ComiDaq.DoPutOne(6, 0);
}

void SetDownCameraLightOn()
{
    GT_SetDoBit(MC_GPO, 15 + 1, 1);
    //m_ComiDaq.DoPutOne(15, 1);
}

void SetDownCameraLightOff()
{
    GT_SetDoBit(MC_GPO, 15 + 1, 0);
    //m_ComiDaq.DoPutOne(15, 0);
}

void SetNozzleDownOn(INT nNozzleNum)
{
    unsigned short value;
    value = 0;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetExtIoBit(EXT_IO, 18 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 18 + 1, 1);
        //m_ComiDaq.DoPutOne(18, 1);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 19 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 19 + 1, 1);
        //m_ComiDaq.DoPutOne(19, 1);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 22 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 22 + 1, 1);
        //m_ComiDaq.DoPutOne(22, 1);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 23 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 23 + 1, 1);
        //m_ComiDaq.DoPutOne(23, 1);
        break;
    default:
        break;
    }
}

void SetNozzleDownOff(INT nNozzleNum)
{
    unsigned short value;
    value = 1;
    switch (nNozzleNum)
    {
    case 0:
        GT_SetExtIoBit(EXT_IO, 18 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 18 + 1, 0);
        //m_ComiDaq.DoPutOne(18, 0);
        break;
    case 1:
        GT_SetExtIoBit(EXT_IO, 19 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 19 + 1, 0);
        //m_ComiDaq.DoPutOne(19, 0);
        break;
    case 2:
        GT_SetExtIoBit(EXT_IO, 22 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 22 + 1, 0);
        //m_ComiDaq.DoPutOne(22, 0);
        break;
    case 3:
        GT_SetExtIoBit(EXT_IO, 23 - BASE_IO, value);
        //GT_SetDoBit(MC_GPO, 23 + 1, 0);
        //m_ComiDaq.DoPutOne(23, 0);
        break;
    default:
        break;
    }
}



#endif // BOARD_TYPE == COMI


#endif