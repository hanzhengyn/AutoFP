// DlgTapeIn.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgTapeIn.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "AutoFPDlg.h"
#include "HalconCpp.h"
#include "DlgPosition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTapeIn dialog


CDlgTapeIn::CDlgTapeIn(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTapeIn::IDD, pParent)
{
	m_bTapeIn = TRUE;
	m_bCameraOpenUp = FALSE;
}
CDlgTapeIn::~CDlgTapeIn()
{

}

void CDlgTapeIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTapeIn)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTapeIn, CDialog)
	//{{AFX_MSG_MAP(CDlgTapeIn)
	ON_BN_CLICKED(IDC_TAPEIN_CCD, OnTapeinCcd)
	ON_BN_CLICKED(IDC_TAPEIN_SAVE, OnTapeinSave)
	ON_BN_CLICKED(IDC_TAPEIN_STEP2, OnTapeinStep2)
	ON_BN_CLICKED(IDC_TAPEIN_SEARCH, OnTapeinSearch)
	ON_BN_CLICKED(IDC_TAPEIN_STEP, OnTapeinStep)
	ON_BN_CLICKED(IDC_TAPEIN_STEP3, OnTapeinStep3)
	ON_BN_CLICKED(IDC_TAPEIN_STEP4, OnTapeinStep4)
	ON_BN_CLICKED(IDC_TAPEIN_STEP5, OnTapeinStep5)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TAPEIN_STEPF2, OnTapeinStepf2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_TAPEIN_SEARCH2, OnTapeinSearch2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CREATE_MODEL, &CDlgTapeIn::OnBnClickedBtnCreateModel)
	ON_BN_CLICKED(IDC_BTN_SEARCH_MODEL, &CDlgTapeIn::OnBnClickedBtnSearchModel)
	ON_BN_CLICKED(IDC_TAPEIN_SAVEXY, &CDlgTapeIn::OnBnClickedTapeinSavexy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTapeIn message handlers

BOOL CDlgTapeIn::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//进料
void CDlgTapeIn::OnRadio1()
{
	// TODO: Add your control notification handler code here
	m_bTapeIn = TRUE;
}

//出料
void CDlgTapeIn::OnRadio2()
{
	// TODO: Add your control notification handler code here
	m_bTapeIn = FALSE;
}

//填充Tape窗口背景色
void CDlgTapeIn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rtW;
	GetWindowRect(rtW);
	ScreenToClient(rtW);
	CBrush brush(RGB(8, 226, 206));
	CBrush *pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(rtW);
	dc.SelectObject(pOldBrush);
	// Do not call CDialog::OnPaint() for painting messages
}

//按钮，上镜头移动到编带处，m_bTapeIn为true时到进料处，否则到出料处
void CDlgTapeIn::OnTapeinCcd()
{
	Point pt{ 0,0 };
	if(m_bTapeIn)
	{
		pt = gm_dataPos.GetPlacePoint("TapeIn");
	}
	else
	{
		pt = gm_dataPos.GetPlacePoint("TapeOut");
	}
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x,pt.y);
	gm_uInfo.xySpeed = oldSpeed;
	GetDlgItem(IDC_TAPEIN_SAVE)->EnableWindow();
}

//保存编带进出料坐标
void CDlgTapeIn::OnTapeinSave()
{
	double x = 0;
	double y = 0;
	x = GetPosition(XAXIS);
	y = GetPosition(YAXIS);
	if(m_bTapeIn)
		gm_dataPos.SetPlacePoint("TapeIn",x,y);
	else
		gm_dataPos.SetPlacePoint("TapeOut",x,y);

	double fx = x / (float)XGUIDE;
	double fy = y / (float)YGUIDE;
	CString str;
	str.Format("%0.2f", fx);
	SetDlgItemText(IDC_TAPEIN_X, str);
	str.Format("%0.2f", fy);
	SetDlgItemText(IDC_TAPEIN_Y, str);

	GetDlgItem(IDC_TAPEIN_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_TAPEIN_SEARCH)->EnableWindow(TRUE);
}
//测高
void CDlgTapeIn::OnTapeinSearch()
{
	if (IDNO == AfxMessageBox("请确定Tape上是否有芯片！", MB_YESNO))
		return;
	GetDlgItem(IDC_TAPEIN_SEARCH)->EnableWindow(FALSE);
	SearchHeight();
	//GetDlgItem(IDC_TAPEIN_SEARCH)->EnableWindow(TRUE);
}

