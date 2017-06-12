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
//���ư忨�ͺ�
#define COMI		0
#define ADVANTECH	1
#define GOOGOL		2	

#define BOARD_TYPE COMI
//#define BOARD_TYPE ADVANTECH
//#define BOARD_TYPE GOOGOL

//�Ƿ�ʹ�÷ɴ�
#define FEEDER 0

//����ͺ�
#define CAMERA_DAHENG_MER 1
#define CAMERA_DAHENG_1351 0

//#define SINGLE_AXIS 0	//���Ῠ

#define SINGLE_AXIS 1	//���Ῠ
#define FOUR_NUZZLE 1	//������ �Ƿ�ʹ��sd404 IO��

#if BOARD_TYPE == GOOGOL
#define XAXIS 1		//x��
#define YAXIS 2		//y��
#define ZAXIS 3		//z��
#if SINGLE_AXIS//������
#define RAXIS_1 4		//r1��
#else
#define RAXIS_1 5		//r1��
#endif
#define RAXIS_2 6		//r2��
#define RAXIS_3 7		//r3��
#define RAXIS_4 8		//r4��
#else
#define XAXIS 0		//x��
#define YAXIS 1		//y��
#define ZAXIS 2		//z��
#if SINGLE_AXIS//������
#define RAXIS_1 3		//r1��
#else
#define RAXIS_1 4		//r1��
#endif

#define RAXIS_2 5		//r2��
#define RAXIS_3 6		//r3��
#define RAXIS_4 7		//r4��

#endif // BOARD_TYPE == GOOGOL

#define EXPOSITION 6000	//����ع�ʱ�䣨us��
#define XGUIDE 400		//����20mm��8000����1Ȧ��400����1mm
#define YGUIDE 500		//����16mm��8000����1Ȧ��400����1mm	
#define ZGUIDE 2000		//����5mm��8000����1Ȧ��1600����1mm
#define RGUIDE 12800	//��ת�ᣬϸ��12800����1Ȧ
//#if BOARD_TYPE == ADVANTECH
//#define ZGUIDE 320		//����5mm��1600����1Ȧ��320����1mm	Advantech�忨�ò���
//#else
//#define ZGUIDE 1600		//����5mm��8000����1Ȧ��1600����1mm    COMI�忨���ŷ�
//#endif

