// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"
#include <afxmt.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile(CString sFullPath)
{
	OperatorFile=sFullPath;
	m_writeFile.open(OperatorFile, ios::out|ios::ate);
	if(!m_writeFile)
		m_bIsOpen = false;
	else
		m_bIsOpen = true;
}

CLogFile::CLogFile()
{
	OperatorFile="";
	m_bIsOpen = false;
}
void CLogFile::SetPath(CString sFullPath)
{
	OperatorFile=sFullPath;
	m_writeFile.open(OperatorFile, ios::out|ios::ate);
	if(!m_writeFile)
		m_bIsOpen = false;
	else
		m_bIsOpen = true;
}
CLogFile::~CLogFile()
{
	m_writeFile.close();
}

void CLogFile::Write(CString action)
{
	CCriticalSection cs;
	cs.Lock();
	CTime time;
	time = CTime::GetCurrentTime();
	
	CString s;
	s.Format("%02d:%02d:%02d",time.GetHour(),time.GetMinute(),time.GetSecond());
	CString line = s;
	line = line + "\t\t";
	line = line + action;
	line = line +"\r\n";
	if(m_bIsOpen)
	{
		m_writeFile.write(line.operator LPCTSTR(),line.GetLength());
		m_writeFile.flush();
		DWORD err=GetLastError();
	}
	cs.Unlock();
	return;
}

void CLogFile::WriteHead()//写入日志文件头
{
	CString line;
	line = "-------------------- Auto Programmer BEGIN --------------------\r\n\r\n";
	if(m_bIsOpen)
	{
		m_writeFile.write(line.operator LPCTSTR(),line.GetLength());
		line = "\t时间\t\t\t\t事件\r\n";
		m_writeFile.write(line.operator LPCTSTR(),line.GetLength());
		Write("进入系统！");
		m_writeFile.flush();
	}
}
//写入日志文件尾
void CLogFile::WriteEnd()
{
	CString line;
	line = "\r\n-------------------- Auto Programmer END --------------------\r\n";
	if(m_bIsOpen)
	{
		Write("退出系统！");
		m_writeFile.write(line.operator LPCTSTR(),line.GetLength());

		m_writeFile.flush();
	}
}

void CLogFile::WriteSocketInfo()
{
	if (m_bIsOpen)
	{
		CString line;
		CString s;
		line = "---------------------------------- Auto Programmer SocketInfo -----------------------------\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		line = "-------------------------------------------------------------------------------------------\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		
		s.Format("Work Number\t\t:%s", gm_sdInfo.strWorkNo);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		s.Format("IC Manufactor\t\t:%s", gm_sdInfo.strICBrand);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		s.Format("IC Type\t\t\t:%s", gm_sdInfo.strICModel);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		s.Format("CheckSum\t\t:%s", gm_sdInfo.strCheckSum);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		s.Format("Location\t\t:%s", "suzhou");
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		s.Format("Start Time\t\t:%s", gm_sdInfo.strStartTime);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		time_t tt = time(NULL);//这句返回的只是一个时间cuo
		tm* t = localtime(&tt);
		//printf("%d-%02d-%02d %02d:%02d:%02d\n",
		//	t->tm_year + 1900,
		//	t->tm_mon + 1,
		//	t->tm_mday,
		//	t->tm_hour,
		//	t->tm_min,
		//	t->tm_sec);
		CString strTemp;
		strTemp.Format("%d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
		gm_sdInfo.strEndTime = strTemp;

		s.Format("End Time\t\t:%s", gm_sdInfo.strEndTime);
		line = "------\t" + s + "\t\t\t" + "\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line = "-------------------------------------------------------------------------------------------\r\n\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line = "Sockets       OK   ID Check  Erase   Blank  Program Verify Insertion System   Pos.  OK Yield\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		line = "-------------------------------------------------------------------------------------------\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		//for (int i = 0; i < MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET; i++)
		//{
		//	line.Format("S%02d  :        %d       %d       %d       %d       %d       %d       %d       %d       %d       %.2f%%\r\n", i + 1, gm_nDisplayGoodForSocket[i], 0, 0, gm_nDisplayFailForSocket[i], 0, 0, 0, 0, 0, gm_nDisplayGoodForSocket[i] + gm_nDisplayFailForSocket[i] == 0 ? 0 : gm_nDisplayGoodForSocket[i] / (float)(gm_nDisplayGoodForSocket[i] + gm_nDisplayFailForSocket[i]) * 100);
		//	m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		//}
		for (int i = 0; i < MAXNUMOFPROGRAMMER; i++)
		{
			for (int j = 0; j < MAXNUMOFSOCKET; j++)
			{
				line.Format("#%2d%d :        %d       %d       %d       %d       %d       %d       %d       %d       %d       %.2f%%\r\n", i + 1,j+1, gm_nDisplayGoodForSocket[i*MAXNUMOFSOCKET+j], 0, 0, gm_nDisplayFailForSocket[i*MAXNUMOFSOCKET + j], 0, 0, 0, 0, 0, gm_nDisplayGoodForSocket[i*MAXNUMOFSOCKET + j] + gm_nDisplayFailForSocket[i*MAXNUMOFSOCKET + j] == 0 ? 0 : gm_nDisplayGoodForSocket[i*MAXNUMOFSOCKET + j] / (float)(gm_nDisplayGoodForSocket[i*MAXNUMOFSOCKET + j] + gm_nDisplayFailForSocket[i*MAXNUMOFSOCKET + j]) * 100);
				m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
			}
		}

		line = "-------------------------------------------------------------------------------------------\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line.Format("Total:        %d       %d       %d       %d       %d       %d       %d       %d       %d       %.2f%%\r\n\r\n", gm_nTotalGoodSize, 0, 0, gm_nTotalFailSize, 0, 0, 0, 0, 0, (gm_nTotalGoodSize + gm_nTotalFailSize)==0?0: gm_nTotalGoodSize / (float)(gm_nTotalGoodSize + gm_nTotalFailSize) * 100);
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line = "**************************************************************\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line.Format("Total Program  = \t%d\r\n", gm_nTotalGoodSize + gm_nTotalFailSize);
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		line.Format("Total OK       = \t%d\r\n", gm_nTotalGoodSize);
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		line.Format("Total NG       = \t%d\r\n", gm_nTotalFailSize);
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		line.Format("Total Yield    = \t%.2f%%\r\n", gm_nTotalGoodSize + gm_nTotalFailSize==0?0:gm_nTotalGoodSize / (float)(gm_nTotalGoodSize + gm_nTotalFailSize) * 100);
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());

		line = "**************************************************************\r\n";
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());


		m_writeFile.flush();
	}

}

void CLogFile::WriteSocket(CString action)
{
	CCriticalSection cs;
	cs.Lock();
	CTime time;
	time = CTime::GetCurrentTime();

	CString s;
	s.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
	CString line = s;
	line = line + "\t\t";
	line = line + action;
	line = line + "\r\n";
	if (m_bIsOpen)
	{
		m_writeFile.write(line.operator LPCTSTR(), line.GetLength());
		m_writeFile.flush();
		DWORD err = GetLastError();
	}
	cs.Unlock();
	return;
}
