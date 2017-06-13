// DlgPosition.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgPosition.h"
#include "BurningPort.h"
#include ".\MotionInclude\Cmmsdk.h"
#include ".\MotionInclude\CmmsdkDef.h"
#include "MotionAct.h"
#include "HalconCpp.h"
#include "IoSd404.h"
#include <fstream>
//#include <opencv2/opencv.hpp>  

//using namespace cv;
#include "sendef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgPosition::CDlgPosition(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgPosition::IDD, pParent)
    , m_strCameraNumber(_T(""))
    , m_radioRotateAngel(0)
    , m_radioNozzleNum(0)
{

    m_bSave1 = false;
    m_bSave2 = false;
    m_bOpenCCD = FALSE;
    m_bStartCCD = FALSE;

    m_pBmpInfo = NULL;
    m_pRawBuffer = NULL;
    m_nXLength = 200;
    m_nYLength = 200;
    for (int i = 0; i < 256; i++)
    {
        m_pLutR[i] = i;
        m_pLutG[i] = i;
        m_pLutB[i] = i;
    }
#if CAMERA_DAHENG_1351
    m_nWidth = 400;
    m_nHeight = 400;
    m_XStart = (640 - m_nWidth) / 2;
    m_YStart = (512 - m_nHeight) / 2;
    HVSTATUS status = STATUS_OK;
    //	����������� 1
    status = BeginHVDevice(1, &m_hhv);
    //	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��
    if (status == STATUS_OK)
        m_bCCDReady = TRUE;
    else
        m_bCCDReady = FALSE;
    HV_VERIFY(status);
#endif // CAMERA_DAHENG_1351
#if CAMERA_DAHENG_MER
    m_nWidth = 1280;
    m_nHeight = 1024;
    //m_nWidth = 2592;
    //m_nHeight = 1944;
    m_XStart = 0;//(640 - m_nWidth)/2;
    m_YStart = 0;//(512 - m_nHeight)/2;
    m_hDevice = NULL;
    m_stOpenParam.pszContent = "1";
    m_nDeviceNum = 0;
#endif // CAMERA_DAHENG_MER

    m_bRect = false;
    m_nXRange = 0;
    m_nYRange = 0;
    m_nZRange = 0;

    m_brush.CreateSolidBrush(RGB(195, 248, 255));
    m_bTimer = FALSE;
    m_bTimerX = FALSE;
    m_bTimerY = FALSE;
    m_bTimerZ = FALSE;
    m_bCheckPoint = true;
}

void CDlgPosition::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgPosition)
    DDX_Control(pDX, IDC_POS_YSUB, m_btnSubY);
    DDX_Control(pDX, IDC_POS_YADD, m_btnAddY);
    DDX_Control(pDX, IDC_POS_XADD, m_btnAddX);
    DDX_Control(pDX, IDC_POS_XSUB, m_btnSubX);
    DDX_Control(pDX, IDC_POS_SLIDERY, m_ySlider);
    DDX_Control(pDX, IDC_POS_SLIDERX, m_xSlider);
    DDX_Control(pDX, IDC_POSBTN_ZUP, m_btnZdown);
    DDX_Control(pDX, IDC_POSBTN_ZDOWN, m_btnZDown);
    DDX_Control(pDX, IDC_POSBTN_YTOP, m_btnUp);
    DDX_Control(pDX, IDC_POSBTN_YBOTTOM, m_btnDown);
    DDX_Control(pDX, IDC_POSBTN_XRIGHT, m_btnRight);
    DDX_Control(pDX, IDC_POSBTN_XLEFT, m_btnLeft);
    //}}AFX_DATA_MAP

    DDX_Radio(pDX, IDC_RADIO_ROTATE_ANGLE, m_radioRotateAngel);
    DDX_Radio(pDX, IDC_RADIO_NOZZLE_NUM, m_radioNozzleNum);
    DDX_Control(pDX, IDC_BTN_COUNTERCLOCKWISE, m_btnCounter);
    DDX_Control(pDX, IDC_BTN_ROTATE_CLOCKWISE, m_btnClock);
}


BEGIN_MESSAGE_MAP(CDlgPosition, CDialog)
    //{{AFX_MSG_MAP(CDlgPosition)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_POS_TRAY, OnPosTray)
    ON_BN_CLICKED(IDC_POS_BASE, OnPosBase)
    ON_BN_CLICKED(IDC_POS_NG, OnPosNg)
    ON_BN_CLICKED(IDC_POS_SOCKET, OnPosSocket)
    ON_BN_CLICKED(IDC_POSRADIO_XY1, OnPosradioXy1)
    ON_BN_CLICKED(IDC_POSRADIO_XY2, OnPosradioXy2)
    ON_BN_CLICKED(IDC_POSRADIO_XY3, OnPosradioXy3)
    ON_BN_CLICKED(IDC_POSRADIO_XY4, OnPosradioXy4)
    ON_BN_CLICKED(IDC_POSRADIO_XY5, OnPosradioXy5)
    ON_BN_CLICKED(IDC_POSRADIO_XY6, OnPosradioXy6)
    ON_BN_CLICKED(IDC_POSBTN_XLEFT, OnPosbtnXleft)
    ON_BN_CLICKED(IDC_POSBTN_XRIGHT, OnPosbtnXright)
    ON_BN_CLICKED(IDC_POSBTN_YTOP, OnPosbtnYtop)
    ON_BN_CLICKED(IDC_POSBTN_YBOTTOM, OnPosbtnYbottom)
    ON_BN_CLICKED(IDC_BTN_HOME, OnHome)
    ON_BN_CLICKED(IDC_POSRADIO_Z1, OnPosradioZ1)
    ON_BN_CLICKED(IDC_POSRADIO_Z2, OnPosradioZ2)
    ON_BN_CLICKED(IDC_POSRADIO_Z3, OnPosradioZ3)
    ON_BN_CLICKED(IDC_POSRADIO_Z4, OnPosradioZ4)
    ON_BN_CLICKED(IDC_POSBTN_ZUP, OnPosbtnZup)
    ON_BN_CLICKED(IDC_POSBTN_ZDOWN, OnPosbtnZdown)
    ON_BN_CLICKED(IDC_CHKPOINT_CCD, OnGotoCheckPoint)
    ON_BN_CLICKED(IDC_CHKPOINT_SAVECCD, OnChkpointSaveccd)
    ON_BN_CLICKED(IDC_CHKPOINT_NOZZLE, OnChkpointNozzle)
    //	ON_BN_CLICKED(IDC_CHKPOINT_GETOFFSET, OnChkpointGetoffset)
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_POS_SLIDERX, OnCustomdrawPosSliderx)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_POS_SLIDERY, OnCustomdrawPosSlidery)
    ON_BN_CLICKED(IDC_POS_XADD, OnPosXadd)
    ON_BN_CLICKED(IDC_POS_XSUB, OnPosXsub)
    ON_BN_CLICKED(IDC_POS_YSUB, OnPosYsub)
    ON_BN_CLICKED(IDC_POS_YADD, OnPosYadd)
    ON_WM_LBUTTONDBLCLK()
    ON_BN_CLICKED(IDC_POS_TAPE, OnPosTape)
    ON_BN_CLICKED(IDC_POS_SUPPLY, OnPosSupply)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_POS_MODEL, &CDlgPosition::OnBnClickedPosModel)
    ON_BN_CLICKED(IDC_BTN_NEXT_CAMERA, &CDlgPosition::OnBnClickedBtnNextCamera)
    ON_BN_CLICKED(IDC_BTN_SWITCH_CAMERA, &CDlgPosition::OnBnClickedBtnSwitchCamera)
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_GO_HOME, &CDlgPosition::OnBnClickedBtnGoHome)
    ON_BN_CLICKED(IDC_CHKPOINT_NOZZLE2, &CDlgPosition::OnBnClickedChkpointNozzle2)
    ON_BN_CLICKED(IDC_CHKPOINT_GETOFFSET2, &CDlgPosition::OnBnClickedChkpointGetoffset2)
    ON_BN_CLICKED(IDC_CHKPOINT_NOZZLE3, &CDlgPosition::OnBnClickedChkpointNozzle3)
    ON_BN_CLICKED(IDC_CHKPOINT_GETOFFSET3, &CDlgPosition::OnBnClickedChkpointGetoffset3)
    ON_BN_CLICKED(IDC_CHKPOINT_NOZZLE4, &CDlgPosition::OnBnClickedChkpointNozzle4)
    ON_BN_CLICKED(IDC_CHKPOINT_GETOFFSET4, &CDlgPosition::OnBnClickedChkpointGetoffset4)
    ON_BN_CLICKED(IDC_BTN_NOZZLE1_UPDOWN, &CDlgPosition::OnBnClickedBtnNozzle1Updown)
    ON_BN_CLICKED(IDC_BTN_NOZZLE2_UPDOWN, &CDlgPosition::OnBnClickedBtnNozzle2Updown)
    ON_BN_CLICKED(IDC_BTN_NOZZLE3_UPDOWN, &CDlgPosition::OnBnClickedBtnNozzle3Updown)
    ON_BN_CLICKED(IDC_BTN_NOZZLE4_UPDOWN, &CDlgPosition::OnBnClickedBtnNozzle4Updown)
    ON_BN_CLICKED(IDC_CHECK_VACUMM1, &CDlgPosition::OnBnClickedCheckVacumm1)
    ON_BN_CLICKED(IDC_CHECK_VACUMM2, &CDlgPosition::OnBnClickedCheckVacumm2)
    ON_BN_CLICKED(IDC_CHECK_VACUMM3, &CDlgPosition::OnBnClickedCheckVacumm3)
    ON_BN_CLICKED(IDC_CHECK_VACUMM4, &CDlgPosition::OnBnClickedCheckVacumm4)
    ON_BN_CLICKED(IDC_CHECK_BLOW1, &CDlgPosition::OnBnClickedCheckBlow1)
    ON_BN_CLICKED(IDC_CHECK_BLOW2, &CDlgPosition::OnBnClickedCheckBlow2)
    ON_BN_CLICKED(IDC_CHECK_BLOW3, &CDlgPosition::OnBnClickedCheckBlow3)
    ON_BN_CLICKED(IDC_CHECK_BLOW4, &CDlgPosition::OnBnClickedCheckBlow4)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER0, &CDlgPosition::OnBnClickedCheckCylinder0)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER1, &CDlgPosition::OnBnClickedCheckCylinder1)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER2, &CDlgPosition::OnBnClickedCheckCylinder2)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER3, &CDlgPosition::OnBnClickedCheckCylinder3)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER4, &CDlgPosition::OnBnClickedCheckCylinder4)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER5, &CDlgPosition::OnBnClickedCheckCylinder5)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER6, &CDlgPosition::OnBnClickedCheckCylinder6)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER7, &CDlgPosition::OnBnClickedCheckCylinder7)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER8, &CDlgPosition::OnBnClickedCheckCylinder8)
    ON_BN_CLICKED(IDC_CHECK_CYLINDER9, &CDlgPosition::OnBnClickedCheckCylinder9)
    ON_BN_CLICKED(IDC_CHECK_UP_LIGHT, &CDlgPosition::OnBnClickedCheckUpLight)
    ON_BN_CLICKED(IDC_CHECK_DOWN_LIGHT, &CDlgPosition::OnBnClickedCheckDownLight)
    ON_BN_CLICKED(IDC_CHKPOINT_GETOFFSET, &CDlgPosition::OnBnClickedChkpointGetoffset)
    ON_BN_CLICKED(IDC_BTN_COUNTERCLOCKWISE, &CDlgPosition::OnBnClickedBtnCounterclockwise)
    ON_BN_CLICKED(IDC_BTN_ROTATE_CLOCKWISE, &CDlgPosition::OnBnClickedBtnRotateClockwise)
    ON_BN_CLICKED(IDCANCEL, &CDlgPosition::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE1, &CDlgPosition::OnBnClickedRadioRotateAngle1)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE91, &CDlgPosition::OnBnClickedRadioRotateAngle91)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLEN91, &CDlgPosition::OnBnClickedRadioRotateAnglen91)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE181, &CDlgPosition::OnBnClickedRadioRotateAngle181)
    ON_BN_CLICKED(IDC_BTN_ROTATE_TEST, &CDlgPosition::OnBnClickedBtnRotateTest)
