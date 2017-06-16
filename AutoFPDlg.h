// AutoFPDlg.h : header file
//

#if !defined(AFX_AUTOFPDLG_H__9F881EEB_7F52_4E66_AF60_E11C170BE538__INCLUDED_)
#define AFX_AUTOFPDLG_H__9F881EEB_7F52_4E66_AF60_E11C170BE538__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//"protected: long __thiscall CAutoFPDlg::OnBurnAgain(unsigned int,long)" 

/////////////////////////////////////////////////////////////////////////////
// CAutoFPDlg dialog
//#include "Afxcoll.h"
#include "HalconCpp.h"	
using namespace HalconCpp;
#include "GxIAPI.h"
#include "DxImageProc.h"
#include "P800IspDlg.h"
#include "ExcelDataDlg.h"
#include "EnumSerial.h"
///< 错误提示函数宏定义
#define  GX_VERIFY(emStatus) \
	if(emStatus != GX_STATUS_SUCCESS) \
								{\
								ShowErrorString(emStatus); \
								return;\
								} 
///<  判断返回值宏定义
#define VERIFY_STATUS_RET(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
								   return emStatus;\
                                   };
#define WM_SNAP_CHANGE		(WM_USER + 200)

#include "Label.h"
#include "XPGroupBox.h"
#include "FOObutton.h"
#include "roundbuttonstyle.h"
#include "roundbutton2.h"
#include "SerialCtl.h"
#include "SocketManager.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "json\json.h"
#include "comidaqctrl1.h"

#define WM_CHECK_STATUS (WM_USER+88)
#define WM_BURN_AGAIN (WM_USER+90)
#define MAX_CONNECTION		10
const GUID NewGuid = { 0x310f4539L, 0xd625, 0x492e, {0x91, 0x56, 0xd1, 0xe4, 0x36, 0x48, 0x56, 0xa4} };

const int MASTER_DATA = 100;
const int SLAVE_DATA = 99;

const  BYTE MT_PBINIT = 0x71;
const  BYTE MT_PBPOS = 0x72;
const  BYTE MT_CONNECTED = 0x73;
const  BYTE MT_CHECKSUM = 0x74;
const  BYTE MT_ADAPTER = 0x75;
const  BYTE MT_DEVICE = 0x76;
const  BYTE MT_RESULT = 0x77;
const  BYTE MT_DEVLIST = 0x78;
const  BYTE MT_FILE_CHECKSUM = 0x79;
const  INT CMD_START_PROG = 7;

#define	WM_RECVDATA		WM_USER+1
struct RECVPARAM
{
    SOCKET sock;
    HWND hwnd;
};
struct LEDSTATUS
{
    BOOL bRed;
    BOOL bGreen;
    BOOL bYellow;
};
class CAutoFPDlg : public CDialog
{
private://各信号灯状态
    LEDSTATUS m_ledDev11{ 0,0,0 };
    LEDSTATUS m_ledDev12{ 0,0,0 };
    LEDSTATUS m_ledDev13{ 0,0,0 };
    LEDSTATUS m_ledDev14{ 0,0,0 };

    LEDSTATUS m_ledDev21{ 0,0,0 };
    LEDSTATUS m_ledDev22{ 0,0,0 };
    LEDSTATUS m_ledDev23{ 0,0,0 };
    LEDSTATUS m_ledDev24{ 0,0,0 };

    LEDSTATUS m_ledDev31{ 0,0,0 };
    LEDSTATUS m_ledDev32{ 0,0,0 };
    LEDSTATUS m_ledDev33{ 0,0,0 };
    LEDSTATUS m_ledDev34{ 0,0,0 };

