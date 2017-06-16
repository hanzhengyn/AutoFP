// DlgSetting.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "DlgSetting.h"
#include "AutoFPDlg.h"
#include "MotionAct.h"
#include "P800IspDlg.h"
#include "DlgPosition.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetting dialog


CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgSetting::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgSetting)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // 	m_dlgP800.DoModal();
    //	m_dlgP800.CreateEx()
}


void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgSetting)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_SOCKETMODEL, m_combSocketModel);
    DDX_Control(pDX, IDC_COMBO_CHIPMODEL, m_combChipModel);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialog)
    //{{AFX_MSG_MAP(CDlgSetting)
    ON_BN_CLICKED(IDC_SETTING_XY1, OnSettingXy1)
    ON_BN_CLICKED(IDC_SETTING_XY2, OnSettingXy2)
    ON_BN_CLICKED(IDC_SETTING_XY3, OnSettingXy3)
    ON_BN_CLICKED(IDC_SETTING_Z1, OnSettingZ1)
    ON_BN_CLICKED(IDC_SETTING_Z2, OnSettingZ2)
    ON_BN_CLICKED(IDC_SETTING_Z3, OnSettingZ3)
    ON_BN_CLICKED(IDC_MODE1, OnMode1)
    ON_BN_CLICKED(IDC_MODE4, OnMode4)
    ON_BN_CLICKED(IDC_MODE5, OnMode5)
    ON_BN_CLICKED(IDC_MODE6, OnMode6)
    ON_BN_CLICKED(IDC_CHECK_STOCK, OnCheckStock)
    ON_BN_CLICKED(IDC_SETTING_Z4, OnSettingZ4)
    ON_BN_CLICKED(IDC_SETTING_Z5, OnSettingZ5)
    ON_BN_CLICKED(IDC_SETTING_XY4, OnSettingXy4)
    ON_BN_CLICKED(IDC_SETTING_XY5, OnSettingXy5)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CHECK_CHECK, &CDlgSetting::OnBnClickedCheckCheck)
    ON_BN_CLICKED(IDC_CHECK_DOWN_CAMERA, &CDlgSetting::OnBnClickedCheckDownCamera)
    ON_BN_CLICKED(IDCANCEL, &CDlgSetting::OnBnClickedCancel)
    ON_BN_CLICKED(IDOK, &CDlgSetting::OnBnClickedOk)
    ON_BN_CLICKED(IDC_AUTO_TRAY, &CDlgSetting::OnBnClickedAutoTray)
    ON_BN_CLICKED(IDC_CHECK_NOZZLE0, &CDlgSetting::OnBnClickedCheckNozzle0)
    ON_BN_CLICKED(IDC_CHECK_NOZZLE1, &CDlgSetting::OnBnClickedCheckNozzle1)
    ON_BN_CLICKED(IDC_CHECK_NOZZLE2, &CDlgSetting::OnBnClickedCheckNozzle2)
    ON_BN_CLICKED(IDC_CHECK_NOZZLE3, &CDlgSetting::OnBnClickedCheckNozzle3)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE0, &CDlgSetting::OnBnClickedRadioRotateAngle0)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE90, &CDlgSetting::OnBnClickedRadioRotateAngle90)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLEN90, &CDlgSetting::OnBnClickedRadioRotateAnglen90)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE180, &CDlgSetting::OnBnClickedRadioRotateAngle180)
    ON_BN_CLICKED(IDC_CHECK_DOWN_CAMERA_OUT, &CDlgSetting::OnBnClickedCheckDownCameraOut)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE7, &CDlgSetting::OnBnClickedRadioRotateAngle7)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE92, &CDlgSetting::OnBnClickedRadioRotateAngle92)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLEN92, &CDlgSetting::OnBnClickedRadioRotateAnglen92)
    ON_BN_CLICKED(IDC_RADIO_ROTATE_ANGLE182, &CDlgSetting::OnBnClickedRadioRotateAngle182)
    //ON_BN_CLICKED(IDC_CHECK_CONVERT_CHIP, &CDlgSetting::OnBnClickedCheckConvertChip)
    ON_BN_CLICKED(IDC_CHECK_CONVERT_CHIP, &CDlgSetting::OnBnClickedCheckConvertChip)
    ON_BN_CLICKED(IDC_CHECK_DOOR, &CDlgSetting::OnBnClickedCheckDoor)
    ON_BN_CLICKED(IDC_CHECK_TAPECAMERA, &CDlgSetting::OnBnClickedCheckTape)
    ON_BN_CLICKED(IDC_CHECK_Tapeout, &CDlgSetting::OnBnClickedCheckTapeout)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetting message handlers