#if CAMERA_DAHENG_1351
    ON_MESSAGE(WM_SNAP_CHANGE, OnSnapChange)
#endif
    ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgPosition::OnBnClickedButtonCreate)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDlgPosition::OnBnClickedButtonSearch)
    ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &CDlgPosition::OnBnClickedButtonSearch2)
END_MESSAGE_MAP()

#if CAMERA_DAHENG_1351
const HV_RESOLUTION Resolution = RES_MODE1;
const HV_SNAP_MODE SnapMode = CONTINUATION;
const HV_BAYER_LAYOUT Layout = BAYER_GR;
const HV_BAYER_CONVERT_TYPE ConvertType = BAYER2RGB_NEIGHBOUR;
const long Gain = 8;
const long ExposureTint_Upper = 60;
const long ExposureTint_Lower = 1000;
const long ADCLevel = ADC_LEVEL2;
void CDlgPosition::InitCamera()
{
    HVSetResolution(m_hhv, Resolution);

    //	�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)
    HVSetSnapMode(m_hhv, SnapMode);

    //  ���ø�������������
    for (int i = 0; i < 4; i++)
    {
        HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
    }

    //	�����ع�ʱ��
    SetExposureTime(m_nWidth, ExposureTint_Upper, ExposureTint_Lower);

    //  ����ADC�ļ���
    HVADCControl(m_hhv, ADC_BITS, ADCLevel);


    HVSetOutputWindow(m_hhv, m_XStart, m_YStart, m_nWidth, m_nHeight);

    //	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
    m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
    //	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
    m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    //	ͼ���ȣ�һ��Ϊ������ڿ��
    m_pBmpInfo->bmiHeader.biWidth = m_nWidth;
    //	ͼ���ȣ�һ��Ϊ������ڸ߶�
    m_pBmpInfo->bmiHeader.biHeight = -m_nHeight;


    m_pBmpInfo->bmiHeader.biPlanes = 1;
    m_pBmpInfo->bmiHeader.biBitCount = 8;
    m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
    m_pBmpInfo->bmiHeader.biSizeImage = 0;
    m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biClrUsed = 0;
    m_pBmpInfo->bmiHeader.biClrImportant = 0;

    for (int i = 0; i < 256; i++)
    {
        m_pBmpInfo->bmiColors[i].rgbRed = i;
        m_pBmpInfo->bmiColors[i].rgbGreen = i;
        m_pBmpInfo->bmiColors[i].rgbBlue = i;
        m_pBmpInfo->bmiColors[i].rgbReserved = 0;
    }
    m_pRawBuffer = new BYTE[m_nWidth * m_nHeight];
    ASSERT(m_pRawBuffer);

    //	m_pImageBuffer = new BYTE[m_nWidth * m_nHeight * 3];
    //	ASSERT(m_pImageBuffer);
}
void CDlgPosition::InitCCD()
{
    HVSTATUS status = STATUS_OK;
    //	����������� 1
    status = BeginHVDevice(1, &m_hhv);
    //	���麯��ִ��״̬�����ʧ�ܣ��򷵻ش���״̬��Ϣ��
    HV_VERIFY(status);
    HVSetResolution(m_hhv, Resolution);

    //	�ɼ�ģʽ������ CONTINUATION(����)��TRIGGER(�ⴥ��)
    HVSetSnapMode(m_hhv, SnapMode);

    //  ���ø�������������
    for (int i = 0; i < 4; i++)
    {
        HVAGCControl(m_hhv, RED_CHANNEL + i, Gain);
    }

    //	�����ع�ʱ��
    SetExposureTime(m_nWidth, ExposureTint_Upper, ExposureTint_Lower);

    //  ����ADC�ļ���
    HVADCControl(m_hhv, ADC_BITS, ADCLevel);


    HVSetOutputWindow(m_hhv, m_XStart, m_YStart, m_nWidth, m_nHeight);
}

void CDlgPosition::SetExposureTime(int nWindWidth, long lTintUpper, long lTintLower)
{
#define  MY_ZERO 0.000000001
    int size = sizeof(HVTYPE);
    HVTYPE type;
    HVGetDeviceInfo(m_hhv, DESC_DEVICE_TYPE, &type, &size);

    //When outputwindow changes, change the exposure 
    //��ο��ع�ϵ��ת����ʽ
    long lClockFreq = 24000000;
    int nOutputWid = nWindWidth;
    double dExposure = 0.0;
    double dTint = max((double)lTintUpper / (double)lTintLower, MY_ZERO);
    if (type == HV1300UCTYPE || type == HV1301UCTYPE)
    {
        long lTb = 0;
        dExposure = (dTint* lClockFreq + 180.0) / ((double)nOutputWid + 244.0 + lTb);
    }
    else
    {
        long lTb = 0;
        dExposure = (dTint* lClockFreq + 180.0) / ((double)nOutputWid + 305.0 + lTb) + 1;
    }

    if (dExposure > 16383)
        dExposure = 16383;
    dExposure = 1400;
    //	HVAECControl(m_hhv, AEC_SHUTTER_UNIT,SHUTTER_MS);
    HVAECControl(m_hhv, AEC_EXPOSURE_TIME, (long)dExposure);
    //	HVAECControl(m_hhv, AEC_SHUTTER_SPEED , (long)dExposure);

}
void CDlgPosition::OpenSnapEx()
{
    HVSTATUS status = STATUS_OK;
    status = HVOpenSnap(m_hhv, SnapThreadCallback, m_hWnd);
    HV_VERIFY(status);
    if (HV_SUCCESS(status))
    {
        m_bOpenCCD = TRUE;		//��־�Ѿ���Snap����
    }
}
void CDlgPosition::StartSnapEx()
{
    if (m_bStartCCD)
        return;
    HVSTATUS status = STATUS_OK;
    BYTE *ppBuf[1];
    ppBuf[0] = m_pRawBuffer;
    status = HVStartSnap(m_hhv, ppBuf, 1);

    HV_VERIFY(status);
    if (HV_SUCCESS(status))
    {
        m_bStartCCD = TRUE;
    }
}
void CDlgPosition::StopSnapEx()
{
    if (!m_bStartCCD)
        return;
    HVSTATUS status = STATUS_OK;
    status = HVStopSnap(m_hhv);
    HV_VERIFY(status);
    if (HV_SUCCESS(status))
    {
        m_bStartCCD = FALSE;
    }
}
void CDlgPosition::CloseSnapEx()
{
    if (!m_bOpenCCD)
        return;
    HVSTATUS status = STATUS_OK;
    status = HVCloseSnap(m_hhv);
    HV_VERIFY(status);

    if (HV_SUCCESS(status))
    {
        m_bOpenCCD = FALSE;
        m_bStartCCD = FALSE;
    }
}

void CDlgPosition::CloseCamera()
{
    if (m_bOpenCCD)
    {
        HVStopSnap(m_hhv);
        HVCloseSnap(m_hhv);
    }

    HVSTATUS status = EndHVDevice(m_hhv);
    HV_VERIFY(status);

    //	����ͼ�񻺳���
    delete[]m_pRawBuffer;
    m_pRawBuffer = NULL;
}

int CALLBACK CDlgPosition::SnapThreadCallback(HV_SNAP_INFO *pInfo)
{
    HWND hwnd = (HWND)(pInfo->pParam);

    ::SendMessage(hwnd, WM_SNAP_CHANGE, 0, 0);

    return 1;
}
LRESULT CDlgPosition::OnSnapChange(WPARAM wParam, LPARAM lParam)
{
    HVSTATUS status = STATUS_OK;
    CRect rt;//(0,0,400,400);
    CWnd *pWnd = GetDlgItem(IDC_CAMERA_FRAME);
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);
    //	GetClientRect(&rt);
    CDC *pDC = GetDC();		//�õ�VIEW��DC

    //	��ԭʼͼ�����ݽ���Bayerת����ת����Ϊ24λ��
    //ͬʱ��ԭʼ���ݽ������·�ת
    //	ConvertBayer2Rgb(m_pImageBuffer,m_pRawBuffer,m_nWidth,m_nHeight,ConvertType,m_pLutR,m_pLutG,m_pLutB,true,Layout);


    //����ͼ�ͻ�����ʾͼ��
    StretchDIBits(pDC->GetSafeHdc(),
        rt.left,
        rt.top,
        rt.Width(),//m_nWidth,					//��ʾ���ڿ��
        rt.Height(),//m_nHeight,					//��ʾ���ڸ߶�
        0,
        0,
        m_nWidth,					//ͼ����
        m_nHeight,					//ͼ��߶�
        m_pRawBuffer,			//ͼ�񻺳���
        m_pBmpInfo,				//BMPͼ��������Ϣ
        DIB_RGB_COLORS,
        SRCCOPY
    );
    POINT pt;
    pt.x = rt.left + rt.Width() / 2;
    pt.y = rt.top + rt.Height() / 2;
    //	if(m_bRect == false)
    DrawCross(pDC, 400, 400, pt);//m_nXLength,m_nYLength,pt);
    //	else
    DrawRectangle(pDC, m_nXLength, m_nYLength, pt);
    ReleaseDC(pDC);

    return 1;
}
void CDlgPosition::DrawCross(CDC *pDC, int xLength, int yLength, POINT center)
{
    CPen newPen, *oldPen;
    newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    oldPen = pDC->SelectObject(&newPen);
    pDC->MoveTo(center.x - xLength / 2, center.y);
    pDC->LineTo(center.x + xLength / 2, center.y);
    pDC->MoveTo(center.x, center.y - yLength / 2);
    pDC->LineTo(center.x, center.y + yLength / 2);
    pDC->SelectObject(oldPen);
    newPen.DeleteObject();
}

