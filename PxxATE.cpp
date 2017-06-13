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
        AfxMessageBox("�������豸��");
        return FALSE;
    }
    PXX_ATE_WLAN wlanInfo;
    strcpy(wlanInfo.ip, m_strIP.GetBuffer(0));
    m_strIP.ReleaseBuffer();
    wlanInfo.port = m_dwPort;
    m_deviceID = ConnectDevice(PXX_ATE_TYPE_WLAN, &wlanInfo);
    if (0 == m_deviceID)
    {
        AfxMessageBox("�����豸ʧ��");
    }
}

BOOL CPxxATE::ConnectWithUlart()
{
    if (0 != m_deviceID)
    {
        AfxMessageBox("�������豸��");
        return FALSE;
    }
    PXX_ATE_COM comInfo;
    comInfo.port = m_dwCom;
    comInfo.baudrate = m_dwBaudrate;
    m_deviceID = ConnectDevice(PXX_ATE_TYPE_COM, &comInfo);
    if (0 == m_deviceID)
    {
        AfxMessageBox("�����豸ʧ��");
    }
}

BOOL CPxxATE::OpenProjectWithIdex()
{
    if (0 == m_deviceID)
    {
        AfxMessageBox("���������豸");
        return FALSE;
    }
    if (!OpenDeviceProject(m_deviceID, m_nProjectIndex))
    {
        AfxMessageBox("�򿪹���ʧ�ܡ�");
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
        AfxMessageBox("���������豸");
        return FALSE;
    }
    if (!OpenDeviceProjectName(m_deviceID, m_strProjectName.GetBuffer(0)))
    {
        AfxMessageBox("�򿪹���ʧ�ܡ�");
        return FALSE;
    }
    return TRUE;
}
//����ִ��ĳ�����ӵ���¼����
BOOL CPxxATE::StartProgramm(int channel)
{
    if (0 == m_deviceID)
    {
        AfxMessageBox("���������豸");
        return FALSE;
    }
    if (!StartDeviceOperater(m_deviceID, 1 << channel))
    {
        CString str;
        str.Format("����%d����¼��ʧ�ܡ�");
        AfxMessageBox(str);
        return FALSE;
    }
    return TRUE;
}

//���豸ִ�о����ĳ��������������У�顢��ϡ�������
BOOL CPxxATE::OperateDevice(int op)
{
    if (m_deviceID == 0)
    {
        AfxMessageBox(_T("���������豸"));
        return FALSE;
    }
    if (!RunDeviceOperator(m_deviceID, op))
    {
        AfxMessageBox(_T("����ʧ��"));
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
                strValue = _T("����ʧ��");
                break;
            case SOFT_ATE_STATUS_OK:
                strValue = _T("�����ɹ�");
                break;
            case SOFT_ATE_STATUS_POWERON:
                strValue = _T("�ϵ���");
                break;
            case SOFT_ATE_STATUS_INIT:
                strValue = _T("��ʼ����");
                break;
            case SOFT_ATE_STATUS_ERASE:
                strValue = _T("������");
                break;
            case SOFT_ATE_STATUS_CHECK:
                strValue = _T("�����");
                break;
            case SOFT_ATE_STATUS_PROGRAM:
                strValue = _T("�����");
                break;
            case SOFT_ATE_STATUS_VERIFY:
                strValue = _T("������");
                break;
            case SOFT_ATE_STATUS_READ:
                strValue = _T("��ȡ��");
                break;
            case SOFT_ATE_STATUS_WAITCONTROL:
                strValue = _T("�ȴ�����");
                break;
            case SOFT_ATE_STATUS_OP_END:
                strValue = _T("��������");
                break;
            case SOFT_ATE_STATUS_UNUSE:
                strValue = _T("δ����");
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
