// AutoFP.h : main header file for the AUTOFP application
//

#if !defined(AFX_AUTOFP_H__3FF63675_8644_4A82_AE7A_96B8E9DA17EA__INCLUDED_)
#define AFX_AUTOFP_H__3FF63675_8644_4A82_AE7A_96B8E9DA17EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAutoFPApp:
// See AutoFP.cpp for the implementation of this class
//

class CAutoFPApp : public CWinApp
{
public:
	CAutoFPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoFPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAutoFPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOFP_H__3FF63675_8644_4A82_AE7A_96B8E9DA17EA__INCLUDED_)