void CDlgSetting::OnCancel()
{

    CDialog::OnCancel();
}
#include   <iostream>    
#include   <io.h>    
#include   <direct.h>    
#include   <string>    
#include   <vector>    
#include   <iomanip>    
#include   <ctime>    
using   namespace   std;
//获取文件列表
//path 文件所在文件夹路径
//files 
void getFiles(string path, vector<string>& files);
void getFiles(string path, vector<string>& files)
{
    //文件句柄    
    long   hFile = 0;
    //文件信息    
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
    {
        do {
            //如果是目录,迭代之  
            //如果不是,加入列表  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("/").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("/").append(fileinfo.name));
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}
BOOL CDlgSetting::OnInitDialog()
{
    CDialog::OnInitDialog();
    //加载模型列表
    vector<string>   filesSocket;
    getFiles("C:\/S100\/Model\/Socket", filesSocket);
    // print the files get  
    for (int j = 0; j < filesSocket.size(); ++j)
    {
        m_combSocketModel.AddString(filesSocket[j].c_str());
        if (gm_strSocketModel.Compare(filesSocket[j].c_str()) == 0)
        {
            m_combSocketModel.SetCurSel(j);
        }
        //cout << files[j] << endl;
    }

    vector<string>   filesChip;
    getFiles("C:\/S100\/Model\/Chip", filesChip);
    // print the files get  
    for (int j = 0; j < filesChip.size(); ++j)
    {
        m_combChipModel.AddString(filesChip[j].c_str());
        if (gm_strChipModel.Compare(filesChip[j].c_str()) == 0)
        {
            m_combChipModel.SetCurSel(j);
        }
        //cout << files[j] << endl;
    }
    //m_combSocketModel.SetWindowTextA(gm_strSocketModel);
    //m_combChipModel.SetWindowTextA(gm_strChipModel);
    //----------------------------------------------------------------
    //初始化tray盘信息
    SetDlgItemInt(IDC_SETTING_XN, gm_dataTray.xn);
    SetDlgItemInt(IDC_SETTING_YN, gm_dataTray.yn);
    SetDlgItemInt(IDC_SETTING_NG_XN, gm_dataNG.xn);
    SetDlgItemInt(IDC_SETTING_NG_YN, gm_dataNG.yn);
    SetDlgItemInt(IDC_SETTING_SUPPLY_XN, gm_dataStock.xn);
    SetDlgItemInt(IDC_SETTING_SUPPLY_YN, gm_dataStock.yn);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //初始化xy轴速度信息
    if (gm_uInfo.xySpeed == 5)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_XY1);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.xySpeed == 4)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_XY2);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.xySpeed == 3)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_XY3);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.xySpeed == 2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_XY4);
        pBtn->SetCheck(1);
    }
    else
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_XY5);
        pBtn->SetCheck(1);
    }
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //初始化z轴速度信息
    if (gm_uInfo.zSpeed == 5)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_Z1);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.zSpeed == 4)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_Z2);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.zSpeed == 3)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_Z3);
        pBtn->SetCheck(1);
    }
    else if (gm_uInfo.zSpeed == 2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_Z4);
        pBtn->SetCheck(1);
    }
    else
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SETTING_Z5);
        pBtn->SetCheck(1);
    }
    //--------------------------------------------------------------*/
    //初始化进出料旋转角度
    if (gm_nRotateAngleIn == 0)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE0);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleIn == 2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE90);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleIn == -2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLEN90);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleIn == 1)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE180);
        pBtn->SetCheck(1);
    }

    if (gm_nRotateAngleOut == 0)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE7);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleOut == 2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE92);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleOut == -2)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLEN92);
        pBtn->SetCheck(1);
    }
    else if (gm_nRotateAngleOut == 1)
    {
        CButton *pBtn = (CButton*)GetDlgItem(IDC_RADIO_ROTATE_ANGLE182);
        pBtn->SetCheck(1);
    }

    //----------------------------------------------------------------
    //自动tray盘复选框
    CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK33);
    if (gm_bAutoTray)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //补料复选框
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_STOCK);
    if (gm_bStock)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //Tape进料出料视觉检测复选框
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_CHECK);
    if (gm_bTapeCheck)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //下相机复选框
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_DOWN_CAMERA);
    if (gm_bDownCameraIn)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    pBtn = (CButton*)GetDlgItem(IDC_CHECK_DOWN_CAMERA_OUT);
    if (gm_bDownCameraOut)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);
    //--------------------------------------------------------------*/

    CString sp;

    //----------------------------------------------------------------
    //取料高度偏差
    float f = gm_uInfo.nGetTrayHeight;
    f = f / ZGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_GET, sp);

    //放料高度偏差
    f = gm_uInfo.nPutTrayHeight;
    f = f / ZGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_PUT, sp);

    //socket取料高度偏差
    f = gm_uInfo.nGetSocketHeight;
    f = f / ZGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_S_GET, sp);

    //socket放料高度偏差
    f = gm_uInfo.nPutSocketHeight;
    f = f / ZGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_S_PUT, sp);

    //socket测高时间
    f = gm_uInfo.nSocketTime;
    f = f / 1000;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_SOCKET_TIME, sp);

    //tray盘测高时间
    f = gm_uInfo.nTrayTime;
    f = f / 1000;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_SEARCHHEIGHT_TIME, sp);

    //烧录死机等待时间
    f = gm_uInfo.nElapsed;
    f = f / 1000;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_ELAPSED, sp);

    //上相机像素换算
    f = gm_uInfo.nPixelsPM_up;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_PIXELS_UP, sp);

    //下相机像素换算
    f = gm_uInfo.nPixelsPM_down;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_PIXELS_DOWN, sp);

    //芯片规格
    f = gm_uInfo.nChipLength;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_CHIP_LENGTH, sp);

    f = gm_uInfo.nChipWidth;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_CHIP_WIDTH, sp);

    //Socket规格
    f = gm_uInfo.nSocketLength;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_SOCKET_LENGTH, sp);

    f = gm_uInfo.nSocketWidth;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_SOCKET_WIDTH, sp);

    //tape规格
    f = gm_uInfo.nTapeLength;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_TAPE_LENGTH2, sp);

    f = gm_uInfo.nTapeWidth;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_TAPE_WIDTH2, sp);

    //补偿参数
    f = gm_uInfo.nOffsetInX;
    f = f / XGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_IN_X, sp);

    f = gm_uInfo.nOffsetInY;
    f = f / YGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_IN_Y, sp);

    f = gm_uInfo.nOffsetOutX;
    f = f / XGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_OUT_X, sp);

    f = gm_uInfo.nOffsetOutY;
    f = f / YGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_OUT_Y, sp);

    f = gm_uInfo.nOffsetSocketX;
    f = f / XGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_SOCKET_X, sp);

    f = gm_uInfo.nOffsetSocketY;
    f = f / YGUIDE;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_OFFSET_SOCKET_Y, sp);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //进出料方式选择
    pBtn = (CButton*)GetDlgItem(IDC_MODE1);
    if (gm_uInfo.nWorkMode == 0)
    {
        pBtn->SetCheck(TRUE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE4);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE5);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE6);
        pBtn->SetCheck(FALSE);
    }
    else if (gm_uInfo.nWorkMode == 1)
    {
        pBtn = (CButton*)GetDlgItem(IDC_MODE4);
        pBtn->SetCheck(TRUE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE1);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE5);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE6);
        pBtn->SetCheck(FALSE);
    }
    else if (gm_uInfo.nWorkMode == 2)
    {
        pBtn = (CButton*)GetDlgItem(IDC_MODE4);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE1);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE5);
        pBtn->SetCheck(TRUE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE6);
        pBtn->SetCheck(FALSE);
    }
    else if (gm_uInfo.nWorkMode == 3)
    {
        pBtn = (CButton*)GetDlgItem(IDC_MODE4);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE1);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE5);
        pBtn->SetCheck(FALSE);
        pBtn = (CButton*)GetDlgItem(IDC_MODE6);
        pBtn->SetCheck(TRUE);
    }
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //初始化编带信息
    SetDlgItemInt(IDC_EDIT_TAPE_COUNT, gm_uInfo.nTapeIn);

    f = gm_uInfo.fTapeInSpeed;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_TAPE_IN_SPEED, sp);

    f = gm_uInfo.fTapeOutSpeed;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_TAPE_OUT_SPEED, sp);

    f = gm_uInfo.fTapeInStep;
    sp.Format("%0.2f", f);
    SetDlgItemText(IDC_EDIT_TAPE_LENGTH, sp);

    SetDlgItemInt(IDC_EDIT_TAPE_COVER_TIME, gm_uInfo.nCoverTime);
    //--------------------------------------------------------------*/
    //----------------------------------------------------------------
    //初始化转料选项
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_CONVERT_CHIP);
    if (gm_bConvertChip)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //初始化门禁选项
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_DOOR);
    if (gm_bDoorAlarm)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //初始化出料检测选项
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_Tapeout);
    if (gm_bTapeOutCheck)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //初始化出料相机选项
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_TAPECAMERA);
    if (gm_bTapeCamera)
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //初始化吸嘴选择
    pBtn = (CButton*)GetDlgItem(IDC_CHECK_NOZZLE0);
    if (gm_bNozzleUsed[0])
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    pBtn = (CButton*)GetDlgItem(IDC_CHECK_NOZZLE1);
    if (gm_bNozzleUsed[1])
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    pBtn = (CButton*)GetDlgItem(IDC_CHECK_NOZZLE2);
    if (gm_bNozzleUsed[2])
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    pBtn = (CButton*)GetDlgItem(IDC_CHECK_NOZZLE3);
    if (gm_bNozzleUsed[3])
        pBtn->SetCheck(1);
    else
        pBtn->SetCheck(0);

    //初始化客户信息
    SetDlgItemText(IDC_EDIT_CUSTOMER, gm_sdInfo.strCustomer);
    SetDlgItemText(IDC_EDIT_WORK_NUM, gm_sdInfo.strWorkNo);
    sp.Format("%d", gm_sdInfo.nLotNum);
    SetDlgItemText(IDC_EDIT_WORKNUM_SUM, sp);
    SetDlgItemText(IDC_EDIT_CHIP_MODEL, gm_sdInfo.strICModel);
    SetDlgItemText(IDC_EDIT_CHIP_BRAND, gm_sdInfo.strICBrand);
    SetDlgItemText(IDC_EDIT_OPERATOR, gm_sdInfo.strOperator);
    SetDlgItemText(IDC_EDIT_CHECK_SUM, gm_sdInfo.strCheckSum);

    //匹配阈值
    sp.Format("%0.2f", gm_uInfo.fCheckScore);
    SetDlgItemText(IDC_EDIT_CHECKSCORE, sp);
    sp.Format("%0.2f", gm_uInfo.fCheckScoreDown);
    SetDlgItemText(IDC_EDIT_CHECKSCORE_DOWN, sp);
    sp.Format("%d", gm_uInfo.nNozzleDelay);
    SetDlgItemText(IDC_EDIT_NOZZLE_DELAY, sp);


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetting::OnSettingXy1()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.xySpeed = 5;
}

