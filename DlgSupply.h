#if !defined(AFX_DLGSUPPLY_H__242DE0DC_FF0E_4377_8FA8_7C4903366A1D__INCLUDED_)
#define AFX_DLGSUPPLY_H__242DE0DC_FF0E_4377_8FA8_7C4903366A1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSupply.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSupply dialog

class CDlgSupply : public CDialog
{
// Construction
private:
	int m_nIndex;
	bool m_bSave1;
	bool m_bSave2;
	bool m_bSave3;
public:
	CDlgSupply(CWnd* pParent = NULL);   // standard constructor
	BOOL SearchHeight();
	void EnableSave(int index,BOOL bEnabled);
// Dialog Data
	//{{AFX_DATA(CDlgSupply)
	enum { IDD = IDD_POSITION_SUPPLY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSupply)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSupply)
	afx_msg void OnSupplyCcd1();
	afx_msg void OnSupplyCcd2();
	afx_msg void OnSupplyCcd3();
	afx_msg void OnSupplySave1();
	afx_msg void OnSupplySave2();
	afx_msg void OnSupplySave3();
	afx_msg void OnSupplySearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnSupplyPitch();
	afx_msg void OnSupplyPitch2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPLY_H__242DE0DC_FF0E_4377_8FA8_7C4903366A1D__INCLUDED_)
