// DlgSocket.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgSocket.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "BurningPort.h"
#include "CameraPort.h"
#include "DlgPosition.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSocket dialog


CDlgSocket::CDlgSocket(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSocket::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSocket)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nSocket = 0;
    m_nProg = 0;
}


void CDlgSocket::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSocket)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSocket, CDialog)
    //{{AFX_MSG_MAP(CDlgSocket)
    ON_BN_CLICKED(IDC_SOCPOS_SOC1, OnSocposSoc1)
    ON_BN_CLICKED(IDC_SOCPOS_SOC2, OnSocposSoc2)
    ON_BN_CLICKED(IDC_SOCPOS_SOC3, OnSocposSoc3)
    ON_BN_CLICKED(IDC_SOCPOS_SOC4, OnSocposSoc4)
    ON_BN_CLICKED(IDC_SOCKPOS_GOTO, OnSockposGoto)
    ON_BN_CLICKED(IDC_SOCKPOS_SAVE, OnSockposSave)
    ON_BN_CLICKED(IDC_SOCKPOS_START, OnSockposStart)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_SOCPOS_NOZZLE, OnSocposNozzle)
    ON_BN_CLICKED(IDC_SOCPOS_SOC5, OnSocposSoc5)
    ON_BN_CLICKED(IDC_SOCPOS_SOC6, OnSocposSoc6)
    ON_BN_CLICKED(IDC_SOCPOS_SOC7, OnSocposSoc7)
    ON_BN_CLICKED(IDC_SOCPOS_SOC8, OnSocposSoc8)
    ON_BN_CLICKED(IDC_SOCPOS_SOC9, OnSocposSoc9)
    ON_BN_CLICKED(IDC_SOCPOS_SOC10, OnSocposSoc10)
    ON_BN_CLICKED(IDC_SOCPOS_SOC11, OnSocposSoc11)
    ON_BN_CLICKED(IDC_SOCPOS_SOC12, OnSocposSoc12)
    ON_BN_CLICKED(IDC_SOCPOS_SOC13, OnSocposSoc13)
    ON_BN_CLICKED(IDC_SOCPOS_SOC14, OnSocposSoc14)
    ON_BN_CLICKED(IDC_SOCKPOS_SAVEHEIGHT, OnSockposSaveheight)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_COPYPOS_TO_ALL, &CDlgSocket::OnBnClickedBtnCopyposToAll)
    ON_BN_CLICKED(IDC_CHECK_CAMREACHECK, &CDlgSocket::OnBnClickedCheckCamreacheck)
    ON_BN_CLICKED(IDC_SOCKPOS_SAVEXY, &CDlgSocket::OnBnClickedSockposSavexy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSocket message handlers

BOOL CDlgSocket::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    CButton *pBtn = (CButton*)GetDlgItem(IDC_SOCPOS_SOC1);
    pBtn->SetCheck(TRUE);
    pBtn = (CButton*)GetDlgItem(IDC_SOCPOS_SOC5);
    pBtn->SetCheck(TRUE);

    CWnd *pWnd = GetDlgItem(IDC_SOCKPOS_SAVE);
    pWnd->EnableWindow(FALSE);
    pWnd = GetDlgItem(IDC_SOCKPOS_START);
    pWnd->EnableWindow(FALSE);

    pBtn = (CButton*)GetDlgItem(IDC_CHECK_CAMERACHECK);
    if (gm_bSocketHeightCamera)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgSocket::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    CRect rtW, rtTop;
    GetWindowRect(rtW);
    ScreenToClient(rtW);

    CBrush brush(RGB(8, 226, 206));
    CBrush *pOldBrush = dc.SelectObject(&brush);
    dc.Rectangle(rtW);
    dc.SelectObject(pOldBrush);
    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgSocket::OnSocposSoc1()
{
    m_nSocket = 0;
}

void CDlgSocket::OnSocposSoc2()
{
    m_nSocket = 1;
}

void CDlgSocket::OnSocposSoc3()
{
    m_nSocket = 2;
}

void CDlgSocket::OnSocposSoc4()
{
    m_nSocket = 3;
}
//----------
void CDlgSocket::OnSocposSoc5()
{
    m_nProg = 0;
}

void CDlgSocket::OnSocposSoc6()
{
    m_nProg = 1;
}

void CDlgSocket::OnSocposSoc7()
{
    m_nProg = 2;
}

void CDlgSocket::OnSocposSoc8()
{
    m_nProg = 3;
}

void CDlgSocket::OnSocposSoc9()
{
    m_nProg = 4;
}

void CDlgSocket::OnSocposSoc10()
{
    m_nProg = 5;
}

void CDlgSocket::OnSocposSoc11()
{
    m_nProg = 6;
}

void CDlgSocket::OnSocposSoc12()
{
    m_nProg = 7;
}

void CDlgSocket::OnSocposSoc13()
{
    m_nProg = 8;
}

void CDlgSocket::OnSocposSoc14()
{
    m_nProg = 9;
}


//相机移动至socket
void CDlgSocket::OnSockposGoto()
{
    int nSocketIndex = m_nProg*SOCKETS_OF_MODULE + m_nSocket;
    CString secStr;
    secStr.Format("Socket%d", nSocketIndex);
    Point pt{ 0,0 };
    pt = gm_dataPos.GetPlacePoint((LPCSTR)secStr);
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x, pt.y);
    gm_uInfo.xySpeed = oldSpeed;
    CWnd *pWnd = GetDlgItem(IDC_SOCKPOS_SAVE);
    pWnd->EnableWindow(TRUE);
}

