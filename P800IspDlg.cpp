// P800IspDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "P800IspDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// P800IspDlg dialog


P800IspDlg::P800IspDlg(CWnd* pParent /*=NULL*/)
	: CDialog(P800IspDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(P800IspDlg)
	m_strIP = _T("192.168.9.140");
	m_strProjectName = _T("");
	m_dwBaudrate = 115200;
	m_dwCOM = 0;
	m_dwPort = 8800;
	m_deviceID = 0;
	m_proIdex = 1;
	m_bGetStatus = FALSE;
	//}}AFX_DATA_INIT
}


void P800IspDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(P800IspDlg)
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_BAUD_PROJECT_NAME, m_strProjectName);
	DDX_Text(pDX, IDC_EDIT_BAUD_RATE, m_dwBaudrate);
	DDX_Text(pDX, IDC_EDIT_COM, m_dwCOM);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
	DDX_Text(pDX, IDC_EDIT_PROJECT_INDEX, m_proIdex);

//	DDX_Control(pDX,IDC_EDIT_STATUS1,m_status[0]);
//	DDX_Control(pDX,IDC_EDIT_STATUS2,m_status[1]);
//	DDX_Control(pDX,IDC_EDIT_STATUS3,m_status[2]);
//	DDX_Control(pDX,IDC_EDIT_STATUS4,m_status[3]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(P800IspDlg, CDialog)
	//{{AFX_MSG_MAP(P800IspDlg)
	ON_BN_CLICKED(IDC_BTN_WLAN_CONNECT, OnBtnWlanConnect)
	ON_BN_CLICKED(IDC_BTN_OPEN_PROJECT, OnBtnOpenProject)
	ON_BN_CLICKED(IDC_BTN_START1, OnBtnStart1)
	ON_BN_CLICKED(IDC_BTN_START2, OnBtnStart2)
	ON_BN_CLICKED(IDC_BTN_START3, OnBtnStart3)
	ON_BN_CLICKED(IDC_BTN_START4, OnBtnStart4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// P800IspDlg message handlers

wchar_t * MBToUnicode(const char* pmb)  
{  
	__int32 mLen = -1;
	// convert an MBCS string to widechar
	wchar_t * pun = NULL;
	__int32 uLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);  
	
	if (uLen<=0)  
	{  
		return pun;  
	}
	pun = new wchar_t[uLen + 1];
	if (NULL == pun)
	{
		return pun;
	}
	memset(pun,0,sizeof(wchar_t) * (uLen+1));
	__int32 nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen,pun, uLen);  
	
	if (nRtn != uLen)  
	{
		delete [] pun;
		pun = NULL;
	}  
	return pun;  
}
//Unicode编码转为多字节编码  
char * UnicodeToMB(const wchar_t* pun)
{  
	__int32 uLen = -1;
	// convert an widechar string to Multibyte   
	char * pmb = NULL;
	__int32 MBLen = WideCharToMultiByte(CP_ACP, 0, pun, uLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return pmb;  
	}
	pmb = new char[MBLen+1];
	if (pmb == NULL)
	{
		return pmb;
	}
	memset(pmb,0,MBLen+1);
	int nRtn = WideCharToMultiByte(CP_ACP, 0, pun, uLen, pmb, MBLen, NULL, NULL);  
	
	if(nRtn != MBLen)  
	{  
		delete [] pmb;
		pmb = NULL;
	}  
	return pmb;  
}
void P800IspDlg::OnBtnWlanConnect()
{
	UpdateData(TRUE);
	if (m_deviceID != 0)
	{
		AfxMessageBox(_T("已连接设备！"));
		return;
	}
	PXX_ATE_WLAN wlanInfo;

	strncpy(wlanInfo.ip,m_strIP,16);
	wlanInfo.port = m_dwPort;
	m_deviceID = ConnectDevice(PXX_ATE_TYPE_WLAN,&wlanInfo);
	if (m_deviceID == 0)
	{
		AfxMessageBox(_T("连接设备失败"));
	}
	//启动线程，检测设备状态
	AfxBeginThread(AFX_THREADPROC(GetStatusThread),this);
}

