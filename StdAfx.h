// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__06239838_0293_4EC2_A23B_B23193D64F7B__INCLUDED_)
#define AFX_STDAFX_H__06239838_0293_4EC2_A23B_B23193D64F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxsock.h>

#include "sendef.h"
#include "global.h"
#include "inifile.h"
#include <afxcontrolbars.h>
//#include "General.h"
#pragma comment(lib, "ws2_32")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__06239838_0293_4EC2_A23B_B23193D64F7B__INCLUDED_)

//--------------------------------------------------------------
//控制板卡型号
#define COMI		0
#define ADVANTECH	1
#define GOOGOL		2	

#define BOARD_TYPE COMI
//#define BOARD_TYPE ADVANTECH
//#define BOARD_TYPE GOOGOL

//是否使用飞达
#define FEEDER 0

//相机型号
#define CAMERA_DAHENG_MER 1
#define CAMERA_DAHENG_1351 0

//#define SINGLE_AXIS 0	//八轴卡

#define SINGLE_AXIS 1	//四轴卡
#define FOUR_NUZZLE 1	//四吸嘴 是否使用sd404 IO卡

#if BOARD_TYPE == GOOGOL
#define XAXIS 1		//x轴
#define YAXIS 2		//y轴
#define ZAXIS 3		//z轴
#if SINGLE_AXIS//单吸嘴
#define RAXIS_1 4		//r1轴
#else
#define RAXIS_1 5		//r1轴
#endif
#define RAXIS_2 6		//r2轴
#define RAXIS_3 7		//r3轴
#define RAXIS_4 8		//r4轴
#else
#define XAXIS 0		//x轴
#define YAXIS 1		//y轴
#define ZAXIS 2		//z轴
#if SINGLE_AXIS//单吸嘴
#define RAXIS_1 3		//r1轴
#else
#define RAXIS_1 4		//r1轴
#endif

#define RAXIS_2 5		//r2轴
#define RAXIS_3 6		//r3轴
#define RAXIS_4 7		//r4轴

#endif // BOARD_TYPE == GOOGOL

#define EXPOSITION 6000	//相机曝光时间（us）
#define XGUIDE 400		//导程20mm，8000脉冲1圈，400脉冲1mm
#define YGUIDE 500		//导程16mm，8000脉冲1圈，400脉冲1mm	
#define ZGUIDE 2000		//导程5mm，8000脉冲1圈，1600脉冲1mm
#define RGUIDE 12800	//旋转轴，细分12800脉冲1圈
//#if BOARD_TYPE == ADVANTECH
//#define ZGUIDE 320		//导程5mm，1600脉冲1圈，320脉冲1mm	Advantech板卡用步进
//#else
//#define ZGUIDE 1600		//导程5mm，8000脉冲1圈，1600脉冲1mm    COMI板卡用伺服
//#endif

