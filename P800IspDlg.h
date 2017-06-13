#if !defined(AFX_P800ISPDLG_H__1C1B2603_7084_405F_81CE_C3CF49FD4F77__INCLUDED_)
#define AFX_P800ISPDLG_H__1C1B2603_7084_405F_81CE_C3CF49FD4F77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// P800IspDlg.h : header file
//
#include "Pxx_ATEInterface.h"

/////////////////////////////////////////////////////////////////////////////
// P800IspDlg dialog

class P800IspDlg : public CDialog
{
    // Construction
public:
    P800IspDlg(CWnd* pParent = NULL);   // standard constructor
    static DWORD WINAPI GetStatusThread(LPVOID lpvoid);
    void Stop();
    void UnConnect();
    void Reset();
    // Dialog Data
        //{{AFX_DATA(P800IspDlg)
    enum { IDD = IDD_P800ISP_DIALOG };
    CString	m_strIP;
    CString	m_strProjectName;
    DWORD	m_dwBaudrate;
    DWORD	m_dwCOM;
    DWORD	m_dwPort;
    DWORD	m_proIdex;
    DWORD   m_deviceID;
    BOOL	m_bGetStatus;
    CEdit	m_status[16];

    //	CEdit   m_status[4];


        //}}AFX_DATA


    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(P800IspDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(P800IspDlg)
    afx_msg void OnBtnWlanConnect();
    afx_msg void OnBtnOpenProject();
    afx_msg void OnBtnStart1();
    afx_msg void OnBtnStart2();
    afx_msg void OnBtnStart3();
    afx_msg void OnBtnStart4();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    virtual void OnCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P800ISPDLG_H__1C1B2603_7084_405F_81CE_C3CF49FD4F77__INCLUDED_)
