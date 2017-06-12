#if !defined(AFX_DLGTRAY_H__2AC18214_D837_41B9_B8B5_6C18778F0D23__INCLUDED_)
#define AFX_DLGTRAY_H__2AC18214_D837_41B9_B8B5_6C18778F0D23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTray.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTray dialog

class CDlgTray : public CDialog
{
private:
	int m_nIndex;
	bool m_bSave1;
	bool m_bSave2;
	bool m_bSave3;
// Construction
public:
	CDlgTray(CWnd* pParent = NULL);   // standard constructor
	BOOL SearchHeight();
	void EnableSave(int index,BOOL bEnabled);
// Dialog Data
	//{{AFX_DATA(CDlgTray)
	enum { IDD = IDD_POSITION_TRAY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTray)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTray)
	afx_msg void OnTrayCcd1();
	afx_msg void OnTraySave1();
	afx_msg void OnTrayCcd2();
	afx_msg void OnTraySave2();
	afx_msg void OnTrayCcd3();
	afx_msg void OnTraySave3();
	afx_msg void OnTraySearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnTrayPitch();
	afx_msg void OnTrayPitch2();
	afx_msg void OnPaint();

	afx_msg void OnBnContinueTest();
	afx_msg void OnBtnAutoset4();
	afx_msg void OnBtnAutoset();
	afx_msg void OnBtnTrayreceived();
	afx_msg void OnBtnFpend();
	afx_msg void OnBtnAutotraystop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFpend2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRAY_H__2AC18214_D837_41B9_B8B5_6C18778F0D23__INCLUDED_)