void CDlgSetting::OnSettingXy2()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.xySpeed = 4;
}

void CDlgSetting::OnSettingXy3()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.xySpeed = 3;
}

void CDlgSetting::OnSettingXy4()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.xySpeed = 2;
}

void CDlgSetting::OnSettingXy5()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.xySpeed = 1;
}
//________________________________________________________

void CDlgSetting::OnSettingZ1()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.zSpeed = 5;
}

void CDlgSetting::OnSettingZ2()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.zSpeed = 4;
}

void CDlgSetting::OnSettingZ3()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.zSpeed = 3;
}
void CDlgSetting::OnSettingZ4()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.zSpeed = 2;
}

void CDlgSetting::OnSettingZ5()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.zSpeed = 1;
}



void CDlgSetting::OnMode1()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.nWorkMode = TRAY_TO_TRAY;
}

void CDlgSetting::OnMode4()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.nWorkMode = TAPE_TO_TRAY;
}

void CDlgSetting::OnMode5()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.nWorkMode = TAPE_TO_TAPE;
}

void CDlgSetting::OnMode6()
{
    // TODO: Add your control notification handler code here
    gm_uInfo.nWorkMode = TRAY_TO_TAPE;
}

void CDlgSetting::OnCheckStock()
{
    // TODO: Add your control notification handler code here
    if (gm_bStock == 1)
        gm_bStock = 0;
    else if (gm_bStock == 0)
        gm_bStock = 1;
}



