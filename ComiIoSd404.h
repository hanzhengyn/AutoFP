#pragma once
#include "comidaqctrl1.h"


// CComiIoSd404 对话框

class CComiIoSd404 : public CDialogEx
{
	DECLARE_DYNAMIC(CComiIoSd404)

public:
	CComiIoSd404(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CComiIoSd404();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SD404 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComidaqctrl1 m_ComiDaq;
public:
	void OpenIOBoard();

	 BOOL CheckDoorOpen();
	 BOOL CheckNegtiveMeter(INT nNozzleNum);//负压表
	 BOOL CheckTrayArrive();
	 BOOL CheckPositiveMeter();//正压表
	 BOOL CheckFeederOutError();
	 BOOL CheckModelCylinderPressed(INT nModelNum);
	 BOOL CheckNozzleUpStatus(INT nNozzleNum);

	 VOID TrayComplete();
	 VOID SetFeederOut();
	 VOID SetFeedIn();
	 VOID SetCylinderDown(INT nModelNum);
	 VOID SetCylinderUp(INT nModelNum);
	 VOID SetNozzleVaccumOn(INT nNozzleNum);
	 VOID SetNozzleVaccumOff(INT nNozzleNum);
	 VOID SetNozzleBlowOn(INT nNozzleNum);
	 VOID SetNozzleBlowOff(INT nNozzleNum);
	 VOID SetRedLightOn();
	 VOID SetRedLightOff();
	 VOID SetBeepOn();
	 VOID SetBeepOff();
	 VOID SetGreenLightOn();
	 VOID SetGreenLightOff();
	 VOID SetUpCameraLightOn();
	 VOID SetUpCameraLightOff();
	 VOID SetDownCameraLightOn();
	 VOID SetDownCameraLightOff();
	 VOID SetNozzleDownOn(INT nNozzleNum);
	 VOID SetNozzleDownOff(INT nNozzleNum);

};
