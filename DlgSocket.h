#if !defined(AFX_DLGSOCKET_H__84446FC2_4A51_47A0_972B_707B6B5DF69C__INCLUDED_)
#define AFX_DLGSOCKET_H__84446FC2_4A51_47A0_972B_707B6B5DF69C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSocket dialog

class CDlgSocket : public CDialog
{
public:
	int m_nSocket;
	int m_nProg;
// Construction
public:
	CDlgSocket(CWnd* pParent = NULL);   // standard constructor
	BOOL SearchHeight();
// Dialog Data
	//{{AFX_DATA(CDlgSocket)
	enum { IDD = IDD_POSITION_SOCKET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSocket)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSocket)
	virtual BOOL OnInitDialog();
	afx_msg void OnSocposSoc1();
	afx_msg void OnSocposSoc2();
	afx_msg void OnSocposSoc3();
	afx_msg void OnSocposSoc4();
	afx_msg void OnSockposGoto();
	afx_msg void OnSockposSave();
	afx_msg void OnSockposStart();
	afx_msg void OnPaint();
	afx_msg void OnSocposNozzle();
	afx_msg void OnSocposSoc5();
	afx_msg void OnSocposSoc6();
	afx_msg void OnSocposSoc7();
	afx_msg void OnSocposSoc8();
	afx_msg void OnSocposSoc9();
	afx_msg void OnSocposSoc10();
	afx_msg void OnSocposSoc11();
	afx_msg void OnSocposSoc12();
	afx_msg void OnSocposSoc13();
	afx_msg void OnSocposSoc14();
	afx_msg void OnSockposSaveheight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCopyposToAll();
	afx_msg void OnBnClickedCheckCamreacheck();
	afx_msg void OnBnClickedSockposSavexy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSOCKET_H__84446FC2_4A51_47A0_972B_707B6B5DF69C__INCLUDED_)
