#include "stdafx.h"
#include "PxxATE.h"


CPxxATE::CPxxATE()
{
    m_strIP = "192.168.9.140";
    m_dwPort = 8800;
}


CPxxATE::~CPxxATE()
{
}

BOOL CPxxATE::ConnectWithWlan()
{
    if (0 != m_deviceID)
    {
        AfxMessageBox("已连接设备。");
        return FALSE;
    }
    PXX_ATE_WLAN wlanInfo;
    strcpy(wlanInfo.ip, m_strIP.GetBuffer(0));
    m_strIP.ReleaseBuffer();
    wlanInfo.port = m_dwPort;
    m_deviceID = ConnectDevice(PXX_ATE_TYPE_WLAN, &wlanInfo);
    if (0 == m_deviceID)
    {
        AfxMessageBox("连接设备失败");
    }
}

BOOL CPxxATE::ConnectWithUlart()
{
    if (0 != m_deviceID)
    {
        AfxMessageBox("已连接设备。");
        return FALSE;
    }
    PXX_ATE_COM comInfo;
    comInfo.port = m_dwCom;
    comInfo.baudrate = m_dwBaudrate;
    m_deviceID = ConnectDevice(PXX_ATE_TYPE_COM, &comInfo);
    if (0 == m_deviceID)
    {
        AfxMessageBox("连接设备失败");
    }
}

BOOL CPxxATE::OpenProjectWithIdex()
{
    if (0 == m_deviceID)
    {
        AfxMessageBox("请先连接设备");
        return FALSE;
    }
    if (!OpenDeviceProject(m_deviceID, m_nProjectIndex))
    {
        AfxMessageBox("打开工程失败。");
        return FALSE;
    }
    char projectName[100];
    GetProjectName(m_deviceID, m_nProjectIndex, projectName, 100);
    m_strProjectName = projectName;
    return TRUE;
}

BOOL CPxxATE::OpenProjectWithName()
{
    if (0 == m_deviceID)
    {
        AfxMessageBox("请先连接设备");
        return FALSE;
    }
    if (!OpenDeviceProjectName(m_deviceID, m_strProjectName.GetBuffer(0)))
    {
        AfxMessageBox("打开工程失败。");
        return FALSE;
    }
    return TRUE;
}
//单独执行某个座子的烧录过程
BOOL CPxxATE::StartProgramm(int channel)
{
    if (0 == m_deviceID)
    {
        AfxMessageBox("请先连接设备");
        return FALSE;
    }
    if (!StartDeviceOperater(m_deviceID, 1 << channel))
    {
        CString str;
        str.Format("启动%d号烧录座失败。");
        AfxMessageBox(str);
        return FALSE;
    }
    return TRUE;
}

//对设备执行具体的某个操作：擦除、校验、组合、量产等
BOOL CPxxATE::OperateDevice(int op)
{
    if (m_deviceID == 0)
    {
        AfxMessageBox(_T("请先连接设备"));
        return FALSE;
    }
    if (!RunDeviceOperator(m_deviceID, op))
    {
        AfxMessageBox(_T("操作失败"));
        return FALSE;
    }
    return TRUE;
}

DWORD WINAPI CPxxATE::GetStatusThread(LPVOID lpvoid)
{
    CPxxATE * pDlg = (CPxxATE*)lpvoid;
    DWORD statusArray[16];
    CString strValue;
    pDlg->m_bGetStatus = true;
    while (pDlg->m_bGetStatus)
    {
        GetDeviceStatus(pDlg->m_deviceID, statusArray);
        for (int i = 0; i < 16; i++)
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
            //if (pDlg->m_status[i].GetSafeHwnd())
            //{
            //	pDlg->m_status[i].SetWindowText(strValue);
            //}
        }
        Sleep(300);
    }
    return 0;
}
