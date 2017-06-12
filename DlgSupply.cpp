// DlgSupply.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgSupply.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "AutoFPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSupply dialog


CDlgSupply::CDlgSupply(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSupply::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSupply)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIndex = 0;
	m_bSave1 = false;
	m_bSave2 = false;
	m_bSave3 = false;
}


void CDlgSupply::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSupply)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
BOOL CDlgSupply::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableSave(0,FALSE);
	EnableSave(1,FALSE);
	EnableSave(2,FALSE);
	CWnd *pWnd = GetDlgItem(IDC_SUPPLY_PITCH);
	pWnd->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CDlgSupply, CDialog)
	//{{AFX_MSG_MAP(CDlgSupply)
	ON_BN_CLICKED(IDC_SUPPLY_CCD1, OnSupplyCcd1)
	ON_BN_CLICKED(IDC_SUPPLY_CCD2, OnSupplyCcd2)
	ON_BN_CLICKED(IDC_SUPPLY_CCD3, OnSupplyCcd3)
	ON_BN_CLICKED(IDC_SUPPLY_SAVE1, OnSupplySave1)
	ON_BN_CLICKED(IDC_SUPPLY_SAVE2, OnSupplySave2)
	ON_BN_CLICKED(IDC_SUPPLY_SAVE3, OnSupplySave3)
	ON_BN_CLICKED(IDC_SUPPLY_SEARCH, OnSupplySearch)
	ON_BN_CLICKED(IDC_SUPPLY_PITCH, OnSupplyPitch)
	ON_BN_CLICKED(IDC_SUPPLY_PITCH2, OnSupplyPitch2)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSupply message handlers


void CDlgSupply::EnableSave(int index,BOOL bEnabled)
{
	UINT nID[]={IDC_SUPPLY_SAVE1,IDC_SUPPLY_SAVE2,IDC_SUPPLY_SAVE3};
	CWnd *pWnd = GetDlgItem(nID[index]);
	pWnd->EnableWindow(bEnabled);
}

void CDlgSupply::OnSupplyCcd1() 
{
	Point pt{ 0,0 };
	pt = gm_dataPos.GetPlacePoint("SupplyPos1");
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x,pt.y);
	gm_uInfo.xySpeed = oldSpeed;
	EnableSave(0,TRUE);
}

void CDlgSupply::OnSupplyCcd2() 
{
	// TODO: Add your control notification handler code here
	Point pt{ 0,0 };
	pt = gm_dataPos.GetPlacePoint("SupplyPos2");
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x,pt.y);
	gm_uInfo.xySpeed = oldSpeed;
	EnableSave(1,TRUE);
}

void CDlgSupply::OnSupplyCcd3() 
{
	Point pt{ 0,0 };
	pt = gm_dataPos.GetPlacePoint("SupplyPos3");
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x,pt.y);
	gm_uInfo.xySpeed = oldSpeed;
	EnableSave(2,TRUE);
}

void CDlgSupply::OnSupplySave1() 
{
	double x = 0;
	double y = 0;
	x = GetPosition(XAXIS);
	y = GetPosition(YAXIS);
	gm_dataPos.SetPlacePoint("SupplyPos1",x,y);

	double fx = x / (float)XGUIDE;
	double fy = y / (float)YGUIDE;
	CString str;
	str.Format("%0.2f",fx);
	SetDlgItemText(IDC_SUPPLY_X,str);
	str.Format("%0.2f",fy);
	SetDlgItemText(IDC_SUPPLY_Y,str);

	EnableSave(0,FALSE);
	m_bSave1 = true;
	if(m_bSave2&&m_bSave3)
	{
		CWnd *pWnd = GetDlgItem(IDC_SUPPLY_PITCH);
		pWnd->EnableWindow(TRUE);
	}
}

void CDlgSupply::OnSupplySave2() 
{
	double x = 0;
	double y = 0;
	x = GetPosition(XAXIS);
	y = GetPosition(YAXIS);
	gm_dataPos.SetPlacePoint("SupplyPos2",x,y);

	double fx = x / (float)XGUIDE;
	double fy = y / (float)YGUIDE;
	CString str;
	str.Format("%0.2f", fx);
	SetDlgItemText(IDC_SUPPLY_X, str);
	str.Format("%0.2f", fy);
	SetDlgItemText(IDC_SUPPLY_Y, str);

	EnableSave(1,FALSE);
	m_bSave2 = true;
	if(m_bSave1&&m_bSave3)
	{
		CWnd *pWnd = GetDlgItem(IDC_SUPPLY_PITCH);
		pWnd->EnableWindow(TRUE);
	}
}

void CDlgSupply::OnSupplySave3() 
{
	double x = 0;
	double y = 0;
	x = GetPosition(XAXIS);
	y = GetPosition(YAXIS);
	gm_dataPos.SetPlacePoint("SupplyPos3",x,y);

	double fx = x / (float)XGUIDE;
	double fy = y / (float)YGUIDE;
	CString str;
	str.Format("%0.2f", fx);
	SetDlgItemText(IDC_SUPPLY_X, str);
	str.Format("%0.2f", fy);
	SetDlgItemText(IDC_SUPPLY_Y, str);

	EnableSave(2,FALSE);
	m_bSave3 = true;
	if(m_bSave1&&m_bSave2)
	{
		CWnd *pWnd = GetDlgItem(IDC_SUPPLY_PITCH);
		pWnd->EnableWindow(TRUE);
	}
}