void CDlgSetting::OnBnClickedCheckCheck()
{
    // TODO:  在此添加控件通知处理程序代码
    if (gm_bTapeCheck == 1)
        gm_bTapeCheck = 0;
    else if (gm_bTapeCheck == 0)
        gm_bTapeCheck = 1;
}


void CDlgSetting::OnBnClickedCheckDownCamera()
{
    // TODO:  在此添加控件通知处理程序代码
    if (gm_bDownCameraIn == 1)
        gm_bDownCameraIn = 0;
    else if (gm_bDownCameraIn == 0)
        gm_bDownCameraIn = 1;
}

void CDlgSetting::OnBnClickedCheckDownCameraOut()
{
    if (gm_bDownCameraOut == 1)
        gm_bDownCameraOut = 0;
    else if (gm_bDownCameraOut == 0)
        gm_bDownCameraOut = 1;
}

void CDlgSetting::OnBnClickedCancel()
{
    // TODO:  在此添加控件通知处理程序代码
    CDialog::OnCancel();
}


void CDlgSetting::OnBnClickedOk()
{
    if (gm_bStock)
        gm_nTotalStock = gm_dataStock.xn*gm_dataStock.yn;
    else
        gm_nTotalStock = 0;

    CString str;
    //int index = m_cmbPort.GetCurSel();
    m_combSocketModel.GetLBText(m_combSocketModel.GetCurSel(), str);
    gm_strSocketModel = str;
    m_combChipModel.GetLBText(m_combChipModel.GetCurSel(), str);
    gm_strChipModel = str;
    // TODO:  在此添加控件通知处理程序代码
    //设置文件保存路径
    //CIniFile inf;
    //inf.SetIniFileName(gm_strPositionFile);
    //----------------------------------------------------------------
    //设置tray盘规格
    CString warning;
    int tray_x = GetDlgItemInt(IDC_SETTING_XN);
    int tray_y = GetDlgItemInt(IDC_SETTING_YN);
    int ng_x = GetDlgItemInt(IDC_SETTING_NG_XN);
    int ng_y = GetDlgItemInt(IDC_SETTING_NG_YN);
    int supply_x = GetDlgItemInt(IDC_SETTING_SUPPLY_XN);
    int supply_y = GetDlgItemInt(IDC_SETTING_SUPPLY_YN);
    if (tray_x < 2 || tray_x > 100 || tray_y < 2 || tray_y > 100)
    {
        warning = "Tray盘规格设置不合理，x取2到100，y取2到100!";
        AfxMessageBox(warning);
        SetDlgItemInt(IDC_SETTING_XN, gm_dataTray.xn);
        SetDlgItemInt(IDC_SETTING_YN, gm_dataTray.yn);
        return;
    }
    if (ng_x < 2 || ng_x > 30 || ng_y < 2 || ng_y > 45)
    {
        warning = "NG盘规格设置不合理，x取2到30，y取2到45！";
        AfxMessageBox(warning);
        SetDlgItemInt(IDC_SETTING_NG_XN, gm_dataNG.xn);
        SetDlgItemInt(IDC_SETTING_NG_YN, gm_dataNG.yn);
        return;
    }
    if (supply_x < 2 || supply_x > 30 || supply_y < 2 || supply_y > 45)
    {
        warning = "补料盘盘规格设置不合理，x取2到30，y取2到45!";
        AfxMessageBox(warning);
        SetDlgItemInt(IDC_SETTING_SUPPLY_XN, gm_dataStock.xn);
        SetDlgItemInt(IDC_SETTING_SUPPLY_YN, gm_dataStock.yn);
        return;
    }
    gm_dataTray.xn = tray_x;
    gm_dataTray.yn = tray_y;
    gm_dataNG.xn = ng_x;
    gm_dataNG.yn = ng_y;
    gm_dataStock.xn = supply_x;
    gm_dataStock.yn = supply_y;
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //设置取放料高度偏差
    //CString str;
    GetDlgItemText(IDC_EDIT_OFFSET_GET, str);
    float tray_get_hight_offset = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_PUT, str);
    float tray_put_hight_offset = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_S_GET, str);
    float socket_get_hight_offset = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_S_PUT, str);
    float socket_put_hight_offset = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    gm_uInfo.nGetTrayHeight = int(tray_get_hight_offset*ZGUIDE);
    gm_uInfo.nPutTrayHeight = int(tray_put_hight_offset*ZGUIDE);
    gm_uInfo.nGetSocketHeight = int(socket_get_hight_offset*ZGUIDE);
    gm_uInfo.nPutSocketHeight = int(socket_put_hight_offset*ZGUIDE);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //设置死机等待时间
    GetDlgItemText(IDC_EDIT_ELAPSED, str);
    float socket_wait_time = atof(str.GetBuffer(0));
    str.ReleaseBuffer();
    gm_uInfo.nElapsed = int(socket_wait_time * 1000);
    //inf.WriteIntNumber("System", "elapsed_time", gm_uInfo.nElapsed);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //设置相机参数
    //芯片规格 长和宽
    GetDlgItemText(IDC_EDIT_CHIP_LENGTH, str);
    float chip_length = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_CHIP_WIDTH, str);
    float chip_width = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    //设置socket规格
    GetDlgItemText(IDC_EDIT_SOCKET_LENGTH, str);
    float socket_length = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_SOCKET_WIDTH, str);
    float socket_width = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    //设置tape规格
    GetDlgItemText(IDC_EDIT_TAPE_LENGTH2, str);
    gm_uInfo.nTapeLength = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_TAPE_WIDTH2, str);
    gm_uInfo.nTapeWidth = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    //上相机像素单位
    GetDlgItemText(IDC_EDIT_PIXELS_UP, str);
    float up_pixels = atof(str.GetBuffer(0));
    str.ReleaseBuffer();
    //下相机像素单位
    GetDlgItemText(IDC_EDIT_PIXELS_DOWN, str);
    float down_pixels = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    gm_uInfo.nPixelsPM_up = up_pixels;
    gm_uInfo.nPixelsPM_down = down_pixels;
    gm_uInfo.nChipLength = chip_length;
    gm_uInfo.nChipWidth = chip_width;
    gm_uInfo.nSocketLength = socket_length;
    gm_uInfo.nSocketWidth = socket_width;
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //设置取放料补偿
    GetDlgItemText(IDC_EDIT_OFFSET_IN_X, str);
    float fOffsetInX = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_IN_Y, str);
    float fOffsetInY = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_OUT_X, str);
    float fOffsetOutX = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_OUT_Y, str);
    float fOffsetOutY = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_SOCKET_X, str);
    float fOffsetSocketX = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OFFSET_SOCKET_Y, str);
    float fOffsetSocketY = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    gm_uInfo.nOffsetInX = int(fOffsetInX * XGUIDE);
    gm_uInfo.nOffsetInY = int(fOffsetInY * YGUIDE);
    gm_uInfo.nOffsetOutX = int(fOffsetOutX * XGUIDE);
    gm_uInfo.nOffsetOutY = int(fOffsetOutY * YGUIDE);
    gm_uInfo.nOffsetSocketX = int(fOffsetSocketX * XGUIDE);
    gm_uInfo.nOffsetSocketY = int(fOffsetSocketY * YGUIDE);
    //--------------------------------------------------------------*/

    //----------------------------------------------------------------
    //设置测高时间
    GetDlgItemText(IDC_EDIT_SOCKET_TIME, str);
    float socket_time = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_SEARCHHEIGHT_TIME, str);
    float search_height_time = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    gm_uInfo.nSocketTime = int(socket_time * 1000);
    gm_uInfo.nTrayTime = int(search_height_time * 1000);
    //--------------------------------------------------------------*/

    //设置匹配阈值
    GetDlgItemText(IDC_EDIT_CHECKSCORE, str);
    gm_uInfo.fCheckScore = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_CHECKSCORE_DOWN, str);
    gm_uInfo.fCheckScoreDown = atof(str.GetBuffer(0));
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_NOZZLE_DELAY, str);
    gm_uInfo.nNozzleDelay = atoll(str);
    str.ReleaseBuffer();

    //----------------------------------------------------------------
    //设置编带各参数
    int nn = GetDlgItemInt(IDC_EDIT_TAPE_COUNT);//满圈数量
    gm_uInfo.nTapeIn = nn;
    //inf.WriteIntNumber("Tape", "in_count", gm_uInfo.nTapeIn);//满圈数量

    GetDlgItemText(IDC_EDIT_TAPE_IN_SPEED, str);//进料速度
    float fsp = atof(str.GetBuffer(0));
    str.ReleaseBuffer();
    gm_uInfo.fTapeInSpeed = fsp;
    //inf.WriteFloatNumber("Tape", "speedIn", gm_uInfo.fTapeInSpeed);

    GetDlgItemText(IDC_EDIT_TAPE_OUT_SPEED, str);//出料速度
    fsp = atof(str.GetBuffer(0));
    str.ReleaseBuffer();
    gm_uInfo.fTapeOutSpeed = fsp;
    //inf.WriteFloatNumber("Tape", "speedOut", gm_uInfo.fTapeOutSpeed);

    GetDlgItemText(IDC_EDIT_TAPE_LENGTH, str);//编带格长
    fsp = atof(str.GetBuffer(0));
    str.ReleaseBuffer();
    gm_uInfo.fTapeInStep = fsp;
    gm_uInfo.fTapeOutStep = fsp;
    //inf.WriteFloatNumber("Tape", "stepIn", gm_uInfo.fTapeInStep);
    //inf.WriteFloatNumber("Tape", "stepOut", gm_uInfo.fTapeOutStep);

    nn = GetDlgItemInt(IDC_EDIT_TAPE_COVER_TIME);//热封时间
    gm_uInfo.nCoverTime = nn;
    //inf.WriteIntNumber("Tape", "coverTime", gm_uInfo.nCoverTime);
    //--------------------------------------------------------------*/

    GetDlgItemText(IDC_EDIT_CUSTOMER, str);
    gm_sdInfo.strCustomer = str;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_WORK_NUM, str);
    gm_sdInfo.strWorkNo = str;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_WORKNUM_SUM, str);
    gm_sdInfo.nLotNum = atoll(str);
    gm_nSum = gm_sdInfo.nLotNum;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_CHIP_MODEL, str);
    gm_sdInfo.strICModel = str;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_CHIP_BRAND, str);
    gm_sdInfo.strICBrand = str;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_OPERATOR, str);
    gm_sdInfo.strOperator = str;
    str.ReleaseBuffer();

    GetDlgItemText(IDC_EDIT_CHECK_SUM, str);
    gm_sdInfo.strCheckSum = str;
    str.ReleaseBuffer();


    CDialog::OnOK();
}


