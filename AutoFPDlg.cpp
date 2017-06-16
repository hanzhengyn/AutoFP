// AutoFPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoFP.h"
#include "AutoFPDlg.h"
#include "dlgPosition.h"
#include "DlgSetting.h"
#include "BurningPort.h"
#include "MotionAct.h"
#include "IoSd404.h"
#include "GxIAPI.h"
#include <dbt.h>
#include <setupapi.h>
#include "afxwin.h"
//#include "json\json.h"
#include "HttpContext.h"
#include "HttpApplication.h"
#include "ExcelDataDlg.h"
#pragma comment(lib,"setupapi")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  MAXBUFLEN 256  
#define PORT 7100 
#define DEST_IP_ADDR "127.0.0.1" //想要链接的目标Server address

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define CLASS_SIZE 500
class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    // Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:

    // 显示更新日志
    CEdit m_editUpdataLog;
    virtual BOOL OnInitDialog();
    CString m_strUpdateLog;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_strUpdateLog(_T(""))
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_EDIT_UPDATA_LOG, m_editUpdataLog);
    DDX_Text(pDX, IDC_EDIT_UPDATA_LOG, m_strUpdateLog);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoFPDlg dialog

CAutoFPDlg::CAutoFPDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CAutoFPDlg::IDD, pParent)

{
    //{{AFX_DATA_INIT(CGxGetImageDlg)
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32

    m_bCameraOpenUp = FALSE;
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bmpBg.LoadBitmap(IDB_COLOR);
    m_xn = 12;
    m_yn = 20;
    m_nTime = 0;
    m_guid = NewGuid;
    m_lpGuid = &m_guid;
    //m_pTrayStatus = new BYTE[FAIL_XN*FAIL_YN];
    //for(int i=0;i<FAIL_XN*FAIL_YN;i++)
    //	m_pTrayStatus[i] = 0;
    m_hDevice = NULL;
    m_bTimerX = FALSE;
    m_bTimerY = FALSE;
    m_bTimerZ = FALSE;
    m_bProgrammer = FALSE;
    m_bCameraOpenDown = FALSE;
    m_bCameraOpenUp = FALSE;
    m_bCameraOpenTape = FALSE;
    m_bSocketConnect = FALSE;
    //m_hv_ModelID = NULL;
    //m_hv_CheckRow = NULL;
    //m_hv_CheckAngle = NULL;
    //m_hv_CheckScore = NULL;

}

void CAutoFPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAutoFPDlg)
    DDX_Control(pDX, IDC_TAPEIN_CUR, m_titleTapeIn);
    DDX_Control(pDX, IDC_TAPEOUT_CUR, m_titleTapeOut);
    DDX_Control(pDX, IDC_COUNT_TAPEOUT, m_lbTapeOut);
    DDX_Control(pDX, IDC_COUNT_TAPEIN, m_lbTapeIn);
    DDX_Control(pDX, IDC_SOCKET_FAIL_11, m_fail11);
    DDX_Control(pDX, IDC_SOCKET_FAIL_12, m_fail12);
    DDX_Control(pDX, IDC_SOCKET_FAIL_13, m_fail13);
    DDX_Control(pDX, IDC_SOCKET_FAIL_14, m_fail14);
    DDX_Control(pDX, IDC_SOCKET_GOOD_11, m_good11);
    DDX_Control(pDX, IDC_SOCKET_GOOD_12, m_good12);
    DDX_Control(pDX, IDC_SOCKET_GOOD_13, m_good13);
    DDX_Control(pDX, IDC_SOCKET_GOOD_14, m_good14);

    DDX_Control(pDX, IDC_SOCKET_FAIL_21, m_fail21);
    DDX_Control(pDX, IDC_SOCKET_FAIL_22, m_fail22);
    DDX_Control(pDX, IDC_SOCKET_FAIL_23, m_fail23);
    DDX_Control(pDX, IDC_SOCKET_FAIL_24, m_fail24);
    DDX_Control(pDX, IDC_SOCKET_GOOD_21, m_good21);
    DDX_Control(pDX, IDC_SOCKET_GOOD_22, m_good22);
    DDX_Control(pDX, IDC_SOCKET_GOOD_23, m_good23);
    DDX_Control(pDX, IDC_SOCKET_GOOD_24, m_good24);

    DDX_Control(pDX, IDC_SOCKET_FAIL_31, m_fail31);
    DDX_Control(pDX, IDC_SOCKET_FAIL_32, m_fail32);
    DDX_Control(pDX, IDC_SOCKET_FAIL_33, m_fail33);
    DDX_Control(pDX, IDC_SOCKET_FAIL_34, m_fail34);
    DDX_Control(pDX, IDC_SOCKET_GOOD_31, m_good31);
    DDX_Control(pDX, IDC_SOCKET_GOOD_32, m_good32);
    DDX_Control(pDX, IDC_SOCKET_GOOD_33, m_good33);
    DDX_Control(pDX, IDC_SOCKET_GOOD_34, m_good34);

    DDX_Control(pDX, IDC_SOCKET_FAIL_41, m_fail41);
    DDX_Control(pDX, IDC_SOCKET_FAIL_42, m_fail42);
    DDX_Control(pDX, IDC_SOCKET_FAIL_43, m_fail43);
    DDX_Control(pDX, IDC_SOCKET_FAIL_44, m_fail44);
    DDX_Control(pDX, IDC_SOCKET_GOOD_41, m_good41);
    DDX_Control(pDX, IDC_SOCKET_GOOD_42, m_good42);
    DDX_Control(pDX, IDC_SOCKET_GOOD_43, m_good43);
    DDX_Control(pDX, IDC_SOCKET_GOOD_44, m_good44);

    DDX_Control(pDX, IDC_SOCKET_FAIL_51, m_fail51);
    DDX_Control(pDX, IDC_SOCKET_FAIL_52, m_fail52);
    DDX_Control(pDX, IDC_SOCKET_FAIL_53, m_fail53);
    DDX_Control(pDX, IDC_SOCKET_FAIL_54, m_fail54);
    DDX_Control(pDX, IDC_SOCKET_GOOD_51, m_good51);
    DDX_Control(pDX, IDC_SOCKET_GOOD_52, m_good52);
    DDX_Control(pDX, IDC_SOCKET_GOOD_53, m_good53);
    DDX_Control(pDX, IDC_SOCKET_GOOD_54, m_good54);

    DDX_Control(pDX, IDC_SOCKET_FAIL_61, m_fail61);
    DDX_Control(pDX, IDC_SOCKET_FAIL_62, m_fail62);
    DDX_Control(pDX, IDC_SOCKET_FAIL_63, m_fail63);
    DDX_Control(pDX, IDC_SOCKET_FAIL_64, m_fail64);
    DDX_Control(pDX, IDC_SOCKET_GOOD_61, m_good61);
    DDX_Control(pDX, IDC_SOCKET_GOOD_62, m_good62);
    DDX_Control(pDX, IDC_SOCKET_GOOD_63, m_good63);
    DDX_Control(pDX, IDC_SOCKET_GOOD_64, m_good64);

    DDX_Control(pDX, IDC_SOCKET_FAIL_71, m_fail71);
    DDX_Control(pDX, IDC_SOCKET_FAIL_72, m_fail72);
    DDX_Control(pDX, IDC_SOCKET_FAIL_73, m_fail73);
    DDX_Control(pDX, IDC_SOCKET_FAIL_74, m_fail74);
    DDX_Control(pDX, IDC_SOCKET_GOOD_71, m_good71);
    DDX_Control(pDX, IDC_SOCKET_GOOD_72, m_good72);
    DDX_Control(pDX, IDC_SOCKET_GOOD_73, m_good73);
    DDX_Control(pDX, IDC_SOCKET_GOOD_74, m_good74);

    DDX_Control(pDX, IDC_SOCKET_FAIL_81, m_fail81);
    DDX_Control(pDX, IDC_SOCKET_FAIL_82, m_fail82);
    DDX_Control(pDX, IDC_SOCKET_FAIL_83, m_fail83);
    DDX_Control(pDX, IDC_SOCKET_FAIL_84, m_fail84);
    DDX_Control(pDX, IDC_SOCKET_GOOD_81, m_good81);
    DDX_Control(pDX, IDC_SOCKET_GOOD_82, m_good82);
    DDX_Control(pDX, IDC_SOCKET_GOOD_83, m_good83);
    DDX_Control(pDX, IDC_SOCKET_GOOD_84, m_good84);

    DDX_Control(pDX, IDC_SOCKET_FAIL_91, m_fail91);
    DDX_Control(pDX, IDC_SOCKET_FAIL_92, m_fail92);
    DDX_Control(pDX, IDC_SOCKET_FAIL_93, m_fail93);
    DDX_Control(pDX, IDC_SOCKET_FAIL_94, m_fail94);
    DDX_Control(pDX, IDC_SOCKET_GOOD_91, m_good91);
    DDX_Control(pDX, IDC_SOCKET_GOOD_92, m_good92);
    DDX_Control(pDX, IDC_SOCKET_GOOD_93, m_good93);
    DDX_Control(pDX, IDC_SOCKET_GOOD_94, m_good94);

    DDX_Control(pDX, IDC_SOCKET_FAIL_101, m_fail101);
    DDX_Control(pDX, IDC_SOCKET_FAIL_102, m_fail102);
    DDX_Control(pDX, IDC_SOCKET_FAIL_103, m_fail103);
    DDX_Control(pDX, IDC_SOCKET_FAIL_104, m_fail104);
    DDX_Control(pDX, IDC_SOCKET_GOOD_101, m_good101);
    DDX_Control(pDX, IDC_SOCKET_GOOD_102, m_good102);
    DDX_Control(pDX, IDC_SOCKET_GOOD_103, m_good103);
    DDX_Control(pDX, IDC_SOCKET_GOOD_104, m_good104);
    //数据显示
    DDX_Control(pDX, IDC_MAIN_YIELD, m_ctrlYield);
    DDX_Control(pDX, IDC_MAIN_RUN_TIME, m_ctrlUsedTime);
    DDX_Control(pDX, IDC_MAIN_PASS, m_ctrlGood);
    DDX_Control(pDX, IDC_MAIN_PROG_NG, m_ctrlFail);
    //数据标签
    DDX_Control(pDX, IDC_STATIC_YIELD, m_lblYield);
    DDX_Control(pDX, IDC_STATIC_TIME, m_lblUsedTime);
    DDX_Control(pDX, IDC_STATIC_PASS, m_lblGood);
    DDX_Control(pDX, IDC_STATIC_SYS_NG, m_lblFail);

    DDX_Control(pDX, IDC_STATIC_ADMIN, m_lblAdmin);

    DDX_Control(pDX, IDC_MAIN_STATUS, m_ctrlStatus);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_CHECK_ALLSELECT, m_checkAllSelect);
    DDX_Control(pDX, IDC_PROGRESS_SOCKET1, m_progressSocket1);
    DDX_Control(pDX, IDC_EDIT_MESSAGE, m_editLog);
    DDX_Control(pDX, IDC_STATIC_PROJECT_NAME, m_lblProjectName);
    DDX_Control(pDX, IDC_STATIC_CHECK_SUM, m_lblCheckSum);
    DDX_Control(pDX, IDC_STATIC_CHIP_MODEL, m_lblChipModel);
    DDX_Control(pDX, IDC_STATIC_CHIP_BRAND, m_lblChipBrand);
    DDX_Control(pDX, IDC_STATIC_PROG_NG, m_lblProgNG);
    DDX_Control(pDX, IDC_STATIC_START_TIME, m_lblStartTime);
    DDX_Control(pDX, IDC_STATIC_END_TIME, m_lblEndTime);
    DDX_Control(pDX, IDC_MAIN_PROJECT_NAME, m_ctrlProjectName);
    DDX_Control(pDX, IDC_MAIN_CHECK_SUM, m_ctrlCheckSum);
    DDX_Control(pDX, IDC_MAIN_CHIP_MODEL, m_ctrlChipModel);
    DDX_Control(pDX, IDC_MAIN_CHIP_BRAND, m_ctrlChipBrand);
    DDX_Control(pDX, IDC_MAIN_SYS_NG, m_ctrlSysNG);
    DDX_Control(pDX, IDC_MAIN_START_TIME, m_ctrlStartTime);
    DDX_Control(pDX, IDC_MAIN_END_TIME, m_ctrlEndTime);
    DDX_Control(pDX, IDC_STATIC_YIELD2, m_lblUPH);
    DDX_Control(pDX, IDC_STATIC_WORKNUM_SUM, m_lblWorkNumSum);
    DDX_Control(pDX, IDC_COMBO_SOCKET_COM, m_cmbSocketPort);
}

BEGIN_MESSAGE_MAP(CAutoFPDlg, CDialog)
    //{{AFX_MSG_MAP(CAutoFPDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DRAWITEM()
    ON_WM_TIMER()
    //	ON_WM_COPYDATA()
    ON_BN_CLICKED(IDC_SOCKET_CHECK1, OnSocketCheck1)
    ON_BN_CLICKED(IDC_SOCKET_CHECK2, OnSocketCheck2)
    ON_BN_CLICKED(IDC_SOCKET_CHECK3, OnSocketCheck3)
    ON_BN_CLICKED(IDC_SOCKET_CHECK4, OnSocketCheck4)
    //	ON_MESSAGE(WM_BURN_AGAIN,OnBurnAgain)
    ON_BN_CLICKED(IDC_SOCKET_CHECK5, OnSocketCheck5)
    ON_BN_CLICKED(IDC_SOCKET_CHECK6, OnSocketCheck6)
    ON_BN_CLICKED(IDC_SOCKET_CHECK7, OnSocketCheck7)
    ON_BN_CLICKED(IDC_SOCKET_CHECK8, OnSocketCheck8)
    ON_WM_CLOSE()

    ON_BN_CLICKED(IDC_SOCKET_CHECK9, OnSocketCheck9)
    ON_BN_CLICKED(IDC_SOCKET_CHECK10, OnSocketCheck10)
    ON_MESSAGE(WM_CHECK_STATUS, OnCheckStatus)
    ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)


    ON_BN_CLICKED(IDC_CHECK_ALLSELECT, OnCheckAllselect)
    //}}AFX_MSG_MAP

    ON_STN_DBLCLK(IDC_SOCKET_GOOD1, &CAutoFPDlg::OnStnDblclickSocketGood1)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD2, &CAutoFPDlg::OnStnDblclickSocketGood2)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD3, &CAutoFPDlg::OnStnDblclickSocketGood3)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD4, &CAutoFPDlg::OnStnDblclickSocketGood4)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD50, &CAutoFPDlg::OnStnDblclickSocketGood50)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD60, &CAutoFPDlg::OnStnDblclickSocketGood60)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD70, &CAutoFPDlg::OnStnDblclickSocketGood70)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD80, &CAutoFPDlg::OnStnDblclickSocketGood80)
    ON_COMMAND(ID_ABOUT, &CAutoFPDlg::OnAbout)
    ON_COMMAND(ID_32772, &CAutoFPDlg::On32772)
    ON_COMMAND(ID_32773, &CAutoFPDlg::On32773)
    ON_COMMAND(ID_32774, &CAutoFPDlg::OnOpenProgrammer)
    ON_COMMAND(ID_MENU_LOAD, &CAutoFPDlg::OnMenuLoad)
    ON_COMMAND(ID_MENU_SAVE, &CAutoFPDlg::OnMenuSave)
    ON_MESSAGE(WM_RECVDATA, &CAutoFPDlg::OnRecvdata)
    ON_BN_CLICKED(IDC_BTN_SENDTEST, &CAutoFPDlg::OnBnClickedBtnSendtest)
    ON_COMMAND(ID_P800_OPEN, &CAutoFPDlg::OnP800Open)
    ON_COMMAND(ID_P800_HIDE, &CAutoFPDlg::OnP800Hide)
    ON_COMMAND(ID_P800_CLOSE, &CAutoFPDlg::OnP800Close)
    ON_BN_CLICKED(IDC_BTN_POS, &CAutoFPDlg::OnBnClickedBtnPos)
    ON_BN_CLICKED(IDC_BTN_SETTING, &CAutoFPDlg::OnBnClickedBtnSetting)
    ON_BN_CLICKED(IDC_BTN_RUN, &CAutoFPDlg::OnBnClickedBtnRun)
    ON_BN_CLICKED(IDC_BTN_SUSPEND, &CAutoFPDlg::OnBnClickedBtnSuspend)
    ON_BN_CLICKED(IDC_BTN_FINISH, &CAutoFPDlg::OnBnClickedBtnFinish)
    ON_BN_CLICKED(IDC_BTN_EXIT, &CAutoFPDlg::OnBnClickedBtnExit)
    ON_BN_CLICKED(IDC_SOCKET_CHECK11, &CAutoFPDlg::OnBnClickedSocketCheck11)
    ON_BN_CLICKED(IDC_SOCKET_CHECK12, &CAutoFPDlg::OnBnClickedSocketCheck12)
    ON_BN_CLICKED(IDC_SOCKET_CHECK13, &CAutoFPDlg::OnBnClickedSocketCheck13)
    ON_BN_CLICKED(IDC_SOCKET_CHECK14, &CAutoFPDlg::OnBnClickedSocketCheck14)
    ON_BN_CLICKED(IDC_SOCKET_CHECK21, &CAutoFPDlg::OnBnClickedSocketCheck21)
    ON_BN_CLICKED(IDC_SOCKET_CHECK22, &CAutoFPDlg::OnBnClickedSocketCheck22)
    ON_BN_CLICKED(IDC_SOCKET_CHECK23, &CAutoFPDlg::OnBnClickedSocketCheck23)
    ON_BN_CLICKED(IDC_SOCKET_CHECK24, &CAutoFPDlg::OnBnClickedSocketCheck24)
    ON_BN_CLICKED(IDC_SOCKET_CHECK31, &CAutoFPDlg::OnBnClickedSocketCheck31)
    ON_BN_CLICKED(IDC_SOCKET_CHECK32, &CAutoFPDlg::OnBnClickedSocketCheck32)
    ON_BN_CLICKED(IDC_SOCKET_CHECK33, &CAutoFPDlg::OnBnClickedSocketCheck33)
    ON_BN_CLICKED(IDC_SOCKET_CHECK34, &CAutoFPDlg::OnBnClickedSocketCheck34)
    ON_BN_CLICKED(IDC_SOCKET_CHECK41, &CAutoFPDlg::OnBnClickedSocketCheck41)
    ON_BN_CLICKED(IDC_SOCKET_CHECK42, &CAutoFPDlg::OnBnClickedSocketCheck42)
    ON_BN_CLICKED(IDC_SOCKET_CHECK43, &CAutoFPDlg::OnBnClickedSocketCheck43)
    ON_BN_CLICKED(IDC_SOCKET_CHECK44, &CAutoFPDlg::OnBnClickedSocketCheck44)
    ON_BN_CLICKED(IDC_SOCKET_CHECK51, &CAutoFPDlg::OnBnClickedSocketCheck51)
    ON_BN_CLICKED(IDC_SOCKET_CHECK52, &CAutoFPDlg::OnBnClickedSocketCheck52)
    ON_BN_CLICKED(IDC_SOCKET_CHECK53, &CAutoFPDlg::OnBnClickedSocketCheck53)
    ON_BN_CLICKED(IDC_SOCKET_CHECK54, &CAutoFPDlg::OnBnClickedSocketCheck54)
    ON_BN_CLICKED(IDC_SOCKET_CHECK61, &CAutoFPDlg::OnBnClickedSocketCheck61)
    ON_BN_CLICKED(IDC_SOCKET_CHECK62, &CAutoFPDlg::OnBnClickedSocketCheck62)
    ON_BN_CLICKED(IDC_SOCKET_CHECK63, &CAutoFPDlg::OnBnClickedSocketCheck63)
    ON_BN_CLICKED(IDC_SOCKET_CHECK64, &CAutoFPDlg::OnBnClickedSocketCheck64)
    ON_BN_CLICKED(IDC_SOCKET_CHECK71, &CAutoFPDlg::OnBnClickedSocketCheck71)
    ON_BN_CLICKED(IDC_SOCKET_CHECK72, &CAutoFPDlg::OnBnClickedSocketCheck72)
    ON_BN_CLICKED(IDC_SOCKET_CHECK73, &CAutoFPDlg::OnBnClickedSocketCheck73)
    ON_BN_CLICKED(IDC_SOCKET_CHECK74, &CAutoFPDlg::OnBnClickedSocketCheck74)
    ON_BN_CLICKED(IDC_SOCKET_CHECK81, &CAutoFPDlg::OnBnClickedSocketCheck81)
    ON_BN_CLICKED(IDC_SOCKET_CHECK82, &CAutoFPDlg::OnBnClickedSocketCheck82)
    ON_BN_CLICKED(IDC_SOCKET_CHECK83, &CAutoFPDlg::OnBnClickedSocketCheck83)
    ON_BN_CLICKED(IDC_SOCKET_CHECK84, &CAutoFPDlg::OnBnClickedSocketCheck84)
    ON_BN_CLICKED(IDC_SOCKET_CHECK91, &CAutoFPDlg::OnBnClickedSocketCheck91)
    ON_BN_CLICKED(IDC_SOCKET_CHECK92, &CAutoFPDlg::OnBnClickedSocketCheck92)
    ON_BN_CLICKED(IDC_SOCKET_CHECK93, &CAutoFPDlg::OnBnClickedSocketCheck93)
    ON_BN_CLICKED(IDC_SOCKET_CHECK94, &CAutoFPDlg::OnBnClickedSocketCheck94)
    ON_BN_CLICKED(IDC_SOCKET_CHECK101, &CAutoFPDlg::OnBnClickedSocketCheck101)
    ON_BN_CLICKED(IDC_SOCKET_CHECK102, &CAutoFPDlg::OnBnClickedSocketCheck102)
    ON_BN_CLICKED(IDC_SOCKET_CHECK103, &CAutoFPDlg::OnBnClickedSocketCheck103)
    ON_BN_CLICKED(IDC_SOCKET_CHECK104, &CAutoFPDlg::OnBnClickedSocketCheck104)
    ON_BN_CLICKED(IDC_BTN_LOCK, &CAutoFPDlg::OnBnClickedBtnLock)
    ON_BN_CLICKED(IDC_BTN_CLEAN_DATA, &CAutoFPDlg::OnBnClickedBtnCleanData)
    ON_BN_CLICKED(IDC_BTN_CLEAN_SOCKET, &CAutoFPDlg::OnBnClickedBtnCleanSocket)
    ON_BN_CLICKED(IDC_BTN_EXCELBOX, &CAutoFPDlg::OnBnClickedBtnExcelbox)
    ON_BN_CLICKED(IDC_BTN_CREATE_LOGFILE, &CAutoFPDlg::OnBnClickedBtnCreateLogfile)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM2, &CAutoFPDlg::OnBnClickedCheckProgram2)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM1, &CAutoFPDlg::OnBnClickedCheckProgram1)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM3, &CAutoFPDlg::OnBnClickedCheckProgram3)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM4, &CAutoFPDlg::OnBnClickedCheckProgram4)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM5, &CAutoFPDlg::OnBnClickedCheckProgram5)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM6, &CAutoFPDlg::OnBnClickedCheckProgram6)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM7, &CAutoFPDlg::OnBnClickedCheckProgram7)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM8, &CAutoFPDlg::OnBnClickedCheckProgram8)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM9, &CAutoFPDlg::OnBnClickedCheckProgram9)
    ON_BN_CLICKED(IDC_CHECK_PROGRAM10, &CAutoFPDlg::OnBnClickedCheckProgram10)
    ON_BN_CLICKED(IDC_BTN_GOHOME, &CAutoFPDlg::OnBnClickedBtnGohome)
    ON_BN_CLICKED(IDC_BTN_RESET_STOCKTRAY, &CAutoFPDlg::OnBnClickedBtnResetStocktray)
    ON_BN_CLICKED(IDC_BTN_FP_NG, &CAutoFPDlg::OnBnClickedBtnFpNg)
    ON_BN_CLICKED(IDC_CHECK_UPLIGHT, &CAutoFPDlg::OnBnClickedCheckUplight)
    ON_BN_CLICKED(IDC_BTN_READ_COM, &CAutoFPDlg::OnBnClickedBtnReadCom)
    ON_BN_CLICKED(IDC_BTN_TRAY_COMPLETE, &CAutoFPDlg::OnBnClickedBtnTrayComplete)
    ON_BN_CLICKED(IDC_BTN_READ_INIFILE, &CAutoFPDlg::OnBnClickedBtnReadInifile)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD_91, &CAutoFPDlg::OnStnDblclickSocketGood91)
    ON_STN_DBLCLK(IDC_SOCKET_GOOD_101, &CAutoFPDlg::OnStnDblclickSocketGood101)
    ON_BN_CLICKED(IDC_CHECK_DOWNLIGHT, &CAutoFPDlg::OnBnClickedCheckDownlight)
        ON_BN_CLICKED(IDC_BTN_TAPECAMERA_CREATE, &CAutoFPDlg::OnBnClickedBtnTapecameraCreate)
        ON_BN_CLICKED(IDC_BTN_TAPECAMEAR_CHECK, &CAutoFPDlg::OnBnClickedBtnTapecamearCheck)
        END_MESSAGE_MAP()


//初始化http服务器
BOOL CAutoFPDlg::InitSocket()
{
    WSADATA Data;
    SOCKADDR_IN serverSockAddr;
    SOCKADDR_IN clientSockAddr;
    int addrlen = sizeof(SOCKADDR_IN);   //16  
    int status;

    //初始化windows Socket Dll  
    status = WSAStartup(MAKEWORD(1, 1), &Data);
    if (0 != status)
    {
        AfxMessageBox("socket初始化失败");
        return FALSE;
    }

    //清0  
    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    //初始化  
    serverSockAddr.sin_port = htons(PORT);
    serverSockAddr.sin_family = AF_INET;    //指定地址协议族  
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);  //初始化ip地址0,0,0,0 任何ip都可以连接  

    //创建套接字  
    m_serversocket = socket(AF_INET, SOCK_STREAM/*套接字类型*/, 0/*套接字使用的特定协议*/);
    if (INVALID_SOCKET == m_serversocket)
    {
        AfxMessageBox("创建socket失败");
        return FALSE;
    }

    //绑定socket到特定地址(结构体地址)  
    status = bind(m_serversocket, (LPSOCKADDR)&serverSockAddr, sizeof(serverSockAddr));
    if (SOCKET_ERROR == status)
    {
        AfxMessageBox("绑定地址失败");
        return FALSE;
    }
    return TRUE;
}

