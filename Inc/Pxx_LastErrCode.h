/************************************************************************
  Copyright (C), 1988-2999, Guangzhou Zhi Yuan Electronics Co., Ltd.
  File name: Pxx_LastErrCode.h
  Author: zhoushiyou	Version:	v1.0   Date: 2015-9-18
  Description:   ATE��ش���������
  Others Description:  
  Function List:  
  History:                     
    1. Date: 2015-9-18
       Author:zhoushiyou
       Modification:create
************************************************************************/
#ifndef PXX_LAST_ERR_CODE_H
#define PXX_LAST_ERR_CODE_H


/************************************************************************/
/* ���������붨��
/************************************************************************/
#define PXX_LASTERRCODE_SUCCESS								0/*��ȷִ��*/
#define PXX_LASTERRCODE_BASE_TRANS_ERR						1/*����ͨѶ�����*/
#define PXX_LASTERRCODE_CONNECT_ERR							2/*����ʧ�ܡ�����Ҫָ�������⣬����֪����ϸ���������ϵͳGetLastError����*/
#define PXX_LASTERRCODE_INVALID_HANDLE						3/*��Ч���*/
#define PXX_LASTERRCODE_SEND_ERR							4/*����ʧ�ܡ�����Ҫָ��������*/
#define PXX_LASTERRCODE_RECEIVE_ERR							5/*����ʧ�ܡ�����Ҫָ��������*/
#define PXX_LASTERRCODE_UN
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_
#define PXX_LASTERRCODE_UNDEFINE_ERR						0xFFFFFFFF/*δ֪�쳣,��Ҫ��ϵͳ���������ڴ治�㣩����ȡ����ԭ�������ϵͳGetLastError����*/


/************************************************************************/
/* ͨѶ������Ϣ����������
/************************************************************************/
#define PXX_LASTERRCODE_CONNECT_CANT_IN						0x100/*�豸���������ATEģʽ*/
#define PXX_LASTERRCODE_CONNECT_ENTER_ATE					0x101/*����ATEģʽʧ��*/
#define PXX_LASTERRCODE_OPEN_PROJECT_ERR					0x102/*�򿪹���ʧ��*/
#define PXX_LASTERRCODE_RUN_ERR								0x103/*ִ�в���ʧ��*/
#define PXX_LASTERRCODE_STOP_RUN_ERR						0x104/*ֹͣ����ʧ��*/
#define PXX_LATEERRCODE_GET_PROJECT_ERR						0x105/*��ȡ����ʧ��*/
#define PXX_LATEERRCODE_DEL_PROJECT_ERR						0x106/*ɾ������ʧ��*/
#define PXX_LATEERRCODE_SEND_PROJECT_ERR					0x107/*���͹���ʧ��*/
#define PXX_LASTERRCODE_NO_VERSIONINFO						0x108/*δ�ҵ��汾��Ϣ*/
#define PXX_LASTERRCODE_NO_SDCARD							0x109/*û�м�⵽SD��*/
#define PXX_LASTERRCODE_CREATE_TMP_FILE_ERR					0x110/*������ʱ�ļ�ʧ��*/
#define PXX_LASTERRCODE_RECEIVE_TMP_FILE_ERR				0x111/*������ʱ�ļ�ʧ��*/
#define PXX_LASTERRCODE_UZIP_FILE_ERROR						0x112/*��ѹ���ļ�ʧ��*/



#endif
