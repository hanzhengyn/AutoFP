// BurningPort.h: interface for the CBurningPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_)
#define AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//\brief  检查要放入的socket，返回放入优先级最高的socket编号
int CheckChipForMoveInSocket(int nNozzle);
//预定烧录座
int BookSocket(int nNozzle);
//\brief  检查需要从socket中取出的芯片，返回取出优先级最高的socket编号
int CheckChipForMoveOutSocket(int nNozzle);
//\brief  检查socket中芯片能否取出，如果能取出，返回true
BOOL IsChipCanBeMoveOutFromSocket(int sock);
//\brief  检查socket是否为空，如果为空，返回true
BOOL CheckSocketIsEmpty(int sock);
//\brief  增大取料优先级
void  AddGetPreference();
//\brief  增大放置优先级
void AddPutPreference();
//\brief  获取tray盘中第presum个位置的坐标pos
BOOL GetChipPosInTray(int presum, Position &pos);
//\brief  获取补料盘第presum个位置的坐标pos
UINT GetChipPosInStockTray(int presum, Position &pos);
//\brief  获取废料盘第presum个位置的坐标pos
BOOL GetChipPosInScrap(int presum, Position &pos);
//\brief  获取编带进料坐标
Position  GetPosForTapeIn();
//\brief  获取编带出料坐标
Position  GetPosForTapeOut();
BOOL  GetPosForTape(const char *inOut, Position &pos);
//\brief  获取第sockInx个socket的坐标
Position  GetPosForSocket(int sockInx);
BOOL  GetPosForSocket(int sockInx, Position &pos);

UINT BeepThreadProc(LPVOID lParam);
UINT GetChip();
UINT PutChip();
UINT GetChip(int nNozzleNum);
UINT PutChip(int nNozzleNum);

//\brief  取芯片过程
//\param  nGetNumber [in] 取出的位置编号
//\param  nNozzleNumber [in] 吸嘴编号
//\param  nWorkMode [in] 工作模式
//\//return
UINT GetChipProc(int nGetNumber, int nNozzleNumber, int nWorkMode);

///brief  放芯片过程
//param  nPutNumber [in] 放入的位置编号,对于烧录座为座子编号，对于tray盘为tray盘中芯片序号，对于编带为编带出料位置
//param  nNozzleNumber [in] 吸嘴编号	0 1 2 3
//param  posOffset [in] 放片偏差
//param  nWorkMode [in] 工作模式
//return
UINT PutChipProc(int nPutNumber, int nNozzleNumber, Position2 &posOffset, int nWorkMode);

//\brief  芯片移动到下相机进行坐标检测
//\param  nNozzleNumber			[in] 吸嘴编号
//\param  posOffset				[out] 偏差值
//\param  lParam					[in] 父窗口指针
//\return TRUE	--	匹配成功
//\return FALSE	--	匹配失败
UINT CheckChipProc(int nNozzleNumber, Position2 &posOffset, LPVOID lParam);

//  旋转芯片线程
//param  lpParameter			[in] 保存偏差信息posOffset
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
//保存配置信息
DWORD WINAPI SaveConfigProc(
    LPVOID lpParameter   // thread data
);
//放满的座子开始烧录
void StartProgramProc(int nSocketNum, LPVOID lParam);

//检查吸嘴是否能取料
//取料要考虑两种情况：
//1.从进料取料：进料是否有料，烧录座是否有地方放
//2.从烧录座取料：烧录座是否有烧录好的料，出料处是否有地方放
//3.从补料盘取料：有补料过程，tray盘已空，出料未满（只针对tray盘进料）
BOOL CheckChipCanBeGet(int nWorkMode, int nNozzle);

//进料过程
UINT InputProc(LPVOID lParam);

//出料过程
UINT OutputProc(LPVOID lParam);

//判断烧录过程是否完成
//烧录完成的几种情况：
//1.无补料，无自动tray盘：	gm_nCurrentOutSize == gm_nSum
//2.有补料，无自动tray盘：	gm_nCurrentGoodSize == gm_nSum
//3.无补料，有自动tray盘：	gm_nCurrentOutSize == gm_nSum && tray盘进料没有感应到
//4.有补料，有自动tray盘：	gm_nCurrentGoodSize == gm_nSum && tray盘进料没有感应到
BOOL CheckBurningProcComplete(LPVOID lParam);

//检查tray盘到达信号
BOOL CheckAutoTrayArrived();

//烧录过程
UINT BurningProc(LPVOID lParam);

//错误信息提示
void ShowErrorMessage(const char *chErrorMessage, LPVOID lParam);
void ShowErrorMessage(const char *chErrorMessage);
void ShowErrorMessage(UINT nErrorCode, int axis, LPVOID lParam);
#endif // !defined(AFX_BURNINGPORT_H__C33D05FE_9F8F_4185_B3A2_D55908F4478D__INCLUDED_)