//接收消息线程
DWORD WINAPI CAutoFPDlg::RecvProc(LPVOID lpParameter)
{
    int status;
    int addrlen = sizeof(SOCKADDR_IN);   //16  
    int numrcv;
    char buffer[MAXBUFLEN];
    SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
    CAutoFPDlg* hwnd = (CAutoFPDlg*)((RECVPARAM*)lpParameter)->hwnd;
    delete lpParameter;	//释放内存

    //允许外来申请链接请求，或者说就是监听  
    status = listen(sock/*套接字号*/, 2/*最大容许链接的套接字个数*/);
    if (SOCKET_ERROR == status)
    {
        AfxMessageBox("监听失败");
    }
    SOCKET clientsocket;
    clientsocket = accept(sock, (LPSOCKADDR)&clientsocket, &addrlen);
    char recvBuf[1024];

    while (TRUE)
    {
        //GET / getWriterInfo.api HTTP / 1.1
        //Host: 127.0.0.1 : 7100
        //Connection : Keep - Alive

        //当有请求申请时就接受链接请求  
        //SOCKET clientsocket;
        //clientsocket = accept(sock, (LPSOCKADDR)&clientsocket, &addrlen);
        //char recvBuf[1024];
        numrcv = recv(clientsocket/*已经建立的套接字*/, recvBuf/*接受输入数据缓冲器的指针*/, sizeof(recvBuf)/*接受缓冲区大小*/, 0/*传输控制方式*/);
        if ((0 == numrcv) || (numrcv == SOCKET_ERROR))
        {
            AfxMessageBox("链接受到限制");

            status = closesocket(clientsocket);
            if (SOCKET_ERROR == status)
                AfxMessageBox("断开链接失败");

            status = WSACleanup();
            if (SOCKET_ERROR == status)
                AfxMessageBox("清理链接失败");
            return(1);
        }
        //处理接收到的http报文
        hwnd->ProccessRequest(recvBuf, clientsocket);
        //	Json::Reader json_reader;
        //	Json::Value json_send;//待发送的字符串
        //	Json::Value json_recv;//接收到的字符串
        //	Json::StyledWriter json_fast_writer;
        //	switch (recvBuf[0])
        //	{
        //	case 'G'://获取烧写器信息接口（getWriterInfo）
        //		json_send["retcode"] = Json::Value("00000");
        //		json_send["retinfo"] = Json::Value("操作成功");
        //		json_send["c_ID"] = Json::Value("1234567890");
        //		json_send["c_manufactor"] = Json::Value("”XXXXXX”");
        //		json_send["c_type"] = Json::Value("”XXXXX_XXXXXX”");
        //		json_send["c_info"] = Json::Value("………");
        //		break;
        //	case '1'://烧写请求（requestWrite）
        //		json_send["retcode"] = Json::Value("00000");
        //		json_send["retinfo"] = Json::Value("操作成功");
        //		char buffer_json[MAXBUFLEN];
        //		strncpy(buffer_json, buffer + 1, sizeof(buffer) - 1); //取出字符串中的json格式字符串
        //		json_reader.parse(buffer_json, json_recv);
        //		//获取客户端发来的烧写信息
        //		json_recv["c_factory"] = Json::Value("haier");
        //		json_recv["c_model"] = Json::Value("haier_xxx");
        //		gm_nSum = json_recv["c_sum"].asInt();			
        //		json_recv["c_file_download"] = Json::Value("d:/temp/haier_xxx.mot");
        //		json_recv["c_checksum"] = Json::Value("FFFF");
        //		json_recv["c_filetype"] = Json::Value(0);
        //		json_recv["c_cache"] = Json::Value(0);
        //		json_recv["c_start_addr"] = Json::Value("0000");
        //		json_recv["c_flag"] = Json::Value(2);
        //		json_recv["c_project_file_updown"] = Json::Value("");
        //		break;
        //	case '2'://烧写进度查询（getProgress）
        //		json_send["retcode"] = Json::Value("00000");
        //		json_send["retinfo"] = Json::Value("操作成功");
        //		json_send["c_state"] = Json::Value("run");
        //		json_send["c_total"] = Json::Value(gm_nSum);
        //		json_send["c_stolal"] = Json::Value(gm_nTotalGoodSize);
        //		json_send["c_ftolal"] = Json::Value(gm_nTotalFailSize);
        //		break;
        //	default:
        //		break;
        //	}
        //	std::string send_buf = json_fast_writer.write(json_send);
        //	int numsnt;
        //	numsnt = send(clientsocket, send_buf.c_str(), send_buf.size() + 1, 0);
        //	if (1/*numsnt != (send_buf.size() + 1)*/)
        //	{
        //		//cout << "Connection terminated" << endl;
        //		//AfxMessageBox("向客户端发送消息失败");
        //		status = closesocket(clientsocket);
        //		if (status == SOCKET_ERROR)
        //			cout << "ERROR: closesocket unsuccessful" << endl;
        //		//status = WSACleanup();
        //		if (status == SOCKET_ERROR)
        //			cout << "ERROR: WSACleanup unsuccessful" << endl;
        //		//return(1);
        //	}
        //	//::PostMessage(hwnd, WM_RECVDATA, 0, (LPARAM)buffer);
    }
    return 0;
}
void CAutoFPDlg::ProccessRequest(const char* httpMessage, SOCKET skCommu)
{
    //1.把用户请求的报文进行封装
    CHttpContext context = CHttpContext(httpMessage);

    //2.处理请求
    send(skCommu, context.response.responseTotal.c_str(), sizeof(context.response.responseHeader) + 1, 0);
    //closesocket(skCommu);
    AfxMessageBox(httpMessage);//发了两次请求
    //SetDlgItemText(IDC_EDIT_MESSAGE, context.request.requestUrl.c_str());
}
//接收消息的消息响应函数
afx_msg LRESULT CAutoFPDlg::OnRecvdata(WPARAM wParam, LPARAM lParam)
{
    CString strTemp = (char*)lParam;
    CString str;

    json_reader.parse(str.GetBuffer(0), json_recv);

    GetDlgItemText(IDC_EDIT_MESSAGE, str);
    str += "\r\n";
    str += strTemp;
    SetDlgItemText(IDC_EDIT_MESSAGE, str);
    return 0;
}
void CAutoFPDlg::AppendLogMessage(CString strMsg)
{
    gm_logFile.Write(strMsg);
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t = localtime(&tt);
    //printf("%d-%02d-%02d %02d:%02d:%02d\n",
    //	t->tm_year + 1900,
    //	t->tm_mon + 1,
    //	t->tm_mday,
    //	t->tm_hour,
    //	t->tm_min,
    //	t->tm_sec);
    CString strTemp;
    strTemp.Format("%02d:%02d:%02d %s", t->tm_hour, t->tm_min, t->tm_sec, strMsg);
    CString str;
    GetDlgItemText(IDC_EDIT_MESSAGE, str);
    str += "\r\n";
    str += strTemp;
    SetDlgItemText(IDC_EDIT_MESSAGE, str);
    int count = m_editLog.GetLineCount();
    m_editLog.LineScroll(count, 0);
}
//发送消息测试按钮
void CAutoFPDlg::OnBnClickedBtnSendtest()
{
    //客户端端口创建  
    SOCKET m_hClientSocket;
    m_hClientSocket = socket(AF_INET, SOCK_DGRAM, 0); 	//客户端向服务器发送连接请求  

    SOCKADDR_IN m_addr1;
    m_addr1.sin_family = AF_INET;
    m_addr1.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//服务器的ip地址  
    m_addr1.sin_port = htons(7100);//服务器socket的端口号  
    connect(m_hClientSocket, (LPSOCKADDR)&m_addr1, sizeof(m_addr1));//从客户端端口m_hClientSocket想服务器端口m_addr1发送连接请求  

    SOCKET com_Sock;
    SOCKADDR_IN addr_conn;
    int nSize = sizeof(addr_conn);
    //initialize the memory block via the memset function  
    memset((void *)&addr_conn, 0, sizeof(addr_conn));
    com_Sock = accept(m_socket, NULL, NULL);
    if (getpeername(com_Sock, (struct sockaddr *)&addr_conn, &nSize))
    {
        CString str;
        str.Format("对方IP：%s:%d ", inet_ntoa(addr_conn.sin_addr), ntohs(addr_conn.sin_port));
        AfxMessageBox(str);
    }

    json_send["c_factory"] = Json::Value("haier");
    json_send["c_model"] = Json::Value("haier_xxx");
    json_send["c_sum"] = Json::Value(100);
    json_send["c_file_download"] = Json::Value("d:/temp/haier_xxx.mot");
    json_send["c_checksum"] = Json::Value("FFFF");
    json_send["c_filetype"] = Json::Value(0);
    json_send["c_cache"] = Json::Value(0);
    json_send["c_start_addr"] = Json::Value("0000");
    json_send["c_flag"] = Json::Value(2);
    json_send["c_project_file_updown"] = Json::Value("");
    std::string send_buf = json_fast_writer.write(json_send);
    sendto(m_socket, send_buf.c_str(), send_buf.size() + 1, 0, (SOCKADDR*)&m_addr1, sizeof(SOCKADDR));
}
BOOL CAutoFPDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    //创建工作目录
    CreateDirectory("C:\\S100", NULL);
    CreateDirectory("C:\\S100\\Model", NULL);
    CreateDirectory("C:\\S100\\Model\\Socket", NULL);
    CreateDirectory("C:\\S100\\Model\\Chip", NULL);
    CreateDirectory("C:\\S100\\Log", NULL);
    CreateDirectory("C:\\S100\\Excel", NULL);
    CreateDirectory("C:\\S100\\backup", NULL);
    CreateDirectory("C:\\S100\\backup\\Log", NULL);
    CreateDirectory("C:\\S100\\backup\\Excel", NULL);

    m_progressSocket1.SetRange(0, 100);
    m_progressSocket1.SetPos(50);
    HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "c:\\S100\\bin\\Multiprog.exe", NULL, NULL, SW_HIDE);
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    EnumSerials();//枚举串口
    m_Menu.LoadMenuA(IDR_MENU1);
    SetMenu(&m_Menu);
    CMenu* pSysMenu = GetSystemMenu(TRUE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    m_Menu.GetSubMenu(3)->EnableMenuItem(1, MF_GRAYED | MF_BYPOSITION);//p800显示/隐藏变灰
    m_Menu.GetSubMenu(3)->EnableMenuItem(2, MF_GRAYED | MF_BYPOSITION);//p800关闭变灰

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    // TODO: Add extra initialization here

    ////http服务器初始化
    //InitSocket();
    //RECVPARAM *pRecvParam = new RECVPARAM;
    //pRecvParam->sock = m_serversocket;
    //pRecvParam->hwnd = (HWND)this;
    //HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);//建立服务器监听线程
    //CloseHandle(hThread);

    ////---------------------------------------------------------------*/


    CRect rt;
    CWnd *pWnd = GetDlgItem(IDC_MAIN_TRAY);
    pWnd->GetWindowRect(m_rt);
    ScreenToClient(m_rt);

    //-----------------------------------------------------------------
    //设置状态栏的显示格式
    m_ctrlStatus.SetBkColor(RGB(95, 195, 255), RGB(95, 195, 255), CLabel::Gradient);
    m_ctrlStatus.SetTextColor(RGB(255, 0, 0));
    m_ctrlStatus.SetFontSize(20);
    m_ctrlStatus.SetFontBold(TRUE);
    //m_ctrlStatus.SetSunken(TRUE);
    //-----------------------------------------------------------------

    //m_ctrlGood.SetBkColor(RGB(160, 195, 255), RGB(95, 195, 255), CLabel::Gradient);
    //m_ctrlGood.SetTextColor(RGB(255,255,255));
    m_ctrlGood.SetFontSize(9);
    //m_ctrlGood.SetFontBold(TRUE);

    //m_ctrlFail.SetBkColor(RGB(160,195,255),RGB(95,195,255),CLabel::Gradient);
    //m_ctrlFail.SetTextColor(RGB(255,255,255));
    m_ctrlFail.SetFontSize(9);
    //m_ctrlFail.SetFontBold(TRUE);

    //m_ctrlUsedTime.SetBkColor(RGB(160,195,255),RGB(95,195,255),CLabel::Gradient);
    //m_ctrlUsedTime.SetTextColor(RGB(255,255,255));
    m_ctrlUsedTime.SetFontSize(9);
    //m_ctrlUsedTime.SetFontBold(TRUE);

    //m_ctrlYield.SetBkColor(RGB(160,195,255),RGB(95,195,255),CLabel::Gradient);
    //m_ctrlYield.SetTextColor(RGB(255,255,255));
    m_ctrlYield.SetFontSize(9);
    //m_ctrlYield.SetFontBold(TRUE);
    m_ctrlProjectName.SetFontSize(9);
    m_ctrlCheckSum.SetFontSize(9);
    m_ctrlChipModel.SetFontSize(9);
    m_ctrlChipBrand.SetFontSize(9);
    m_ctrlSysNG.SetFontSize(9);
    m_ctrlStartTime.SetFontSize(9);
    m_ctrlEndTime.SetFontSize(9);

    m_lblGood.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblGood.SetTextColor(RGB(255, 255, 255));
    m_lblGood.SetFontSize(9);

    m_lblFail.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblFail.SetTextColor(RGB(255, 255, 255));
    m_lblFail.SetFontSize(9);

    m_lblYield.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblYield.SetTextColor(RGB(255, 255, 255));
    m_lblYield.SetFontSize(9);

    m_lblUsedTime.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblUsedTime.SetTextColor(RGB(255, 255, 255));
    m_lblUsedTime.SetFontSize(9);

    m_lblProjectName.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblProjectName.SetTextColor(RGB(255, 255, 255));
    m_lblProjectName.SetFontSize(9);

    m_lblWorkNumSum.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblWorkNumSum.SetTextColor(RGB(255, 255, 255));
    m_lblWorkNumSum.SetFontSize(9);

    m_lblCheckSum.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblCheckSum.SetTextColor(RGB(255, 255, 255));
    m_lblCheckSum.SetFontSize(9);

    m_lblChipModel.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblChipModel.SetTextColor(RGB(255, 255, 255));
    m_lblChipModel.SetFontSize(9);

    m_lblChipBrand.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblChipBrand.SetTextColor(RGB(255, 255, 255));
    m_lblChipBrand.SetFontSize(9);

    m_lblProgNG.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblProgNG.SetTextColor(RGB(255, 255, 255));
    m_lblProgNG.SetFontSize(9);

    m_lblStartTime.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblStartTime.SetTextColor(RGB(255, 255, 255));
    m_lblStartTime.SetFontSize(9);

    m_lblEndTime.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblEndTime.SetTextColor(RGB(255, 255, 255));
    m_lblEndTime.SetFontSize(9);

    m_lblAdmin.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    //m_lblAdmin.SetTextColor(RGB(255, 255, 255));
    m_lblAdmin.SetFontSize(9);

    m_lblUPH.SetBkColor(RGB(214, 219, 233), RGB(214, 219, 233), CLabel::Gradient);
    m_lblUPH.SetFontSize(9);

    m_xn = gm_dataTray.xn;
    m_yn = gm_dataTray.yn;

    m_lbTapeIn.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
    m_lbTapeIn.SetTextColor(RGB(0, 0, 0));
    m_lbTapeIn.SetFontSize(16);
    m_lbTapeIn.SetText("0");

    m_lbTapeOut.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
    m_lbTapeOut.SetTextColor(RGB(0, 0, 0));
    m_lbTapeOut.SetFontSize(16);
    m_lbTapeOut.SetText("0");

    m_titleTapeOut.SetTextColor(RGB(0, 0, 0));
    m_titleTapeOut.SetFontSize(10);
    m_titleTapeOut.SetText("当前");

    m_titleTapeIn.SetTextColor(RGB(0, 0, 0));
    m_titleTapeIn.SetFontSize(10);
    m_titleTapeIn.SetText("当前");


    m_good11.SetFontSize(12);
    m_fail11.SetFontSize(12);
    m_good12.SetFontSize(12);
    m_fail12.SetFontSize(12);
    m_good13.SetFontSize(12);
    m_fail13.SetFontSize(12);
    m_good14.SetFontSize(12);
    m_fail14.SetFontSize(12);
    m_good21.SetFontSize(12);
    m_fail21.SetFontSize(12);
    m_good22.SetFontSize(12);
    m_fail22.SetFontSize(12);
    m_good23.SetFontSize(12);
    m_fail23.SetFontSize(12);
    m_good24.SetFontSize(12);
    m_fail24.SetFontSize(12);
    m_good31.SetFontSize(12);
    m_fail31.SetFontSize(12);
    m_good32.SetFontSize(12);
    m_fail32.SetFontSize(12);
    m_good33.SetFontSize(12);
    m_fail33.SetFontSize(12);
    m_good34.SetFontSize(12);
    m_fail34.SetFontSize(12);
    m_good41.SetFontSize(12);
    m_fail41.SetFontSize(12);
    m_good42.SetFontSize(12);
    m_fail42.SetFontSize(12);
    m_good43.SetFontSize(12);
    m_fail43.SetFontSize(12);
    m_good44.SetFontSize(12);
    m_fail44.SetFontSize(12);
    m_good51.SetFontSize(12);
    m_fail51.SetFontSize(12);
    m_good52.SetFontSize(12);
    m_fail52.SetFontSize(12);
    m_good53.SetFontSize(12);
    m_fail53.SetFontSize(12);
    m_good54.SetFontSize(12);
    m_fail54.SetFontSize(12);
    m_good61.SetFontSize(12);
    m_fail61.SetFontSize(12);
    m_good62.SetFontSize(12);
    m_fail62.SetFontSize(12);
    m_good63.SetFontSize(12);
    m_fail63.SetFontSize(12);
    m_good64.SetFontSize(12);
    m_fail64.SetFontSize(12);
    m_good71.SetFontSize(12);
    m_fail71.SetFontSize(12);
    m_good72.SetFontSize(12);
    m_fail72.SetFontSize(12);
    m_good73.SetFontSize(12);
    m_fail73.SetFontSize(12);
    m_good74.SetFontSize(12);
    m_fail74.SetFontSize(12);
    m_good81.SetFontSize(12);
    m_fail81.SetFontSize(12);
    m_good82.SetFontSize(12);
    m_fail82.SetFontSize(12);
    m_good83.SetFontSize(12);
    m_fail83.SetFontSize(12);
    m_good84.SetFontSize(12);
    m_fail84.SetFontSize(12);
    m_good91.SetFontSize(12);
    m_fail91.SetFontSize(12);
    m_good92.SetFontSize(12);
    m_fail92.SetFontSize(12);
    m_good93.SetFontSize(12);
    m_fail93.SetFontSize(12);
    m_good94.SetFontSize(12);
    m_fail94.SetFontSize(12);
    m_good101.SetFontSize(12);
    m_fail101.SetFontSize(12);
    m_good102.SetFontSize(12);
    m_fail102.SetFontSize(12);
    m_good103.SetFontSize(12);
    m_fail103.SetFontSize(12);
    m_good104.SetFontSize(12);
    m_fail104.SetFontSize(12);
    //////////////////////////////////////////////////////////////////////////////////////////////
    CString str;
    //串口参数设置
    configSerial_.ByteSize = 8;
    configSerial_.StopBits = ONESTOPBIT;
    configSerial_.Parity = NOPARITY;
    configSerial_.BaudRate = CBR_9600;
    //configSerial_.

#if BOARD_TYPE != GOOGOL
    OpenMotionBoard();//打开板卡
#endif // BOARD_TYPE != GOOGOL


//打开编带PLC串口
    int nPort = 1;
    str.Format("\\\\.\\COM%d", nPort);
    BOOL bOpen = m_port.openPort(configSerial_, str);
    if (bOpen == FALSE)
        AfxMessageBox("编带PLC端口打开失败！");


    //打开tray盘PLC串口
    int nTrayPort = 2;
    str.Format("\\\\.\\COM%d", nTrayPort);
    bOpen = m_TrayPort.openPort(configSerial_, str);
    if (bOpen == FALSE)
        AfxMessageBox("Tray盘PLC端口打开失败。");




    for (int i = 0; i < MAX_CONNECTION; i++)
    {
        m_SocketManager[i].SetMainWindow(this);
        m_SocketManager[i].SetServerState(true);	//有多少个编程器连接，就建立多少个服务器，每个编程器都是独立的
    }

    PickNextAvailable();
    bool bNet = StartServer();
    if (!bNet)
        AfxMessageBox("网络端口被占用，请关闭！");

    m_dlgExcel.Create(IDD_DATAEXCEL, this);
    m_dlgExcel.ShowWindow(SW_HIDE);
    AppendLogMessage("程序启动");

    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t = localtime(&tt);
    //printf("%d-%02d-%02d %02d:%02d:%02d\n",
    //	t->tm_year + 1900,
    //	t->tm_mon + 1,
    //	t->tm_mday,
    //	t->tm_hour,
    //	t->tm_min,
    //	t->tm_sec);
    CString strTemp;
    strTemp.Format("%d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    gm_sdInfo.strStartTime = strTemp;
    SetDlgItemText(IDC_MAIN_START_TIME, strTemp);
    //SetDlgItemText(IDC_MAIN_CHIP_MODEL, gm_sdInfo.strICModel);
    //SetDlgItemText(IDC_MAIN_CHIP_BRAND, gm_sdInfo.strICBrand);
    //SetDlgItemText(IDC_MAIN_CHECK_SUM, gm_sdInfo.strCheckSum);
    ////SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_uInfo.strProcName);
    //SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_sdInfo.strWorkNo);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    SetTimer(TIMER_GOHOME, 10, NULL);//回原
    SetTimer(TIMER_REFRESH_SOCKET_STATUS, 1000, NULL);//显示座子状态
    //载入设置参数
    LoadConfig();

    //for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    //{
    //	InitSockStatus(i);
    //}
    InitStatusCheck(TRUE);
    //SetDlgItemText(IDC_MAIN_DEVICE2,gm_strPositionFile);
    if (gm_uInfo.nWorkMode == TRAYTOTRAY) //tray
    {
        ShowTapeIn(0, FALSE);
        ShowTapeOut(0, FALSE);
    }
    else if (gm_uInfo.nWorkMode == TAPETOTRAY) //tape2tray
    {
        ShowTapeIn(1, TRUE);
        ShowTapeOut(0, FALSE);
    }
    else if (gm_uInfo.nWorkMode == TAPETOTAPE)  //tape2tape
    {
        ShowTapeIn(0, TRUE);
        ShowTapeOut(0, TRUE);
    }
    else if (gm_uInfo.nWorkMode == TRAYTOTAPE)   //tray2tape
    {
        ShowTapeIn(0, FALSE);
        ShowTapeOut(1, TRUE);
    }

    return TRUE;  // return TRUE  unless you set the focus to a control
}
//初始化烧录座状态
void CAutoFPDlg::InitSockStatus(UINT nSocket)
{
    switch (nSocket)
    {
    case 0:
        if (gm_bSocketUsed[0])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK11);
            pBtn->SetCheck(1);
            m_good11.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good11.SetTextColor(RGB(0, 0, 0));
            m_fail11.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail11.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK11);
            pBtn->SetCheck(0);
            m_good11.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good11.SetTextColor(RGB(0, 0, 0));
            m_fail11.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail11.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 1:
        if (gm_bSocketUsed[1])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK12);
            pBtn->SetCheck(1);
            m_good12.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good12.SetTextColor(RGB(0, 0, 0));
            m_fail12.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail12.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK12);
            pBtn->SetCheck(0);
            m_good12.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good12.SetTextColor(RGB(0, 0, 0));
            m_fail12.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail12.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 2:
        if (gm_bSocketUsed[2])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK13);
            pBtn->SetCheck(1);
            m_good13.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good13.SetTextColor(RGB(0, 0, 0));
            m_fail13.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail13.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK13);
            pBtn->SetCheck(0);
            m_good13.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good13.SetTextColor(RGB(0, 0, 0));
            m_fail13.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail13.SetTextColor(RGB(0, 0, 0));
        }

        break;
    case 3:
        if (gm_bSocketUsed[3])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK14);
            pBtn->SetCheck(1);
            m_good14.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good14.SetTextColor(RGB(0, 0, 0));
            m_fail14.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail14.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK14);
            pBtn->SetCheck(0);
            m_good14.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good14.SetTextColor(RGB(0, 0, 0));
            m_fail14.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail14.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 4:
        if (gm_bSocketUsed[4])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK21);
            pBtn->SetCheck(1);
            m_good21.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good21.SetTextColor(RGB(0, 0, 0));
            m_fail21.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail21.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK21);
            pBtn->SetCheck(0);
            m_good21.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good21.SetTextColor(RGB(0, 0, 0));
            m_fail21.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail21.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 5:
        if (gm_bSocketUsed[5])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK22);
            pBtn->SetCheck(1);
            m_good22.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good22.SetTextColor(RGB(0, 0, 0));
            m_fail22.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail22.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK22);
            pBtn->SetCheck(0);
            m_good22.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good22.SetTextColor(RGB(0, 0, 0));
            m_fail22.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail22.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 6:
        if (gm_bSocketUsed[6])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK23);
            pBtn->SetCheck(1);
            m_good23.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good23.SetTextColor(RGB(0, 0, 0));
            m_fail23.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail23.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK23);
            pBtn->SetCheck(0);
            m_good23.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good23.SetTextColor(RGB(0, 0, 0));
            m_fail23.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail23.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 7:
        if (gm_bSocketUsed[7])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK24);
            pBtn->SetCheck(1);
            m_good24.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good24.SetTextColor(RGB(0, 0, 0));
            m_fail24.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail24.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK24);
            pBtn->SetCheck(0);
            m_good24.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good24.SetTextColor(RGB(0, 0, 0));
            m_fail24.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail24.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 8:
        if (gm_bSocketUsed[8])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK31);
            pBtn->SetCheck(1);
            m_good31.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good31.SetTextColor(RGB(0, 0, 0));
            m_fail31.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail31.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK31);
            pBtn->SetCheck(0);
            m_good31.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good31.SetTextColor(RGB(0, 0, 0));
            m_fail31.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail31.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 9:
        if (gm_bSocketUsed[9])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK32);
            pBtn->SetCheck(1);
            m_good32.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good32.SetTextColor(RGB(0, 0, 0));
            m_fail32.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail32.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK32);
            pBtn->SetCheck(0);
            m_good32.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good32.SetTextColor(RGB(0, 0, 0));
            m_fail32.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail32.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 10:
        if (gm_bSocketUsed[10])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK33);
            pBtn->SetCheck(1);
            m_good33.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good33.SetTextColor(RGB(0, 0, 0));
            m_fail33.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail33.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK33);
            pBtn->SetCheck(0);
            m_good33.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good33.SetTextColor(RGB(0, 0, 0));
            m_fail33.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail33.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 11:
        if (gm_bSocketUsed[11])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK34);
            pBtn->SetCheck(1);
            m_good34.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good34.SetTextColor(RGB(0, 0, 0));
            m_fail34.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail34.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK34);
            pBtn->SetCheck(0);
            m_good34.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good34.SetTextColor(RGB(0, 0, 0));
            m_fail34.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail34.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 12:
        if (gm_bSocketUsed[12])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK41);
            pBtn->SetCheck(1);
            m_good41.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good41.SetTextColor(RGB(0, 0, 0));
            m_fail41.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail41.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK41);
            pBtn->SetCheck(0);
            m_good41.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good41.SetTextColor(RGB(0, 0, 0));
            m_fail41.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail41.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 13:
        if (gm_bSocketUsed[13])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK42);
            pBtn->SetCheck(1);
            m_good42.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good42.SetTextColor(RGB(0, 0, 0));
            m_fail42.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail42.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK42);
            pBtn->SetCheck(0);
            m_good42.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good42.SetTextColor(RGB(0, 0, 0));
            m_fail42.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail42.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 14:
        if (gm_bSocketUsed[14])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK43);
            pBtn->SetCheck(1);
            m_good43.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good43.SetTextColor(RGB(0, 0, 0));
            m_fail43.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail43.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK43);
            pBtn->SetCheck(0);
            m_good43.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good43.SetTextColor(RGB(0, 0, 0));
            m_fail43.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail43.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 15:
        if (gm_bSocketUsed[15])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK44);
            pBtn->SetCheck(1);
            m_good44.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good44.SetTextColor(RGB(0, 0, 0));
            m_fail44.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail44.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK44);
            pBtn->SetCheck(0);
            m_good44.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good44.SetTextColor(RGB(0, 0, 0));
            m_fail44.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail44.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 16:
        if (gm_bSocketUsed[16])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK51);
            pBtn->SetCheck(1);
            m_good51.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good51.SetTextColor(RGB(0, 0, 0));
            m_fail51.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail51.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK51);
            pBtn->SetCheck(0);
            m_good51.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good51.SetTextColor(RGB(0, 0, 0));
            m_fail51.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail51.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 17:
        if (gm_bSocketUsed[17])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK52);
            pBtn->SetCheck(1);
            m_good52.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good52.SetTextColor(RGB(0, 0, 0));
            m_fail52.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail52.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK52);
            pBtn->SetCheck(0);
            m_good52.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good52.SetTextColor(RGB(0, 0, 0));
            m_fail52.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail52.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 18:
        if (gm_bSocketUsed[18])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK53);
            pBtn->SetCheck(1);
            m_good53.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good53.SetTextColor(RGB(0, 0, 0));
            m_fail53.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail53.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK53);
            pBtn->SetCheck(0);
            m_good53.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good53.SetTextColor(RGB(0, 0, 0));
            m_fail53.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail53.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 19:
        if (gm_bSocketUsed[19])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK54);
            pBtn->SetCheck(1);
            m_good54.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good54.SetTextColor(RGB(0, 0, 0));
            m_fail54.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail54.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK54);
            pBtn->SetCheck(0);
            m_good54.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good54.SetTextColor(RGB(0, 0, 0));
            m_fail54.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail54.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 20:
        if (gm_bSocketUsed[20])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK61);
            pBtn->SetCheck(1);
            m_good61.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good61.SetTextColor(RGB(0, 0, 0));
            m_fail61.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail61.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK61);
            pBtn->SetCheck(0);
            m_good61.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good61.SetTextColor(RGB(0, 0, 0));
            m_fail61.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail61.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 21:
        if (gm_bSocketUsed[21])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK62);
            pBtn->SetCheck(1);
            m_good62.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good62.SetTextColor(RGB(0, 0, 0));
            m_fail62.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail62.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK62);
            pBtn->SetCheck(0);
            m_good62.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good62.SetTextColor(RGB(0, 0, 0));
            m_fail62.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail62.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 22:
        if (gm_bSocketUsed[22])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK63);
            pBtn->SetCheck(1);
            m_good63.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good63.SetTextColor(RGB(0, 0, 0));
            m_fail63.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail63.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK63);
            pBtn->SetCheck(0);
            m_good63.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good63.SetTextColor(RGB(0, 0, 0));
            m_fail63.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail63.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 23:
        if (gm_bSocketUsed[23])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK64);
            pBtn->SetCheck(1);
            m_good64.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good64.SetTextColor(RGB(0, 0, 0));
            m_fail64.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail64.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK64);
            pBtn->SetCheck(0);
            m_good64.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good64.SetTextColor(RGB(0, 0, 0));
            m_fail64.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail64.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 24:
        if (gm_bSocketUsed[24])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK71);
            pBtn->SetCheck(1);
            m_good71.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good71.SetTextColor(RGB(0, 0, 0));
            m_fail71.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail71.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK71);
            pBtn->SetCheck(0);
            m_good71.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good71.SetTextColor(RGB(0, 0, 0));
            m_fail71.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail71.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 25:
        if (gm_bSocketUsed[25])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK72);
            pBtn->SetCheck(1);
            m_good72.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good72.SetTextColor(RGB(0, 0, 0));
            m_fail72.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail72.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK72);
            pBtn->SetCheck(0);
            m_good72.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good72.SetTextColor(RGB(0, 0, 0));
            m_fail72.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail72.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 26:
        if (gm_bSocketUsed[26])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK73);
            pBtn->SetCheck(1);
            m_good73.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good73.SetTextColor(RGB(0, 0, 0));
            m_fail73.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail73.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK73);
            pBtn->SetCheck(0);
            m_good73.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good73.SetTextColor(RGB(0, 0, 0));
            m_fail73.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail73.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 27:
        if (gm_bSocketUsed[27])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK74);
            pBtn->SetCheck(1);
            m_good74.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good74.SetTextColor(RGB(0, 0, 0));
            m_fail74.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail74.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK74);
            pBtn->SetCheck(0);
            m_good74.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good74.SetTextColor(RGB(0, 0, 0));
            m_fail74.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail74.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 28:
        if (gm_bSocketUsed[28])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK81);
            pBtn->SetCheck(1);
            m_good81.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good81.SetTextColor(RGB(0, 0, 0));
            m_fail81.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail81.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK81);
            pBtn->SetCheck(0);
            m_good81.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good81.SetTextColor(RGB(0, 0, 0));
            m_fail81.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail81.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 29:
        if (gm_bSocketUsed[29])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK82);
            pBtn->SetCheck(1);
            m_good82.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good82.SetTextColor(RGB(0, 0, 0));
            m_fail82.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail82.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK82);
            pBtn->SetCheck(0);
            m_good82.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good82.SetTextColor(RGB(0, 0, 0));
            m_fail82.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail82.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 30:
        if (gm_bSocketUsed[30])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK83);
            pBtn->SetCheck(1);
            m_good83.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good83.SetTextColor(RGB(0, 0, 0));
            m_fail83.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail83.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK83);
            pBtn->SetCheck(0);
            m_good83.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good83.SetTextColor(RGB(0, 0, 0));
            m_fail83.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail83.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 31:
        if (gm_bSocketUsed[31])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK84);
            pBtn->SetCheck(1);
            m_good84.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good84.SetTextColor(RGB(0, 0, 0));
            m_fail84.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail84.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK84);
            pBtn->SetCheck(0);
            m_good84.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good84.SetTextColor(RGB(0, 0, 0));
            m_fail84.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail84.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 32:
        if (gm_bSocketUsed[32])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK91);
            pBtn->SetCheck(1);
            m_good91.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good91.SetTextColor(RGB(0, 0, 0));
            m_fail91.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail91.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK91);
            pBtn->SetCheck(0);
            m_good91.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good91.SetTextColor(RGB(0, 0, 0));
            m_fail91.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail91.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 33:
        if (gm_bSocketUsed[33])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK92);
            pBtn->SetCheck(1);
            m_good92.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good92.SetTextColor(RGB(0, 0, 0));
            m_fail92.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail92.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK92);
            pBtn->SetCheck(0);
            m_good92.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good92.SetTextColor(RGB(0, 0, 0));
            m_fail92.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail92.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 34:
        if (gm_bSocketUsed[34])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK93);
            pBtn->SetCheck(1);
            m_good93.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good93.SetTextColor(RGB(0, 0, 0));
            m_fail93.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail93.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK93);
            pBtn->SetCheck(0);
            m_good93.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good93.SetTextColor(RGB(0, 0, 0));
            m_fail93.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail93.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 35:
        if (gm_bSocketUsed[35])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK94);
            pBtn->SetCheck(1);
            m_good94.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good94.SetTextColor(RGB(0, 0, 0));
            m_fail94.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail94.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK94);
            pBtn->SetCheck(0);
            m_good94.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good94.SetTextColor(RGB(0, 0, 0));
            m_fail94.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail94.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 36:
        if (gm_bSocketUsed[36])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK101);
            pBtn->SetCheck(1);
            m_good101.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good101.SetTextColor(RGB(0, 0, 0));
            m_fail101.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail101.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK101);
            pBtn->SetCheck(0);
            m_good101.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good101.SetTextColor(RGB(0, 0, 0));
            m_fail101.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail101.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 37:
        if (gm_bSocketUsed[37])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK102);
            pBtn->SetCheck(1);
            m_good102.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good102.SetTextColor(RGB(0, 0, 0));
            m_fail102.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail102.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK102);
            pBtn->SetCheck(0);
            m_good102.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good102.SetTextColor(RGB(0, 0, 0));
            m_fail102.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail102.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 38:
        if (gm_bSocketUsed[38])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK103);
            pBtn->SetCheck(1);
            m_good103.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good103.SetTextColor(RGB(0, 0, 0));
            m_fail103.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail103.SetTextColor(RGB(0, 0, 0));

        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK103);
            pBtn->SetCheck(0);
            m_good103.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good103.SetTextColor(RGB(0, 0, 0));
            m_fail103.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail103.SetTextColor(RGB(0, 0, 0));
        }
        break;
    case 39:
        if (gm_bSocketUsed[39])
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK104);
            pBtn->SetCheck(1);
            m_good104.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
            m_good104.SetTextColor(RGB(0, 0, 0));
            m_fail104.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
            m_fail104.SetTextColor(RGB(0, 0, 0));
        }
        else
        {
            CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK104);
            pBtn->SetCheck(0);
            m_good104.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_good104.SetTextColor(RGB(0, 0, 0));
            m_fail104.SetBkColor(RGB(255, 255, 255), RGB(255, 255, 255), CLabel::Gradient);
            m_fail104.SetTextColor(RGB(0, 0, 0));
        }
        break;
    default:
        break;
    }
}

void CAutoFPDlg::PickNextAvailable()
{
    m_pCurServer = NULL;
    for (int i = 0; i < MAX_CONNECTION; i++)
    {
        if (!m_SocketManager[i].IsOpen())
        {
            m_pCurServer = &m_SocketManager[i];
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// StartServer : Start the server
bool CAutoFPDlg::StartServer()
{
    bool bSuccess = false;
    if (m_pCurServer != NULL)
    {
        m_pCurServer->SetSmartAddressing(false);
        bSuccess = m_pCurServer->CreateSocket("23", AF_INET, SOCK_STREAM, 0); // TCP
        if (bSuccess)
        {
            m_pCurServer->WatchComm();
            SendInfo("Slave Mode");
        }
    }
    return bSuccess;
}
//update connection status
LRESULT CAutoFPDlg::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
    UINT uEvent = (UINT)wParam;
    CSocketManager* pManager = reinterpret_cast<CSocketManager*>(lParam);

    if (pManager != NULL)
    {
        if (uEvent == EVT_CONSUCCESS)
        {
            PickNextAvailable();
            StartServer();
        }
        else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
        {
            pManager->StopComm();
            if (m_pCurServer == NULL)
            {
                PickNextAvailable();
                StartServer();
            }
        }
    }

    return 1L;
}

void CAutoFPDlg::SendInfo(LPCSTR info)
{
    CString strText;
    strText = info;
    int nLen = strText.GetLength();
    BYTE byData[BUFFER_SIZE];
    if (nLen > 0)
    {
        strText += _T("\r\n");
        nLen = strText.GetLength();
        //nLen = strText.GetLength();
        for (int i = 0; i < nLen; i++)
        {
            byData[i] = strText.GetAt(i);
        }
        for (int i = 0; i < MAX_CONNECTION; i++)
        {
            if (m_SocketManager[i].IsOpen() && m_pCurServer != &m_SocketManager[i])
                m_SocketManager[i].WriteComm(byData, nLen, INFINITE);
        }
    }
}

void CAutoFPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoFPDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this); // device context for painting
        CRect rtW, rtTop;
        GetWindowRect(rtW);
        ScreenToClient(rtW);

        CBrush brush(RGB(214, 219, 233));
        CBrush *pOldBrush = dc.SelectObject(&brush);
        dc.Rectangle(rtW);
        dc.SelectObject(pOldBrush);

        /*	CDC* pDC = GetDC();
        CRect rcClip;
        pDC->GetClipBox (&rcClip);

        pDC->FillSolidRect (&rcClip, 0xA0A0A0);
        */
        if (gm_uInfo.nWorkMode != TAPETOTAPE)
            DrawTray(dc);
        //DrawNGTray(dc);
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAutoFPDlg::OnQueryDragIcon()
{
    return (HCURSOR)m_hIcon;
}

BOOL CAutoFPDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)// || pMsg->wParam == VK_RETURN))
        return TRUE;
    return CDialog::PreTranslateMessage(pMsg);
}

void CAutoFPDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO: Add your message handler code here and/or call default

    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CAutoFPDlg::DrawTray(CPaintDC &dc)
{
    CDC cdc;
    cdc.CreateCompatibleDC(&dc);
    cdc.SelectObject(m_bmpBg);

    //	CRect rt;
    //	CWnd *pWnd = GetDlgItem(IDC_MAIN_TRAY);
    //	pWnd->GetWindowRect(rt);


    int  MINEAREA_FRAME_X = m_rt.left;
    int  MINEAREA_FRAME_Y = m_rt.top;

    int xWidth = m_rt.Width() / m_xn;
    int yHeight = m_rt.Height() / m_yn;

    //	CPen pen(PS_SOLID,2,RGB(168,168,168));
    //	dc.drawr
    //	dc.Draw3dRect(m_rt.left,m_rt.top,xWidth*m_xn,yHeight*m_yn,RGB(168,168,168),RGB(192,192,192));
    for (int i = 0; i < m_yn; i++)
    {
        for (int j = 0; j < m_xn; j++)
        {
            dc.StretchBlt(MINEAREA_FRAME_X + xWidth * j,
                MINEAREA_FRAME_Y + yHeight * i,
                xWidth,
                yHeight,
                &cdc,
                0,
                16 * gm_pTray[i*m_xn + j],
                16,
                16,
                SRCCOPY);
        }
    }

    //CPen pen(PS_SOLID,3,RGB(128,128,128));
    //dc.SelectObject(&pen);

    //int x = MINEAREA_FRAME_X + xWidth * m_xn;
    //int y = MINEAREA_FRAME_Y + yHeight * m_yn;
    //dc.MoveTo(x,m_rt.top);
    //dc.LineTo(x,y);

    //dc.MoveTo(x,y);
    //dc.LineTo(m_rt.left,y);
}
//void CAutoFPDlg::DrawNGTray(CPaintDC &dc)
//{
//	CDC cdc;
//	cdc.CreateCompatibleDC(&dc);
//	cdc.SelectObject(m_bmpBg);
//
//	int  MINEAREA_FRAME_X = m_rtng.left;
//	int  MINEAREA_FRAME_Y = m_rtng.top;
//
//	int xWidth = m_rtng.Width()/FAIL_XN;
//	int yHeight = m_rtng.Height()/FAIL_YN;
//	
//	for (int i = 0; i<FAIL_YN; i++) {
//		for (int j = 0; j<FAIL_XN; j++) {
//			dc.StretchBlt(MINEAREA_FRAME_X + xWidth * j, MINEAREA_FRAME_Y + yHeight * i, 
//				xWidth, yHeight, &cdc, 0,16*m_pTrayStatus[i*FAIL_XN+j], 16, 16, SRCCOPY);
//		}
//	}
//
//	CPen pen(PS_SOLID,3,RGB(128,128,128));
//	dc.SelectObject(&pen);
//
//	int x = MINEAREA_FRAME_X + xWidth * FAIL_XN;
//	int y = MINEAREA_FRAME_Y + yHeight * FAIL_YN;
//
//	dc.MoveTo(MINEAREA_FRAME_X,m_rtng.top);
//	dc.LineTo(x,m_rtng.top);
//
//	dc.MoveTo(x,m_rtng.top);
//	dc.LineTo(x,y);
//
//	dc.MoveTo(x,y);
//	dc.LineTo(m_rtng.left,y);
//}
void CAutoFPDlg::OnTimer(UINT nIDEvent)
{
    CString str;
    int nTemp = 0;
    int nSecond = 0;
    int nHour = 0;
    int nMinute = 0;
    switch (nIDEvent)
    {
    case TIMER_GOHOME:
        KillTimer(TIMER_GOHOME);
        m_ctrlStatus.SetText("System is initializing...");
        GoHome();

        m_ctrlStatus.SetText("Ready");
        //SetTimer(TIMER_SHOW_POS, 300, NULL);
        SetGreenLightOn();

        break;
    case 2:
        CheckSocketStatus();
        break;
    case 3:
        CheckStatusLogFile();
        break;
    case 4:

        break;
    case TIMER_RUN_TIME://时间显示
        if (gm_bStart || gm_bFinish)
        {
            m_nTime++;
            nTemp = m_nTime / 60;
            nSecond = m_nTime % 60;
            nHour = nTemp / 60;
            nMinute = nTemp % 60;

            str.Format("%02d:%02d:%02d", nHour, nMinute, nSecond);
            m_ctrlUsedTime.SetText(str);
        }
        break;
    case 6:

        break;
    case TIMER_SHOW_POS://显示坐标值
    //double x = GetPosition(XAXIS);
    //double y = GetPosition(YAXIS);
    //double z = GetPosition(ZAXIS);
        str.Format("X：%0.2f", GetPosition(XAXIS) / double(XGUIDE));
        SetDlgItemText(IDC_COORDINATE_X, str);
        str.Format("Y：%0.2f", GetPosition(YAXIS) / double(YGUIDE));
        SetDlgItemText(IDC_COORDINATE_Y, str);
        str.Format("Z：%0.2f", GetPosition(ZAXIS) / double(ZGUIDE));
        SetDlgItemText(IDC_COORDINATE_Z, str);
        break;
    case TIMER_REFRESH_SOCKET_STATUS://显示烧录状态
        RefreshSocketStatus();
        break;
    case TIMER_CHECK_TAPEOUT://检查编带出料故障
        KillTimer(TIMER_CHECK_TAPEOUT);
        gm_bCheckTapeOut = CheckTapeOut();
        break;
    case TIMER_CHECK_TRAY_ARRIVED://检查tray盘到达信号
        gm_bTrayArrived = ReadTrayReadySignal();
        break;
    case TIMER_CHECK_TAPE_STATUS:
        KillTimer(TIMER_CHECK_TAPE_STATUS);
        CheckOutTape();
        break;
    default:
        break;
    }

    CDialog::OnTimer(nIDEvent);
}
void CAutoFPDlg::ShowTapeOut(int type, BOOL bShow)
{
    CRect rt, rt1, rt2;
    CWnd *pWnd = GetDlgItem(IDC_STATIC_TAPEOUT);
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);

    CWnd *pWnd1 = GetDlgItem(IDC_TAPEOUT_CUR);
    m_titleTapeOut.GetClientRect(rt1);

    CWnd *pWnd2 = GetDlgItem(IDC_COUNT_TAPEOUT);
    m_lbTapeOut.GetClientRect(rt2);

    int x1 = rt1.left - rt.left;
    int x2 = rt2.left - rt.left;
    int y1 = rt1.top - rt.top;
    int y2 = rt2.top - rt.top;
    int y = 60;
    int x = 30;
    if (type == 0)  //tape2tape
    {
        y = 200;
    }
    else if (type == 1) //Tray2Tape
    {
        y = 20;
    }
    if (bShow)
    {
        int width2 = rt2.Width();

        m_titleTapeOut.MoveWindow(x + 5, y + 40, rt1.Width(), rt1.Height());
        m_lbTapeOut.MoveWindow(x + 45, y + 20, rt2.Width(), rt2.Height());
        pWnd->MoveWindow(x - 8, y, rt.Width(), rt.Height());
        pWnd->ShowWindow(SW_SHOW);
        m_lbTapeOut.ShowWindow(SW_SHOW);
        //Invalidate();

    }
    else
    {
        m_titleTapeOut.ShowWindow(SW_HIDE);
        m_lbTapeOut.ShowWindow(SW_HIDE);
        pWnd->ShowWindow(SW_HIDE);
    }
}

void CAutoFPDlg::ShowTapeIn(int type, BOOL bShow)
{
    CRect rt, rt1, rt2;
    CWnd *pWnd = GetDlgItem(IDC_STATIC_TAPEIN);//外框
    pWnd->GetWindowRect(rt);
    ScreenToClient(rt);

    CWnd *pWnd1 = GetDlgItem(IDC_TAPEIN_CUR);//文字
    m_titleTapeOut.GetClientRect(rt1);

    CWnd *pWnd2 = GetDlgItem(IDC_COUNT_TAPEIN);//数字
    m_lbTapeOut.GetClientRect(rt2);

    int x1 = rt1.left - rt.left;
    int x2 = rt2.left - rt.left;
    int y1 = rt1.top - rt.top;
    int y2 = rt2.top - rt.top;
    int y = 60;
    int x = 30;
    if (type == 0)  //tape2tape
    {
        y = 60;
    }
    else if (type == 1) //Tape2Tray
    {
        y = 20;
    }
    if (bShow)
    {
        m_titleTapeIn.MoveWindow(x + 5, y + 40, rt1.Width(), rt1.Height());
        m_lbTapeIn.MoveWindow(x + 45, y + 20, rt2.Width(), rt2.Height());
        pWnd->MoveWindow(x - 8, y, rt.Width(), rt.Height());
        pWnd->ShowWindow(SW_SHOW);
        m_lbTapeIn.ShowWindow(SW_SHOW);
        m_lbTapeIn.ShowWindow(SW_SHOW);
        Invalidate();

    }
    else
    {
        m_titleTapeIn.ShowWindow(SW_HIDE);
        m_lbTapeIn.ShowWindow(SW_HIDE);
        pWnd->ShowWindow(SW_HIDE);
    }
}
void CAutoFPDlg::RefreshSocketStatus()
{
    UINT nID[] = { IDC_STATUS11, IDC_STATUS12, IDC_STATUS13, IDC_STATUS14,
    IDC_STATUS21, IDC_STATUS22, IDC_STATUS23, IDC_STATUS24,
    IDC_STATUS31, IDC_STATUS32, IDC_STATUS33, IDC_STATUS34,
    IDC_STATUS41, IDC_STATUS42, IDC_STATUS43, IDC_STATUS44,
    IDC_STATUS51, IDC_STATUS52, IDC_STATUS53, IDC_STATUS54,
    IDC_STATUS61, IDC_STATUS62, IDC_STATUS63, IDC_STATUS64,
    IDC_STATUS71, IDC_STATUS72, IDC_STATUS73, IDC_STATUS74,
    IDC_STATUS81, IDC_STATUS82, IDC_STATUS83, IDC_STATUS84,
    IDC_STATUS91, IDC_STATUS92, IDC_STATUS93, IDC_STATUS94,
    IDC_STATUS101, IDC_STATUS102, IDC_STATUS103, IDC_STATUS104 };
    for (int i = 0; i < MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET; i++)
    {
        int evalue = gm_sBurningStatus[i / MAXNUMOFSOCKET].bEmpty[i%MAXNUMOFSOCKET];
        int count = gm_sBurningStatus[i / MAXNUMOFSOCKET].nAdpStatus[i%MAXNUMOFSOCKET];
        if (gm_bSocketUsed[i])
        {
            //HWND h_static = GetDlgItem(nID[i])->m_hWnd;
            //HDC hdc = ::GetDC(h_static);
            //CDC *pdc = CDC::FromHandle(hdc);
            //CRect rect;
            //::GetClientRect(h_static, rect);
            //pdc->FillSolidRect(rect, RGB(0, 255, 255));
            switch (count)
            {
            case 0:
                SetDlgItemText(nID[i], "ready");
                break;
            case 1:
                SetDlgItemText(nID[i], "烧录中");
                break;
            case 2:
                SetDlgItemText(nID[i], "OK");
                break;
            case 3:
                SetDlgItemText(nID[i], "NG");
                break;
            default:
                break;
            }
        }
        else
        {
            SetDlgItemText(nID[i], "未使用");
        }
    }
}
void CAutoFPDlg::SetRunStatus(LPCSTR info, int type)
{
    if (type == 0)
    {
        m_ctrlStatus.SetTextColor(RGB(0, 255, 0));
        m_ctrlStatus.FlashText(FALSE);
        m_ctrlStatus.SetText(info);
    }
    else if (type == 1)
    {
        m_ctrlStatus.SetTextColor(RGB(255, 0, 0));
        m_ctrlStatus.FlashText(TRUE);
        m_ctrlStatus.SetText(info);
    }
    else if (type == 2)
    {
        m_ctrlStatus.SetTextColor(RGB(255, 255, 255));
        m_ctrlStatus.FlashText(FALSE);
        m_ctrlStatus.SetText(info);
    }
}
void CAutoFPDlg::OnBnClickedBtnPos()
{
    try
    {
        if (m_bCameraOpenUp)
        {
            CloseFramegrabber(m_hv_AcqHandle_Up);
            m_bCameraOpenUp = FALSE;
        }
        if (m_bCameraOpenDown)
        {
            CloseFramegrabber(m_hv_AcqHandle_Down);
            m_bCameraOpenDown = FALSE;
        }
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        //MessageBox("没有需要关闭的相机");
    }
    SetUpCameraLightOn();
    CDlgPosition dlg;
    int nRet = dlg.DoModal();
    SetUpCameraLightOff();
    SetDownCameraLightOff();

    if (gm_bTapeCheck && !m_bCameraOpenUp)
        OpenFramegrabUp();
    if ((gm_bDownCameraIn || gm_bDownCameraOut) && !m_bCameraOpenDown)
        OpenFramegrabDown();
    //InitSockStatus(TRUE);

}
void CAutoFPDlg::OnBnClickedBtnSetting()
{
    CDlgSetting dlg;
    int nRet = dlg.DoModal();
    //设置主界面进出料显示
    if (gm_uInfo.nWorkMode == TRAY_TO_TRAY)
    {
        ShowTapeIn(0, FALSE);
        ShowTapeOut(0, FALSE);
    }
    else if (gm_uInfo.nWorkMode == TAPE_TO_TRAY)
    {
        ShowTapeIn(1, TRUE);
        ShowTapeOut(0, FALSE);
    }
    else if (gm_uInfo.nWorkMode == TAPE_TO_TAPE)
    {
        ShowTapeIn(0, TRUE);
        ShowTapeOut(0, TRUE);
    }
    else if (gm_uInfo.nWorkMode == TRAY_TO_TAPE)
    {
        ShowTapeIn(0, FALSE);
        ShowTapeOut(1, TRUE);
    }
    gm_nNozzleNum = 0;
    for (int i = 0; i < 4; i++)
    {
        if (gm_bNozzleUsed[i])
            gm_nNozzleNum++;
    }
    ////刷新tray盘显示
    if (m_xn != gm_dataTray.xn || m_yn != gm_dataTray.yn)
    {
        m_xn = gm_dataTray.xn;
        m_yn = gm_dataTray.yn;
        //Invalidate(FALSE);
        delete[]gm_pTray;
        gm_pTray = NULL;
        gm_pTray = new BYTE[gm_dataTray.xn*gm_dataTray.yn];

        int total = gm_dataTray.xn * gm_dataTray.yn;
        for (int i = 0; i < total; i++)
            gm_pTray[i] = 2;
    }
    InvalidateRect(&m_rt, TRUE);

    CString str;
    str.Format("%d", gm_sdInfo.nLotNum);
    SetDlgItemText(IDC_MAIN_WORKNUM_SUM, str);
    SetDlgItemText(IDC_MAIN_CHIP_MODEL, gm_sdInfo.strICModel);
    SetDlgItemText(IDC_MAIN_CHIP_BRAND, gm_sdInfo.strICBrand);
    SetDlgItemText(IDC_MAIN_CHECK_SUM, gm_sdInfo.strCheckSum);
    //SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_uInfo.strProcName);
    SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_sdInfo.strWorkNo);
    //InitSockStatus(TRUE);

}

void CAutoFPDlg::OnBnClickedBtnRun()
{
    //if (!CleanSocket())
    //{
    //	GetDlgItem(IDC_BTN_RUN)->EnableWindow();
    //	GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow();
    //	GetDlgItem(IDC_BTN_FINISH)->EnableWindow();
    //	GetDlgItem(IDC_BTN_EXIT)->EnableWindow();
    //	GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow();
    //	GetDlgItem(IDC_BTN_GOHOME)->EnableWindow();
    //	return;
    //}
    //CRect rt;
    //GetDlgItem(IDC_CAMERA_FRAME)->GetWindowRect(&rt);
    //ScreenToClient(rt);
    //InvalidateRect(&rt, TRUE);

    if (gm_bTapeCamera)
        CreateTapeModel();

    if (!CheckPositiveMeter())
    {
        gm_bBeep = TRUE;
        AfxBeginThread(BeepThreadProc, NULL);
        if (IDNO == AfxMessageBox("气压较低,请检测气压!\r\n是否继续执行...", MB_YESNO))
        {
            gm_bBeep = FALSE;
            return;
        }
        gm_bBeep = FALSE;
        gm_logFile.Write("提示气压较低，用户选择继续运行。");
    }
    AppendLogMessage("----运行----");
    gm_logFile.Write("----运行----");
    gm_logFile.Write("运行参数：");
    CString strLog;
    strLog.Format("进出料方式：%d XY轴速度：%d Z轴速度：%d	\n \
下相机：%d 编带进出料检测：%d 芯片尺寸：%0.2f×%0.2f \n \
单位换算：下相机 1mm=%0.2fpixels 上相机 1mm=%0.2fpixels	\n \
补料：%d	 自动Tray盘机：%d",
gm_uInfo.nWorkMode,
gm_uInfo.xySpeed,
gm_uInfo.zSpeed,
gm_bDownCameraIn,
gm_bTapeCheck,
gm_uInfo.nChipLength,
gm_uInfo.nChipWidth,
gm_uInfo.nPixelsPM_down,
gm_uInfo.nPixelsPM_up,
gm_bStock,
gm_bAutoTray);
    gm_logFile.Write(strLog);
    GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_FINISH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_GOHOME)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_FP_NG)->EnableWindow(FALSE);

    GetDlgItem(IDC_CHECK_PROGRAM1)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM2)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM3)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM4)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM5)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM6)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM7)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM8)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM9)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM10)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK11)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK12)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK13)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK14)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK21)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK22)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK23)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK24)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK31)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK32)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK33)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK34)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK41)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK42)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK43)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK44)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK51)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK52)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK53)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK54)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK61)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK62)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK63)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK64)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK71)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK72)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK73)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK74)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK81)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK82)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK83)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK84)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK91)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK92)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK93)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK94)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK101)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK102)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK103)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK104)->EnableWindow(FALSE);

    gm_bStop = FALSE;
    gm_bStart = TRUE;
    gm_bFinish = FALSE;

    //GetDlgItem(IDC_BTN_POS)->EnableWindow(FALSE);
    //GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);

    if (gm_bTapeCheck && !m_bCameraOpenUp)
        OpenFramegrabUp();
    if ((gm_bDownCameraIn || gm_bDownCameraOut) && !m_bCameraOpenDown)
        OpenFramegrabDown();
    StartPLC();

    if (gm_bStock)
        gm_nTotalStock = gm_dataStock.xn*gm_dataStock.yn;
    else
        gm_nTotalStock = 0;

    SendInfo("Slave Mode");//发送连接编程器信号
    SetRunStatus("Running...");
    SetTimer(TIMER_RUN_TIME, 1000, NULL);//开启烧录时间计时定时器
    //AfxBeginThread(BurningThread, this);
    AfxBeginThread(BurningProc, this);
}

void CAutoFPDlg::OnBnClickedBtnSuspend()
{
    AppendLogMessage("----暂停----");
    gm_logFile.Write("----暂停----");
    if (gm_bStart && !gm_bFinish)
        GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow(FALSE);

    gm_bStop = TRUE;

    //CString str;
    //GetDlgItemText(IDC_MFCEDITBROWSE1, str);
    //MessageBox(str);
    //try
    //{
    //	if (m_bCameraOpenUp)
    //	{
    //		CloseFramegrabber(m_hv_AcqHandle_Up);
    //		m_bCameraOpenUp = FALSE;
    //	}
    //	if (m_bCameraOpenDown)
    //	{
    //		CloseFramegrabber(m_hv_AcqHandle_Down);
    //		m_bCameraOpenDown = FALSE;
    //	}
    //}
    //catch (HalconCpp::HException &HDevExpDefaultException)
    //{
    //	//MessageBox("没有需要关闭的相机");
    //}
    //GetDlgItem(IDC_MAIN_SUSPEND)->EnableWindow();
}

void CAutoFPDlg::OnBnClickedBtnFinish()
{
    AppendLogMessage("----结束----");
    gm_logFile.Write("----结束----");
    if (gm_bTapeCheck && !m_bCameraOpenUp)
        OpenFramegrabUp();
    if ((gm_bDownCameraIn || gm_bDownCameraOut) && !m_bCameraOpenDown)
        OpenFramegrabDown();
    gm_bStop = FALSE;
    gm_bStart = FALSE;
    gm_bFinish = TRUE;

    GetDlgItem(IDC_BTN_RUN)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_SUSPEND)->EnableWindow(TRUE);
    GetDlgItem(IDC_BTN_FINISH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_GOHOME)->EnableWindow(FALSE);
    GetDlgItem(IDC_BTN_FP_NG)->EnableWindow(FALSE);

    GetDlgItem(IDC_CHECK_PROGRAM1)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM2)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM3)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM4)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM5)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM6)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM7)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM8)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM9)->EnableWindow(FALSE);
    GetDlgItem(IDC_CHECK_PROGRAM10)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK11)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK12)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK13)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK14)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK21)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK22)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK23)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK24)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK31)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK32)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK33)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK34)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK41)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK42)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK43)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK44)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK51)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK52)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK53)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK54)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK61)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK62)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK63)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK64)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK71)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK72)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK73)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK74)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK81)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK82)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK83)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK84)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK91)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK92)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK93)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK94)->EnableWindow(FALSE);

    GetDlgItem(IDC_SOCKET_CHECK101)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK102)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK103)->EnableWindow(FALSE);
    GetDlgItem(IDC_SOCKET_CHECK104)->EnableWindow(FALSE);

    StartPLC();

    SetRunStatus("Running...");
    SetTimer(TIMER_RUN_TIME, 1000, NULL);//开启烧录时间计时定时器
    //AfxBeginThread(BurningThread, this);
    AfxBeginThread(BurningProc, this);
}
////暂停按钮
//void CAutoFPDlg::Close()
//{
//
//}


void CAutoFPDlg::SetSocketInfo(int sock, LPCSTR info)
{
    UINT nID[] = { IDC_STATUS11, IDC_STATUS12, IDC_STATUS13, IDC_STATUS14,
    IDC_STATUS21, IDC_STATUS22, IDC_STATUS23, IDC_STATUS24,
    IDC_STATUS31, IDC_STATUS32, IDC_STATUS33, IDC_STATUS34,
    IDC_STATUS41, IDC_STATUS42, IDC_STATUS43, IDC_STATUS44,
    IDC_STATUS51, IDC_STATUS52, IDC_STATUS53, IDC_STATUS54,
    IDC_STATUS61, IDC_STATUS62, IDC_STATUS63, IDC_STATUS64,
    IDC_STATUS71, IDC_STATUS72, IDC_STATUS73, IDC_STATUS74,
    IDC_STATUS81, IDC_STATUS82, IDC_STATUS83, IDC_STATUS84,
    IDC_STATUS91, IDC_STATUS92, IDC_STATUS93, IDC_STATUS94,
    IDC_STATUS101, IDC_STATUS102, IDC_STATUS103, IDC_STATUS104 };
    SetDlgItemText(nID[sock], info);
}
void CAutoFPDlg::ShowYield(float yield)
{
    CString strTmp;
    strTmp.Format("%0.2f%%", yield * 100);
    m_ctrlYield.SetText(strTmp);
}
//OK数量显示
void CAutoFPDlg::ShowGoodSize(int size)
{
    CString strTmp;
    strTmp.Format("%d", size);
    m_ctrlGood.SetText(strTmp);
}
//NG数量显示
void CAutoFPDlg::ShowFailSize(int fpFail, int checkFail)
{
    CString strTmp;
    strTmp.Format("%d", fpFail);
    m_ctrlFail.SetText(strTmp);

    strTmp.Format("%d", checkFail);
    SetDlgItemTextA(IDC_MAIN_SYS_NG, strTmp);
}
typedef struct
{
    char Serial[16];  // SerialNumber
    int  DrvIndex;   // -1 = not present
    int  DevStatus;  // Reserved
    char ProjName[255];
    DWORD CheckSum;
}CTest;

//BOOL CAutoFPDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
//{
//	// TODO: Add your message handler code here and/or call default
//	int nBufLen;
//	int i=0;
//	BYTE status;
//	CString strTmp;
//	if(pCopyDataStruct->dwData == SLAVE_DATA)
//	{
//		nBufLen =  pCopyDataStruct->cbData;
//		TPbInfo *pMsg = (TPbInfo*)(pCopyDataStruct->lpData);	
//		CString temp;
//		CString sf;
//		char chTmp[2000];
//		memcpy(chTmp,pCopyDataStruct->lpData,nBufLen);
//	//	AfxMessageBox(chTmp);
//
//	//	CTest *pTest = (CTest*)(pCopyDataStruct->lpData);
//		switch(pMsg->MsgType)
//		{
//		case MT_PBINIT:
//			
//			gm_sBurningStatus[pMsg->Index].nAdpStatus[0] = 1;
//			gm_sBurningStatus[pMsg->Index].nAdpStatus[1] = 1;
//			gm_sBurningStatus[pMsg->Index].nAdpStatus[2] = 1;
//			gm_sBurningStatus[pMsg->Index].nAdpStatus[3] = 1;
//		//	AfxMessageBox("连接成功!!");
//			break;
//		case MT_PBPOS:
//	
//			
//			break;
//		case MT_CONNECTED:
//		
//			break;
//		case MT_CHECKSUM:
//			
//			strTmp.Format("%8X",pMsg->Step);
//		//	SetDlgItemText(IDC_MAIN_MFR,strTmp);
//			break;
//		case MT_FILE_CHECKSUM:
//			break;
//		case MT_RESULT:
//			
//			status = pMsg->AdpStatus;
//
//			CString stt;
//			stt.Format("编程器 %d 结果:%d",pMsg->Index,status);
//		//	AfxMessageBox(stt);
//			for(i=0;i<4;i++)
//			{
//				if ((status & (1 << i)) != 0 )
//					gm_sBurningStatus[pMsg->Index].nAdpStatus[i] = 2;
//				else
//					gm_sBurningStatus[pMsg->Index].nAdpStatus[i] = 3;
//			}
//			break;
//		}
//	}
//	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
//}

#define XELTEK5K_HARDWAREID_ALIAS "usb\\vid_6022&pid_5000"
BOOL CAutoFPDlg::EnumDevices()
{
    m_deviceCount = 0;
    gm_logFile.Write("开始查找编程器!");
    for (int p = 0; p < MAXNUMOFPROGRAMMER; p++)
        gm_bSocketUsed[p] = FALSE;
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    hDevInfo = SetupDiGetClassDevs(m_lpGuid,
        0, // Enumerator
        0,
        DIGCF_PRESENT);

    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        // Insert error handling here.
        AfxMessageBox("查找失败");
        //		SetupDiDestroyDeviceInfoList(hDevInfo);
        return FALSE;
    }
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD i;
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
    {
        DWORD       dwRegType;
        TCHAR       harewareId[MAX_PATH];
        SetupDiGetDeviceRegistryProperty(hDevInfo,
            &DeviceInfoData,
            SPDRP_HARDWAREID,
            &dwRegType,
            (BYTE*)harewareId,
            sizeof(harewareId),
            NULL);
        CString hwid;
        hwid = harewareId;
        hwid.MakeLower();
        int result1 = hwid.Find(XELTEK5K_HARDWAREID_ALIAS);
        if (result1 != -1)
        {
            TCHAR buf[100];
            DWORD buffersize = 0;
            CString insResult;
            insResult.Empty();
            if (SetupDiGetDeviceInstanceId(hDevInfo, &DeviceInfoData, buf, sizeof(buf), &buffersize))
            {
                ExtractInstanceId(buf, insResult);
                gm_logFile.Write(insResult);
                insResult.MakeLower();
                char tempData[1];
                strcpy(tempData, "");
                tempData[0] = insResult.GetAt(insResult.GetLength() - 1);
                int digitalValue = atoi(tempData) - 1;
                gm_bSocketUsed[i] = true;
            }
            m_deviceCount++;
        }
    }
    CString logstr;
    logstr.Format("找到%d个编程器!", m_deviceCount);
    gm_logFile.Write(logstr);
    SetupDiDestroyDeviceInfoList(hDevInfo);
    return TRUE;
}

void CAutoFPDlg::ExtractInstanceId(LPCTSTR insStr, CString &result)
{
    ASSERT(insStr != NULL);
    CString source = insStr;
    int pos = 0;
    pos = source.ReverseFind('\\');
    result = source.Mid(pos + 1, source.GetLength() - pos - 1);
}

void CAutoFPDlg::EnableRun(BOOL bEnabled)
{
    m_btnRun.EnableWindow(bEnabled);
}

void CAutoFPDlg::OnSocketCheck1()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK1);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[0] = TRUE;
        gm_bSocketUsed[1] = TRUE;
        gm_bSocketUsed[2] = TRUE;
        gm_bSocketUsed[3] = TRUE;
    }
    else
    {
        gm_bSocketUsed[0] = FALSE;
        gm_bSocketUsed[1] = FALSE;
        gm_bSocketUsed[2] = FALSE;
        gm_bSocketUsed[3] = FALSE;
    }

}

void CAutoFPDlg::OnSocketCheck2()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK2);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[4] = TRUE;
        gm_bSocketUsed[5] = TRUE;
        gm_bSocketUsed[6] = TRUE;
        gm_bSocketUsed[7] = TRUE;
    }
    else
    {
        gm_bSocketUsed[4] = FALSE;
        gm_bSocketUsed[5] = FALSE;
        gm_bSocketUsed[6] = FALSE;
        gm_bSocketUsed[7] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck3()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK3);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[8] = TRUE;
        gm_bSocketUsed[9] = TRUE;
        gm_bSocketUsed[10] = TRUE;
        gm_bSocketUsed[11] = TRUE;
    }
    else
    {
        gm_bSocketUsed[8] = FALSE;
        gm_bSocketUsed[9] = FALSE;
        gm_bSocketUsed[10] = FALSE;
        gm_bSocketUsed[11] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck4()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK4);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[12] = TRUE;
        gm_bSocketUsed[13] = TRUE;
        gm_bSocketUsed[14] = TRUE;
        gm_bSocketUsed[15] = TRUE;
    }
    else
    {
        gm_bSocketUsed[12] = FALSE;
        gm_bSocketUsed[13] = FALSE;
        gm_bSocketUsed[14] = FALSE;
        gm_bSocketUsed[15] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck5()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK5);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[16] = TRUE;
        gm_bSocketUsed[17] = TRUE;
        gm_bSocketUsed[18] = TRUE;
        gm_bSocketUsed[19] = TRUE;
    }
    else
    {
        gm_bSocketUsed[16] = FALSE;
        gm_bSocketUsed[17] = FALSE;
        gm_bSocketUsed[18] = FALSE;
        gm_bSocketUsed[19] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck6()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK6);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[20] = TRUE;
        gm_bSocketUsed[21] = TRUE;
        gm_bSocketUsed[22] = TRUE;
        gm_bSocketUsed[23] = TRUE;
    }
    else
    {
        gm_bSocketUsed[20] = FALSE;
        gm_bSocketUsed[21] = FALSE;
        gm_bSocketUsed[22] = FALSE;
        gm_bSocketUsed[23] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck7()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK7);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[24] = TRUE;
        gm_bSocketUsed[25] = TRUE;
        gm_bSocketUsed[26] = TRUE;
        gm_bSocketUsed[27] = TRUE;
    }
    else
    {
        gm_bSocketUsed[24] = FALSE;
        gm_bSocketUsed[25] = FALSE;
        gm_bSocketUsed[26] = FALSE;
        gm_bSocketUsed[27] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck8()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK8);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[28] = TRUE;
        gm_bSocketUsed[29] = TRUE;
        gm_bSocketUsed[30] = TRUE;
        gm_bSocketUsed[31] = TRUE;
    }
    else
    {
        gm_bSocketUsed[28] = FALSE;
        gm_bSocketUsed[29] = FALSE;
        gm_bSocketUsed[30] = FALSE;
        gm_bSocketUsed[31] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck9()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK9);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[32] = TRUE;
        gm_bSocketUsed[33] = TRUE;
        gm_bSocketUsed[34] = TRUE;
        gm_bSocketUsed[35] = TRUE;
    }
    else
    {
        gm_bSocketUsed[32] = FALSE;
        gm_bSocketUsed[33] = FALSE;
        gm_bSocketUsed[34] = FALSE;
        gm_bSocketUsed[35] = FALSE;
    }
}

void CAutoFPDlg::OnSocketCheck10()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK10);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[36] = TRUE;
        gm_bSocketUsed[37] = TRUE;
        gm_bSocketUsed[38] = TRUE;
        gm_bSocketUsed[39] = TRUE;
    }
    else
    {
        gm_bSocketUsed[36] = FALSE;
        gm_bSocketUsed[37] = FALSE;
        gm_bSocketUsed[38] = FALSE;
        gm_bSocketUsed[39] = FALSE;
    }
}

