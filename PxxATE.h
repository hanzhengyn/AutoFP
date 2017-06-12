#pragma once
#include "Pxx_LastErrCode.h"
#include "Pxx_ATEInterface.h"
class CPxxATE
{
public:
	CPxxATE();
	~CPxxATE();
public:
	//操作设备的方法
	BOOL ConnectWithWlan();//通过网络链接
	BOOL ConnectWithUlart();//通过串口连接
	BOOL ConnectWithUSB();//通过USB连接

	BOOL OpenProjectWithIdex();//通过序号打开工程
	BOOL OpenProjectWithName();//通过工程名打开工程
	BOOL StartProgramm(int channel);//开始烧录
	BOOL OperateDevice(int op);//对烧录器执行操作
	static DWORD WINAPI GetStatusThread(LPVOID lpvoid);//获取烧录状态

public:
	bool m_bGetStatus;//状态检测标识符
	int m_deviceID;//设备ID
	CString m_strIP;//设备IP
	DWORD m_dwPort;//设备端口
	DWORD m_dwCom;//设备串口
	DWORD m_dwBaudrate;//设备波特率
	int m_nProjectIndex;//工程序号
	CString m_strProjectName;//工程名
};

