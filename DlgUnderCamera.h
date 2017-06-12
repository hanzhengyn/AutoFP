#pragma once
#include "HalconCpp.h"	
#include "afxwin.h"
using namespace HalconCpp;

// CDlgUnderCamera 对话框

class CDlgUnderCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUnderCamera)

public:
	CDlgUnderCamera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUnderCamera();

// 对话框数据
	enum { IDD = IDD_POSITION_UNDER_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dAngle;
	HWindow m_Window;
	HTuple  m_hv_AcqHandle_Down;
	HObject m_ho_Image, m_ho_ROI_Search_Domain, m_ho_ROI_Model_Domain, m_ho_ImageReducedSearch;
	HObject m_ho_ImageMean, m_ho_Region, m_ho_ConnectionRegions, m_ho_RegionUnion, m_ho_RegionDilation;

	HTuple m_hv_Width, m_hv_Height;
	HObject m_ho_ROI_0, m_ho_ImageReduced;
	HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours;
	HTuple m_hv_ModelID;
	HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
	HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;
	double m_nOffsetX;
	double m_nOffsetY;
	double m_nOffsetAngle;
	CString m_strCameraNum;
	CString m_strCameraVersion;
	BOOL m_bCameraOpen;
public:
	void OpenFramegrab();
	void InitModelCamera();
	void CreateModel();
	Position2 CheckModel();
	afx_msg void OnBnClickedBtnCreateModel();
	afx_msg void OnBnClickedBtnCheckModel();
	afx_msg void OnBnClickedBtnNozzle2camera();
	afx_msg void OnBnClickedBtnSaveNuzzle();
	afx_msg void OnEnChangeEditCameraVersion();
	afx_msg void OnEnChangeEditCameraNumber();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnGetChipToCamera();
	afx_msg void OnBnClickedBtnRotation();
	afx_msg void OnBnClickedBtnMatchAdjust();
	afx_msg void OnBnClickedBtnPutChipToTape();
	afx_msg void OnBnClickedBtnRotation3();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	// 芯片模型名称
	CString m_strChipModel;
	CEdit m_editCameraVersion;
	CEdit m_editCameraNum;
};
