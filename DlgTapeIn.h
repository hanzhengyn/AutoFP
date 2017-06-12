#if !defined(AFX_DLGTAPEIN_H__C1C0E9DD_FE54_41B1_B51A_C8DFE91923C5__INCLUDED_)
#define AFX_DLGTAPEIN_H__C1C0E9DD_FE54_41B1_B51A_C8DFE91923C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTapeIn.h : header file
//
#include "HalconCpp.h"	
using namespace HalconCpp;
/////////////////////////////////////////////////////////////////////////////
// CDlgTapeIn dialog

class CDlgTapeIn : public CDialog
{
// Construction
public:
	CDlgTapeIn(CWnd* pParent = NULL);   // standard constructor
	~CDlgTapeIn();
	BOOL SearchHeight();
	BOOL m_bTapeIn;
// Dialog Data
	//{{AFX_DATA(CDlgTapeIn)
	enum { IDD = IDD_POSITION_TAPEIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTapeIn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTapeIn)
	virtual BOOL OnInitDialog();
	afx_msg void OnTapeinCcd();
	afx_msg void OnTapeinSave();
	afx_msg void OnTapeinStep2();
	afx_msg void OnTapeinSearch();
	afx_msg void OnTapeinStep();
	afx_msg void OnTapeinStep3();
	afx_msg void OnTapeinStep4();
	afx_msg void OnTapeinStep5();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnPaint();
	afx_msg void OnTapeinStepf2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnTapeinSearch2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCreateModel();
public:
	HWindow m_Window;
	HTuple  m_hv_AcqHandle_Up;
	HObject m_ho_Image, m_ho_ROI_Search_Domain, m_ho_ROI_Model_Domain;
	HTuple m_hv_Width, m_hv_Height;
	HObject m_ho_ROI_0, m_ho_ImageReduced;
	HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours, m_ho_ImageReducedSearch;
	HTuple m_hv_ModelID;
	HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
	HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;
public:
	void OpenFramegrab();
	void InitModelCamera();
	void CreateModel();
	Position2 CheckModel();
	double m_nOffsetX;
	double m_nOffsetY;
	double m_nOffsetAngle;
	static UINT CheckModelThread(LPVOID lParam);
	CString m_strCameraNum;
	CString m_strCameraVersion;
	BOOL m_bCameraOpenUp;
	afx_msg void OnBnClickedBtnSearchModel();
	afx_msg void OnBnClickedTapeinSavexy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTAPEIN_H__C1C0E9DD_FE54_41B1_B51A_C8DFE91923C5__INCLUDED_)
