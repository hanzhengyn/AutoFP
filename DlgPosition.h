#if !defined(AFX_DLGPOSITION_H__D85D3BA7_94BE_4D22_AE47_5BE318843547__INCLUDED_)
#define AFX_DLGPOSITION_H__D85D3BA7_94BE_4D22_AE47_5BE318843547__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPosition.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPosition dialog
#include ".\Inc\HVDAILT.h"
#include ".\Inc\HVDef.h"
#include ".\Inc\HVUtil.h"
#include ".\Inc\Raw2Rgb.h"

#include "DlgReject.h"
#include "DlgTray.h"
#include "DlgSocket.h"
#include "DlgTapeIn.h"
#include "DlgSupply.h"
#include "DlgModelCheck.h"
#include "DlgUnderCamera.h"
#include "MotionAct.h"
#include "BtnST.h"
#include "roundbuttonstyle.h"
#include "roundbutton2.h"

#include "GxIAPI.h"
#include "DxImageProc.h"

#include "HalconCpp.h"	
#include "afxwin.h"
using namespace HalconCpp;



///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
	if(emStatus != GX_STATUS_SUCCESS) \
								{\
								ShowErrorString(emStatus); \
								return;\
								} 
///<  判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
								   return emStatus;\
                                   };
#define WM_SNAP_CHANGE		(WM_USER + 200)
class CDlgPosition : public CDialog
{
public:
	HTuple  m_hv_AcqHandle_Up;
	HObject m_ho_Image;
	HTuple m_hv_Width, m_hv_Height;
	HObject m_ho_ROI_0, m_ho_ImageReduced;
	HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours;
	HTuple m_hv_ModelID;
	HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
	HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;
	void OpenFramegrab();
	void dev_display_shape_matching_results(HTuple hv_ModelID, HTuple hv_Color, HTuple hv_Row,
		HTuple hv_Column, HTuple hv_Angle, HTuple hv_ScaleR, HTuple hv_ScaleC, HTuple hv_Model);
public:
	HTuple m_htRowCheck, m_htColumnCheck, m_htAngleCheck, m_htScore, m_htMinDistance;
	HTuple m_htRowModel, m_htColumnModel, m_htAngleModel;
public:

	HImage m_Image, m_ImageReduced, m_ShapeModelImage;
	HRegion m_ShapeModelContours;

	HWindow m_Window;
	HFramegrabber m_Acquisition;
	HShapeModel m_ShapeModel;

	Hlong m_WindowRow, m_WindowColumn, m_WindowWidth, m_WindowHeight;
	Hlong m_Width, m_Height, m_Area;

	double m_CenterRow, m_CenterColumn;
	double m_Rect1Row, m_Rect1Col, m_Rect2Row, m_Rect2Col;
	double m_RectPhi, m_RectLength, m_RectWidth;

	
public:
	void InitModelCamera();
	void CreateModel();
	Position2 CheckModel();
public:
	BOOL m_bCCDReady;
	bool m_bCheckPoint;
	int m_nXLength;
	int m_nYLength;
	int m_nWidth;
	int m_nHeight;
	int m_XStart;
	int m_YStart;

	bool m_bRect;
	bool m_bSave1;
	bool m_bSave2;
private:
	CBrush m_brush;
	GX_DEV_HANDLE m_hDevice;
	GX_OPEN_PARAM  m_stOpenParam;
	uint32_t       m_nDeviceNum;
	BOOL m_bOpenCCD;		
	BOOL m_bStartCCD;		
	BITMAPINFO *m_pBmpInfo;
	BYTE *m_pRawBuffer;
//	BYTE *m_pImageBuffer;
    char m_chBmpBuf[2048];
	//颜色查找表
	BYTE m_pLutR[256] ;
	BYTE m_pLutG[256] ;
	BYTE m_pLutB[256] ;
	
//	static int CALLBACK SnapThreadCallback(HV_SNAP_INFO *pInfo); 
	/// 回调函数
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);	
	/// 显示图像
	void DrawImage(BYTE *pImageBuf, int nWidth, int nHeight, BITMAPINFO *pBmpInfo);
	void ShowErrorString(GX_STATUS emErrorStatus);
	void DrawImg();
	//移动至基准点函数
	void MoveToBasePoint(const char *ch);
	//计算偏差值函数
	void GetOffset(const char *offset, const char *chFirstPoint, const char *chSecondPoint = "BasePoint");
public:
	HHV	m_hhv;
	void InitCamera();
	void InitCCD();
	void SetExposureTime(int nWindWidth, long lTintUpper, long lTintLower);
	void OpenSnapEx();
	void StartSnapEx();
	void StopSnapEx();
	void CloseSnapEx();
	void CloseCamera();
	static int CALLBACK SnapThreadCallback(HV_SNAP_INFO *pInfo);
	LRESULT OnSnapChange(WPARAM wParam, LPARAM lParam);
	void DrawCross(CDC *pDC, int xLength, int yLength, POINT center);
	void DrawRectangle(CDC *pDC,int xlength,int yLength,POINT center);
private:
	CDlgReject m_dlgReject;
	CDlgTray   m_dlgTray;
	CDlgSocket m_dlgSocket;
	CDlgTapeIn m_dlgTapeIn;
	CDlgSupply m_dlgSupply;
	CDlgModelCheck m_dlgModelCheck;
	CDlgUnderCamera m_dlgUnderCamera;