//测高程序
BOOL CDlgTapeIn::SearchHeight()
{
	Point pt{0,0};
	Point offset{0,0};
	if (m_bTapeIn)
		pt = gm_dataPos.GetPlacePoint("TapeIn");
	else
		pt = gm_dataPos.GetPlacePoint("TapeOut");

	offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
	if (!ZComeBack())
		return FALSE;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x + offset.x, pt.y + offset.y);
	gm_uInfo.xySpeed = oldSpeed;
	double height = 0.0;
	if (!MeasureHeight(height))
		return FALSE;
	if (m_bTapeIn)
		gm_dataPos.SetHeight("TapeIn", height);
	else
		gm_dataPos.SetHeight("TapeOut", height);
	CString strValue;
	strValue.Format("%0.2f", float(height / ZGUIDE));
	SetDlgItemText(IDC_TAPEIN_Z, strValue);
	return TRUE;
}

//吸嘴移动至进出料处
void CDlgTapeIn::OnTapeinStep2()
{
	Point pt{ 0,0 }, offset{ 0,0 };
	double height = 0.0;
	if (m_bTapeIn)
	{
		pt = gm_dataPos.GetPlacePoint("TapeIn");
		height = gm_dataPos.GetHeight("TapeIn");
	}
	else
	{
		pt = gm_dataPos.GetPlacePoint("TapeOut");
		height = gm_dataPos.GetHeight("TapeOut");
	}
	offset=gm_dataPos.GetPlacePoint("offset_nozzle1");
	if (!ZComeBack())
		return;
	SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
	SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
	int oldSpeed = gm_uInfo.xySpeed;
	gm_uInfo.xySpeed = 1;
	XYMove(pt.x + offset.x+ gm_uInfo.nOffsetOutX, pt.y + offset.y+ gm_uInfo.nOffsetOutY);
	//SzMove(height);
	gm_uInfo.xySpeed = oldSpeed;
	GetDlgItem(IDC_TAPEIN_SEARCH2)->EnableWindow();
	GetDlgItem(IDC_TAPEIN_SAVEXY)->EnableWindow();
}


//手动保存高度坐标
void CDlgTapeIn::OnTapeinSearch2()
{
	double height = GetPosition(ZAXIS);
	if(m_bTapeIn)
		gm_dataPos.SetHeight("TapeIn",height);
	else
		gm_dataPos.SetHeight("TapeOut",height);
	CString strValue;
	strValue.Format("%0.2f", float(height / ZGUIDE));
	SetDlgItemText(IDC_TAPEIN_Z,strValue);
	GetDlgItem(IDC_TAPEIN_SEARCH2)->EnableWindow(FALSE);
}
//吸嘴在芯片上方时，保存xy坐标
void CDlgTapeIn::OnBnClickedTapeinSavexy()
{
	double x = 0;
	double y = 0;
	x = GetPosition(XAXIS);
	y = GetPosition(YAXIS);
	if (m_bTapeIn)
	{
		//gm_dataPos.SetPlacePoint("TapeIn", x - gm_dataPos.GetPlacePoint("offset_nozzle1").x, y - gm_dataPos.GetPlacePoint("offset_nozzle1").y);
		gm_uInfo.nOffsetInX = x - gm_dataPos.GetPlacePoint("offset_nozzle1").x - gm_dataPos.GetPlacePoint("TapeIn").x;
		gm_uInfo.nOffsetInY = y - gm_dataPos.GetPlacePoint("offset_nozzle1").y - gm_dataPos.GetPlacePoint("TapeIn").y;
	}
	else
	{
		//gm_dataPos.SetPlacePoint("TapeOut", x - gm_dataPos.GetPlacePoint("offset_nozzle1").x, y - gm_dataPos.GetPlacePoint("offset_nozzle1").y);
		gm_uInfo.nOffsetOutX = x - gm_dataPos.GetPlacePoint("offset_nozzle1").x - gm_dataPos.GetPlacePoint("TapeOut").x;
		gm_uInfo.nOffsetOutY = y - gm_dataPos.GetPlacePoint("offset_nozzle1").y - gm_dataPos.GetPlacePoint("TapeOut").y;
	}
	GetDlgItem(IDC_TAPEIN_SAVEXY)->EnableWindow(FALSE);
}

//前进一格
void CDlgTapeIn::OnTapeinStep()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	if(m_bTapeIn)
		pDlg->TapeInOk();
	else
		pDlg->TapeOutOk();
}

