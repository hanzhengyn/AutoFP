/************************************************************************
  Copyright (C), 1988-2999, Guangzhou Zhi Yuan Electronics Co., Ltd.
  File name: Pxx_ATEInterface.h
  Author: zhoushiyou	Version:	v1.0   Date: 2015-9-14
  Description:   上位机 ATE操作接口
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

/*所有接口字符串采用多字节编码*/

/************************************************************************/
/* 配置参数类型定义
/* 主要应用于API:ConnectDevice
/************************************************************************/
typedef enum _pxx_ate_type{PXX_ATE_TYPE_WLAN,PXX_ATE_TYPE_COM,PXX_ATE_TYPE_USB}PXX_ATE_TYPE; //通讯类型
typedef struct _pxx_ate_wlan	//网络参数结构体
{
	char 		ip[16];//IP地址
	int			port;//设备端口号
}PXX_ATE_WLAN,*PPXX_ATE_WLAN;
typedef struct _pxx_ate_com		//串口参数结构体
{
	int			port;//串口号
	int			baudrate;//波特率
}PXX_ATE_COM,*PPXX_ATE_COM;
typedef struct _pxx_ate_usb		//USB参数结构体 
{/*暂时未使用*/
}PXX_ATE_USB,*PPXX_ATE_USB;

/************************************************************************/
/* 执行操作对应描述
/* 对应API为：RunDeviceOperator
/************************************************************************/
#define PXX_ATE_OP_RUN_ERASE								1//擦除
#define PXX_ATE_OP_RUN_CHECK								2//查空
#define PXX_ATE_OP_RUN_PROGRAM								3//编程
#define PXX_ATE_OP_RUN_VERIFY								4//校验
#define PXX_ATE_OP_RUN_READ									5//读取
#define PXX_ATE_OP_RUN_COMBINATION							6//组合
#define PXX_ATE_OP_RUN_AUTOBATCH							7//量产
#define PXX_ATE_OP_RUN_PROGRAM_C							8//编程配置
#define PXX_ATE_OP_RUN_VERIFY_C								9//校验配置
#define PXX_ATE_OP_RUN_ERASE_C								10//擦除配置
#define PXX_ATE_OP_RUN_CHECK_C								11//查空配置
#define PXX_ATE_OP_RUN_READ_C								12//读配置
#define PXX_ATE_OP_RUN_LOCK									13//加锁
#define PXX_ATE_OP_RUN_UNLOCK								14//解锁

/************************************************************************/
/* 通道状态值
/* ATE控制过程中，通过调用接口GetDeviceStatus获取每个通道的状态值
/************************************************************************/
#define SOFT_ATE_STATUS_ERR			0
#define SOFT_ATE_STATUS_OK			1

#define SOFT_ATE_STATUS_POWERON		0x100	//上电中
#define SOFT_ATE_STATUS_INIT		0x101	//初始化运行中
#define SOFT_ATE_STATUS_ERASE		0x102	//擦除中
#define SOFT_ATE_STATUS_CHECK		0x103	//查空中
#define SOFT_ATE_STATUS_PROGRAM		0x104	//编程中
#define SOFT_ATE_STATUS_VERIFY		0x105	//校验中
#define SOFT_ATE_STATUS_READ		0x106	//读取中
#define SOFT_ATE_STATUS_C_PROGRAM	0x107	//编程配置区
#define SOFT_ATE_STATUS_C_VERIFY	0x108	//校验配置区
#define SOFT_ATE_STATUS_C_ERASE		0x109	//擦除配置区
#define SOFT_ATE_STATUS_C_CHECK		0x10A	//查空配置区
#define SOFT_ATE_STATUS_C_READ		0x10B	//读取配置区
#define SOFT_ATE_STATUS_LOCK		0x10C	//加锁
#define SOFT_ATE_STATUS_UNLOCK		0x10D	//解锁

#define SOFT_ATE_STATUS_WAITCONTROL	0x200	//等待控制
#define SOFT_ATE_STATUS_OP_END		0x201	//操作结束