void CDlgPosition::DrawRectangle(CDC *pDC, int xLength, int yLength, POINT center)
{
    CPen newPen, *oldPen;
    newPen.CreatePen(PS_SOLID, 1, RGB(192, 0, 192));
    oldPen = pDC->SelectObject(&newPen);
    pDC->MoveTo(center.x - xLength / 2, center.y - yLength / 2);
    pDC->LineTo(center.x + xLength / 2, center.y - yLength / 2);
    pDC->MoveTo(center.x + xLength / 2, center.y - yLength / 2);
    pDC->LineTo(center.x + xLength / 2, center.y + yLength / 2);
    pDC->MoveTo(center.x + xLength / 2, center.y + yLength / 2);
    pDC->LineTo(center.x - xLength / 2, center.y + yLength / 2);
    pDC->MoveTo(center.x - xLength / 2, center.y + yLength / 2);
    pDC->LineTo(center.x - xLength / 2, center.y - yLength / 2);

    pDC->SelectObject(oldPen);
    newPen.DeleteObject();
}

#endif

#if CAMERA_DAHENG_MER
//��ʼ�����
void CDlgPosition::InitCamera()
{
    //----------------------------------------------------------------
    //�ж���Ҫ�������е��ĸ����
    if (m_stOpenParam.pszContent == "1")
    {
        SetUpCameraLightOn();
        SetDownCameraLightOff();
    }

    else if (m_stOpenParam.pszContent == "2")
    {
        SetDownCameraLightOn();
        SetUpCameraLightOff();
    }
    //--------------------------------------------------------------*/

    m_hDevice = NULL;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

    //----------------------------------------------------------------
    //��ʼ�����
    // ��ʼ����
    emStatus = GXInitLib();
    if (emStatus != GX_STATUS_SUCCESS)
        AfxMessageBox("Camera lib open error!");

    // ö���豸����
    emStatus = GXUpdateDeviceList(&m_nDeviceNum, 1000);
    GX_VERIFY(emStatus);

    // �жϵ�ǰ�����豸����
    if (m_nDeviceNum <= 0)
    {
        //AfxMessageBox("δ��⵽����豸!");
        return;
    }

    // �ڴ��豸֮ǰ�Ծ�������жϣ����Ա�֤����ڳ�ʼ������������ٴδ�
    if (m_hDevice != NULL)
    {
        emStatus = GXCloseDevice(m_hDevice);
        GX_VERIFY(emStatus);
        m_hDevice = NULL;
    }

    // ��ö���б��еĵ�1̨�豸
    m_stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    m_stOpenParam.openMode = GX_OPEN_INDEX;

    emStatus = GXOpenDevice(&m_stOpenParam, &m_hDevice);
    GX_VERIFY(emStatus);
    m_bOpenCCD = TRUE;

    emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);

    emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
    double gain = 9;
    emStatus = GXSetFloat(m_hDevice, GX_FLOAT_GAIN, gain);

    double dShutterValue = EXPOSITION;
    emStatus = GXSetFloat(m_hDevice, GX_FLOAT_EXPOSURE_TIME, dShutterValue);
    // ��ʼ������

    //	m_pBmpInfo��ָ��m_chBmpBuf���������û������Լ�����BTIMAPINFO������	
    m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
    //	��ʼ��BITMAPINFO �ṹ���˽ṹ�ڱ���bmp�ļ�����ʾ�ɼ�ͼ��ʱʹ��
    m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    //	ͼ���ȣ�һ��Ϊ������ڿ��
    m_pBmpInfo->bmiHeader.biWidth = m_nWidth;
    //	ͼ���ȣ�һ��Ϊ������ڸ߶�
    m_pBmpInfo->bmiHeader.biHeight = -m_nHeight;

    m_pBmpInfo->bmiHeader.biPlanes = 1;
    m_pBmpInfo->bmiHeader.biBitCount = 8;
    m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
    m_pBmpInfo->bmiHeader.biSizeImage = 0;
    m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biClrUsed = 0;
    m_pBmpInfo->bmiHeader.biClrImportant = 0;

    for (int i = 0; i < 256; i++)
    {
        m_pBmpInfo->bmiColors[i].rgbRed = i;
        m_pBmpInfo->bmiColors[i].rgbGreen = i;
        m_pBmpInfo->bmiColors[i].rgbBlue = i;
        m_pBmpInfo->bmiColors[i].rgbReserved = 0;
    }
    m_pRawBuffer = new BYTE[m_nWidth * m_nHeight];
    ASSERT(m_pRawBuffer);

    //	m_pImageBuffer = new BYTE[m_nWidth * m_nHeight * 3];
    //	ASSERT(m_pImageBuffer);
}

//��ʼ�ɼ�ͼ��
void CDlgPosition::OpenSnapEx()
{
    GX_STATUS     emStatus = GX_STATUS_SUCCESS;
    //ע��ص�����
    emStatus = GXRegisterCaptureCallback(m_hDevice, this, OnFrameCallbackFun);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        //	UnPrepareForShowImg();
        //	ShowErrorString(emStatus);
        return;
    }

    //����ʼ�ɼ�����
    emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        //	UnPrepareForShowImg();
        //	ShowErrorString(emStatus);
        return;
    }
}
void CDlgPosition::StartSnapEx()
{

}
void CDlgPosition::StopSnapEx()
{

}
//ֹͣ�ɼ�ͼ��
void CDlgPosition::CloseSnapEx()
{
    if (!m_bOpenCCD)
        return;
    GX_STATUS emStatus = GX_STATUS_SUCCESS;

    //����ֹͣ�ɼ�����
    emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
    GX_VERIFY(emStatus);

    //ע���ص�
    emStatus = GXUnregisterCaptureCallback(m_hDevice);
    GX_VERIFY(emStatus);
    m_bOpenCCD = FALSE;
    m_bStartCCD = FALSE;

}

//������������еĴ��������
void CDlgPosition::ShowErrorString(GX_STATUS emErrorStatus)
{
    char      *pchErrorInfo = NULL;
    size_t    nSize = 0;
    GX_STATUS emStatus = GX_STATUS_ERROR;

    // ��ȡ����������Ϣ����
    emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
    pchErrorInfo = new char[nSize];
    if (pchErrorInfo == NULL)
        return;

    // ��ȡ������Ϣ����
    emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);

    // ��ʾ����������Ϣ
    if (emStatus != GX_STATUS_SUCCESS)
        AfxMessageBox("GXGetLastError�ӿڵ���ʧ�ܣ�");
    else
        AfxMessageBox((LPCTSTR)pchErrorInfo);

    // �ͷ���Դ
    if (pchErrorInfo != NULL)
    {
        delete[]pchErrorInfo;
        pchErrorInfo = NULL;
    }
}

//����ɼ�ͼ��Ļص�����
void __stdcall CDlgPosition::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
    CDlgPosition *pDlg = (CDlgPosition*)(pFrame->pUserParam);

    if (pFrame->status == 0)
    {
        memcpy(pDlg->m_pRawBuffer, pFrame->pImgBuf, pFrame->nImgSize);
        pDlg->DrawImg();
    }
}

//������л���ͼ��
void CDlgPosition::DrawImg()
{
    //----------------------------------------------------------------
    //
    CRect rt;
    CWnd *pWnd = GetDlgItem(IDC_CAMERA_FRAME);
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);

    // Ϊ��ͼ��׼��
    int nWndWidth = 0;
    int nWndHeight = 0;
    nWndWidth = rt.Width();
    nWndHeight = rt.Height();

    CDC *pDC = GetDC();
    // ������ø���䣬����ͼ�����ˮ��
    ::SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);
    ::StretchDIBits(pDC->GetSafeHdc(),
        rt.left,			//Ŀ��������Ͻ�λ�õ�X�����꣬�߼���λ	
        rt.top,
        nWndWidth,			//Ŀ����εĿ��
        nWndHeight,
        0,					//DIB��Դ���ε�X���꣬���ص�
        0,
        (int)m_nWidth,		//�����ص�ָ��DIB��Դ���εĿ��
        (int)m_nHeight,
        m_pRawBuffer,		//ָ��DIBλ��ָ��
        m_pBmpInfo,			//ָ��BITMAPINFO�ṹ��ָ�룬����DIB����Ϣ
        DIB_RGB_COLORS,
        SRCCOPY
    );

    POINT pt;
    pt.x = rt.left + rt.Width() / 2;
    pt.y = rt.top + rt.Height() / 2;
    //	if(m_bRect == false)
    DrawCross(pDC, nWndWidth, nWndHeight, pt);
    //	else
    DrawRectangle(pDC, m_nXLength, m_nYLength, pt);
    ReleaseDC(pDC);
}
void CDlgPosition::DrawCross(CDC *pDC, int xLength, int yLength, POINT center)
{
    CPen newPen, *oldPen;
    newPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    oldPen = pDC->SelectObject(&newPen);
    pDC->MoveTo(center.x - xLength / 2, center.y);
    pDC->LineTo(center.x + xLength / 2, center.y);
    pDC->MoveTo(center.x, center.y - yLength / 2);
    pDC->LineTo(center.x, center.y + yLength / 2);
    pDC->SelectObject(oldPen);
    newPen.DeleteObject();
}

void CDlgPosition::DrawRectangle(CDC *pDC, int xLength, int yLength, POINT center)
{
    CPen newPen, *oldPen;
    newPen.CreatePen(PS_SOLID, 1, RGB(192, 0, 192));
    oldPen = pDC->SelectObject(&newPen);
    pDC->MoveTo(center.x - xLength / 2, center.y - yLength / 2);
    pDC->LineTo(center.x + xLength / 2, center.y - yLength / 2);
    pDC->MoveTo(center.x + xLength / 2, center.y - yLength / 2);
    pDC->LineTo(center.x + xLength / 2, center.y + yLength / 2);
    pDC->MoveTo(center.x + xLength / 2, center.y + yLength / 2);
    pDC->LineTo(center.x - xLength / 2, center.y + yLength / 2);
    pDC->MoveTo(center.x - xLength / 2, center.y + yLength / 2);
    pDC->LineTo(center.x - xLength / 2, center.y - yLength / 2);

    pDC->SelectObject(oldPen);
    newPen.DeleteObject();
}
//�ر�������ͷ���Դ
void CDlgPosition::CloseCamera()
{
    if (m_bOpenCCD)
        CloseSnapEx();

    //����ͼ�񻺳���
    if (m_pRawBuffer)
    {
        delete[]m_pRawBuffer;
        m_pRawBuffer = NULL;
    }

    GXCloseDevice(m_hDevice);
    GXCloseLib();
}