//启动编带PLC
void CDlgTapeIn::OnTapeinStep3()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	pDlg->StartPLC();
}

//停止编带PLC
void CDlgTapeIn::OnTapeinStep4()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	pDlg->StopPLC();
}

//步进补偿
void CDlgTapeIn::OnTapeinStep5()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	if (m_bTapeIn)
	{
		//pDlg->TapeInInit();
	}
	else
		//pDlg->TapeOutInit();
		pDlg->TapeOutBackOk();//新编带机没有了步进补偿，暂时将出料后退一个放在这儿
}


//消息，步进手动
BOOL CDlgTapeIn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	int buID;
	CWnd* pWnd=WindowFromPoint(pMsg->pt); //获得指定点句柄
	buID=pWnd->GetDlgCtrlID();//获得该句柄的ID号。
	//步进手动按下时
	if(pMsg->message==WM_LBUTTONDOWN) 
	{  
		if(buID==IDC_TAPEIN_STEPF)
		{
			if(m_bTapeIn)
				pDlg->TapeInGo();
			else
				pDlg->TapeOutGo();
		}
	}
	//步进手动弹起或鼠标移开时
	if((pMsg->message==WM_LBUTTONUP)||(pMsg->message==WM_MOUSELEAVE)) 
	{
		if(buID==IDC_TAPEIN_STEPF)
		{
			if(m_bTapeIn)
				pDlg->TapeInGoDisable();
			else
				pDlg->TapeOutGoDisable();
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//微调
void CDlgTapeIn::OnTapeinStepf2()
{
	// TODO: Add your control notification handler code here
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	if(m_bTapeIn)
	{
		pDlg->TapeInTune();
	}
	else
		pDlg->TapeOutTune();
}

//正向
void CDlgTapeIn::OnRadio3()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	//CButton *pBtn1 = (CButton*)GetDlgItem(IDC_RADIO3);
	//CButton *pBtn2 = (CButton*)GetDlgItem(IDC_RADIO4);
	if(m_bTapeIn)
		pDlg->TapeInDir(TRUE);
	else
		pDlg->TapeOutDir(TRUE);
}

//反向
void CDlgTapeIn::OnRadio4()
{
	CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
	if(m_bTapeIn)
		pDlg->TapeInDir(FALSE);
		//pBtn1->SetCheck(FALSE);
		//pBtn2->SetCheck(TRUE);
	else
		pDlg->TapeOutDir(FALSE);
}

//建立模型
void CDlgTapeIn::OnBnClickedBtnCreateModel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlgPosition* pWnd = (CDlgPosition*)GetParent();
	pWnd->CloseCamera();
	GetDlgItem(IDC_BTN_SEARCH_MODEL)->EnableWindow(TRUE);
	SetUpCameraLightOn();
	Sleep(100);
	InitModelCamera();
	if (m_bCameraOpenUp)
	{
		try
		{
			CloseFramegrabber(m_hv_AcqHandle_Up);
			m_bCameraOpenUp = FALSE;
			CDlgPosition *pWnd = (CDlgPosition*)GetParent();
			pWnd->InitCamera();
			pWnd->OpenSnapEx();
		}
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			MessageBox("没有需要关闭的相机");
		}
	}

}

//Halcon 打开相机
void CDlgTapeIn::OpenFramegrab()
{
	try
	{
		m_strCameraNum = gm_uInfo.strCameraNum;//MER-130-30UM-L
		m_strCameraVersion = gm_uInfo.strCameraVersion;
		HTuple Device = m_strCameraVersion + "(" + m_strCameraNum + ") producer:C:\\Program Files\\Daheng Imavision\\MER-Series\\GenTL\\Win32\\GxUSBTL.cti interface:0:DHUSBBase vendor:Daheng Imavision model:" + m_strCameraVersion;

		OpenFramegrabber("GenICamTL",
			0, 0, 0, 0, 0, 0,
			"progressive",
			-1,
			"default",
			-1,
			"false",
			"default",
			Device,
			0,
			-1,
			&m_hv_AcqHandle_Up);

		m_bCameraOpenUp = TRUE;

		SetFramegrabberParam(m_hv_AcqHandle_Up, "ExposureTime", EXPOSITION);

		GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);

		GetImageSize(m_ho_Image, &m_hv_Width, &m_hv_Height);

		// Initialize window
		CRect rt;
		GetDlgItem(IDC_CAMERA_FRAME)->GetWindowRect(&rt);
		ScreenToClient(rt);

		m_Window.OpenWindow(rt.top,
			rt.left,
			rt.Width(),
			rt.Height(),
			(Hlong)m_hWnd,
			"visible",
			"");

		//m_Window.SetPart(0, 0, m_hv_Width, m_hv_Height);
		m_Window.SetLineWidth(1);
		m_Window.DispObj(m_ho_Image);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		MessageBox("OpenFramegrab Camera Error.");
	}
}

