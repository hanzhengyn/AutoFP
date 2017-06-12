// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__626E5B1B_33B3_40A8_ABD7_88199B305484__INCLUDED_)
#define AFX_LOGFILE_H__626E5B1B_33B3_40A8_ABD7_88199B305484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <fstream>
#include <iostream>
using namespace std;
class CLogFile  
{
public:
	CLogFile();
	CLogFile(CString sFullPath);
	virtual ~CLogFile();
	void Write(CString action);
	void SetPath(CString sFullPath);
	void WriteHead();//写入日志文件头
	void WriteEnd();
	void WriteSocketInfo();
	void WriteSocket(CString action);
private:
	CString     OperatorFile;
	fstream		m_writeFile;
	bool m_bIsOpen;
};

#endif // !defined(AFX_LOGFILE_H__626E5B1B_33B3_40A8_ABD7_88199B305484__INCLUDED_)