#endif // CAMERA_MER


//��ʼ���Ի���
BOOL CDlgPosition::OnInitDialog()
{
    CDialog::OnInitDialog();
    //----------------------------------------------------------------
    //��ʼ��������
    m_xSlider.SetRange(0, 400);//pFrm->m_nWidth);
    m_xSlider.SetPos(50);//m_nXLength);
    m_xSlider.SetTicFreq(20);

    m_ySlider.SetRange(0, 400);//pFrm->m_nHeight-10);
    m_ySlider.SetPos(350);
    m_ySlider.SetTicFreq(20);
    //--------------------------------------------------------------*/
    //----------------------------------------------------------------
    //��ʼ�����
#if CAMERA_DAHENG_1351
    if (m_bCCDReady)
    {
        InitCamera();
        OpenSnapEx();
        StartSnapEx();
    }
#endif // CAMERA_1351

#if CAMERA_DAHENG_MER
    CloseCamera();
    InitCamera();
    OpenSnapEx();
#endif // CAMERA_MER

    CRect rt;
    GetDlgItem(IDC_POS_XSUB)->GetWindowRect(rt);
    ScreenToClient(rt);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //��ʼ��������
    m_dlgTray.Create(IDD_POSITION_TRAY, this);
    m_dlgSupply.Create(IDD_POSITION_SUPPLY, this);
    m_dlgReject.Create(IDD_POSITION_NGBIN, this);
    m_dlgSocket.Create(IDD_POSITION_SOCKET, this);
    m_dlgTapeIn.Create(IDD_POSITION_TAPEIN, this);
    m_dlgModelCheck.Create(IDD_POSITION_MODEL, this);
    m_dlgUnderCamera.Create(IDD_POSITION_UNDER_CAMERA, this);

    //����λ�þ���
    CRect rtFeed;
    CWnd *pWnd = GetDlgItem(IDC_POS_FRAME);
    pWnd->GetWindowRect(rtFeed);
    ScreenToClient(rtFeed);
    CRect drt;

    //Tray�̴���
    m_dlgTray.GetClientRect(drt);
    int x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    int y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgTray.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgTray.ShowWindow(SW_HIDE);

    //Socketģ��ƥ�䴰��
    m_dlgModelCheck.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgModelCheck.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);

    //�����̴���
    m_dlgSupply.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgSupply.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgSupply.ShowWindow(SW_HIDE);

    //�����̴���
    m_dlgReject.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgReject.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgReject.ShowWindow(SW_HIDE);

    //���������
    m_dlgSocket.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgSocket.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgSocket.ShowWindow(SW_HIDE);

    //���������
    m_dlgTapeIn.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgTapeIn.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);

    //���������
    m_dlgUnderCamera.GetClientRect(drt);
    x = rtFeed.left + (rtFeed.Width() - drt.Width()) / 2;
    y = rtFeed.top + (rtFeed.Height() - drt.Height()) / 2;
    m_dlgUnderCamera.SetWindowPos(NULL, x, y, drt.Width(), drt.Height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //��ʼ����������up/down��������
    m_btnLeft.SetIcon(IDI_LEFT);
    m_btnLeft.SetFlat();

    m_btnRight.SetIcon(IDI_RIGHT);
    m_btnRight.SetFlat();

    m_btnDown.SetIcon(IDI_DOWN);
    m_btnDown.SetFlat();

    m_btnUp.SetIcon(IDI_UP);
    m_btnUp.SetFlat();

    m_btnZDown.SetIcon(IDI_DOWN);
    m_btnZDown.SetFlat();

    m_btnZdown.SetIcon(IDI_UP);
    m_btnZdown.SetFlat();

    m_btnCounter.SetIcon(IDI_COUNTER_ROTATE);
    m_btnCounter.SetFlat();

    m_btnClock.SetIcon(IDI_CLOCK_ROTATE);
    m_btnClock.SetFlat();
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //��ʼ���г̵�λ
    CButton *pBtn = (CButton*)GetDlgItem(IDC_POSRADIO_XY1);
    pBtn->SetCheck(TRUE);

    pBtn = (CButton*)GetDlgItem(IDC_POSRADIO_Z1);
    pBtn->SetCheck(TRUE);

    m_nXRange = (LONG)(XGUIDE * 0.05f);
    m_nYRange = (LONG)(YGUIDE * 0.05f);
    //--------------------------------------------------------------*/
    //��ʼ����ת�Ƕ�Ϊ0
    pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE1);
    pBtn->SetCheck(TRUE);


    //----------------------------------------------------------------
    //���ù��������߼Ӽ���ť���
    tButtonStyle tStyle;
    m_tButtonStyle.GetButtonStyle(&tStyle);
    tStyle.m_tColorFace.m_tEnabled = RGB(236, 236, 236);
    tStyle.m_tColorBorder.m_tEnabled = RGB(255, 255, 255);

    tStyle.m_tColorFace.m_tClicked = RGB(168, 168, 168);
    tStyle.m_tColorBorder.m_tClicked = RGB(0, 0, 0);

    tStyle.m_tColorFace.m_tPressed = RGB(168, 168, 168);
    tStyle.m_tColorBorder.m_tPressed = RGB(0, 0, 0);

    m_tButtonStyle.SetButtonStyle(&tStyle);

    m_btnAddX.SetRoundButtonStyle(&m_tButtonStyle);
    m_btnAddY.SetRoundButtonStyle(&m_tButtonStyle);
    m_btnSubX.SetRoundButtonStyle(&m_tButtonStyle);
    m_btnSubY.SetRoundButtonStyle(&m_tButtonStyle);
    //--------------------------------------------------------------*/

    //--------------------------------------------------------------*/
    //�������װ�ť����
    if (!CheckNozzleUpStatus(0))
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    else
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");

    if (!CheckNozzleUpStatus(1))
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    else
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");

    if (!CheckNozzleUpStatus(2))
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    else
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");

    if (!CheckNozzleUpStatus(3))
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    else
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");

    //SetTimer(0, 100, NULL);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
//��ʾ��׼��Ի���
void CDlgPosition::ShowBasePoint(BOOL bShow)
{
    CWnd *pWnd = GetDlgItem(IDC_STATIC_DATAUM_1);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);
    pWnd = GetDlgItem(IDC_STATIC_DATAUM_3);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);
    pWnd = GetDlgItem(IDC_STATIC_DATAUM_4);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_STATIC_DATAUM_5);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_STATIC_DATAUM_6);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_X);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_Y);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_OFFSETX);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_OFFSETY);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_CCD);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);
    pWnd = GetDlgItem(IDC_CHKPOINT_SAVECCD);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_NOZZLE);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);
    pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_NOZZLE2);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);
    pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET2);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_NOZZLE3);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET3);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_NOZZLE4);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

    pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET4);
    if (bShow)
        pWnd->ShowWindow(SW_SHOW);
    else
        pWnd->ShowWindow(SW_HIDE);

}

//��ʾ/���ض�λ�еĸ����Ի���
void CDlgPosition::OnPosBase()
{
    // TODO: Add your control notification handler code here
    m_bCheckPoint = true;
    ShowBasePoint(TRUE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}

void CDlgPosition::OnPosTray()
{
    // TODO: Add your control notification handler code here
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_SHOW);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}
void CDlgPosition::OnPosTape()
{
    // TODO: Add your control notification handler code here
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_SHOW);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}


void CDlgPosition::OnPosNg()
{
    // TODO: Add your control notification handler code here
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_SHOW);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}
void CDlgPosition::OnPosSupply()
{
    // TODO: Add your control notification handler code here
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_SHOW);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}


void CDlgPosition::OnPosSocket()
{
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_SHOW);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}
void CDlgPosition::OnBnClickedPosModel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_SHOW);
    m_dlgUnderCamera.ShowWindow(SW_HIDE);
}


void CDlgPosition::OnBnClickedBtnNextCamera()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    m_bTimer = FALSE;
    m_bCheckPoint = false;
    ShowBasePoint(FALSE);
    m_dlgSupply.ShowWindow(SW_HIDE);
    m_dlgTray.ShowWindow(SW_HIDE);
    m_dlgReject.ShowWindow(SW_HIDE);
    m_dlgSocket.ShowWindow(SW_HIDE);
    m_dlgTapeIn.ShowWindow(SW_HIDE);
    m_dlgModelCheck.ShowWindow(SW_HIDE);
    m_dlgUnderCamera.ShowWindow(SW_SHOW);
}



//����xy����ƶ����뵥λ
void CDlgPosition::OnPosradioXy1()
{
    SetSingleSpeed(XAXIS, 10000, 10000, 10000);
    SetSingleSpeed(YAXIS, 10000, 10000, 10000);
    m_nXRange = (LONG)(XGUIDE*0.01f);
    m_nYRange = (LONG)(YGUIDE*0.01f);
}

void CDlgPosition::OnPosradioXy2()
{
    SetSingleSpeed(XAXIS, 10000, 10000, 10000);
    SetSingleSpeed(YAXIS, 10000, 10000, 10000);
    m_nXRange = (LONG)(XGUIDE*0.1f);
    m_nYRange = (LONG)(YGUIDE*0.1f);
}

void CDlgPosition::OnPosradioXy3()
{
    SetSingleSpeed(XAXIS, 10000, 10000, 10000);
    SetSingleSpeed(YAXIS, 10000, 10000, 10000);
    m_nXRange = XGUIDE * 1;
    m_nYRange = YGUIDE * 1;
}

void CDlgPosition::OnPosradioXy4()
{
    SetSingleSpeed(XAXIS, 10000, 10000, 10000);
    SetSingleSpeed(YAXIS, 10000, 10000, 10000);
    m_nXRange = XGUIDE * 5;
    m_nYRange = YGUIDE * 5;
}

void CDlgPosition::OnPosradioXy5()
{
    SetSingleSpeed(XAXIS, 10000, 10000, 10000);
    SetSingleSpeed(YAXIS, 10000, 10000, 10000);
    m_nXRange = XGUIDE * 10;
    m_nYRange = YGUIDE * 10;
}

