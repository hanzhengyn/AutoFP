#pragma once
#include "HalconCpp.h"	
using namespace HalconCpp;

// CDlgModelCheck 对话框

class CDlgModelCheck : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelCheck)

public:
	CDlgModelCheck(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModelCheck();

// 对话框数据
	enum { IDD = IDD_POSITION_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateModel();
	afx_msg void OnBnClickedBtnCheckModel2();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnMoveToModel();
	afx_msg void OnEnChangeEditCameraNumber();
	afx_msg void OnBnClickedBtnCheckCoord();
public:
	HWindow m_Window;
	HTuple  m_hv_AcqHandle_Up;
	HObject m_ho_Image;
	HTuple m_hv_Width, m_hv_Height;
	HObject m_ho_ROI_MODEL_DOMAIN, m_ho_ImageReduced;
	HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours;
	HTuple m_hv_ModelID;
	HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
	HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;
public:
	void OpenFramegrab();
	void CreateModel();
	Position2 CheckModel();
	double m_nOffsetX;
	double m_nOffsetY;
	double m_nOffsetAngle;
	static UINT CheckModelThread(LPVOID lParam);
	CString m_strCameraNum;
	virtual BOOL OnInitDialog();
	BOOL m_bCameraOpenUp;
	CString m_strCameraVersion;
	afx_msg void OnEnChangeEditCameraVersion();
	afx_msg void OnPaint();
	CString m_strSocketModel;
};