//保存相机在socket的坐标
void CDlgSocket::OnSockposSave()
{
    CString str;
    double x = 0;
    double y = 0;

    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);
    str.Format("Socket%d", m_nProg*SOCKETS_OF_MODULE + m_nSocket);
    gm_dataPos.SetPlacePoint((LPCSTR)str, x, y);

    double fx = x / (float)XGUIDE;
    double fy = y / (float)XGUIDE;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_SOCPOS_EX, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_SOCPOS_EY, str);


    CWnd *pWnd = GetDlgItem(IDC_SOCKPOS_SAVE);
    pWnd->EnableWindow(FALSE);
    pWnd = GetDlgItem(IDC_SOCKPOS_START);
    pWnd->EnableWindow(TRUE);
}

//测高
void CDlgSocket::OnSockposStart()
{
    if (IDNO == MessageBox("确认烧录座中是否有芯片", "测高", MB_YESNO))
        return;
    GetDlgItem(IDC_SOCKPOS_START)->EnableWindow(FALSE);

    if (gm_bSocketHeightCamera)
    {
        CDlgPosition *pWnd = (CDlgPosition*)GetParent();
        pWnd->CloseCamera();
        SearchHeight();
        pWnd->InitCamera();
        pWnd->OpenSnapEx();
    }
    else
        SearchHeight();

    GetDlgItem(IDC_SOCKPOS_START)->EnableWindow(TRUE);
}
BOOL CDlgSocket::SearchHeight()
{
    CString secStr;
    Point socpt{ 0,0 };
    Point offset{ 0,0 };

    secStr.Format("Socket%d", m_nProg * SOCKETS_OF_MODULE + m_nSocket);
    socpt = gm_dataPos.GetPlacePoint((LPCSTR)secStr);
    if (!ZComeBack())
        return FALSE;

    if (gm_bSocketHeightCamera)
    {
        SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
        SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
        int oldSpeed = gm_uInfo.xySpeed;
        gm_uInfo.xySpeed = 1;
        XYMove(socpt.x, socpt.y);
        gm_uInfo.xySpeed = oldSpeed;
        CameraPort cp;
        cp.OpenFramegrabUp();
        Position2 pos2{ 0,0,0 };
        if (cp.CheckModel(pos2, "socket"))
        {//匹配的烧录座的模型，烧录座模型建立的时候里面是没有芯片的，所以，如果匹配成功，说明座子里面没有芯片，不能测高
            AfxMessageBox("请放入芯片再进行测高。");
            cp.CloseFramegrab();
            return FALSE;
        }
        cp.CloseFramegrab();
    }

    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    SetCylinderDown(m_nProg);
    if (!ZComeBack())
        return FALSE;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(socpt.x + offset.x, socpt.y + offset.y);
    gm_uInfo.xySpeed = oldSpeed;
    BOOL bPressed = CheckModelCylinderPressed(m_nProg);
    DWORD dwStart = ::GetTickCount();
    BOOL bFailed = FALSE;
    while (!bPressed)
    {
        bPressed = CheckModelCylinderPressed(m_nProg);
        Sleep(200);
        DWORD dwElapse = ::GetTickCount() - dwStart;
        if (dwElapse > gm_uInfo.nSocketTime)
        {
            AfxMessageBox("未检测到下拉气缸信号!");
            return FALSE;
        }
    }
    double height = 0;
    if (!MeasureHeight(height))
        return FALSE;
    gm_dataPos.SetHeight((LPCSTR)secStr, height);//保存高度
    secStr.Format("%0.2f", float(height / ZGUIDE));
    SetDlgItemText(IDC_SOCKPOS_ZHEIGHT, secStr);

    return TRUE;
}