void CDlgSetting::OnBnClickedAutoTray()
{
    if (gm_bAutoTray == 1)
        gm_bAutoTray = 0;
    else if (gm_bAutoTray == 0)
        gm_bAutoTray = 1;
}


void CDlgSetting::OnBnClickedCheckNozzle0()
{
    if (gm_bNozzleUsed[0] == 1)
        gm_bNozzleUsed[0] = 0;
    else if (gm_bNozzleUsed[0] == 0)
        gm_bNozzleUsed[0] = 1;
}


void CDlgSetting::OnBnClickedCheckNozzle1()
{
    if (gm_bNozzleUsed[1] == 1)
        gm_bNozzleUsed[1] = 0;
    else if (gm_bNozzleUsed[1] == 0)
        gm_bNozzleUsed[1] = 1;
}


void CDlgSetting::OnBnClickedCheckNozzle2()
{
    if (gm_bNozzleUsed[2] == 1)
        gm_bNozzleUsed[2] = 0;
    else if (gm_bNozzleUsed[2] == 0)
        gm_bNozzleUsed[2] = 1;
}


void CDlgSetting::OnBnClickedCheckNozzle3()
{
    if (gm_bNozzleUsed[3] == 1)
        gm_bNozzleUsed[3] = 0;
    else if (gm_bNozzleUsed[3] == 0)
        gm_bNozzleUsed[3] = 1;
}

