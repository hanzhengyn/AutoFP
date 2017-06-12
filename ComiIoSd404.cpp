// ComiIoSd404.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoFP.h"
#include "ComiIoSd404.h"
#include "afxdialogex.h"
#include "ComiDaqApi.h"


// CComiIoSd404 对话框

IMPLEMENT_DYNAMIC(CComiIoSd404, CDialogEx)

CComiIoSd404::CComiIoSd404(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SD404, pParent)
{

}

CComiIoSd404::~CComiIoSd404()
{
}

void CComiIoSd404::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMIDAQCTRL1, m_ComiDaq);
}

void CComiIoSd404::OpenIOBoard()
{
	m_ComiDaq.GnDeviceLoad();
	m_ComiDaq.SetDeviceId(COMI_SD404);
	m_ComiDaq.SetDevInstance(0);
	if (m_ComiDaq.GnGetErrorCode() < 0)
	{
		m_ComiDaq.GnShowLastError();
	}
	else
	{
		m_ComiDaq.SetDlogLevel(0);
		m_ComiDaq.SetDlogFile("c:\\S100\\DaqLog\\ComiDAQ.log");
		m_ComiDaq.SetEnableErrorMsgDlg(1);
		m_ComiDaq.DioSetUsage(cmDI_DO);
	}
}

BOOL CComiIoSd404::CheckDoorOpen()
{
	return m_ComiDaq.DiGetOne(0);
}

BOOL CComiIoSd404::CheckNegtiveMeter(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		return m_ComiDaq.DiGetOne(1);
		break;
	case 1:
		return m_ComiDaq.DiGetOne(20);
		break;
	case 2:
		return m_ComiDaq.DiGetOne(21);
		break;
	case 3:
		return m_ComiDaq.DiGetOne(22);
		break;
	default:
		break;
	}
	return FALSE;
}

BOOL CComiIoSd404::CheckTrayArrive()
{
	return m_ComiDaq.DiGetOne(4);
}

BOOL CComiIoSd404::CheckPositiveMeter()
{
	return m_ComiDaq.DiGetOne(5);
}

BOOL CComiIoSd404::CheckFeederOutError()
{
	return m_ComiDaq.DiGetOne(7);
}

BOOL CComiIoSd404::CheckModelCylinderPressed(INT nModelNum)
{
	switch (nModelNum)
	{
	case 0:
		return m_ComiDaq.DiGetOne(12);
		break;
	case 1:
		return m_ComiDaq.DiGetOne(13);
		break;
	case 2:
		return m_ComiDaq.DiGetOne(14);
		break;
	case 3:
		return m_ComiDaq.DiGetOne(15);
		break;
	case 4:
		return m_ComiDaq.DiGetOne(10);
		break;
	case 5:
		return m_ComiDaq.DiGetOne(11);
		break;
	case 6:
		return m_ComiDaq.DiGetOne(8);
		break;
	case 7:
		return m_ComiDaq.DiGetOne(9);
		break;
	default:
		break;
	}
	return FALSE;
}

BOOL CComiIoSd404::CheckNozzleUpStatus(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		return m_ComiDaq.DiGetOne(16);
		break;
	case 1:
		return m_ComiDaq.DiGetOne(17);
		break;
	case 2:
		return m_ComiDaq.DiGetOne(18);
		break;
	case 3:
		return m_ComiDaq.DiGetOne(19);
		break;
	default:
		break;
	}
	return FALSE;
}

VOID CComiIoSd404::TrayComplete()
{
	m_ComiDaq.DoPutOne(0, 1);
	Sleep(500);
	m_ComiDaq.DoPutOne(0, 0);
}

VOID CComiIoSd404::SetFeederOut()
{
	m_ComiDaq.DoPutOne(0, 1);
	Sleep(500);
	m_ComiDaq.DoPutOne(0, 0);
}

VOID CComiIoSd404::SetFeedIn()
{
	m_ComiDaq.DoPutOne(11, 1);
	Sleep(500);
	m_ComiDaq.DoPutOne(11, 0);
}