void CDlgSupply::OnSupplySearch() 
{
	if (IDYES != AfxMessageBox("请确定三个角上有芯片！", MB_YESNO))
		return;
	CWnd *pWnd = GetDlgItem(IDC_SUPPLY_SEARCH);
	pWnd->EnableWindow(FALSE);
	SearchHeight();
	pWnd->EnableWindow(TRUE);
}
BOOL CDlgSupply::SearchHeight()
{
	CString str;
	Point pt{ 0,0 };
	Point offset{ 0,0 };
	for (int i = 1; i < 4; i++)
	{
		str.Format("SupplyPos%d", i);
		pt = gm_dataPos.GetPlacePoint(str);
		offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
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
		str.Format("SupplyHeight%d", i);
		gm_dataPos.SetHeight(str, height);
		str.Format("%0.2f", float(height / ZGUIDE));
		SetDlgItemText(IDC_TRAY_Z, str);
	}
	return TRUE;
}
//计算补料盘芯片间距
void CDlgSupply::OnSupplyPitch() 
{
	// TODO: Add your control notification handler code here
	int xn = gm_dataStock.xn;
	int yn = gm_dataStock.yn;
	if((xn -1)<1 || (yn-1)<1)
	{
		AfxMessageBox("Supply Tray Size is not correct!");
		return;
	}
	
	double xpitch=0.0;
	double ypitch=0.0;
	double x1,y1,x2,y2,x3,y3;
	CString strValue;
	
	Point pt1 = gm_dataPos.GetPlacePoint("SupplyPos1");
	Point pt2 = gm_dataPos.GetPlacePoint("SupplyPos2");
	Point pt3 = gm_dataPos.GetPlacePoint("SupplyPos3");
	
	x1 = pt1.x;//inf.GetInt("TrayPos1","X",0);
	y1 = pt1.y;//inf.GetInt("TrayPos1","Y",0);
	x2 = pt2.x;//inf.GetInt("TrayPos2","X",0);
	y2 = pt2.y;//inf.GetInt("TrayPos2","Y",0);
	x3 = pt3.x;//inf.GetInt("TrayPos3","X",0);
	y3 = pt3.y;//inf.GetInt("TrayPos3","Y",0);
	xpitch = (float)((x1-x2)/(xn-1));
	ypitch = (float)((y3-y2)/(yn-1));
	
	
	strValue.Format("%0.2f", xpitch / (float)XGUIDE);
	SetDlgItemText(IDC_SUPPLY_X2,strValue);
	strValue.Format("%0.2f",ypitch / (float)YGUIDE);
	SetDlgItemText(IDC_SUPPLY_Y2,strValue);
	
	gm_dataPos.SetPitch(xpitch,ypitch);
	m_bSave1 = false;
	m_bSave2 = false;
	m_bSave3 = false;
}

//依次移动上相机到达补料盘各芯片位置，检查坐标是否准确
void CDlgSupply::OnSupplyPitch2() 
{
	int xn = m_nIndex%gm_dataStock.xn;
	int yn = m_nIndex/gm_dataStock.xn;
	if(yn>=(gm_dataStock.yn))
	{
		m_nIndex = 0;
		return;
	}

	double xPitch = gm_dataPos.GetPitchX();//inf.GetFloat("Pitch","X",0.0);
	double yPitch = gm_dataPos.GetPitchY();//inf.GetFloat("Pitch","Y",0.0);
	
	Point pt1 = gm_dataPos.GetPlacePoint("SupplyPos1");
	Point pt2 = gm_dataPos.GetPlacePoint("SupplyPos2");
	Point pt3 = gm_dataPos.GetPlacePoint("SupplyPos3");
	
	double xFirst = pt1.x;//inf.GetInt("TrayPos1","X",0);
	double yFirst = pt1.y;//inf.GetInt("TrayPos1","Y",0);
	
	double cornerX = 0.0;
	double cornerY = 0.0;
	double lastX = 0.0;
	int trayXn = gm_dataStock.xn;
	int trayYn = gm_dataStock.yn;
	cornerX = pt2.x;//inf.GetInt("TrayPos2","X",0);
	cornerY = pt2.y;//inf.GetInt("TrayPos2","Y",0);
	
	lastX = pt3.x;//inf.GetInt("TrayPos3","X",0);
	
	double xPos = xFirst - xn * xPitch + (lastX-cornerX)/(trayYn - 1) * yn;;
	double yPos = yFirst + yn * yPitch + (cornerY - yFirst) / (trayXn - 1)*xn;
	Point pt;
	pt.x = (ULONG)xPos;
	pt.y = (ULONG)yPos;
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x, pt.y);
	gm_uInfo.xySpeed = oldSpeed;
	m_nIndex++;
	
	double x = GetPosition(XAXIS);
	double y = GetPosition(YAXIS);
	double fx = x / (float)XGUIDE;
	double fy = y / (float)YGUIDE;
	CString str;
	str.Format("%0.2f",fx);
	SetDlgItemText(IDC_SUPPLY_X,str);
	str.Format("%0.2f",fy);
	SetDlgItemText(IDC_SUPPLY_Y,str);
}


void CDlgSupply::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rtW, rtTop;
	GetWindowRect(rtW);
	ScreenToClient(rtW);

	CBrush brush(RGB(220, 126, 216));
	CBrush *pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(rtW);
	dc.SelectObject(pOldBrush);
}