void CDlgPosition::OnPosradioXy6()
{
    SetSingleSpeed(XAXIS, 20000, 20000, 20000);
    SetSingleSpeed(YAXIS, 20000, 20000, 20000);
    m_nXRange = XGUIDE * 100;
    m_nYRange = YGUIDE * 100;
}

//�ƶ�xy�ᣬTimer1����ʵʱ��ʾxy�ĵ�ǰ����ֵ��Timer2��3�����������Ƿ񵽴Ｋ��.
void CDlgPosition::OnPosbtnXleft()
{
    double height = GetPosition(ZAXIS);
    if (height > 1000)//��ֹz��������ʱ��Χ�ƶ�xy��
    {
        if (m_nXRange >= XGUIDE * 1)
        {
            m_nXRange = XGUIDE * 1;
        }
    }
    SxMove(XAXIS, m_nXRange);
}

void CDlgPosition::OnPosbtnXright()
{
    double height = GetPosition(ZAXIS);
    if (height > 1000)
    {
        if (m_nXRange >= XGUIDE * 1)
        {
            m_nXRange = XGUIDE * 1;
        }
    }
    SxMove(XAXIS, -m_nXRange);
}

void CDlgPosition::OnPosbtnYtop()
{
    double height = GetPosition(ZAXIS);
    if (height > 1000)
    {
        if (m_nYRange >= YGUIDE * 1)
        {
            m_nYRange = YGUIDE * 1;
        }
    }
    SxMove(YAXIS, -m_nYRange);
}

void CDlgPosition::OnPosbtnYbottom()
{
    double height = GetPosition(ZAXIS);
    if (height > 1000)
    {
        if (m_nYRange >= YGUIDE * 1)
        {
            m_nYRange = YGUIDE * 1;
        }
    }
    SxMove(YAXIS, m_nYRange);
}

void CDlgPosition::OnHome()
{
    if (!ZComeBack())
    {
        MessageBox("ZComeBack Error!");
        return;
    }
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    int nResult = XYMove(0, 0);
    if (0 != nResult)
    {
        CString str;
        str.Format("XYMove Error:%d", nResult);
        MessageBox(str);
    }
    gm_uInfo.xySpeed = oldSpeed;
}

//����z����ƶ���λ
void CDlgPosition::OnPosradioZ1()
{
    m_nZRange = LONG(ZGUIDE * 0.05f);
}

void CDlgPosition::OnPosradioZ2()
{
    m_nZRange = LONG(ZGUIDE *0.1f);
}

void CDlgPosition::OnPosradioZ3()
{
    m_nZRange = ZGUIDE * 1;
}

void CDlgPosition::OnPosradioZ4()
{
    m_nZRange = ZGUIDE * 5;
}

//�����ƶ�z�ᣬ��ʱ�ر�������ʾxyʵʱ����Ķ�ʱ��1
void CDlgPosition::OnPosbtnZup()
{
#if BOARD_TYPE == COMI
    long dwMioStatus = 0;
    cmmStReadMioStatuses(ZAXIS, &dwMioStatus);
    if ((dwMioStatus >> cmIOST_ORG) & 0x1)
    {
        MessageBox("z���ѵ����ϼ��ޡ�");
        return;
    }

#endif // BOARD_TYPE == COMI
    SxMove(ZAXIS, -m_nZRange);
}

void CDlgPosition::OnPosbtnZdown()
{
    SxMove(ZAXIS, m_nZRange);
}

//�ƶ�����׼��
//ch == "BasePoint"		������ƶ�����׼��
//ch == "Nozzle1"		����1�ƶ�����׼��
//ch == "Nozzle2"		����1�ƶ�����׼��
//ch == "Nozzle3"		����1�ƶ�����׼��
//ch == "Nozzle4"		����1�ƶ�����׼��
void CDlgPosition::MoveToBasePoint(const char* ch)
{
    double x = (gm_dataPos.GetPlacePoint(ch)).x;
    double y = (gm_dataPos.GetPlacePoint(ch)).y;
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(x, y);
    gm_uInfo.xySpeed = oldSpeed;
    x = GetPosition(XAXIS);
    y = GetPosition(YAXIS);

    //�ڴ�����ʾ����
    double fx = x / (double)XGUIDE; //���廻���mm//x * 0.0025f;
    double fy = y / (double)YGUIDE; // y * 0.0025f;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_CHKPOINT_X, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_CHKPOINT_Y, str);
}
//\brief  �������������ƫ��ֵ
//\param  offset			[in] ƫ��ֵ�ַ���
//\param  chFirstPoint	[in] ��һ�����ַ���
//\param  chSecondPoint	[in] �ڶ������ַ���
//\return
void CDlgPosition::GetOffset(const char *offset, const char *chFirstPoint, const char *chSecondPoint/* = "BasePoint"*/)
{
    double xNozzle = GetPosition(XAXIS);
    double yNozzle = GetPosition(YAXIS);
    gm_dataPos.SetPlacePoint(chFirstPoint, xNozzle, yNozzle);//���������ڻ�׼�������

    double xBasePoint = (gm_dataPos.GetPlacePoint(chSecondPoint)).x;
    double yBasePoint = (gm_dataPos.GetPlacePoint(chSecondPoint)).y;
    gm_dataPos.SetPlacePoint(offset, xNozzle - xBasePoint, yNozzle - yBasePoint);//����ƫ��

    //�ڴ�������ʾ����
    double x = xNozzle - xBasePoint;
    double y = yNozzle - yBasePoint;
    double fx = x / (double)XGUIDE; //���廻���mm//x * 0.0025f;
    double fy = y / (double)YGUIDE; // y * 0.0025f;
    CString str;
    str.Format("%0.2f", fx);
    SetDlgItemText(IDC_CHKPOINT_OFFSETX, str);
    str.Format("%0.2f", fy);
    SetDlgItemText(IDC_CHKPOINT_OFFSETY, str);
}

//�Ͼ�ͷ�ƶ�����׼��--------------------------------------
void CDlgPosition::OnGotoCheckPoint()
{
    MoveToBasePoint("BasePoint");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_SAVECCD);
    pWnd->EnableWindow(TRUE);
}
//�����������׼������
void CDlgPosition::OnChkpointSaveccd()
{
    double x = GetPosition(XAXIS);
    double y = GetPosition(YAXIS);
    gm_dataPos.SetPlacePoint("BasePoint", x, y);
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_SAVECCD);
    pWnd->EnableWindow(FALSE);
}


//����1�ƶ�����׼��--------------------------------------
void CDlgPosition::OnChkpointNozzle()
{
    MoveToBasePoint("Nozzle1");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET);
    pWnd->EnableWindow(TRUE);
}
//��������1���������ƫ��
void CDlgPosition::OnChkpointGetoffset()
{
}
void CDlgPosition::OnBnClickedChkpointGetoffset()
{
    GetOffset("offset_nozzle1", "Nozzle1");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET);
    pWnd->EnableWindow(FALSE);
    SetNozzleDownOff(0);
}

//����2�ƶ�����׼��--------------------------------------
void CDlgPosition::OnBnClickedChkpointNozzle2()
{
    MoveToBasePoint("Nozzle2");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET2);
    pWnd->EnableWindow(TRUE);
}
//��������2���������ƫ��
void CDlgPosition::OnBnClickedChkpointGetoffset2()
{
    GetOffset("offset_nozzle2", "Nozzle2");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET2);
    pWnd->EnableWindow(FALSE);
    SetNozzleDownOff(1);
}

//����3�ƶ�����׼��--------------------------------------
void CDlgPosition::OnBnClickedChkpointNozzle3()
{
    MoveToBasePoint("Nozzle3");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET3);
    pWnd->EnableWindow(TRUE);
}
//��������3���������ƫ��
void CDlgPosition::OnBnClickedChkpointGetoffset3()
{
    GetOffset("offset_nozzle3", "Nozzle3");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET3);
    pWnd->EnableWindow(FALSE);
    SetNozzleDownOff(2);
}

//����4�ƶ�����׼��--------------------------------------
void CDlgPosition::OnBnClickedChkpointNozzle4()
{
    MoveToBasePoint("Nozzle4");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET4);
    pWnd->EnableWindow(TRUE);
}
//��������4���������ƫ��
void CDlgPosition::OnBnClickedChkpointGetoffset4()
{
    GetOffset("offset_nozzle4", "Nozzle4");
    CWnd *pWnd = GetDlgItem(IDC_CHKPOINT_GETOFFSET4);
    pWnd->EnableWindow(FALSE);
    SetNozzleDownOff(3);
}

//���ƶ�λ���ڵ���ɫ
void CDlgPosition::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rtW, rtTop;
    GetWindowRect(rtW);
    ScreenToClient(rtW);
    //��ȡ��λ�����Ҳ�ľ�������
    CWnd *pWnd = GetDlgItem(IDC_STATICBOX_XY);
    pWnd->GetWindowRect(rtTop);
    ScreenToClient(rtTop);
    rtW.left = rtTop.left - 4;

    //pWnd = GetDlgItem(IDC_STATICBOX_Z2);
    //pWnd->GetWindowRect(rtTop);
    //ScreenToClient(rtTop);
    //rtW.bottom = rtTop.bottom + 4;

    //���ƶ�λ�����ұ߲��ֵĵ�ɫ
    CBrush brush(RGB(195, 248, 255));
    CBrush *pOldBrush = dc.SelectObject(&brush);
    dc.Rectangle(rtW);
    dc.SelectObject(pOldBrush);

    //ȥ�������ɫ�߿�
    CPen pen(PS_SOLID, 2, RGB(195, 248, 255));
    CPen *poldpen = dc.SelectObject(&pen);
    dc.MoveTo(rtW.left, rtW.top);
    dc.LineTo(rtW.left, rtW.bottom);
    dc.MoveTo(rtW.left, rtW.bottom);
    dc.LineTo(rtW.right, rtW.bottom);
    dc.SelectObject(poldpen);

    if (m_bCheckPoint)
    {
        //���ƻ�׼�㴰�ڵı���ɫ
        CRect rtFeed;
        pWnd = GetDlgItem(IDC_POS_FRAME);
        pWnd->GetWindowRect(rtFeed);
        ScreenToClient(rtFeed);

        CBrush brush1(RGB(255, 192, 128));
        pOldBrush = dc.SelectObject(&brush1);
        dc.Rectangle(rtFeed);
        dc.SelectObject(pOldBrush);
    }
}	// Do not call pe

