// ExcelDataDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "sqlite3.h"
#include "AutoFP.h"
#include "ExcelDataDlg.h"
#include "afxdialogex.h"
#include "BasicExcel.hpp"
using  namespace YExcel;

#pragma comment(lib,"sqlite3")

// CExcelDataDlg 对话框

IMPLEMENT_DYNAMIC(CExcelDataDlg, CDialogEx)

CExcelDataDlg::CExcelDataDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DATAEXCEL, pParent)
{

}

CExcelDataDlg::~CExcelDataDlg()
{
    sqlite3_close(pdb);
}

void CExcelDataDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LISTDATA, m_list);
    DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_datatimeStart);
}


BEGIN_MESSAGE_MAP(CExcelDataDlg, CDialogEx)
    ON_BN_CLICKED(IDC_READ, &CExcelDataDlg::OnBnClickedRead)
    ON_BN_CLICKED(IDC_OUTPUT_TO_EXCEL, &CExcelDataDlg::OnBnClickedOutputToExcel)
    ON_BN_CLICKED(IDC_INSERT, &CExcelDataDlg::OnBnClickedInsert)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BTN_FILTER, &CExcelDataDlg::OnBnClickedBtnFilter)
    ON_BN_CLICKED(IDC_BTN_ALL_DISPLAY, &CExcelDataDlg::OnBnClickedBtnAllDisplay)
    ON_BN_CLICKED(IDC_BTN_DELETE_ITEM, &CExcelDataDlg::OnBnClickedBtnDeleteItem)
END_MESSAGE_MAP()


// CExcelDataDlg 消息处理程序


