// AutoFP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AutoFP.h"
#include "AutoFPDlg.h"
#include "MotionAct.h"
#include "MotionAct.h"
#include "SocketManager.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"

#include <winsock2.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoFPApp

BEGIN_MESSAGE_MAP(CAutoFPApp, CWinApp)
    //{{AFX_MSG_MAP(CAutoFPApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoFPApp construction

CAutoFPApp::CAutoFPApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAutoFPApp object

CAutoFPApp theApp;
HANDLE hMutex;
/////////////////////////////////////////////////////////////////////////////
// CAutoFPApp initialization

BOOL CAutoFPApp::InitInstance()
{
    //创建互斥对象，防止程序被再次打开
    hMutex = CreateMutexA(NULL, FALSE, _T("AutoFP"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex);
        MessageBox(NULL, _T("程序已经被打开"), _T("AutoFP"), MB_ICONERROR);
        return FALSE;
    }
    //CWnd* pWndExist;
    //CWnd* pWndPopup;
    //pWndExist = CWnd::FindWindowA(_T("ArwenAppClass"), NULL);
    //if (pWndExist)
    //{
    //	//如果程序已经打开， 则返回FALSE
    //	pWndPopup = pWndExist->GetLastActivePopup();//获得打开的活动窗体
    //	if (pWndPopup->IsIconic())
    //	{
    //		pWndPopup->ShowWindow(SW_RESTORE);//如果窗体最小化了，restore它
    //	}
    //	pWndPopup->ShowWindow(SW_SHOW);//显示已经打开的窗体
    //	pWndPopup->SetForegroundWindow();//让窗体显示在最前面
    //	return FALSE;
    //}

    //应用程序或DLL在使用Windows Sockets服务之前必须要进行一次成功的WSAStartup()调用.当它完成了Windows Sockets的使用后，应用程序或DLL必须
    //调用WSACleanup（）将其从Windows Sockets的实现中注销，并且该实现释放为应用程序或DLL分配的任何资源.任何打开的并已建立连接的SOCK_STREAM
    //类型套接口在调用WSACleanup（）时会重置; 而已经由closesocket（）关闭却仍有要发送的悬而未决数据的套接口则不会受影响－ 该数据仍要发送.
    WSADATA		WSAData = { 0 };
    if (0 != WSAStartup(MAKEWORD(2, 0), &WSAData))
    {
        // Tell the user that we could not find a usable WinSock DLL.
        if (LOBYTE(WSAData.wVersion) != LOBYTE(MAKEWORD(2, 0)) || HIBYTE(WSAData.wVersion) != HIBYTE(MAKEWORD(2, 0)))
            ::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);

        WSACleanup();
        return FALSE;
    }
    AfxEnableControlContainer();//允许ActiveX控件包容器

    TCHAR szBuffer[MAX_PATH];//用于存储路径
    ::GetModuleFileName(m_hInstance, szBuffer, MAX_PATH);//获取当前进程已加载模块的文件的完整路径，该模块必须由当前进程加载。当前应用程序的完整路径，路径名并不包括应用程序名称
    char *p;
    p = strrchr(szBuffer, '\\');//查找字符在指定字符串中从左面开始的最后一次出现的位置,如果成功，返回该字符以及其后面的字符
    *p = '\0';//将路径的最后一格‘\’去掉
    gm_strSetupPath = szBuffer;
    gm_strSetupPath = gm_strSetupPath + "\\";
    gm_strConfigFile = gm_strSetupPath + "config.ini";//配置文件路径



    CString strLogFile = gm_strSetupPath;
    strLogFile = strLogFile + "\\log\\";//日志文件路径
    //设置日志文件名以时间命名
    CTime time;
    time = CTime::GetCurrentTime();
    CString str;
    str.Format("%d%02d%02d-%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
    strLogFile = strLogFile + str;//日志完整路径
    gm_logFile.SetPath(strLogFile);
    gm_logFile.WriteHead();

    InitGlobalMembers();//初始化全局变量

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
//cmmLoadDll();
//if (cmmGnDeviceLoad(cmTRUE, 0) != cmERR_NONE)
//{
//	cmmErrShowLast(NULL);
//	return -1;
//}
//InitMotionDevices(szBuffer);
    CAutoFPDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();


    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

int CAutoFPApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    //if(gm_pTray)
    //{
    //	delete[] gm_pTray;
    //	gm_pTray = NULL;
    //}
    //gm_logFile.WriteEnd();
    //CloseMotionCard();
    //WSACleanup();

    ////保存位置文件路径到配置文件中
    //CIniFile inf;
    //inf.SetIniFileName(gm_strConfigFile);
    //inf.WriteString("File", "Last", gm_strPositionFile);
    //for (int i = 0; i<MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET; i++)
    //{
    //	int evalue = gm_sBurningStatus[i / MAXNUMOFSOCKET].bEmpty[i%MAXNUMOFSOCKET];
    //	CString str;
    //	str.Format("Socket%d", i);
    //	inf.WriteIntNumber("LastUsed", str, gm_bSocketUsed[i]);
    //}

    return CWinApp::ExitInstance();
}
