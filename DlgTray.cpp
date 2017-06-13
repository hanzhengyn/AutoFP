// DlgTray.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgTray.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "AutoFPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTray dialog


CDlgTray::CDlgTray(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTray::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgTray)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nIndex = 0;
    m_bSave1 = false;
    m_bSave2 = false;
    m_bSave3 = false;
}


void CDlgTray::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgTray)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTray, CDialog)
    //{{AFX_MSG_MAP(CDlgTray)
    ON_BN_CLICKED(IDC_TRAY_CCD1, OnTrayCcd1)
    ON_BN_CLICKED(IDC_TRAY_SAVE1, OnTraySave1)
    ON_BN_CLICKED(IDC_TRAY_CCD2, OnTrayCcd2)
    ON_BN_CLICKED(IDC_TRAY_SAVE2, OnTraySave2)
    ON_BN_CLICKED(IDC_TRAY_CCD3, OnTrayCcd3)
    ON_BN_CLICKED(IDC_TRAY_SAVE3, OnTraySave3)
    ON_BN_CLICKED(IDC_TRAY_SEARCH, OnTraySearch)
    ON_BN_CLICKED(IDC_TRAY_PITCH, OnTrayPitch)
    ON_BN_CLICKED(IDC_TRAY_PITCH2, OnTrayPitch2)
    ON_WM_PAINT()

    ON_BN_CLICKED(IDC_BTN_AUTOSET, OnBtnAutoset)
    ON_BN_CLICKED(IDC_BTN_TRAYRECEIVED, OnBtnTrayreceived)
    ON_BN_CLICKED(IDC_BTN_FPEND, OnBtnFpend)
    ON_BN_CLICKED(IDC_BTN_AUTOTRAYSTOP, OnBtnAutotraystop)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BTN_FPEND2, &CDlgTray::OnBnClickedBtnFpend2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTray message handlers
BOOL CDlgTray::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    EnableSave(0, FALSE);
    EnableSave(1, FALSE);
    EnableSave(2, FALSE);
    CWnd *pWnd = GetDlgItem(IDC_TRAY_PITCH);
    pWnd->EnableWindow(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgTray::EnableSave(int index, BOOL bEnabled)
{
    UINT nID[] = { IDC_TRAY_SAVE1,IDC_TRAY_SAVE2,IDC_TRAY_SAVE3 };
    CWnd *pWnd = GetDlgItem(nID[index]);
    pWnd->EnableWindow(bEnabled);
}

void CDlgTray::OnTrayCcd1()
{
    Point pt{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("TrayPos1");
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x, pt.y);
    gm_uInfo.xySpeed = oldSpeed;
    EnableSave(0, TRUE);
}

void CDlgTray::OnTraySave1()
{
    double x = 0;
    double y = 0;
    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);
    gm_dataPos.SetPlacePoint("TrayPos1", x, y);

    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_TRAY_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_TRAY_Y, str);

    EnableSave(0, FALSE);
    m_bSave1 = true;
    if (m_bSave2&&m_bSave3)
    {
        CWnd *pWnd = GetDlgItem(IDC_TRAY_PITCH);
        pWnd->EnableWindow(TRUE);
    }
}

void CDlgTray::OnTrayCcd2()
{
    Point pt{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("TrayPos2");
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x, pt.y);
    gm_uInfo.xySpeed = oldSpeed;
    EnableSave(1, TRUE);
}

void CDlgTray::OnTraySave2()
{
    double x = 0;
    double y = 0;
    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);
    gm_dataPos.SetPlacePoint("TrayPos2", x, y);

    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_TRAY_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_TRAY_Y, str);

    EnableSave(1, FALSE);
    m_bSave2 = true;
    if (m_bSave1&&m_bSave3)
    {
        CWnd *pWnd = GetDlgItem(IDC_TRAY_PITCH);
        pWnd->EnableWindow(TRUE);
    }
}

void CDlgTray::OnTrayCcd3()
{
    Point pt{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("TrayPos3");
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x, pt.y);
    gm_uInfo.xySpeed = oldSpeed;
    EnableSave(2, TRUE);
}

void CDlgTray::OnTraySave3()
{
    double x = 0;
    double y = 0;
    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);
    gm_dataPos.SetPlacePoint("TrayPos3", x, y);

    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_TRAY_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_TRAY_Y, str);

    EnableSave(2, FALSE);
    m_bSave3 = true;
    if (m_bSave1&&m_bSave2)
    {
        CWnd *pWnd = GetDlgItem(IDC_TRAY_PITCH);
        pWnd->EnableWindow(TRUE);
    }
}


void CDlgTray::OnTraySearch()
{
    // TODO: Add your control notification handler code here
    if (IDYES != AfxMessageBox("请确定三个角上有芯片！", MB_YESNO))
        return;
    CWnd *pWnd = GetDlgItem(IDC_TRAY_SEARCH);
    pWnd->EnableWindow(FALSE);
    SearchHeight();
    pWnd->EnableWindow(TRUE);
}
BOOL CDlgTray::SearchHeight()
{
    CString str;
    Point pt{ 0,0 };
    Point offset{ 0,0 };
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    for (int i = 1; i < 4; i++)
    {
        str.Format("TrayPos%d", i);
        pt = gm_dataPos.GetPlacePoint(str);
        if (!ZComeBack())
            return FALSE;
        SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
        SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
        int oldSpeed = gm_uInfo.xySpeed;
        gm_uInfo.xySpeed = 1;
        XYMove(pt.x + offset.x, pt.y + offset.y);
        gm_uInfo.xySpeed = oldSpeed;
        double height = 0;
        if (!MeasureHeight(height))
            return FALSE;
        str.Format("TrayHeight%d", i);
        gm_dataPos.SetHeight(str, height);
        str.Format("%0.2f", float(height / ZGUIDE));
        SetDlgItemText(IDC_TRAY_Z, str);
    }
    return TRUE;
}