BOOL CExcelDataDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    DWORD dwStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
    dwStyle &= ~(LVS_TYPEMASK);
    dwStyle &= ~(LVS_EDITLABELS);
    SetWindowLong(m_list.m_hWnd, GWL_STYLE,
        dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS);
    DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
    m_list.SetExtendedStyle(styles);
    m_list.InsertColumn(0, _T("Customer"), LVCFMT_CENTER, 70);
    m_list.InsertColumn(1, _T("WorkNo."), LVCFMT_CENTER, 70);
    m_list.InsertColumn(2, _T("LotNum"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(3, _T("ICBrand"), LVCFMT_CENTER, 80);
    m_list.InsertColumn(4, _T("ICModel"), LVCFMT_CENTER, 80);
    m_list.InsertColumn(5, _T("Pass"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(6, _T("Yield"), LVCFMT_CENTER, 60);
    m_list.InsertColumn(7, _T("ProgNG"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(8, _T("SysNG"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(9, _T("FailRate"), LVCFMT_CENTER, 60);
    m_list.InsertColumn(10, _T("UPH"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(11, _T("Input"), LVCFMT_CENTER, 50);
    m_list.InsertColumn(12, _T("RunTime"), LVCFMT_CENTER, 100);
    m_list.InsertColumn(13, _T("StartTime"), LVCFMT_CENTER, 100);
    m_list.InsertColumn(14, _T("EndTime"), LVCFMT_CENTER, 100);
    m_list.InsertColumn(15, _T("CheckSum"), LVCFMT_CENTER, 100);
    m_list.InsertColumn(16, _T("Operator"), LVCFMT_CENTER, 60);
    m_list.InsertColumn(17, _T("Date"), LVCFMT_CENTER, 60);
    ReadData();
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CExcelDataDlg::ReadData()
{
#ifdef _UNICODE
    _tcscpy_s(dbName, _MAX_PATH, "c:\\S100\\bin\\datareport.db");
#else
    MultiByteToWideChar(CP_ACP, 0, "c:\\S100\\bin\\datareport.db", _tcslen("c:\\S100\\bin\\datareport.db") + 1,
        dbName, _MAX_PATH);
#endif //_UNICODE
    int res = sqlite3_open16(dbName, &pdb);

    if (res != SQLITE_OK)
    {
        MessageBox(_T("不能打开项目，请检查!"), _T("注意"), MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        m_list.DeleteAllItems();
        OpenData(pdb);
    }
}
void CExcelDataDlg::OnBnClickedRead()
{
    TCHAR drive[_MAX_DRIVE] = _T("");
    TCHAR dir[_MAX_DIR] = _T("");
    TCHAR fname[_MAX_FNAME] = _T("");
    TCHAR ext[_MAX_EXT] = _T("");

    TCHAR lpstrFilename[_MAX_PATH] = _T("");
    TCHAR lpstrPathName[_MAX_PATH] = _T("");

    //WCHAR dbName[_MAX_PATH] = L"";
    //sqlite3* pdb = NULL;
    CFileDialog dlgFileOpen(TRUE);
    int structsize = 0;

    DWORD dwVersion = GetVersion();
    if (dwVersion < 0x80000000)
        structsize = 88;
    else
        structsize = 76;
    dlgFileOpen.m_ofn.lStructSize = structsize;
    dlgFileOpen.m_ofn.lpstrFile = lpstrFilename;
    dlgFileOpen.m_ofn.lpstrFilter = _T("项目文件:*.db\0*.db\0所有文件:*.*\0*.*\0\0");
    dlgFileOpen.m_ofn.Flags |= OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (dlgFileOpen.DoModal() == IDOK)
    {
        _tsplitpath_s(dlgFileOpen.m_ofn.lpstrFile, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
        _stprintf_s(lpstrPathName, _MAX_PATH, _T("%s%s"), drive, dir);
        SetCurrentDirectory(lpstrPathName);
        lpstrPathName[_tcslen(lpstrPathName) - 1] = _T('\0');
        _tcscat_s(lpstrPathName, _MAX_PATH, _T(".db"));
    }
    else
        return;

    AfxGetApp()->LoadStandardCursor(IDC_WAIT);

#ifdef _UNICODE
    _tcscpy_s(dbName, _MAX_PATH, dlgFileOpen.m_ofn.lpstrFile);
#else
    MultiByteToWideChar(CP_ACP, 0, dlgFileOpen.m_ofn.lpstrFile, _tcslen(dlgFileOpen.m_ofn.lpstrFile) + 1,
        dbName, _MAX_PATH);
#endif //_UNICODE
    int res = sqlite3_open16(dbName, &pdb);

    if (res != SQLITE_OK)
    {
        MessageBox(_T("不能打开项目，请检查!"), _T("注意"), MB_OK | MB_ICONEXCLAMATION);
    }
    else
    {
        m_list.DeleteAllItems();
        OpenData(pdb);
    }
}

BOOL CExcelDataDlg::OpenData(sqlite3 * pdb)
{
    char sql[_MAX_SQL];
    TCHAR fn[_MAX_PATH];
    char *errMsg;
    int res;
    char **dbResult;
    int nRow, nColumn;
    char* stopStr;

    sprintf_s(sql, _MAX_SQL, "select \
Customer, WorkNo, LotNum, ICBrand, ICModel, Pass, Yield, ProgNG, SysNG, FailRate, UPH, Input, RunTime, StartTime, EndTime, CheckSum, Operator,Date \
from FPReport \
order by EndTime");
    res = sqlite3_get_table(pdb, sql, &dbResult, &nRow, &nColumn, &errMsg);
    if (SQLITE_OK == res)
    {
        int index = nColumn;
        for (int i = 0; i < nRow; i++)
        {
            try
            {
                sData *pSdata = new sData();
                //TCHAR *pname = dbResult[index + 0];
                //double x = strtod(dbResult[index + 1], &stopStr);
                //double y = strtod(dbResult[index + 2], &stopStr);
                //double angle = strtod(dbResult[index + 3], &stopStr);

                //TCHAR *pCustomer = dbResult[index + 0];
                //TCHAR *pWorkNo = dbResult[index + 1];
                //int nLotNum = atoi(dbResult[index + 2]);
                //TCHAR *pICBrand = dbResult[index + 3];
                //TCHAR *pICModel = dbResult[index + 4];
                //int nPass = atoi(dbResult[index + 5]);
                //double dYield = strtod(dbResult[index + 6], &stopStr);
                //int nProgNG = atoi(dbResult[index + 7]);
                //int nSysNG = atoi(dbResult[index + 8]);
                //double dFailRate = strtod(dbResult[index + 9], &stopStr);
                //int nUPH = atoi(dbResult[index + 10]);
                //int nInput = atoi(dbResult[index + 11]);
                //TCHAR *pRunTime = dbResult[index + 12];
                //TCHAR *pProjDate = dbResult[index + 13];

                pSdata->strCustomer = dbResult[index + 0];
                pSdata->strWorkNo = dbResult[index + 1];
                pSdata->nLotNum = atoi(dbResult[index + 2]);
                pSdata->strICBrand = dbResult[index + 3];
                pSdata->strICModel = dbResult[index + 4];
                pSdata->nPass = atoi(dbResult[index + 5]);
                pSdata->fYield = strtod(dbResult[index + 6], &stopStr);
                pSdata->nProgNG = atoi(dbResult[index + 7]);
                pSdata->nSysNG = atoi(dbResult[index + 8]);
                pSdata->fFailRate = strtod(dbResult[index + 9], &stopStr);
                pSdata->nUPH = atoi(dbResult[index + 10]);
                pSdata->nInput = atoi(dbResult[index + 11]);
                pSdata->strRunTime = dbResult[index + 12];
                pSdata->strStartTime = dbResult[index + 13];
                pSdata->strEndTime = dbResult[index + 14];
                pSdata->strCheckSum = dbResult[index + 15];
                pSdata->strOperator = dbResult[index + 16];
                pSdata->strDate = dbResult[index + 17];

                //pSdata->pchar = pname;
                //pSdata->x = x;
                //pSdata->y = y;
                //pSdata->angle = angle;
                m_sData.push_back(pSdata);

                AddOutRes(pSdata);
                //AddOutRes(pname, x, y, angle);
            }
            catch (CException &e)
            {
                e.ReportError();

            }
            catch (...)
            {
                AfxMessageBox("出错", MB_OK | MB_ICONEXCLAMATION);

            }
            index += nColumn;
        }
    }
    sqlite3_free_table(dbResult);
    return TRUE;
}
void CExcelDataDlg::InSertFPData()
{
    if (pdb != NULL)
    {
        if (1/*gm_nTotalGoodSize+gm_nTotalFailSize==gm_nSum*/)//烧录完成时才插入数据
        {
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
            strTemp.Format("%02d/%02d/%02d %02d:%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, t->tm_hour, t->tm_min, t->tm_sec);
            gm_sdInfo.strEndTime = strTemp;

            strTemp.Format("%02d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
            gm_sdInfo.strDate = strTemp;

            gm_sdInfo.nPass = gm_nTotalGoodSize;
            gm_sdInfo.nProgNG = gm_nTotalFPFail;
            gm_sdInfo.nInput = gm_nTotalGoodSize + gm_nTotalFailSize;
            gm_sdInfo.nSysNG = gm_nTotalCheckFail;
            if (gm_nTotalGoodSize + gm_nTotalFailSize == 0)
            {
                gm_sdInfo.fYield = 0;
                gm_sdInfo.fFailRate = 0;
            }
            else
            {
                gm_sdInfo.fYield = gm_nTotalGoodSize + gm_nTotalFailSize == 0 ? 0 : (float)gm_nTotalGoodSize / (float)(gm_nTotalGoodSize + gm_nTotalFailSize);
                gm_sdInfo.fFailRate = 1 - gm_sdInfo.fYield;
            }
            //sData sd{ "sinaen","no1",100,"abc","fff",100,1,0,0,0,1000,100,"12:20","12010" };
            InsertData(&gm_sdInfo);
        }
        //sqlite3_close(pdb);
    }
}

void CExcelDataDlg::AddOutRes(sData *sd)
{
    TCHAR szVal[_MAX_PATH];
    int nIndex = m_list.GetItemCount();

    LV_ITEM   lvitemAdd = { 0 };
    lvitemAdd.mask = LVIF_TEXT;
    lvitemAdd.iItem = nIndex;
    lvitemAdd.iSubItem = 0;
    lvitemAdd.pszText = sd->strCustomer.GetBuffer();
    m_list.InsertItem(&lvitemAdd);


    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strWorkNo);
    lvitemAdd.iSubItem = 1;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nLotNum);
    lvitemAdd.iSubItem = 2;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strICBrand);
    lvitemAdd.iSubItem = 3;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strICModel);
    lvitemAdd.iSubItem = 4;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nPass);
    lvitemAdd.iSubItem = 5;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%.2f%%"), sd->fYield * 100);
    lvitemAdd.iSubItem = 6;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nProgNG);
    lvitemAdd.iSubItem = 7;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nSysNG);
    lvitemAdd.iSubItem = 8;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%.2f%%"), sd->fFailRate * 100);
    lvitemAdd.iSubItem = 9;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nUPH);
    lvitemAdd.iSubItem = 10;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%d"), sd->nInput);
    lvitemAdd.iSubItem = 11;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strRunTime);
    lvitemAdd.iSubItem = 12;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strStartTime);
    lvitemAdd.iSubItem = 13;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strEndTime);
    lvitemAdd.iSubItem = 14;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strCheckSum);
    lvitemAdd.iSubItem = 15;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strOperator);
    lvitemAdd.iSubItem = 16;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%s"), sd->strDate);
    lvitemAdd.iSubItem = 17;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);


    //_stprintf_s(szVal, _MAX_PATH, _T("%.2f"), y);
    //lvitemAdd.iSubItem = 2;
    //lvitemAdd.pszText = szVal;
    //m_list.SetItem(&lvitemAdd);


    //_stprintf_s(szVal, _MAX_PATH, _T("%.2f"), tstThres);
    //lvitemAdd.iSubItem = 3;
    //lvitemAdd.pszText = szVal;
    //m_list.SetItem(&lvitemAdd);
}