//���Ƹ�radio��ť�ı���ɫ�����򱳾�ɫһ��
HBRUSH CDlgPosition::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    UINT nID = pWnd->GetDlgCtrlID();
    switch (nID)
    {
    case IDC_POSRADIO_XY1:
    case IDC_STATIC4:
    case IDC_STATIC5:
    case IDC_POSRADIO_XY2:
    case IDC_POSRADIO_XY3:
    case IDC_POSRADIO_XY4:
    case IDC_POSRADIO_XY5:
    case IDC_POSRADIO_XY6:
    case IDC_POSRADIO_XY8:
    case IDC_POSRADIO_XY9:
    case IDC_POSRADIO_XY11:
    case IDC_POSRADIO_XY12:
    case IDC_POSRADIO_XY14:
    case IDC_POSRADIO_XY16:
    case IDC_POSRADIO_XY17:
    case IDC_POSRADIO_XY18:
    case IDC_POSRADIO_XY19:
    case IDC_POSRADIO_XY20:
    case IDC_POSRADIO_XY21:
    case IDC_POSRADIO_XY22:
    case IDC_POSRADIO_XY23:
    case IDC_POSRADIO_XY24:
    case IDC_POSRADIO_XY25:
    case IDC_POSRADIO_XY26:
    case IDC_POSRADIO_XY27:
    case IDC_POSRADIO_XY28:
    case IDC_POSRADIO_XY29:
    case IDC_POSRADIO_XY30:
    case IDC_POSRADIO_XY31:
    case IDC_POSRADIO_XY32:
    case IDC_POSRADIO_XY33:
    case IDC_POSRADIO_Z1:
    case IDC_POSRADIO_Z2:
    case IDC_POSRADIO_Z3:
    case IDC_POSRADIO_Z4:
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetBkColor(RGB(195, 248, 255));
        hbr = m_brush;
        break;
    }
    return hbr;
}

//----------------------------------------------------------------------------------
//����ʵʱ��ʾxy�������
//----------------------------------------------------------------------------------
void CDlgPosition::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    double x = GetPosition(XAXIS);
    double y = GetPosition(YAXIS);
    double z = GetPosition(ZAXIS);
    CString str;
    str.Format("%0.2f", x / float(XGUIDE));
    SetDlgItemText(IDC_POSEDT_X, str);
    str.Format("%0.2f", y / float(YGUIDE));
    SetDlgItemText(IDC_POSEDT_Y, str);
    str.Format("%0.2f", z / (float)ZGUIDE);
    SetDlgItemText(IDC_POSEDT_Z, str);
    CDialog::OnTimer(nIDEvent);
}

//----------------------------------------------------------------------------------
/**
\brief  ֪ͨ����ȡ��ǰX�϶��������λ�ã����丳ֵ��gm_nXLength
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnCustomdrawPosSliderx(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_nXLength = m_xSlider.GetPos();
    gm_nXLength = m_nXLength;
    *pResult = 0;
}

//----------------------------------------------------------------------------------
/**
\brief  ֪ͨ����ȡ��ǰY�϶��������λ�ã����丳ֵ��gm_nYLength
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnCustomdrawPosSlidery(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_nYLength = 400 - m_ySlider.GetPos();
    gm_nYLength = m_nYLength;
    *pResult = 0;
}

//----------------------------------------------------------------------------------
/**
\brief  ��ť���϶���x���ӣ�����Ϊ2
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnPosXadd()
{
    m_nXLength += 2;
    gm_nXLength = m_nXLength;
    m_xSlider.SetPos(m_xSlider.GetPos() + 2);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ť���϶���x���٣�����Ϊ2
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnPosXsub()
{
    m_nXLength -= 2;
    gm_nXLength = m_nXLength;
    m_xSlider.SetPos(m_xSlider.GetPos() - 2);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ť���϶���y���٣�����Ϊ2
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnPosYsub()
{
    m_nYLength -= 2;
    gm_nYLength = m_nYLength;
    m_ySlider.SetPos(m_ySlider.GetPos() + 2);
}

//----------------------------------------------------------------------------------
/**
\brief  ��ť���϶���y���ӣ�����Ϊ2
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnPosYadd()
{
    m_nYLength += 2;
    gm_nYLength = m_nYLength;
    m_ySlider.SetPos(m_ySlider.GetPos() - 2);
}

//----------------------------------------------------------------------------------
/**
\brief  ˫��������
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    CRect rt;//(0,0,400,400);
    CWnd *pWnd = GetDlgItem(IDC_CAMERA_FRAME);
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);
    if (rt.PtInRect(point))
        m_bRect = !m_bRect;
    CDialog::OnLButtonDblClk(nFlags, point);
}

//----------------------------------------------------------------------------------
/**
\brief  �л��������
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnBnClickedBtnSwitchCamera()
{

    if (m_nDeviceNum == 1)
    {
        MessageBox("û�и�������");
        return;
    }
    CloseCamera();

    if (m_stOpenParam.pszContent == "2")
        m_stOpenParam.pszContent = "1";
    else
        m_stOpenParam.pszContent = "2";
    InitCamera();
    OpenSnapEx();
}

//----------------------------------------------------------------------------------
/**
\brief  �Ҽ���������ԭ����ť��ִ�л�ԭ����
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CRect rt;//(0,0,400,400);
    CWnd *pWnd = GetDlgItem(IDC_BTN_HOME);
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);
    if (rt.PtInRect(point))
        MessageBox("1");// GoHome();
    CDialog::OnRButtonDown(nFlags, point);
}
//----------------------------------------------------------------------------------
/**
\brief  ����ԭ����ť��ִ����ԭ����
*/
//----------------------------------------------------------------------------------
void CDlgPosition::OnBnClickedBtnGoHome()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    //CWnd* pWnd = GetParent();
    //GetParent()->m_ctrlStatus.SetText("System is initializing...");
    for (int i = 0; i < 4; i++)
        SetNozzleDownOff(i);

    BOOL bSuccess = GoHome();
    if (bSuccess)
    {
        MessageBox("��ԭ������");
        //GetParent()->m_ctrlStatus.SetText("Ready");
        //GreenLightOn();
    }
}


void CDlgPosition::OnBnClickedBtnNozzle1Updown()
{
    if (CheckNozzleUpStatus(NOZZLE_0))
    {
        SetNozzleDownOn(NOZZLE_0);
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    }
    else
    {
        SetNozzleDownOff(NOZZLE_0);
        SetDlgItemText(IDC_BTN_NOZZLE1_UPDOWN, "����");
    }
}


void CDlgPosition::OnBnClickedBtnNozzle2Updown()
{
    if (CheckNozzleUpStatus(NOZZLE_1))
    {
        SetNozzleDownOn(NOZZLE_1);
        SetDlgItemText(IDC_BTN_NOZZLE2_UPDOWN, "����");
    }
    else
    {
        SetNozzleDownOff(NOZZLE_1);
        SetDlgItemText(IDC_BTN_NOZZLE2_UPDOWN, "����");
    }
}


void CDlgPosition::OnBnClickedBtnNozzle3Updown()
{
    if (CheckNozzleUpStatus(NOZZLE_2))
    {
        SetNozzleDownOn(NOZZLE_2);
        SetDlgItemText(IDC_BTN_NOZZLE3_UPDOWN, "����");
    }
    else
    {
        SetNozzleDownOff(NOZZLE_2);
        SetDlgItemText(IDC_BTN_NOZZLE3_UPDOWN, "����");
    }
}


void CDlgPosition::OnBnClickedBtnNozzle4Updown()
{
    if (CheckNozzleUpStatus(NOZZLE_3))
    {
        SetNozzleDownOn(NOZZLE_3);
        SetDlgItemText(IDC_BTN_NOZZLE4_UPDOWN, "����");
    }
    else
    {
        SetNozzleDownOff(NOZZLE_3);
        SetDlgItemText(IDC_BTN_NOZZLE4_UPDOWN, "����");
    }
}


void CDlgPosition::OnBnClickedCheckVacumm1()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_VACUMM1);
    if (pBtn->GetCheck())
        SetNozzleVaccumOn(NOZZLE_0);
    else
        SetNozzleVaccumOff(NOZZLE_0);
}


void CDlgPosition::OnBnClickedCheckVacumm2()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_VACUMM2);
    if (pBtn->GetCheck())
        SetNozzleVaccumOn(NOZZLE_1);
    else
        SetNozzleVaccumOff(NOZZLE_1);
}


void CDlgPosition::OnBnClickedCheckVacumm3()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_VACUMM3);
    if (pBtn->GetCheck())
        SetNozzleVaccumOn(NOZZLE_2);
    else
        SetNozzleVaccumOff(NOZZLE_2);
}


void CDlgPosition::OnBnClickedCheckVacumm4()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_VACUMM4);
    if (pBtn->GetCheck())
        SetNozzleVaccumOn(NOZZLE_3);
    else
        SetNozzleVaccumOff(NOZZLE_3);
}


void CDlgPosition::OnBnClickedCheckBlow1()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_BLOW1);
    if (pBtn->GetCheck())
        SetNozzleBlowOn(NOZZLE_0);
    else
        SetNozzleBlowOff(NOZZLE_0);
}


void CDlgPosition::OnBnClickedCheckBlow2()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_BLOW2);
    if (pBtn->GetCheck())
        SetNozzleBlowOn(NOZZLE_1);
    else
        SetNozzleBlowOff(NOZZLE_1);
}


void CDlgPosition::OnBnClickedCheckBlow3()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_BLOW3);
    if (pBtn->GetCheck())
        SetNozzleBlowOn(NOZZLE_2);
    else
        SetNozzleBlowOff(NOZZLE_2);
}


void CDlgPosition::OnBnClickedCheckBlow4()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_BLOW4);
    if (pBtn->GetCheck())
        SetNozzleBlowOn(NOZZLE_3);
    else
        SetNozzleBlowOff(NOZZLE_3);
}



void CDlgPosition::OnBnClickedCheckCylinder0()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER0);
    if (pBtn->GetCheck())
        SetCylinderDown(0);
    else
        SetCylinderUp(0);
}


void CDlgPosition::OnBnClickedCheckCylinder1()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER1);
    if (pBtn->GetCheck())
        SetCylinderDown(1);
    else
        SetCylinderUp(1);
}


void CDlgPosition::OnBnClickedCheckCylinder2()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER2);
    if (pBtn->GetCheck())
        SetCylinderDown(2);
    else
        SetCylinderUp(2);
}


void CDlgPosition::OnBnClickedCheckCylinder3()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER3);
    if (pBtn->GetCheck())
        SetCylinderDown(3);
    else
        SetCylinderUp(3);
}


void CDlgPosition::OnBnClickedCheckCylinder4()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER4);
    if (pBtn->GetCheck())
        SetCylinderDown(4);
    else
        SetCylinderUp(4);
}