    LEDSTATUS m_ledDev41{ 0,0,0 };
    LEDSTATUS m_ledDev42{ 0,0,0 };
    LEDSTATUS m_ledDev43{ 0,0,0 };
    LEDSTATUS m_ledDev44{ 0,0,0 };
    BOOL m_bSocketConnect;
public:
    Json::Reader json_reader;
    Json::Value json_send;
    Json::Value json_recv;
    Json::StyledWriter json_fast_writer;
    CMenu m_Menu;
    BOOL m_bProgrammer;
    BOOL InitSocket();
    static DWORD WINAPI RecvProc(LPVOID lpParameter);
    void ProccessRequest(const char* httpMessage, SOCKET skCommu);
private:
    SOCKET m_serversocket;  //其实就是一个unsig int类型，用来记录已经建立或者尚未建立的套接字号  
    SOCKET m_clientsocket;
    SOCKET m_socket;
    SOCKET m_client;
public:
    //////////////////Halcon 参数/////////////////////////////////
public:
    HWindow m_Window;
    HTuple  m_hv_AcqHandle_Up, m_hv_AcqHandle_Down, m_hv_AcqHandle_Tape;
    HObject m_ho_Image, m_ho_ROI_Search_Domain, m_ho_ROI_Model_Domain;
    HObject m_ho_ImageMean, m_ho_Region, m_ho_ConnectionRegions, m_ho_RegionUnion, m_ho_RegionDilation;
    HObject m_ho_ImagePart;
    HTuple m_hv_Width, m_hv_Height;
    HObject m_ho_ROI_0, m_ho_ImageReduced;
    HObject m_ho_ModelImages, m_ho_ModelRegions, m_ho_ModelContours, m_ho_ImageReducedSearch;
    HTuple m_hv_ModelID, m_hv_ModelID_Tape;
    HTuple m_hv_Row, m_hv_Column, m_hv_Angle, m_hv_Score;
    HTuple m_hv_CheckRow, m_hv_CheckColumn, m_hv_CheckAngle, m_hv_CheckScore;
public:
    void OpenFramegrabUp();
    void OpenFramegrabDown();
    void OpenTapeCamera();
    BOOL CreateTapeModel();
    BOOL CheckTapeModel();
    BOOL CheckModel(Position2 &posOffset, LPCTSTR place);
    double m_nOffsetX;
    double m_nOffsetY;
    double m_nOffsetAngle;
    CString m_strCameraNum;
    CString m_strCameraVersion;
    BOOL m_bCameraOpenUp;
    BOOL m_bCameraOpenDown;
    BOOL m_bCameraOpenTape;
    BOOL m_bTimerX;
    BOOL m_bTimerY;
    BOOL m_bTimerZ;
    ///////////////////////////////////////////////////////////////////////////
public:
    void SendSimpleCmd(BYTE cmd);
    void SendComplexCmd(BYTE cmd, CString s);
    void StartOneProgrammer(BYTE nIndex);

    //void SetModuleCheck(int nIndex,BOOL bCheck);
    int Str2Hex(CString str, char *data);

    //serial port
    BOOL SendCommand(LPCSTR command);
    BOOL SendCommandToTray(LPCSTR command);
    int str_to_hex(char* string, unsigned char* cbuf, int len);
    void hex_to_str(char* ptr, char* buf, int len);
    char CAutoFPDlg::HexChar(char c);
    BOOL ReadTrayReadySignal();
    BOOL TrayComplete();
    BOOL CheckTapeIn();
    BOOL CheckTapeOut();
    BOOL TapeInOk();
    BOOL TapeInDisable();
    BOOL TapeOutDisable();
    BOOL TapeOutOk();
    BOOL TapeOutBackOk();
    BOOL TapeInGo();
    BOOL TapeOutGo();
    BOOL TapeInTune();
    BOOL TapeOutTune();
    BOOL TapeInDir(BOOL bPos);
    BOOL TapeOutDir(BOOL bPos);
    BOOL TapeInGoDisable();
    BOOL TapeOutGoDisable();
    BOOL TapeInTuneStep(float step);
    BOOL TapeOutTuneStep(float step);
    BOOL TapeInInit();
    BOOL TapeOutInit();
    void ShowTapeInState(int nState);
    BOOL StartPLC();
    BOOL StartTrayPLC();
    BOOL ContinueTrayPLC();
    BOOL StopPLC();
    BOOL StopTrayPLC();
    BOOL CheckOutTape();
    BOOL ContinueTestTray();
    BOOL SetTapeInSpeed(float speed);
    BOOL SetTapeOutSpeed(float speed);
    BOOL SetTapeInStep(float step);
    BOOL SetTapeOutStep(float step);
    BOOL SetCoverTime(int nTime);
public:
    CRoundButtonStyle m_tButtonStyle;
    int m_nTime;
    BOOL EnumDevices();
    void ExtractInstanceId(LPCTSTR insStr, CString &result);
    int m_deviceCount;
    GUID *m_lpGuid;
    GUID m_guid;
    BYTE *m_pTrayStatus;
    CRect m_rtng;
    SerialCtl m_port;
    SerialCtl m_TrayPort;
    SerialCtl m_SocketPort;
    DCB configSerial_;
private:
    CSocketManager m_SocketManager[MAX_CONNECTION];//所有的socket管理器
    CSocketManager* m_pCurServer;//当前操作的socket
public:
    void PickNextAvailable();
    bool StartServer();
    void SendInfo(LPCSTR info);
    // Construction
public:
    CAutoFPDlg(CWnd* pParent = NULL);	// standard constructor
    void DrawTray(CPaintDC &dc);
    //void DrawNGTray(CPaintDC &dc);
    void ChangeTrayStatus(int currentIndex, BYTE status);
    void SetSocketInfo(int sock, LPCSTR info);
    void ShowGoodSize(int size);
    void ShowFailSize(int fpFail, int checkFail);
    void ShowYield(float yield);
    void SetRunStatus(LPCSTR info, int type = 0);
    void EnableRun(BOOL bEnabled);
    void DisplayGood(int socket, int size);
    void DisplayFail(int socket, int size);
    void EnableFinish(BOOL enabled);
    void WholeTrayRfresh();
    void InitSockStatus(UINT nSocket);