void CExcelDataDlg::AddOutRes(TCHAR * szPosName, float x, float y, float tstThres)
{
    TCHAR szVal[_MAX_PATH];
    int nIndex = m_list.GetItemCount();

    LV_ITEM   lvitemAdd = { 0 };
    lvitemAdd.mask = LVIF_TEXT;
    lvitemAdd.iItem = nIndex;
    lvitemAdd.iSubItem = 0;
    lvitemAdd.pszText = szPosName;
    m_list.InsertItem(&lvitemAdd);


    _stprintf_s(szVal, _MAX_PATH, _T("%.2f"), x);
    lvitemAdd.iSubItem = 1;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);

    _stprintf_s(szVal, _MAX_PATH, _T("%.2f"), y);
    lvitemAdd.iSubItem = 2;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);


    _stprintf_s(szVal, _MAX_PATH, _T("%.2f"), tstThres);
    lvitemAdd.iSubItem = 3;
    lvitemAdd.pszText = szVal;
    m_list.SetItem(&lvitemAdd);
}

bool CExcelDataDlg::GetExcelHead(CListCtrl * plist)
{
    int nRow = plist->GetItemCount();
    if (nRow > 0)
    {
        LVCOLUMN lvcol;
        char str[256];
        int nColNum;
        //CString strColumnName[4];//假如有4列
        nColNum = 0;
        lvcol.mask = LVCF_TEXT;
        lvcol.pszText = str;
        lvcol.cchTextMax = 256;
        while (plist->GetColumn(nColNum, &lvcol))
        {
            strColumnName[nColNum] = lvcol.pszText;
            nColNum++;
        }
        return true;
    }

    return false;
}