//Halcon 初始化相机
void CDlgTapeIn::InitModelCamera()
{
	try
	{
		OpenFramegrab();
		HTuple row1 = 512 - gm_uInfo.nTapeWidth*gm_uInfo.nPixelsPM_up / 2 - 8;
		HTuple column1 = 640 - gm_uInfo.nTapeLength*gm_uInfo.nPixelsPM_up / 2 - 8;
		HTuple row2 = 512 + gm_uInfo.nTapeWidth*gm_uInfo.nPixelsPM_up / 2 + 8;
		HTuple column2 = 640 + gm_uInfo.nTapeLength*gm_uInfo.nPixelsPM_up / 2 + 8;
		GenRectangle1(&m_ho_ROI_Model_Domain, row1, column1, row2, column2);

		GenRectangle1(&m_ho_ROI_Search_Domain, 462, 589, 566.5, 697.5);
		//GenRectangle1(&m_ho_ROI_Model_Domain, 485, 614.5, 541, 672.5);
		
		ReduceDomain(m_ho_Image, m_ho_ROI_Model_Domain, &m_ho_ImageReduced);

		InspectShapeModel(m_ho_ImageReduced, &m_ho_ModelImages, &m_ho_ModelRegions, 1, 30);
		CreateModel();

		FindShapeModel(m_ho_Image,
			m_hv_ModelID,				//ModelID
			HTuple(-30).TupleRad(),							//AngleStart
			HTuple(60).TupleRad(),		//AngleExtent
			0.5,						//MinScore
			1,							//NumMatches
			0.5,						//MaxOverlap
			"least_squares",			//SubPixel
			0,							//NumLevels
			0.9,						//Greediness
			&m_hv_Row,
			&m_hv_Column,
			&m_hv_Angle,
			&m_hv_Score);
//		GetShapeModelContours(&m_ho_ModelContours, m_hv_ModelID, 1);
		//dev_display_shape_matching_results(m_hv_ModelID, "green", m_hv_Row, m_hv_Column, m_hv_Angle,1, 1, 0);
		//if (m_bTapeIn)
		//{
		//	WriteShapeModel(m_hv_ModelID, "c:/S100/Model/TapeInModel.shm");
		//	gm_hv_Row_In = m_hv_Row;
		//	gm_hv_Column_In = m_hv_Column;
		//	gm_hv_Angle_In = m_hv_Angle;
		//}
		//else
		//{
			WriteShapeModel(m_hv_ModelID, "c:/S100/Model/TapeOutModel.shm");
			gm_hv_Row_Out = m_hv_Row;
			gm_hv_Column_Out = m_hv_Column;
			gm_hv_Angle_Out = m_hv_Angle;
		//}

		//Hlong row_model = atof((m_hv_Row.ToString()).Text());
		//Hlong column_model = atof((m_hv_Column.ToString()).Text());
		//m_Window.SetTposition(row_model, column_model);
		m_Window.SetDraw("margin");
		m_Window.SetColor("green");
		//
		m_Window.SetPart(0, 0, 1024, 1280);
		m_Window.DispObj(m_ho_ROI_Model_Domain);
		m_Window.SetColor("red");
		m_Window.DispCross(m_hv_Row, m_hv_Column, 1000, 0);
		//m_Window.DispObj(m_ho_ImageReduced);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		MessageBox("FindShapeModel Error.");
	}
}

//Halcon 建立模型
void CDlgTapeIn::CreateModel()
{
	try
	{
		CreateShapeModel(m_ho_ImageReduced,
			"auto",						//NumLevels
			HTuple(-30).TupleRad(),							//AngleStart
			HTuple(60).TupleRad(),		//AngleExtent
			0.0175,						//AngleStep
			"auto",						//Optimization
			"ignore_global_polarity",	//Metric
			"auto",						//Contrast
			"auto",						//MinContrast
			&m_hv_ModelID);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		MessageBox("CreateShapeModel Error.");
	}
}