void CAutoFPDlg::DisplayGood(int socket, int size)
{
    CString num;
    num.Format("%d", size);
    if (socket == 0)
    {
        m_good11.SetText(num);
    }
    else if (socket == 1)
    {
        m_good12.SetText(num);
    }
    else if (socket == 2)
    {
        m_good13.SetText(num);
    }
    else if (socket == 3)
    {
        m_good14.SetText(num);
    }
    else if (socket == 4)
    {
        m_good21.SetText(num);
    }
    else if (socket == 5)
    {
        m_good22.SetText(num);
    }
    else if (socket == 6)
    {
        m_good23.SetText(num);
    }
    else if (socket == 7)
    {
        m_good24.SetText(num);
    }
    else if (socket == 8)
    {
        m_good31.SetText(num);
    }
    else if (socket == 9)
    {
        m_good32.SetText(num);
    }
    else if (socket == 10)
    {
        m_good33.SetText(num);
    }
    else if (socket == 11)
    {
        m_good34.SetText(num);
    }
    else if (socket == 12)
    {
        m_good41.SetText(num);
    }
    else if (socket == 13)
    {
        m_good42.SetText(num);
    }
    else if (socket == 14)
    {
        m_good43.SetText(num);
    }
    else if (socket == 15)
    {
        m_good44.SetText(num);
    }
    else if (socket == 16)
    {
        m_good51.SetText(num);
    }
    else if (socket == 17)
    {
        m_good52.SetText(num);
    }
    else if (socket == 18)
    {
        m_good53.SetText(num);
    }
    else if (socket == 19)
    {
        m_good54.SetText(num);
    }
    else if (socket == 20)
    {
        m_good61.SetText(num);
    }
    else if (socket == 21)
    {
        m_good62.SetText(num);
    }
    else if (socket == 22)
    {
        m_good63.SetText(num);
    }
    else if (socket == 23)
    {
        m_good64.SetText(num);
    }
    else if (socket == 24)
    {
        m_good71.SetText(num);
    }
    else if (socket == 25)
    {
        m_good72.SetText(num);
    }
    else if (socket == 26)
    {
        m_good73.SetText(num);
    }
    else if (socket == 27)
    {
        m_good74.SetText(num);
    }
    else if (socket == 28)
    {
        m_good81.SetText(num);
    }
    else if (socket == 29)
    {
        m_good82.SetText(num);
    }
    else if (socket == 30)
    {
        m_good83.SetText(num);
    }
    else if (socket == 31)
    {
        m_good84.SetText(num);
    }
    else if (socket == 32)
    {
        m_good91.SetText(num);
    }
    else if (socket == 33)
    {
        m_good92.SetText(num);
    }
    else if (socket == 34)
    {
        m_good93.SetText(num);
    }
    else if (socket == 35)
    {
        m_good94.SetText(num);
    }
    else if (socket == 36)
    {
        m_good101.SetText(num);
    }
    else if (socket == 37)
    {
        m_good102.SetText(num);
    }
    else if (socket == 38)
    {
        m_good103.SetText(num);
    }
    else if (socket == 39)
    {
        m_good104.SetText(num);
    }
}

void CAutoFPDlg::DisplayFail(int socket, int size)
{
    CString num;
    num.Format("%d", size);
    if (socket == 0)
    {
        m_fail11.SetText(num);
    }
    else if (socket == 1)
    {
        m_fail12.SetText(num);
    }
    else if (socket == 2)
    {
        m_fail13.SetText(num);
    }
    else if (socket == 3)
    {
        m_fail14.SetText(num);
    }
    else if (socket == 4)
    {
        m_fail21.SetText(num);
    }
    else if (socket == 5)
    {
        m_fail22.SetText(num);
    }
    else if (socket == 6)
    {
        m_fail23.SetText(num);
    }
    else if (socket == 7)
    {
        m_fail24.SetText(num);
    }
    else if (socket == 8)
    {
        m_fail31.SetText(num);
    }
    else if (socket == 9)
    {
        m_fail32.SetText(num);
    }
    else if (socket == 10)
    {
        m_fail33.SetText(num);
    }
    else if (socket == 11)
    {
        m_fail34.SetText(num);
    }
    else if (socket == 12)
    {
        m_fail41.SetText(num);
    }
    else if (socket == 13)
    {
        m_fail42.SetText(num);
    }
    else if (socket == 14)
    {
        m_fail43.SetText(num);
    }
    else if (socket == 15)
    {
        m_fail44.SetText(num);
    }
    else if (socket == 16)
    {
        m_fail51.SetText(num);
    }
    else if (socket == 17)
    {
        m_fail52.SetText(num);
    }
    else if (socket == 18)
    {
        m_fail53.SetText(num);
    }
    else if (socket == 19)
    {
        m_fail54.SetText(num);
    }
    else if (socket == 20)
    {
        m_fail61.SetText(num);
    }
    else if (socket == 21)
    {
        m_fail62.SetText(num);
    }
    else if (socket == 22)
    {
        m_fail63.SetText(num);
    }
    else if (socket == 23)
    {
        m_fail64.SetText(num);
    }
    else if (socket == 24)
    {
        m_fail71.SetText(num);
    }
    else if (socket == 25)
    {
        m_fail72.SetText(num);
    }
    else if (socket == 26)
    {
        m_fail73.SetText(num);
    }
    else if (socket == 27)
    {
        m_fail74.SetText(num);
    }
    else if (socket == 28)
    {
        m_fail81.SetText(num);
    }
    else if (socket == 29)
    {
        m_fail82.SetText(num);
    }
    else if (socket == 30)
    {
        m_fail83.SetText(num);
    }
    else if (socket == 31)
    {
        m_fail84.SetText(num);
    }
    else if (socket == 32)
    {
        m_fail91.SetText(num);
    }
    else if (socket == 33)
    {
        m_fail92.SetText(num);
    }
    else if (socket == 34)
    {
        m_fail93.SetText(num);
    }
    else if (socket == 35)
    {
        m_fail94.SetText(num);
    }
    else if (socket == 36)
    {
        m_fail101.SetText(num);
    }
    else if (socket == 37)
    {
        m_fail102.SetText(num);
    }
    else if (socket == 38)
    {
        m_fail103.SetText(num);
    }
    else if (socket == 39)
    {
        m_fail104.SetText(num);
    }
}

void CAutoFPDlg::EnableFinish(BOOL enabled)
{
    m_btnFinish.EnableWindow(enabled);
}

void CAutoFPDlg::WholeTrayRfresh()
{
    for (int i = 0; i < gm_dataTray.xn*gm_dataTray.yn; i++)
    {
        gm_pTray[i] = 2;
    }
    //Invalidate(FALSE);
    InvalidateRect(&m_rt, TRUE);
    //ChangeTrayStatus(i,2);
}
void CAutoFPDlg::ChangeTrayStatus(int currentIndex, BYTE status)
{
    //int  MINEAREA_FRAME_X = m_rt.left;
    //int  MINEAREA_FRAME_Y = m_rt.top;

    //int xWidth = m_rt.Width() / m_xn;
    //int yHeight = m_rt.Height() / m_yn;

    //int xn = currentIndex%m_xn;
    //int yn = currentIndex / m_xn;
    //RECT rt;
    //rt.left = MINEAREA_FRAME_X + xWidth * xn;
    //rt.right = rt.left + xWidth;
    //rt.top = MINEAREA_FRAME_Y + yHeight*yn;
    //rt.bottom = rt.top + yHeight;

    gm_pTray[currentIndex] = status;

    InvalidateRect(&m_rt, TRUE);
    //Sleep(15);
    //InvalidateRect(&rt,FALSE);
}

//LRESULT CAutoFPDlg::OnBurnAgain(WPARAM wParam, LPARAM lParam)
//{
//	int currentMoveOutSocket = wParam;
//	PushDownSocket(currentMoveOutSocket);
//	BOOL bPressed = CheckSocketPressed(currentMoveOutSocket);
//	DWORD dwStart = ::GetTickCount();
//	BOOL bFailed=FALSE;
//	while(!bPressed)
//	{
//		bPressed = CheckSocketPressed(currentMoveOutSocket);
//		Sleep(200);
//		DWORD dwElapse=::GetTickCount()-dwStart;
//		if(dwElapse>3000)
//		{
//			bFailed = TRUE;
//			break;
//		}
//	}
//	if(bFailed)
//	{
//		return 0;
//	}
//	Sleep(200);
//	PullUpSocket(currentMoveOutSocket);
//	bPressed = CheckSocketPressed(currentMoveOutSocket);
//	dwStart = ::GetTickCount();
//	bFailed=FALSE;
//	while(bPressed)
//	{
//		bPressed = CheckSocketPressed(currentMoveOutSocket);
//		Sleep(200);
//		DWORD dwElapse=::GetTickCount()-dwStart;
//		if(dwElapse>3000)
//		{
//			bFailed = TRUE;
//			break;
//		}
//	}
//	if(bFailed)
//	{
//		return 0;
//	}
//	//SetSpecialInfo(currentMoveOutSocket,"empty",0);
//	//SetSpecialInfo(currentMoveOutSocket,"Tag",0);
//	
//	sockPutPreference[currentMoveOutSocket] = 0;
//	AddPutPreference();
////	gm_bPushed[currentMoveOutSocket] = true;
//	
//	CString strlast;
//	strlast.Format("%d:重压!",currentMoveOutSocket);
//	gm_logFile.Write(strlast);
//	return 1;
//}
LRESULT CAutoFPDlg::OnCheckStatus(WPARAM wParam, LPARAM lParam)
{
    unsigned short nDevice = wParam;
    AfxBeginThread(CheckProgrammerProc, &nDevice);
    return 1;
}
UINT CheckProgrammerProc(LPVOID lPara)
{
    Sleep(200);
    DWORD dwStart = GetTickCount();

    unsigned short *pDevice = (unsigned short*)lPara;
    unsigned short nSock = *pDevice;
    CAutoFPDlg *pDlg = (CAutoFPDlg*)AfxGetMainWnd();
    CString info;
    while (TRUE)
    {
        info = pDlg->GetCurrentProgInfo(nSock);
        if (info.Find("运行"))
        {
            //SetSpecialInfo(nSock,"Busy",1);
            //SetSpecialInfo(nSock,"Tag",1);
        }
        else if (info.Find("成功"))
        {
            //SetSpecialInfo(nSock,"Busy",0);
            //SetSpecialInfo(nSock,"Status",1);
            break;
        }
        else if (info.Find("失败"))
        {
            //SetSpecialInfo(nSock,"Busy",0);
            //SetSpecialInfo(nSock,"Status",0);	
            break;
        }
        else if (info.Find("插入"))
        {
            DWORD dwSpace = GetTickCount() - dwStart;
            if (dwSpace > 5000)
            {
                //SetSpecialInfo(nSock,"Busy",0);
                //SetSpecialInfo(nSock,"Status",0);
                break;
            }
        }
        gm_logFile.Write(info);
        Sleep(400);
    }
    return 0;
}
CString CAutoFPDlg::GetCurrentProgInfo(int nIndex)
{
    CString sInfo;
    LPTSTR    lptstr;
    HGLOBAL   hglb;
    HWND hwnd = gm_hArray[nIndex];
    ::SendMessage(hwnd, EM_SETSEL, 0, -1); //start selecting
    ::SendMessage(hwnd, WM_COPY, 0, 0);
    ::SendMessage(hwnd, EM_SETSEL, -1, 0); //end selecting

    if (!IsClipboardFormatAvailable(CF_TEXT))
    {
        sInfo = "";
        return sInfo;
    }

    if (!::OpenClipboard(NULL))
    {
        sInfo = "";
        return sInfo;
    }
    hglb = GetClipboardData(CF_TEXT);
    if (hglb != NULL)
    {
        lptstr = (LPTSTR)GlobalLock(hglb);
        GlobalUnlock(hglb);
        EmptyClipboard();
        CloseClipboard();
        sInfo = lptstr;
    }
    else
        sInfo = "";
    return sInfo;
}
BOOL CALLBACK CAutoFPDlg::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    TCHAR buff[1000];
    int buffsize = 100;
    HWND hMSNWnd;
    hMSNWnd = NULL;

    ::GetWindowText(hwnd, buff, buffsize);
    if (strlen(buff) < 1)
        return TRUE;

    CString strTemp = buff;

    int pos = 0;
    pos = strTemp.Find("SUPERPRO 5004GP 视窗版");
    if (pos != -1)
        EnumChildWindows(hwnd, ChildWndProc, 0);

    return TRUE;
}

//剪切板传数据
BOOL CALLBACK CAutoFPDlg::ChildWndProc(HWND hwnd, LPARAM lParam)
{
    LPTSTR    lptstr;
    HGLOBAL   hglb;
    char wndowclass[CLASS_SIZE];

    if (GetClassName(hwnd, wndowclass, CLASS_SIZE) == 0)
        return TRUE;

    CString strTemp(wndowclass);
    if (strTemp == "Edit")
    {
        ::SendMessage(hwnd, EM_SETSEL, 0, -1); //start selecting
        ::SendMessage(hwnd, WM_COPY, 0, 0);
        ::SendMessage(hwnd, EM_SETSEL, -1, 0); //end selecting

        if (!IsClipboardFormatAvailable(CF_TEXT))
            return TRUE;

        if (!::OpenClipboard(NULL))
            return TRUE;

        hglb = GetClipboardData(CF_TEXT);
        if (hglb != NULL)
        {
            lptstr = (LPTSTR)GlobalLock(hglb);
            GlobalUnlock(hglb);
            EmptyClipboard();
            CloseClipboard();

            if (gm_nHCount < 4)
            {
                gm_hArray[gm_nHCount] = hwnd;
                gm_nHCount++;
            }
        }

    }
    return TRUE;
}






void CAutoFPDlg::SendSimpleCmd(BYTE cmd)
{
    LRESULT copyDataResult;
    CWnd *pOtherWnd = CWnd::FindWindow("TFrmMulti", "Multi Control System for Windows");
    if (pOtherWnd)
    {
        COPYDATASTRUCT cpd;
        cpd.dwData = MASTER_DATA;

        cpd.cbData = 1;
        cpd.lpData = (void*)&cmd;
        copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
            (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
            (LPARAM)&cpd);
    }
}
void CAutoFPDlg::SendComplexCmd(BYTE cmd, CString s)
{
    BYTE temp[256];
    LRESULT copyDataResult;
    CWnd *pOtherWnd = CWnd::FindWindow("TFrmMulti", "Multi Control System for Windows");
    if (pOtherWnd)
    {
        COPYDATASTRUCT cpd;
        cpd.dwData = MASTER_DATA;
        temp[0] = cmd;
        temp[1] = (BYTE)s.GetLength();
        for (int i = 0; i < s.GetLength(); i++)
            temp[2 + i] = s.GetAt(i);
        cpd.cbData = s.GetLength() + 2;
        cpd.lpData = (void*)temp;
        copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
            (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
            (LPARAM)&cpd);
    }

}

void CAutoFPDlg::StartOneProgrammer(BYTE nIndex)
{
    CString str;
    if (m_bSocketConnect)
    {
        //通用板开始烧录信号
        unsigned long len1;
        str.Format("*LED_Check,com_dev%d_start,\r\n", nIndex);
        BOOL bRet = m_SocketPort.write_scc(str.GetBuffer(0), str.GetLength(), len1);
        str.ReleaseBuffer();
    }
    else
    {
        str.Format("Run Prog#%d", nIndex);
        SendInfo(str);
    }
}



void CAutoFPDlg::OnClose()
{
    // TODO: Add your message handler code here and/or call default
    //delete[]m_pTrayStatus;
    //m_pTrayStatus = NULL;
    CDialog::OnClose();
}


void CAutoFPDlg::ChangeTapeIn(int nCount)
{
    CString str;
    str.Format("%d", nCount + 1);
    m_lbTapeIn.SetText(str);
}
void CAutoFPDlg::ChangeTapeOut(int nCount)
{
    CString str;
    str.Format("%d", nCount + 1);
    m_lbTapeOut.SetText(str);
}



CLogFile fl;


BOOL CAutoFPDlg::SendCommand(LPCSTR command)
{
    CString strCommand;
    strCommand = command;
    unsigned long len1;
    BOOL bRet = m_port.write_scc(strCommand.GetBuffer(0), strCommand.GetLength(), len1);
    strCommand.ReleaseBuffer();
    //Sleep(10);
    return bRet;
}

BOOL CAutoFPDlg::SendCommandToTray(LPCSTR command)
{
    //CString strCommand;
    //strCommand = command;
    ////BYTE buff[255];
    //unsigned char cbuf[256];
    //memset(cbuf,0,sizeof(cbuf));
    ////StrToHex(strCommand,cbuf);
    //str_to_hex(strCommand.GetBuffer(0),cbuf,strCommand.GetLength());
    //unsigned long len1;
    //BOOL bRet = m_TrayPort.write_scc( cbuf,strCommand.GetLength(),len1);
    //strCommand.ReleaseBuffer();
    //Sleep(10);
    //return bRet;
    CString strCommand;
    strCommand = command;
    unsigned long len1;
    char data[80];
    int len = Str2Hex(strCommand, data);
    BOOL bRet = m_TrayPort.write_scc(data, len, len1);

    Sleep(10);
    return bRet;
}
int CAutoFPDlg::Str2Hex(CString str, char *data)
{
    int t, t1;
    int rlen = 0;//字节数组长度
    int len = str.GetLength();
    //data.SetSize(len/2);
    for (int i = 0; i < len;)
    {
        char l;
        char h = str[i];
        if (h == ' ')
        {
            i++;
            continue;
        }
        i++;
        if (i >= len)
            break;
        l = str[i];
        t = HexChar(h);
        t1 = HexChar(l);
        if ((t == 16) || (t1 == 16))
            break;
        else
            t = t * 16 + t1;//?????
        i++;
        data[rlen] = (char)t;
        rlen++;
    }
    return rlen;
}
char CAutoFPDlg::HexChar(char c)
{
    if ((c >= '0') && (c <= '9'))
        return char(c - 0x30);
    else if ((c >= 'A') && (c <= 'F'))
        return char(c - 'A' + 10);
    else if ((c >= 'a') && (c <= 'f'))
        return char(c - 'a' + 10);
    else if (c == ' ')
        return char(0x00);
    else
        return char(0x10);

}
int CAutoFPDlg::str_to_hex(char* string, unsigned char* cbuf, int len)
{
    BYTE high, low;
    int idx, ii = 0;
    for (idx = 0; idx < len; idx += 2)
    {
        high = string[idx];
        low = string[idx + 1];

        if (high >= '0' && high <= '9')
            high = high - '0';
        else if (high >= 'A' &&high <= 'F')
            high = high - 'A' + 10;
        else if (high >= 'a' && high <= 'f')
            high = high - 'a' + 10;
        else
            return -1;

        if (low >= '0' && low <= '9')
            low = low - '0';
        else if (low >= 'A' &&low <= 'F')
            low = low - 'A' + 10;
        else if (high >= 'a' && low <= 'f')
            low = low - 'a' + 10;
        else
            return -1;

        cbuf[ii++] = high << 4 | low;
    }
    return 0;
}
void CAutoFPDlg::hex_to_str(char* ptr, char* buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        sprintf(ptr, "%02x", buf[i]);
        ptr += 2;
    }
}

BOOL CAutoFPDlg::TrayComplete()
{

    SendCommandToTray("05303046464257304D30333232303131");//置位烧录完成信号
    //int  nResult = 0;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //m_TrayPort.read_scc(mess,lenBuff,lenMessage);
    Sleep(500);
    SendCommandToTray("05303046464257304D30333232303130");//复位烧录完成信号
    Sleep(50);
    SendCommandToTray("05303046464257304D30333231303130");//复位Tray盘到达信号
    Sleep(2000);//等待到达信号复位，必须有的延时
    //m_TrayPort.read_scc(mess,lenBuff,lenMessage);
    return TRUE;
}
//BOOL CAutoFPDlg::SendCommandHex(LPCSTR command)
//{
//	CString strCommand;
//	strCommand = command;
//	unsigned long len1;
//	char data[80];
//	int len = Str2Hex(strCommand, data);
//	BOOL bRet = m_port2.write_scc(data, len, len1);
//
//	Sleep(10);
//	return bRet;
//}
BOOL CAutoFPDlg::ReadTrayReadySignal()
{
    //SendCommandToTray("05303046464252304D303332313031");//读tray到达信号
    //Sleep(1);
    //CString readTag = "FF1";
    //CString failTag = "FF0";
    BOOL  nResult = FALSE;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //if (m_TrayPort.read_scc(mess, lenBuff, lenMessage) == TRUE)
    //{
    //	if (lenMessage > 0)
    //	{
    //		outPut = mess;
    //		//fl.Write(outPut);
    //		int nindex = outPut.Find(readTag);
    //		if (nindex >= 0)
    //		{
    //			nResult = TRUE;
    //		}
    //	}
    //}

    if (CheckTrayArrive())
        nResult = TRUE;
    return nResult;
}
BOOL CAutoFPDlg::CheckTapeIn()
{

    SendCommand("%01#RCSR0024**\r");
    Sleep(1);
    CString readTag = "%01$";
    CString failTag = "%01";
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    if (m_port.read_scc(mess, lenBuff, lenMessage) == TRUE)
    {
        if (lenMessage > 0)
        {
            outPut = mess;
            //fl.Write(outPut);
            int nindex = outPut.Find(readTag);
            if (nindex >= 0)
            {
                CString strTmp = outPut.GetAt(nindex + 6);//.Mid(readTag.GetLength()+2,4);
                nResult = atoi(strTmp.GetBuffer(0));
                strTmp.ReleaseBuffer();
            }
        }
    }
    return nResult;
}

BOOL CAutoFPDlg::CheckTapeOut()
{
    if (FEEDER == 1)
    {
        return !CheckFeederOutError();
    }
    else
    {
        Sleep(1);
        CString readTag = "%01$";
        CString failTag = "%01";
        BOOL  nResult = FALSE;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        SendCommand("%01#RCSR0025**\r");
        if (m_port.read_scc(mess, lenBuff, lenMessage) == TRUE)
        {
            if (lenMessage > 0)
            {
                outPut = mess;
                if (outPut.Find("%01$RC12") == 0)
                {
                    /*	CString strTmp = outPut.GetAt(6);//.Mid(readTag.GetLength()+2,4);
                    nResult = atoi(strTmp.GetBuffer(0));
                    strTmp.ReleaseBuffer();*/
                    nResult = TRUE;
                }
            }
        }
        if (nResult)
            SendCommand("%01#WCSR00250**\r");
        return nResult;
    }

}

BOOL CAutoFPDlg::StartPLC()
{
    if (gm_bAutoTray)
    {
        if (!gm_bTrayPLCStart)
        {
            StartTrayPLC();
            SendCommandToTray("05303046464257304D30333232303130");//复位烧录完成信号
            return TRUE;
        }
        else
            ContinueTrayPLC();
        return TRUE;
    }
    if (gm_uInfo.nWorkMode == TRAYTOTAPE || gm_uInfo.nWorkMode == TAPETOTAPE || gm_uInfo.nWorkMode == TAPETOTRAY)
    {
        if (FEEDER != 1)
        {
            SendCommand("%01#WCSR00211**\r");//21为停止位，常闭
            Sleep(10);
            SendCommand("%01#WCSR00201**\r");
            SendCommand("%01#WCSR00210**\r");//21为停止位，常闭
            //SendCommand("%01#WCSR00251**\r\n");
            //SetTapeInSpeed(gm_uInfo.fTapeInSpeed);
            //Sleep(10);
            SetTapeOutSpeed(gm_uInfo.fTapeOutSpeed);
            //Sleep(10);
            //SetTapeInStep(gm_uInfo.fTapeInStep);
            //Sleep(10);
            SetTapeOutStep(gm_uInfo.fTapeOutStep);
            //Sleep(10);
            SetCoverTime(gm_uInfo.nCoverTime);
            SendCommand("%01#WDD00500005000000**\r");//清空编带机状态
        }
    }
    return TRUE;
}
BOOL CAutoFPDlg::StopPLC()
{
    if (TRAY_TO_TRAY != gm_uInfo.nWorkMode)
    {
        if (FEEDER != 1)
        {
            SendCommand("%01#WCSR00200**\r");
            Sleep(10);
            SendCommand("%01#WCSR00211**\r");
            //Sleep(10);
            //SendCommand("%01#WCSR00210**\r");
            return TRUE;
        }
    }
    return FALSE;
}
BOOL CAutoFPDlg::ContinueTrayPLC()
{
    SendCommandToTray("05303046464257304D30303233303131");//将M23置位
    Sleep(10);
    SendCommandToTray("05303046464257304D30303233303130");//将M23复位
    return TRUE;
}
BOOL CAutoFPDlg::StartTrayPLC()
{
    SendCommandToTray("05303046464257304D30303139303131");//将M19置位
    Sleep(10);
    SendCommandToTray("05303046464257304D30303139303130");//将M19复位

    SendCommandToTray("05303046464257304D30303232303131");//将M22置位
    Sleep(10);
    SendCommandToTray("05303046464257304D30303232303130");//将M22复位
    gm_bTrayPLCStart = TRUE;
    return TRUE;
}

BOOL CAutoFPDlg::ContinueTestTray()
{
    SendCommandToTray("05303046464257304D30303234303131");//将M24置位
    //Sleep(100);
    //SendCommandToTray("05303046464257304D30303234303130");//将M24复位
    return TRUE;
}
BOOL CAutoFPDlg::StopTrayPLC()
{
    SendCommandToTray("05303046464257304D30303231303131");//将M21置位
    Sleep(10);
    SendCommandToTray("05303046464257304D30303231303130");//将M21复位
    return TRUE;
}
//出料料带检测，如果有料带，则返回1，否则返回0
int CAutoFPDlg::CheckOutTape()
{
    int  nResult = 0;
    if (FEEDER == 1)
    {
        //nResult = CheckOutTapeIO();	//无料带，返回1，	
    }
    else
    {
        CString readTag = "%01$";
        CString failTag = "%01";
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        //m_port.read_scc(mess, lenBuff, lenMessage);
        //SendCommand("%01#RCSR0025**\r");
        SendCommand("%01#RDD0050000500**\r");
        if (m_port.read_scc(mess, lenBuff, lenMessage) == TRUE)
        {
            if (lenMessage > 0)
            {
                outPut = mess;
                //MessageBox(outPut);
                //if (outPut.Find("%01$RC12") == 0)
                if (outPut.Find("%01$RD00") == 0)//返回0000表示无异常
                {
                    gm_bTapeStatus = 0;
                }
                else if (outPut.Find("%01$RD01") == 0)//返回0100表示载带用完
                {
                    gm_bTapeStatus = 1;
                }
                else if (outPut.Find("%01$RD02") == 0)//返回0200表示原点复位异常
                {
                    gm_bTapeStatus = 2;
                }
                else if (outPut.Find("%01$RD03") == 0)//返回0300表示料膜用完
                {
                    gm_bTapeStatus = 3;
                }
                else
                {
                    //MessageBox(outPut);
                    gm_bTapeStatus = -1;//通信错误
                }
            }
        }
        SetDlgItemText(IDC_STATIC_TAPE_STATUS, "Tape Status:"+outPut);
        //if (nResult)
        SendCommand("%01#WDD00500005000000**\r");
    }
    return 0;
}

//进料编带前进一格
BOOL CAutoFPDlg::TapeInOk()
{
    if (FEEDER == 1)
    {
        SetFeedIn();
    }
    else
    {
        //SendCommand("%01#WCSR00221**\r");
        ////	Sleep(5);
        ////	SendCommand("%01#WCSR00220**\r");
        SendCommand("%01#WCSR00501**\r");
        //MessageBox("tape in");
        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        //Sleep(100);
        //TapeInDisable();
    }

    return TRUE;
}

//出料编带前进一格
BOOL CAutoFPDlg::TapeOutOk()
{
    if (FEEDER == 1)
    {
        SetFeederOut();
    }
    else
    {
        SendCommand("%01#WCSR00221**\r");
        //	Sleep(5);
        //	SendCommand("%01#WCSR00230**\r");

        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        Sleep(10);
        TapeOutDisable();
    }
    return TRUE;
}
//出料编带后退一格
BOOL CAutoFPDlg::TapeOutBackOk()
{
    SendCommand("%01#WCSR00631**\r");

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(10);
    //SendCommand("%01#WCSR00630**\r");
    return TRUE;
}
//进料微调
BOOL CAutoFPDlg::TapeInTune()
{
    //SendCommand("%01#WCSR002A1**\r");
    //int  nResult = 0;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //m_port.read_scc(mess, lenBuff, lenMessage);
    //Sleep(10);
    //SendCommand("%01#WCSR002A0**\r");
    //m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

//出料微调
BOOL CAutoFPDlg::TapeOutTune()
{
    //SendCommand("%01#WCSR002B1**\r");
    SendCommand("%01#WCSR00621**\r");//新编带机的复位
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(10);
    SendCommand("%01#WCSR00620**\r");
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::TapeInDisable()
{
    //SendCommand("%01#WCSR00500**\r");
    //int  nResult = 0;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //if (m_port.read_scc(mess, lenBuff, lenMessage) == TRUE)
    //	return TRUE;
    //else
    return FALSE;
}

BOOL CAutoFPDlg::TapeOutDisable()
{
    SendCommand("%01#WCSR00220**\r");
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

//进料编带一直前进
BOOL CAutoFPDlg::TapeInGo()
{
    SendCommand("%01#WCSR00271**\r");

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(5);

    return TRUE;
}
//出料编带一直前进
BOOL CAutoFPDlg::TapeOutGo()
{
    SendCommand("%01#WCSR00291**\r");
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(50);
    return TRUE;
}

//进料编带停止前进
BOOL CAutoFPDlg::TapeInGoDisable()
{
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    SendCommand("%01#WCSR00270**\r");
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

//出料编带停止前进
BOOL CAutoFPDlg::TapeOutGoDisable()
{
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    SendCommand("%01#WCSR00290**\r");
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}
//TapeIn方向控制，bPos为true时正向
BOOL CAutoFPDlg::TapeInDir(BOOL bPos)
{
    if (bPos)
    {
        SendCommand("%01#WCSR002E1**\r");

        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        Sleep(5);
        SendCommand("%01#WCSR002E0**\r");
        m_port.read_scc(mess, lenBuff, lenMessage);
    }
    else
    {
        SendCommand("%01#WCSR002C1**\r");

        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        Sleep(5);
        SendCommand("%01#WCSR002C0**\r");
        m_port.read_scc(mess, lenBuff, lenMessage);
    }

    return TRUE;
}

//TapeOut方向控制，bPos为true时正向
BOOL CAutoFPDlg::TapeOutDir(BOOL bPos)
{
    if (bPos)
    {
        SendCommand("%01#WCSR002F1**\r");

        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        Sleep(5);
        SendCommand("%01#WCSR002F0**\r");
        m_port.read_scc(mess, lenBuff, lenMessage);
    }
    else
    {
        SendCommand("%01#WCSR002D1**\r");

        int  nResult = 0;
        char mess[256];
        unsigned int lenBuff = 256;
        unsigned long lenMessage;
        CString outPut;
        m_port.read_scc(mess, lenBuff, lenMessage);
        Sleep(5);
        SendCommand("%01#WCSR002D0**\r");
        m_port.read_scc(mess, lenBuff, lenMessage);
    }

    return TRUE;
}

//编带，步进补偿
BOOL CAutoFPDlg::TapeInInit()
{
    SendCommand("%01#WCSR00261**\r");

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(5);
    SendCommand("%01#WCSR00260**\r");
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::TapeOutInit()
{
    SendCommand("%01#WCSR00281**\r");

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    Sleep(5);
    SendCommand("%01#WCSR00280**\r");
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::SetTapeInSpeed(float speed)
{
    float fPulse = speed * 640 / 12;
    int nPulse = (int)fPulse;
    CString str;
    str.Format("%04X", nPulse);
    CString strNew;
    strNew = str.Right(2);
    strNew = strNew + str.Left(2);
    CString strCommand = "%01#WDD3245032451";
    strCommand += strNew;
    strCommand += "0000**\r";
    SendCommand(strCommand);
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::TapeInTuneStep(float step)
{
    //float fPulse = step * 640 / 12;
    //int nPulse = (int)fPulse;
    //CString str;
    //str.Format("%04X", nPulse);
    //CString strNew;
    //strNew = str.Right(2);
    //strNew = strNew + str.Left(2);
    //CString strCommand = "%01#WDD3252032521";
    //strCommand += strNew;
    //strCommand += "0000**\r";
    //SendCommand(strCommand);
    //int  nResult = 0;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::TapeOutTuneStep(float step)
{
    //float fPulse = step * 640 / 12;
    //int nPulse = (int)fPulse;
    //CString str;
    //str.Format("%04X", nPulse);
    //CString strNew;
    //strNew = str.Right(2);
    //strNew = strNew + str.Left(2);
    //CString strCommand = "%01#WDD3253032531";
    //strCommand += strNew;
    //strCommand += "0000**\r";
    //SendCommand(strCommand);
    //int  nResult = 0;
    //char mess[256];
    //unsigned int lenBuff = 256;
    //unsigned long lenMessage;
    //CString outPut;
    //m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::SetTapeInStep(float step)
{
    float fPulse = step * 640 / 12;
    int nPulse = (int)step;
    CString str;
    str.Format("%04X", nPulse);
    CString strNew;
    strNew = str.Right(2);
    strNew = strNew + str.Left(2);
    CString strCommand = "%01#WDD3246032461";
    strCommand += strNew;
    strCommand += "0000**\r";
    SendCommand(strCommand);
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

//设置出料速度
BOOL CAutoFPDlg::SetTapeOutSpeed(float speed)
{
    float fPulse = speed * 640 / 12;
    int nPulse = (int)speed;
    CString str;
    str.Format("%04X", nPulse);

    //%01# WD D32470 32470 0040 **\r
    //出料速度设置，写到后个字节，0040表示64mm/s,**表示忽略校验位
    CString strCommand = "%01#WDD3247032470";
    strCommand += str;
    strCommand += "**\r";
    SendCommand(strCommand);

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}
//设置出料步长
BOOL CAutoFPDlg::SetTapeOutStep(float step)
{
    //float fPulse = step * 640 / 12;
    int nPulse = (int)step;
    CString str;
    str.Format("%04X", nPulse);
    CString strNew;
    strNew = str.Right(2);
    strNew = strNew + str.Left(2);//数据位在高位，所以需要调换高低位
    //CString strCommand = "%01#WDD3248032481";
    //%01# WD D06112 06112 0800 **\r
    CString strCommand = "%01#WDD3260032600";
    strCommand += strNew;
    strCommand += "**\r";
    SendCommand(strCommand);

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

BOOL CAutoFPDlg::SetCoverTime(int nTime)
{
    CString str;
    str.Format("%04X", nTime);
    CString strNew;
    strNew = str.Right(2);
    strNew = strNew + str.Left(2);
    CString strCommand = "%01#WDD3251032511";
    strCommand += strNew;
    strCommand += "0000**\r";
    SendCommand(strCommand);

    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    m_port.read_scc(mess, lenBuff, lenMessage);
    return TRUE;
}

void CAutoFPDlg::ShowTapeInState(int nState)
{
    SetDlgItemInt(IDC_EDIT2, nState);
}

UINT CheckThreadProc(LPVOID lPara)
{
    CAutoFPDlg *pDlg = (CAutoFPDlg*)lPara;
    DWORD dwStart = GetTickCount();
    pDlg->TapeInOk();
    Sleep(1);
    BOOL bOK = pDlg->CheckTapeIn();
    pDlg->ShowTapeInState(bOK);
    while (!bOK)
    {
        bOK = pDlg->CheckTapeIn();
        //	Sleep(2);
    }
    int nTime = GetTickCount() - dwStart;
    pDlg->ShowTapeInState(bOK);
    pDlg->TapeInDisable();
    CString str;
    str.Format("%d", nTime);
    AfxMessageBox(str);
    return 1;
}



void CAutoFPDlg::OnCheckAllselect()
{
    // TODO: Add your control notification handler code here
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_ALLSELECT);
    if (pBtn->GetCheck())
    {
        for (int i = 0; i < 40; i++)
        {
            gm_bSocketUsed[i] = TRUE;
        }
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK1);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK2);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK3);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK4);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK5);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK6);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK7);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK8);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK9);
        pBtn->SetCheck(1);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK10);
        pBtn->SetCheck(1);

    }
    else
    {
        for (int i = 0; i < 40; i++)
        {
            gm_bSocketUsed[i] = FALSE;
        }
        CButton *pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK1);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK2);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK3);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK4);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK5);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK6);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK7);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK8);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK9);
        pBtn->SetCheck(0);

        pBtn = (CButton*)GetDlgItem(IDC_SOCKET_CHECK10);
        pBtn->SetCheck(0);

    }
}