void CExcelDataDlg::OnBnClickedOutputToExcel()
{
    if (GetExcelHead(&m_list))
    {
        int datatype = 0;
        BasicExcel pBasicExcel;
        pBasicExcel.New(1);
        pBasicExcel.RenameWorksheet("Sheet1", "Data1");
        BasicExcelWorksheet* sheet = pBasicExcel.GetWorksheet("Data1");
        BasicExcelCell* cell;
        if (sheet)
        {
            int nColNum = m_list.GetHeaderCtrl()->GetItemCount();
            int nRow = m_sData.size();
            for (int i = 0; i < nColNum; i++)
            {
                sheet->Cell(0, i)->SetWString(strColumnName[i].AllocSysString());
            }

            sheet->Cell(0, 0)->SetString("Customer");
            sheet->Cell(0, 1)->SetString("WorkNo");
            sheet->Cell(0, 2)->SetString("LotNum");
            sheet->Cell(0, 3)->SetString("ICBrand");
            sheet->Cell(0, 4)->SetString("ICModel");
            sheet->Cell(0, 5)->SetString("Pass");
            sheet->Cell(0, 6)->SetString("Yield");
            sheet->Cell(0, 7)->SetString("ProgNG");
            sheet->Cell(0, 8)->SetString("SysNG");
            sheet->Cell(0, 9)->SetString("FailRate");
            sheet->Cell(0, 10)->SetString("UPH");
            sheet->Cell(0, 11)->SetString("Input");
            sheet->Cell(0, 12)->SetString("RunTime");
            sheet->Cell(0, 13)->SetString("StartTime");
            sheet->Cell(0, 14)->SetString("EndTime");
            sheet->Cell(0, 15)->SetString("CheckSum");
            sheet->Cell(0, 16)->SetString("Operator");
            sheet->Cell(0, 17)->SetString("Date");

            for (int j = 1; j <= nRow; j++)
            {
                //for (int k = 0; k < nColNum; k++)
                //{
                //	switch (datatype)
                //	{
                //	case MYINT:
                //		sheet->Cell(j, k)->SetInteger(0);
                //		break;
                //	case MYFLOAT:
                //	case MYDOUBLE:
                //		sheet->Cell(j, k)->SetDouble(0);
                //		break;
                //	case MYSTRING:
                //		sheet->Cell(j, k)->SetString("");
                //		break;
                //	default:
                //		break;
                //	}
                //}
                //sheet->Cell(j, 0)->SetWString(m_sData.at(j - 1)->pchar.AllocSysString());
                //sheet->Cell(j, 1)->SetDouble(m_sData.at(j - 1)->x);
                //sheet->Cell(j, 2)->SetDouble(m_sData.at(j - 1)->y);
                //sheet->Cell(j, 3)->SetDouble(m_sData.at(j - 1)->angle);

                sheet->Cell(j, 0)->SetString(m_sData.at(j - 1)->strCustomer);
                sheet->Cell(j, 1)->SetString(m_sData.at(j - 1)->strWorkNo);
                sheet->Cell(j, 2)->SetDouble(m_sData.at(j - 1)->nLotNum);
                sheet->Cell(j, 3)->SetString(m_sData.at(j - 1)->strICBrand);
                sheet->Cell(j, 4)->SetString(m_sData.at(j - 1)->strICModel);
                sheet->Cell(j, 5)->SetDouble(m_sData.at(j - 1)->nPass);
                sheet->Cell(j, 6)->SetDouble(m_sData.at(j - 1)->fYield);
                sheet->Cell(j, 7)->SetDouble(m_sData.at(j - 1)->nProgNG);
                sheet->Cell(j, 8)->SetDouble(m_sData.at(j - 1)->nSysNG);
                sheet->Cell(j, 9)->SetDouble(m_sData.at(j - 1)->fFailRate);
                sheet->Cell(j, 10)->SetDouble(m_sData.at(j - 1)->nUPH);
                sheet->Cell(j, 11)->SetDouble(m_sData.at(j - 1)->nInput);
                sheet->Cell(j, 12)->SetString(m_sData.at(j - 1)->strRunTime);
                sheet->Cell(j, 13)->SetString(m_sData.at(j - 1)->strStartTime);
                sheet->Cell(j, 14)->SetString(m_sData.at(j - 1)->strEndTime);
                sheet->Cell(j, 15)->SetString(m_sData.at(j - 1)->strCheckSum);
                sheet->Cell(j, 16)->SetString(m_sData.at(j - 1)->strOperator);
                sheet->Cell(j, 17)->SetString(m_sData.at(j - 1)->strDate);

                //sheet->Cell(j, 0)->SetString(m_sData.at(j)->strCustomer);
                //sheet->Cell(j, 1)->SetString(m_sData.at(j)->strWorkNo);
                //sheet->Cell(j, 2)->SetDouble(m_sData.at(j)->nLotNum);
                //sheet->Cell(j, 3)->SetString(m_sData.at(j)->strICBrand);
                //sheet->Cell(j, 4)->SetString(m_sData.at(j)->strICModel);
                //sheet->Cell(j, 5)->SetDouble(m_sData.at(j)->nPass);
                //sheet->Cell(j, 6)->SetDouble(m_sData.at(j)->fYield);
                //sheet->Cell(j, 7)->SetDouble(m_sData.at(j)->nProgNG);
                //sheet->Cell(j, 8)->SetDouble(m_sData.at(j)->nSysNG);
                //sheet->Cell(j, 9)->SetDouble(m_sData.at(j)->fFailRate);
                //sheet->Cell(j, 10)->SetDouble(m_sData.at(j)->nUPH);
                //sheet->Cell(j, 11)->SetDouble(m_sData.at(j)->nInput);
                //sheet->Cell(j, 12)->SetString(m_sData.at(j)->strRunTime);
                //sheet->Cell(j, 13)->SetString(m_sData.at(j)->strStartTime);
                //sheet->Cell(j, 14)->SetString(m_sData.at(j)->strEndTime);
                //sheet->Cell(j, 15)->SetString(m_sData.at(j)->strCheckSum);
                //sheet->Cell(j, 16)->SetString(m_sData.at(j)->strOperator);
            }
        }

        CTime time;
        time = CTime::GetCurrentTime();
        CString str;
        str.Format("c:\\S100\\Excel\\%d%02d%02d-%02d%02d.xls", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());

        if (pBasicExcel.SaveAs(str))
        {
            AfxMessageBox("导出成功");
        }
    }
}
//update TblCalss set tclassName = 'xxxx', tclassDesc = 'yyyy' where tclassId>105
void CExcelDataDlg::UpdateData(sData *pSdata)
{
    char sql[_MAX_SQL];
    char *errMsg;
    int res;
    try
    {
        sprintf_s(sql, _MAX_SQL, "update FPReport \
set\
Customer='%s',\
LotNum=%d,\
ICBrand='%s',\
ICModel='%s',\
Pass=%d,\
Yield=%f,\
ProgNG=%d,\
SysNG=%d,\
FailRate=%f	,\
UPH	=%d,\
Input=%d,\
RunTime='%s',\
StartTime='%s',\
EndTime='%s',\
CheckSum='%s',\
Operator='%s',\
Date='%s'\
where WorkNo = '%s'",
pSdata->strCustomer,
pSdata->nLotNum,
pSdata->strICBrand,
pSdata->strICModel,
pSdata->nPass,
pSdata->fYield,
pSdata->nProgNG,
pSdata->nSysNG,
pSdata->fFailRate,
pSdata->nUPH,
pSdata->nInput,
pSdata->strRunTime,
pSdata->strStartTime,
pSdata->strEndTime,
pSdata->strCheckSum,
pSdata->strOperator,
pSdata->strDate,
pSdata->strWorkNo);

        res = sqlite3_exec(pdb, sql, NULL, NULL, &errMsg);
        if (res != SQLITE_OK)
        {
            AfxMessageBox((LPCTSTR)errMsg, MB_OK | MB_ICONWARNING);
            return;
        }

        return;
    }
    catch (CException &e)
    {
        e.ReportError();
    }
    return;
}