void CDlgPosition::OnBnClickedCheckCylinder5()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER5);
    if (pBtn->GetCheck())
        SetCylinderDown(5);
    else
        SetCylinderUp(5);
}


void CDlgPosition::OnBnClickedCheckCylinder6()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER6);
    if (pBtn->GetCheck())
        SetCylinderDown(6);
    else
        SetCylinderUp(6);
}


void CDlgPosition::OnBnClickedCheckCylinder7()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER7);
    if (pBtn->GetCheck())
        SetCylinderDown(7);
    else
        SetCylinderUp(7);
}


void CDlgPosition::OnBnClickedCheckCylinder8()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER8);
    if (pBtn->GetCheck())
        SetCylinderDown(8);
    else
        SetCylinderUp(8);
}


void CDlgPosition::OnBnClickedCheckCylinder9()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_CYLINDER9);
    if (pBtn->GetCheck())
        SetCylinderDown(9);
    else
        SetCylinderUp(9);
}


void CDlgPosition::OnBnClickedCheckUpLight()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_UP_LIGHT);
    if (pBtn->GetCheck())
        SetUpCameraLightOn();
    else
        SetUpCameraLightOff();
}


void CDlgPosition::OnBnClickedCheckDownLight()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_DOWN_LIGHT);
    if (pBtn->GetCheck())
        SetDownCameraLightOn();
    else
        SetDownCameraLightOff();
}

//��ʱ��ת��angleȡ��
void CDlgPosition::OnBnClickedBtnCounterclockwise()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData();
    double angle = 0;
    switch (m_radioRotateAngel)
    {
    case 0:
        angle = 0.1;
        break;
    case 1:
        angle = 1;
        break;
    case 2:
        angle = 10;
        break;
    case 3:
        angle = 90;
        break;
    case 4:
        angle = 180;
        break;
    case 5:
        angle = 360;
        break;
    default:
        break;
    }
    SrMove(angle*PI / 180, m_radioNozzleNum/* + RAXIS_1*/);
}

//˳ʱ��ת��angleȡ��
void CDlgPosition::OnBnClickedBtnRotateClockwise()
{
    UpdateData();
    double angle = 0;
    switch (m_radioRotateAngel)
    {
    case 0:
        angle = 0.1;
        break;
    case 1:
        angle = 1;
        break;
    case 2:
        angle = 10;
        break;
    case 3:
        angle = 90;
        break;
    case 4:
        angle = 180;
        break;
    case 5:
        angle = 360;
        break;
    default:
        break;
    }
    SrMove(-angle*PI / 180, m_radioNozzleNum/* + RAXIS_1*/);
}




void CDlgPosition::OnBnClickedRadioRotateAngle1()
{
    gm_nRotateAngleIn = 0;
}


void CDlgPosition::OnBnClickedRadioRotateAngle91()
{
    gm_nRotateAngleIn = 2;
}


void CDlgPosition::OnBnClickedRadioRotateAnglen91()
{
    gm_nRotateAngleIn = -2;
}


void CDlgPosition::OnBnClickedRadioRotateAngle181()
{
    gm_nRotateAngleIn = 1;
}


void CDlgPosition::OnBnClickedBtnRotateTest()
{
    //��ת90��
    if (gm_nRotateAngleIn != 0)
    {
        double angle = PI / (double)gm_nRotateAngleIn;
        SrMove(angle, 0/*RAXIS_1*/);
    }

}

///////////////////////////////////////////////////////////////////////////////
//  File generated by HDevelop for HALCON/C++ Version 11.0
///////////////////////////////////////////////////////////////////////////////



#ifndef __APPLE__
#  include "HalconCpp.h"
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#  endif
#endif



using namespace HalconCpp;


