#if !defined(AFX_DLGSETTING_H__670BBF5E_1BB6_49EE_AA2E_927A3C95D65D__INCLUDED_)
#define AFX_DLGSETTING_H__670BBF5E_1BB6_49EE_AA2E_927A3C95D65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetting dialog
#include "P800IspDlg.h"
#include "afxwin.h"
class CDlgSetting : public CDialog
{
// Construction
public:
	CDlgSetting(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlgSetting)
	enum { IDD = IDD_DIALOG_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetting)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingXy1();
	afx_msg void OnSettingXy2();
	afx_msg void OnSettingXy3();
	afx_msg void OnSettingZ1();
	afx_msg void OnSettingZ2();
	afx_msg void OnSettingZ3();
	afx_msg void OnMode1();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	afx_msg void OnCheckStock();
	afx_msg void OnSettingZ4();
	afx_msg void OnSettingZ5();
	afx_msg void OnSettingXy4();
	afx_msg void OnSettingXy5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckCheck();
	afx_msg void OnBnClickedCheckDownCamera();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAutoTray();
	afx_msg void OnBnClickedCheckNozzle0();
	afx_msg void OnBnClickedCheckNozzle1();
	afx_msg void OnBnClickedCheckNozzle2();
	afx_msg void OnBnClickedCheckNozzle3();
	afx_msg void OnBnClickedRadioRotateAngle0();
	afx_msg void OnBnClickedRadioRotateAngle90();
	afx_msg void OnBnClickedRadioRotateAnglen90();
	afx_msg void OnBnClickedRadioRotateAngle180();
	afx_msg void OnBnClickedCheckDownCameraOut();
	afx_msg void OnBnClickedRadioRotateAngle7();
	afx_msg void OnBnClickedRadioRotateAngle92();
	afx_msg void OnBnClickedRadioRotateAnglen92();
	afx_msg void OnBnClickedRadioRotateAngle182();
	CComboBox m_combSocketModel;
	CComboBox m_combChipModel;
	afx_msg void OnBnClickedCheckConvertChip();
	afx_msg void OnBnClickedCheckDoor();
	afx_msg void OnBnClickedCheckTape();
	afx_msg void OnBnClickedCheckTapeout();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTING_H__670BBF5E_1BB6_49EE_AA2E_927A3C95D65D__INCLUDED_)