//查找模型
void CDlgTapeIn::OnBnClickedBtnSearchModel()
{
	SetUpCameraLightOn();
	Sleep(10);
	CheckModel();
	SetUpCameraLightOff();
	if (m_bCameraOpenUp)
	{
		try
		{
			CloseFramegrabber(m_hv_AcqHandle_Up);
			m_bCameraOpenUp = FALSE;
			CDlgPosition *pWnd = (CDlgPosition*)GetParent();
			pWnd->InitCamera();
			pWnd->OpenSnapEx();
		}
		catch (HalconCpp::HException &HDevExpDefaultException)
		{
			MessageBox("没有需要关闭的相机");
		}
	}
}

//查找模型，返回值为找到模型与标准模型的偏差值
Position2 CDlgTapeIn::CheckModel()
{
	Position2 posOffset{0,0,0};

	try
	{
		if (m_bTapeIn)
			ReadShapeModel("c:\\S100\\Model\\TapeInModel.shm", &m_hv_ModelID);
		else
			ReadShapeModel("c:/S100/Model/TapeOutModel.shm", &m_hv_ModelID);
		GenRectangle1(&m_ho_ROI_Search_Domain, 462, 589, 566.5, 697.5);
		GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);
		GetImageSize(m_ho_Image, &m_hv_Width, &m_hv_Height);
		// Initialize window
		CRect rt;
		GetDlgItem(IDC_CAMERA_FRAME)->GetWindowRect(&rt);
		ScreenToClient(rt);
		m_Window.OpenWindow(rt.top,
			rt.left,
			rt.Width(),
			rt.Height(),
			(Hlong)m_hWnd,
			"visible",
			"");

		//m_Window.SetPart(0, 0, m_hv_Width, m_hv_Height);
		m_Window.SetLineWidth(1);
		m_Window.DispObj(m_ho_Image);
		ReduceDomain(m_ho_Image, m_ho_ROI_Search_Domain, &m_ho_ImageReducedSearch);

		FindShapeModel(m_ho_ImageReducedSearch,
			m_hv_ModelID,				//ModelID
			HTuple(-30).TupleRad(),							//AngleStart
			HTuple(60).TupleRad(),		//AngleExtent
			0.5,						//MinScore
			1,							//NumMatches
			0.5,						//MaxOverlap
			"least_squares",			//SubPixel
			0,							//NumLevels
			0.9,						//Greediness
			&m_hv_CheckRow,
			&m_hv_CheckColumn,
			&m_hv_CheckAngle,
			&m_hv_CheckScore);

		m_Window.SetColor("red");
		//	dev_display_shape_matching_results(m_ShapeModel.GetHandle(), "green", hv_Row, hv_Column, hv_Angle, 1, 1, 0);
		HTuple hv_AngleOffset, hv_ColumnOffset, hv_RowOffset;
		hv_AngleOffset = m_hv_CheckAngle - m_hv_Angle;
		hv_ColumnOffset = m_hv_CheckColumn - m_hv_Column;
		hv_RowOffset = m_hv_CheckRow - m_hv_Row;
		double a = hv_AngleOffset;
		double x = hv_ColumnOffset;
		double y = hv_RowOffset;
		double s = m_hv_CheckScore;

		CString str;
		m_Window.SetTposition(m_hv_Height - 400, 50);
		str.Format("匹配度:%4.2f", s);
		m_Window.WriteString(str);

		m_Window.SetTposition(m_hv_Height - 300, 50);
		str.Format("Offset Of Angle:%5.2f", a);
		m_Window.WriteString(str);

		m_Window.SetTposition(m_hv_Height - 200, 50);
		str.Format("Offset Of X:%5.2f", x);
		m_Window.WriteString(str);

		m_Window.SetTposition(m_hv_Height - 100, 50);
		str.Format("Offset Of Y:%5.2f", y);
		m_Window.WriteString(str);

		m_Window.SetColor("green");
		m_Window.SetLineWidth(1);
		m_Window.DispCross(m_hv_CheckRow, m_hv_CheckColumn, 1000, a);

		//	m_Window.DumpWindow("bmp", "/dump_window/halcon_dump00");
		posOffset.x = x;
		posOffset.y = y;
		posOffset.phi = a;

		m_nOffsetX = x;
		m_nOffsetY = y;
		m_nOffsetAngle = a;
		UpdateData(FALSE);
	}
	catch (HalconCpp::HException &HDevExpDefaultException)
	{
		MessageBox("FindShapeModel Error.");
	}
	return posOffset;
}