#ifndef NO_EXPORT_MAIN
// Main procedure
Point action(int nMode)
{
    Point ptOffset{ 0,0 };
    if (nMode == 2)
    {
        if (HDevWindowStack::IsOpen())
            CloseWindow(HDevWindowStack::Pop());
        return ptOffset;
    }
    //int nWidth = 2592;
    //int _nHeight = 1944;

    // Local iconic variables 
    HObject  ho_Image, ho_Rectangle, ho_ImageReduced;
    HObject  ho_ImageMean, ho_ContoursAffinTrans;
    HObject  ho_ImageMean1;
    static HObject ho_ModelContours;

    // Local control variables 
    HTuple  hv_Width, hv_Height, hv_WindowHandle;
    HTuple  hv_XRectRow, hv_XRectColumn, hv_XRectWidth, hv_XRechHeight;
    HTuple  hv_YRectRow, hv_YRectColumn, hv_YRectWidth, hv_YRechHeight;
    HTuple  hv_MeasureHandleX, hv_MeasureHandleY, hv_RowEdgeFirstX;
    HTuple  hv_ColumnEdgeFirstX, hv_AmplitudeFirstX, hv_RowEdgeSecondX;
    HTuple  hv_ColumnEdgeSecondX, hv_AmplitudeSecondX, hv_IntraDistanceX;
    HTuple  hv_InterDistanceX, hv_RowEdgeFirstY, hv_ColumnEdgeFirstY;
    HTuple  hv_AmplitudeFirstY, hv_RowEdgeSecondY, hv_ColumnEdgeSecondY;
    HTuple  hv_AmplitudeSecondY, hv_IntraDistanceY, hv_InterDistanceY;
    HTuple  hv_CountFirstX, hv_LengthFirstX, hv_PixlsPerMMFirstX;
    HTuple  hv_CountSecondX, hv_LengthSecondX, hv_PixlsPerMMSecondX;
    HTuple   hv_CountFirstY, hv_LengthFirstY;
    HTuple  hv_PixlsPerMMFirstY, hv_CountSecondY, hv_LengthSecondY;
    HTuple  hv_PixlsPerMMSecondY, hv_PixlsPerMM;
    HTuple  hv_ModelCenterRow, hv_ModelCenterColumn;
    HTuple  hv_AngleModel, hv_Score, hv_HomMat2D, hv_RowOffset;
    HTuple  hv_ColumnOffset, hv_ImageFiles, hv_Index, hv_RowSearch;
    HTuple  hv_ColumnSearch, hv_AngleSearch, hv_ScoreSearch;
    HTuple hv_AcqHandle;
    static HTuple hv_ModelID, hv_ModelRectHeight, hv_ModelRectWidth, hv_RowModel, hv_ColumnModel, hv_PixlsPerMMY, hv_PixlsPerMMX;
    //Image Acquisition 01: Code generated by Image Acquisition 01
    if (HDevWindowStack::IsOpen())
        CloseWindow(HDevWindowStack::Pop());
    //ReadImage(&ho_Image, "D:/halconimage/rule/2017-05-05_09_44_22_496.bmp");
    try
    {
        HTuple Device = gm_uInfo.strCameraVersion + "(" + gm_uInfo.strCameraNum + ") producer:C:\\Program Files\\Daheng Imavision\\MER-Series\\GenTL\\Win32\\GxUSBTL.cti interface:0:DHUSBBase vendor:Daheng Imavision model:" + gm_uInfo.strCameraVersion;

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
            &hv_AcqHandle);

        SetFramegrabberParam(hv_AcqHandle, "ExposureTime", EXPOSITION * 30);

        GrabImage(&ho_Image, hv_AcqHandle);

        GetImageSize(ho_Image, &hv_Width, &hv_Height);
        SetWindowAttr("background_color", "black");
        OpenWindow(0, 0, hv_Width / 3, hv_Height / 3, 0, "", "", &hv_WindowHandle);
        HDevWindowStack::Push(hv_WindowHandle);
        if (HDevWindowStack::IsOpen())
            DispObj(ho_Image, HDevWindowStack::GetActive());

        if (nMode == 0)
        {
            hv_XRectRow = 490;
            hv_XRectColumn = 540;
            hv_XRectWidth = 90;
            hv_XRechHeight = 5;

            hv_YRectRow = 620;
            hv_YRectColumn = 660;
            hv_YRectWidth = 90;
            hv_YRechHeight = 5;

            //hv_XRectRow = 920;
            //hv_XRectColumn = 820;
            //hv_XRectWidth = 300;
            //hv_XRechHeight = 20;

            //hv_YRectRow = 1400;
            //hv_YRectColumn = 1350;
            //hv_YRectWidth = 300;
            //hv_YRechHeight = 20;

            GenMeasureRectangle2(hv_XRectRow, hv_XRectColumn, HTuple(0).TupleRad(), hv_XRectWidth,
                hv_XRechHeight, hv_Width, hv_Height, "bilinear", &hv_MeasureHandleX);
            GenMeasureRectangle2(hv_YRectRow, hv_YRectColumn, HTuple(-90).TupleRad(), hv_YRectWidth,
                hv_YRechHeight, hv_Width, hv_Height, "bilinear", &hv_MeasureHandleY);
            DispRectangle2(hv_WindowHandle, hv_XRectRow, hv_XRectColumn, HTuple(0).TupleRad(),
                hv_XRectWidth, hv_XRechHeight);
            DispRectangle2(hv_WindowHandle, hv_YRectRow, hv_YRectColumn, HTuple(-90).TupleRad(),
                hv_YRectWidth, hv_YRechHeight);
            MeasurePairs(ho_Image, hv_MeasureHandleX, 1, 30, "all", "all", &hv_RowEdgeFirstX,
                &hv_ColumnEdgeFirstX, &hv_AmplitudeFirstX, &hv_RowEdgeSecondX, &hv_ColumnEdgeSecondX,
                &hv_AmplitudeSecondX, &hv_IntraDistanceX, &hv_InterDistanceX);
            MeasurePairs(ho_Image, hv_MeasureHandleY, 1, 30, "all", "all", &hv_RowEdgeFirstY,
                &hv_ColumnEdgeFirstY, &hv_AmplitudeFirstY, &hv_RowEdgeSecondY, &hv_ColumnEdgeSecondY,
                &hv_AmplitudeSecondY, &hv_IntraDistanceY, &hv_InterDistanceY);
            hv_CountFirstX = hv_ColumnEdgeFirstX.TupleLength();
            hv_LengthFirstX = HTuple(hv_ColumnEdgeFirstX[hv_CountFirstX - 1]) - HTuple(hv_ColumnEdgeFirstX[0]);
            hv_PixlsPerMMFirstX = hv_LengthFirstX / (hv_CountFirstX - 1);
            hv_CountSecondX = hv_ColumnEdgeSecondX.TupleLength();
            hv_LengthSecondX = HTuple(hv_ColumnEdgeSecondX[hv_CountSecondX - 1]) - HTuple(hv_ColumnEdgeSecondX[0]);
            hv_PixlsPerMMSecondX = hv_LengthSecondX / (hv_CountSecondX - 1);
            hv_PixlsPerMMX = (hv_PixlsPerMMFirstX + hv_PixlsPerMMSecondX) / 2;

            hv_CountFirstY = hv_RowEdgeFirstY.TupleLength();
            hv_LengthFirstY = HTuple(hv_RowEdgeFirstY[hv_CountFirstY - 1]) - HTuple(hv_RowEdgeFirstY[0]);
            hv_PixlsPerMMFirstY = hv_LengthFirstY / (hv_CountFirstY - 1);
            hv_CountSecondY = hv_RowEdgeSecondY.TupleLength();
            hv_LengthSecondY = HTuple(hv_RowEdgeSecondY[hv_CountSecondY - 1]) - HTuple(hv_RowEdgeSecondY[0]);
            hv_PixlsPerMMSecondY = hv_LengthSecondY / (hv_CountSecondY - 1);
            hv_PixlsPerMMY = (hv_PixlsPerMMFirstY + hv_PixlsPerMMSecondY) / 2;

            hv_PixlsPerMM = (hv_PixlsPerMMX + hv_PixlsPerMMY) / 2;
            gm_uInfo.nPixelsPM_up = hv_PixlsPerMM;
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), "cyan");
            SetTposition(hv_WindowHandle, 100, 100);
            WriteString(hv_WindowHandle, ("X: 1mm = " + hv_PixlsPerMMX) + " pixels");
            SetTposition(hv_WindowHandle, 150, 100);
            WriteString(hv_WindowHandle, ("Y: 1mm = " + hv_PixlsPerMMY) + " pixels");
            // stop(); only in hdevelop

            //if (HDevWindowStack::IsOpen())
            //	ClearWindow(HDevWindowStack::GetActive());
            //ReadImage(&ho_Image, "D:/halconimage/rule/2017-05-05_09_44_22_496.bmp");
            //if (HDevWindowStack::IsOpen())
            //	DispObj(ho_Image, HDevWindowStack::GetActive());
            if (HDevWindowStack::IsOpen())
                SetDraw(HDevWindowStack::GetActive(), "margin");
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), "green");

            hv_ModelCenterRow = 542;
            hv_ModelCenterColumn = 610;
            hv_ModelRectWidth = 120;
            hv_ModelRectHeight = 120;

            //hv_ModelCenterRow = 1100;
            //hv_ModelCenterColumn = 1170;
            //hv_ModelRectWidth = 400;
            //hv_ModelRectHeight = 400;

            GenRectangle2(&ho_Rectangle, hv_ModelCenterRow, hv_ModelCenterColumn, 0, hv_ModelRectWidth,
                hv_ModelRectHeight);
            ReduceDomain(ho_Image, ho_Rectangle, &ho_ImageReduced);
            //mean_image (ImageReduced, ImageMean, 3, 3)
            CreateShapeModel(ho_ImageReduced, "auto", -0.39, 0.79, "auto", "auto", "use_polarity",
                "auto", "auto", &hv_ModelID);
            Sleep(100);
            FindShapeModel(ho_Image, hv_ModelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares",
                0, 0.9, &hv_RowModel, &hv_ColumnModel, &hv_AngleModel, &hv_Score);
            GetShapeModelContours(&ho_ModelContours, hv_ModelID, 1);
            VectorAngleToRigid(0, 0, 0, hv_RowModel, hv_ColumnModel, hv_AngleModel, &hv_HomMat2D);
            AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2D);
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), "green");
            DispRectangle2(hv_WindowHandle, hv_RowModel, hv_ColumnModel, hv_AngleModel, hv_ModelRectWidth,
                hv_ModelRectHeight);
            if (HDevWindowStack::IsOpen())
                SetColor(HDevWindowStack::GetActive(), "yellow");
            if (HDevWindowStack::IsOpen())
                DispObj(ho_ContoursAffinTrans, HDevWindowStack::GetActive());
        }
        else if (nMode == 1)
        {
            if (HDevWindowStack::IsOpen())
                ClearWindow(HDevWindowStack::GetActive());
            hv_RowOffset = 0;
            hv_ColumnOffset = 0;
            ////Image Acquisition 01: Code generated by Image Acquisition 01
            //ListFiles("D:/halconimage/rule", (HTuple("files").Append("follow_links")), &hv_ImageFiles);
            //TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")),
            //	&hv_ImageFiles);
            //HTuple end_val77 = (hv_ImageFiles.TupleLength()) - 1;
            //HTuple step_val77 = 1;
            //for (hv_Index = 0; hv_Index.Continue(end_val77, step_val77); hv_Index += step_val77)
            //{
            //	if (HDevWindowStack::IsOpen())
            //		ClearWindow(HDevWindowStack::GetActive());
            //	ReadImage(&ho_Image, HTuple(hv_ImageFiles[hv_Index]));
            if (HDevWindowStack::IsOpen())
                DispObj(ho_Image, HDevWindowStack::GetActive());
            //mean_image (Image, ImageMean1, 9, 9)
            //Image Acquisition 01: Do something
            FindShapeModel(ho_Image, hv_ModelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares",
                0, 0.9, &hv_RowSearch, &hv_ColumnSearch, &hv_AngleSearch, &hv_ScoreSearch);
            if (0 != (hv_ScoreSearch > 0))
            {
                VectorAngleToRigid(0, 0, 0, hv_RowSearch, hv_ColumnSearch, hv_AngleSearch,
                    &hv_HomMat2D);
                AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hv_HomMat2D);
                if (HDevWindowStack::IsOpen())
                    SetColor(HDevWindowStack::GetActive(), "green");
                if (HDevWindowStack::IsOpen())
                    SetDraw(HDevWindowStack::GetActive(), "margin");
                DispRectangle2(hv_WindowHandle, hv_RowSearch, hv_ColumnSearch, hv_AngleSearch,
                    hv_ModelRectWidth, hv_ModelRectHeight);
                if (HDevWindowStack::IsOpen())
                    SetColor(HDevWindowStack::GetActive(), "yellow");
                if (HDevWindowStack::IsOpen())
                    DispObj(ho_ContoursAffinTrans, HDevWindowStack::GetActive());

                hv_RowOffset = ((hv_RowSearch - hv_RowModel) + 0.01) * 1;
                hv_ColumnOffset = (hv_ColumnSearch - hv_ColumnModel)*2.3;
                ptOffset.x = hv_ColumnOffset / hv_PixlsPerMMX;
                ptOffset.y = hv_RowOffset / hv_PixlsPerMMY;
                if (HDevWindowStack::IsOpen())
                    SetColor(HDevWindowStack::GetActive(), "cyan");
                SetTposition(hv_WindowHandle, 100, 100);
                WriteString(hv_WindowHandle, ("1mm = " + hv_PixlsPerMM) + " pixels");
                SetTposition(hv_WindowHandle, 150, 100);
                WriteString(hv_WindowHandle, ("x��ƫ�ƣ�" + (hv_ColumnOffset / hv_PixlsPerMMX)) + " mm");
                SetTposition(hv_WindowHandle, 200, 100);
                WriteString(hv_WindowHandle, ("y��ƫ�ƣ�" + (hv_RowOffset / hv_PixlsPerMMY)) + " mm");
            }
            // stop(); only in hdevelop
            //}
        }

        // stop(); only in hdevelop
        CloseFramegrabber(hv_AcqHandle);
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        if (HDevWindowStack::IsOpen())
            CloseWindow(HDevWindowStack::Pop());
        CloseFramegrabber(hv_AcqHandle);
    }
    return ptOffset;
}


#endif

void CDlgPosition::OnBnClickedCancel()
{
    action(2);
    CloseCamera();
    KillTimer(0);
    CDialog::OnCancel();
}


void CDlgPosition::OnBnClickedButtonCreate()
{
    CloseCamera();
    SetUpCameraLightOff();

    action(0);

    InitCamera();
    OpenSnapEx();
}

DWORD WINAPI GetCPKDataDynamic(
    LPVOID lpParameter   // thread data
)
{
    //Position2 *posOffset = (Position2*)lpParameter;
    //SrMove(posOffset->phi, RAXIS_1);
    double *angle = (double*)lpParameter;
    return 0;
}

void CDlgPosition::OnBnClickedButtonSearch()
{

    int n = 0;
    CString strTemp;
    ofstream fout;
    Point ptOffset{ 0,0 };
    fout.open("d:\\cpkdata-dynamic.txt", std::ios_base::out);
    double x = GetPosition(XAXIS);
    double y = GetPosition(YAXIS);
    CloseCamera();
    SetUpCameraLightOff();
    while (true)
    {
        SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
        SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
        ZComeBack();
        XYMove(0, 0);
        Sleep(500);
        XYMove(x, y);
        Sleep(100);
        ptOffset = action(1);

        strTemp.Format("�� %d �����ݣ�", n + 1);
        fout << strTemp << std::endl;
        strTemp.Format("x:%0.5f", ptOffset.x);
        fout << strTemp << std::endl;
        strTemp.Format("y:%0.5f", ptOffset.y);
        fout << strTemp << std::endl;
        n++;

        if (n % 10 == 0)
        {
            strTemp.Format("��ǰ������%d", n);
            if (IDCANCEL == MessageBox(strTemp, "����ͳ��", MB_RETRYCANCEL))
                break;
        }
    }
    fout.close();
    InitCamera();
    OpenSnapEx();
}


void CDlgPosition::OnBnClickedButtonSearch2()
{
    int n = 0;
    CString strTemp;
    ofstream fout;
    Point ptOffset{ 0,0 };
    fout.open("d:\\cpkdata-static.txt", std::ios_base::out);
    double x = GetPosition(XAXIS);
    double y = GetPosition(YAXIS);
    CloseCamera();
    SetUpCameraLightOff();
    while (true)
    {
        //SetAxisSpeed(XAXIS, 100, gm_uInfo.xySpeed);
        //SetAxisSpeed(YAXIS, 100, gm_uInfo.xySpeed);
        //ZComeBack();
        //XYMove(0, 0);
        //Sleep(500);
        //XYMove(x, y);
        //Sleep(100);
        ptOffset = action(1);
        strTemp.Format("�� %d �����ݣ�", n + 1);
        fout << strTemp << std::endl;
        strTemp.Format("x:%0.5f", ptOffset.x);
        fout << strTemp << std::endl;
        strTemp.Format("y:%0.5f", ptOffset.y);
        fout << strTemp << std::endl;
        n++;

        if (n % 10 == 0)
        {
            strTemp.Format("��ǰ������%d", n);
            if (IDCANCEL == MessageBox(strTemp, "����ͳ��", MB_RETRYCANCEL))
                break;
        }
    }
    fout.close();
    InitCamera();
    OpenSnapEx();
}