VOID CComiIoSd404::SetCylinderDown(INT nModelNum)
{
	switch (nModelNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(12, 1);
		break;
	case 1:
		m_ComiDaq.DoPutOne(5, 1);
		break;
	case 2:
		m_ComiDaq.DoPutOne(14, 1);
		break;
	case 3:
		m_ComiDaq.DoPutOne(13, 1);
		break;
	case 4:
		m_ComiDaq.DoPutOne(4, 1);
		break;
	case 5:
		m_ComiDaq.DoPutOne(7, 1);
		break;
	case 6:
		m_ComiDaq.DoPutOne(1, 1);
		break;
	case 7:
		m_ComiDaq.DoPutOne(8, 1);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetCylinderUp(INT nModelNum)
{
	switch (nModelNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(12, 0);
		break;
	case 1:
		m_ComiDaq.DoPutOne(5, 0);
		break;
	case 2:
		m_ComiDaq.DoPutOne(14, 0);
		break;
	case 3:
		m_ComiDaq.DoPutOne(13, 0);
		break;
	case 4:
		m_ComiDaq.DoPutOne(4, 0);
		break;
	case 5:
		m_ComiDaq.DoPutOne(7, 0);
		break;
	case 6:
		m_ComiDaq.DoPutOne(1, 0);
		break;
	case 7:
		m_ComiDaq.DoPutOne(8, 0);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetNozzleVaccumOn(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(2, 1);
		break;
	case 1:
		m_ComiDaq.DoPutOne(16, 1);
		break;
	case 2:
		m_ComiDaq.DoPutOne(20, 1);
		break;
	case 3:
		m_ComiDaq.DoPutOne(24, 1);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetNozzleVaccumOff(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(2, 0);
		break;
	case 1:
		m_ComiDaq.DoPutOne(16, 0);
		break;
	case 2:
		m_ComiDaq.DoPutOne(20, 0);
		break;
	case 3:
		m_ComiDaq.DoPutOne(24, 0);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetNozzleBlowOn(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(3, 1);
		break;
	case 1:
		m_ComiDaq.DoPutOne(17, 1);
		break;
	case 2:
		m_ComiDaq.DoPutOne(21, 1);
		break;
	case 3:
		m_ComiDaq.DoPutOne(25, 1);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetNozzleBlowOff(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(3, 0);
		break;
	case 1:
		m_ComiDaq.DoPutOne(17, 0);
		break;
	case 2:
		m_ComiDaq.DoPutOne(21, 0);
		break;
	case 3:
		m_ComiDaq.DoPutOne(25, 0);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetRedLightOn()
{
	m_ComiDaq.DoPutOne(9, 1);
}

VOID CComiIoSd404::SetRedLightOff()
{
	m_ComiDaq.DoPutOne(9, 0);
}

VOID CComiIoSd404::SetBeepOn()
{
	m_ComiDaq.DoPutOne(9, 1);
}

VOID CComiIoSd404::SetBeepOff()
{
	m_ComiDaq.DoPutOne(9, 0);
}

VOID CComiIoSd404::SetGreenLightOn()
{
	m_ComiDaq.DoPutOne(10, 1);
}

VOID CComiIoSd404::SetGreenLightOff()
{
	m_ComiDaq.DoPutOne(10, 0);
}

VOID CComiIoSd404::SetUpCameraLightOn()
{
	m_ComiDaq.DoPutOne(6, 1);
}

VOID CComiIoSd404::SetUpCameraLightOff()
{
	m_ComiDaq.DoPutOne(6, 0);
}

VOID CComiIoSd404::SetDownCameraLightOn()
{
	m_ComiDaq.DoPutOne(15, 1);
}

VOID CComiIoSd404::SetDownCameraLightOff()
{
	m_ComiDaq.DoPutOne(15, 0);
}

VOID CComiIoSd404::SetNozzleDownOn(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(18, 1);
		break;
	case 1:
		m_ComiDaq.DoPutOne(19, 1);
		break;
	case 2:
		m_ComiDaq.DoPutOne(22, 1);
		break;
	case 3:
		m_ComiDaq.DoPutOne(23, 1);
		break;
	default:
		break;
	}
}

VOID CComiIoSd404::SetNozzleDownOff(INT nNozzleNum)
{
	switch (nNozzleNum)
	{
	case 0:
		m_ComiDaq.DoPutOne(18, 0);
		break;
	case 1:
		m_ComiDaq.DoPutOne(19, 0);
		break;
	case 2:
		m_ComiDaq.DoPutOne(22, 0);
		break;
	case 3:
		m_ComiDaq.DoPutOne(23, 0);
		break;
	default:
		break;
	}
}


BEGIN_MESSAGE_MAP(CComiIoSd404, CDialogEx)
END_MESSAGE_MAP()


// CComiIoSd404 消息处理程序