void CDlgSocket::OnSocposNozzle()
{
    int nSocketIndex = m_nProg*SOCKETS_OF_MODULE + m_nSocket;
    CString secStr;
    secStr.Format("Socket%d", nSocketIndex);
    Point pt{ 0,0 }, offset{ 0,0 };
    pt = gm_dataPos.GetPlacePoint((LPCSTR)secStr);
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x + offset.x + gm_uInfo.nOffsetSocketX, pt.y + offset.y + gm_uInfo.nOffsetSocketY);
    gm_uInfo.xySpeed = oldSpeed;

    GetDlgItem(IDC_SOCKPOS_SAVEHEIGHT)->EnableWindow();
    GetDlgItem(IDC_SOCKPOS_SAVEXY)->EnableWindow();
}
//手动保存高度
void CDlgSocket::OnSockposSaveheight()
{
    // TODO: Add your control notification handler code here
    CString secStr;
    secStr.Format("Socket%d", m_nProg*SOCKETS_OF_MODULE + m_nSocket);
    double height = GetPosition(ZAXIS);
    gm_dataPos.SetHeight((LPCSTR)secStr, height);
    CString strValue;
    strValue.Format("%0.2f", float(height / ZGUIDE));
    SetDlgItemText(IDC_SOCKPOS_ZHEIGHT, strValue);
    GetDlgItem(IDC_SOCKPOS_SAVEHEIGHT)->EnableWindow(FALSE);


}
//吸嘴在芯片上方时，手动保存xy坐标
void CDlgSocket::OnBnClickedSockposSavexy()
{
    CString str;
    double x = 0;
    double y = 0;

    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);
    str.Format("Socket%d", m_nProg*SOCKETS_OF_MODULE + m_nSocket);
    gm_dataPos.SetPlacePoint((LPCSTR)str, x - gm_dataPos.GetPlacePoint("offset_nozzle1").x, y - gm_dataPos.GetPlacePoint("offset_nozzle1").y);
    //gm_uInfo.nOffsetSocketX = x - gm_dataPos.GetPlacePoint("offset_nozzle1").x - gm_dataPos.GetPlacePoint("TapeIn").x;
    //gm_uInfo.nOffsetSocketY = y - gm_dataPos.GetPlacePoint("offset_nozzle1").y - gm_dataPos.GetPlacePoint("TapeIn").y;
    GetDlgItem(IDC_SOCKPOS_SAVEXY)->EnableWindow(FALSE);

}



void CDlgSocket::OnBnClickedBtnCopyposToAll()
{
    if (IDOK != AfxMessageBox("将复制第一个socket的高度到所有socket，是否确定？", MB_OKCANCEL))
    {
        return;
    }
    CString secStr;
    secStr.Format("Socket%d", 0);
    double height = 0;
    height = gm_dataPos.GetHeight(secStr);
    for (int i = 0; i < SOCKETS_OF_MODULE*MAXNUMOFPROGRAMMER; i++)
    {
        secStr.Format("Socket%d", i);
        gm_dataPos.SetHeight((LPCSTR)secStr, height);//保存高度
    }

}


void CDlgSocket::OnBnClickedCheckCamreacheck()
{
    // gm_bSocketHeightCamera
    if (gm_bSocketHeightCamera == 1)
        gm_bSocketHeightCamera = 0;
    else if (gm_bSocketHeightCamera == 0)
        gm_bSocketHeightCamera = 1;
}


