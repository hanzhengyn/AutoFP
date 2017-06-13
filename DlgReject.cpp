// DlgReject.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgReject.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include "MotionAct.h"
#include "Burningport.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgReject dialog


CDlgReject::CDlgReject(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgReject::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgReject)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CDlgReject::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgReject)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgReject, CDialog)
    //{{AFX_MSG_MAP(CDlgReject)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO1, OnNgbinCcdto1)
    ON_BN_CLICKED(IDC_NGBIN_SAVE1, OnNgbinSave1)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO2, OnNgbinCcdto2)
    ON_BN_CLICKED(IDC_NGBIN_SAVE2, OnNgbinSave2)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO3, OnNgbinCcdto3)
    ON_BN_CLICKED(IDC_NGBIN_SAVE3, OnNgbinSave3)
    ON_BN_CLICKED(IDC_NGBIN_SAVE5, OnNgbinSave5)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO4, OnNgbinCcdto4)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO5, OnNgbinCcdto5)
    ON_BN_CLICKED(IDC_NGBIN_CCDTO6, OnNgbinCcdto6)
    ON_WM_PAINT()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_NGBIN_SAVE7, &CDlgReject::OnBnClickedNgbinSave7)
    ON_BN_CLICKED(IDC_NGBIN_SAVE6, &CDlgReject::OnBnClickedNgbinSave6)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgReject message handlers

void CDlgReject::OnNgbinCcdto1()
{
    Point pt{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("NGBinPos1");
    double x = pt.x;
    double y = pt.y;
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    XYMove(x, y);
    gm_uInfo.xySpeed = oldSpeed;
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE1);
    pWnd->EnableWindow(TRUE);
}

void CDlgReject::OnNgbinSave1()
{
    double x = 0;
    x = GetPosition(XAXIS);
    double y = 0;
    y = GetPosition(YAXIS);

    gm_dataPos.SetPlacePoint("NGBinPos1", x, y);
    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_NGBIN_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_NGBIN_Y, str);
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE1);
    pWnd->EnableWindow(FALSE);
}

void CDlgReject::OnNgbinCcdto2()
{
    Point pt = gm_dataPos.GetPlacePoint("NGBinPos2");
    double x = pt.x;
    double y = pt.y;
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    XYMove(x, y);
    gm_uInfo.xySpeed = oldSpeed;
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE2);
    pWnd->EnableWindow(TRUE);
}

void CDlgReject::OnNgbinSave2()
{
    double x = 0;
    x = GetPosition(XAXIS);
    double y = 0;
    y = GetPosition(YAXIS);

    gm_dataPos.SetPlacePoint("NGBinPos2", x, y);
    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_NGBIN_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_NGBIN_Y, str);
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE2);
    pWnd->EnableWindow(FALSE);
}

void CDlgReject::OnNgbinCcdto3()
{
    Point pt = gm_dataPos.GetPlacePoint("NGBinPos3");
    double x = pt.x;
    double y = pt.y;
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    XYMove(x, y);
    gm_uInfo.xySpeed = oldSpeed;
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE3);
    pWnd->EnableWindow(TRUE);
}

void CDlgReject::OnNgbinSave3()
{
    double x = 0;
    x = GetPosition(XAXIS);
    double y = 0;
    y = GetPosition(YAXIS);

    gm_dataPos.SetPlacePoint("NGBinPos3", x, y);
    double fx = x / (float)XGUIDE;
    double fy = y / (float)YGUIDE;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_NGBIN_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_NGBIN_Y, str);
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE3);
    pWnd->EnableWindow(FALSE);
}

BOOL CDlgReject::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO: Add extra initialization here
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE1);
    pWnd->EnableWindow(FALSE);
    pWnd = GetDlgItem(IDC_NGBIN_SAVE2);
    pWnd->EnableWindow(FALSE);
    pWnd = GetDlgItem(IDC_NGBIN_SAVE3);
    pWnd->EnableWindow(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CDlgReject::SearchHeight()
{
    Point pt = gm_dataPos.GetPlacePoint("NGBinPos1");
    Point offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    if (!ZComeBack())
        return FALSE;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x + offset.x, pt.y + offset.y);
    gm_uInfo.xySpeed = oldSpeed;
    double height = 0;
    if (!MeasureHeight(height))
        return FALSE;
    gm_dataPos.SetHeight("NGBinHeight", height);
    CString strValue;
    strValue.Format("%0.2f", float(height / ZGUIDE));
    SetDlgItemText(IDC_TRAY_Z, strValue);
    return TRUE;
}