// 插入数据
int CExcelDataDlg::InsertData(sData *pdata)
{
    //sData pdata;
    sqlite3_exec(pdb, "begin transaction", 0, 0, NULL); //开始一个事务
    insertData(pdb, pdata);
    sqlite3_exec(pdb, "commit transaction", 0, 0, NULL); //提交事务
    //sqlite3_close(pdb);
    return 0;
}
BOOL CExcelDataDlg::insertData(sqlite3 * pdb, sData * pSdata)
{
    char sql[_MAX_SQL];
    char *errMsg;
    int res;
    //TCHAR fname[_MAX_FNAME] = _T("cf");
    //float ffloat1 = 0.0, ffloat2 = 0.0, ffloat3 = 0.0;
    try
    {
        //sprintf_s(sql, _MAX_SQL, "insert into dataReport \
        //														  (posName, x, y, angle) \
        //												  values \
        //												  ('%s', %f, %f, %f)",
        //	pdata->pchar/*.AllocSysString()*/, pdata->x,
        //	pdata->y, pdata->angle);

        sprintf_s(sql, _MAX_SQL, "insert into FPReport \
	(Customer, WorkNo, LotNum, ICBrand, ICModel, Pass, Yield, ProgNG, SysNG, FailRate, UPH, Input, RunTime, StartTime, EndTime, CheckSum, Operator, Date) \
	values \
	('%s','%s', %d,'%s','%s', %d, %f,%d, %d,%f, %d, %d,'%s','%s','%s','%s','%s','%s')",
            pSdata->strCustomer,
            pSdata->strWorkNo,
            pSdata->nLotNum,
            pSdata->strICBrand,
            pSdata->strICModel,
            pSdata->nPass,
            pSdata->fYield,
            pSdata->nProgNG,
            pSdata->nSysNG,
            pSdata->fFailRate,
            pSdata->nUPH,
            pSdata->nInput,
            pSdata->strRunTime,
            pSdata->strStartTime,
            pSdata->strEndTime,
            pSdata->strCheckSum,
            pSdata->strOperator,
            pSdata->strDate);

        res = sqlite3_exec(pdb, sql, NULL, NULL, &errMsg);
        if (res != SQLITE_OK)
        {
            AfxMessageBox((LPCTSTR)errMsg, MB_OK | MB_ICONWARNING);
            return FALSE;
        }

        return TRUE;
    }
    catch (CException &e)
    {
        e.ReportError();
    }
    return FALSE;
}