    CString  GetCurrentProgInfo(int nIndex);
    void CheckSocketStatus();
    void CheckStatusLogFile();

    void ChangeTapeIn(int nCount);
    void ChangeTapeOut(int nCount);
    void ShowTapeOut(int type, BOOL bShow);
    void ShowTapeIn(int type, BOOL bShow);
    // Dialog Data
        //{{AFX_DATA(CAutoFPDlg)
    enum { IDD = IDD_AUTOFP_DIALOG };
    CButton	m_btnCamera;
    CLabel	m_titleTapeIn;
    CLabel	m_titleTapeOut;
    CLabel	m_lbTapeOut;
    CLabel	m_lbTapeIn;
    CStatic	m_wndTrayNG;

    CLabel	m_fail11;
    CLabel	m_fail12;
    CLabel	m_fail13;
    CLabel	m_fail14;
    CLabel	m_good11;
    CLabel	m_good12;
    CLabel	m_good13;
    CLabel	m_good14;

    CLabel	m_fail21;
    CLabel	m_fail22;
    CLabel	m_fail23;
    CLabel	m_fail24;
    CLabel	m_good21;
    CLabel	m_good22;
    CLabel	m_good23;
    CLabel	m_good24;

    CLabel	m_fail31;
    CLabel	m_fail32;
    CLabel	m_fail33;
    CLabel	m_fail34;
    CLabel	m_good31;
    CLabel	m_good32;
    CLabel	m_good33;
    CLabel	m_good34;

    CLabel	m_fail41;
    CLabel	m_fail42;
    CLabel	m_fail43;
    CLabel	m_fail44;
    CLabel	m_good41;
    CLabel	m_good42;
    CLabel	m_good43;
    CLabel	m_good44;

    CLabel	m_fail51;
    CLabel	m_fail52;
    CLabel	m_fail53;
    CLabel	m_fail54;
    CLabel	m_good51;
    CLabel	m_good52;
    CLabel	m_good53;
    CLabel	m_good54;

    CLabel	m_fail61;
    CLabel	m_fail62;
    CLabel	m_fail63;
    CLabel	m_fail64;
    CLabel	m_good61;
    CLabel	m_good62;
    CLabel	m_good63;
    CLabel	m_good64;

    CLabel	m_fail71;
    CLabel	m_fail72;
    CLabel	m_fail73;
    CLabel	m_fail74;
    CLabel	m_good71;
    CLabel	m_good72;
    CLabel	m_good73;
    CLabel	m_good74;

    CLabel	m_fail81;
    CLabel	m_fail82;
    CLabel	m_fail83;
    CLabel	m_fail84;
    CLabel	m_good81;
    CLabel	m_good82;
    CLabel	m_good83;
    CLabel	m_good84;

    CLabel	m_fail91;
    CLabel	m_fail92;
    CLabel	m_fail93;
    CLabel	m_fail94;
    CLabel	m_good91;
    CLabel	m_good92;
    CLabel	m_good93;
    CLabel	m_good94;

    CLabel	m_fail101;
    CLabel	m_fail102;
    CLabel	m_fail103;
    CLabel	m_fail104;
    CLabel	m_good101;
    CLabel	m_good102;
    CLabel	m_good103;
    CLabel	m_good104;

    CLabel	m_lblStatus11;
    CLabel	m_lblStatus12;
    CLabel	m_lblStatus13;
    CLabel	m_lblStatus14;