//   CDlgTapeOut m_dlgTapeOut;
	CRoundButtonStyle m_tButtonStyle;
private://control information
	LONG m_nXRange;
	LONG m_nYRange;
	LONG m_nZRange;
	BOOL m_bTimer;
	BOOL m_bTimerX;
	BOOL m_bTimerY;
	BOOL m_bTimerZ;
	// Construction
public:
	CDlgPosition(CWnd* pParent = NULL);   // standard constructor
	void ShowBasePoint(BOOL bShow);
// Dialog Data
	//{{AFX_DATA(CDlgPosition)
	enum { IDD = IDD_POSITION };
	CRoundButton2	m_btnSubY;
	CRoundButton2	m_btnAddY;
	CRoundButton2	m_btnAddX;
	CRoundButton2	m_btnSubX;
	CSliderCtrl	m_ySlider;
	CSliderCtrl	m_xSlider;
	CButtonST	m_btnZdown;
	CButtonST	m_btnZDown;
	CButtonST	m_btnUp;
	CButtonST	m_btnDown;
	CButtonST	m_btnRight;
	CButtonST	m_btnLeft;
	// 逆时针按钮
	CButtonST m_btnCounter;
	// 顺时针按钮
	CButtonST m_btnClock;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPosition)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPosition)
	virtual BOOL OnInitDialog();
	afx_msg void OnPosTray();
	afx_msg void OnPosBase();
	afx_msg void OnPosNg();
	afx_msg void OnPosSocket();
	afx_msg void OnPosTape();
	afx_msg void OnPosradioXy1();
	afx_msg void OnPosradioXy2();
	afx_msg void OnPosradioXy3();
	afx_msg void OnPosradioXy4();
	afx_msg void OnPosradioXy5();
	afx_msg void OnPosradioXy6();
	afx_msg void OnPosbtnXleft();
	afx_msg void OnPosbtnXright();
	afx_msg void OnPosbtnYtop();
	afx_msg void OnPosbtnYbottom();
	afx_msg void OnHome();
	afx_msg void OnPosradioZ1();
	afx_msg void OnPosradioZ2();
	afx_msg void OnPosradioZ3();
	afx_msg void OnPosradioZ4();
	afx_msg void OnPosbtnZup();
	afx_msg void OnPosbtnZdown();
	afx_msg void OnGotoCheckPoint();
	afx_msg void OnChkpointSaveccd();
	afx_msg void OnChkpointNozzle();
	afx_msg void OnChkpointGetoffset();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCustomdrawPosSliderx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawPosSlidery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPosXadd();
	afx_msg void OnPosXsub();
	afx_msg void OnPosYsub();
	afx_msg void OnPosYadd();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg void OnPosSupply();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	CString m_strCameraNumber;
	afx_msg void OnBnClickedPosModel();
	afx_msg void OnBnClickedBtnNextCamera();
	afx_msg void OnBnClickedBtnSwitchCamera();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnGoHome();
	afx_msg void OnBnClickedChkpointNozzle2();
	afx_msg void OnBnClickedChkpointGetoffset2();
	afx_msg void OnBnClickedChkpointNozzle3();
	afx_msg void OnBnClickedChkpointGetoffset3();
	afx_msg void OnBnClickedChkpointNozzle4();
	afx_msg void OnBnClickedChkpointGetoffset4();
	afx_msg void OnBnClickedBtnNozzle1Updown();
	afx_msg void OnBnClickedBtnNozzle2Updown();
	afx_msg void OnBnClickedBtnNozzle3Updown();
	afx_msg void OnBnClickedBtnNozzle4Updown();
	afx_msg void OnBnClickedCheckVacumm1();
	afx_msg void OnBnClickedCheckVacumm2();
	afx_msg void OnBnClickedCheckVacumm3();
	afx_msg void OnBnClickedCheckVacumm4();
	afx_msg void OnBnClickedCheckBlow1();
	afx_msg void OnBnClickedCheckBlow2();
	afx_msg void OnBnClickedCheckBlow3();
	afx_msg void OnBnClickedCheckBlow4();
	afx_msg void OnBnClickedCheckCylinder0();
	afx_msg void OnBnClickedCheckCylinder1();
	afx_msg void OnBnClickedCheckCylinder2();
	afx_msg void OnBnClickedCheckCylinder3();
	afx_msg void OnBnClickedCheckCylinder4();
	afx_msg void OnBnClickedCheckCylinder5();
	afx_msg void OnBnClickedCheckCylinder6();
	afx_msg void OnBnClickedCheckCylinder7();
	afx_msg void OnBnClickedCheckCylinder8();
	afx_msg void OnBnClickedCheckCylinder9();
	afx_msg void OnBnClickedCheckUpLight();
	afx_msg void OnBnClickedCheckDownLight();
	afx_msg void OnBnClickedChkpointGetoffset();
	int m_radioRotateAngel;
	afx_msg void OnBnClickedBtnCounterclockwise();
	afx_msg void OnBnClickedBtnRotateClockwise();
	// 吸嘴编号
	int m_radioNozzleNum;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioRotateAngle1();
	afx_msg void OnBnClickedRadioRotateAngle91();
	afx_msg void OnBnClickedRadioRotateAnglen91();
	afx_msg void OnBnClickedRadioRotateAngle181();
	afx_msg void OnBnClickedBtnRotateTest();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonSearch2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOSITION_H__D85D3BA7_94BE_4D22_AE47_5BE318843547__INCLUDED_)