#define SOFT_ATE_ERR_TIMES_END		0x400	//量产次数用光
#define SOFT_ATE_ERR_BEG_BATCH		0x401	//量产开始失败
#define SOFT_ATE_ERR_POWER_ON		0x402	//上电过程失败
#define SOFT_ATE_ERR_INIT			0x403	//初始化失败（系统上电+读ID）
#define SOFT_ATE_ERR_OP_FILE		0x404	//烧写文件处理失败（下传数据错误）
#define SOFT_ATE_ERR_PROGRAM		0x405	//编程失败——详细看日志信息
#define SOFT_ATE_ERR_VERIFY			0x406	//校验失败——详细看日志信息
#define SOFT_ATE_ERR_ERASE			0x407	//擦除失败——详细看日志信息
#define SOFT_ATE_ERR_CHECK			0x408	//查空失败——详细看日志信息
#define SOFT_ATE_ERR_READ			0x409	//读取失败——详细看日志信息
#define SOFT_ATE_ERR_C_PROGRAM		0x40A	//编程配置失败
#define SOFT_ATE_ERR_C_VERIFY		0x40B	//校验配置失败
#define SOFT_ATE_ERR_C_ERASE		0x40C	//擦除配置失败
#define SOFT_ATE_ERR_C_CHECK		0x40D	//查空配置失败
#define SOFT_ATE_ERR_C_READ			0x40E	//读取配置失败
#define SOFT_ATE_ERR_LOCK			0x40F	//加锁失败
#define SOFT_ATE_ERR_UNLOCK			0x410	//解锁失败