    CLabel	m_lblStatus21;
    CLabel	m_lblStatus22;
    CLabel	m_lblStatus23;
    CLabel	m_lblStatus24;

    CLabel	m_lblStatus31;
    CLabel	m_lblStatus32;
    CLabel	m_lblStatus33;
    CLabel	m_lblStatus34;

    CLabel	m_lblStatus41;
    CLabel	m_lblStatus42;
    CLabel	m_lblStatus43;
    CLabel	m_lblStatus44;

    CLabel	m_lblStatus51;
    CLabel	m_lblStatus52;
    CLabel	m_lblStatus53;
    CLabel	m_lblStatus54;

    CLabel	m_lblStatus61;
    CLabel	m_lblStatus62;
    CLabel	m_lblStatus63;
    CLabel	m_lblStatus64;

    CLabel	m_lblStatus71;
    CLabel	m_lblStatus72;
    CLabel	m_lblStatus73;
    CLabel	m_lblStatus74;

    CLabel	m_lblStatus81;
    CLabel	m_lblStatus82;
    CLabel	m_lblStatus83;
    CLabel	m_lblStatus84;

    CLabel	m_lblStatus91;
    CLabel	m_lblStatus92;
    CLabel	m_lblStatus93;
    CLabel	m_lblStatus94;

    CLabel	m_lblStatus101;
    CLabel	m_lblStatus102;
    CLabel	m_lblStatus103;
    CLabel	m_lblStatus104;

    CLabel	m_ctrlGood;//OK数量
    CLabel	m_ctrlFail;//NG数量
    CLabel	m_ctrlYield;//良率
    CLabel	m_ctrlUsedTime;//运行时间
    CLabel m_ctrlProjectName;
    CLabel m_ctrlCheckSum;
    CLabel m_ctrlChipModel;
    CLabel m_ctrlChipBrand;
    CLabel m_ctrlSysNG;
    CLabel m_ctrlStartTime;
    CLabel m_ctrlEndTime;

    CLabel	m_lblGood;
    CLabel	m_lblFail;
    CLabel	m_lblYield;
    CLabel	m_lblUsedTime;
    CLabel m_lblProjectName;
    CLabel m_lblCheckSum;
    CLabel m_lblChipModel;
    CLabel m_lblChipBrand;
    CLabel m_lblProgNG;
    CLabel m_lblStartTime;
    CLabel m_lblEndTime;
    CLabel m_lblUPH;
    CLabel m_lblWorkNumSum;


    CLabel	m_lblAdmin;

    CLabel	m_ctrlStatus;//状态栏
    CRoundButton2	m_btnSuspend;
    CRoundButton2	m_btnTray;
    CRoundButton2	m_btnRun;
    CRoundButton2	m_btnPos;
    CRoundButton2	m_btnFinish;
    CRoundButton2	m_btnExit;
    CXPGroupBox	m_gbM4;
    CXPGroupBox	m_gbM3;
    CXPGroupBox	m_gbM2;
    CXPGroupBox	m_gbM1;
    CXPGroupBox	m_gbM9;
    CXPGroupBox	m_gbM10;
    CXPGroupBox	m_gbM8;
    CXPGroupBox	m_gbM7;
    CXPGroupBox	m_gbM6;
    CXPGroupBox	m_gbM5;
    CXPGroupBox m_gbM;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAutoFPDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;
    CBitmap		m_bmpBg;
    CRect m_rt;//tray盘框
    int m_xn;//tray盘规格，x
    int m_yn;//tray盘规格，y
    // Generated message map functions
    //{{AFX_MSG(CAutoFPDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnTimer(UINT nIDEvent);
    //	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
    afx_msg void OnSocketCheck1();
    afx_msg void OnSocketCheck2();
    afx_msg void OnSocketCheck3();
    afx_msg void OnSocketCheck4();
    afx_msg LRESULT OnCheckStatus(WPARAM wParam, LPARAM lParam);
    //	afx_msg LRESULT OnBurnAgain(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSocketCheck5();
    afx_msg void OnSocketCheck6();
    afx_msg void OnSocketCheck7();
    afx_msg void OnSocketCheck8();
    afx_msg void OnClose();

    afx_msg void OnSocketCheck9();
    afx_msg void OnSocketCheck10();

    afx_msg LRESULT OnUpdateConnection(WPARAM wParam, LPARAM lParam);

    afx_msg void OnCheckAllselect();
    afx_msg void OnOpenProgrammer();
    afx_msg void OnMenuLoad();
    afx_msg void OnMenuSave();
    //	afx_msg void OnMainGetImage();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    static BOOL CALLBACK ChildWndProc(HWND hwnd, LPARAM lParam);
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);


    //////////////////////////CCD////////////////////////