void CDlgReject::OnNgbinSave5()
{
    if (IDYES != AfxMessageBox("请确保测高位置有芯片。", MB_YESNO))
        return;
    CWnd *pWnd = GetDlgItem(IDC_NGBIN_SAVE5);
    pWnd->EnableWindow(FALSE);
    if (!ZComeBack())
    {
        AfxMessageBox("z轴回原异常");
        pWnd->EnableWindow(TRUE);
        return;
    }
    SearchHeight();
    pWnd->EnableWindow(TRUE);
}

//吸嘴移动至1
void CDlgReject::OnNgbinCcdto4()
{
    Point pt{ 0,0 }, offset{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("NGBinPos1");
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x + offset.x, pt.y + offset.y);
    gm_uInfo.xySpeed = oldSpeed;
}

void CDlgReject::OnNgbinCcdto5()
{
    Point pt{ 0,0 }, offset{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("NGBinPos2");
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x + offset.x, pt.y + offset.y);
    gm_uInfo.xySpeed = oldSpeed;
}

void CDlgReject::OnNgbinCcdto6()
{
    Point pt{ 0,0 }, offset{ 0,0 };
    pt = gm_dataPos.GetPlacePoint("NGBinPos3");
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    if (!ZComeBack())
        return;
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(pt.x + offset.x, pt.y + offset.y);
    gm_uInfo.xySpeed = oldSpeed;
}

void CDlgReject::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // TODO: Add your message handler code here
    CRect rtW, rtTop;
    GetWindowRect(rtW);
    ScreenToClient(rtW);

    CBrush brush(RGB(192, 255, 192));
    CBrush *pOldBrush = dc.SelectObject(&brush);
    dc.Rectangle(rtW);
    dc.SelectObject(pOldBrush);
    // Do not call CDialog::OnPaint() for painting messages
}

void CDlgReject::OnBnClickedNgbinSave7()
{
    double height = GetPosition(ZAXIS);
    gm_dataPos.SetHeight("NGBinHeight", height);
    CString strValue;
    strValue.Format("%0.2f", float(height / ZGUIDE));
    SetDlgItemText(IDC_TRAY_Z, strValue);
}


void CDlgReject::OnBnClickedNgbinSave6()
{
    static int m_nIndex = 0;
    int xn = m_nIndex%gm_dataNG.xn;
    int yn = m_nIndex / gm_dataNG.xn;
    if (yn >= (gm_dataNG.yn))
    {
        m_nIndex = 0;
        return;
    }

    Point pt1 = gm_dataPos.GetPlacePoint("NGBinPos1");
    Point pt2 = gm_dataPos.GetPlacePoint("NGBinPos2");
    Point pt3 = gm_dataPos.GetPlacePoint("NGBinPos3");
    double xpitch = 0.0;
    double ypitch = 0.0;
    double x1, y1, x2, y2, x3, y3;

    x1 = pt1.x;
    y1 = pt1.y;
    x2 = pt2.x;
    y2 = pt2.y;
    x3 = pt3.x;
    y3 = pt3.y;
    xpitch = (float)((x1 - x2) / (gm_dataNG.xn - 1));
    ypitch = (float)((y3 - y2) / (gm_dataNG.yn - 1));

    double xFirst = pt1.x;//inf.GetInt("TrayPos1","X",0);
    double yFirst = pt1.y;//inf.GetInt("TrayPos1","Y",0);

    double cornerX = 0.0;
    double cornerY = 0.0;
    double lastX = 0.0;
    int trayXn = gm_dataNG.xn;
    int trayYn = gm_dataNG.yn;
    cornerX = pt2.x;//inf.GetInt("TrayPos2","X",0);
    cornerY = pt2.y;//inf.GetInt("TrayPos2","Y",0);

    lastX = pt3.x;//inf.GetInt("TrayPos3","X",0);

    double xPos = xFirst - xn * xpitch + (lastX - cornerX) / (trayYn - 1) * yn;;
    double yPos = yFirst + yn * ypitch + (cornerY - yFirst) / (trayXn - 1)*xn;
    Point pt;
    pt.x = (ULONG)xPos;
    pt.y = (ULONG)yPos;
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    XYMove(pt.x, pt.y);
    m_nIndex++;

}