#define SOFT_ATE_STATUS_UNUSE		0xFFFFFFFF//未使用



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	/*通用ATE功能*/
	/************************************************************************/
	/* Description:
		连接编程器设备
		ATE主控机在连接设备前必须调用该接口获取连接设备标识（API返回值）
		ConnectDevice：通用接口，通过传递不通的类型来执行不同的打开操作
		以网络连接威力，代码如：
		PXX_ATE_WLAN wlanInfo;
		strcpy_s(wlanInfo.ip,16,"192.168.9.140");
		wlanInfo.port = 8800;//恒定值
		m_deviceID = ConnectDevice(PXX_ATE_TYPE_WLAN,&wlanInfo);
		
		ConnectDeviceWlan直接通过网络方式连接设备
		ConnectDeviceUart直接通过串口方式连接设备
		ConnectDeviceUSB直接通过USB方式连接设备
	/* Param:
		ConnectDevice::type:通过type模式打开编程器设备
		ConnectDevice::pParam:模式对应的结构体指针

		ConnectDeviceWlan::ip:待连接设备的IP地址，如"192.168.9.140"
		ConnectDeviceWlan::port:待连接设备端口，这里填写恒定值8800

		ConnectDeviceUart::port:主控机连接到设备上的COM端口号，取值为0.1.2.3...
		ConnectDeviceUart::baudrate:为匹配设备波特率，这里设置恒定值115200

	/* Result:
		0:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		非0:连接设备标识，该标识符需要保存使用，其他API均需要传递该标识符进行操作，用来匹配正确的操作机器
	/************************************************************************/
	PXX_ATE_API int ConnectDevice(PXX_ATE_TYPE type,void * pParam);
	PXX_ATE_API int ConnectDeviceWlan(char ip[16],DWORD port);
	PXX_ATE_API int ConnectDeviceUart(int port,int baudrate);
	PXX_ATE_API int ConnectDeviceUSB();

	/************************************************************************/
	/* Description:
		断开与设备的连接，在主控机不需要再连接该设备的时调用，断开连接后的设备才能被再行控制。
		该系列函数与ConnectDevice系列函数成对调用
		如：软件关闭时调用，以断开ATE连接控制。
		UnConnectDevice为通用断开接口
		以以太网断开为例，代码如：
		UnConnectDevice(PXX_ATE_TYPE_WLAN,m_deviceID);

		UnConnectDeviceWlan断开网络连接的设备
		UnConnectDeviceUart断开串口连接的设备
		UnConnectDeviceUSB断开USB连接的设备
	/* Param:
		UnConnectDevice::type:断开连接的类型
		UnConnectDevice::deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		UnConnectDevice::bForce:是否强制断开，如果传递为true，则断开握手失败时也强制关闭主控端的连接，并反馈成功。

		UnConnectDeviceWlan::deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		UnConnectDeviceWlan::bForce:是否强制断开，如果传递为true，则断开握手失败时也强制关闭主控端的连接，并反馈成功。

		UnConnectDeviceUart::deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		UnConnectDeviceUart::bForce:是否强制断开，如果传递为true，则断开握手失败时也强制关闭主控端的连接，并反馈成功。

		UnConnectDeviceUSB::deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		UnConnectDeviceUSB::bForce:是否强制断开，如果传递为true，则断开握手失败时也强制关闭主控端的连接，并反馈成功。
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool UnConnectDevice(PXX_ATE_TYPE type,int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceWlan(int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceUart(int deviceID,bool bForce = true);
	PXX_ATE_API bool UnConnectDeviceUSB(int deviceID,bool bForce = true);

	/************************************************************************/
	/* Description:
		通过工程索引远程打开设备上的工程
		工程索引是指设备上SD卡工程目录下工程文件名的排序序号（当前已打开工程默认占用序号1）
		调用接口如：
		OpenDeviceProject(m_deviceID,m_proIdex);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		proIdex:设备上工程列表的索引,范围(1-N)
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool OpenDeviceProject(int deviceID,int proIdex);

	/************************************************************************/
	/* Description:
		执行操作——擦除、编程、校验、组合、量产等。
		可执行操作列表参照本文件中宏定义类型，选择需要执行的操作后调用该接口即可，例子代码如：
		RunDeviceOperator(m_deviceID,PXX_ATE_OP_RUN_ERASE);
		该操作仅为启动操作，主控机调用该操作后，需要执行ATE相关的机械动作，比如放置芯片、连接目标板的动作。
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		op:操作类型，见文件上述中的宏定义
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool RunDeviceOperator(int deviceID,int op);

	/************************************************************************/
	/* Description:
		开始执行单次操作
		该函数为配合RunDeviceOperator使用，在RunDeviceOperator之后执行完机械动作后，应该调用该接口启动真正的操作流程。
		该操作是分通道来处理的，意思是单个通道单独控制，也可以直接以掩码的形式同时启动多个，根据用户需求不同来执行不同的调用方式
		例子代码如：
		StartDeviceOperater(m_deviceID,1);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		channel:要启动的通道掩码
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool StartDeviceOperater(int deviceID,int channel);
	
	/************************************************************************/
	/* Description:
		获取设备所有通道的当前状态
		在成功调用连接设备的API后即可开启后台线程来调用该接口获取每个通道的状态值，状态值参照文件上述中得宏定义
		可根据反馈不同的状态值，在软件UI上进行显示，
		调用代码如：
		DWORD statusArray[16];//16个为单机最大通道个数
		GetDeviceStatus(pDlg->m_deviceID,statusArray);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		pbuffer:至少16个DWORD长度的缓冲区
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool GetDeviceStatus(int deviceID,DWORD* pBuffer);

	/************************************************************************/
	/* Description:
		获取操作的log信息，在操作结束时调用即可，避免增加系统负担。
		每次操作均需要调用该接口来获取日志信息，否则日志信息会被下一次操作覆盖掉。
		调用代码如：
		char pBuffer[4096];
		memset(pBuffer,0,4096);
		GetCurOPLogs(m_deviceID,1,pBuffer);
		日志信息的编码方式为：多字节编码
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		channel:一个通道一个通道获取，同步模式直接获取一号通道
		pLog:用来缓存日志，大小保证4096以上长度
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool GetCurOPLogs(int deviceID,DWORD channel,char * pLog);

	/************************************************************************/
	/* Description:
		停止操作执行
		该接口跟RunDeviceOperator成对应关系，用来终止操作的执行
		调用代码如：
		StopRunning(m_deviceID);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool StopRunning(int deviceID);

	/************************************************************************/
	/* Description:
		复位系统——Pxx系列机器即重启设备
		功能暂未支持，勿用
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool ResetSystem(int deviceID);
	
	/************************************************************************/
	/* Description:
		获取本接口文件提供的接口出错调用后的错误码
	/* Result:
		返回值为文件Pxx_LastErrCode.h中得错误码宏定义值
	/************************************************************************/
	PXX_ATE_API int Pxx_GetLastErr();

	/*以下接口为高级ATE功能*/
	/************************************************************************/
	/* Description:
		根据索引号获取设备中的工程名称
		索引号传递跟打开工程的索引号一致
		调用代码如：
		char pName[MAX_PATH] = {0};
		GetProjectName(m_deviceID,m_proIdex,pName,MAX_PATH);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		idex:工程索引
		pBuffer:用来存储工程名称信息，多字节编码方式
		dwSize:pBuffer大小
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool GetProjectName(int deviceID,int idex,char * pBuffer,int dwSize);

	/************************************************************************/
	/* Description:
		通过工程名称打开设备工程，该函数功能跟根据索引打开工程的API：OpenDeviceProject相同
		用户可以传入事先知道的工程名称也可以根据GetProjectName获取设备的工程名称去打开
		调用代码为：
		char pName[MAX_PATH] = {0};
		GetProjectName(m_deviceID,m_proIdex,pName,MAX_PATH);
		OpenDeviceProjectName(m_deviceID,pName);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		name:工程名称,为多字节编码方式的字符串
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool OpenDeviceProjectName(int deviceID,char * name);

	/************************************************************************/
	/* Description:
		删除设备上的工程
		支持两种删除模式：
		通过序号删除DelProjectIdex
		通过工程名称删除DelProjectName
		以序号删除为例，例子代码如：
		DelProjectIdex(m_deviceID,m_proIdex);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		DelProjectIdex::idex:要删除工程的索引
		DelProjectName::name:要删除工程的名称，采用多字节编码方式传递
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool DelProjectIdex(int deviceID,int idex);
	PXX_ATE_API bool DelProjectName(int deviceID,char * name);

	/************************************************************************/
	/* Description:
		发送本地工程文件夹到设备上（SD卡P500_Projects），为避免跟用户压缩算法不一致，这里只支持文件夹的发送
		只需传递本地的工程目录到接口中即可完成下发，需要保证设备中有足够的空间存储该工程，及设备中不存在对应的同名工程，如果有则先删除
		例子代码如：
		SendProjectForder(m_deviceID,_T("C:\\TestProject"));
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		path:要发送的本地路径，采用多字节编码
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool SendProjectForder(int deviceID,char * path);

	
	/************************************************************************/
	/* Description:
		获取编程器设备软件版本，特殊用户会关心软件版本和工程版本的匹配，如果发现版本不匹配则需要更新对应的版本
		例子代码如：
		char pName[MAX_PATH] = {0};
		GetSoftVersion(m_deviceID,pName,MAX_PATH);
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		pBuffer:用来缓存版本信息，返回的结果为多字节编码方式的字符串
		dwSize:缓存大小（>=MAX_PATH)
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool GetSoftVersion(int deviceID,char * pBuffer,int dwSize);

	/************************************************************************/
	/* Description:
		更新编程器中的软件
		用户可讲本地软件目录更新到设备中，更新时间比较久，大致持续3-5分钟左右，用户这里最好在线程中去更新
		该函数正确返回后用户可调用ConnectDevice系列函数执行连接操作，直到成功即为软件启动完成（超时时间设置为10分钟）
		例子代码如：
		UpdateSoft(m_deviceID,_T("C:\\P500"));
		DWORD beg = GetTickCount();
		while(GetTickCount()-beg<1000*60*10)
		{
			if(ConnectDevice(...))
				break;//成功
		}
	/* Param:
		deviceID:操作设备对应的ID标识，该值为ConnectDevice系列函数的返回值。
		path:要更新的本地软件路径
	/* Result:
		false:失败,失败后可以通过调用接口Pxx_GetLastErr()获取失败信息，失败具体描述参照Pxx_LastErrCode.h中定义
		true:成功
	/************************************************************************/
	PXX_ATE_API bool UpdateSoft(int deviceID,char * path);


#ifdef __cplusplus
}
#endif

#endif