//----------------------------------------------------------------------------------
/**
\brief  打开设备

\return 打开设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CAutoFPDlg::OpenDevice()
{
    GX_STATUS      emStatus = GX_STATUS_SUCCESS;
    GX_OPEN_PARAM  stOpenParam;

    // 初始化设备打开参数,默认打开序号为1的设备
    stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
    stOpenParam.openMode = GX_OPEN_INDEX;
    stOpenParam.pszContent = "1";

    // 打开枚举列表中的第一台设备
    emStatus = GXOpenDevice(&stOpenParam, &m_hDevice);
    return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  关闭设备

\return 关闭设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CAutoFPDlg::CloseDevice()
{
    GX_STATUS emStatus = GX_STATUS_ERROR;

    // 释放资源
    UnPrepareForShowImg();

    // 关闭设备
    emStatus = GXCloseDevice(m_hDevice);

    return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  为保存图像分配Buffer,为图像显示准备资源

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CAutoFPDlg::PrepareForShowImg()
{
    bool bRet = false;

    //为GetImage参数分配空间
    m_stFrameData.pImgBuf = new BYTE[(size_t)m_nPayLoadSize];
    if (m_stFrameData.pImgBuf == NULL)
    {
        bRet = false;
    }

    if (m_bIsColorFilter)
    {
        // 为彩色图像显示准备资源,分配Buffer
        bRet = PrepareForShowColorImg();
    }
    else
    {
        // 为黑白图像显示准备资源,分配Buffer
        bRet = PrepareForShowMonoImg();
    }

    // 若分配失败则释放已分配的资源
    if (!bRet)
    {
        UnPrepareForShowImg();
    }

    return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  为彩色图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CAutoFPDlg::PrepareForShowColorImg()
{
    //--------------------------------------------------------------------
    //---------------------------初始化bitmap头---------------------------
    m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
    m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
    m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

    m_pBmpInfo->bmiHeader.biPlanes = 1;
    m_pBmpInfo->bmiHeader.biBitCount = 24; // 彩色图像为24,黑白图像为8
    m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
    m_pBmpInfo->bmiHeader.biSizeImage = 0;
    m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biClrUsed = 0;
    m_pBmpInfo->bmiHeader.biClrImportant = 0;


    //---------------------------------------------------------------------
    //----------------------------为图像数据分配Buffer---------------------

    // 为经过RGB转换后的图像分配Buffer
    m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight * 3)];
    if (m_pImgBuffer == NULL)
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------
/**
\brief 为黑白图像显示准备资源,分配Buffer

\return true:为图像显示准备资源成功  false:准备资源失败
*/
//----------------------------------------------------------------------------------
bool  CAutoFPDlg::PrepareForShowMonoImg()
{
    //--------------------------------------------------------------------
    //---------------------------初始化bitmap头---------------------------
    m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
    m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth;
    m_pBmpInfo->bmiHeader.biHeight = (LONG)m_nImageHeight;

    m_pBmpInfo->bmiHeader.biPlanes = 1;
    m_pBmpInfo->bmiHeader.biBitCount = 8;  // 彩色图像为24,黑白图像为8
    m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
    m_pBmpInfo->bmiHeader.biSizeImage = 0;
    m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBmpInfo->bmiHeader.biClrUsed = 0;
    m_pBmpInfo->bmiHeader.biClrImportant = 0;

    // 黑白相机需要进行初始化调色板操作
    for (int i = 0; i < 256; i++)
    {
        m_pBmpInfo->bmiColors[i].rgbBlue = i;
        m_pBmpInfo->bmiColors[i].rgbGreen = i;
        m_pBmpInfo->bmiColors[i].rgbRed = i;
        m_pBmpInfo->bmiColors[i].rgbReserved = i;
    }

    //---------------------------------------------------------------------
    //----------------------------为图像数据分配Buffer---------------------
    // 黑白图像Buffer分配
    m_pImgBuffer = new BYTE[(size_t)(m_nImageWidth * m_nImageHeight)];
    if (m_pImgBuffer == NULL)
    {
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------
/**
\brief  释放为图像显示准备资源

\return 无
*/
//----------------------------------------------------------------------------------
void CAutoFPDlg::UnPrepareForShowImg()
{
    if (m_stFrameData.pImgBuf != NULL)
    {
        delete[]m_stFrameData.pImgBuf;
        m_stFrameData.pImgBuf = NULL;
    }

    if (m_pImgBuffer != NULL)
    {
        delete[]m_pImgBuffer;
        m_pImgBuffer = NULL;
    }
}

// ---------------------------------------------------------------------------------
/**
\brief   相机初始化

\return  无
*/
// ----------------------------------------------------------------------------------
GX_STATUS CAutoFPDlg::InitDevice()
{
    // 	, m_bIsColorFilter(false)
    // 		, m_bIsOpen(false)
    // 		, m_bIsSnap(false)
    // 		, m_nPayLoadSize(0)
    // 		, m_nPixelColorFilter(GX_COLOR_FILTER_NONE)
    // 		, m_nImageWidth(0)
    // 		, m_nImageHeight(0)
    // 		, m_pBmpInfo(NULL)
    // 		, m_hDevice(NULL)
    // 		, m_pImgBuffer(NULL)
    // 		, m_pWnd(NULL)
    // 	, m_hDC(NULL)

    GX_STATUS emStatus = GX_STATUS_SUCCESS;




    //设置采集模式连续采集
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
    VERIFY_STATUS_RET(emStatus);

    //设置触发模式为开
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
    VERIFY_STATUS_RET(emStatus);

    // 已知当前相机支持哪个8位图像数据格式可以直接设置
    // 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
    // emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
    // VERIFY_STATUS_RET(emStatus);

    // 不清楚当前相机的数据格式时，可以调用SetPixelFormat8bit函数将图像数据格式设置为8Bit
    emStatus = SetPixelFormat8bit();
    VERIFY_STATUS_RET(emStatus);

    //选择触发源为软触发
    emStatus = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, GX_TRIGGER_SOURCE_SOFTWARE);

    return emStatus;
}

// ---------------------------------------------------------------------------------
/**
\brief   设置相机的数据格式为8bit

\return  emStatus GX_STATUS_SUCCESS:设置成功，其它:设置失败
*/
// ----------------------------------------------------------------------------------
GX_STATUS CAutoFPDlg::SetPixelFormat8bit()
{
    GX_STATUS emStatus = GX_STATUS_SUCCESS;
    int64_t   nPixelSize = 0;
    uint32_t  nEnmuEntry = 0;
    size_t    nBufferSize = 0;
    BOOL      bIs8bit = TRUE;

    GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
    GX_ENUM_DESCRIPTION  *pEnumTemp = NULL;

    // 获取像素点大小
    emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
    VERIFY_STATUS_RET(emStatus);

    // 判断为8bit时直接返回,否则设置为8bit
    if (nPixelSize == GX_PIXEL_SIZE_BPP8)
    {
        return GX_STATUS_SUCCESS;
    }
    else
    {
        // 获取设备支持的像素格式枚举数
        emStatus = GXGetEnumEntryNums(m_hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
        VERIFY_STATUS_RET(emStatus);

        // 为获取设备支持的像素格式枚举值准备资源
        nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
        pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

        // 获取支持的枚举值
        emStatus = GXGetEnumDescription(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
        if (emStatus != GX_STATUS_SUCCESS)
        {
            if (pEnumDescription != NULL)
            {
                delete[]pEnumDescription;
                pEnumDescription = NULL;
            }

            return emStatus;
        }

        // 遍历设备支持的像素格式,设置像素格式为8bit,
        // 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
        for (uint32_t i = 0; i < nEnmuEntry; i++)
        {
            if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
            {
                emStatus = GXSetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
                break;
            }
        }

        // 释放资源
        if (pEnumDescription != NULL)
        {
            delete[]pEnumDescription;
            pEnumDescription = NULL;
        }
    }

    return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  将获取到的图像数据依据黑白彩色的不同进行处理并显示到界面
\param  pFrameData  [in] 指向用户传入的图像数据的地址指针

\return 无
*/
//----------------------------------------------------------------------------------
void  CAutoFPDlg::ShowImage(GX_FRAME_DATA* pFrameData)
{
    // 获取到的图像是否为完整的图像
    if (pFrameData->nStatus != 0)
    {
        return;
    }

    //若支持彩色,转换为RGB图像后输出
    if (m_bIsColorFilter)
    {
        //将Raw8图像转换为RGB图像以供显示
        //		DxRaw8toRGB24((BYTE*)pFrameData->pImgBuf, m_pImgBuffer, (VxUint32)m_nImageWidth, (VxUint32)m_nImageHeight, RAW2RGB_NEIGHBOUR,
        //			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);
    }
    else
    {
        // 黑白相机需要翻转数据后显示
        for (int i = 0; i < m_nImageHeight; i++)
        {
            memcpy(m_pImgBuffer + i*m_nImageWidth, (BYTE *)(pFrameData->pImgBuf) + (m_nImageHeight - i - 1)*m_nImageWidth, (size_t)m_nImageWidth);
        }
    }

    //在指定窗口上一帧一帧的画图
    DrawImg(m_pImgBuffer, m_nImageWidth, m_nImageHeight);
}

//----------------------------------------------------------------------------------
/**
\brief  在图像显示窗口画图
\param  pImageBuf    [in] 指向图像缓冲区的指针
\param  nImageWidth  [in] 图像宽
\param  nImageHeight [in] 图像高

\return 无
*/
//----------------------------------------------------------------------------------
void CAutoFPDlg::DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight)
{
    int nWndWidth = 0;
    int nWndHeight = 0;

    // 为画图做准备
    RECT objRect;
    m_pWnd->GetClientRect(&objRect);
    nWndWidth = objRect.right - objRect.left;
    nWndHeight = objRect.bottom - objRect.top;

    // 必须调用该语句，否则图像出现水纹
    ::SetStretchBltMode(m_hDC, COLORONCOLOR);
    ::StretchDIBits(m_hDC,
        0,
        0,
        nWndWidth,
        nWndHeight,
        0,
        0,
        (int)nImageWidth,
        (int)nImageHeight,
        pImageBuf,
        m_pBmpInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}



//----------------------------------------------------------------------------------
/**
\brief  获取设备的宽高等信息

\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CAutoFPDlg::GetDeviceInitParam()
{
    GX_STATUS emStatus = GX_STATUS_ERROR;
    bool      bIsImplemented = false;

    // 查询当前相机是否支持GX_ENUM_PIXEL_COLOR_FILTER
    emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &bIsImplemented);
    VERIFY_STATUS_RET(emStatus);
    m_bIsColorFilter = bIsImplemented;

    // 支持表示输出彩色图像
    if (bIsImplemented)
    {
        emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
        VERIFY_STATUS_RET(emStatus);
    }

    // 获取宽度
    emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
    VERIFY_STATUS_RET(emStatus);

    // 获取高度
    emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
    VERIFY_STATUS_RET(emStatus);

    // 获取图像大小
    emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);

    return emStatus;
}


void CAutoFPDlg::ShowErrorString(GX_STATUS emErrorStatus)
{
    char*     pchErrorInfo = NULL;
    size_t    nSize = 0;
    GX_STATUS emStatus = GX_STATUS_ERROR;

    // 获取错误信息长度，并申请内存空间
    emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
    pchErrorInfo = new char[nSize];
    if (NULL == pchErrorInfo)
    {
        return;
    }

    // 获取错误信息，并显示
    emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
    if (emStatus != GX_STATUS_SUCCESS)
    {
        MessageBox("GXGetLastError接口调用失败！");
    }
    else
    {
        MessageBox((LPCTSTR)pchErrorInfo);
    }

    // 释放申请的内存空间
    if (NULL != pchErrorInfo)
    {
        delete[] pchErrorInfo;
        pchErrorInfo = NULL;
    }
}


Point CAutoFPDlg::ComputeTest()
{

    Point pt;
    INT* Xoffset;
    INT* Yoffset;
    BYTE *pRawBuffer;
    pRawBuffer = m_pImgBuffer;
    int i = 0;
    int j = 0;
    int s = 0;
    int t = 0;
    INT64 tValue = 0;
    for (i = 0; i < 1024; i++)
        for (j = 0; j < 1280; j++)
        {
            tValue += pRawBuffer[i * 1280 + j];
        }
    int mean = tValue / (1280 * 1024);
    for (i = 0; i < 1024; i++)
        for (j = 0; j < 1280; j++)
        {
            if (pRawBuffer[i * 1280 + j] < mean)
                pRawBuffer[i * 1280 + j] = 0;
            else
                pRawBuffer[i * 1280 + j] = 255;
        }

    DrawImg(pRawBuffer, m_nImageWidth, m_nImageHeight);
    int nLenX = 0;
    bool bFindX = false;
    int rx = 0;
    int nLenY = 0;
    bool bFindY = false;
    int ry = 0;
    for (i = 300; i < 700; i++)
    {
        for (j = 400; j < 800; j++)
        {
            if (pRawBuffer[i * 1280 + j] == 255)
            {
                nLenX = 1;
                int xp = 0;
                BOOL bContinue = TRUE;
                int k = 0;
                for (k = (j + 1); k < 880; k++)
                {
                    if (pRawBuffer[i * 1280 + k] == 255)
                    {
                        nLenX++;
                        bContinue = TRUE;

                    }
                    else
                    {
                        bContinue = FALSE;
                        xp = k;
                        break;
                    }
                }
                if (nLenX >= 130) // 8 will be changed
                {
                    //Xoffset[s]=xp;
                    bFindX = true;
                    rx = xp;
                    break;
                }
            }
            if (pRawBuffer[i + j * 1280] == 255)
            {
                nLenY = 1;
                int xp = 0;
                BOOL bContinue = TRUE;
                int k = 0;
                for (k = (j + 1); k < 880; k++)
                {
                    if (pRawBuffer[i + k * 1280] == 255)
                    {
                        nLenY++;
                        bContinue = TRUE;

                    }
                    else
                    {
                        bContinue = FALSE;
                        xp = k;
                        break;
                    }
                }
                if (nLenY >= 130) // 8 will be changed
                {
                    bFindY = true;
                    ry = xp;
                    break;
                }
            }
        }
        if (bFindX&&bFindY)
            break;
    }
    pt.x = rx - nLenX / 2;//Dot center.
    pt.y = ry - nLenY / 2;//Dot center.
    gm_nOutOrigePos.x = pt.x;
    gm_nOutOrigePos.y = pt.y;
    // 	OpenSnapEx();
    return pt;
}
BOOL CAutoFPDlg::ComputeOffset()
{
    Point ptOffset;
    ptOffset = ComputeTest();
    gm_nTapeOutOffset.x = ptOffset.x - gm_nOutOrigePos.x;
    gm_nTapeOutOffset.y = ptOffset.y - gm_nOutOrigePos.y;
    if (abs(gm_nTapeOutOffset.x) >= 50 || abs(gm_nTapeOutOffset.y) >= 50)
    {
        AfxMessageBox("编带出料偏差过大，请调整编带坐标。");
        return FALSE;
    }
    return TRUE;

}





void CAutoFPDlg::OnStnDblclickSocketGood1()
{
    // TODO:  在此添加控件通知处理程序代码
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(1);
}


void CAutoFPDlg::OnStnDblclickSocketGood2()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(2);
}


void CAutoFPDlg::OnStnDblclickSocketGood3()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(3);
}


void CAutoFPDlg::OnStnDblclickSocketGood4()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(4);
}


void CAutoFPDlg::OnStnDblclickSocketGood50()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(5);
}


void CAutoFPDlg::OnStnDblclickSocketGood60()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(6);
}


void CAutoFPDlg::OnStnDblclickSocketGood70()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(7);
}


void CAutoFPDlg::OnStnDblclickSocketGood80()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(8);
}

void CAutoFPDlg::OnStnDblclickSocketGood91()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(9);
}


void CAutoFPDlg::OnStnDblclickSocketGood101()
{
    if (IDOK == AfxMessageBox("是否要手动烧录？", MB_OKCANCEL))
        StartOneProgrammer(10);
}



void CAutoFPDlg::OpenFramegrabUp()
{
    //Image Acquisition 01: Code generated by Image Acquisition 01
    try
    {
        if (!m_bCameraOpenUp)
        {
            m_strCameraNum = gm_uInfo.strCameraNum;
            m_strCameraVersion = gm_uInfo.strCameraVersion;//MER-130-30UM-L
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
            //m_Window.SetPart(0, 0, m_hv_Width, m_hv_Height);
        }

    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        MessageBox("OpenFramegrabUp Camera Error.");
    }
}
void CAutoFPDlg::OpenFramegrabDown()
{
    //Image Acquisition 01: Code generated by Image Acquisition 01
    try
    {
        if (!m_bCameraOpenDown)
        {
            m_strCameraNum = gm_uInfo.strDownCameraNum;
            m_strCameraVersion = gm_uInfo.strDownCameraVersion;//MER-130-30UM-L
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
                &m_hv_AcqHandle_Down);

            m_bCameraOpenDown = TRUE;

            SetFramegrabberParam(m_hv_AcqHandle_Down, "ExposureTime", EXPOSITION);
            //m_Window.SetPart(0, 0, m_hv_Width, m_hv_Height);
        }
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        MessageBox("OpenFramegrabDown Camera Error.");
    }
}

void CAutoFPDlg::OpenTapeCamera()
{
    try
    {
        if (!m_bCameraOpenTape)
        {
            OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
                "default", "HD USB Camera", 0, -1, &m_hv_AcqHandle_Tape);
            m_bCameraOpenTape = TRUE;
            
        }
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        m_bCameraOpenTape = FALSE;
        MessageBox("Open Tape Camera Error!");
    }
}

BOOL CAutoFPDlg::CreateTapeModel()
{
    OpenTapeCamera();
    if (!m_bCameraOpenTape)
        return FALSE;
    try
    {
        HObject ho_ROI_Model, ho_ImageReduced;
        GrabImage(&m_ho_Image, m_hv_AcqHandle_Tape);
        WaitSeconds(1);
        GrabImage(&m_ho_Image, m_hv_AcqHandle_Tape);
        GetImageSize(m_ho_Image, &m_hv_Width, &m_hv_Height);
        GenRectangle2(&ho_ROI_Model, 
            m_hv_Height / 2,
            m_hv_Width / 2, 
            0, 
            m_hv_Width / 3, 
            m_hv_Height / 3);
        ReduceDomain(m_ho_Image, ho_ROI_Model, &ho_ImageReduced);
        //CreateShapeModel(ho_ImageReduced,
        //    "auto",						//NumLevels
        //    HTuple(-30).TupleRad(),							//AngleStart
        //    HTuple(60).TupleRad(),		//AngleExtent
        //    0.0175,						//AngleStep
        //    "auto",						//Optimization
        //    "ignore_global_polarity",	//Metric
        //    "auto",						//Contrast
        //    "auto",						//MinContrast
        //    &m_hv_ModelID_Tape);
        CreateShapeModel(ho_ImageReduced, "auto", -0.39, 0.79, "auto", "auto", "use_polarity",
            "auto", "auto", &m_hv_ModelID_Tape);
        FindShapeModel(m_ho_Image, m_hv_ModelID_Tape, -0.39, 0.78, 0.5, 1, 0.5, "least_squares",
            0, 0.9, &m_hv_Row, &m_hv_Column, &m_hv_Angle, &m_hv_Score);
        if (0 != (HTuple((((m_hv_Height / 2 - m_hv_Row).TupleAbs()) + ((m_hv_Width / 2 - m_hv_Column).TupleAbs()))<100).TupleAnd(m_hv_Score>0.5)))
        {
            WriteShapeModel(m_hv_ModelID_Tape, "c:/S100/Model/TapeOutCameraModel.shm");
            return TRUE;
        }
        return FALSE;
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        MessageBox("Create Tape Model Error!");
        return FALSE;
    }
}

BOOL CAutoFPDlg::CheckTapeModel()
{
    OpenTapeCamera();
    if (!m_bCameraOpenTape)
        return FALSE;
    try
    {
        HObject ho_ImageSearch;
        HTuple hv_RowSearch, hv_ColumnSearch, hv_AngleSearch, hv_ScoreSearch, hv_ModelID;
        ReadShapeModel("c:/S100/Model/TapeOutCameraModel.shm", &hv_ModelID);
        GrabImage(&ho_ImageSearch, m_hv_AcqHandle_Tape);
        GrabImage(&ho_ImageSearch, m_hv_AcqHandle_Tape);
        FindShapeModel(ho_ImageSearch, hv_ModelID, -0.39, 0.78, 0.5, 1, 0.5, "least_squares",
            0, 0.9, &hv_RowSearch, &hv_ColumnSearch, &hv_AngleSearch, &hv_ScoreSearch);
        if (0 != (HTuple((((hv_RowSearch - m_hv_Row).TupleAbs()) + ((hv_ColumnSearch - m_hv_Column).TupleAbs()))<100).TupleAnd(hv_ScoreSearch>0.5)))
        {
            return TRUE;
        }
        return FALSE;
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        return FALSE;
    }
}

BOOL CAutoFPDlg::CheckModel(Position2 &posOffset, LPCTSTR place)
{

    CString strPlace = place;
    posOffset.x = 0;
    posOffset.y = 0;
    posOffset.phi = 0;
    double length = 0;
    double width = 0;
    CRect rt;
    GetDlgItem(IDC_CAMERA_FRAME)->GetWindowRect(&rt);
    ScreenToClient(rt);

    try
    {
        GenRectangle1(&m_ho_ROI_Search_Domain, 512 - 100, 640 - 100, 512 + 100, 640 + 100);

        if (strPlace.Compare("down_camera_in") == 0)
        {
            HTuple filename = /*"c:/S100/Model/" + */gm_strChipModel;
            //ReadShapeModel("c:/S100/Model/ChipModel.shm", &m_hv_ModelID);//下相机检测芯片角度位置偏差
            ReadShapeModel(filename, &m_hv_ModelID);//下相机检测芯片角度位置偏差
            length = gm_uInfo.nPixelsPM_down*gm_uInfo.nChipLength;
            width = gm_uInfo.nPixelsPM_down*gm_uInfo.nChipWidth;
            GrabImage(&m_ho_Image, m_hv_AcqHandle_Down);
        }
        else if (strPlace.Compare("tape_in") == 0)
        {
            ReadShapeModel("c:/S100/Model/TapeInModel.shm", &m_hv_ModelID);//检测编带进料
            length = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipLength;
            width = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipWidth;
            GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);
        }
        else if (strPlace.Compare("tape_out") == 0)
        {
            ReadShapeModel("c:/S100/Model/TapeOutModel.shm", &m_hv_ModelID);//检测编带出料
            length = gm_uInfo.nPixelsPM_up*gm_uInfo.nTapeLength;
            width = gm_uInfo.nPixelsPM_up*gm_uInfo.nTapeWidth;
            GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);
        }
        else if (strPlace.Compare("socket") == 0)
        {
            HTuple filename = /*"c:/S100/Model/" + */gm_strSocketModel;
            ReadShapeModel(filename, &m_hv_ModelID);//烧录座中检测是否有芯片，所以模型可以使用TapeIn的模型
            //ReadShapeModel("c:/S100/Model/SocketModel.shm", &m_hv_ModelID);//检测烧录座位置
            length = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipLength * 20;
            width = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipLength * 20;
            GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);
        }
        else if (strPlace.Compare("socket_have_chip") == 0)
        {
            HTuple filename = /*"c:/S100/Model/" +*/ gm_strSocketModel;
            ReadShapeModel(filename, &m_hv_ModelID);//烧录座中检测是否有芯片，所以模型可以使用TapeIn的模型
            //ReadShapeModel("c:/S100/Model/TapeInModel.shm", &m_hv_ModelID);//烧录座中检测是否有芯片，所以模型可以使用TapeIn的模型
            length = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipLength;
            width = gm_uInfo.nPixelsPM_up*gm_uInfo.nChipWidth;
            GrabImage(&m_ho_Image, m_hv_AcqHandle_Up);
        }

        GetImageSize(m_ho_Image, &m_hv_Width, &m_hv_Height);


        m_Window.OpenWindow(rt.top,
            rt.left,
            rt.Width(),
            rt.Height(),
            (Hlong)m_hWnd,
            "visible",
            "");

        //m_Window.SetPart(0, 0, m_hv_Width, m_hv_Height);
        m_Window.SetLineWidth(1);
        //MeanImage(m_ho_Image, &m_ho_ImageMean, 9, 9);
        //BinThreshold(m_ho_ImageMean, &m_ho_Region);
        //Connection(m_ho_Region, &m_ho_ConnectionRegions);
        //Union1(m_ho_ConnectionRegions, &m_ho_RegionUnion);
        //DilationCircle(m_ho_RegionUnion, &m_ho_RegionDilation, 0.5);

        ReduceDomain(m_ho_Image, m_ho_ROI_Search_Domain, &m_ho_ImageReducedSearch);
        HTuple htTop, htBottom, htRight, htLeft;
        htTop = 512 - width;
        htBottom = 512 + width;
        htLeft = 640 - length;
        htRight = 640 + length;

        CropDomainRel(m_ho_Image, &m_ho_ImagePart, htTop, htLeft, htBottom, htRight);//截取范围图片

        m_Window.DispObj(m_ho_Image);

        FindShapeModel(m_ho_Image,
            m_hv_ModelID,				//ModelID
            HTuple(-45).TupleRad(),		//AngleStart
            HTuple(90).TupleRad(),		//AngleExtent  角度范围
            0.1,						//MinScore
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
        HTuple hv_AngleOffset, hv_ColumnOffset, hv_RowOffset;

        hv_AngleOffset = m_hv_CheckAngle - 0;// gm_hv_Angle_Chip;
        hv_ColumnOffset = m_hv_CheckColumn - 640;// gm_hv_Column_Chip;
        hv_RowOffset = m_hv_CheckRow - 512;// gm_hv_Row_Chip;

        double a = hv_AngleOffset;
        //if (abs(a) > 3.14)
        //{
        //	a = a - 2 * 3.1415;
        //}
        double x = hv_ColumnOffset;
        double y = hv_RowOffset;
        double s = m_hv_CheckScore;
        m_Window.SetColor("red");
        CString str;
        m_Window.SetTposition(m_hv_Height - 400, 50);
        str.Format("匹配度:%0.2f%%", s * 100);
        m_Window.WriteString(str);

        m_Window.SetTposition(m_hv_Height - 300, 50);
        str.Format("Offset Of Angle:%0.2f", a);
        m_Window.WriteString(str);

        m_Window.SetTposition(m_hv_Height - 200, 50);
        str.Format("Offset Of X:%0.2f", x);
        m_Window.WriteString(str);

        m_Window.SetTposition(m_hv_Height - 100, 50);
        str.Format("Offset Of Y:%0.2f", y);
        m_Window.WriteString(str);

        m_Window.SetColor("green");
        m_Window.SetLineWidth(1);
        m_Window.DispCross(m_hv_CheckRow, m_hv_CheckColumn, 1000, a);

        double row = m_hv_CheckRow;
        double column = m_hv_CheckColumn;
        m_Window.SetColor("blue");
        m_Window.SetDraw("margin");
        m_Window.DispRectangle2(row, column, a, length / 2, width / 2);
        //m_Window.DumpWindow("bmp", "/dump_window/halcon_dump00");
        float score = 0.0;
        if (strPlace.Compare("tape_out") == 0)
        {
            score = gm_uInfo.fCheckScoreDown;
        }
        else
        {
            score = gm_uInfo.fCheckScore;
        }
        if (s < score)
        {
            return FALSE;
        }

        posOffset.x = x;
        posOffset.y = y;
        posOffset.phi = a;

        //CRect rt;
        //CWnd *pWnd = GetDlgItem(IDC_CAMERA_FRAME);
        //pWnd->GetWindowRect(rt);
        //ScreenToClient(rt);
        //InvalidateRect(&rt, TRUE);

        return TRUE;
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        //CString str;
        //str.Format("故障代码：s%", HDevExpDefaultException.);
        //MessageBox(str);
        return FALSE;
    }
}
void CAutoFPDlg::OnBnClickedBtnExit()
{
    if (IDOK != AfxMessageBox("确定要退出程序吗？", MB_OKCANCEL))
    {
        return;
    }
    KillTimer(TIMER_SHOW_POS);//刷新三轴的坐标值
    KillTimer(TIMER_REFRESH_SOCKET_STATUS);//刷新烧录状态
    //KillTimer(3);
    try
    {
        if (m_bCameraOpenUp)
        {
            CloseFramegrabber(m_hv_AcqHandle_Up);
            m_bCameraOpenUp = FALSE;
        }
        if (m_bCameraOpenDown)
        {
            CloseFramegrabber(m_hv_AcqHandle_Down);
            m_bCameraOpenDown = FALSE;
        }
    }
    catch (HalconCpp::HException &HDevExpDefaultException)
    {
        //MessageBox("没有需要关闭的相机");
    }

    for (size_t i = 0; i < MAXNUMOFPROGRAMMER; i++)
    {
        if (CheckModelCylinderPressed(i))
            SetCylinderUp(i);
    }

    CLogFile logFile;
    CString strLogFile = "c:\\S100\\backup\\Log\\";
    CTime time;
    time = CTime::GetCurrentTime();
    CString str;
    str.Format("%s-%d%02d%02d-%02d%02d.txt", gm_sdInfo.strWorkNo, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
    strLogFile = strLogFile + str;//日志完整路径
    logFile.SetPath(strLogFile);

    logFile.WriteSocketInfo();

    SaveConfig();

    gm_logFile.WriteEnd();
    CloseMotionCard();
    WSACleanup();

    if (NULL != gm_pTray)
    {
        delete[] gm_pTray;
        gm_pTray = NULL;
    }
    CDialog::OnCancel();
}

void CAutoFPDlg::SaveConfig()
{
    //保存位置文件路径到配置文件中
    CString str;
    CIniFile inf;
    inf.SetIniFileName(gm_strConfigFile);
    //保存坐标文件路径
    inf.WriteString("File", "Last", gm_strPositionFile);
    //保存烧录座选中状态
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER * MAXNUM_OF_SOCKET; i++)
    {
        int emptyEvalue = gm_sBurningStatus[i / MAXNUM_OF_SOCKET].bEmpty[i % MAXNUM_OF_SOCKET];
        int fpStatus = gm_sBurningStatus[i / MAXNUM_OF_SOCKET].nAdpStatus[i % MAXNUM_OF_SOCKET];
        str.Format("Socket%d", i);
        inf.WriteIntNumber("LastUsed", str, gm_bSocketUsed[i]);
        inf.WriteIntNumber("PassNumPerSocket", str, gm_nDisplayGoodForSocket[i]);
        inf.WriteIntNumber("FailNumPerSocket", str, gm_nDisplayFailForSocket[i]);
        inf.WriteIntNumber("Empty", str, emptyEvalue);
        inf.WriteIntNumber("Status", str, fpStatus);
    }

    //保存吸嘴选中状态
    for (int i = 0; i < 4; i++)
    {
        str.Format("Nozzle%d", i);
        inf.WriteIntNumber("Used", str, gm_bNozzleUsed[i]);
    }

    inf.WriteIntNumber("ConvertChip", "ConvertChip", gm_bConvertChip);
    //保存tray盘显示状态
    for (int i = 0; i < gm_dataTray.xn*gm_dataTray.yn; i++)
    {
        str.Format("tray%d", i);
        inf.WriteIntNumber("TrayStatus", str, gm_pTray[i]);
    }
    //保存所有全局变量
    inf.WriteIntNumber("GlobalParam", "gm_nTotalInSize", gm_nTotalInSize);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalOutSize", gm_nTotalOutSize);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentInSize", gm_nCurrentInSize);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentOutSize", gm_nCurrentOutSize);
    inf.WriteIntNumber("GlobalParam", "gm_nSum", gm_nSum);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentGoodSize", gm_nCurrentGoodSize);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentFailSize", gm_nCurrentFailSize);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalCheckFail", gm_nTotalCheckFail);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalFPFail", gm_nTotalFPFail);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalGoodSize", gm_nTotalGoodSize);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalFailSize", gm_nTotalFailSize);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentTotalSize", gm_nCurrentTotalSize);
    inf.WriteIntNumber("GlobalParam", "gm_nTotalStock", gm_nTotalStock);
    inf.WriteIntNumber("GlobalParam", "gm_nCurrentStock", gm_nCurrentStock);
    inf.WriteIntNumber("GlobalParam", "gm_nStockGoodSize", gm_nStockGoodSize);
    inf.WriteIntNumber("GlobalParam", "gm_bTapeCamera", gm_bTapeCamera);
    inf.WriteIntNumber("GlobalParam", "m_nTime", m_nTime);

    inf.WriteString("gm_sdInfo", "strCheckSum", gm_sdInfo.strCheckSum);
    inf.WriteString("gm_sdInfo", "strCustomer", gm_sdInfo.strCustomer);
    inf.WriteString("gm_sdInfo", "strDate", gm_sdInfo.strDate);
    inf.WriteString("gm_sdInfo", "strEndTime", gm_sdInfo.strEndTime);
    inf.WriteString("gm_sdInfo", "strICBrand", gm_sdInfo.strICBrand);
    inf.WriteString("gm_sdInfo", "strICModel", gm_sdInfo.strICModel);
    inf.WriteString("gm_sdInfo", "strOperator", gm_sdInfo.strOperator);
    inf.WriteString("gm_sdInfo", "strRunTime", gm_sdInfo.strRunTime);
    inf.WriteString("gm_sdInfo", "strStartTime", gm_sdInfo.strStartTime);
    inf.WriteString("gm_sdInfo", "strWorkNo", gm_sdInfo.strWorkNo);
    inf.WriteIntNumber("gm_sdInfo", "nLotNum", gm_sdInfo.nLotNum);

    inf.WriteString("Model", "chip", gm_strChipModel);
    inf.WriteString("Model", "socket", gm_strSocketModel);

    //inf.WriteIntNumber("RotateMode", "In", gm_nRotateAngleIn);
    //inf.WriteIntNumber("RotateMode", "Out", gm_nRotateAngleOut);

    SavePosition();//保存坐标

}