DWORD WINAPI P800IspDlg::GetStatusThread(LPVOID lpvoid)
{
	P800IspDlg * pDlg = (P800IspDlg*)lpvoid;
	DWORD statusArray[16];
	CString strValue;
	pDlg->m_bGetStatus = true;
	while (pDlg->m_bGetStatus)
	{
		GetDeviceStatus(pDlg->m_deviceID,statusArray);
		for (int i = 0;i < 4;i++)
		{
			switch (statusArray[i])
			{
			case SOFT_ATE_STATUS_ERR:
				strValue = _T("操作失败");
				break;
			case SOFT_ATE_STATUS_OK:
				strValue = _T("操作成功");
				break;
			case SOFT_ATE_STATUS_POWERON:
				strValue = _T("上电中");
				break;
			case SOFT_ATE_STATUS_INIT:
				strValue = _T("初始化中");
				break;
			case SOFT_ATE_STATUS_ERASE:
				strValue = _T("擦除中");
				break;
			case SOFT_ATE_STATUS_CHECK:
				strValue = _T("查空中");
				break;
			case SOFT_ATE_STATUS_PROGRAM:
				strValue = _T("编程中");
				break;
			case SOFT_ATE_STATUS_VERIFY:
				strValue = _T("检验中");
				break;
			case SOFT_ATE_STATUS_READ:
				strValue = _T("读取中");
				break;				
			case SOFT_ATE_STATUS_WAITCONTROL:
				strValue = _T("等待控制");
				break;
			case SOFT_ATE_STATUS_OP_END:
				strValue = _T("操作结束");
				break;
				
			case SOFT_ATE_STATUS_UNUSE:
				strValue = _T("未启用");
				break;
			}
			if (pDlg->m_status[i].GetSafeHwnd())
			{
				pDlg->m_status[i].SetWindowText(strValue);
			}
		}
		Sleep(300);
	}
	return 0;
}
void P800IspDlg::OnBtnOpenProject() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_deviceID == 0)
	{
		AfxMessageBox(_T("请先连接设备"));
		return;
	}
	if (!OpenDeviceProject(m_deviceID,m_proIdex))
	{
		AfxMessageBox(_T("打开设备工程失败"));
	}
}

void P800IspDlg::OnBtnStart1() 
{
	// TODO: Add your control notification handler code here
	if(m_deviceID == 0)
	{
		AfxMessageBox(_T("请先连接设备"));
		return;
	}
	if(!StartDeviceOperater(m_deviceID,1))
	{
		AfxMessageBox(_T("启动"));
	}
}

void P800IspDlg::OnBtnStart2() 
{
	// TODO: Add your control notification handler code here
	
}

void P800IspDlg::OnBtnStart3() 
{
	// TODO: Add your control notification handler code here
	
}

void P800IspDlg::OnBtnStart4() 
{
	// TODO: Add your control notification handler code here
	
}
void P800IspDlg::Stop()
{
	if(m_deviceID == 0)
	{
		AfxMessageBox(_T("请先连接设备"));
		return;
	}
	if(!StopRunning(m_deviceID))
	{
		AfxMessageBox(_T("停止失败"));
	}
}
void P800IspDlg::UnConnect()
{
	if(m_deviceID == 0)
	{
		AfxMessageBox(_T("请先连接设备"));
		return;
	}
	m_bGetStatus = false;
	Sleep(100);
	if(!UnConnectDevice(PXX_ATE_TYPE_COM,m_deviceID))
	{
		AfxMessageBox(_T("断开连接失败"));
	}
	else
		m_deviceID = 0;
}
void P800IspDlg::Reset()
{
	if(m_deviceID == 0)
	{
		AfxMessageBox(_T("请先连接设备"));
		return;
	}
	m_bGetStatus = false;
	Sleep(100);
	if(!ResetSystem(m_deviceID))
	{
		AfxMessageBox(_T("重启失败"));
	}
	else
		m_deviceID = 0;
}


void P800IspDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类
	Stop();
	UnConnect();

	CDialog::OnCancel();
}