//旋转角度设定
void CDlgSetting::OnBnClickedRadioRotateAngle0()
{
    gm_nRotateAngleIn = 0;
}


void CDlgSetting::OnBnClickedRadioRotateAngle90()
{
    gm_nRotateAngleIn = 2;
}


void CDlgSetting::OnBnClickedRadioRotateAnglen90()
{
    gm_nRotateAngleIn = -2;
}


void CDlgSetting::OnBnClickedRadioRotateAngle180()
{
    gm_nRotateAngleIn = 1;
}




void CDlgSetting::OnBnClickedRadioRotateAngle7()
{
    gm_nRotateAngleOut = 0;
}


void CDlgSetting::OnBnClickedRadioRotateAngle92()
{
    gm_nRotateAngleOut = 2;
}


void CDlgSetting::OnBnClickedRadioRotateAnglen92()
{
    gm_nRotateAngleOut = -2;
}


void CDlgSetting::OnBnClickedRadioRotateAngle182()
{
    gm_nRotateAngleOut = 1;
}


void CDlgSetting::OnBnClickedCheckConvertChip()
{
    if (gm_bConvertChip == 1)
        gm_bConvertChip = 0;
    else if (gm_bConvertChip == 0)
        gm_bConvertChip = 1;
}




void CDlgSetting::OnBnClickedCheckDoor()
{
    if (gm_bDoorAlarm == 1)
        gm_bDoorAlarm = 0;
    else if (gm_bDoorAlarm == 0)
        gm_bDoorAlarm = 1;
}


void CDlgSetting::OnBnClickedCheckTape()
{
    if (gm_bTapeCamera == 1)
        gm_bTapeCamera = 0;
    else if (gm_bTapeCamera == 0)
        gm_bTapeCamera = 1;
}


void CDlgSetting::OnBnClickedCheckTapeout()
{
    if (gm_bTapeOutCheck == 1)
        gm_bTapeOutCheck = 0;
    else if (gm_bTapeOutCheck == 0)
        gm_bTapeOutCheck = 1;
}