void CAutoFPDlg::LoadConfig()
{
    CString str;
    CIniFile inf;
    inf.SetIniFileName(gm_strConfigFile);
    inf.GetString("File", "Last", str, "sinaen");
    if (str != "sinaen")
    {
        gm_strPositionFile = str;//获取最后结束时，程序载入的位置文件
    }
    else
    {
        AfxMessageBox("位置文件丢失，需要重新载入");
        gm_strPositionFile = gm_strSetupPath + "default.dat";//位置文件路径
    }
    LoadPosition();
    //获取上次退出时选中的烧录座的信息
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER*MAXNUM_OF_SOCKET; i++)
    {
        //int evalue = gm_sBurningStatus[i / MAXNUMOFSOCKET].bEmpty[i%MAXNUMOFSOCKET];
        str.Format("Socket%d", i);
        gm_bSocketUsed[i] = inf.GetInt("LastUsed", str, 1);
        gm_nDisplayGoodForSocket[i] = inf.GetInt("PassNumPerSocket", str, 0);
        gm_nDisplayFailForSocket[i] = inf.GetInt("FailNumPerSocket", str, 0);
        gm_sBurningStatus[i / MAXNUM_OF_SOCKET].bEmpty[i % MAXNUM_OF_SOCKET] = inf.GetInt("Empty", str, 1);
        gm_sBurningStatus[i / MAXNUM_OF_SOCKET].nAdpStatus[i % MAXNUM_OF_SOCKET] = inf.GetInt("Status", str, 0);
        DisplayGood(i, gm_nDisplayGoodForSocket[i]);
        DisplayFail(i, gm_nDisplayFailForSocket[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        str.Format("Nozzle%d", i);
        gm_bNozzleUsed[i] = inf.GetInt("Used", str, 0);
    }
    gm_bConvertChip = inf.GetInt("ConvertChip", "ConvertChip", 0);
    gm_nNozzleNum = 0;
    for (int i = 0; i < 4; i++)
    {
        if (gm_bNozzleUsed[i])
            gm_nNozzleNum++;
    }

    for (int i = 0; i < gm_dataTray.xn*gm_dataTray.yn; i++)
    {
        str.Format("tray%d", i);
        gm_pTray[i] = inf.GetInt("TrayStatus", str, 2);
    }
    //载入所有全局变量
    gm_nTotalInSize = inf.GetInt("GlobalParam", "gm_nTotalInSize", 0);
    gm_nTotalOutSize = inf.GetInt("GlobalParam", "gm_nTotalOutSize", 0);
    gm_nCurrentInSize = inf.GetInt("GlobalParam", "gm_nCurrentInSize", 0);
    gm_nCurrentOutSize = inf.GetInt("GlobalParam", "gm_nCurrentOutSize", 0);
    gm_nSum = inf.GetInt("GlobalParam", "gm_nSum", 0);
    gm_nCurrentGoodSize = inf.GetInt("GlobalParam", "gm_nCurrentGoodSize", 0);
    gm_nCurrentFailSize = inf.GetInt("GlobalParam", "gm_nCurrentFailSize", 0);
    gm_nTotalCheckFail = inf.GetInt("GlobalParam", "gm_nTotalCheckFail", 0);
    gm_nTotalFPFail = inf.GetInt("GlobalParam", "gm_nTotalFPFail", 0);
    gm_nTotalGoodSize = inf.GetInt("GlobalParam", "gm_nTotalGoodSize", 0);
    gm_nTotalFailSize = inf.GetInt("GlobalParam", "gm_nTotalFailSize", 0);
    gm_nCurrentTotalSize = inf.GetInt("GlobalParam", "gm_nCurrentTotalSize", 0);
    gm_nTotalStock = inf.GetInt("GlobalParam", "gm_nTotalStock", 0);
    gm_nCurrentStock = inf.GetInt("GlobalParam", "gm_nCurrentStock", 0);
    gm_nStockGoodSize = inf.GetInt("GlobalParam", "gm_nStockGoodSize", 0);
    gm_bTapeCamera = inf.GetInt("GlobalParam", "gm_bTapeCamera", 0);
    m_nTime = inf.GetInt("GlobalParam", "m_nTime", 0);

    inf.GetString("gm_sdInfo", "strCheckSum", gm_sdInfo.strCheckSum, "");
    inf.GetString("gm_sdInfo", "strCustomer", gm_sdInfo.strCustomer, "");
    inf.GetString("gm_sdInfo", "strDate", gm_sdInfo.strDate, "");
    inf.GetString("gm_sdInfo", "strEndTime", gm_sdInfo.strEndTime, "");
    inf.GetString("gm_sdInfo", "strICBrand", gm_sdInfo.strICBrand, "");
    inf.GetString("gm_sdInfo", "strICModel", gm_sdInfo.strICModel, "");
    inf.GetString("gm_sdInfo", "strOperator", gm_sdInfo.strOperator, "");
    inf.GetString("gm_sdInfo", "strRunTime", gm_sdInfo.strRunTime, "");
    inf.GetString("gm_sdInfo", "strStartTime", gm_sdInfo.strStartTime, "");
    inf.GetString("gm_sdInfo", "strWorkNo", gm_sdInfo.strWorkNo, "");
    gm_sdInfo.nLotNum = inf.GetInt("gm_sdInfo", "nLotNum", 0);

    inf.GetString("Model", "chip", gm_strChipModel, "");
    inf.GetString("Model", "socket", gm_strSocketModel, "");

    //gm_nRotateAngleIn = inf.GetInt("RotateMode", "In", 0);
    //gm_nRotateAngleOut = inf.GetInt("RotateMode", "Out", 0);

    RefreshMainData();
    if (gm_sdInfo.nLotNum <= gm_nTotalGoodSize + gm_nTotalFailSize && gm_nTotalGoodSize != 0 || gm_nTotalFailSize != 0)
    {
        if (IDYES == AfxMessageBox("上次烧录项目已经完成，是否清空上次烧录数据？", MB_YESNO))
        {
            CleanData();
        }
    }
}
//刷新主界面数据信息显示
void CAutoFPDlg::RefreshMainData()
{
    //SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_uInfo.strProcName);
    SetDlgItemText(IDC_MAIN_PROJECT_NAME, gm_sdInfo.strWorkNo);//工单号码
    SetDlgItemText(IDC_MAIN_CHECK_SUM, gm_sdInfo.strCheckSum);//CheckSum
    SetDlgItemText(IDC_MAIN_CHIP_MODEL, gm_sdInfo.strICModel);//芯片类型
    SetDlgItemText(IDC_MAIN_CHIP_BRAND, gm_sdInfo.strICBrand);//芯片厂家
    CString str;
    str.Format("%d", gm_sdInfo.nLotNum);
    SetDlgItemText(IDC_MAIN_WORKNUM_SUM, str);//工单总数
    ShowGoodSize(gm_nTotalGoodSize);		//Pass数量
    ShowFailSize(gm_nTotalFPFail, gm_nTotalCheckFail);//烧录NG，操作NG
    float yield = 0.0;
    yield = (gm_nTotalGoodSize + gm_nTotalFailSize == 0) ? 0 : float(gm_nTotalGoodSize) / float(gm_nTotalGoodSize + gm_nTotalFailSize);
    ShowYield(yield);			//良率

    gm_sdInfo.nUPH = m_nTime == 0 ? 0 : gm_nTotalGoodSize / (m_nTime / float(3600));
    str.Format("%d", gm_sdInfo.nUPH);
    SetDlgItemText(IDC_MAIN_UPH, str);//UPH

    int nTemp = 0;
    int nSecond = 0;
    int nHour = 0;
    int nMinute = 0;
    nTemp = m_nTime / 60;
    nSecond = m_nTime % 60;
    nHour = nTemp / 60;
    nMinute = nTemp % 60;
    str.Format("%02d:%02d:%02d", nHour, nMinute, nSecond);
    m_ctrlUsedTime.SetText(str);//运行时间

    //SetTimer(TIMER_RUN_TIME, 1000, NULL);
    //KillTimer(TIMER_RUN_TIME);

    //Invalidate(FALSE);
    Invalidate(TRUE);

}
void CAutoFPDlg::LoadPosition()
{
    //载入设置参数
    SetWindowText(gm_strPositionFile);

    gm_dataPos.Init(gm_strPositionFile);

    CIniFile inf;
    inf.SetIniFileName(gm_strPositionFile);
    gm_dataTray.xn = inf.GetInt("Tray", "XN", 8);
    gm_dataTray.yn = inf.GetInt("Tray", "YN", 12);

    gm_dataStock.xn = inf.GetInt("Stock", "XN", 8);
    gm_dataStock.yn = inf.GetInt("Stock", "YN", 12);

    gm_dataNG.xn = inf.GetInt("NG", "XN", 8);
    gm_dataNG.yn = inf.GetInt("NG", "YN", 12);

    gm_uInfo.xySpeed = inf.GetInt("System", "xyspeed", 3);
    gm_uInfo.zSpeed = inf.GetInt("System", "zspeed", 3);
    gm_uInfo.nGetTrayHeight = inf.GetInt("System", "get_tray", 0);
    gm_uInfo.nPutTrayHeight = inf.GetInt("System", "put_tray", 0);
    gm_uInfo.nGetSocketHeight = inf.GetInt("System", "get_socket", 2000);
    gm_uInfo.nPutSocketHeight = inf.GetInt("System", "put_socket", 400);
    gm_uInfo.nSocketTime = inf.GetInt("System", "socket_time", 400);
    gm_uInfo.nTrayTime = inf.GetInt("System", "tray_time", 400);
    gm_uInfo.nElapsed = inf.GetInt("System", "elapsed_time", 6 * 1000);

    CString str;
    inf.GetString("System", "camera_number", str, "RH0892004016");
    gm_uInfo.strCameraNum = str;
    inf.GetString("System", "camera_version", str, "MER-130-30UM-L");
    gm_uInfo.strCameraVersion = str;
    inf.GetString("System", "down_camera_number", str, "RH0892004016");
    gm_uInfo.strDownCameraNum = str;
    inf.GetString("System", "down_camera_version", str, "MER-130-30UM-L");
    gm_uInfo.strDownCameraVersion = str;
    //inf.GetString("Model", "chip", str, "");
    //gm_strChipModel = str;
    //inf.GetString("Model", "socket", str, "");
    //gm_strSocketModel = str;

    gm_uInfo.nTapeIn = inf.GetInt("Tape", "in_count", 4000);
    gm_uInfo.nWorkMode = inf.GetInt("System", "workmode", 0);
    gm_bAutoTray = inf.GetInt("Tray", "mode", 0);
    gm_bStock = inf.GetInt("Tray", "stock", 0);
    gm_bTapeCheck = inf.GetInt("Tray", "tape_check", 0);
    gm_bDownCameraIn = inf.GetInt("Tray", "down_camera_in", 0);
    gm_bDownCameraOut = inf.GetInt("Tray", "down_camera_out", 0);

    gm_uInfo.fTapeInSpeed = inf.GetFloat("Tape", "speedIn", 4);
    gm_uInfo.fTapeOutSpeed = inf.GetFloat("Tape", "speedOut", 4);
    gm_uInfo.fTapeInStep = inf.GetFloat("Tape", "stepIn", 4);
    gm_uInfo.fTapeOutStep = inf.GetFloat("Tape", "stepOut", 4);
    gm_uInfo.nPixelsPM_up = inf.GetFloat("System", "up_pixels_per_mm", 17.5);
    gm_uInfo.nPixelsPM_down = inf.GetFloat("System", "down_pixels_per_mm", 50);
    gm_uInfo.nChipLength = inf.GetFloat("System", "chip_length", 4);
    gm_uInfo.nChipWidth = inf.GetFloat("System", "chip_width", 4);
    gm_uInfo.nSocketLength = inf.GetFloat("System", "socket_length", 20);
    gm_uInfo.nSocketWidth = inf.GetFloat("System", "socket_width", 20);
    gm_uInfo.nTapeLength = inf.GetFloat("System", "tape_length", 20);
    gm_uInfo.nTapeWidth = inf.GetFloat("System", "tape_width", 20);
    gm_uInfo.nOffsetInX = inf.GetFloat("System", "offset_in_x", 0);
    gm_uInfo.nOffsetInY = inf.GetFloat("System", "offset_in_y", 0);
    gm_uInfo.nOffsetOutX = inf.GetFloat("System", "offset_out_x", 0);
    gm_uInfo.nOffsetOutY = inf.GetFloat("System", "offset_out_y", 0);
    gm_uInfo.nOffsetSocketX = inf.GetFloat("System", "offset_socket_x", 0);
    gm_uInfo.nOffsetSocketY = inf.GetFloat("System", "offset_socket_y", 0);

    gm_uInfo.fCheckScore = inf.GetFloat("System", "check_score", 0.75);
    gm_uInfo.fCheckScoreDown = inf.GetFloat("System", "check_score_down", 0.75);
    gm_uInfo.nNozzleDelay = inf.GetInt("System", "nozzle_delay", 100);

    gm_uInfo.nCoverTime = inf.GetInt("Tape", "coverTime", 2);

    gm_nRotateAngleIn = inf.GetInt("RotateMode", "In", 0);
    gm_nRotateAngleOut = inf.GetInt("RotateMode", "Out", 0);

    if ((m_xn != gm_dataTray.xn) || (m_yn != gm_dataTray.yn))
    {
        Invalidate(FALSE);
        m_xn = gm_dataTray.xn;
        m_yn = gm_dataTray.yn;
        delete[]gm_pTray;
        gm_pTray = NULL;
        gm_pTray = new BYTE[gm_dataTray.xn*gm_dataTray.yn];
        int total = gm_dataTray.xn * gm_dataTray.yn;
        for (int i = 0; i < total; i++)
            gm_pTray[i] = 2;
        Invalidate(TRUE);
    }
}
void CAutoFPDlg::SavePosition()
{
    gm_dataPos.Save(gm_strPositionFile);

    CIniFile inf;
    inf.SetIniFileName(gm_strPositionFile);
    inf.WriteIntNumber("Tray", "XN", gm_dataTray.xn);
    inf.WriteIntNumber("Tray", "YN", gm_dataTray.yn);
    inf.WriteIntNumber("Tray", "mode", gm_bAutoTray);
    inf.WriteIntNumber("Tray", "stock", gm_bStock);
    inf.WriteIntNumber("Tray", "tape_check", gm_bTapeCheck);
    inf.WriteIntNumber("Tray", "down_camera_in", gm_bDownCameraIn);
    inf.WriteIntNumber("Tray", "down_camera_out", gm_bDownCameraOut);
    inf.WriteIntNumber("Stock", "XN", gm_dataStock.xn);
    inf.WriteIntNumber("Stock", "YN", gm_dataStock.yn);

    inf.WriteIntNumber("NG", "XN", gm_dataNG.xn);
    inf.WriteIntNumber("NG", "YN", gm_dataNG.yn);

    inf.WriteIntNumber("System", "xyspeed", gm_uInfo.xySpeed);
    inf.WriteIntNumber("System", "zspeed", gm_uInfo.zSpeed);
    inf.WriteIntNumber("System", "get_tray", gm_uInfo.nGetTrayHeight);
    inf.WriteIntNumber("System", "put_tray", gm_uInfo.nPutTrayHeight);
    inf.WriteIntNumber("System", "put_socket", gm_uInfo.nPutSocketHeight);
    inf.WriteIntNumber("System", "get_socket", gm_uInfo.nGetSocketHeight);
    inf.WriteIntNumber("System", "get_socket", gm_uInfo.nGetSocketHeight);

    inf.WriteIntNumber("System", "socket_time", gm_uInfo.nSocketTime);
    inf.WriteIntNumber("System", "tray_time", gm_uInfo.nTrayTime);
    inf.WriteIntNumber("System", "elapsed_time", gm_uInfo.nElapsed);
    inf.WriteFloatNumber("System", "up_pixels_per_mm", gm_uInfo.nPixelsPM_up);
    inf.WriteFloatNumber("System", "down_pixels_per_mm", gm_uInfo.nPixelsPM_down);
    inf.WriteFloatNumber("System", "chip_length", gm_uInfo.nChipLength);
    inf.WriteFloatNumber("System", "chip_width", gm_uInfo.nChipWidth);
    inf.WriteFloatNumber("System", "socket_length", gm_uInfo.nSocketLength);
    inf.WriteFloatNumber("System", "socket_width", gm_uInfo.nSocketWidth);
    inf.WriteFloatNumber("System", "tape_length", gm_uInfo.nTapeLength);
    inf.WriteFloatNumber("System", "tape_width", gm_uInfo.nTapeWidth);

    inf.WriteFloatNumber("System", "offset_in_x", gm_uInfo.nOffsetInX);
    inf.WriteFloatNumber("System", "offset_in_y", gm_uInfo.nOffsetInY);
    inf.WriteFloatNumber("System", "offset_out_x", gm_uInfo.nOffsetOutX);
    inf.WriteFloatNumber("System", "offset_out_y", gm_uInfo.nOffsetOutY);
    inf.WriteFloatNumber("System", "offset_socket_x", gm_uInfo.nOffsetSocketX);
    inf.WriteFloatNumber("System", "offset_socket_y", gm_uInfo.nOffsetSocketY);


    inf.WriteIntNumber("System", "workmode", gm_uInfo.nWorkMode);
    inf.WriteIntNumber("Tape", "in_count", gm_uInfo.nTapeIn);//满圈数量
    inf.WriteIntNumber("Tape", "out", gm_uInfo.nTapeOut);
    inf.WriteFloatNumber("Tape", "speedIn", gm_uInfo.fTapeInSpeed);
    inf.WriteFloatNumber("Tape", "speedOut", gm_uInfo.fTapeOutSpeed);
    inf.WriteFloatNumber("Tape", "stepIn", gm_uInfo.fTapeInStep);
    inf.WriteFloatNumber("Tape", "stepOut", gm_uInfo.fTapeOutStep);
    inf.WriteIntNumber("Tape", "coverTime", gm_uInfo.nCoverTime);

    inf.WriteString("System", "camera_number", gm_uInfo.strCameraNum);
    inf.WriteString("System", "camera_version", gm_uInfo.strCameraVersion);
    inf.WriteString("System", "down_camera_number", gm_uInfo.strDownCameraNum);
    inf.WriteString("System", "down_camera_version", gm_uInfo.strDownCameraVersion);

    inf.WriteFloatNumber("System", "check_score", gm_uInfo.fCheckScore);
    inf.WriteFloatNumber("System", "check_score_down", gm_uInfo.fCheckScoreDown);
    inf.WriteIntNumber("System", "nozzle_delay", gm_uInfo.nNozzleDelay);

    inf.WriteIntNumber("RotateMode", "In", gm_nRotateAngleIn);
    inf.WriteIntNumber("RotateMode", "Out", gm_nRotateAngleOut);
    //inf.WriteString("Model", "chip", gm_strChipModel);
    //inf.WriteString("Model", "socket", gm_strSocketModel);
}
void CAutoFPDlg::OnAbout()
{
    // TODO:  在此添加命令处理程序代码
    CAboutDlg dlg;
    dlg.DoModal();
}

//显示编程器软件
void CAutoFPDlg::On32772()
{
    SendInfo("Show App");
}

//隐藏编程器软件
void CAutoFPDlg::On32773()
{
    SendInfo("Hide App");
}

//打开编程器软件
void CAutoFPDlg::OnOpenProgrammer()
{
    HINSTANCE hNewExe = ShellExecuteA(NULL, "open", "d:\\Multiprog.exe", NULL, NULL, SW_HIDE);
}

//载入工程
void CAutoFPDlg::OnMenuLoad()
{
    if (gm_bStart || gm_bFinish)
    {
        AfxMessageBox("机器正在运行，无法载入工程!");
        return;
    }
    CFileDialog saveprj(TRUE,
        "*.dat",
        "*.dat",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Project file (*.dat)|*.dat|All file(*.*)|*.*||"
    );
    CString s;
    saveprj.m_ofn.lpstrTitle = "Open User Data File";
    if (saveprj.DoModal() != IDOK)
        return;

    s = saveprj.GetPathName();

    CFileFind finder;
    BOOL bFind = finder.FindFile(s);
    if (!bFind)
    {
        AfxMessageBox("该数据文件不存在!");
        finder.Close();
        return;
    }
    finder.Close();

    CString ext = s.Right(3);
    ext.MakeLower();
    if (ext.Compare("dat") != 0)
    {
        AfxMessageBox("无效的用户数据文件!");
        return;
    }
    SetWindowText(s);
    gm_strPositionFile = s;
    gm_dataPos.Init(gm_strPositionFile);

    CIniFile inf;
    inf.SetIniFileName(gm_strPositionFile);
    gm_dataTray.xn = inf.GetInt("Tray", "XN", 8);
    gm_dataTray.yn = inf.GetInt("Tray", "YN", 12);

    gm_dataStock.xn = inf.GetInt("Stock", "XN", 8);
    gm_dataStock.yn = inf.GetInt("Stock", "YN", 12);

    gm_dataNG.xn = inf.GetInt("NG", "XN", 8);
    gm_dataNG.yn = inf.GetInt("NG", "YN", 12);

    gm_uInfo.xySpeed = inf.GetInt("System", "xyspeed", 3);
    gm_uInfo.zSpeed = inf.GetInt("System", "zspeed", 3);
    gm_uInfo.nGetTrayHeight = inf.GetInt("System", "get_tray", 0);
    gm_uInfo.nPutTrayHeight = inf.GetInt("System", "put_tray", 0);
    gm_uInfo.nGetSocketHeight = inf.GetInt("System", "get_socket", 2000);
    gm_uInfo.nPutSocketHeight = inf.GetInt("System", "put_socket", 400);
    gm_uInfo.nSocketTime = inf.GetInt("System", "socket_time", 400);
    gm_uInfo.nTrayTime = inf.GetInt("System", "tray_time", 400);
    gm_uInfo.nElapsed = inf.GetInt("System", "elapsed_time", 6 * 1000);
    gm_uInfo.nPixelsPM_up = inf.GetFloat("System", "up_pixels_per_mm", 17.5);
    gm_uInfo.nPixelsPM_down = inf.GetFloat("System", "down_pixels_per_mm", 17.5);
    gm_uInfo.nChipLength = inf.GetFloat("System", "chip_length", 4);
    gm_uInfo.nChipWidth = inf.GetFloat("System", "chip_width", 4);
    gm_uInfo.nSocketLength = inf.GetFloat("System", "socket_length", 20);
    gm_uInfo.nSocketWidth = inf.GetFloat("System", "socket_width", 20);
    gm_uInfo.nTapeLength = inf.GetFloat("System", "tape_length", 20);
    gm_uInfo.nTapeWidth = inf.GetFloat("System", "tape_width", 20);
    gm_uInfo.nOffsetInX = inf.GetFloat("System", "offset_in_x", 0);
    gm_uInfo.nOffsetInY = inf.GetFloat("System", "offset_in_y", 0);
    gm_uInfo.nOffsetOutX = inf.GetFloat("System", "offset_out_x", 0);
    gm_uInfo.nOffsetOutY = inf.GetFloat("System", "offset_out_y", 0);
    gm_uInfo.nOffsetSocketX = inf.GetFloat("System", "offset_socket_x", 0);
    gm_uInfo.nOffsetSocketY = inf.GetFloat("System", "offset_socket_y", 0);

    gm_uInfo.fCheckScore = inf.GetFloat("System", "check_score", 0.75);
    gm_uInfo.fCheckScoreDown = inf.GetFloat("System", "check_score_down", 0.75);
    gm_uInfo.nNozzleDelay = inf.GetInt("System", "nozzle_delay", 100);

    CString str;
    inf.GetString("System", "camera_number", str, "RH0892004016");
    gm_uInfo.strCameraNum = str;
    inf.GetString("System", "camera_version", str, "MER-130-30UM-L");
    gm_uInfo.strCameraVersion = str;
    inf.GetString("System", "down_camera_number", str, "RH0892004016");
    gm_uInfo.strDownCameraNum = str;
    inf.GetString("System", "down_camera_version", str, "MER-130-30UM-L");
    gm_uInfo.strDownCameraVersion = str;
    //inf.GetString("Model", "chip", str, "");
    //gm_strChipModel = str;
    //inf.GetString("Model", "socket", str, "");
    //gm_strSocketModel = str;

    gm_uInfo.nTapeIn = inf.GetInt("Tape", "in_count", 4000);
    gm_uInfo.nWorkMode = inf.GetInt("System", "workmode", 0);
    gm_bAutoTray = inf.GetInt("Tray", "mode", 0);
    gm_bStock = inf.GetInt("Tray", "stock", 0);
    gm_bTapeCheck = inf.GetInt("Tray", "tape_check", 0);
    gm_bDownCameraIn = inf.GetInt("Tray", "down_camera_in", 0);
    gm_bDownCameraOut = inf.GetInt("Tray", "down_camera_out", 0);

    gm_uInfo.fTapeInSpeed = inf.GetFloat("Tape", "speedIn", 4);
    gm_uInfo.fTapeOutSpeed = inf.GetFloat("Tape", "speedOut", 4);
    gm_uInfo.fTapeInStep = inf.GetFloat("Tape", "stepIn", 4);
    gm_uInfo.fTapeOutStep = inf.GetFloat("Tape", "stepOut", 4);
    gm_uInfo.nCoverTime = inf.GetInt("Tape", "coverTime", 2);

    gm_nRotateAngleIn = inf.GetInt("RotateMode", "In", 0);
    gm_nRotateAngleOut = inf.GetInt("RotateMode", "Out", 0);

    if ((m_xn != gm_dataTray.xn) || (m_yn != gm_dataTray.yn))
    {
        Invalidate(FALSE);
        m_xn = gm_dataTray.xn;
        m_yn = gm_dataTray.yn;
        delete[]gm_pTray;
        gm_pTray = NULL;
        gm_pTray = new BYTE[gm_dataTray.xn*gm_dataTray.yn];
        int total = gm_dataTray.xn * gm_dataTray.yn;
        for (int i = 0; i < total; i++)
            gm_pTray[i] = 2;
        Invalidate(TRUE);
    }
}
//保存工程
void CAutoFPDlg::OnMenuSave()
{
    CFileDialog saveprj(FALSE,
        "*.dat",
        "*.dat",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Project file (*.dat)|*.dat|All file(*.*)|*.*||"
    );
    CString strFilePath;
    saveprj.m_ofn.lpstrTitle = "Save User Data File";
    if (saveprj.DoModal() != IDOK)
        return;

    strFilePath = saveprj.GetPathName();
    CString ext = strFilePath.Right(3);
    if (ext.Compare("dat") == 0)
    {
        SetWindowText(strFilePath);
        gm_strPositionFile = strFilePath;

        gm_dataPos.Save(gm_strPositionFile);

        CIniFile inf;
        inf.SetIniFileName(gm_strPositionFile);
        inf.WriteIntNumber("Tray", "XN", gm_dataTray.xn);
        inf.WriteIntNumber("Tray", "YN", gm_dataTray.yn);
        inf.WriteIntNumber("Tray", "mode", gm_bAutoTray);
        inf.WriteIntNumber("Tray", "stock", gm_bStock);
        inf.WriteIntNumber("Tray", "tape_check", gm_bTapeCheck);
        inf.WriteIntNumber("Tray", "down_camera_in", gm_bDownCameraIn);
        inf.WriteIntNumber("Tray", "down_camera_out", gm_bDownCameraOut);
        inf.WriteIntNumber("Stock", "XN", gm_dataStock.xn);
        inf.WriteIntNumber("Stock", "YN", gm_dataStock.yn);

        inf.WriteIntNumber("NG", "XN", gm_dataNG.xn);
        inf.WriteIntNumber("NG", "YN", gm_dataNG.yn);

        inf.WriteIntNumber("System", "xyspeed", gm_uInfo.xySpeed);
        inf.WriteIntNumber("System", "zspeed", gm_uInfo.zSpeed);
        inf.WriteIntNumber("System", "get_tray", gm_uInfo.nGetTrayHeight);
        inf.WriteIntNumber("System", "put_tray", gm_uInfo.nPutTrayHeight);
        inf.WriteIntNumber("System", "put_socket", gm_uInfo.nPutSocketHeight);
        inf.WriteIntNumber("System", "get_socket", gm_uInfo.nGetSocketHeight);
        inf.WriteIntNumber("System", "get_socket", gm_uInfo.nGetSocketHeight);

        inf.WriteIntNumber("System", "socket_time", gm_uInfo.nSocketTime);
        inf.WriteIntNumber("System", "tray_time", gm_uInfo.nTrayTime);
        inf.WriteIntNumber("System", "elapsed_time", gm_uInfo.nElapsed);
        inf.WriteFloatNumber("System", "up_pixels_per_mm", gm_uInfo.nPixelsPM_up);
        inf.WriteFloatNumber("System", "down_pixels_per_mm", gm_uInfo.nPixelsPM_down);
        inf.WriteFloatNumber("System", "chip_length", gm_uInfo.nChipLength);
        inf.WriteFloatNumber("System", "chip_width", gm_uInfo.nChipWidth);
        inf.WriteFloatNumber("System", "socket_length", gm_uInfo.nSocketLength);
        inf.WriteFloatNumber("System", "socket_width", gm_uInfo.nSocketWidth);
        inf.WriteFloatNumber("System", "tape_length", gm_uInfo.nTapeLength);
        inf.WriteFloatNumber("System", "tape_width", gm_uInfo.nTapeWidth);

        inf.WriteFloatNumber("System", "offset_in_x", gm_uInfo.nOffsetInX);
        inf.WriteFloatNumber("System", "offset_in_y", gm_uInfo.nOffsetInY);
        inf.WriteFloatNumber("System", "offset_out_x", gm_uInfo.nOffsetOutX);
        inf.WriteFloatNumber("System", "offset_out_y", gm_uInfo.nOffsetOutY);
        inf.WriteFloatNumber("System", "offset_socket_x", gm_uInfo.nOffsetSocketX);
        inf.WriteFloatNumber("System", "offset_socket_y", gm_uInfo.nOffsetSocketY);


        inf.WriteIntNumber("System", "workmode", gm_uInfo.nWorkMode);
        inf.WriteIntNumber("Tape", "in_count", gm_uInfo.nTapeIn);//满圈数量
        inf.WriteIntNumber("Tape", "out", gm_uInfo.nTapeOut);
        inf.WriteFloatNumber("Tape", "speedIn", gm_uInfo.fTapeInSpeed);
        inf.WriteFloatNumber("Tape", "speedOut", gm_uInfo.fTapeOutSpeed);
        inf.WriteFloatNumber("Tape", "stepIn", gm_uInfo.fTapeInStep);
        inf.WriteFloatNumber("Tape", "stepOut", gm_uInfo.fTapeOutStep);
        inf.WriteIntNumber("Tape", "coverTime", gm_uInfo.nCoverTime);

        inf.WriteString("System", "camera_number", gm_uInfo.strCameraNum);
        inf.WriteString("System", "camera_version", gm_uInfo.strCameraVersion);
        inf.WriteString("System", "down_camera_number", gm_uInfo.strDownCameraNum);
        inf.WriteString("System", "down_camera_version", gm_uInfo.strDownCameraVersion);

        inf.WriteFloatNumber("System", "check_score", gm_uInfo.fCheckScore);
        inf.WriteFloatNumber("System", "check_score_down", gm_uInfo.fCheckScoreDown);
        inf.WriteIntNumber("System", "nozzle_delay", gm_uInfo.nNozzleDelay);

        //inf.WriteString("Model", "chip", gm_strChipModel);
        //inf.WriteString("Model", "socket", gm_strSocketModel);
        inf.WriteIntNumber("RotateMode", "In", gm_nRotateAngleIn);
        inf.WriteIntNumber("RotateMode", "Out", gm_nRotateAngleOut);

    }
    else
    {
        AfxMessageBox("无效的数据文件!");
    }
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化

    m_strUpdateLog = "更新日志：\r\n2016年10月10日：\r\n1.增加研华板卡机械轴正极限检测,各轴到达正极限时，会自动停止，但y轴存在bug，到达正极限后无法返回。\r\n2.重新调整配置文件载入方式，程序启动时系统会自动载入之前保存的文件，无需再手动载入。\r\n3.调整蜂鸣器响度。\r\n4.增加了关于菜单项，用以查看更新日志。\r\n5.取消设置界面SlaveMode按钮，程序运行时会自动连接编程器。\r\n6.每个烧录座添加手动烧录功能，双击每个烧录座的第一个Good状态栏即可。\r\n\r\n";
    m_strUpdateLog += "2016年10月12日：\r\n1.添加下相机图像检测功能\r\n2.增加旋转轴\r\n\r\n";
    m_strUpdateLog += "2016年10月13日：\r\n1.增加记忆烧录座选中状态\r\n2.增加芯片规格设置，便于下相机模板建立\r\n\r\n";
    m_strUpdateLog += "2016年10月14日：\r\n1.1.增加飞达反馈信号处理；\r\n2.更改废料盘计数规则；\r\n3.增加烧录座自动校准功能，并且同时检测座子中是否有芯片，如果有，则不再往里放；\r\n\r\n";
    m_strUpdateLog += "2016年10月17日：\r\n1.完善日志文件;\r\n2.优化运行过程;\r\n3.增加主界面xyz轴坐标实时显示\r\n\r\n";
    m_strUpdateLog += "2016年10月19日：\r\n1.增加进出料xy坐标补偿;\r\n2.完善日志文件;\r\n3.修改主界面烧录状态显示\r\n\r\n";
    m_strUpdateLog += "2016年10月21日：\r\n1.优化补料程序;\r\n2.优化自动上下料程序;\r\n3.优化飞达上下料程序;\r\n4.修复编带机结束时误报警bug\r\n5.修改补料状态下，结束时烧录座中芯片的放置问题（如果有补料过程，则结束时烧录座中芯片全部放入补料盘）;\r\n\r\n";
    m_strUpdateLog += "2016年10月24日：\r\n1.优化操作界面，将设置中烧录座选择功能取消，增加主界面点击烧录座显示状态来选中/取消该烧录座;\r\n2.修复烧录完成时tray盘刷新异常显示bug;\r\n3.优化芯片取放过程，减少不必要的延时，提高取放效率\r\n4.增加启动机械手软件时，自动启动编程器软件（编程器软件需要放在\"c:\\S100\\bin\\Multiprog.exe\"目录下）\r\n5.增加烧录座烧录状态实时显示\r\n\r\n";
    UpdateData(false);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常:  OCX 属性页应返回 FALSE
}




void CAutoFPDlg::OnP800Open()
{
    // TODO:  在此添加命令处理程序代码
    m_dlgP800 = new P800IspDlg();
    //Check if new succeeded and a valid pointer to a dialog object is   //created.
    if (m_dlgP800 != NULL)
    {
        BOOL ret = m_dlgP800->Create(IDD_P800ISP_DIALOG, this);
        if (!ret)  //Create failed.
            AfxMessageBox("Error creating Dialog");
        m_dlgP800->ShowWindow(SW_SHOW);
    }
    m_Menu.GetSubMenu(3)->EnableMenuItem(0, MF_GRAYED | MF_BYPOSITION);//变灰
    m_Menu.GetSubMenu(3)->EnableMenuItem(1, MF_BYCOMMAND | MF_BYPOSITION);//变黑
    m_Menu.GetSubMenu(3)->EnableMenuItem(2, MF_BYCOMMAND | MF_BYPOSITION);//变黑
}


void CAutoFPDlg::OnP800Hide()
{
    static int n = 0;
    if (n == 0)
    {
        m_dlgP800->ShowWindow(SW_HIDE);
        m_Menu.GetSubMenu(3)->ModifyMenuA(1, MF_BYPOSITION, ID_P800_HIDE, "显示");
        n++;
    }
    else
    {
        m_dlgP800->ShowWindow(SW_SHOW);
        m_Menu.GetSubMenu(3)->ModifyMenuA(1, MF_BYPOSITION, ID_P800_HIDE, "隐藏");
        n = 0;
    }
}


void CAutoFPDlg::OnP800Close()
{
    if (IDOK == AfxMessageBox("确定要关闭p800编程器吗", MB_OKCANCEL))
    {
        m_dlgP800->DestroyWindow();
        m_dlgP800 = NULL;

        m_Menu.GetSubMenu(3)->EnableMenuItem(0, MF_BYCOMMAND | MF_BYPOSITION);//打开变黑
        m_Menu.GetSubMenu(3)->EnableMenuItem(1, MF_GRAYED | MF_BYPOSITION);//隐藏变灰
        m_Menu.GetSubMenu(3)->EnableMenuItem(2, MF_GRAYED | MF_BYPOSITION);//关闭变灰
    }
}
void CAutoFPDlg::InitStatusCheck(BOOL bFig)
{
    UINT nSocketCheckID[] = { IDC_SOCKET_CHECK11, IDC_SOCKET_CHECK12, IDC_SOCKET_CHECK13, IDC_SOCKET_CHECK14,
    IDC_SOCKET_CHECK21, IDC_SOCKET_CHECK22, IDC_SOCKET_CHECK23, IDC_SOCKET_CHECK24,
    IDC_SOCKET_CHECK31, IDC_SOCKET_CHECK32, IDC_SOCKET_CHECK33, IDC_SOCKET_CHECK34,
    IDC_SOCKET_CHECK41, IDC_SOCKET_CHECK42, IDC_SOCKET_CHECK43, IDC_SOCKET_CHECK44,
    IDC_SOCKET_CHECK51, IDC_SOCKET_CHECK52, IDC_SOCKET_CHECK53, IDC_SOCKET_CHECK54,
    IDC_SOCKET_CHECK61, IDC_SOCKET_CHECK62, IDC_SOCKET_CHECK63, IDC_SOCKET_CHECK64,
    IDC_SOCKET_CHECK71, IDC_SOCKET_CHECK72, IDC_SOCKET_CHECK73, IDC_SOCKET_CHECK74,
    IDC_SOCKET_CHECK81, IDC_SOCKET_CHECK82, IDC_SOCKET_CHECK83, IDC_SOCKET_CHECK84,
    IDC_SOCKET_CHECK91, IDC_SOCKET_CHECK92, IDC_SOCKET_CHECK93, IDC_SOCKET_CHECK94,
    IDC_SOCKET_CHECK101, IDC_SOCKET_CHECK102, IDC_SOCKET_CHECK103, IDC_SOCKET_CHECK104 };//socket check number
    UINT nStatusID[] = { IDC_STATUS11, IDC_STATUS12, IDC_STATUS13, IDC_STATUS14,
    IDC_STATUS21, IDC_STATUS22, IDC_STATUS23, IDC_STATUS24,
    IDC_STATUS31, IDC_STATUS32, IDC_STATUS33, IDC_STATUS34,
    IDC_STATUS41, IDC_STATUS42, IDC_STATUS43, IDC_STATUS44,
    IDC_STATUS51, IDC_STATUS52, IDC_STATUS53, IDC_STATUS54,
    IDC_STATUS61, IDC_STATUS62, IDC_STATUS63, IDC_STATUS64,
    IDC_STATUS71, IDC_STATUS72, IDC_STATUS73, IDC_STATUS74,
    IDC_STATUS81, IDC_STATUS82, IDC_STATUS83, IDC_STATUS84,
    IDC_STATUS91, IDC_STATUS92, IDC_STATUS93, IDC_STATUS94,
    IDC_STATUS101, IDC_STATUS102, IDC_STATUS103, IDC_STATUS104 };
    //CLabel lblGood[] = {	m_good11, m_good12, m_good13, m_good14,
    //						m_good21, m_good22, m_good23, m_good24, 
    //						m_good31, m_good32, m_good33, m_good34, 
    //						m_good41, m_good42, m_good43, m_good44, 
    //						m_good51, m_good52, m_good53, m_good54, 
    //						m_good61, m_good62, m_good63, m_good64, 
    //						m_good71, m_good72, m_good73, m_good74, 
    //						m_good81, m_good82, m_good83, m_good84, 
    //						m_good91, m_good92, m_good93, m_good94, 
    //						m_good101, m_good102, m_good103, m_good104	};
    //CLabel lblFail[] = {	m_fail11, m_fail11, m_fail11, m_fail11,
    //						m_fail21, m_fail21, m_fail21, m_fail21,
    //						m_fail31, m_fail31, m_fail31, m_fail31,
    //						m_fail41, m_fail41, m_fail41, m_fail41,
    //						m_fail51, m_fail51, m_fail51, m_fail51,
    //						m_fail61, m_fail61, m_fail61, m_fail61,
    //						m_fail71, m_fail71, m_fail71, m_fail71,
    //						m_fail81, m_fail81, m_fail81, m_fail81,
    //						m_fail91, m_fail91, m_fail91, m_fail91,
    //						m_fail101, m_fail101, m_fail101, m_fail101	};
    RefreshSocketStatus();
    for (int i = 0; i < MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET; i++)
    {
        if (gm_bSocketUsed[i])
        {
            CButton *pBtn = (CButton *)GetDlgItem(nSocketCheckID[i]);
            pBtn->SetCheck(1);
            InitSockStatus(i);
        }
    }
}

void CAutoFPDlg::OnBnClickedSocketCheck11()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK11);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[0] = 1;
        //m_good11.EnableWindow();
        //m_fail11.EnableWindow();
        //m_good11.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good11.SetTextColor(RGB(0, 0, 0));
        //m_good11.SetFontSize(12);
        //m_fail11.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail11.SetTextColor(RGB(0, 0, 0));
        //m_fail11.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[0] = 0;
        //m_good11.EnableWindow(FALSE);
        //m_fail11.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS11)->EnableWindow(FALSE);
    }
    InitSockStatus(0);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck12()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK12);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[1] = 1;
        //m_good12.EnableWindow();
        //m_fail12.EnableWindow();
        //m_good12.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good12.SetTextColor(RGB(0, 0, 0));
        //m_good12.SetFontSize(12);
        //m_fail12.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail12.SetTextColor(RGB(0, 0, 0));
        //m_fail12.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[1] = 0;
        //m_good12.EnableWindow(FALSE);
        //m_fail12.EnableWindow(FALSE);
    }
    InitSockStatus(1);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck13()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK13);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[2] = 1;
        //m_good13.EnableWindow();
        //m_fail13.EnableWindow();
        //m_good13.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good13.SetTextColor(RGB(0, 0, 0));
        //m_good13.SetFontSize(12);
        //m_fail13.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail13.SetTextColor(RGB(0, 0, 0));
        //m_fail13.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[2] = 0;
        //m_good13.EnableWindow(FALSE);
        //m_fail13.EnableWindow(FALSE);
    }
    InitSockStatus(2);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck14()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK14);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[3] = 1;
        //m_good14.EnableWindow();
        //m_fail14.EnableWindow();
        //m_good14.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good14.SetTextColor(RGB(0, 0, 0));
        //m_good14.SetFontSize(12);
        //m_fail14.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail14.SetTextColor(RGB(0, 0, 0));
        //m_fail14.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[3] = 0;
        //m_good14.EnableWindow(FALSE);
        //m_fail14.EnableWindow(FALSE);
    }
    InitSockStatus(3);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck21()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK21);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[4] = 1;
        //m_good21.EnableWindow();
        //m_fail21.EnableWindow();
        //m_good21.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good21.SetTextColor(RGB(0, 0, 0));
        //m_good21.SetFontSize(12);
        //m_fail21.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail21.SetTextColor(RGB(0, 0, 0));
        //m_fail21.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[4] = 0;
        //m_good21.EnableWindow(FALSE);
        //m_fail21.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS21)->EnableWindow(FALSE);
    }
    InitSockStatus(4);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck22()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK22);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[5] = 1;
        //m_good22.EnableWindow();
        //m_fail22.EnableWindow();
        //m_good22.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good22.SetTextColor(RGB(0, 0, 0));
        //m_good22.SetFontSize(12);
        //m_fail22.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail22.SetTextColor(RGB(0, 0, 0));
        //m_fail22.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[5] = 0;
        //m_good22.EnableWindow(FALSE);
        //m_fail22.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS22)->EnableWindow(FALSE);
    }
    InitSockStatus(5);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck23()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK23);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[6] = 1;
        //m_good23.EnableWindow();
        //m_fail23.EnableWindow();
        //m_good23.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good23.SetTextColor(RGB(0, 0, 0));
        //m_good23.SetFontSize(12);
        //m_fail23.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail23.SetTextColor(RGB(0, 0, 0));
        //m_fail23.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[6] = 0;
        //m_good23.EnableWindow(FALSE);
        //m_fail23.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS23)->EnableWindow(FALSE);
    }
    InitSockStatus(6);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck24()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK24);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[7] = 1;
        //m_good24.EnableWindow();
        //m_fail24.EnableWindow();
        //m_good24.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good24.SetTextColor(RGB(0, 0, 0));
        //m_good24.SetFontSize(12);
        //m_fail24.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail24.SetTextColor(RGB(0, 0, 0));
        //m_fail24.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[7] = 0;
        //m_good24.EnableWindow(FALSE);
        //m_fail24.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS24)->EnableWindow(FALSE);
    }
    InitSockStatus(7);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck31()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK31);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[8] = 1;
        //m_good31.EnableWindow();
        //m_fail31.EnableWindow();
        //m_good31.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good31.SetTextColor(RGB(0, 0, 0));
        //m_good31.SetFontSize(12);
        //m_fail31.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail31.SetTextColor(RGB(0, 0, 0));
        //m_fail31.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[8] = 0;
        //m_good31.EnableWindow(FALSE);
        //m_fail31.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS31)->EnableWindow(FALSE);
    }
    InitSockStatus(8);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck32()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK32);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[9] = 1;
        //m_good32.EnableWindow();
        //m_fail32.EnableWindow();
        //m_good32.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good32.SetTextColor(RGB(0, 0, 0));
        //m_good32.SetFontSize(12);
        //m_fail32.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail32.SetTextColor(RGB(0, 0, 0));
        //m_fail32.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[9] = 0;
        //m_good32.EnableWindow(FALSE);
        //m_fail32.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS32)->EnableWindow(FALSE);
    }
    InitSockStatus(9);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck33()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK33);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[10] = 1;
        //m_good33.EnableWindow();
        //m_fail33.EnableWindow();
        //m_good33.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good33.SetTextColor(RGB(0, 0, 0));
        //m_good33.SetFontSize(12);
        //m_fail33.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail33.SetTextColor(RGB(0, 0, 0));
        //m_fail33.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[10] = 0;
        //m_good33.EnableWindow(FALSE);
        //m_fail33.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS33)->EnableWindow(FALSE);
    }
    InitSockStatus(10);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck34()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK34);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[11] = 1;
        //m_good34.EnableWindow();
        //m_fail34.EnableWindow();
        //m_good34.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good34.SetTextColor(RGB(0, 0, 0));
        //m_good34.SetFontSize(12);
        //m_fail34.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail34.SetTextColor(RGB(0, 0, 0));
        //m_fail34.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[11] = 0;
        //m_good34.EnableWindow(FALSE);
        //m_fail34.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS34)->EnableWindow(FALSE);
    }
    InitSockStatus(11);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck41()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK41);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[12] = 1;
        //m_good41.EnableWindow();
        //m_fail41.EnableWindow();
        //m_good41.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good41.SetTextColor(RGB(0, 0, 0));
        //m_good41.SetFontSize(12);
        //m_fail41.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail41.SetTextColor(RGB(0, 0, 0));
        //m_fail41.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[12] = 0;
        //m_good41.EnableWindow(FALSE);
        //m_fail41.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS41)->EnableWindow(FALSE);
    }
    InitSockStatus(12);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck42()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK42);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[13] = 1;
        //m_good42.EnableWindow();
        //m_fail42.EnableWindow();
        //m_good42.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good42.SetTextColor(RGB(0, 0, 0));
        //m_good42.SetFontSize(12);
        //m_fail42.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail42.SetTextColor(RGB(0, 0, 0));
        //m_fail42.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[13] = 0;
        //m_good42.EnableWindow(FALSE);
        //m_fail42.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS42)->EnableWindow(FALSE);
    }
    InitSockStatus(13);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck43()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK43);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[14] = 1;
        //m_good43.EnableWindow();
        //m_fail43.EnableWindow();
        //m_good43.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good43.SetTextColor(RGB(0, 0, 0));
        //m_good43.SetFontSize(12);
        //m_fail43.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail43.SetTextColor(RGB(0, 0, 0));
        //m_fail43.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[14] = 0;
        //m_good43.EnableWindow(FALSE);
        //m_fail43.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS43)->EnableWindow(FALSE);
    }
    InitSockStatus(14);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck44()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK44);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[15] = 1;
        //m_good44.EnableWindow();
        //m_fail44.EnableWindow();
        //m_good44.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good44.SetTextColor(RGB(0, 0, 0));
        //m_good44.SetFontSize(12);
        //m_fail44.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail44.SetTextColor(RGB(0, 0, 0));
        //m_fail44.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[15] = 0;
        //m_good44.EnableWindow(FALSE);
        //m_fail44.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS44)->EnableWindow(FALSE);
    }
    InitSockStatus(15);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck51()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK51);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[16] = 1;
        //m_good51.EnableWindow();
        //m_fail51.EnableWindow();
        //m_good51.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good51.SetTextColor(RGB(0, 0, 0));
        //m_good51.SetFontSize(12);
        //m_fail51.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail51.SetTextColor(RGB(0, 0, 0));
        //m_fail51.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[16] = 0;
        //m_good51.EnableWindow(FALSE);
        //m_fail51.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS51)->EnableWindow(FALSE);
    }
    InitSockStatus(16);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck52()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK52);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[17] = 1;
        //m_good52.EnableWindow();
        //m_fail52.EnableWindow();
        //m_good52.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good52.SetTextColor(RGB(0, 0, 0));
        //m_good52.SetFontSize(12);
        //m_fail52.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail52.SetTextColor(RGB(0, 0, 0));
        //m_fail52.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[17] = 0;
        //m_good52.EnableWindow(FALSE);
        //m_fail52.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS52)->EnableWindow(FALSE);
    }
    InitSockStatus(17);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck53()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK53);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[18] = 1;
        //m_good53.EnableWindow();
        //m_fail53.EnableWindow();
        //m_good53.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good53.SetTextColor(RGB(0, 0, 0));
        //m_good53.SetFontSize(12);
        //m_fail53.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail53.SetTextColor(RGB(0, 0, 0));
        //m_fail53.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[18] = 0;
        //m_good53.EnableWindow(FALSE);
        //m_fail53.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS53)->EnableWindow(FALSE);
    }
    InitSockStatus(18);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck54()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK54);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[19] = 1;
        //m_good54.EnableWindow();
        //m_fail54.EnableWindow();
        //m_good54.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good54.SetTextColor(RGB(0, 0, 0));
        //m_good54.SetFontSize(12);
        //m_fail54.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail54.SetTextColor(RGB(0, 0, 0));
        //m_fail54.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[19] = 0;
        //m_good54.EnableWindow(FALSE);
        //m_fail54.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS54)->EnableWindow(FALSE);
    }
    InitSockStatus(19);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck61()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK61);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[20] = 1;
        //m_good61.EnableWindow();
        //m_fail61.EnableWindow();
        //m_good61.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good61.SetTextColor(RGB(0, 0, 0));
        //m_good61.SetFontSize(12);
        //m_fail61.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail61.SetTextColor(RGB(0, 0, 0));
        //m_fail61.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[20] = 0;
        //m_good61.EnableWindow(FALSE);
        //m_fail61.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS61)->EnableWindow(FALSE);
    }
    InitSockStatus(20);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck62()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK62);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[21] = 1;
        //m_good62.EnableWindow();
        //m_fail62.EnableWindow();
        //m_good62.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good62.SetTextColor(RGB(0, 0, 0));
        //m_good62.SetFontSize(12);
        //m_fail62.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail62.SetTextColor(RGB(0, 0, 0));
        //m_fail62.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[21] = 0;
        //m_good62.EnableWindow(FALSE);
        //m_fail62.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS62)->EnableWindow(FALSE);
    }
    InitSockStatus(21);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck63()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK63);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[22] = 1;
        //m_good63.EnableWindow();
        //m_fail63.EnableWindow();
        //m_good63.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good63.SetTextColor(RGB(0, 0, 0));
        //m_good63.SetFontSize(12);
        //m_fail63.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail63.SetTextColor(RGB(0, 0, 0));
        //m_fail63.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[22] = 0;
        //m_good63.EnableWindow(FALSE);
        //m_fail63.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS63)->EnableWindow(FALSE);
    }
    InitSockStatus(22);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck64()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK64);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[23] = 1;
        //m_good64.EnableWindow();
        //m_fail64.EnableWindow();
        //m_good64.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good64.SetTextColor(RGB(0, 0, 0));
        //m_good64.SetFontSize(12);
        //m_fail64.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail64.SetTextColor(RGB(0, 0, 0));
        //m_fail64.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[23] = 0;
        //m_good64.EnableWindow(FALSE);
        //m_fail64.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS64)->EnableWindow(FALSE);
    }
    InitSockStatus(23);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck71()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK71);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[24] = 1;
        //m_good71.EnableWindow();
        //m_fail71.EnableWindow();
        //m_good71.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good71.SetTextColor(RGB(0, 0, 0));
        //m_good71.SetFontSize(12);
        //m_fail71.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail71.SetTextColor(RGB(0, 0, 0));
        //m_fail71.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[24] = 0;
        //m_good71.EnableWindow(FALSE);
        //m_fail71.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS71)->EnableWindow(FALSE);
    }
    InitSockStatus(24);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck72()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK72);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[25] = 1;
        //m_good72.EnableWindow();
        //m_fail72.EnableWindow();
        //m_good72.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good72.SetTextColor(RGB(0, 0, 0));
        //m_good72.SetFontSize(12);
        //m_fail72.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail72.SetTextColor(RGB(0, 0, 0));
        //m_fail72.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[25] = 0;
        //m_good72.EnableWindow(FALSE);
        //m_fail72.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS72)->EnableWindow(FALSE);
    }
    InitSockStatus(25);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck73()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK73);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[26] = 1;
        //m_good73.EnableWindow();
        //m_fail73.EnableWindow();
        //m_good73.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good73.SetTextColor(RGB(0, 0, 0));
        //m_good73.SetFontSize(12);
        //m_fail73.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail73.SetTextColor(RGB(0, 0, 0));
        //m_fail73.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[26] = 0;
        //m_good73.EnableWindow(FALSE);
        //m_fail73.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS73)->EnableWindow(FALSE);
    }
    InitSockStatus(26);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck74()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK74);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[27] = 1;
        //m_good74.EnableWindow();
        //m_fail74.EnableWindow();
        //m_good74.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good74.SetTextColor(RGB(0, 0, 0));
        //m_good74.SetFontSize(12);
        //m_fail74.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail74.SetTextColor(RGB(0, 0, 0));
        //m_fail74.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[27] = 0;
        //m_good74.EnableWindow(FALSE);
        //m_fail74.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS74)->EnableWindow(FALSE);
    }
    InitSockStatus(27);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck81()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK81);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[28] = 1;
        //m_good81.EnableWindow();
        //m_fail81.EnableWindow();
        //m_good81.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good81.SetTextColor(RGB(0, 0, 0));
        //m_good81.SetFontSize(12);
        //m_fail81.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail81.SetTextColor(RGB(0, 0, 0));
        //m_fail81.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[28] = 0;
        //m_good81.EnableWindow(FALSE);
        //m_fail81.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS81)->EnableWindow(FALSE);
    }
    InitSockStatus(28);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck82()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK82);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[29] = 1;
        //m_good82.EnableWindow();
        //m_fail82.EnableWindow();
        //m_good82.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good82.SetTextColor(RGB(0, 0, 0));
        //m_good82.SetFontSize(12);
        //m_fail82.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail82.SetTextColor(RGB(0, 0, 0));
        //m_fail82.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[29] = 0;
        //m_good82.EnableWindow(FALSE);
        //m_fail82.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS82)->EnableWindow(FALSE);
    }
    InitSockStatus(29);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck83()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK83);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[30] = 1;
        //m_good83.EnableWindow();
        //m_fail83.EnableWindow();
        //m_good83.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good83.SetTextColor(RGB(0, 0, 0));
        //m_good83.SetFontSize(12);
        //m_fail83.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail83.SetTextColor(RGB(0, 0, 0));
        //m_fail83.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[30] = 0;
        //m_good83.EnableWindow(FALSE);
        //m_fail83.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS83)->EnableWindow(FALSE);
    }
    InitSockStatus(30);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck84()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK84);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[31] = 1;
        //m_good84.EnableWindow();
        //m_fail84.EnableWindow();
        //m_good84.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good84.SetTextColor(RGB(0, 0, 0));
        //m_good84.SetFontSize(12);
        //m_fail84.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail84.SetTextColor(RGB(0, 0, 0));
        //m_fail84.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[31] = 0;
        //m_good84.EnableWindow(FALSE);
        //m_fail84.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS84)->EnableWindow(FALSE);
    }
    InitSockStatus(31);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck91()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK91);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[32] = 1;
        //m_good91.EnableWindow();
        //m_fail91.EnableWindow();
        //m_good91.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good91.SetTextColor(RGB(0, 0, 0));
        //m_good91.SetFontSize(12);
        //m_fail91.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail91.SetTextColor(RGB(0, 0, 0));
        //m_fail91.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[32] = 0;
        //m_good91.EnableWindow(FALSE);
        //m_fail91.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS91)->EnableWindow(FALSE);
    }
    InitSockStatus(32);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck92()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK92);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[33] = 1;
        //m_good92.EnableWindow();
        //m_fail92.EnableWindow();
        //m_good92.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good92.SetTextColor(RGB(0, 0, 0));
        //m_good92.SetFontSize(12);
        //m_fail92.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail92.SetTextColor(RGB(0, 0, 0));
        //m_fail92.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[33] = 0;
        //m_good92.EnableWindow(FALSE);
        //m_fail92.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS92)->EnableWindow(FALSE);
    }
    InitSockStatus(33);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck93()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK93);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[34] = 1;
        //m_good93.EnableWindow();
        //m_fail93.EnableWindow();
        //m_good93.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good93.SetTextColor(RGB(0, 0, 0));
        //m_good93.SetFontSize(12);
        //m_fail93.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail93.SetTextColor(RGB(0, 0, 0));
        //m_fail93.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[34] = 0;
        //m_good93.EnableWindow(FALSE);
        //m_fail93.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS93)->EnableWindow(FALSE);
    }
    InitSockStatus(34);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck94()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK94);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[35] = 1;
        //m_good94.EnableWindow();
        //m_fail94.EnableWindow();
        //m_good94.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good94.SetTextColor(RGB(0, 0, 0));
        //m_good94.SetFontSize(12);
        //m_fail94.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail94.SetTextColor(RGB(0, 0, 0));
        //m_fail94.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[35] = 0;
        //m_good94.EnableWindow(FALSE);
        //m_fail94.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS94)->EnableWindow(FALSE);
    }
    InitSockStatus(35);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck101()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK101);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[36] = 1;
        //m_good101.EnableWindow();
        //m_fail101.EnableWindow();
        //m_good101.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good101.SetTextColor(RGB(0, 0, 0));
        //m_good101.SetFontSize(12);
        //m_fail101.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail101.SetTextColor(RGB(0, 0, 0));
        //m_fail101.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[36] = 0;
        //m_good101.EnableWindow(FALSE);
        //m_fail101.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS101)->EnableWindow(FALSE);
    }
    InitSockStatus(36);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck102()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK102);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[37] = 1;
        //m_good102.EnableWindow();
        //m_fail102.EnableWindow();
        //m_good102.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good102.SetTextColor(RGB(0, 0, 0));
        //m_good102.SetFontSize(12);
        //m_fail102.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail102.SetTextColor(RGB(0, 0, 0));
        //m_fail102.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[37] = 0;
        //m_good102.EnableWindow(FALSE);
        //m_fail102.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS102)->EnableWindow(FALSE);
    }
    InitSockStatus(37);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck103()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK103);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[38] = 1;
        //m_good103.EnableWindow();
        //m_fail103.EnableWindow();
        //m_good103.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good103.SetTextColor(RGB(0, 0, 0));
        //m_good103.SetFontSize(12);
        //m_fail103.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail103.SetTextColor(RGB(0, 0, 0));
        //m_fail103.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[38] = 0;
        //m_good103.EnableWindow(FALSE);
        //m_fail103.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS103)->EnableWindow(FALSE);
    }
    InitSockStatus(38);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedSocketCheck104()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_SOCKET_CHECK104);
    if (pBtn->GetCheck())
    {
        gm_bSocketUsed[39] = 1;
        //m_good104.EnableWindow();
        //m_fail104.EnableWindow();
        //m_good104.SetBkColor(RGB(38, 223, 56), RGB(196, 247, 201), CLabel::Gradient);
        //m_good104.SetTextColor(RGB(0, 0, 0));
        //m_good104.SetFontSize(12);
        //m_fail104.SetBkColor(RGB(238, 48, 32), RGB(249, 177, 172), CLabel::Gradient);
        //m_fail104.SetTextColor(RGB(0, 0, 0));
        //m_fail104.SetFontSize(12);
    }
    else
    {
        gm_bSocketUsed[39] = 0;
        //m_good104.EnableWindow(FALSE);
        //m_fail104.EnableWindow(FALSE);
        //GetDlgItem(IDC_STATUS104)->EnableWindow(FALSE);
    }
    InitSockStatus(39);
    RefreshSocketStatus();
}


