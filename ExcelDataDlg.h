#pragma once
#include "StdAfx.h"
#include "afxcmn.h"
#include "sqlite3.h"
#include <vector>
#include "afxdtctl.h"
#define _MAX_SQL	(1000)

#define HEADSTR 4
#define MYINT 1
#define MYFLOAT 2
#define MYSTRING 3
#define MYDOUBLE 4

class CExcelDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExcelDataDlg)

public:
	CExcelDataDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExcelDataDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATAEXCEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	sqlite3* pdb = NULL;
	WCHAR dbName[_MAX_PATH] = L"";
	CString strColumnName[18];
	vector<sData*> m_sData;
	DECLARE_MESSAGE_MAP()
public:
	// 数据列表
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	afx_msg
		void ReadData();
	void OnBnClickedRead();
	BOOL OpenData(sqlite3 *pdb);
	void InSertFPData();
	BOOL insertData(sqlite3 *pdb, sData *pdata);
	void AddOutRes(TCHAR * szPosName, float x, float y, float tstThres);
	void AddOutRes(sData *sd);
	bool GetExcelHead(CListCtrl *plist);
	CString GetLevelDate(int nID);
	afx_msg void OnBnClickedOutputToExcel();
	void UpdateData(sData * pdata);
	// 插入数据
	int InsertData(sData *pdata);
	afx_msg void OnBnClickedInsert();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnFilter();
	CDateTimeCtrl m_datatimeStart;
	afx_msg void OnBnClickedBtnAllDisplay();
	afx_msg void OnBnClickedBtnDeleteItem();
};