public:
    INT	CGxGetImageDlg(CWnd* pParent = NULL);	// standard constructor

    /// 注册显示窗口
    GX_STATUS RegisterShowWnd(void* pWnd);

    /// 注册打印窗口
    GX_STATUS RegisterLogWnd(void* pWnd);

    /// 注册时间窗口
    GX_STATUS RegisterTimeWnd(void* pWnd);

    /// 打开相机
    GX_STATUS OpenDevice();

    /// 关闭相机
    GX_STATUS CloseDevice();

    /// 初始化设备的默认参数
    GX_STATUS InitDevice();

    /// 获取设备的宽高等属性值
    GX_STATUS GetDeviceInitParam();

    /// 设置相机的数据格式为8bit
    GX_STATUS SetPixelFormat8bit();

    ///  为彩色图像显示准备资源
    bool PrepareForShowColorImg();

    /// 为黑白图像显示准备资源
    bool PrepareForShowMonoImg();

    /// 为图像显示准备资源,分配Buffer
    bool PrepareForShowImg();

    /// 获取图像
    void  ShowImage(GX_FRAME_DATA* pFrameData);

    /// 显示图像
    void DrawImg(void *pImageBuf, int64_t nImageWidth, int64_t nImageHeight);

    /// 释放资源
    void UnPrepareForShowImg();

    /// 刷新UI
    void UpdateUI();

    /// 错误信息提示
    void ShowErrorString(GX_STATUS emErrorStatus);

    /// 更新界面的统计数据
    void UpdateStatisticalData(double dData);

    //----------------------------------------------------------------------------------
    //void Close();

    Point ComputeTest();
    BOOL ComputeOffset();
public:
    GX_DEV_HANDLE  m_hDevice;           ///< 设备句柄
    bool           m_bIsOpen;           ///< 设备打开状态
    bool           m_bIsSnap;           ///< 发送开采命令标识
    BITMAPINFO    *m_pBmpInfo;          ///< 用来显示图像的结构指针
    char           m_chBmpBuf[2048];    ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
    int64_t        m_nImageWidth;       ///< 原始图像宽
    int64_t        m_nImageHeight;      ///< 原始图像高
    int64_t        m_nPixelColorFilter; ///< Bayer格式
    int64_t        m_nPayLoadSize;      ///< 原始Raw图像大小
    bool           m_bIsColorFilter;    ///< 是否支持输出彩色图像
    BYTE          *m_pImgBuffer;        ///< 指向经过处理后的图像数据缓冲区
    GX_FRAME_DATA  m_stFrameData;       ///< GetImage获取的图像地址	


    CWnd           *m_pWnd;             ///< 显示图像窗口(控件)指针
    HDC             m_hDC;              ///< 绘制图像DC句柄

    afx_msg void OnStnDblclickSocketGood1();
    afx_msg void OnStnDblclickSocketGood2();
    afx_msg void OnStnDblclickSocketGood3();
    afx_msg void OnStnDblclickSocketGood4();
    afx_msg void OnStnDblclickSocketGood50();
    afx_msg void OnStnDblclickSocketGood60();
    afx_msg void OnStnDblclickSocketGood70();
    afx_msg void OnStnDblclickSocketGood80();
    CButton m_checkAllSelect;
    void OnAbout();
    //	afx_msg void OnStnDblclickMainGood();
    afx_msg void On32772();
    afx_msg void On32773();
    CProgressCtrl m_progressSocket1;