void CAutoFPDlg::OnBnClickedBtnLock()
{
    // TODO:  在此添加控件通知处理程序代码
    CString strLock;
    CString strLockBtn;
    GetDlgItemText(IDC_BTN_LOCK, strLockBtn);
    if (strLockBtn.Compare("解锁") == 0)
    {
        GetDlgItemText(IDC_EDIT_LOCK, strLock);
        if (strLock.Compare("0000") != 0)
        {
            return;
        }
        if (gm_bStart || gm_bFinish)
        {
            AfxMessageBox("程序正在运行，请暂停后再解锁。");
            return;
        }
        GetDlgItem(IDC_BTN_POS)->EnableWindow();
        GetDlgItem(IDC_BTN_SETTING)->EnableWindow();
        GetDlgItem(IDC_BTN_CLEAN_DATA)->EnableWindow();
        GetDlgItem(IDC_BTN_CREATE_LOGFILE)->EnableWindow();
        GetDlgItem(IDC_BTN_TRAY_COMPLETE)->EnableWindow();
        //GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow();
        SetDlgItemText(IDC_BTN_LOCK, "锁定");
        SetDlgItemText(IDC_EDIT_LOCK, "");
        AppendLogMessage("解锁");
    }
    else
    {
        GetDlgItem(IDC_BTN_POS)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_CLEAN_DATA)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_CREATE_LOGFILE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BTN_TRAY_COMPLETE)->EnableWindow(FALSE);
        //GetDlgItem(IDC_BTN_CLEAN_SOCKET)->EnableWindow(FALSE);
        SetDlgItemText(IDC_BTN_LOCK, "解锁");
        SetDlgItemText(IDC_EDIT_LOCK, "");
        AppendLogMessage("锁定");
    }
}
void CAutoFPDlg::CleanData()
{
    if (IDYES != AfxMessageBox("确定要清零所有数据吗？", MB_YESNO))
        return;
    gm_nCurrentTotalSize = 0;
    gm_nCurrentOutSize = 0;
    gm_nCurrentInSize = 0;
    gm_nCurrentGoodSize = 0;
    gm_nCurrentFailSize = 0;
    gm_nStockGoodSize = 0;
    gm_nTotalGoodSize = 0;
    gm_nTotalFailSize = 0;
    gm_nTotalCheckFail = 0;
    gm_nTotalFPFail = 0;
    gm_nCurrentStock = 0;
    gm_nTotalInSize = 0;
    gm_nTotalOutSize = 0;
    m_nTime = 0;

    ShowYield(0);
    ShowGoodSize(0);
    ShowFailSize(0, 0);
    CString strTmp;
    strTmp.Format("%d", 0);
    SetDlgItemTextA(IDC_MAIN_UPH, strTmp);
    SetDlgItemTextA(IDC_MAIN_RUN_TIME, strTmp);
    for (int i = 0; i < MAXNUM_OF_PROGRAMMER; i++)
    {
        gm_nModelPutPreference[i] = 0;
        gm_nModelGetPreference[i] = 0;
    }
    //清零每个烧录座的信息
    for (int i = 0; i < MAXNUMOFSOCKET*MAXNUMOFPROGRAMMER; i++)
    {
        gm_sBurningStatus[i / 4].nAdpStatus[i % 4] = 0;//状态清零
        gm_sBurningStatus[i / 4].bEmpty[i % 4] = TRUE;//座子清空
        gm_sBurningStatus[i / 4].nContinueFailSize[i % 4] = 0;//连续NG
        gm_nDisplayFailForSocket[i] = 0;
        gm_nDisplayGoodForSocket[i] = 0;
        DisplayGood(i, gm_nDisplayGoodForSocket[i]);
        DisplayFail(i, gm_nDisplayFailForSocket[i]);
    }

    if (gm_uInfo.nWorkMode != TAPE_TO_TAPE)
        WholeTrayRfresh();
    ChangeTapeIn(0);
    ChangeTapeOut(0);
}

