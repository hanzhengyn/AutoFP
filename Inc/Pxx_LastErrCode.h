/************************************************************************
  Copyright (C), 1988-2999, Guangzhou Zhi Yuan Electronics Co., Ltd.
  File name: Pxx_LastErrCode.h
  Author: zhoushiyou	Version:	v1.0   Date: 2015-9-18
  Description:   ATE相关错误码描述
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
/* 基础错误码定义
/************************************************************************/
#define PXX_LASTERRCODE_SUCCESS								0/*正确执行*/
#define PXX_LASTERRCODE_BASE_TRANS_ERR						1/*基础通讯库错误*/
#define PXX_LASTERRCODE_CONNECT_ERR							2/*连接失败——主要指网络问题，如需知道详细问题请调用系统GetLastError函数*/
#define PXX_LASTERRCODE_INVALID_HANDLE						3/*无效句柄*/
#define PXX_LASTERRCODE_SEND_ERR							4/*发送失败——主要指网络问题*/
#define PXX_LASTERRCODE_RECEIVE_ERR							5/*接受失败——主要指网络问题*/
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
#define PXX_LASTERRCODE_UNDEFINE_ERR						0xFFFFFFFF/*未知异常,主要由系统产生（如内存不足），获取具体原因请调用系统GetLastError函数*/


/************************************************************************/
/* 通讯交互信息错误码描述
/************************************************************************/
#define PXX_LASTERRCODE_CONNECT_CANT_IN						0x100/*设备不允许进入ATE模式*/
#define PXX_LASTERRCODE_CONNECT_ENTER_ATE					0x101/*进入ATE模式失败*/
#define PXX_LASTERRCODE_OPEN_PROJECT_ERR					0x102/*打开工程失败*/
#define PXX_LASTERRCODE_RUN_ERR								0x103/*执行操作失败*/
#define PXX_LASTERRCODE_STOP_RUN_ERR						0x104/*停止操作失败*/
#define PXX_LATEERRCODE_GET_PROJECT_ERR						0x105/*获取工程失败*/
#define PXX_LATEERRCODE_DEL_PROJECT_ERR						0x106/*删除工程失败*/
#define PXX_LATEERRCODE_SEND_PROJECT_ERR					0x107/*发送工程失败*/
#define PXX_LASTERRCODE_NO_VERSIONINFO						0x108/*未找到版本信息*/
#define PXX_LASTERRCODE_NO_SDCARD							0x109/*没有检测到SD卡*/
#define PXX_LASTERRCODE_CREATE_TMP_FILE_ERR					0x110/*创建临时文件失败*/
#define PXX_LASTERRCODE_RECEIVE_TMP_FILE_ERR				0x111/*接收临时文件失败*/
#define PXX_LASTERRCODE_UZIP_FILE_ERROR						0x112/*解压缩文件失败*/



#endif