//计算偏差
void CDlgTray::OnTrayPitch()
{
    // TODO: Add your control notification handler code here
    int xn = gm_dataTray.xn;
    int yn = gm_dataTray.yn;
    if ((xn - 1) < 1 || (yn - 1) < 1)
    {
        AfxMessageBox("Tray Size is not correct!");
        return;
    }

    double xpitch = 0.0;
    double ypitch = 0.0;
    double x1, y1, x2, y2, x3, y3;

    Point pt1 = gm_dataPos.GetPlacePoint("TrayPos1");
    Point pt2 = gm_dataPos.GetPlacePoint("TrayPos2");
    Point pt3 = gm_dataPos.GetPlacePoint("TrayPos3");

    x1 = pt1.x;//inf.GetInt("TrayPos1","X",0);
    y1 = pt1.y;//inf.GetInt("TrayPos1","Y",0);
    x2 = pt2.x;//inf.GetInt("TrayPos2","X",0);
    y2 = pt2.y;//inf.GetInt("TrayPos2","Y",0);
    x3 = pt3.x;//inf.GetInt("TrayPos3","X",0);
    y3 = pt3.y;//inf.GetInt("TrayPos3","Y",0);
    xpitch = (float)((x1 - x2) / (xn - 1));
    ypitch = (float)((y3 - y2) / (yn - 1));
    gm_dataPos.SetPitch(xpitch, ypitch);

    CString strValue;
    strValue.Format("%0.2f", xpitch / (float)XGUIDE);
    SetDlgItemText(IDC_TRAY_X2, strValue);
    strValue.Format("%0.2f", ypitch / (float)YGUIDE);
    SetDlgItemText(IDC_TRAY_Y2, strValue);

    m_bSave1 = false;
    m_bSave2 = false;
    m_bSave3 = false;
}

//芯片位置
void CDlgTray::OnTrayPitch2()
{
    int xn = m_nIndex%gm_dataTray.xn;
    int yn = m_nIndex / gm_dataTray.xn;
    if (yn >= (gm_dataTray.yn))
    {
        m_nIndex = 0;
        return;
    }
    double xPitch = gm_dataPos.GetPitchX();//inf.GetFloat("Pitch","X",0.0);
    double yPitch = gm_dataPos.GetPitchY();//inf.GetFloat("Pitch","Y",0.0);

    Point pt1 = gm_dataPos.GetPlacePoint("TrayPos1");
    Point pt2 = gm_dataPos.GetPlacePoint("TrayPos2");
    Point pt3 = gm_dataPos.GetPlacePoint("TrayPos3");

    double xFirst = pt1.x;//inf.GetInt("TrayPos1","X",0);
    double yFirst = pt1.y;//inf.GetInt("TrayPos1","Y",0);

    double cornerX = 0.0;
    double cornerY = 0.0;
    double lastX = 0.0;
    int trayXn = gm_dataTray.xn;
    int trayYn = gm_dataTray.yn;
    cornerX = pt2.x;//inf.GetInt("TrayPos2","X",0);
    cornerY = pt2.y;//inf.GetInt("TrayPos2","Y",0);

    lastX = pt3.x;//inf.GetInt("TrayPos3","X",0);

    double xPos = xFirst - xn * xPitch + (lastX - cornerX) / (trayYn - 1) * yn;;
    double yPos = yFirst + yn * yPitch + (cornerY - yFirst) / (trayXn - 1)*xn;
    Point pt;
    pt.x = (ULONG)xPos;
    pt.y = (ULONG)yPos;
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    XYMove(pt.x, pt.y);
    m_nIndex++;

    double x = GetPosition(XAXIS);
    double y = GetPosition(YAXIS);
    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_TRAY_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_TRAY_Y, str);
}

void CDlgTray::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    CRect rtW, rtTop;
    GetWindowRect(rtW);
    ScreenToClient(rtW);

    CBrush brush(RGB(60, 164, 255));
    CBrush *pOldBrush = dc.SelectObject(&brush);
    dc.Rectangle(rtW);
    dc.SelectObject(pOldBrush);
    // Do not call CDialog::OnPaint() for painting messages
}


void CDlgTray::OnBnContinueTest()
{
    // TODO: Add your control notification handler code here

}

void CDlgTray::OnBtnAutoset()
{
    // TODO: Add your control notification handler code here

    CAutoFPDlg * pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    pDlg->StartTrayPLC();
}

void CDlgTray::OnBtnTrayreceived()
{
    // TODO: Add your control notification handler code here
    CAutoFPDlg * pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    gm_bTrayArrived = pDlg->ReadTrayReadySignal();
}

void CDlgTray::OnBtnFpend()
{
    // TODO: Add your control notification handler code here
    CAutoFPDlg * pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    pDlg->SendCommandToTray("05303046464257304D30333232303131");
    Sleep(500);
    pDlg->SendCommandToTray("05303046464257304D30333232303130");
}

void CDlgTray::OnBtnAutotraystop()
{
    // TODO: Add your control notification handler code here
    CAutoFPDlg * pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    pDlg->SendCommandToTray("05303046464257304D30303231303131");
    Sleep(500);
    pDlg->SendCommandToTray("05303046464257304D30303231303130");

}


void CDlgTray::OnBnClickedBtnFpend2()
{
    CAutoFPDlg * pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    pDlg->SendCommandToTray("05303046464257304D30303231303130");
}