void CAutoFPDlg::OnBnClickedBtnCleanData()
{
    // TODO:  在此添加控件通知处理程序代码
    CleanData();
    AppendLogMessage("数据清零");
}

BOOL CAutoFPDlg::CleanSocket()
{
    AppendLogMessage("执行清空烧录座过程。");
    if (m_bCameraOpenDown == TRUE)
    {
        CloseFramegrabber(m_hv_AcqHandle_Down);
        m_bCameraOpenDown = FALSE;
    }
    OpenFramegrabUp();
    SetUpCameraLightOn();
    Point offset{ 0, 0 };//吸嘴与上相机的偏差
    offset = gm_dataPos.GetPlacePoint("offset_nozzle1");
    Position socketPos{ 0, 0, 0 };//socket坐标
    Position movePos{ 0, 0, 0 };//要移动至的坐标（此处为废料盘坐标）
    Position2 posOffset{ 0, 0, 0 };//测量到的偏差值
    for (int i = 0; i < MAXNUMOFPROGRAMMER*MAXNUMOFSOCKET; i++)
    {
        if (gm_bSocketUsed[i] && TRUE == gm_sBurningStatus[i / MAXNUMOFSOCKET].bEmpty[i%MAXNUMOFSOCKET])
        {
            CString secStr;
            secStr.Format("Socket%d", i);
            socketPos = GetPosForSocket(i);
            int n = 0;
            while (1)
            {
                n++;
                if (n >= 3)
                {
                    //匹配失败，禁用该烧录座
                    gm_sBurningStatus[i / MAXNUMOFSOCKET].bUsed = 0;
                    break;
                }
                if (!CheckModelCylinderPressed(i / MAXNUMOFSOCKET))
                {
                    SetCylinderDown(i / MAXNUMOFSOCKET);
                }
                if (!ZComeBack())
                    return FALSE;
                SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
                SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
                XYMove(socketPos.x, socketPos.y);
                if (!CheckModel(posOffset, "socket"))
                {
                    //如果匹配失败，说明烧录座中有芯片，将其取出
                    if (!ZComeBack())
                        return FALSE;
                    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
                    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
                    XYMove(socketPos.x + offset.x, socketPos.y + offset.y);
                    if (!CheckModelCylinderPressed(i / MAXNUMOFSOCKET))
                    {
                        CString str;
                        str.Format("未检测到%d号气缸下拉信号。", i / MAXNUMOFSOCKET);
                        AfxMessageBox(str);
                        return FALSE;
                    }
                    SzMove(socketPos.z);
                    GetChip(NOZZLE_0);
                    ZComeBack();
                    if (!CheckNegtiveMeter(NOZZLE_0))
                    {
                        SetNozzleVaccumOff(NOZZLE_0);
                        if (IDYES == AfxMessageBox("未吸取到芯片。\r\n是否停止清空过程？", MB_YESNO))
                        {
                            return FALSE;
                        }
                        else
                        {
                            break;
                        }
                    }
                    GetChipPosInScrap(gm_nCurrentFailSize, movePos);
                    if (!ZComeBack())
                        return FALSE;
                    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
                    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
                    XYMove(movePos.x + offset.x, movePos.y + offset.y);
                    SzMove(movePos.z);
                    PutChip(NOZZLE_0);
                    ZComeBack();
                    gm_nCurrentFailSize++;
                    break;
                }
                else
                {
                    ////如果匹配成功则进行坐标调整
                    //socketPos.x = socketPos.x - posOffset.x * XGUIDE / (double)gm_uInfo.nPixelsPM_up;//将像素偏差转换成脉冲偏差
                    //socketPos.y = socketPos.y + posOffset.y * YGUIDE / (double)gm_uInfo.nPixelsPM_up;
                    //if (abs(posOffset.x) < 20 && abs(posOffset.y) < 20)
                    //{
                    //	gm_dataPos.SetPlacePoint((LPCSTR)secStr, socketPos.x, socketPos.y);
                    break;
                    //}
                }
            }
        }
    }

    if (m_bCameraOpenUp)
    {
        CloseFramegrabber(m_hv_AcqHandle_Up);
        m_bCameraOpenUp = FALSE;
    }
    SetUpCameraLightOff();
    return TRUE;
}

void CAutoFPDlg::OnBnClickedBtnCleanSocket()
{
    if (gm_bStart || gm_bFinish)
    {
        gm_bBeep = TRUE;
        AfxBeginThread(BeepThreadProc, NULL);
        AfxMessageBox("请先暂停烧录过程");
        gm_bBeep = FALSE;
        return;
    }
    if (IDOK != AfxMessageBox("确定要清空烧录座？", MB_OKCANCEL))
        return;
    if (!CheckDoorOpen())
    {
        gm_bBeep = TRUE;
        AfxBeginThread(BeepThreadProc, NULL);
        AfxMessageBox("请关闭前门");
        gm_bBeep = FALSE;
        return;
    }

    CleanSocket();
    AfxMessageBox("清空过程结束");
}


void CAutoFPDlg::OnBnClickedBtnExcelbox()
{
    CString str;
    int nTemp = m_nTime / 60;
    int nSecond = m_nTime % 60;
    int nHour = nTemp / 60;
    int nMinute = nTemp % 60;
    str.Format("%02d:%02d:%02d", nHour, nMinute, nSecond);
    gm_sdInfo.strRunTime = str;
    if (m_nTime == 0)
    {
        gm_sdInfo.nUPH = 0;
    }
    else
    {
        gm_sdInfo.nUPH = (gm_nTotalGoodSize + gm_nTotalFailSize) / ((float)m_nTime / 3600);
    }
    time_t tt = time(NULL);//这句返回的只是一个时间cuo
    tm* t = localtime(&tt);
    //printf("%d-%02d-%02d %02d:%02d:%02d\n",
    //	t->tm_year + 1900,
    //	t->tm_mon + 1,
    //	t->tm_mday,
    //	t->tm_hour,
    //	t->tm_min,
    //	t->tm_sec);
    str.Format("%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

    //_stprintf_s(gm_sdInfo.strStartTime, _MAX_PATH, _T("%02d:%02d:%02d"), t->tm_hour, t->tm_min, t->tm_sec);

    m_dlgExcel.ShowWindow(SW_SHOW);
}


void CAutoFPDlg::OnBnClickedBtnCreateLogfile()
{
    CreateLogfile();
}

void CAutoFPDlg::CreateLogfile()
{
    CLogFile logFile;
    CString strLogFile = "c:\\S100\\Log\\";
    CTime time;
    time = CTime::GetCurrentTime();
    CString str;
    str.Format("%s-%d%02d%02d-%02d%02d.txt", gm_sdInfo.strWorkNo, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
    strLogFile = strLogFile + str;//日志完整路径
    logFile.SetPath(strLogFile);

    logFile.WriteSocketInfo();
    str.Format("生成日志文件：c:\\S100\\Log\\%s-%d%02d%02d-%02d%02d.txt", gm_sdInfo.strWorkNo, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
    AppendLogMessage(str);
    AfxMessageBox("日志生成成功！");

}


void CAutoFPDlg::OnBnClickedCheckProgram1()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM1);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(0);
        SetDlgItemTextA(IDC_CHECK_PROGRAM1, "上拉");
    }
    else
    {
        SetCylinderUp(0);
        SetDlgItemTextA(IDC_CHECK_PROGRAM1, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram2()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM2);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(1);
        SetDlgItemTextA(IDC_CHECK_PROGRAM2, "上拉");
    }
    else
    {
        SetCylinderUp(1);
        SetDlgItemTextA(IDC_CHECK_PROGRAM2, "下拉");
    }
}




void CAutoFPDlg::OnBnClickedCheckProgram3()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM3);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(2);
        SetDlgItemTextA(IDC_CHECK_PROGRAM3, "上拉");
    }
    else
    {
        SetCylinderUp(2);
        SetDlgItemTextA(IDC_CHECK_PROGRAM3, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram4()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM4);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(3);
        SetDlgItemTextA(IDC_CHECK_PROGRAM4, "上拉");
    }
    else
    {
        SetCylinderUp(3);
        SetDlgItemTextA(IDC_CHECK_PROGRAM4, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram5()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM5);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(4);
        SetDlgItemTextA(IDC_CHECK_PROGRAM5, "上拉");
    }
    else
    {
        SetCylinderUp(4);
        SetDlgItemTextA(IDC_CHECK_PROGRAM5, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram6()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM6);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(5);
        SetDlgItemTextA(IDC_CHECK_PROGRAM6, "上拉");
    }
    else
    {
        SetCylinderUp(5);
        SetDlgItemTextA(IDC_CHECK_PROGRAM6, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram7()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM7);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(6);
        SetDlgItemTextA(IDC_CHECK_PROGRAM7, "上拉");
    }
    else
    {
        SetCylinderUp(6);
        SetDlgItemTextA(IDC_CHECK_PROGRAM7, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram8()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM8);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(7);
        SetDlgItemTextA(IDC_CHECK_PROGRAM8, "上拉");
    }
    else
    {
        SetCylinderUp(7);
        SetDlgItemTextA(IDC_CHECK_PROGRAM8, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram9()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM9);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(8);
        SetDlgItemTextA(IDC_CHECK_PROGRAM9, "上拉");
    }
    else
    {
        SetCylinderUp(8);
        SetDlgItemTextA(IDC_CHECK_PROGRAM9, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedCheckProgram10()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_PROGRAM10);
    if (pBtn->GetCheck())
    {
        SetCylinderDown(9);
        SetDlgItemTextA(IDC_CHECK_PROGRAM10, "上拉");
    }
    else
    {
        SetCylinderUp(9);
        SetDlgItemTextA(IDC_CHECK_PROGRAM10, "下拉");
    }
}


void CAutoFPDlg::OnBnClickedBtnGohome()
{
    if (!ZComeBack())
        return;
    SetAxisSpeed(XAXIS, 100, POSITION_SPEED);
    SetAxisSpeed(YAXIS, 100, POSITION_SPEED);
    int oldSpeed = gm_uInfo.xySpeed;
    gm_uInfo.xySpeed = 1;
    XYMove(0, 0);
    gm_uInfo.xySpeed = oldSpeed;
}


void CAutoFPDlg::OnBnClickedBtnResetStocktray()
{
    gm_nCurrentStock = 0;
    gm_nStockGoodSize = 0;
    AfxMessageBox("重置成功!");
}

//烧录NG料
//NG料数量作为临时进料数量
//烧录OK料加入OK料总量
void CAutoFPDlg::OnBnClickedBtnFpNg()
{
    if (IDYES != AfxMessageBox("是否烧录NG料？", MB_YESNO))
        return;

    if (gm_nTotalInSize < gm_nTotalFailSize)
    {
        AfxMessageBox("参数错误");
        return;
    }
    //总进料量减去总NG料，也就是对NG料二次烧录
    gm_nTotalInSize -= gm_nTotalFailSize;
    gm_nTotalFailSize = 0;
    gm_nTotalFPFail = 0;
    gm_nTotalCheckFail = 0;
    gm_nCurrentFailSize = 0;
    ShowFailSize(gm_nTotalFPFail, gm_nTotalCheckFail);

    for (int i = 0; i < MAXNUM_OF_PROGRAMMER; i++)
    {
        gm_nModelPutPreference[i] = 0;
        gm_nModelGetPreference[i] = 0;
    }
    //清零每个烧录座的信息
    for (int i = 0; i < MAXNUMOFSOCKET*MAXNUMOFPROGRAMMER; i++)
    {
        gm_sBurningStatus[i / MAXNUMOFSOCKET].nAdpStatus[i % MAXNUMOFSOCKET] = FP_READY;//状态清零
        gm_sBurningStatus[i / MAXNUMOFSOCKET].bEmpty[i % MAXNUMOFSOCKET] = TRUE;//座子清空
    }
}


void CAutoFPDlg::OnBnClickedCheckUplight()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_UPLIGHT);
    if (pBtn->GetCheck())
    {
        SetUpCameraLightOn();
        SetDlgItemTextA(IDC_CHECK_UPLIGHT, "关闭上光源");
    }
    else
    {
        SetUpCameraLightOff();
        SetDlgItemTextA(IDC_CHECK_UPLIGHT, "打开上光源");
    }
}

/*
*LED_Check,DEV1_RGY,000:000:000:000
*LED_Check,DEV2_RGY,000:000:000:000
*LED_Check,DEV3_RGY,000:000:000:000
*LED_Check,DEV4_RGY,000:000:000:000
*/
//枚举串口
void CAutoFPDlg::EnumSerials()
{
    int m_nSerialPortNum(0);// 串口计数
    CString          strSerialList[256];  // 临时定义 256 个字符串组
    CArray<SSerInfo, SSerInfo&> asi;
    EnumSerialPorts(asi, TRUE);// 参数为 TRUE 时枚举当前可以打开的串口，
    // 否则枚举所有串口
    m_nSerialPortNum = asi.GetSize();
    for (int i = 0; i < m_nSerialPortNum; i++)
    {
        CString str = asi[i].strFriendlyName;
        int nStart = 0;
        nStart = str.Find("COM");
        str = str.Mid(nStart, 4);
        m_cmbSocketPort.AddString(str);
    }
}

void CAutoFPDlg::OnBnClickedBtnReadCom()
{
    //打开烧录状态监测串口
    configSerial_.BaudRate = CBR_115200;
    //int nSocketPort = 4;
    //CString str;
    //str.Format("\\\\.\\COM%d", nSocketPort);
    CString str;
    //int index = m_cmbPort.GetCurSel();
    m_cmbSocketPort.GetLBText(m_cmbSocketPort.GetCurSel(), str);
    if (!m_bSocketConnect)
    {
        if (!m_SocketPort.openPort(configSerial_, str))
        {
            AfxMessageBox("烧录状态监测串口打开失败。");
        }
        else
        {
            SetTimer(2, 50, NULL);
            m_bSocketConnect = TRUE;
            SetDlgItemText(IDC_BTN_READ_COM, "已打开");
        }
    }
    else
    {
        if (!m_SocketPort.closePort())
        {
            AfxMessageBox("串口关闭失败。");
        }
        else
        {
            KillTimer(2);
            m_bSocketConnect = FALSE;
            SetDlgItemText(IDC_BTN_READ_COM, "已关闭");
        }
    }

}
void CAutoFPDlg::CheckSocketStatus()
{
    int  nResult = 0;
    char mess[256];
    unsigned int lenBuff = 256;
    unsigned long lenMessage;
    CString outPut;
    if (m_SocketPort.read_scc(mess, lenBuff, lenMessage))
    {
        if (lenMessage > 0)
        {
            outPut = mess;
            //AppendLogMessage(mess);
            int nDev1 = outPut.Find("*LED_Check,DEV1_RGY,");
            if (nDev1 >= 0)
            {
                CString strTemp11 = outPut.Mid(nDev1 + sizeof("*LED_Check,DEV1_RGY,") - 1, 3);
                if (strTemp11.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev11.bYellow = TRUE;
                    gm_sBurningStatus[0].nAdpStatus[0] = FP_PROGM;
                }
                else if (strTemp11.Compare("010") == 0)
                {
                    if (m_ledDev11.bYellow)
                    {
                        m_ledDev11.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[0] = FP_OK;
                    }
                }
                else if (strTemp11.Compare("100") == 0)
                {
                    if (m_ledDev11.bYellow)
                    {
                        m_ledDev11.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[0] = FP_NG;
                    }
                }
                CString strTemp12 = outPut.Mid(nDev1 + sizeof("*LED_Check,DEV1_RGY,") - 1 + 4, 3);
                if (strTemp12.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev12.bYellow = TRUE;
                    gm_sBurningStatus[0].nAdpStatus[1] = FP_PROGM;
                }
                else if (strTemp12.Compare("010") == 0)
                {
                    if (m_ledDev12.bYellow)
                    {
                        m_ledDev12.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[1] = FP_OK;
                    }
                }
                else if (strTemp12.Compare("100") == 0)
                {
                    if (m_ledDev12.bYellow)
                    {
                        m_ledDev12.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[1] = FP_NG;
                    }
                }
                CString strTemp13 = outPut.Mid(nDev1 + sizeof("*LED_Check,DEV1_RGY,") - 1 + 8, 3);
                if (strTemp13.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev13.bYellow = TRUE;
                    gm_sBurningStatus[0].nAdpStatus[2] = FP_PROGM;
                }
                else if (strTemp13.Compare("010") == 0)
                {
                    if (m_ledDev13.bYellow)
                    {
                        m_ledDev13.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[2] = FP_OK;
                    }
                }
                else if (strTemp13.Compare("100") == 0)
                {
                    if (m_ledDev13.bYellow)
                    {
                        m_ledDev13.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[2] = FP_NG;
                    }
                }
                CString strTemp14 = outPut.Mid(nDev1 + sizeof("*LED_Check,DEV1_RGY,") - 1 + 12, 3);
                if (strTemp14.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev14.bYellow = TRUE;
                    gm_sBurningStatus[0].nAdpStatus[3] = FP_PROGM;
                }
                else if (strTemp14.Compare("010") == 0)
                {
                    if (m_ledDev14.bYellow)
                    {
                        m_ledDev14.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[3] = FP_OK;
                    }
                }
                else if (strTemp14.Compare("100") == 0)
                {
                    if (m_ledDev14.bYellow)
                    {
                        m_ledDev14.bYellow = FALSE;
                        gm_sBurningStatus[0].nAdpStatus[3] = FP_NG;
                    }
                }
            }

            int nDev2 = outPut.Find("*LED_Check,DEV2_RGY,");
            if (nDev2 >= 0)
            {
                CString strTemp21 = outPut.Mid(nDev2 + sizeof("*LED_Check,DEV2_RGY,") - 1, 3);
                if (strTemp21.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev21.bYellow = TRUE;
                    gm_sBurningStatus[1].nAdpStatus[0] = FP_PROGM;
                }
                else if (strTemp21.Compare("010") == 0)
                {
                    if (m_ledDev21.bYellow)
                    {
                        m_ledDev21.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[0] = FP_OK;
                    }
                }
                else if (strTemp21.Compare("100") == 0)
                {
                    if (m_ledDev21.bYellow)
                    {
                        m_ledDev21.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[0] = FP_NG;
                    }
                }

                CString strTemp22 = outPut.Mid(nDev2 + sizeof("*LED_Check,DEV2_RGY,") - 1 + 4, 3);
                if (strTemp22.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev22.bYellow = TRUE;
                    gm_sBurningStatus[1].nAdpStatus[1] = FP_PROGM;
                }
                else if (strTemp22.Compare("010") == 0)
                {
                    if (m_ledDev22.bYellow)
                    {
                        m_ledDev22.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[1] = FP_OK;
                    }
                }
                else if (strTemp22.Compare("100") == 0)
                {
                    if (m_ledDev22.bYellow)
                    {
                        m_ledDev22.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[1] = FP_NG;
                    }
                }
                CString strTemp23 = outPut.Mid(nDev2 + sizeof("*LED_Check,DEV2_RGY,") - 1 + 8, 3);
                if (strTemp23.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev23.bYellow = TRUE;
                    gm_sBurningStatus[1].nAdpStatus[2] = FP_PROGM;
                }
                else if (strTemp23.Compare("010") == 0)
                {
                    if (m_ledDev23.bYellow)
                    {
                        m_ledDev23.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[2] = FP_OK;
                    }
                }
                else if (strTemp23.Compare("100") == 0)
                {
                    if (m_ledDev23.bYellow)
                    {
                        m_ledDev23.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[2] = FP_NG;
                    }
                }
                CString strTemp24 = outPut.Mid(nDev2 + sizeof("*LED_Check,DEV2_RGY,") - 1 + 12, 3);
                if (strTemp24.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev24.bYellow = TRUE;
                    gm_sBurningStatus[1].nAdpStatus[3] = FP_PROGM;
                }
                else if (strTemp24.Compare("010") == 0)
                {
                    if (m_ledDev24.bYellow)
                    {
                        m_ledDev24.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[3] = FP_OK;
                    }
                }
                else if (strTemp24.Compare("100") == 0)
                {
                    if (m_ledDev24.bYellow)
                    {
                        m_ledDev24.bYellow = FALSE;
                        gm_sBurningStatus[1].nAdpStatus[3] = FP_NG;
                    }
                }
            }
            int nDev3 = outPut.Find("*LED_Check,DEV3_RGY,");
            if (nDev3 >= 0)
            {
                CString strTemp31 = outPut.Mid(nDev3 + sizeof("*LED_Check,DEV3_RGY,") - 1, 3);
                if (strTemp31.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev31.bYellow = TRUE;
                    gm_sBurningStatus[2].nAdpStatus[0] = FP_PROGM;
                }
                else if (strTemp31.Compare("010") == 0)
                {
                    if (m_ledDev31.bYellow)
                    {
                        m_ledDev31.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[0] = FP_OK;
                    }
                }
                else if (strTemp31.Compare("100") == 0)
                {
                    if (m_ledDev31.bYellow)
                    {
                        m_ledDev31.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[0] = FP_NG;
                    }
                }
                CString strTemp32 = outPut.Mid(nDev3 + sizeof("*LED_Check,DEV3_RGY,") - 1 + 4, 3);
                if (strTemp32.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev32.bYellow = TRUE;
                    gm_sBurningStatus[2].nAdpStatus[1] = FP_PROGM;
                }
                else if (strTemp32.Compare("010") == 0)
                {
                    if (m_ledDev32.bYellow)
                    {
                        m_ledDev32.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[1] = FP_OK;
                    }
                }
                else if (strTemp32.Compare("100") == 0)
                {
                    if (m_ledDev32.bYellow)
                    {
                        m_ledDev32.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[1] = FP_NG;
                    }
                }
                CString strTemp33 = outPut.Mid(nDev3 + sizeof("*LED_Check,DEV3_RGY,") - 1 + 8, 3);
                if (strTemp33.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev33.bYellow = TRUE;
                    gm_sBurningStatus[2].nAdpStatus[2] = FP_PROGM;
                }
                else if (strTemp33.Compare("010") == 0)
                {
                    if (m_ledDev33.bYellow)
                    {
                        m_ledDev33.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[2] = FP_OK;
                    }
                }
                else if (strTemp33.Compare("100") == 0)
                {
                    if (m_ledDev33.bYellow)
                    {
                        m_ledDev33.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[2] = FP_NG;
                    }
                }
                CString strTemp34 = outPut.Mid(nDev3 + sizeof("*LED_Check,DEV3_RGY,") - 1 + 12, 3);
                if (strTemp34.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev34.bYellow = TRUE;
                    gm_sBurningStatus[2].nAdpStatus[3] = FP_PROGM;
                }
                else if (strTemp34.Compare("010") == 0)
                {
                    if (m_ledDev34.bYellow)
                    {
                        m_ledDev34.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[3] = FP_OK;
                    }
                }
                else if (strTemp34.Compare("100") == 0)
                {
                    if (m_ledDev34.bYellow)
                    {
                        m_ledDev34.bYellow = FALSE;
                        gm_sBurningStatus[2].nAdpStatus[3] = FP_NG;
                    }
                }
            }

            int nDev4 = outPut.Find("*LED_Check,DEV4_RGY,");
            if (nDev4 >= 0)
            {
                CString strTemp41 = outPut.Mid(nDev4 + sizeof("*LED_Check,DEV4_RGY,") - 1, 3);
                if (strTemp41.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev41.bYellow = TRUE;
                    gm_sBurningStatus[3].nAdpStatus[0] = FP_PROGM;
                }
                else if (strTemp41.Compare("010") == 0)
                {
                    if (m_ledDev41.bYellow)
                    {
                        m_ledDev41.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[0] = FP_OK;
                    }
                }
                else if (strTemp41.Compare("100") == 0)
                {
                    if (m_ledDev41.bYellow)
                    {
                        m_ledDev41.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[0] = FP_NG;
                    }
                }
                CString strTemp42 = outPut.Mid(nDev4 + sizeof("*LED_Check,DEV4_RGY,") - 1 + 4, 3);
                if (strTemp42.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev42.bYellow = TRUE;
                    gm_sBurningStatus[3].nAdpStatus[1] = FP_PROGM;
                }
                else if (strTemp42.Compare("010") == 0)
                {
                    if (m_ledDev42.bYellow)
                    {
                        m_ledDev42.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[1] = FP_OK;
                    }
                }
                else if (strTemp42.Compare("100") == 0)
                {
                    if (m_ledDev42.bYellow)
                    {
                        m_ledDev42.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[1] = FP_NG;
                    }
                }
                CString strTemp43 = outPut.Mid(nDev4 + sizeof("*LED_Check,DEV4_RGY,") - 1 + 8, 3);
                if (strTemp43.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev43.bYellow = TRUE;
                    gm_sBurningStatus[3].nAdpStatus[2] = FP_PROGM;
                }
                else if (strTemp43.Compare("010") == 0)
                {
                    if (m_ledDev43.bYellow)
                    {
                        m_ledDev43.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[2] = FP_OK;
                    }
                }
                else if (strTemp43.Compare("100") == 0)
                {
                    if (m_ledDev43.bYellow)
                    {
                        m_ledDev43.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[2] = FP_NG;
                    }
                }
                CString strTemp44 = outPut.Mid(nDev4 + sizeof("*LED_Check,DEV4_RGY,") - 1 + 12, 3);
                if (strTemp44.Compare("001") == 0)//黄灯亮
                {
                    m_ledDev44.bYellow = TRUE;
                    gm_sBurningStatus[3].nAdpStatus[3] = FP_PROGM;
                }
                else if (strTemp44.Compare("010") == 0)
                {
                    if (m_ledDev44.bYellow)
                    {
                        m_ledDev44.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[3] = FP_OK;
                    }
                }
                else if (strTemp44.Compare("100") == 0)
                {
                    if (m_ledDev44.bYellow)
                    {
                        m_ledDev44.bYellow = FALSE;
                        gm_sBurningStatus[3].nAdpStatus[3] = FP_NG;
                    }
                }
            }
        }
    }
}

void CAutoFPDlg::OnBnClickedBtnTrayComplete()
{
    if (gm_bStart || gm_bFinish)
    {
        MessageBox("请先暂停程序！");
        return;
    }
    TrayComplete();
    gm_nCurrentInSize = 0;
    gm_nCurrentOutSize = 0;
    gm_nCurrentGoodSize = 0;
    //SetRunStatus("换盘中…");
    WholeTrayRfresh();

}


void CAutoFPDlg::OnBnClickedBtnReadInifile()
{
    if (gm_bStart || gm_bFinish)
    {
        AfxMessageBox("机器正在运行，无法载入工程!");
        return;
    }
    CFileDialog saveprj(TRUE,
        "*.log",
        "*.log",
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Project file (*.log)|*.dat|All file(*.*)|*.*||"
    );
    CString s;
    saveprj.m_ofn.lpstrTitle = "Open User Data File";
    if (saveprj.DoModal() != IDOK)
        return;

    s = saveprj.GetPathName();

    CFileFind finder;
    BOOL bFind = finder.FindFile(s);
    if (!bFind)
    {
        AfxMessageBox("该数据文件不存在!");
        finder.Close();
        return;
    }
    finder.Close();

    CString ext = s.Right(3);
    ext.MakeLower();
    if (ext.Compare("log") != 0)
    {
        AfxMessageBox("无效的用户数据文件!");
        return;
    }
    //SetWindowText(s);
    gm_strStatusLog = s;
    AppendLogMessage(s);
    SetTimer(3, 200, NULL);
}
//艾科烧录器，通过检查烧录日志判断烧录信号
void CAutoFPDlg::CheckStatusLogFile()
{
    CIniFile inf;
    inf.SetIniFileName(gm_strStatusLog);

    CString strProgrammer, strSocket, str;
    for (int nProgrammer = 0; nProgrammer < 8; nProgrammer++)
    {
        for (int nSocket = 0; nSocket < 4; nSocket++)
        {
            strProgrammer.Format("Programmer%d", nProgrammer + 1);
            strSocket.Format("Socket%d", nSocket + 1);
            inf.GetString(strProgrammer, strSocket, str, "Cancel");
            if (str.Compare("Running") == 0)
            {
                gm_sBurningStatus[nProgrammer].nAdpStatus[nSocket] = 1;
            }
            else if (str.Compare("Success") == 0)
            {
                gm_sBurningStatus[nProgrammer].nAdpStatus[nSocket] = 2;
            }
            else if (str.Compare("Failure") == 0)
            {
                gm_sBurningStatus[nProgrammer].nAdpStatus[nSocket] = 3;
            }
            //else if (str.Compare("Cancel") == 0)
            //{
            //	gm_sBurningStatus[nProgrammer].nAdpStatus[nSocket] = 0;
            //}
        }
    }
}




void CAutoFPDlg::OnBnClickedCheckDownlight()
{
    CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_DOWNLIGHT);
    if (pBtn->GetCheck())
    {
        SetDownCameraLightOn();
        SetDlgItemTextA(IDC_CHECK_DOWNLIGHT, "关闭下光源");
    }
    else
    {
        SetDownCameraLightOff();
        SetDlgItemTextA(IDC_CHECK_DOWNLIGHT, "打开下光源");
    }
}


void CAutoFPDlg::OnBnClickedBtnTapecameraCreate()
{
    if (!CreateTapeModel())
        MessageBox("Fail");
    else
        MessageBox("ok");
}


void CAutoFPDlg::OnBnClickedBtnTapecamearCheck()
{
    if (!CheckTapeModel())
        MessageBox("Fail");
    else
        MessageBox("ok");
}