protected:
    afx_msg LRESULT OnRecvdata(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnBnClickedBtnSendtest();
    // 主界面Log窗口
    CEdit m_editLog;
    afx_msg void OnP800Open();
    P800IspDlg *m_dlgP800;
    CExcelDataDlg m_dlgExcel;

    afx_msg void OnP800Hide();
    afx_msg void OnP800Close();
    afx_msg void OnBnClickedBtnPos();
    afx_msg void OnBnClickedBtnSetting();
    afx_msg void OnBnClickedBtnRun();
    afx_msg void OnBnClickedBtnSuspend();
    afx_msg void OnBnClickedBtnFinish();
    afx_msg void OnBnClickedBtnExit();
    void LoadConfig();
    void RefreshMainData();
    void LoadPosition();
    void SavePosition();
    void SaveConfig();
    afx_msg void OnBnClickedSocketCheck11();

public:
    void InitStatusCheck(BOOL bFig);
    void RefreshSocketStatus();
    void AppendLogMessage(CString strMsg);
    afx_msg void OnBnClickedSocketCheck12();
    afx_msg void OnBnClickedSocketCheck13();
    afx_msg void OnBnClickedSocketCheck14();
    afx_msg void OnBnClickedSocketCheck21();
    afx_msg void OnBnClickedSocketCheck22();
    afx_msg void OnBnClickedSocketCheck23();
    afx_msg void OnBnClickedSocketCheck24();
    afx_msg void OnBnClickedSocketCheck31();
    afx_msg void OnBnClickedSocketCheck32();
    afx_msg void OnBnClickedSocketCheck33();
    afx_msg void OnBnClickedSocketCheck34();
    afx_msg void OnBnClickedSocketCheck41();
    afx_msg void OnBnClickedSocketCheck42();
    afx_msg void OnBnClickedSocketCheck43();
    afx_msg void OnBnClickedSocketCheck44();
    afx_msg void OnBnClickedSocketCheck51();
    afx_msg void OnBnClickedSocketCheck52();
    afx_msg void OnBnClickedSocketCheck53();
    afx_msg void OnBnClickedSocketCheck54();
    afx_msg void OnBnClickedSocketCheck61();
    afx_msg void OnBnClickedSocketCheck62();
    afx_msg void OnBnClickedSocketCheck63();
    afx_msg void OnBnClickedSocketCheck64();
    afx_msg void OnBnClickedSocketCheck71();
    afx_msg void OnBnClickedSocketCheck72();
    afx_msg void OnBnClickedSocketCheck73();
    afx_msg void OnBnClickedSocketCheck74();
    afx_msg void OnBnClickedSocketCheck81();
    afx_msg void OnBnClickedSocketCheck82();
    afx_msg void OnBnClickedSocketCheck83();
    afx_msg void OnBnClickedSocketCheck84();
    afx_msg void OnBnClickedSocketCheck91();
    afx_msg void OnBnClickedSocketCheck92();
    afx_msg void OnBnClickedSocketCheck93();
    afx_msg void OnBnClickedSocketCheck94();
    afx_msg void OnBnClickedSocketCheck101();
    afx_msg void OnBnClickedSocketCheck102();
    afx_msg void OnBnClickedSocketCheck103();
    afx_msg void OnBnClickedSocketCheck104();
    afx_msg void OnBnClickedBtnLock();
    void CleanData();
    afx_msg void OnBnClickedBtnCleanData();
    BOOL CleanSocket();
    afx_msg void OnBnClickedBtnCleanSocket();
    afx_msg void OnBnClickedBtnExcelbox();
    afx_msg void OnBnClickedBtnCreateLogfile();
    void CreateLogfile();
    afx_msg void OnBnClickedCheckProgram2();
    afx_msg void OnBnClickedCheckProgram1();
    afx_msg void OnBnClickedCheckProgram3();
    afx_msg void OnBnClickedCheckProgram4();
    afx_msg void OnBnClickedCheckProgram5();
    afx_msg void OnBnClickedCheckProgram6();
    afx_msg void OnBnClickedCheckProgram7();
    afx_msg void OnBnClickedCheckProgram8();
    afx_msg void OnBnClickedCheckProgram9();
    afx_msg void OnBnClickedCheckProgram10();
    // COMI数据卡	// COMI数据卡
    afx_msg void OnBnClickedBtnGohome();
    afx_msg void OnBnClickedBtnResetStocktray();
    afx_msg void OnBnClickedBtnFpNg();
    afx_msg void OnBnClickedCheckUplight();
    afx_msg
        void EnumSerials();
    void OnBnClickedBtnReadCom();
    CComboBox m_cmbSocketPort;
    afx_msg void OnBnClickedBtnTrayComplete();
    afx_msg void OnBnClickedBtnReadInifile();
    afx_msg void OnStnDblclickSocketGood91();
    afx_msg void OnStnDblclickSocketGood101();
    afx_msg void OnBnClickedCheckDownlight();
    afx_msg void OnBnClickedBtnTapecameraCreate();
    afx_msg void OnBnClickedBtnTapecamearCheck();
};
UINT CheckThreadProc(LPVOID lPara);
UINT CheckProgrammerProc(LPVOID lPara);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOFPDLG_H__9F881EEB_7F52_4E66_AF60_E11C170BE538__INCLUDED_)
