#if !defined(AFX_DLGREJECT_H__80497257_A439_4302_B064_83D85F73F97A__INCLUDED_)
#define AFX_DLGREJECT_H__80497257_A439_4302_B064_83D85F73F97A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgReject.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgReject dialog

class CDlgReject : public CDialog
{
    // Construction
public:
    CDlgReject(CWnd* pParent = NULL);   // standard constructor
    BOOL SearchHeight();
    // Dialog Data
        //{{AFX_DATA(CDlgReject)
    enum { IDD = IDD_POSITION_NGBIN };
    // NOTE: the ClassWizard will add data members here
//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDlgReject)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDlgReject)
    afx_msg void OnNgbinCcdto1();
    afx_msg void OnNgbinSave1();
    afx_msg void OnNgbinCcdto2();
    afx_msg void OnNgbinSave2();
    afx_msg void OnNgbinCcdto3();
    afx_msg void OnNgbinSave3();
    virtual BOOL OnInitDialog();
    afx_msg void OnNgbinSave5();
    afx_msg void OnNgbinCcdto4();
    afx_msg void OnNgbinCcdto5();
    afx_msg void OnNgbinCcdto6();
    afx_msg void OnPaint();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedNgbinSave7();
    afx_msg void OnBnClickedNgbinSave6();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREJECT_H__80497257_A439_4302_B064_83D85F73F97A__INCLUDED_)
