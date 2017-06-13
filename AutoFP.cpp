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
    //����������󣬷�ֹ�����ٴδ�
    hMutex = CreateMutexA(NULL, FALSE, _T("AutoFP"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex);
        MessageBox(NULL, _T("�����Ѿ�����"), _T("AutoFP"), MB_ICONERROR);
        return FALSE;
    }
    //CWnd* pWndExist;
    //CWnd* pWndPopup;
    //pWndExist = CWnd::FindWindowA(_T("ArwenAppClass"), NULL);
    //if (pWndExist)
    //{
    //	//��������Ѿ��򿪣� �򷵻�FALSE
    //	pWndPopup = pWndExist->GetLastActivePopup();//��ô򿪵Ļ����
    //	if (pWndPopup->IsIconic())
    //	{
    //		pWndPopup->ShowWindow(SW_RESTORE);//���������С���ˣ�restore��
    //	}
    //	pWndPopup->ShowWindow(SW_SHOW);//��ʾ�Ѿ��򿪵Ĵ���
    //	pWndPopup->SetForegroundWindow();//�ô�����ʾ����ǰ��
    //	return FALSE;
    //}

    //Ӧ�ó����DLL��ʹ��Windows Sockets����֮ǰ����Ҫ����һ�γɹ���WSAStartup()����.���������Windows Sockets��ʹ�ú�Ӧ�ó����DLL����
    //����WSACleanup���������Windows Sockets��ʵ����ע�������Ҹ�ʵ���ͷ�ΪӦ�ó����DLL������κ���Դ.�κδ򿪵Ĳ��ѽ������ӵ�SOCK_STREAM
    //�����׽ӿ��ڵ���WSACleanup����ʱ������; ���Ѿ���closesocket�����ر�ȴ����Ҫ���͵�����δ�����ݵ��׽ӿ��򲻻���Ӱ�죭 ��������Ҫ����.
    WSADATA		WSAData = { 0 };
    if (0 != WSAStartup(MAKEWORD(2, 0), &WSAData))
    {
        // Tell the user that we could not find a usable WinSock DLL.
        if (LOBYTE(WSAData.wVersion) != LOBYTE(MAKEWORD(2, 0)) || HIBYTE(WSAData.wVersion) != HIBYTE(MAKEWORD(2, 0)))
            ::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);

        WSACleanup();
        return FALSE;
    }
    AfxEnableControlContainer();//����ActiveX�ؼ�������

    TCHAR szBuffer[MAX_PATH];//���ڴ洢·��
    ::GetModuleFileName(m_hInstance, szBuffer, MAX_PATH);//��ȡ��ǰ�����Ѽ���ģ����ļ�������·������ģ������ɵ�ǰ���̼��ء���ǰӦ�ó��������·����·������������Ӧ�ó�������
    char *p;
    p = strrchr(szBuffer, '\\');//�����ַ���ָ���ַ����д����濪ʼ�����һ�γ��ֵ�λ��,����ɹ������ظ��ַ��Լ��������ַ�
    *p = '\0';//��·�������һ��\��ȥ��
    gm_strSetupPath = szBuffer;
    gm_strSetupPath = gm_strSetupPath + "\\";
    gm_strConfigFile = gm_strSetupPath + "config.ini";//�����ļ�·��



    CString strLogFile = gm_strSetupPath;
    strLogFile = strLogFile + "\\log\\";//��־�ļ�·��
    //������־�ļ�����ʱ������
    CTime time;
    time = CTime::GetCurrentTime();
    CString str;
    str.Format("%d%02d%02d-%02d%02d.txt", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
    strLogFile = strLogFile + str;//��־����·��
    gm_logFile.SetPath(strLogFile);
    gm_logFile.WriteHead();

    InitGlobalMembers();//��ʼ��ȫ�ֱ���

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

    ////����λ���ļ�·���������ļ���
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