void CExcelDataDlg::OnBnClickedInsert()
{
    if (pdb == NULL)
    {
        AfxMessageBox("请先打开需要插入的报表。");
        return;
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
    CString strTemp;
    strTemp.Format("%02d/%02d/%02d %02d:%02d:%02d", t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, t->tm_hour, t->tm_min, t->tm_sec);
    gm_sdInfo.strEndTime = strTemp;

    strTemp.Format("%02d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    gm_sdInfo.strDate = strTemp;

    gm_sdInfo.nPass = gm_nTotalGoodSize;
    gm_sdInfo.nProgNG = gm_nTotalFailSize;
    gm_sdInfo.nInput = gm_nTotalGoodSize + gm_nTotalFailSize;
    if (gm_nTotalGoodSize + gm_nTotalFailSize == 0)
    {
        gm_sdInfo.fYield = 0;
        gm_sdInfo.fFailRate = 0;
    }
    else
    {
        gm_sdInfo.fYield = (float)gm_nTotalGoodSize / (float)(gm_nTotalGoodSize + gm_nTotalFailSize);
        gm_sdInfo.fFailRate = 1 - gm_sdInfo.fYield;
    }
    //sData sd{ "sinaen","no1",100,"abc","fff",100,1,0,0,0,1000,100,"12:20","12010" };
    InsertData(&gm_sdInfo);
    m_list.DeleteAllItems();
    OpenData(pdb);
}


void CExcelDataDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    this->ShowWindow(SW_HIDE);
    CDialogEx::OnClose();
}


void CExcelDataDlg::OnBnClickedBtnFilter()
{
    // TODO: 在此添加控件通知处理程序代码
    //GetLevelDate(IDC_DATETIMEPICKER_START);
    m_list.DeleteAllItems();

    char sql[_MAX_SQL];
    TCHAR fn[_MAX_PATH];
    char *errMsg;
    int res;
    char **dbResult;
    int nRow, nColumn;
    char* stopStr;

    sprintf_s(sql, _MAX_SQL, "select \
Customer, WorkNo, LotNum, ICBrand, ICModel, Pass, Yield, ProgNG, SysNG, FailRate, UPH, Input, RunTime, StartTime, EndTime, CheckSum, Operator,Date \
from FPReport \
where(Date='%s')\
order by EndTime", GetLevelDate(IDC_DATETIMEPICKER_START));
    res = sqlite3_get_table(pdb, sql, &dbResult, &nRow, &nColumn, &errMsg);
    if (SQLITE_OK == res)
    {
        int index = nColumn;
        for (int i = 0; i < nRow; i++)
        {
            try
            {
                sData *pSdata = new sData();

                pSdata->strCustomer = dbResult[index + 0];
                pSdata->strWorkNo = dbResult[index + 1];
                pSdata->nLotNum = atoi(dbResult[index + 2]);
                pSdata->strICBrand = dbResult[index + 3];
                pSdata->strICModel = dbResult[index + 4];
                pSdata->nPass = atoi(dbResult[index + 5]);
                pSdata->fYield = strtod(dbResult[index + 6], &stopStr);
                pSdata->nProgNG = atoi(dbResult[index + 7]);
                pSdata->nSysNG = atoi(dbResult[index + 8]);
                pSdata->fFailRate = strtod(dbResult[index + 9], &stopStr);
                pSdata->nUPH = atoi(dbResult[index + 10]);
                pSdata->nInput = atoi(dbResult[index + 11]);
                pSdata->strRunTime = dbResult[index + 12];
                pSdata->strStartTime = dbResult[index + 13];
                pSdata->strEndTime = dbResult[index + 14];
                pSdata->strCheckSum = dbResult[index + 15];
                pSdata->strOperator = dbResult[index + 16];
                pSdata->strDate = dbResult[index + 17];

                m_sData.push_back(pSdata);

                AddOutRes(pSdata);
            }
            catch (CException &e)
            {
                e.ReportError();

            }
            catch (...)
            {
                AfxMessageBox("出错", MB_OK | MB_ICONEXCLAMATION);

            }
            index += nColumn;
        }
    }
    sqlite3_free_table(dbResult);
    return;
}
CString CExcelDataDlg::GetLevelDate(int nID)
{
    CDateTimeCtrl * pWndTemp = (CDateTimeCtrl*)GetDlgItem(nID);
    CTime tempTime;
    pWndTemp->GetTime(tempTime);
    CString strTemp1 = tempTime.Format("%Y-%m-%d");
    return strTemp1;
}

void CExcelDataDlg::OnBnClickedBtnAllDisplay()
{
    m_list.DeleteAllItems();

    char sql[_MAX_SQL];
    TCHAR fn[_MAX_PATH];
    char *errMsg;
    int res;
    char **dbResult;
    int nRow, nColumn;
    char* stopStr;

    sprintf_s(sql, _MAX_SQL, "select \
Customer, WorkNo, LotNum, ICBrand, ICModel, Pass, Yield, ProgNG, SysNG, FailRate, UPH, Input, RunTime, StartTime, EndTime, CheckSum, Operator,Date \
from FPReport \
order by EndTime");
    res = sqlite3_get_table(pdb, sql, &dbResult, &nRow, &nColumn, &errMsg);
    if (SQLITE_OK == res)
    {
        int index = nColumn;
        for (int i = 0; i < nRow; i++)
        {
            try
            {
                sData *pSdata = new sData();

                pSdata->strCustomer = dbResult[index + 0];
                pSdata->strWorkNo = dbResult[index + 1];
                pSdata->nLotNum = atoi(dbResult[index + 2]);
                pSdata->strICBrand = dbResult[index + 3];
                pSdata->strICModel = dbResult[index + 4];
                pSdata->nPass = atoi(dbResult[index + 5]);
                pSdata->fYield = strtod(dbResult[index + 6], &stopStr);
                pSdata->nProgNG = atoi(dbResult[index + 7]);
                pSdata->nSysNG = atoi(dbResult[index + 8]);
                pSdata->fFailRate = strtod(dbResult[index + 9], &stopStr);
                pSdata->nUPH = atoi(dbResult[index + 10]);
                pSdata->nInput = atoi(dbResult[index + 11]);
                pSdata->strRunTime = dbResult[index + 12];
                pSdata->strStartTime = dbResult[index + 13];
                pSdata->strEndTime = dbResult[index + 14];
                pSdata->strCheckSum = dbResult[index + 15];
                pSdata->strOperator = dbResult[index + 16];
                pSdata->strDate = dbResult[index + 17];

                m_sData.push_back(pSdata);

                AddOutRes(pSdata);
            }
            catch (CException &e)
            {
                e.ReportError();

            }
            catch (...)
            {
                AfxMessageBox("出错", MB_OK | MB_ICONEXCLAMATION);

            }
            index += nColumn;
        }
    }
    sqlite3_free_table(dbResult);
    return;
}


void CExcelDataDlg::OnBnClickedBtnDeleteItem()
{
    // TODO: 在此添加控件通知处理程序代码
}
